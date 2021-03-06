#include <DAFFTransformerIR2DFT.h>

#include <DAFFContentDFT.h>
#include <DAFFContentIR.h>
#include "DAFFPropertiesImpl.h"

#include "Utils.h"

#include <cassert>
#include <algorithm>
#include <fftw3.h>

// Inner content interface realization
class DAFFContentDFTRealization : public DAFFContentDFT
{
public:
	inline DAFFContentDFTRealization(DAFFTransformerIR2DFT* pParent, const DAFFContentIR* pInputContent)
		: m_pParent(pParent), m_pInputContent(pInputContent)
	{
		m_oProps = *(pInputContent->getProperties());
		m_oProps.m_iContentType = DAFF_DFT_SPECTRUM;
	};
		
	inline virtual ~DAFFContentDFTRealization() {};

	// --= Interface "DAFFContentDFT" =--

	inline int getTransformSize() const {
		// Same as filter length in input content
		return m_pInputContent->getFilterLength();
	};

	inline int getNumDFTCoeffs() const {
		// The input data is always real valued,
		// resulting in complex-conjugate symmetric DFT spectra.
		// Their number of symmetric coefficients is given by
		// ceil( (K+1) / 2 )
			
		int K = getTransformSize();
		return (int) ceil( ((double) K + 1)/2 );
		// TODO: Simplify formula to integer arithmetic
	};

	inline bool isSymmetric() const {
		return true;
	};

	inline double getSamplerate() const {
		return m_pInputContent->getSamplerate();
	};

	inline double getFrequencyBandwidth() const {
		return getSamplerate() / (double) getTransformSize();
	};

	inline float getOverallMagnitudeMaximum() const {
		return m_pParent->getOverallMagnitudeMaximum();
	};

	inline int getDFTCoeff(int iRecordIndex, int iChannel, int iDFTCoeff, float& fReal, float& fImag) const {
		return m_pParent->getDFTCoeff(iRecordIndex, iChannel, iDFTCoeff, fReal, fImag);
	};

	inline int getDFTCoeffs(int iRecordIndex, int iChannel, float* pfDest) const {
		return m_pParent->getDFTCoeffs(iRecordIndex, iChannel, pfDest);
	};

	// --= Interface "DAFFContent" =--

	// This interface is completely delegated to the input content of the transform

	inline DAFFReader* getParent() const {
		return m_pInputContent->getParent();
	};

	inline const DAFFPropertiesImpl* getProperties() const {
		return reinterpret_cast<const DAFFPropertiesImpl*>(&m_oProps);
		//return m_pInputContent->getProperties();
	};

	inline const DAFFMetadata* getRecordMetadata(int iRecordIndex) const {
		return m_pInputContent->getRecordMetadata(iRecordIndex);
	};

	inline int getRecordCoords(int iRecordIndex, int iView, float& fAngle1, float& fAngle2) const {
		return m_pInputContent->getRecordCoords(iRecordIndex, iView, fAngle1, fAngle2);
	};

	inline void getNearestNeighbour(int iView, float fAngle1, float fAngle2, int& iRecordIndex) const {
		m_pInputContent->getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex);
	};

	inline void getNearestNeighbour(int iView, float fAngle1, float fAngle2, int& iRecordIndex, bool& bOutOfBounds) const {
		m_pInputContent->getNearestNeighbour(iView, fAngle1, fAngle2, iRecordIndex, bOutOfBounds);
	};

	inline void getCell(int iView, float fAngle1, float fAngle2, DAFFQuad& qIndices) const {
		m_pInputContent->getCell(iView, fAngle1, fAngle2, qIndices);
	};

	inline void transformAnglesD2O(float fAlpha,
							float fBeta,
							float& fAzimuth,
							float& fElevation) const {
		m_pInputContent->transformAnglesD2O(fAlpha, fBeta, fAzimuth, fElevation);
	};

	inline void transformAnglesO2D(float fAzimuth,
							float fElevation,
							float& fAlpha,
							float& fBeta) const {
		m_pInputContent->transformAnglesO2D(fAzimuth, fElevation, fAlpha, fBeta);
	};

private:
	DAFFTransformerIR2DFT* m_pParent;
	const DAFFContentIR* m_pInputContent;
	DAFFPropertiesImpl m_oProps;
};

DAFFTransformerIR2DFT::DAFFTransformerIR2DFT()
: m_pInputContent(NULL), m_pOutputContent(NULL), m_iWindowFunction(DAFF_WINDOW_NONE), m_pfBuf(NULL)
{

}

DAFFTransformerIR2DFT::DAFFTransformerIR2DFT(const DAFFContentIR* pInputContent, int iWindowFunction, bool bTransform)
: m_pInputContent(NULL), m_pOutputContent(NULL), m_iWindowFunction(DAFF_WINDOW_NONE), m_pfBuf(NULL)
{
	setInputContent(pInputContent, bTransform);
	setWindowFunction(iWindowFunction);
}

DAFFTransformerIR2DFT::~DAFFTransformerIR2DFT() {
	clear();
}

void DAFFTransformerIR2DFT::setInputContent(const DAFFContentIR* pInputContent, bool bTransform) {
	m_pInputContent = pInputContent;
	if (bTransform) transform();
}

DAFFContentDFT* DAFFTransformerIR2DFT::getOutputContent() const {
	return m_pOutputContent;
}

int DAFFTransformerIR2DFT::getWindowFunction() const {
	return m_iWindowFunction;
}

void DAFFTransformerIR2DFT::setWindowFunction(int iWindowFunction, bool bTransform) {
	m_iWindowFunction = iWindowFunction;
	//if (m_pOutputContent) m_pOutputData->setWindow(iWindow, bTransform);
}

