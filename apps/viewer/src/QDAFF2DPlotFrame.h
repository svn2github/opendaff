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

#ifndef QDAFF2DPLOTFRAME_H
#define QDAFF2DPLOTFRAME_H

#include <QFrame>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QOpenGLWidget>

#include <DAFF.h>

#include <iostream>

class QDAFF2DPlotFrame : public QFrame
{
    //Q_OBJECT

public:
	inline QDAFF2DPlotFrame( QWidget *parent = Q_NULLPTR )
		: QFrame( parent )
	{
		QGraphicsView* pView = new QGraphicsView( new QGraphicsScene(), this );
    }

public slots:
    inline void ReadDAFF( const DAFFReader* pReader )
    {
        std::cout << "2D: " << pReader->getFilename() << std::endl;

    }
};

#endif // QDAFF2DPLOTFRAME_H
