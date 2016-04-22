#include <daffviz/DAFFViz.h>
#include "VTKDAFFVizWindow.h"

int main(int, char**)
{
	DAFFViz::VTKDAFFVizWindow win;
	DAFFViz::Grid node;
	win.SetSceneGraphRootNode(&node);
	win.Start();

	return 0;
}
