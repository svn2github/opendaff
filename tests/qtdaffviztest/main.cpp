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

#include "QtDAFFVizTestWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName( "Institute of Technical Acoustics" );
    a.setOrganizationDomain( "akustik.rwth-aachen.de" );
    a.setApplicationName( "DAFFVizQtTest" );
    a.setApplicationDisplayName( "OpenDAFF" );

	QtDAFFVizTestWindow win;
    win.show();

    return a.exec();
}
