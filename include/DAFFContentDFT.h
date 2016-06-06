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

#ifndef IW_DAFF_CONTENTDFT
#define IW_DAFF_CONTENTDFT

#include <DAFFDefs.h>
#include <DAFFContent.h>
#include <vector>

//! Discrete Fourier spectrum content interface
/**
 * This purely abstract class defines the special interface for
 * discrete Fourier spectrum content (frequency-domain). Each record contains
 * for each channel a fixed number of complex-valued DFT coefficients,
 * stored as real and imaginary part.
 *
 * The number and values of the support frequencies can be determined
 * using the methods getNumDFTCoeffs(), getFrequencyBandwidth() and getSamplingrate().
 * The magnitude coefficients can be retrieved using the method getDFTCoeffs().
 */

class DAFF_API DAFFContentDFT: public DAFFContent
{
public:
	inline virtual ~DAFFContentDFT() {};

	//! Returns the size of the transform (number of overall DFT coefficients)
	/**
	 * This functions returns the overall number of complex-valued DFT coefficients.
	 * Note: For real-valued time-domain data, this also includes the symmetric coefficients.
	 *
	 * @return DFT transform size
	 */
	virtual int getTransformSize() const=0;

	//! Returns the number of DFT coefficients
	/**
	 * This functions returns the actually stored number of complex-valued DFT coefficients.
	 * Note: For real-valued time-domain data, this includes only the non-symmetric coefficients.
	 * Examplarily, for a transform size of N the function returns floor(N/2)+1.
	 * For arbitrary complex-valued time-domain data the returned number equals the
	 * transform size.
	 *
	 * @return Number of DFT coefficients
	 */
	virtual int getNumDFTCoeffs() const=0;

	//! Returns whether the spectrum is complex-conjugated symmetric
	/**
	  * @return True, if the DFT spectrum is complex-conjugated symmetric
	  */
	virtual bool isSymmetric() const=0;

	//! Returns the correspondig sampling rate [in Hertz]
	/**
	* @return Sampling rate in Hertz
	*/
	virtual double getSamplerate() const=0;

	//! Returns the frequency resolution [in Hertz]
	/**
	* @return Linear frequency spacing in Hertz
	*/
	virtual double getFrequencyBandwidth() const=0;

	//! Returns the overall greatest magnitude value
	/**
	 * This method returns the greatest magnitude value of the
	 * DFT coefficients over all directions, channels and frequencies.
	 *
	 * @return Absolute maximum magnitude value
	 */
	virtual float getOverallMagnitudeMaximum() const=0;

	// --= Data access =--

	//! Retrieve a single magnitude coefficient
	/**
     * This method retrives a single complex-valued DFT coefficient for the given direction (record index)
	 * and channel and stores them in the supplied destination variable.
	 *
	 * @param [in] iRecordIndex Record index (direction)
	 * @param [in] iChannel     Channel index
	 * @param [in] iDFTCoeff    DFT coefficient index
	 * @param [out] fReal	Real part
	 * @param [out] fImag	Imaginary part
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 */
	virtual int getDFTCoeff(int iRecordIndex, int iChannel, int iDFTCoeff, float& fReal, float& fImag) const=0;

	//! Retrieves magnitude coefficients
	/**
     * This method retrives the complex-valued DFT coefficients for the given direction (record index)
	 * and channel and stores them in the supplied destination buffer. The method writes exactly
	 * as many complex-valued coefficients as the function getNumDFTCoeffs() returns.
	 *
	 * The output storage scheme is: pfDest = (Re[0], Im[0], Re[1], Im[1], ...)
	 *
	 * \param iRecordIndex  Record index (direction)
	 * \param iChannel      Channel index
	 * \param pfDest		Destination buffer (size >= 2*getNumDFTCoeffs())
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 */
	virtual int getDFTCoeffs(int iRecordIndex, int iChannel, float* pfDest) const=0;
};

#endif // IW_DAFF_CONTENTDFT
