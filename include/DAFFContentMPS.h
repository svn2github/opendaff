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

#ifndef IW_DAFF_CONTENTMPS
#define IW_DAFF_CONTENTMPS

#include <DAFFDefs.h>
#include <DAFFContent.h>
#include <vector>

//! Magnitude-phase spectrum content interface
/**
 * This purely abstract class defines the special interface for
 * spectrum content (frequency-domain). Each record contains
 * for each channel a fixed number of magnitude and phase coefficients 
 * defined over a common support of frequencies.
 *
 * The number and values of the support frequencies can be determined
 * using the methods getNumFrequencies and getFrequencies.
 * The magnitude coefficients can be retrieved using the method getMagnitudes.
 * The phase can be retrieved using getPhases. If you want both magnitude 
 * and phase use getCoefficientsMP. You will get the same as number 
 * with a real and imagninary part by getCoefficientsRI.
 */

class DAFF_API DAFFContentMPS: public DAFFContent
{
public:
	inline virtual ~DAFFContentMPS() {};

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
	 * \param iRecordIndex  Record index (direction)
	 * \param iChannel      Channel index
	 * \param pfDest		Destination buffer (size >= number of frequencies)
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
	
	//! Retrieves a single phase coefficient
	/**
     * This method retrives the phase coefficient for the given direction (record index),
	 * channel and frequency index. It stores the value in the supplied destination. 
	 *
	 * \param [in] iRecordIndex  Record index (direction)
	 * \param [in] iChannel      Channel index
	 * \param [in] iFreqIndex    Frequency index
	 * \param [out] fPhase		Phase value
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 *
	 */
	virtual int getPhase(int iRecordIndex, int iChannel, int iFreqIndex, float& fPhase) const=0;

	//! Retrieves coefficients in polar form
	/**
     * This method retrives the coefficients for the given direction (record index)
	 * and channel and stores them in the supplied destination buffers. 
	 * Each coefficient consists of a magnitude and phase which will be returned 
	 * in the destination buffer. The method writes exactly
	 * as many coefficients as there are support frequencies. Their number can be determined
	 * using the method getNumFrequencies.
	 *
	 * The output storage scheme is: pfDest = (Mag[0], Ph[0], Mag[1], Ph[1], ...)
	 *
	 * \param [in] iRecordIndex		Record index (direction)
	 * \param [in] iChannel			Channel index
	 * \param [out] pfDest			Destination buffer (size >= number of frequencies)
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 */
	// TODO: Ausgabe Interleaved
	virtual int getCoefficientsMP(int iRecordIndex, int iChannel, float* pfDest) const=0;
	
	//! Retrieves coefficients in cartesian form
	/**
     * This method retrives the coefficients for the given direction (record index)
	 * and channel and stores them in the supplied destination buffers. 
	 * Each coefficient consists of a real and imaginary part which will be returned 
	 * in the destination buffer. The method writes exactly
	 * as many coefficients as there are support frequencies. Their number can be determined
	 * using the method getNumFrequencies.
	 *
	 * The output storage scheme is: pfDest = (Re[0], Im[0], Re[1], Im[1], ...)
	 *
	 * \param [in] iRecordIndex		Record index (direction)
	 * \param [in] iChannel			Channel index
	 * \param [out] pfDest			Magnitudes destination buffer (size >= number of frequencies)
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 */
	// TODO: Ausgabe Interleaved
	virtual int getCoefficientsRI(int iRecordIndex, int iChannel, float* pfDest) const=0;
};

#endif // IW_DAFF_CONTENTMPS
