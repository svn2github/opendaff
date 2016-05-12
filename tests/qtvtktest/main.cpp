#include <QApplication>
#include "DAFFVizQtVTKRenderWindow.h"
#include <daffviz/DAFFVizSphericalCoordinateAssistant.h>

int main( int argc, char** argv )
{
  QApplication app( argc, argv );
 
  DAFFVizQtVTKRenderWindow win;

  // Add - for example - a SCA node
  DAFFViz::SphericalCoordinateAssistant node;
  win.SetSceneGraphRootNode(&node);

  win.show();
 
  return app.exec();
}