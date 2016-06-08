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
//#include <qwt_plot.h>


#include <DAFF.h>

#include <iostream>
#include "QBoxLayout"

class QDAFF2DPlot : public QFrame
{
    Q_OBJECT

public:
	inline QDAFF2DPlot( QWidget *parent = Q_NULLPTR )
        : QFrame( parent )
	{
		//QwtPlot* plot = new QwtPlot( this );

		QHBoxLayout *layout = new QHBoxLayout( this );
		layout->setContentsMargins( 0, 0, 0, 0 );
		//layout->addWidget( plot );

    }

public slots:
    inline void on_readDAFF( const DAFFReader* pReader )
    {
        std::cout << "2D: " << pReader->getFilename() << std::endl;

    }
};

#endif // QDAFF2DPLOT_H
