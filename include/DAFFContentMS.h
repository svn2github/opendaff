/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef IW_DAFF_CONTENTMS
#define IW_DAFF_CONTENTMS

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

class DAFF_API DAFFContentMS: public DAFFContent
{
public:
	inline virtual ~DAFFContentMS() {};

	//! Returns the number of support points (frequencies)
	/*
	* @return Number of frequencies
	*/
	virtual int getNumFrequencies() const=0;

	//! Retrieves the frequencies [in Hertz] at which values are defined
	/*
	* @return Vector of frequencies
	*/
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
	 * \param [in] iRecordIndex  Record index (direction)
	 * \param [in] iChannel      Channel index
	 * \param [out] pfDest		Destination buffer (size >= number of frequencies)
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 *
	 * \note The magnitude values are factors (no decibel).
	 */
	virtual int getMagnitudes(int iRecordIndex, int iChannel, float* pfDest) const=0;

	//! Retrieves a single magnitude coefficient
	/**
     * This method retrives the magnitude coefficient for the given direction (record index),
	 * channel and frequency index. It stores the value in the supplied destination. 
	 *
	 * \param [in] iRecordIndex  Record index (direction)
	 * \param [in] iChannel      Channel index
	 * \param [in] iFreqIndex    Frequency index
	 * \param [out] fMag			Magnuitude value
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 *
	 * \note The magnitude value is a factor (no decibel).
	 */
	virtual int getMagnitude(int iRecordIndex, int iChannel, int iFreqIndex, float& fMag) const=0;
};

#endif // IW_DAFF_CONTENTMS
