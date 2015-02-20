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

#ifndef __DAFFCONTENTMS_H__
#define __DAFFCONTENTMS_H__

#include <DAFFDefs.h>
#include <DAFFContent.h>
#include <vector>

//! Magnitude spectrum content interface
/**
 * This purely abstract class defines the special interface for
 * magnitude spectrum content (frequency-domain). Each record contains
 * for each channel a fixed number of magnitude coefficients defined
 * over a common support of frequencies.
 *
 * The number and values of the support frequencies can be determined
 * using the methods getNumFrequencies and getFrequencies.
 * The magnitude coefficients can be retrieved using the method getMagnitudes.
 */

class DAFF_API DAFFContentMS: public DAFFContent {
public:
	virtual ~DAFFContentMS() {};

	//! Returns the number of support points (frequencies)
	virtual int getNumFrequencies() const=0;

	//! Retrieves the frequencies [in Hertz] at which values are defined
	virtual const std::vector<float>& getFrequencies() const=0;

	//! Returns the overall greatest magnitude value
	/**
	 * This method returns the greatest magnitude value over all
	 * directions, channels and frequencies. It is important to
	 * know this value for instance for visualization.
	 */
	virtual float getOverallMagnitudeMaximum() const=0;

	// --= Data access =--

	//! Retrieves magnitude coefficients
	/**
     * This method retrives the magnitude coefficients for the given direction (record index)
	 * and channel and stores them in the supplied destination buffer. The method writes exactly
	 * as many coefficients as there are support frequencies. Their number can be determined
	 * using the method getNumFrequencies.
	 *
	 * \param iRecordIndex  Record index (direction)
	 * \param iChannel      Channel index
	 * \param pfDest		Destination buffer (size >= number of frequencies)
	 *
	 * \return 0 on success, errorcode otherwise
	 *
	 * \note The magnitude values are factors (no decibel).
	 */
	virtual int getMagnitudes(int iRecordIndex, int iChannel, float* pfDest) const=0;

	//! Retrieves a single magnitude coefficient
	/**
     * This method retrives the magnitude coefficient for the given direction (record index),
	 * channel and frequency index. It stores the value in the supplied destination. 
	 *
	 * \param iRecordIndex  Record index (direction)
	 * \param iChannel      Channel index
	 * \param iFreqIndex    Frequency index
	 * \param fMag			Magnuitude value
	 *
	 * \return 0 on success, errorcode otherwise
	 *
	 * \note The magnitude value is a factor (no decibel).
	 */
	virtual int getMagnitude(int iRecordIndex, int iChannel, int iFreqIndex, float& fMag) const=0;
};

#endif // __DAFFCONTENTMS_H__
