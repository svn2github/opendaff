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

#ifndef IW_DAFF_CONTENTIR
#define IW_DAFF_CONTENTIR

#include <DAFFDefs.h>
#include <DAFFContent.h>

//! Impulse response content interface
/**
 * This purely abstract class defines the special interface for
 * impulse response content (time-domain). Each record contains
 * impulse responses (IRs) of a fixed length (number of filter
 * coefficients) defined for each channel.
 *
 * The simple way to access the IR coefficients is to use the
 * method getFilterCoeffs, which delivers the full impulse
 * response for a given direction (record index) and channel.
 *
 * A more advanced method is to only copy and process those 
 * filter coefficients which are effective, meaning that they
 * are non-zero. To determine this effectiv range (start and length)
 * you can employ the method getEffectiveFilterBounds.
 * You can request the effective filter coefficients only using
 * the method getEffectiveFilterCoeffs and thus improve
 * execution speed. For more details please refer to the manual.
 */

class DAFF_API DAFFContentIR : public DAFFContent
{
public:
	inline virtual ~DAFFContentIR() {};

	//! Returns the sampling rate [in Hertz]
	/*
	* @return Sampling rate
	*/
	virtual double getSamplerate() const=0;

	// --= Basic data access =--

	//! Returns the number of filter coefficients
	/**
	 * This function returns the number of data elements each data
	 * record consists of. For impuls response this means the number
	 * of filter coefficients, ergo the filter length.
	 */
	virtual int getFilterLength() const=0;

	//! Retrieves filter coefficients for record and channel
	/**
	 * This method retrieves the full filter impulse response for the given
	 * direction (record index) and channel and stores them in the supplied
	 * destination buffer. The method writes exactly as many filter coefficients
	 * into the buffer as the filter length.
	 *
	 * \param [in] iRecordIndex  Record index (direction)
	 * \param [in] iChannel      Channel index
	 * \param [out] pfDest		Destination buffer (size >= filter length)
	 * \param [in] fGain			Gain factor (optional, default: 1)
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 */
	virtual int getFilterCoeffs(int iRecordIndex, int iChannel, float* pfDest, float fGain=1.0F) const=0;

	//! Adds filter coefficients for record and channel to a given buffer
	/**
	 * This method retrieves the full filter impulse response for the given
	 * direction (record index) and channel and numerically adds them in the supplied
	 * destination buffer. The method writes exactly as many filter coefficients
	 * into the buffer as the filter is long. A copy of the entire filter in user
	 * code can be avoided using the add filter funcionality. Extensive use of 
	 * record requests can be accelerated by this method.
	 *
	 * \param [in] iRecordIndex  Record index (direction)
	 * \param [in] iChannel      Channel index
	 * \param [out] pfDest		Destination buffer (size >= filter length)
	 * \param [in] fGain			Gain factor (optional, default: 1)
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 */
	virtual int addFilterCoeffs(int iRecordIndex, int iChannel, float* pfDest, float fGain=1.0F) const=0;

	// --= Low-level data access =--

	//! Returns the minimal effective filter offset over all records
	/**
	 * This methods returns the minimal effective filter offset over all records,
	 * ergo the smallest offset you could be faced with among all records.
	 *
	 * This value is helpful if you for instance want to ignore the sound runtime
	 * (leading zeros) and compensate it.
	 */
	virtual int getMinEffectiveFilterOffset() const=0;

	//! Returns the maximum effective filter length over all records
	/**
	 * This methods returns the maximum effective filter length over all records,
	 * ergo the greatest length you could be faced with among all records.
	 *
	 * This value is helpful for buffer dimensioning, when using the low-level interface.
	 */
	virtual int getMaxEffectiveFilterLength() const=0;

	//! Retrieves the offset and length of the effective part of a filter
	/**
	 * This method retrieves the start (offset) and length of the effective filter zone
	 * for the given direction (record index) and channel.
	 *
	 * \param [in] iRecordIndex  Record index (direction) [in]
	 * \param [in] iChannel      Channel index [in]
	 * \param [out] iOffset		Effective filter offset [out]
	 * \param [out] iLength		Effective filter length [out]
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 *
	 * \sa getEffectiveFilterCoeffs
	 */
	virtual int getEffectiveFilterBounds(int iRecordIndex, int iChannel, int& iOffset, int& iLength) const=0;

	//! Retrieves effective filter coefficients for record and channel
	/**
	 * This method retrieves the only the effective (non-zero) filter impulse coefficients
	 * for the given direction (record index) and channel and stores them in the supplied
	 * destination buffer. The method only writes as many filter coefficients
	 * into the buffer as this very filter's effective length. This length can be determined
	 * using the method getEffectiveFilterBounds. The method starts to write into the
	 * destination buffer at position 0 (but not the effective filter offset).
	 *
	 * \param [in] iRecordIndex  Record index (direction)
	 * \param [in] iChannel      Channel index
	 * \param [out] pfDest		Destination buffer (size >= effective filter length)
	 * \param [in] fGain			Gain factor (optional, default: 1)
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 *
	 * \sa getEffectiveFilterBounds
	 */
	virtual int getEffectiveFilterCoeffs(int iRecordIndex, int iChannel, float* pfDest, float fGain=1.0F) const=0;
	
	//! Adds effective filter coefficients for record and channel to a given buffer
	/**
	 * This method retrieves effective filter impulse response only (non-zero sample range)
	 * for the given direction (record index) and channel and numerically adds them in the supplied
	 * destination buffer. The method writes exactly as many filter coefficients
	 * into the buffer as the effective filter is long. This length can be requested
	 * using the getEffectiveFilterBounds() method. A copy of the entire filter in user
	 * code can be avoided using the add filter funcionality. Extensive use of 
	 * record requests can be accelerated by this method. The method starts to write into the
	 * destination buffer at position 0 (but not the effective filter offset).
	 *
	 * \param iRecordIndex  Record index (direction)
	 * \param iChannel      Channel index
	 * \param pfDest		Destination buffer (size >= effective filter length)
	 * \param fGain			Gain factor (optional, default: 1)
	 *
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 *
	 * \sa getEffectiveFilterBounds
	 */
	virtual int addEffectiveFilterCoeffs(int iRecordIndex, int iChannel, float* pfDest, float fGain=1.0F) const=0;

	//! Get overall peak value
	/**
	  * @return Overall peak value
	  */
	virtual float getOverallPeak()=0;
};

#endif // IW_DAFF_CONTENTIR
