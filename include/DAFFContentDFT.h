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
 *  File:		DAFFContentMS.h
 *  Purpose:	Magnitude spectra content interface definition
 *  Version:    $Id: DAFFContentMS.h,v 1.8 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#ifndef __DAFFCONTENTDFT_H__
#define __DAFFCONTENTDFT_H__

#include <DAFFDefs.h>
#include <DAFFContent.h>
#include <vector>

//! Discrete Fourier spectrum content interface
/**
 * This purely abstract class defines the special interface for
 * discrete Fourier spectrum content (frequency-domain). Each record contains
 * for each channel a fixed number of complex-valued DFT coefficients,
 * stored as real and imaginary part.
 *
 * The number and values of the support frequencies can be determined
 * using the methods getNumFrequencies and getFrequencies.
 * The magnitude coefficients can be retrieved using the method getMagnitudes.
 */

class DAFF_API DAFFContentDFT: public DAFFContent {
public:
	virtual ~DAFFContentDFT() {};

	//! Returns the size of the transform (number of overall DFT coefficients)
	/**
	 * This functions returns the overall number of complex-valued DFT coefficients.
	 * Note: For real-valued time-domain data, this also includes the symetric coefficients.
	 */
	virtual int getTransformSize() const=0;

	//! Returns the number of DFT coefficients
	/**
	 * This functions returns the actually stored number of complex-valued DFT coefficients.
	 * Note: For real-valued time-domain data, this includes only the non-symetric coefficients.
	 * Examplarily, for a transform size of N the function returns floor(N/2)+1.
	 * For arbitrary complex-valued time-domain data the returned number equals the
	 * transform size.
	 */
	virtual int getNumDFTCoeffs() const=0;

	//! Returns wheather the spectrum is complex-conjugated symetric
	virtual bool isSymetric() const=0;

	//! Returns the correspondig sampling rate [in Hertz]
	virtual double getSamplerate() const=0;

	//! Returns the frequency resolution [in Hertz]
	virtual double getFrequencyBandwidth() const=0;

	//! Returns the overall greatest magnitude value
	/**
	 * This method returns the greatest magnitude value of the
	 * DFT coefficients over all directions, channels and frequencies.
	 */
	virtual float getOverallMagnitudeMaximum() const=0;

	// --= Data access =--

	//! Retrieve a single magnitude coefficient
	/**
     * This method retrives a single complex-valued DFT coefficient for the given direction (record index)
	 * and channel and stores them in the supplied destination variable.
	 *
	 * \param iRecordIndex  Record index (direction)
	 * \param iChannel      Channel index
	 * \param iDFTCoeff     DFT coefficient index
	 * \param fDestReal		Real part
	 * \param fDestImag		Imaginary part
	 *
	 * \return 0 on success, errorcode otherwise
	 */
	virtual int getDFTCoeff(int iRecordIndex, int iChannel, int iDFTCoeff, float& fReal, float& fImag) const=0;

	//! Retrieves magnitude coefficients
	/**
     * This method retrives the complex-valued DFT coefficients for the given direction (record index)
	 * and channel and stores them in the supplied destination buffer. The method writes exactly
	 * as many complex-valued coefficients as the function getNumDFTCoeffs() returns.
	 *
	 * The output storage scheme is: pfDest = (Re[0], Im[0], Re[1], Im[1], ...)
	 *
	 * \param iRecordIndex  Record index (direction)
	 * \param iChannel      Channel index
	 * \param pfDest		Destination buffer (size >= 2*getNumDFTCoeffs())
	 *
	 * \return 0 on success, errorcode otherwise
	 */
	virtual int getDFTCoeffs(int iRecordIndex, int iChannel, float* pfDest) const=0;
};

#endif // __DAFFCONTENTDFT_H__
