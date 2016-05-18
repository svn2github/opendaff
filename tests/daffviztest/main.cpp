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

	DAFFViz::SGNode oRootNode;
	DAFFViz::SGNode* pContentNode = NULL;

	if( pReader->getContentType() == DAFF_IMPULSE_RESPONSE )
	{
		DAFFContentIR* pContent = dynamic_cast< DAFFContentIR* >( pReader->getContent() );
		pContentNode = new DAFFViz::CarpetPlot( pContent );
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
		

	//DAFFViz::SphericalCoordinateAssistant oSCA;
	//oRootNode.AddChildNode( &oSCA );

	DAFFViz::VTKDAFFVizWindow win;
	win.SetSceneGraphRootNode( pContentNode );
	win.Start();
	win.SetSceneGraphRootNode( NULL );
	
	oRootNode.RemoveChildNode( pContentNode );
	delete pContentNode;

	return 0;
}
