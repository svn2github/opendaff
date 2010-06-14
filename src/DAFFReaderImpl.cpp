/*
 *  OpenDAFF - A free, open-source software package for directional audio data,
 *  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
 *
 *  Copyright (C) Institute of Technical Acoustics, RWTH Aachen University, 2009-2010
 *
 *  Visit the OpenDAFF homepage: http://www.opendaff.org
 *
 *
 *  --= License & warranty =--
 *
 *  OpenDAFF is free software, distributed under the terms of the
 *  GNU Lesser General Public License (LGPL) version 3.
 *  You can redistribute it and/or modify it under the terms of the
 *  GNU Lesser General Public License (LGPL) version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  --= File information =--
 *
 *  File:		DAFFReaderImpl.cpp
 *  Purpose:	Reader implementation
 *  Version:    $Id: DAFFReaderImpl.cpp,v 1.35 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de),
 *              Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)
 *
 */

#include <DAFFReaderImpl.h>

// #define WITH_PROFILING

#include <DAFFContent.h>
#include <DAFFContentIR.h>
#include <DAFFContentMS.h>
#include <DAFFDefs.h>
#include <DAFFHeader.h>
#include <DAFFMetadataImpl.h>
#include <DAFFUtils.h>
#include <Utils.h>

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sstream>

DAFFReaderImpl::DAFFReaderImpl()
: m_bFileOpened(false),
  m_file(NULL),
  m_pFileBlockTable(NULL),
  m_pMainHeader(NULL),
  m_pContentHeader(NULL),
  m_pRecordDescBlock(NULL),
  m_pDataBlock(NULL),
  m_pMetadata(NULL)
{}

DAFFReaderImpl::~DAFFReaderImpl() {
	if (isFileOpened()) closeFile();
}

bool DAFFReaderImpl::isFileOpened() const {
	return m_bFileOpened;
}

