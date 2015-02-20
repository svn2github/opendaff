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
 *  File:		DAFFReaderImpl.h
 *  Purpose:	Reader implementation headerfile
 *  Version:    $Id: DAFFReaderImpl.h,v 1.11 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de),
 *              Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)
 *
 */

#ifndef __DAFFREADERIMPL_H__
#define __DAFFREADERIMPL_H__

#include <DAFFDefs.h>
#include <DAFFContent.h>
#include <DAFFContentIR.h>
#include <DAFFContentMS.h>
#include <DAFFContentPS.h>
#include <DAFFContentMPS.h>
#include <DAFFContentDFT.h>
#include "DAFFHeader.h"
#include <DAFFProperties.h>
#include <DAFFReader.h>
#include "DAFFSCTransform.h"

class DAFFMetadataImpl;

class DAFFReaderImpl : public DAFFReader, public DAFFProperties, public DAFFContentIR, public DAFFContentMS, public DAFFContentPS, public DAFFContentMPS, public DAFFContentDFT {
public:
	DAFFReaderImpl();
	~DAFFReaderImpl();

	bool isFileOpened() const;
	int openFile(const std::string& sFilename);
	void closeFile();
	std::string getFilename() const;

	int getFileFormatVersion() const;
	int getContentType() const;
	DAFFContent* getContent() const;
	const DAFFMetadata* getMetadata() const;
	DAFFProperties* getProperties() const;

	std::string toString() const;	

	// --= Implementation of the interface "DAFFProperties" =--

	int getQuantization() const;
	int getNumberOfChannels() const;
	int getNumberOfRecords() const;
	std::string getChannelLabel(int iChannel) const;

	int getAlphaPoints() const;
	float getAlphaResolution() const;
	float getAlphaStart() const;
	float getAlphaEnd() const;
	float getAlphaSpan() const;
	int getBetaPoints() const;
	float getBetaResolution() const;
	float getBetaStart() const;
	float getBetaEnd() const;
	float getBetaSpan() const;

	void getDefaultOrientation(DAFFOrientationYPR& o) const;
	void setDefaultOrientation();
	void getOrientation(DAFFOrientationYPR& o) const;
	void setOrientation(const DAFFOrientationYPR& o);

	bool coversFullAlphaRange() const;
	bool coversFullBetaRange() const;
	bool coversFullSphere() const;

	// --= Interface "DAFFContent" =--

	DAFFReader* getParent() const;
	const DAFFMetadata* getRecordMetadata(int iRecordIndex) const;
	int getRecordCoords(int iRecordIndex, int iView, float& fAngle1, float& fAngle2) const;
	void getNearestNeighbour(int iView, float fAngle1, float fAngle2, int& iRecordIndex) const;
	void getNearestNeighbour(int iView, float fAngle1, float fAngle2, int& iRecordIndex, bool& bOutOfBounds) const;
	void getCell(int iView, const float fAngle1, const float fAngle2, DAFFQuad& qIndices) const;
	void transformAnglesD2O(const float fAlpha, const float fBeta, float& fAzimuth, float& fElevation) const;
	void transformAnglesO2D(const float fAzimuth, const float fElevation, float& fAlpha, float& fBeta) const;

	// --= Interface "DAFFContentIR" =--

	double getSamplerate() const;
	int getFilterLength() const;
	int getFilterCoeffs(int iRecordIndex, int iChannel, float* pfDest, float fGain=1.0F) const;
	int getMinEffectiveFilterOffset() const;
	int getMaxEffectiveFilterLength() const;
	int getEffectiveFilterBounds(int iRecordIndex, int iChannel, int& iOffset, int& iLength) const;
	int getEffectiveFilterCoeffs(int iRecordIndex, int iChannel, float* pfDest, float fGain=1.0F) const;
	float getOverallPeak(); // no const because of lazy initialization

	// --= Interface "DAFFContentMS" =--

	int getNumFrequencies() const;
	const std::vector<float>& getFrequencies() const;
	float getOverallMagnitudeMaximum() const;
	int getMagnitudes(int iRecordIndex, int iChannel, float* pfData) const;
	int getMagnitude(int iRecordIndex, int iChannel, int iFreqIndex, float& fMag) const;
	
