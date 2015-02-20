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
 *  File:		DAFFProperties.h
 *  Purpose:	Property interface definition
 *  Version:    $Id: DAFFProperties.h,v 1.6 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de),
 *              Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)
 *
 */

#ifndef __DAFFPROPERTIES_H__
#define __DAFFPROPERTIES_H__

#include <DAFFDefs.h>

class DAFF_API DAFFProperties {
public:
	//! Destructor
	virtual ~DAFFProperties() {};

	// --= Properties =--

	//! Returns the DAFF version of the file format
	virtual int getFileFormatVersion() const=0;

	//! Returns the content type
	virtual int getContentType() const=0;

	//! Returns the quantization of the data elements
	virtual int getQuantization() const=0;

	//! Returns the number of channels
	virtual int getNumberOfChannels() const=0;

	//! Returns the overall number of records
	virtual int getNumberOfRecords() const=0;

	//! Returns the label of a channel
	virtual std::string getChannelLabel(int iChannel) const=0;

	// -= Alpha angle =----------------------------------

	// Returns the number of measurement points of the alpha range
	virtual int getAlphaPoints() const=0;

	// Returns the angular resolution of the alpha range
	virtual float getAlphaResolution() const=0;

	// Returns starting angle of the alpha range
	virtual float getAlphaStart() const=0;

	// Returns ending angle of the alpha range
	virtual float getAlphaEnd() const=0;

	// Returns the spanned alpha range
	virtual float getAlphaSpan() const=0;

	// -= Beta angle =----------------------------------

	// Returns the number of measurement points of the beta range
	virtual int getBetaPoints() const=0;

	// Returns the angular resolution of the beta range
	virtual float getBetaResolution() const=0;

	// Returns starting angle of the beta range
	virtual float getBetaStart() const=0;

	// Returns ending angle of the beta range
	virtual float getBetaEnd() const=0;

	// Returns the spanned beta range
	virtual float getBetaSpan() const=0;

	// -= Orientations =----------------------------------

	//! Returns the default orientation as stored in the file
	virtual void getDefaultOrientation(DAFFOrientationYPR& o) const=0;

	//! Sets/alters the default orientation manually
	virtual void setDefaultOrientation()=0;

	//! Returns the current orientation of the object view
	virtual void getOrientation(DAFFOrientationYPR& o) const=0;

	//! Sets the current orientation of the object view
	virtual void setOrientation(const DAFFOrientationYPR& o)=0;

	// -= Coverage =--------------------------------------

	//! Indicates wheather the data covers the full alpha range [0°, 360°)
	virtual bool coversFullAlphaRange() const=0;

	//! Indicates wheather the data covers the full beta range [0°, 180°]
	virtual bool coversFullBetaRange() const=0;

	//! Indicates wheather the data covers the full sphere
	virtual bool coversFullSphere() const=0;
};

#endif // __DAFFPROPERTIES_H__
