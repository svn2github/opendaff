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
 *  File:		DAFFTransformerIR2DFT.h
 *  Purpose:	Transformer from impulse responses to Discrete Fourier spectra
 *  Version:    $Id: $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#ifndef __DAFFTRANSFORMER_IR2DFT_H__
#define __DAFFTRANSFORMER_IR2DFT_H__

#include <DAFFDefs.h>
#include <DAFFContentDFT.h>
#include <DAFFContentIR.h>
#include <DAFFPropertiesImpl.h>
#include <cmath>

//! Window functions
enum {
	DAFF_WINDOW_NONE = 0,	//!< No windowing
	DAFF_WINDOW_HANN,		//!< Hann window
	// TODO: Add more by request ...
};

//! Transformer from impulse responses (IR) to Discrete Fourier spectra (DFT)
/**
 * This class is associated a DAFFContentIR instance and transforms this data
 * into Discrete Fourier spectra (DFT) using real-data Fast Fourier Transforms.
 * Thereby is provides a DAFFContentDFT view on DAFFContentIR datasets.
 * The views lifetime is limited to the lifetime of the transformed content.
 * Additional parameters can be changed (e.g. window function).
 */

class DAFF_API DAFFTransformerIR2DFT {
public:
	//! Default constructor
	DAFFTransformerIR2DFT();

	//! Initializing constructor
	/**
	 * \param pInputData	Input data
	 * \param iWindow		Window type [optional, default: none]
	 * \param bTransform	Transform the data directly? [optional, default: yes]
	 */
	DAFFTransformerIR2DFT(const DAFFContentIR* pInputContent, int iWindowFunction=DAFF_WINDOW_NONE, bool bTransform=true);

	//! Destructor
	virtual ~DAFFTransformerIR2DFT();

	//! Set input content
	/**
	 * Sets impulse response input data.
	 * 
	 * \param pInputContent	Input content (impulse responses)
	 * \param bTransform	Transform the data directly? [optional, default: yes]
	 */
	void setInputContent(const DAFFContentIR* pInputContent, bool bTransform=true);

	//! Get transformed output data
	/**
	 * \note This method returns NULL if not input data has been assigned
	 */
	DAFFContentDFT* getOutputContent() const;

	//! Returns the window function
	int getWindowFunction() const;

	//! Set the window function
	/**
	 * Sets windowing for the impulse responses.
	 * 
	 * \param iWindowFunction	Window function
	 * \param bTransform		Transform the data directly? [optional, default: yes]
	 */
	void setWindowFunction(int iWindowFunction, bool bTransform=true);

	//! Free memory
	/**
	 * This function clears all memory allocated for the DFT representation.
	 * Afterwards getOutputData returns NULL, until transform is called again.
	 */
	void clear();

	//! Transform the data
	/**
	 * This method triggers the transformation of the input data (manually)
	 * and transforms all time-domain impulse responses into the
	 * Discrete Fourier domain.
	 */
	void transform();

private:
	// Inner content interface realization
	class ContentDFTRealization : public DAFFContentDFT {
	public:
		ContentDFTRealization(DAFFTransformerIR2DFT* pParent, const DAFFContentIR* pInputContent)
			: m_pParent(pParent), m_pInputContent(pInputContent)
		{
			m_oProps = *(pInputContent->getProperties());
			m_oProps.m_iContentType = DAFF_DFT_SPECTRUM;
		};
		
		virtual ~ContentDFTRealization() {};

		// --= Interface "DAFFContentDFT" =--

		int getTransformSize() const {
			// Same as filter length in input content
			return m_pInputContent->getFilterLength();
		}

		int getNumDFTCoeffs() const {
			// The input data is always real valued,
			// resulting in complex-conjugate symmetric DFT spectra.
			// Their number of symmetric coefficients is given by
			// ceil( (K+1) / 2 )
			
			int K = getTransformSize();
			return (int) ceil( ((double) K + 1)/2 );
			// TODO: Simplify formula to integer arithmetic
		}

		bool isSymetric() const {
			return true;
		}

		double getSamplerate() const {
			return m_pInputContent->getSamplerate();
		}

		double getFrequencyBandwidth() const {
			return getSamplerate() / (double) getTransformSize();
		}

		float getOverallMagnitudeMaximum() const {
			return m_pParent->getOverallMagnitudeMaximum();
		}

		int getDFTCoeff(int iRecordIndex, int iChannel, int iDFTCoeff, float& fReal, float& fImag) const {
			return m_pParent->getDFTCoeff(iRecordIndex, iChannel, iDFTCoeff, fReal, fImag);
		}

		int getDFTCoeffs(int iRecordIndex, int iChannel, float* pfDest) const {
			return m_pParent->getDFTCoeffs(iRecordIndex, iChannel, pfDest);
		}

		// --= Interface "DAFFContent" =--

		// This interface is completely delegated to the input content of the transform

		DAFFReader* getParent() const {
			return m_pInputContent->getParent();
		}

		const DAFFPropertiesImpl* getProperties() const {
			return reinterpret_cast<const DAFFPropertiesImpl*>(&m_oProps);
			//return m_pInputContent->getProperties();
		}

		const DAFFMetadata* getRecordMetadata(int iRecordIndex) const {
			return m_pInputContent->getRecordMetadata(iRecordIndex);
		}

		int getRecordCoords(int iRecordIndex, int iView, float& fAngle1, float& fAngle2) const {
			return m_pInputContent->getRecordCoords(iRecordIndex, iView, fAngle1, fAngle2);
		}

		void getNearestNeighbour(int iView, float fAngle1, float fAngle2, int& iRecordIndex) const {
			m_pInputContent->getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex);
		}

		void getNearestNeighbour(int iView, float fAngle1, float fAngle2, int& iRecordIndex, bool& bOutOfBounds) const {
			m_pInputContent->getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex, bOutOfBounds);
		}

		void getCell(int iView, const float fAngle1, const float fAngle2, DAFFQuad& qIndices) const {
			m_pInputContent->getCell(iView, fAngle1, fAngle2, qIndices);
		}

		void transformAnglesD2O(const float fAlpha,
								const float fBeta,
								float& fAzimuth,
								float& fElevation) const {
			m_pInputContent->transformAnglesD2O(fAlpha, fBeta, fAzimuth, fElevation);
		}

		void transformAnglesO2D(const float fAzimuth,
								const float fElevation,
								float& fAlpha,
								float& fBeta) const {
			m_pInputContent->transformAnglesO2D(fAzimuth, fElevation, fAlpha, fBeta);
		}

	private:
		DAFFTransformerIR2DFT* m_pParent;
		const DAFFContentIR* m_pInputContent;
		DAFFPropertiesImpl m_oProps;
	};

	const DAFFContentIR* m_pInputContent;		// Assigned input data
	ContentDFTRealization* m_pOutputContent;	// Transformed output data
	int m_iWindowFunction;						// Window function
	float* m_pfBuf;								// Buffer for transformed DFT spectra
	int m_iNumDFTCoeffs;						// Number of symmetric DFT coefficients
	int m_iElementSize;							// Size of transformed DFT spectrum (number of elements)
	float m_fOverallMagnitudePeak;				// Maximum magnitude over all records/channels

	// Called by inner content class
	float getOverallMagnitudeMaximum() const;
	int getDFTCoeff(int iRecordIndex, int iChannel, int iDFTCoeff, float& fReal, float& fImag) const;
	int getDFTCoeffs(int iRecordIndex, int iChannel, float* pfDest) const;
};

#endif // __DAFFTRANSFORMER_IR2DFT_H__
