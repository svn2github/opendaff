#include <daffviz/DAFFViz.h>
#include "VTKDAFFVizWindow.h"

#include <DAFF.h>
#include <string>
#include <iostream>

using namespace std;

int main( int, char** )
{
	string sInputFilePath = "../../../matlab/ExampleShortDiracOmni.ir.daff";

	int iDAFFERROR = DAFF_NO_ERROR;
	DAFFReader* pReader = DAFFReader::create();
	if( ( iDAFFERROR = pReader->openFile( sInputFilePath ) ) != DAFF_NO_ERROR )
	{
		cerr << "Error while opening DAFF file '" << sInputFilePath << "': " << DAFFUtils::StrError( iDAFFERROR ) << endl;
		return 255;
	}
	else
		cout << "Sucessfully opened DAFF file '" << sInputFilePath << "'" << endl;

	DAFFViz::SGNode* pContentNode = NULL;

	if( pReader->getContentType() == DAFF_IMPULSE_RESPONSE )
	{
		DAFFContentIR* pContent = dynamic_cast< DAFFContentIR* >( pReader->getContent() );
		pContentNode = new DAFFViz::CarpetPlot( pContent );
		//DAFFViz::CarpetPlot oCarpet( pContent );
	}
	else if( pReader->getContentType() == DAFF_MAGNITUDE_SPECTRUM )
	{
		DAFFContentMS* pContent = dynamic_cast< DAFFContentMS* >( pReader->getContent() );
		pContentNode = new DAFFViz::BalloonPlot( pContent );
	}
	else
	{
		cerr << "Content type '" << DAFFUtils::StrContentType( pReader->getContentType() ) << "' nit implemented in this test." << endl;
	}

	pReader->closeFile();

	/*
	DAFFViz::SGNode oRootNode;	
	DAFFViz::Arrow arrow;
	oRootNode.AddChildNode( &arrow );
	*/
	
	DAFFViz::VTKDAFFVizWindow* win = NULL;
	//win = new DAFFViz::VTKDAFFVizWindow;
	//win.SetSceneGraphRootNode( pContentNode );
	//win.Start();

	delete pContentNode;

	delete win;

	
	return 0;
}
