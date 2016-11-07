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

#ifndef QDAFFVTKWIDGET_H
#define QDAFFVTKWIDGET_H

#include <QObject>
#include <QVTKWidget.h>

#include <DAFF.h>
#include <DAFFViz/DAFFViz.h>

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include <iostream>

class QDAFFVTKWidget : public QVTKWidget
{
    Q_OBJECT

public:
	struct CAnimation
	{
		int iNumFrames;
		
		int iWidth, iHeight;

		bool bYaw, bPitch;
		double dYawStart, dYawEnd;
		double dPitchStart, dPitchEnd;

		bool bFrequencies;
		int iFreqIdxStart, iFreqIdxEnd;

		bool bChannels;
		int iChannelIdxStart, iChannelIdxEnd;

		bool bElevation;
		double dEleStart, dEleEnd;

		inline CAnimation()
		{
			iNumFrames = 75;
			iWidth = 1920;
			iHeight = 1080;

			bYaw = true;
			bPitch = false;
			dYawStart = dPitchEnd = dPitchStart = 0.0f;
			dYawEnd = 360.0f;

			bChannels = bElevation = bFrequencies = false;
		};
	};

	QDAFFVTKWidget( QWidget *parent = Q_NULLPTR );
	~QDAFFVTKWidget();

public slots:

	void ReadDAFF( const DAFFReader* pReader );
	void CloseDAFF();

	void ChangeFrequencyIndex( int iFrequencyIndex );
	void ChangeChannelIndex( int iChannelIndex );
	void ChangeAlpha( double dAlphaDeg );
	void ChangeBeta( double dBetaDeg );
	void ChangePhi( double dPhiDeg );
	void ChangeTheta( double dThetaDeg );

	void ExportScreenshotPNG( QString sFilePath, int iWidth = 1920, int iHeight = 1080 );
	void ExportScrenshotSeriesPNG( QString sExportDirectory, QString sFileBaseName, const CAnimation& );

	void SetCoordinateAssistanceVisible( bool );
	void SetDirectionIndicatorVisible( bool );
	void SetArrowsVisible( bool );
	void SetPolesVisible( bool );
	void SetGridVisible( bool );
	void SetEquatorVisible( bool );
	void SetMeridiansVisible( bool );
	void SetLogScale( bool );
	void SetPhaseColorMap( bool );
	void SetCarpetPlotWarp( bool );
	void SetNormalizeFrequenciesIndividually( bool );
	void SetNormalize( bool );

private:
	vtkSmartPointer< vtkRenderer > m_pRenderer;
	DAFFViz::SGNode* m_pSGRootNode;
	DAFFViz::SphericalCoordinateAssistant* m_pSCA;
	DAFFViz::BalloonPlot* m_pDAFFContentBalloon;
	DAFFViz::CartesianCoordinateAssistant* m_pCCA;
	DAFFViz::SphericalDirectionIndicator* m_pSDI;
	DAFFViz::CarpetPlot* m_pDAFFContentCarpet;
	bool m_bBalloonPhaseColor, m_bCarpetWarp;
	int m_iCarpetScaling, m_iBalloonScaling;
	bool m_bNormalizeFreqsIndiv, m_bNormalize;
};

#endif // QDAFFVTKWIDGET_H
