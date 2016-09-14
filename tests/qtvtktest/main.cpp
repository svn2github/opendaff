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

#include <QApplication>
#include "DAFFVizQtVTKRenderWindow.h"

int main( int argc, char** argv )
{
  QApplication app( argc, argv );
 
  DAFFVizQtVTKRenderWindow win;
  win.show();
 
  return app.exec();
}