	// --= Interface "DAFFContentPS" =--

	int getPhases(int iRecordIndex, int iChannel, float* pfData) const;
	int getPhase(int iRecordIndex, int iChannel, int iFreqIndex, float& fPhase) const;

	// --= Interface "DAFFContentMPS" =--

	int getCoefficientsMP(int iRecordIndex, int iChannel, float* pfDest) const;
	int getCoefficientsRI(int iRecordIndex, int iChannel, float* pfDest) const;

	// --= Interface "DAFFContentDFT" =--

	int getTransformSize() const;
	int getNumDFTCoeffs() const;
	bool isSymetric() const;
	double getFrequencyBandwidth() const;
	int getDFTCoeff(int iRecordIndex, int iChannel, int iDFTCoeff, float& fReal, float& fImag) const;
	int getDFTCoeffs(int iRecordIndex, int iChannel, float* pfDest) const;

private:
	bool m_bFileOpened;							// File state
	std::string m_sFilename;					// Filename
	FILE* m_file;								// File handle
	DAFFFileHeader m_fileHeader;				// File header
	DAFFFileBlockEntry* m_pFileBlockTable;		// File block table
	DAFFMainHeader* m_pMainHeader;				// Main header
	DAFFFileBlockEntry* m_pfbRDT;				// File block of the record descriptor table
	DAFFFileBlockEntry* m_pfbData;				// File block of the record descriptor table
	void* m_pContentHeader;						// Content related header (will become IR or MS)
	void* m_pRecordDescBlock;					// Record descriptor block
	void* m_pDataBlock;							// Record data block
	int m_iRecordChannelDescSize;				// Size of a record channel descriptor (Bytes)

	DAFFContentHeaderIR* m_pContentHeaderIR;	// Access pointer for additional header for impulse response content
	DAFFContentHeaderMS* m_pContentHeaderMS;	// Access pointer for additional header for magnitude spectrum content
	DAFFContentHeaderPS* m_pContentHeaderPS;	// Access pointer for additional header for phase spectrum content
	DAFFContentHeaderMPS* m_pContentHeaderMPS;	// Access pointer for additional header for magnitude-phase spectrum content
	DAFFContentHeaderDFT* m_pContentHeaderDFT;	// Access pointer for additional header for discrete fourier-spectrum content
	std::vector<float> m_vfFreqs;				// List of frequencies (magnitude spectra)
	
	const DAFFMetadataImpl* m_pEmptyMetadata;	// Emptry metadata instance. getRecordMetadata() will return this as fallback
	std::vector<DAFFMetadataImpl*> m_vpMetadata;// Metadata pointer
	DAFFProperties* m_pProperties;				// Properties pointer
	bool m_bOverallPeakInitialized;				// tells if fOverallPeak has already been initialized (lazy initialization)
	float m_fOverallPeak;						// Peak value over all records and channels

	DAFFOrientationYPR m_orientation;			// Current orientation
	DAFFOrientationYPR m_orientationDefault;	// Default orientation

	float m_fAlphaResolution;					// Alpha resolution [�]
	float m_fBetaResolution;					// Beta resolution [�]
	DAFFSCTransform m_tTrans;			 		// Spherical coordinates transformer

	// Search for the first file block that matches the given ID
	// (returns total number of matching file blocks)
	int getFirstFileBlockByID(int iID, DAFFFileBlockEntry*& pfDest) const;

	// Search for all file blocks with the given ID (returns the number of matches)
	int getFileBlocksByID(int iID, std::vector<DAFFFileBlockEntry*>& vpfDest) const;

	// Returns the memory address of a record channel descriptor in the RDB
	void* getRecordChannelDescPtr(int iRecord, int iChannel) const;

	// Returns the memory address of a record metadata index in the RDB
	int* getRecordMetadataIndexPtr(int iRecord) const;

	// Clear up the instance, free resources revert to the uninitialized state
	void tidyup();
};

#endif // __DAFFREADERIMPL_H__
