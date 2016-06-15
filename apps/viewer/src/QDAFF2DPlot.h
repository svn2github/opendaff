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
#include <QOpenGLWidget>

#include <DAFF.h>

#include <iostream>

class QDAFF2DPlot : public QGraphicsView
{
    Q_OBJECT

public:
	inline QDAFF2DPlot( QWidget *parent = Q_NULLPTR )
		: QGraphicsView( new QGraphicsScene(), parent )
	{
		setScene( new QGraphicsScene() );
		QGraphicsRectItem *rect = scene()->addRect( QRectF( 0, 0, 100, 100 ) );
		QGraphicsItem *item = scene()->itemAt( 50, 50, QTransform() );
		
		//setViewport( new QOpenGLWidget( this ) );
		setBackgroundBrush( QBrush( Qt::darkBlue ) );
    }

public slots:
    inline void ReadDAFF( const DAFFReader* pReader )
    {
        std::cout << "2D: " << pReader->getFilename() << std::endl;

    }
};

#endif // QDAFF2DPLOT_H
