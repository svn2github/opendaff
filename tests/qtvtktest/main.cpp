#include <QApplication>
#include "DAFFVizQtVTKRenderWindow.h"

int main( int argc, char** argv )
{
  QApplication app( argc, argv );
 
  DAFFVizQtVTKRenderWindow win;
  win.show();
 
  return app.exec();
}