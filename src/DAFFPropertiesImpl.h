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

#ifndef IW_DAFF_PROPERTIESIMPL
#define IW_DAFF_PROPERTIESIMPL

#include <DAFFDefs.h>
#include <DAFFProperties.h>
#include <../src/DAFFSCTransform.h>
#include <cassert>
#include <cstdlib>

//! Implements the interface of DAFFProperties
class DAFF_API DAFFPropertiesImpl : public DAFFProperties
{
public:
	// Default constructor
	inline DAFFPropertiesImpl()
		: m_iFileFormatVersion(0),
		m_iContentType(0),
		m_iQuantization(0),
		m_iNumChannels(0),
		m_iNumRecords(0),
		m_iAlphaPoints(0),
		m_fAlphaStart(0),
		m_fAlphaEnd(0),
		m_fAlphaResolution(0),
		m_iBetaPoints(0),
		m_fBetaStart(0),
		m_fBetaEnd(0),
		m_fBetaResolution(0),
		m_pOrientationDefault(0),
		m_pTrans(0)
	{};

	//! Copy constructor
	inline DAFFPropertiesImpl(const DAFFProperties* pProps)
	{
		assert( pProps != NULL );
		*this = *pProps;
	};

	inline ~DAFFPropertiesImpl()
	{
		delete m_pOrientationDefault;
	};
		
	//! Assignment operator
	inline DAFFPropertiesImpl& operator=(const DAFFProperties& oProps)
	{
		m_iFileFormatVersion = oProps.getFileFormatVersion();
		m_iContentType = oProps.getContentType();
		m_iQuantization = oProps.getQuantization();
		m_iNumChannels = oProps.getNumberOfChannels();
		m_iNumRecords = oProps.getNumberOfRecords();
		m_iAlphaPoints = oProps.getAlphaPoints();
		m_iBetaPoints = oProps.getBetaPoints();
		m_fAlphaStart = oProps.getAlphaStart();
		m_fAlphaEnd = oProps.getAlphaEnd();
		m_fAlphaResolution = oProps.getAlphaResolution();
		m_fBetaStart = oProps.getBetaStart();
		m_fBetaEnd = oProps.getBetaEnd();
		m_fBetaResolution = oProps.getBetaResolution();

		m_pOrientationDefault = new DAFFOrientationYPR;
		oProps.getDefaultOrientation(*m_pOrientationDefault);
		DAFFOrientationYPR* orient = new DAFFOrientationYPR;
		oProps.getOrientation(*orient);
		m_pTrans = new DAFFSCTransform(*orient);
		delete orient;

		std::vector<std::string> m_vChannelLabels;
		for (int i=0; i<oProps.getNumberOfChannels(); i++) 
			m_vChannelLabels.push_back(oProps.getChannelLabel(i));

		return *this;
	};

	// --= Interface "DAFFProperties" =--

	inline virtual int getFileFormatVersion() const
	{
		return m_iFileFormatVersion;
	};

	//! Returns the content type
	inline virtual int getContentType() const
	{
		return m_iContentType;
	};

	//! Returns the quantization of the data elements
	inline virtual int getQuantization() const
	{
		return m_iQuantization;
	};

	//! Returns the number of channels
	inline virtual int getNumberOfChannels() const
	{
		return m_iNumChannels;
	};

	//! Returns the overall number of records
	inline virtual int getNumberOfRecords() const
	{
		return m_iNumRecords;
	};

	//! Returns the label of a channel
	inline virtual std::string getChannelLabel(int iChannel) const
	{
		return m_vChannelLabels[iChannel];
	};

	// -= Alpha angle =----------------------------------

	// Returns the number of measurement points of the alpha range
	inline virtual int getAlphaPoints() const
	{
		return m_iAlphaPoints;
	};

	// Returns the angular resolution of the alpha range
	inline virtual float getAlphaResolution() const
	{
		return m_fAlphaResolution;
	};

	// Returns starting angle of the alpha range
	inline virtual float getAlphaStart() const
	{
		return m_fAlphaStart;
	};

	// Returns ending angle of the alpha range
	inline virtual float getAlphaEnd() const
	{
		return m_fAlphaEnd;
	};

	// Returns the spanned alpha range
	inline virtual float getAlphaSpan() const
	{
		if (m_fAlphaStart <= m_fAlphaEnd)
			return m_fAlphaEnd - m_fAlphaStart;
		else // wrap around 0&deg;
			return 360 - m_fAlphaStart + m_fAlphaEnd;
	};

	// -= Beta angle =----------------------------------

	// Returns the number of measurement points of the beta range
	inline virtual int getBetaPoints() const
	{
		return m_iBetaPoints;
	};

	// Returns the angular resolution of the beta range
	inline virtual float getBetaResolution() const
	{
		return m_fBetaResolution;
	};

	// Returns starting angle of the beta range
	inline virtual float getBetaStart() const
	{
		return m_fBetaStart;
	};

	// Returns ending angle of the beta range
	inline virtual float getBetaEnd() const
	{
		return m_fBetaEnd;
	};

	// Returns the spanned beta range
	inline virtual float getBetaSpan() const
	{
		return m_fAlphaEnd - m_fAlphaStart;
	};

	// -= Orientations =----------------------------------

	//! Returns the default orientation as stored in the file
	inline virtual void getDefaultOrientation(DAFFOrientationYPR& o) const
	{
		o = *m_pOrientationDefault;
	};

	//! Sets/alters the default orientation manually
	inline virtual void setDefaultOrientation()
	{
		m_pTrans->setOrientation(*m_pOrientationDefault);
	};

	//! Returns the current orientation of the object view
	inline virtual void getOrientation(DAFFOrientationYPR& o) const
	{
		m_pTrans->getOrientation(o);
	};

	//! Sets the current orientation of the object view
	inline virtual void setOrientation(const DAFFOrientationYPR& o)
	{
		m_pTrans->setOrientation(o);
	};

	// -= Coverage =--------------------------------------

	//! Indicates wheather the data covers the full alpha range [0&deg;, 360&deg;)
	inline virtual bool coversFullAlphaRange() const
	{
		// full range coverage is given only when alphastart == 0 and alphaend == 360
		if ((m_fAlphaStart == 0) && (m_fAlphaEnd == 360))
			return true;
		else
			return false;
	};

	//! Indicates wheather the data covers the full beta range [0&deg;, 180&deg;]
	inline virtual bool coversFullBetaRange() const
	{
		// full range coverage is given only when betastart == 0 and betaend == 180
		if ((m_fBetaStart == 0) && (m_fBetaEnd == 180))
			return true;
		else
			return false;
	};

	//! Indicates wheather the data covers the full sphere
	inline virtual bool coversFullSphere() const
	{
		return coversFullAlphaRange() && coversFullBetaRange();
	};

	// --= Variables =--

	int	m_iFileFormatVersion, 
		m_iContentType, 
		m_iQuantization, 
		m_iNumChannels,
		m_iNumRecords,
		m_iAlphaPoints,
		m_iBetaPoints;

	float	m_fAlphaStart, 
			m_fAlphaEnd,
			m_fAlphaResolution,
			m_fBetaStart, 
			m_fBetaEnd,
			m_fBetaResolution;

	DAFFOrientationYPR* m_pOrientationDefault;
	DAFFSCTransform* m_pTrans;
	std::vector<std::string> m_vChannelLabels;

};

#endif // IW_DAFF_PROPERTIESIMPL
