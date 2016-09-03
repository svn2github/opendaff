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

	void CloseDAFF();
	void ReadDAFF( const DAFFReader* pReader );
	void ChangeFrequencyIndex( int iFrequencyIndex );
	void ChangeChannelIndex( int iChannelIndex );
	void ChangeAlpha( double dAlphaDeg );
	void ChangeBeta( double dBetaDeg );
	void ExportScreenshotPNG( QString sFilePath );
	void SetCoordinateAssistanceEnabled( bool bClicked );
	void SetDirectionIndicatorEnabled( bool bClicked );
};

#endif // QDAFFVTKWIDGET_H
