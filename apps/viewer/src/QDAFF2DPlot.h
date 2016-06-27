/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.s
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef QDAFF2DPLOT_H
#define QDAFF2DPLOT_H

#include <QFrame>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtOpenGL\qgl.h>

#include <DAFF.h>

#include <iostream>

class QDAFF2DPlot : public QGraphicsView
{
    Q_OBJECT

public:
	QDAFF2DPlot( QWidget *parent = Q_NULLPTR );

public slots:
     void ReadDAFF( const DAFFReader* pReader );
	 void CloseDAFF();
	 void ChangeRecordIndex( int );
	 void ChangeChannelIndex( int );
private:
	const DAFFReader* m_pReader;

	//help Functions
	void DrawCoordinateSystem();
	void DrawGraph(int);
	void ShowOnlyChannel(int);
	const std::string convertFloat(float);

	//items
	QGraphicsLineItem *m_pXAxis, *m_pYAxis;
	std::vector<QGraphicsLineItem*> m_vpXGrid, m_vpYGrid, m_vpXMarker, m_vpYMarker;
	std::vector<QGraphicsTextItem*> m_vpXMarkerText;
	std::vector<QGraphicsTextItem*> m_vpYMarkerText;
	std::vector<std::vector<QGraphicsLineItem*>> m_vvpGraphs;
	std::vector<QColor> m_voColors;

	//indices
	int m_iChannelIndex, m_iRecordIndex;

	//parameters
	//scene
	const int m_iSceneHeight = 400;
	const int m_iSceneWidth = 800;
	//axis
	const int m_iAxisOffsetLeft = 30; 
	const int m_iAxisOffsetDown = 30;
	const int m_iXAxisLength = 600;
	const int m_iYAxisLength = 300;
	const int m_iTipLength = 10;
	const int m_iTipWidth = 5; //from one end to the other is double this width
	const int m_iTipDistanceX = 50;
	const int m_iTipDistanceY = 50;
	//grid
	const int m_iGridXOffset = 60;
	const int m_iGridYOffset = 60;
	//marker
	const int m_iNXMarker = 11;
	const int m_iNYMarker = 6;
	const int m_iXMarkerLength = 5;
	const int m_iYMarkerLength = 5;
	const int m_iXMarkerTextOffset = 0;
	const int m_iYMarkerTextOffset = 20;
	
};

#endif // QDAFF2DPLOT_H
