/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
 *
 *  Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
 *
 *  ------------------------------------------------------------------------------------
 *
 *  Visit the OpenDAFF homepage: http://www.opendaff.org
 *
 *  ------------------------------------------------------------------------------------
 *
 *  License and warranty notes
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
 *  ------------------------------------------------------------------------------------
 *
 */

#include <DAFFDefs.h>

#include <DAFFContent.h>
#include <DAFFMetadata.h>
#include <DAFFProperties.h>
#include <DAFFReader.h>

#include <DAFFContentIR.h>
#include <DAFFContentMS.h>
#include <DAFFContentPS.h>
#include <DAFFContentMPS.h>
#include <DAFFContentDFT.h>

#include <DAFFUtils.h>


/*!

@mainpage

@section intro_sec Introduction

The OpenDAFF project is free, open source software package for directional audio data. It is distributed under the terms of the GNU Lesser Public License (LGPLv3) with an exception that permits static linking of the *Original Work* for commercial use.
The official website can be found at http://www.opendaff.org

@section license_sec License and warranty notes

OpenDAFF is free software, distributed under the terms of the GNU Lesser General Public License (LGPL) version 3. You can redistribute it and/or modify it under the terms of the GNU Lesser General Public License (LGPL) version 3, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section daff_content_sec DAFF specic content types

DAFF provides a list of specific content types that can be found in #DAFF_CONTENT_TYPES with their respective class definitions:

@see DAFFContent 
@see DAFFContentDFT 
@see DAFFContentIR 
@see DAFFContentMPS 
@see DAFFContentMS 
@see DAFFContentPS

@section daff_coords_sec Coordinate system

Everyone has an own perspective when it comes to coordinate systems. And because there is no best solution, DAFF can not provide a way that satisfies everyone. But at the end of the day one definition has to be settled when designing a extensive spatial and/or directional audio data format.
DAFF uses the common OpenGL Cartesian coordinate system, where the default view direction of an object is the -Z axis and the up direction points along +Y axis. For orientations, say a right-handed yaw-pith-roll definition, the euler angles are based on this coordinate system. This may appear odd to Matlab users, which might prefer a view direction along +X and an up direction along +Z axis but is perfectly normal for users from the Virtual Reality sector since it is widely used for 3D visualization.
DAFF also uses a common view on the directional data, the object view, with a spherical coordinate system where the front direction is defined by the angular pair (phi,theta) = (0,0) and the up direction is (anyvalue,pi/2). Phi is a right-hand azimuthal rotation around the up vector (+Y) in the range of [0,2pi). Theta is the elevation angle with a range of [-pi/2,pi/2]. Wherever possible, DAFF will use function and variable names that indicate the units of angles (rad or grad), but if not stated grad values (may be negative) can be expected.
However, DAFF also partly supports another view on directional data that is represented by a spatial sampling of equi-angular grids, the so-called data view. The object view starts at south pole with the angular pair (alpha,beta) = (0,0) and ends at north pole with the pair (0,pi). To lock the rotation between object and data view the creator of the DAFF content has to define a rotation that will guarantee that the object view will _always_ the front direction at angle (0,0) and up direction at (x,pi/2).

@see #DAFF_VIEWS

*/
