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
	const DAFFContentIR* m_pInputContent;		// Assigned input data
	DAFFContentDFT* m_pOutputContent;			// Transformed output data
	int m_iWindowFunction;						// Window function
	float* m_pfBuf;								// Buffer for transformed DFT spectra
	int m_iNumDFTCoeffs;						// Number of symmetric DFT coefficients
	int m_iElementSize;							// Size of transformed DFT spectrum (number of elements)
	float m_fOverallMagnitudePeak;				// Maximum magnitude over all records/channels

	// Called by inner content class
	float getOverallMagnitudeMaximum() const;
	int getDFTCoeff(int iRecordIndex, int iChannel, int iDFTCoeff, float& fReal, float& fImag) const;
	int getDFTCoeffs(int iRecordIndex, int iChannel, float* pfDest) const;

	friend class DAFFContentDFTRealization;
};

#endif // __DAFFTRANSFORMER_IR2DFT_H__
