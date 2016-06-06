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

#ifndef QDAFFMETADATATABLE_H
#define QDAFFMETADATATABLE_H

#include <QTableView>

#include <DAFF.h>

#include <iostream>

class QDAFFMetadataTableView : public QTableView
{
    Q_OBJECT

public:
	inline QDAFFMetadataTableView( QWidget *parent = Q_NULLPTR )
                : QTableView( parent )
    {
    }

public slots:
    inline void on_readDAFF( const DAFFReader* pReader )
    {
        std::cout << "Metadata reder: " << pReader->getFilename() << std::endl;
    }
};

#endif // QDAFFMETADATATABLE_H