void DAFFTransformerIR2DFT::clear() {
	delete m_pOutputContent;
	m_pOutputContent = NULL;

	DAFF::free_aligned16(m_pfBuf);
	m_pfBuf = NULL;
}

void DAFFTransformerIR2DFT::transform() {
	// Discard previous DFT representation
	clear();
	
	int iRecords(0);
	int iChannels(0);
	int iFilterLength(0);

	m_fOverallMagnitudePeak = 0;

	// Create new output content instance
	if (m_pInputContent) {
		m_pOutputContent = new DAFFContentDFTRealization(this, m_pInputContent);

		m_iNumDFTCoeffs = m_pOutputContent->getNumDFTCoeffs();
		
		// Number of bytes per DFT spectrum. Aligned on 16-bytes.
		m_iElementSize = m_iNumDFTCoeffs*2;
		int iElementSizeBytes = m_iElementSize*sizeof(float);
		int n = iElementSizeBytes / 16;
		if ((iElementSizeBytes % 16) > 0) n++;
		iElementSizeBytes = n*16;

		// Allocate buffer for DFT spectra
		iRecords = m_pInputContent->getProperties()->getNumberOfRecords();
		iChannels = m_pInputContent->getProperties()->getNumberOfChannels();
		
		m_pfBuf = static_cast<float*>( DAFF::malloc_aligned16( iRecords*iChannels*iElementSizeBytes ) );
	}

	// Transform the contents
	if (m_pOutputContent) {
		iFilterLength = m_pInputContent->getFilterLength();
		float* pfInputData = static_cast<float*>( DAFF::malloc_aligned16( iFilterLength*sizeof(float) ) );
		float* pfWindow = static_cast<float*>( DAFF::malloc_aligned16( iFilterLength*sizeof(float) ) );
		
		fftwf_plan oFFTPlan = fftwf_plan_dft_r2c_1d(iFilterLength,
			                                        pfInputData,
										            reinterpret_cast<fftwf_complex*>( m_pfBuf ),
										            FFTW_ESTIMATE);

		// Create window function
		switch (m_iWindowFunction) {
		case DAFF_WINDOW_HANN:
			{
					// TODO: 
			}
			break;
		}

		// Written using one loop for effective parallelization
		for (int n=0; n<iRecords*iChannels; n++) {
			int iRecord = n/iChannels;
			int iChannel = n%iChannels;

			// Fetch input data
			m_pInputContent->getFilterCoeffs(iRecord, iChannel, pfInputData);

			// Apply window function
			if (m_iWindowFunction != DAFF_WINDOW_NONE)
				for (int i=0; i<iFilterLength; i++) pfInputData[i] *= pfWindow[i];

			// Transform into frequency-domain
			float* pfOutputData = m_pfBuf + (iRecord*iChannels + iChannel)*m_iElementSize;
			fftwf_execute_dft_r2c(oFFTPlan, pfInputData, reinterpret_cast<fftwf_complex*>( pfOutputData ));

			// Determine maximum
			for (int i=0; i<m_iNumDFTCoeffs; i++) {
				float fReal = pfOutputData[2*i+0];
				float fImag = pfOutputData[2*i+1];
				float fMagSq = fReal*fReal + fImag*fImag;
				m_fOverallMagnitudePeak = (std::max)(m_fOverallMagnitudePeak, fMagSq);
			}
		}

		DAFF::free_aligned16(pfInputData);
		DAFF::free_aligned16(pfWindow);

		fftwf_destroy_plan(oFFTPlan);

		m_fOverallMagnitudePeak = sqrt(m_fOverallMagnitudePeak);
	}
}

float DAFFTransformerIR2DFT::getOverallMagnitudeMaximum() const {
	return m_fOverallMagnitudePeak;
}

int DAFFTransformerIR2DFT::getDFTCoeff(int iRecordIndex, int iChannel, int iDFTCoeff, float& fReal, float& fImag) const {
	if (!m_pInputContent) return -1;

	int iRecords = m_pInputContent->getProperties()->getNumberOfRecords();
	int iChannels = m_pInputContent->getProperties()->getNumberOfChannels();

	assert( (iRecordIndex >= 0) && (iRecordIndex < iRecords) );
	assert( (iChannel >= 0) && (iChannel < iChannels) );
	assert( (iDFTCoeff >= 0) && (iDFTCoeff < m_iNumDFTCoeffs) );

	if ((iRecordIndex < 0) || (iRecordIndex >= iRecords)) return -1;
	if ((iChannel < 0) || (iChannel >= iChannels)) return -1;
	if ((iDFTCoeff < 0) || (iDFTCoeff > m_iNumDFTCoeffs)) return -1;

	float* pfData = m_pfBuf + (iRecordIndex*iChannels + iChannel)*m_iElementSize;

	fReal = pfData[2*iDFTCoeff+0];
	fImag = pfData[2*iDFTCoeff+1];

	return 0;
}

int DAFFTransformerIR2DFT::getDFTCoeffs(int iRecordIndex, int iChannel, float* pfDest) const {
	if (!m_pInputContent) return -1;

	int iRecords = m_pInputContent->getProperties()->getNumberOfRecords();
	int iChannels = m_pInputContent->getProperties()->getNumberOfChannels();

	assert( (iRecordIndex >= 0) && (iRecordIndex < iRecords) );
	assert( (iChannel >= 0) && (iChannel < iChannels) );

	if ((iRecordIndex < 0) || (iRecordIndex >= iRecords)) return -1;
	if ((iChannel < 0) || (iChannel >= iChannels)) return -1;

	float* pfData = m_pfBuf + (iRecordIndex*iChannels + iChannel)*m_iElementSize;

	assert( pfDest != 0 );
	memcpy(pfDest, pfData, m_iNumDFTCoeffs*2*sizeof(float));
	return 0;
}
