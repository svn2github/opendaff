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

#ifndef IW_DAFF_CONTENTPS
#define IW_DAFF_CONTENTPS

#include <DAFFDefs.h>
#include <DAFFContent.h>
#include <vector>

//! Phase spectrum content interface
/**
 * This purely abstract class defines the special interface for
 * phase spectrum content (frequency-domain). Each record contains
 * for each channel a fixed number of phase coefficients defined
 * over a common support of frequencies.
 *
 * The number and values of the support frequencies can be determined
 * using the methods getNumFrequencies and getFrequencies.
 * The phase coefficients can be retrieved using the method getPhases.
 */

class DAFF_API DAFFContentPS: public DAFFContent
{
public:
	inline virtual ~DAFFContentPS() {};

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

	// --= Data access =--

	//! Retrieves phase coefficients
	/**
     * This method retrives the phase coefficients for the given direction (record index)
	 * and channel and stores them in the supplied destination buffer. The method writes exactly
	 * as many coefficients as there are support frequencies. Their number can be determined
	 * using the method getNumFrequencies.
	 *
	 * \param [in] iRecordIndex  Record index (direction)
	 * \param [in] iChannel      Channel index
	 * \param [out] pfDest		Destination buffer (size >= number of frequencies)
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 */
	virtual int getPhases(int iRecordIndex, int iChannel, float* pfDest) const=0;
};

#endif // IW_DAFF_CONTENTPS
