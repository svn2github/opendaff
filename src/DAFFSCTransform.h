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
 *  File:		DAFFSCTransform.h
 *  Purpose:	Spherical coordinate transformations headerfile
 *  Version:    $Id: DAFFSCTransform.h,v 1.5 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */
#ifndef __DAFFSCTRANSFORM_H__
#define __DAFFSCTRANSFORM_H__

#include <DAFFDefs.h>

/**
 * This library-internal class realizes transformations of spherical coordinates.
 * It implements the rather complex rotation yaw-pitch-roll convention
 * expressed in spherical coordinates.
 */

class DAFFSCTransform {
public:
	// Constructors
	DAFFSCTransform();
	DAFFSCTransform(const DAFFOrientationYPR& orient);

	// Return the OSC->DSC orientation (yaw-pitch-roll)
	void getOrientation(DAFFOrientationYPR& orient) const;
	
	// Sets the OSC->DSC orientation (yaw-pitch-roll)
	void setOrientation(const DAFFOrientationYPR& orient);

	// Transform coordinates from OSC -> DSC (including coordinate system rotation)
	void transformOSC2DSC(const float azimuth_in, const float elevation_in,
					      float& alpha_out, float& beta_out) const;

	// Transform coordinates from DSC -> OSC (including coordinate system rotation)
	void transformDSC2OSC(const float alpha_in, const float beta_in,
					      float& azimuth_out, float& elevation_out) const;

private:
	// Cached trigenometric terms of yaw-pitch-roll angles
	class RotationConstants {
	public:
		double t1, t2, t3, t4, t5, t6, t7, t8, t9;

		// Initialize constants (Angles in degrees [°])
		void init(const double yaw, const double pitch, const double roll);
	};

	DAFFOrientationYPR m_orient;	// Orientation
	RotationConstants m_const;		// Rotation constants
	int m_orient_index;				// Index of the orientation for simple cases, -1 otherwise
};

#endif // __DAFFSCTRANSFORM_H__
