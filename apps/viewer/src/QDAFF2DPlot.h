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
	QDAFF2DPlot( QWidget *parent = Q_NULLPTR );

public slots:
     void ReadDAFF( const DAFFReader* pReader );
	 void CloseDAFF();
	 void ChangeFrequencyIndex( int );
private:
	const DAFFReader* m_pReader;
};

#endif // QDAFF2DPLOT_H
