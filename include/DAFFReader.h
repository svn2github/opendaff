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
 *  File:		DAFFReader.h
 *  Purpose:	File reader interface definition
 *  Version:    $Id: DAFFReader.h,v 1.7 2010/03/11 12:49:27 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *              Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)
 *
 */

#ifndef __DAFFREADER_H__
#define __DAFFREADER_H__

#include <DAFFDefs.h>
#include <string>

// Forward declarations
class DAFFContent;
class DAFFMetadata;
class DAFFProperties;

//! Reader interface for DAFF files
/**
 * This purely abstract class defines the reader interface for DAFF files.
 * Instances of the class must be created using the factory method create()
 *
 * This is only a brief summary of the basics of DAFF. If you are new to DAFF we recommend
 * visiting the project website at http://www.opendaff.org to get in touch with DAFF. It
 * comes with detailed explanations, nice and supportive figures and tables and
 * some little tutorials and code examples.
 * 
 *
 *		Agreements
 * 
 * All angles in the DAFF Reader Interface and all angles in a daff file are in gradients [°]!
 * If not marked otherwise, we use right-hand rotation. 
 * The default coordinate system and it's cartesian representation:  * 
 *		- front is negative Z axis
 *		- up    is positive X axis 
 *		- left  is positive Y axis
 *		- right is negative Y axis
 *		- down  is negative X axis
 *		- back  is positive Z axis
 *
 * The default coordinate system and it's polar representation:
 * for data view:
 *		- alpha is the angle on horizontal plane beginning at front direction with 0°
 *		  and ends with 360° (not included) with right-hand rotation around X axis.
 *		- beta is the angle on vertical plane beginning at south pole (down direction, south pole)
 *		  whith 0° and ends at 180° (included) (up direction, north pole) with left-hand rotation
 *		  around Y axis (or right-hand rotation around negative Y axis!).
 *		  The front direction is represented with 90°.
 * for object view:
 *		- phi is the angle on horizontal plane beginning at -180° (not included) (back direction) 
 *		  and ends with +180° (included) (also back direction) in right-hand rotation around X axis.
 *		  The front direction is represented with 0°
 *		- beta is the angle on vertical plane beginning at down direction (south pole) with -90° (included)
 *		  and ends with +90° (up direction, north pole) (included) with right-hand rotation.
 *		  The front direction is represented with 0°.
 *
 * The orientation of an object is defined by its yaw, pitch and roll rotation according to the specification
 * used in aerospace applications projected on our default coordinate system:
 *		- Yaw rotation is right-hand rotation around X axis
 *		- Pitch rotation is left-hand rotation around Y axis
 *		- Roll rotation is left-hand rotation around Z axis
 * An object with orientation (0°, 0°, 0°) has its front direction equally to our defined front direction.
 *
 *
 *		Coordinate systems
 *
 * OpenDAFF differentiates heavily between the data view and the object view. The data
 * view has its own coordinate system (Data Spherical Coordinate system, DSC) represented by the
 * angles alpha and beta. It describes in which way your data has been recorded, measured or generated.
 * The object view uses the azimuth and elevation nominclature with phi and theta angles for representation 
 * of the object (Object Spherical Coordinate system, OSC). It is invented to access your data
 * without bothering how it is saved, you simply request a point on the sphere grid of your object
 * and the transformation from object view to data view is covered by the library. Both the data and the object
 * coordinate system are connected by the orientation which is defined by yaw, pitch and roll
 * angles once in the daff file header block. If you are only using daff files you will only need
 * the object view. If you are generating daff files from your records you have to define the correct
 * orientation once (according to you setup) and never have to think about it anymore.
 *
 * 
 *		Reference parameter
 * 
 * A daff file version 0.1 can only save one record set of a fixed distance. If not specified, it will
 * assume a distance of 1 meter for simplification matters. Anyway it's up to your software application
 * if you use multiple distances and how you pay respect on displacement.
 * Same applies for sound pressure level reference.
 *
 * 
 *		Interpolation
 *
 * When using the methods getNearestNeighbour or getCell, the library will always work on data view 
 * coordinate system. If you are using the object view (which is recommended in any case) it will
 * transform from OSC to DSC first and then interpolate the requested angular pair or cell and then
 * return the appripriate index of the record. This can lead to some difficulties especially if
 * the data sphere and the object sphere do not have the poles at same places. E.g. if you have recorder
 * head-related impulse responses with the poles at the up and down direction but you change the direction
 * so that the poles of the object are lying at front/back direction you will get bigger errors in
 * that areas because the grids do not match. Be aware of that and, if possible, simply avoid it :)
 *
 *
 *		Definitions
 *
 * It is generally a good idea to have a look at the "DAFFDefs.h" header file. Here you encounter a couple
 * of codes which we recommend to use when working with OpenDAFF, especially this will lead to a greater
 * robustness of your code with later versions of DAFF. Codes available:
 *		- error codes
 *		- views
 *		- quantization modes
 *		- content types
 *
 */

class DAFF_API DAFFReader {
public:
	//! Factory method
	/**
	 * This method must be used to create reader instances
	 */
	static DAFFReader* create();

	//! Destructor
	virtual ~DAFFReader();

	// --= File handling =--

	//! Returns wheather a file is opened
	virtual bool isFileOpened() const=0;

	//! Opens a DAFF file for reading
	/**
	 * This method opens the given DAFF file for reading and
	 * loads all of its data into the memory.
	 *
	 * \return Errorcode (0 => No error)
	 */
	virtual int openFile(const std::string& sFilename)=0;

	//! Closes an opened DAFF file
	/**
	 * \return Errorcode (0 => No error)
	 */
	virtual void closeFile()=0;

	//! Returns the name of the opened DAFF file
	virtual std::string getFilename() const=0;


	// --= Properties =--

	//! Returns the DAFF version of the file format
	virtual int getFileFormatVersion() const=0;

	//! Returns the content type
	virtual int getContentType() const=0;

	//! Returns the content
	virtual DAFFContent* getContent() const=0;

	//! Returns the metadata
	virtual DAFFMetadata* getMetadata() const=0;

	//! Returns the properties of the file
	virtual const DAFFProperties* getProperties() const=0;


	// --= Utility & helper methods =--

	//! Returns string with information about the reader
	virtual std::string toString() const=0;	
};

#endif // __DAFFREADER_H__
