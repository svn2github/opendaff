/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016-2018 Institute of Technical Acoustics (ITA), RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef IW_DAFF_PROPERTIES
#define IW_DAFF_PROPERTIES

#include <DAFFDefs.h>

//! Properties of a DAFF file that uses regular sphere grids (or parts of a regular grid)
class DAFF_API DAFFProperties
{
public:
	//! Destructor
	inline virtual ~DAFFProperties() {};

	// --= Properties =--

	//! Returns the DAFF version of the file format
	/**
	  * \return File format version number (i.e. 170)
	  */
	virtual int getFileFormatVersion() const=0;

	//! Returns the content type
	/**
	  * \return Content type, one of @DAFF_CONTENT_TYPES
	  */
	virtual int getContentType() const=0;

	//! Returns the quantization of the data elements
	/**
	  * \return Sample quantization, one of @DAFF_QUANTIZATIONS
	  */
	virtual int getQuantization() const=0;

	//! Returns the number of channels
	/**
	  * \return Number of chanels
	  */
	virtual int getNumberOfChannels() const=0;

	//! Returns the overall number of records
	/**
	  * \return Total number of data records (multi channel data sets)
	  */
	virtual int getNumberOfRecords() const=0;

	//! Returns the label of a channel
	virtual std::string getChannelLabel( int iChannel ) const = 0;

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
	virtual void getDefaultOrientation( DAFFOrientationYPR& o ) const = 0;

	//! Sets/alters the default orientation manually
	virtual void setDefaultOrientation()=0;

	//! Returns the current orientation of the object view
	virtual void getOrientation( DAFFOrientationYPR& o ) const = 0;

	//! Sets the current orientation of the object view
	virtual void setOrientation( const DAFFOrientationYPR& o ) = 0;

	// -= Coverage =--------------------------------------

	//! Indicates whether the data covers the full alpha range [0&deg;, 360&deg;)
	virtual bool coversFullAlphaRange() const=0;

	//! Indicates whether the data covers the full beta range [0&deg;, 180&deg;]
	virtual bool coversFullBetaRange() const=0;

	//! Indicates whether the data covers the full sphere
	virtual bool coversFullSphere() const=0;
};

#endif // IW_DAFF_PROPERTIES
