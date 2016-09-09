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
	QDAFFVTKWidget( QWidget *parent = Q_NULLPTR );
	~QDAFFVTKWidget();

private:

    vtkSmartPointer< vtkRenderer > m_pRenderer;
	DAFFViz::SGNode* m_pSGRootNode;
	DAFFViz::SphericalCoordinateAssistant* m_pSCA;
	DAFFViz::BalloonPlot* m_pDAFFContentBalloon;
	DAFFViz::CartesianCoordinateAssistant* m_pCCA;
	DAFFViz::CarpetPlot* m_pDAFFContentCarpet;

public slots:

void ReadDAFF( const DAFFReader* pReader );
void CloseDAFF();

	void ChangeFrequencyIndex( int iFrequencyIndex );
	void ChangeChannelIndex( int iChannelIndex );
	void ChangeAlpha( double dAlphaDeg );
	void ChangeBeta( double dBetaDeg );

	void ExportScreenshotPNG( QString sFilePath );

	void SetCoordinateAssistanceVisible( bool );
	void SetDirectionIndicatorVisible( bool );
	void SetArrowsVisible( bool );
	void SetPolesVisible( bool );
	void SetCirclesVisible( bool ) {};
	void SetEquatorVisible( bool ) {};
	void SetMeridianVisible( bool ) {};
};

#endif // QDAFFVTKWIDGET_H
