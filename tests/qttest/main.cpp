/*
* -------------------------------------------------------------------------------------
*
*  OpenDAFF - A free, open source software package for directional audio data
*  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
*
*  Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
*
*  ------------------------------------------------------------------------------------
*
*/

#include "DAFFVizQtTestWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName( "Institute of Technical Acoustics" );
    a.setOrganizationDomain( "akustik.rwth-aachen.de" );
    a.setApplicationName( "DAFFVizQtTest" );
    a.setApplicationDisplayName( "OpenDAFF" );

    DAFFVizQtWindow w;
    w.show();

    return a.exec();
}
