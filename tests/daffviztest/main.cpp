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

	DAFFViz::SGNode oRootNode;

	oRootNode.AddChildNode( pContentNode );

	DAFFViz::Arrow arrow;
	arrow.SetScale( 2, 1, 0.3f );
	oRootNode.AddChildNode( &arrow );
	oRootNode.SetOrientationYPR( 13, -13, 25 );

	DAFFViz::CartesianCoordinateAssistant cca;
	oRootNode.AddChildNode( &cca );

	DAFFViz::SphericalCoordinateAssistant sca;
	sca.SetEquatorVisible( true );
	sca.SetGridVisible( true );
	sca.SetMeridiansVisible( true );
	sca.SetViewUpVectorsVisible( true );
	
	oRootNode.AddChildNode( &sca );

	DAFFViz::Grid grid, grid2;
	oRootNode.AddChildNode( &grid );
	oRootNode.AddChildNode( &grid2 );
	grid2.SetPosition( 0, .2, 0 );

	DAFFViz::Sphere oSphere;
	oRootNode.AddChildNode( &oSphere );

	DAFFViz::Label oTextLabel;
	oTextLabel.SetText( "This is the DAFFVizTest test." );
	oRootNode.AddChildNode( &oTextLabel );

	DAFFViz::VTKDAFFVizWindow win;
	win.SetSceneGraphRootNode( &oRootNode );
	win.Start();

	delete pContentNode;

	return 0;
}