int DAFFReaderImpl::openFile(const std::string& sFilename) {

	if (m_bFileOpened) return DAFF_MODAL_ERROR;

	/*
	 *  1st step: Load the file header and validate it for correctness
	 */

	m_file = fopen(sFilename.c_str(), "rb");
	if (!m_file) return DAFF_FILE_NOT_FOUND;

	if (fread(&m_fileHeader, 1, DAFF_FILE_HEADER_SIZE, m_file) != DAFF_FILE_HEADER_SIZE) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_fileHeader.fixEndianness();

	// Check signature
	if (! ((m_fileHeader.pcSignature[0] == 'F') && (m_fileHeader.pcSignature[1] == 'W'))) {
		// File is not an OpenDAFF database.
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	// Check version
	// [This implementation supports version number 0.1]
	if (m_fileHeader.iFileFormatVersion != 100) {
		// Database version not supported
		tidyup();
		return DAFF_FILE_FORMAT_VERSION_UNSUPPORTED;
	}

	// Check file block entries
	if (m_fileHeader.iNumFileBlocks <= 0) {
		// There must be at least one block
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	// Read the file block table
	size_t iFileBlockTableSize = m_fileHeader.iNumFileBlocks * DAFF_FILE_BLOCK_ENTRY_SIZE;
	m_pFileBlockTable = (DAFFFileBlockEntry*) DAFF::malloc_aligned16(iFileBlockTableSize);
	if (fread(m_pFileBlockTable, 1, iFileBlockTableSize, m_file) != iFileBlockTableSize) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	for (int i=0; i<m_fileHeader.iNumFileBlocks; i++)
		m_pFileBlockTable[i].fixEndianness();

	/*  DEBUG:
	for (int i=0; i<m_fileHeader.iNumFileBlocks; i++)
		printf("FILE BLOCK[%d] = { ID = 0x%04X, offset = %I64d, size = %I64d bytes }\n",
		       i, m_pFileBlockTable[i].iID, m_pFileBlockTable[i].ui64Offset, m_pFileBlockTable[i].ui64Size);
	*/

	// Check for correctness
	for (int i=0; i<m_fileHeader.iNumFileBlocks; i++) {
		// Note: IDs are not checked
		if ((m_pFileBlockTable[i].ui64Offset < (DAFF_FILE_HEADER_SIZE + iFileBlockTableSize)) ||
			(m_pFileBlockTable[i].ui64Size <= 0)) {
			tidyup();
			return DAFF_FILE_CORRUPTED;
		}
	}

	/*
	 *  2nd step: Load the main header, validate it for correctness
	 */

	// Search for the main header block
	DAFFFileBlockEntry* pfbMainHeader;
	if (getFirstFileBlockByID(FILEBLOCK_DAFF1_MAIN_HEADER, pfbMainHeader) != 1) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pMainHeader = (DAFFMainHeader*) DAFF::malloc_aligned16(DAFF_MAIN_HEADER_SIZE);
	if (fread(m_pMainHeader, 1, DAFF_MAIN_HEADER_SIZE, m_file) != DAFF_MAIN_HEADER_SIZE) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pMainHeader->fixEndianness();

	// Content type
	switch (m_pMainHeader->iContentType) {
	case DAFF_IMPULSE_RESPONSE:
	case DAFF_MAGNITUDE_SPECTRUM:
		break;

	default:
		// Invalid content type
		tidyup();
		return DAFF_FILE_CORRUPTED;
	};

	// Quantization
	switch (m_pMainHeader->iQuantization) {
	case DAFF_INT16:
	case DAFF_INT24:
	case DAFF_FLOAT32:
		break;

	default:
		// Invalid quantization
		tidyup();
		return DAFF_FILE_CORRUPTED;
	};

	if (m_pMainHeader->iNumChannels < 1)
		return DAFF_FILE_CORRUPTED;

	if (m_pMainHeader->iNumRecords < 1)
		return DAFF_FILE_CORRUPTED;

	if (m_pMainHeader->iElementsPerRecord < 1)
		return DAFF_FILE_CORRUPTED;

	if (m_pMainHeader->fMeasurementDistance < 0.0f)
		return DAFF_FILE_CORRUPTED;

	// Alpha angle validation
	if (m_pMainHeader->iAlphaPoints < 1)
		return DAFF_FILE_CORRUPTED;

	if (m_pMainHeader->fAlphaStart > m_pMainHeader->fAlphaEnd)
		return DAFF_FILE_CORRUPTED;

	if ( (m_pMainHeader->fAlphaStart < 0.0f) || (m_pMainHeader->fAlphaStart > 360.0f) )
		return DAFF_FILE_CORRUPTED;

	if ( (m_pMainHeader->fAlphaEnd < 0.0f) || (m_pMainHeader->fAlphaEnd > 360.0f) )
		return DAFF_FILE_CORRUPTED;

	// Beta angle validation
	if (m_pMainHeader->iBetaPoints < 1)
		return DAFF_FILE_CORRUPTED;

	if (m_pMainHeader->fBetaStart > m_pMainHeader->fBetaEnd)
		return DAFF_FILE_CORRUPTED;

	if ( (m_pMainHeader->fBetaStart < 0.0f) || (m_pMainHeader->fBetaStart > 180.0f) )
		return DAFF_FILE_CORRUPTED;

	if ( (m_pMainHeader->fBetaEnd < 0.0f) || (m_pMainHeader->fBetaEnd > 180.0f) )
		return DAFF_FILE_CORRUPTED;

	// Orientation
	m_orientationDefault.fYawAngle = m_pMainHeader->fOrientYaw;
	m_orientationDefault.fPitchAngle = m_pMainHeader->fOrientPitch;
	m_orientationDefault.fRollAngle = m_pMainHeader->fOrientRoll;
	m_tTrans.setOrientation(m_orientationDefault);

	/*
	 *  3nd step: Load the content header, validate it for correctness
	 */

	DAFFFileBlockEntry* pfbContentHeader;
	if (getFirstFileBlockByID(FILEBLOCK_DAFF1_CONTENT_HEADER, pfbContentHeader) != 1) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pContentHeader = (char*) DAFF::malloc_aligned16((size_t) pfbContentHeader->ui64Size);
	fseek(m_file, (long) pfbContentHeader->ui64Offset, SEEK_SET);
	if (fread(m_pContentHeader, 1, (size_t) pfbContentHeader->ui64Size, m_file) != (size_t) pfbContentHeader->ui64Size) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	switch (m_pMainHeader->iContentType) {
	case DAFF_IMPULSE_RESPONSE:
		m_pContentHeaderIR = static_cast<DAFFContentHeaderIR*>( m_pContentHeader );
		m_pContentHeaderIR->fixEndianness();

		if (m_pContentHeaderIR->fSamplerate < 0.0f)
			return DAFF_FILE_CORRUPTED;

		if ( (m_pContentHeaderIR->iMaxEffectiveFilterLength < 0) ||
			 (m_pContentHeaderIR->iMaxEffectiveFilterLength > m_pMainHeader->iElementsPerRecord) )
			return DAFF_FILE_CORRUPTED;

		if ( (m_pContentHeaderIR->iMinFilterOffset < 0) || 
			 (m_pContentHeaderIR->iMinFilterOffset > m_pMainHeader->iElementsPerRecord) )
			return DAFF_FILE_CORRUPTED;

		break;

	case DAFF_MAGNITUDE_SPECTRUM:
		m_pContentHeaderMS = static_cast<DAFFContentHeaderMS*>( m_pContentHeader );
		//m_pContentHeaderMS->fixEndianness();

		if (m_pContentHeaderMS->iNumFreqs <= 0)
			return DAFF_FILE_CORRUPTED;

		float* pfFreqs = (float*) ((char*) m_pContentHeader + 4);

		// Fix the endianness of the frequency list
		DAFF::le2se_4byte(pfFreqs, m_pContentHeaderMS->iNumFreqs);

		// Load the (dynamically sized) frequency list
		m_vfFreqs.resize(m_pContentHeaderMS->iNumFreqs);
		for (int i=0; i<m_pContentHeaderMS->iNumFreqs; i++) m_vfFreqs[i] = pfFreqs[i];
				
		break;
	};

	/*
	 *  4rd step: Load the record descriptor block
	 */

	DAFFFileBlockEntry* pfbRDT;
	if (getFirstFileBlockByID(FILEBLOCK_DAFF1_RECORD_DESC, pfbRDT) != 1) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pRecordDescBlock = (char*) DAFF::malloc_aligned16((size_t) pfbRDT->ui64Size);
	fseek(m_file, (long) pfbRDT->ui64Offset, SEEK_SET);
	if (fread(m_pRecordDescBlock, 1, (size_t) pfbRDT->ui64Size, m_file) != (size_t) pfbRDT->ui64Size) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	// Set access pointer and fix the endianness
	switch (m_pMainHeader->iContentType) {
	case DAFF_IMPULSE_RESPONSE:
		m_pRecordDescIR = reinterpret_cast<DAFFRecordDescIR*>( m_pRecordDescBlock );
		for (int i=0; i<m_pMainHeader->iNumRecords; i++) m_pRecordDescIR[i].fixEndianness();

		break;

	case DAFF_MAGNITUDE_SPECTRUM:
		m_pRecordDescMS = reinterpret_cast<DAFFRecordDescMS*>( m_pRecordDescBlock );
		for (int i=0; i<m_pMainHeader->iNumRecords; i++) m_pRecordDescMS[i].fixEndianness();
		
		break;
	};

	/*
	 *  5th step: Load the record data
	 */

	DAFFFileBlockEntry* pfbData;
	if (getFirstFileBlockByID(FILEBLOCK_DAFF1_DATA, pfbData) != 1) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pDataBlock = DAFF::malloc_aligned16((size_t) pfbData->ui64Size);
	fseek(m_file, (long) pfbData->ui64Offset, SEEK_SET);
	if (fread(m_pDataBlock, 1, (size_t) pfbData->ui64Size, m_file) != (size_t) pfbData->ui64Size) {
		DAFF::free_aligned16(m_pDataBlock);
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	// Fix the endianess of the data
	switch (m_pMainHeader->iQuantization) {
	case DAFF_INT16:
		DAFF::le2se_2byte(m_pDataBlock, (size_t) (pfbData->ui64Size / 2));
		break;

	case DAFF_INT24:
		DAFF::le2se_3byte(m_pDataBlock, (size_t) (pfbData->ui64Size / 3));
		break;

	case DAFF_FLOAT32:
		DAFF::le2se_4byte(m_pDataBlock, (size_t) (pfbData->ui64Size / 4));
		break;
	}

	/*
	 *  6th step: Load the metadata
	 */

	DAFFFileBlockEntry* pfbMetadata;
	if (getFirstFileBlockByID(FILEBLOCK_DAFF1_METADATA, pfbMetadata) > 1) {
		tidyup();
		return DAFF_FILE_CORRUPTED;
	}

	m_pMetadata = new DAFFMetadataImpl;
	if (pfbMetadata) {
		// Metadata block present
		void* pMetadataBuf = DAFF::malloc_aligned16((size_t) pfbMetadata->ui64Size);
		fseek(m_file, (long) pfbMetadata->ui64Offset, SEEK_SET);
		if (fread(pMetadataBuf, 1, (size_t) pfbMetadata->ui64Size, m_file) != (size_t) pfbMetadata->ui64Size) {
			DAFF::free_aligned16(pMetadataBuf);
			tidyup();
			return DAFF_FILE_CORRUPTED;
		}

		int iError = m_pMetadata->load(pMetadataBuf);
		if (iError != 0) {
			DAFF::free_aligned16(pMetadataBuf);
			tidyup();
			return iError;
		}

		DAFF::free_aligned16(pMetadataBuf);
	}

	// Everything worked fine ...

	fclose(m_file);

	// Important: If there is only one point in a dimension => Then there is no resolution
	// TODO: Recheck if this is correct for non-full and full angular ranges
	
	if (m_pMainHeader->iAlphaPoints > 1)
		m_fAlphaResolution = (m_pMainHeader->fAlphaEnd - m_pMainHeader->fAlphaStart) / m_pMainHeader->iAlphaPoints;
	else
		m_fAlphaResolution = 0;

	if (m_pMainHeader->iBetaPoints > 1)
		m_fBetaResolution = (m_pMainHeader->fBetaEnd - m_pMainHeader->fBetaStart) / (m_pMainHeader->iBetaPoints-1);
	else
		m_fBetaResolution = 0;

	m_sFilename = sFilename;
	m_bFileOpened = true;

	return 0;
}

void DAFFReaderImpl::closeFile() {
	if (!m_bFileOpened) return;

	tidyup();
}

void DAFFReaderImpl::tidyup() {
	if (m_file) fclose(m_file);

	DAFF::free_aligned16(m_pFileBlockTable);
	m_pFileBlockTable = NULL;

	DAFF::free_aligned16(m_pMainHeader);
	m_pMainHeader = NULL;

	DAFF::free_aligned16(m_pContentHeader);
	m_pContentHeader = NULL;

	DAFF::free_aligned16(m_pRecordDescBlock);
	m_pRecordDescBlock = NULL;

	DAFF::free_aligned16(m_pDataBlock);
	m_pDataBlock = NULL;

	delete m_pMetadata;
	m_pMetadata = NULL;

	m_sFilename = "";
	m_bFileOpened = false;
}

int DAFFReaderImpl::getFirstFileBlockByID(int iID, DAFFFileBlockEntry*& pfDest) const {
	std::vector<DAFFFileBlockEntry*> v;
	getFileBlocksByID(iID, v);

	if (v.empty()) {
		pfDest = NULL;
		return 0;
	}

	pfDest = v.front();
	return (int) v.size();
}

int DAFFReaderImpl::getFileBlocksByID(int iID, std::vector<DAFFFileBlockEntry*>& vpfDest) const {
	vpfDest.clear();
	for (int i=0; i<m_fileHeader.iNumFileBlocks; i++) {
		if (m_pFileBlockTable[i].iID == iID) vpfDest.push_back( &(m_pFileBlockTable[i]) );
	}

	return (int) vpfDest.size();
}

std::string DAFFReaderImpl::getFilename() const {
	assert( m_bFileOpened );
	return m_sFilename;
}

int DAFFReaderImpl::getFileFormatVersion() const {
	assert( m_bFileOpened );
	return m_fileHeader.iFileFormatVersion;
}

int DAFFReaderImpl::getContentType() const {
	assert( m_bFileOpened );
	return m_pMainHeader->iContentType;
}

DAFFContent* DAFFReaderImpl::getContent() const {
	assert( m_bFileOpened );
	// Logical constness here!
	// TODO: Bad cast
	return (DAFFContentIR*) this;
}

DAFFMetadata* DAFFReaderImpl::getMetadata() const {
	assert( m_bFileOpened );
	return m_pMetadata;
}

const DAFFProperties* DAFFReaderImpl::getProperties() const {
	assert( m_bFileOpened );
	return this;
}

std::string DAFFReaderImpl::toString() const {
	assert( m_bFileOpened );
	
	std::stringstream ss;

	float fVersion = getFileFormatVersion() / 1000.0F;;
	ss << "File format version: " << DAFFUtils::Float2StrNice(fVersion, 3, false) << std::endl;
	ss << "Content type:        " << DAFFUtils::StrContentType( getProperties()->getContentType()) << std::endl;
	ss << "Quantization:        " << DAFFUtils::StrQuantizationType( getProperties()->getQuantization()) << std::endl << std::endl;
	ss << "Number of channels:  " << getProperties()->getNumberOfChannels() << std::endl;
	ss << "Number of records:   " << getProperties()->getNumberOfRecords() << std::endl << std::endl;
	ss << "Alpha points:        " << getProperties()->getAlphaPoints() << std::endl;
	ss << "Alpha range:         [" <<
		   DAFFUtils::Float2StrNice(getProperties()->getAlphaStart(), 3, false) << "\xF8, " <<
		   DAFFUtils::Float2StrNice(getProperties()->getAlphaEnd(), 3, false) << "\xF8]" << std::endl;
	ss << "Beta points:         " << getProperties()->getBetaPoints() << std::endl;
	ss << "Beta range:          [" <<
		   DAFFUtils::Float2StrNice(getProperties()->getBetaStart(), 3, false) << "\xF8, " <<
		   DAFFUtils::Float2StrNice(getProperties()->getBetaEnd(), 3, false) << "\xF8]" << std::endl;
	ss << "Full sphere:         " << (getProperties()->coversFullSphere() ? "yes" : "no") << std::endl;
	DAFFOrientationYPR o;
	getOrientation(o);
	ss << "Orientation:         " << o.toString() << std::endl;

	DAFFContentIR* pContentIR;
	DAFFContentMS* pContentMS;
	
	switch (getContentType()) {
		case DAFF_IMPULSE_RESPONSE:
			pContentIR = dynamic_cast<DAFFContentIR*>(getContent());
			ss << "Samplerate:          " << DAFFUtils::Double2StrNice(pContentIR->getSamplerate(), 1, false) << " Hz" << std::endl;
			ss << "Filter length:       " << pContentIR->getFilterLength() << std::endl;
			break;

		case DAFF_MAGNITUDE_SPECTRUM:
			pContentMS = dynamic_cast<DAFFContentMS*>(getContent());
			ss << "Number of frequencies: " << pContentMS->getNumFrequencies() << std::endl;
			const std::vector<float> vsFrequencies(pContentMS->getFrequencies());
			ss << "Frequency support:     ";
			for (size_t i=0; i<vsFrequencies.size(); i++) {
				ss << DAFFUtils::Float2StrNice(vsFrequencies[i], 3, false);
				if (i < (vsFrequencies.size()-1)) ss << ", ";
			}
			ss << " Hz" << std::endl;
			break;
	}
	return ss.str();
}

// --------------------------------------------------------

int DAFFReaderImpl::getQuantization() const {
	assert( m_bFileOpened );
	return m_pMainHeader->iQuantization;
}

int DAFFReaderImpl::getNumberOfChannels() const {
	assert( m_bFileOpened );
	return m_pMainHeader->iNumChannels;
}

int DAFFReaderImpl::getNumberOfRecords() const {
	assert( m_bFileOpened );
	return m_pMainHeader->iNumRecords;
}

float DAFFReaderImpl::getMeasurementDistance() const {
	assert( m_bFileOpened );
	return m_pMainHeader->fMeasurementDistance;
}

std::string DAFFReaderImpl::getChannelLabel(int iChannel) const {
	assert( m_bFileOpened );
	assert( (iChannel >= 0) && (iChannel < m_pMainHeader->iNumChannels));

	// Fetch the channel name from the metadata
	std::stringstream ss;
	ss << "LABEL_CHANNEL_" << (iChannel+1);
	return (m_pMetadata->hasKey(ss.str()) ? m_pMetadata->getKeyString(ss.str()) : "");
}

int DAFFReaderImpl::getAlphaPoints() const {
	assert( m_bFileOpened );
	return m_pMainHeader->iAlphaPoints;
}

float DAFFReaderImpl::getAlphaResolution() const {
	assert( m_bFileOpened );

	return m_fAlphaResolution;
}

float DAFFReaderImpl::getAlphaStart() const {
	assert( m_bFileOpened );
	return m_pMainHeader->fAlphaStart;
}

float DAFFReaderImpl::getAlphaEnd() const {
	assert( m_bFileOpened );
	return m_pMainHeader->fAlphaEnd;
}

int DAFFReaderImpl::getBetaPoints() const {
	assert( m_bFileOpened );
	return m_pMainHeader->iBetaPoints;
}

float DAFFReaderImpl::getBetaResolution() const {
	assert( m_bFileOpened );
	return m_fBetaResolution;
}

float DAFFReaderImpl::getBetaStart() const {
	assert( m_bFileOpened );
	return m_pMainHeader->fBetaStart;
}

float DAFFReaderImpl::getBetaEnd() const {
	assert( m_bFileOpened );
	return m_pMainHeader->fBetaEnd;
}

void DAFFReaderImpl::getDefaultOrientation(DAFFOrientationYPR& o) const {
	assert( m_bFileOpened );
	o = m_orientationDefault;
}

void DAFFReaderImpl::setDefaultOrientation() {
	assert( m_bFileOpened );
	m_tTrans.setOrientation(m_orientationDefault);
}

void DAFFReaderImpl::getOrientation(DAFFOrientationYPR& o) const {
	assert( m_bFileOpened );
	m_tTrans.getOrientation(o);
}

void DAFFReaderImpl::setOrientation(const DAFFOrientationYPR& o) {
	assert( m_bFileOpened );
	m_tTrans.setOrientation(o);
}

bool DAFFReaderImpl::coversFullSphere() const {
	assert( m_bFileOpened );
	if ( ((m_pMainHeader->fAlphaEnd - m_pMainHeader->fAlphaStart) >= (360.0f - getAlphaResolution()) &&
		 ((m_pMainHeader->fBetaEnd - m_pMainHeader->fBetaStart) == 180.0f)) )
		return true;
	return false;
}

DAFFReader* DAFFReaderImpl::getParent() const {
	// We are the reader ourself!
	// Logical constness here!
	return const_cast<DAFFReaderImpl*>(this);
}

void DAFFReaderImpl::getNearestNeighbour(int iView, float fAngle1, float fAngle2, int& iRecordIndex) {
	assert( (iView == DAFF_DATA_VIEW) || (iView == DAFF_OBJECT_VIEW) );

	bool bDummy;
	getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex, bDummy);
}

int DAFFReaderImpl::getRecordCoords(int iRecordIndex, int iView, float& fAngle1, float& fAngle2) const {
	assert( (iRecordIndex >= 0) || (iRecordIndex < m_pMainHeader->iNumRecords) );
	assert( (iView == DAFF_DATA_VIEW) || (iView == DAFF_OBJECT_VIEW) );

	// Invalid record index - projection to get a valid index
	if ((iRecordIndex < 0) || (iRecordIndex >= m_pMainHeader->iNumRecords)) {
		return -1;
	}

	int iAlpha, iBeta;
	float fAlpha, fBeta;

	bool bSouthPolePresent = false;
	if (m_pMainHeader->fBetaStart == 0.0f) // South pole present (single record here)
		bSouthPolePresent = true;

	if (bSouthPolePresent) {
		if (iRecordIndex == 0) { // Hit south pole
			fAlpha = 0.0f;
			fBeta  = 0.0f;
		} else { // somewhere inside the grid (with single value at south pole)
			iAlpha = (iRecordIndex - 1) % m_pMainHeader->iAlphaPoints;
			iBeta = 1 + (int) ((iRecordIndex - 1) / m_pMainHeader->iAlphaPoints);
			fAlpha = m_pMainHeader->fAlphaStart + ((float) iAlpha * m_fAlphaResolution);
			fBeta  = (float) iBeta  * m_fBetaResolution;
		}
	} else { // somewhere inside the grid (without south pole present)
			iAlpha = iRecordIndex % m_pMainHeader->iAlphaPoints;
			iBeta = (int) (iRecordIndex / m_pMainHeader->iAlphaPoints);
			fAlpha = m_pMainHeader->fAlphaStart + ((float) iAlpha * m_fAlphaResolution);
			fBeta  = m_pMainHeader->fBetaStart  + ((float) iBeta  * m_fBetaResolution);
	}

	if (iView == DAFF_DATA_VIEW) {
		fAngle1 = fAlpha;
		fAngle2 = fBeta;
	} else {
		transformAnglesD2O(fAlpha, fBeta, fAngle1, fAngle2);
	}
	return 0;
}

void DAFFReaderImpl::getNearestNeighbour(int iView, float fAngle1, float fAngle2, int& iRecordIndex, bool& bOutOfBounds) {
	assert( (iView == DAFF_DATA_VIEW) || (iView == DAFF_OBJECT_VIEW) );

	float fAlpha;
	float fBeta;

	if (iView == DAFF_DATA_VIEW) {
		fAlpha = fAngle1;
		fBeta = fAngle2;
	} else {
		// Transform the coordinates into the DSC
		transformAnglesO2D(fAngle1, fAngle2, fAlpha, fBeta);
	}

	// Normalize the direction
	DAFFUtils::NormalizeDirection(DAFF_DATA_VIEW, fAlpha, fBeta, fAlpha, fBeta);

	iRecordIndex = -1;
	bOutOfBounds = false;
	
	int iAlphaIndex, iBetaIndex;

	if (m_pMainHeader->iAlphaPoints == 1) {
		// Trivial case: Just one point
		iAlphaIndex = 0;
	} else {
		if ((fAlpha >= m_pMainHeader->fAlphaStart) && (fAlpha <= m_pMainHeader->fAlphaEnd)) {
			// Within the covered alpha range
			iAlphaIndex = (int) roundf((fAlpha - m_pMainHeader->fAlphaStart) / m_fAlphaResolution);
		} else {
			// Outside the covered alpha range
			// Decide: Which is closer? Start boundary or end boundary?
			if (DAFF::anglef_mindiff_abs_0_360_DEG(m_pMainHeader->fAlphaStart, fAlpha) <= DAFF::anglef_mindiff_abs_0_360_DEG(m_pMainHeader->fAlphaEnd, fAlpha))
				iAlphaIndex = 0; // Start index
			else
				iAlphaIndex = (int) m_pMainHeader->iAlphaPoints - 1; // End index
		}
	}

	if (m_pMainHeader->iBetaPoints == 1) {
		// Trivial case: Just one point
		iBetaIndex = 0;
	} else {
		if ((fBeta >= m_pMainHeader->fBetaStart) && (fBeta <= m_pMainHeader->fBetaEnd)) {
			// Within the covered beta range
			iBetaIndex = (int) roundf((fBeta - m_pMainHeader->fBetaStart) / m_fBetaResolution);
		} else {
			// Outside the covered beta range
			// Decide: Which is closer? Start boundary or end boundary?
			if (std::abs(m_pMainHeader->fBetaStart - fBeta) <= std::abs(m_pMainHeader->fBetaEnd - fBeta))
				iBetaIndex = 0; // Start index
			else
				iBetaIndex = (int) m_pMainHeader->iBetaPoints - 1; // End index
		}
	}

	// Calculate index
	if (m_pMainHeader->fBetaStart == 0.0f) { // South pole present: increment by one (single record at poles)
		if (iBetaIndex == 0) { // Hit south pole
			iRecordIndex = 0;
		} else {
			if ((iBetaIndex == m_pMainHeader->iBetaPoints-1) && (m_pMainHeader->fBetaEnd == 180.0f)) // Hit north pole
				iRecordIndex = 1 + (iBetaIndex - 1) * m_pMainHeader->iAlphaPoints;
			else
				iRecordIndex = 1 + (iBetaIndex - 1) * m_pMainHeader->iAlphaPoints + iAlphaIndex;
		}
	} else {
		if ((iBetaIndex == m_pMainHeader->iBetaPoints-1) && (m_pMainHeader->fBetaEnd == 180.0f)) // Hit north pole
			iRecordIndex = iBetaIndex * m_pMainHeader->iAlphaPoints;
		else
			iRecordIndex = iBetaIndex * m_pMainHeader->iAlphaPoints + iAlphaIndex;
	}

	return;
}

void DAFFReaderImpl::getCell(int iView, const float fAngle1, const float fAngle2, DAFFQuad& qIndices) {

	//switch to data view
	float fAlpha = fAngle1;
	float fBeta = fAngle2;
	if (iView == DAFF_OBJECT_VIEW)
		transformAnglesO2D(fAngle1, fAngle2, fAlpha, fBeta);
	DAFFUtils::NormalizeDirection(DAFF_DATA_VIEW, fAlpha, fBeta, fAlpha, fBeta);

	// south pole with full sphere covered is a problem (but algorithm holds for north pole)
	if (fBeta == 0.0f && coversFullSphere()) { // return the south pole 4 times
		getNearestNeighbour(DAFF_DATA_VIEW, 0.0f, 0.0f, qIndices.iIndex1);
		qIndices.iIndex2 = qIndices.iIndex1;
		qIndices.iIndex3 = qIndices.iIndex1;
		qIndices.iIndex4 = qIndices.iIndex1;
		return;
	}

	// grid angles
	float fAlpha1=0, fAlpha2=0, fAlpha3=0, fAlpha4=0;
	float fBeta1=0, fBeta2=0, fBeta3=0, fBeta4=0;

	fAlpha1 = fAlpha - fmodf(fAlpha, getAlphaResolution());
	fAlpha2 = fAlpha1;
	fAlpha3 = fAlpha + getAlphaResolution() - fmodf(fAlpha, getAlphaResolution());
	fAlpha4 = fAlpha3;

	fBeta1 = fBeta - fmodf(fBeta, getBetaResolution());
	fBeta2 = fBeta + getBetaResolution() - fmodf(fBeta, getBetaResolution());
	fBeta3 = fBeta2;
	fBeta4 = fBeta1;

	// Upper beta angles are not allowed to overrun the north pole
	if (fBeta2 > 180.0f) {
		fBeta2 = 180.0f;
		fBeta3 = 180.0f;
	}

	// Normalize & get indices
	DAFFUtils::NormalizeDirection(DAFF_DATA_VIEW, fAlpha1, fBeta1, fAlpha1, fBeta1);
	DAFFUtils::NormalizeDirection(DAFF_DATA_VIEW, fAlpha2, fBeta2, fAlpha2, fBeta2);
	DAFFUtils::NormalizeDirection(DAFF_DATA_VIEW, fAlpha3, fBeta3, fAlpha3, fBeta3);
	DAFFUtils::NormalizeDirection(DAFF_DATA_VIEW, fAlpha4, fBeta4, fAlpha4, fBeta4);

	getNearestNeighbour(DAFF_DATA_VIEW, fAlpha1, fBeta1, qIndices.iIndex1);
	getNearestNeighbour(DAFF_DATA_VIEW, fAlpha2, fBeta2, qIndices.iIndex2);
	getNearestNeighbour(DAFF_DATA_VIEW, fAlpha3, fBeta3, qIndices.iIndex3);
	getNearestNeighbour(DAFF_DATA_VIEW, fAlpha4, fBeta4, qIndices.iIndex4);
}

void DAFFReaderImpl::transformAnglesD2O(const float fAlpha, const float fBeta, float& fAzimuth, float& fElevation) const {
	m_tTrans.transformDSC2OSC(fAlpha, fBeta, fAzimuth, fElevation);
}

void DAFFReaderImpl::transformAnglesO2D(const float fAzimuth, const float fElevation, float& fAlpha, float& fBeta) const {
	m_tTrans.transformOSC2DSC(fAzimuth, fElevation, fAlpha, fBeta);
}

double DAFFReaderImpl::getSamplerate() const {
	return m_pContentHeaderIR->fSamplerate;
}

int DAFFReaderImpl::getFilterLength() const {
	return m_pMainHeader->iElementsPerRecord;
}

int DAFFReaderImpl::getMinEffectiveFilterOffset() const {
	return m_pContentHeaderIR->iMinFilterOffset;
}

int DAFFReaderImpl::getMaxEffectiveFilterLength() const {
	return m_pContentHeaderIR->iMaxEffectiveFilterLength;
}

int DAFFReaderImpl::getFilterCoeffs(int iRecordIndex, int iChannel, float* pfDest) const {
	int iOffset;
	int iEffectiveLength;
	int iError;

	iError = getEffectiveFilterBounds(iRecordIndex, iChannel, iOffset, iEffectiveLength);
	if (iError != DAFF_NO_ERROR) return iError;

	// Place zeros before the data
	for (int i=0; i<iOffset; i++) pfDest[i] = 0;

	// Insert the data
	iError = getEffectiveFilterCoeffs(iRecordIndex, iChannel, pfDest+iOffset);
	
	// Place zeros behind the data
	for	(int i=iOffset+iEffectiveLength; i<getFilterLength(); i++) pfDest[i] = 0;
		
	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getEffectiveFilterBounds(int iRecordIndex, int iChannel, int& iOffset, int& iLength) const {
	assert( (iRecordIndex >= 0) && (iRecordIndex < m_pMainHeader->iNumRecords) );
	assert( (iChannel >= 0) && (iChannel < m_pMainHeader->iNumChannels) );

	if ((iRecordIndex < 0) || (iRecordIndex >= m_pMainHeader->iNumRecords) ||
		(iChannel < 0) || (iChannel >= m_pMainHeader->iNumChannels) )
		return DAFF_INVALID_INDEX;

	DAFFRecordDescIR& pDesc = m_pRecordDescIR[iRecordIndex*m_pMainHeader->iNumChannels + iChannel];
	iOffset = pDesc.iOffset;
	iLength = pDesc.iLength;

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getEffectiveFilterCoeffs(int iRecordIndex, int iChannel, float* pfDest) const {
	assert( (iRecordIndex >= 0) && (iRecordIndex < m_pMainHeader->iNumRecords) );
	assert( (iChannel >= 0) && (iChannel < m_pMainHeader->iNumChannels) );

	if ((iRecordIndex < 0) || (iRecordIndex >= m_pMainHeader->iNumRecords) ||
		(iChannel < 0) || (iChannel >= m_pMainHeader->iNumChannels) )
		return DAFF_INVALID_INDEX;

	if (pfDest == NULL) return DAFF_NO_ERROR;

	DAFFRecordDescIR& pDesc = m_pRecordDescIR[iRecordIndex*m_pMainHeader->iNumChannels + iChannel];
	void* pData = reinterpret_cast<char*>(m_pDataBlock) + pDesc.ui64DataOffset;

	// Data type conversion
	switch (m_pMainHeader->iQuantization) {
		case DAFF_INT16:
			DAFF::stc_sint16_to_float(pfDest, (const short*) pData, pDesc.iLength);
			break;

		case DAFF_INT24:
			DAFF::stc_sint24_to_float(pfDest, pData, pDesc.iLength);
			break;

		case DAFF_FLOAT32:
			// Direct copy
			memcpy(pfDest, pData, pDesc.iLength*sizeof(float));
			break;
	}

	return DAFF_NO_ERROR;
}

int DAFFReaderImpl::getNumFrequencies() const {
	return m_pContentHeaderMS->iNumFreqs;
}

const std::vector<float>& DAFFReaderImpl::getFrequencies() const {
	return m_vfFreqs;
}

float DAFFReaderImpl::getOverallMagnitudeMaximum() const {
	return m_pContentHeaderMS->fMax;
}

int DAFFReaderImpl::getMagnitudes(int iRecordIndex, int iChannel, float* pfData) const {
	assert( (iRecordIndex >= 0) && (iRecordIndex < m_pMainHeader->iNumRecords) );
	assert( (iChannel >= 0) && (iChannel < m_pMainHeader->iNumChannels) );

	if ( (iRecordIndex < 0) || (iRecordIndex >= m_pMainHeader->iNumRecords) ||
		(iChannel < 0) || (iChannel >= m_pMainHeader->iNumChannels) )
		return DAFF_INVALID_INDEX;

	if (pfData == NULL) return DAFF_NO_ERROR;

	DAFFRecordDescMS& pDesc = m_pRecordDescMS[iRecordIndex];
	float* pfSrc = reinterpret_cast<float*>( reinterpret_cast<char*>(m_pDataBlock) + pDesc.ui64DataOffset );
	memcpy(pfData, pfSrc, m_pContentHeaderMS->iNumFreqs*sizeof(float));

	return DAFF_NO_ERROR;
}
