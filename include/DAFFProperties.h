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
*/

#ifndef IW_DAFF_PROPERTIES
#define IW_DAFF_PROPERTIES

#include <DAFFDefs.h>

class DAFF_API DAFFProperties
{
public:
	//! Destructor
	inline virtual ~DAFFProperties() {};

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

	//! Indicates whether the data covers the full alpha range [0&deg;, 360&deg;)
	virtual bool coversFullAlphaRange() const=0;

	//! Indicates whether the data covers the full beta range [0&deg;, 180&deg;]
	virtual bool coversFullBetaRange() const=0;

	//! Indicates whether the data covers the full sphere
	virtual bool coversFullSphere() const=0;
};

#endif // IW_DAFF_PROPERTIES
