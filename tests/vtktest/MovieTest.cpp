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

#include <vtkArrowSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkAssembly.h>
#include <vtkActor.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkWarpScalar.h>
#include <vtkMath.h>
#include <vtkAVIWriter.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkWindowToImageFilter.h>

#include <Windows.h>
#include "vtkPNGWriter.h"
#include <sstream>

/* The source code has been taken from the vtk.org/Wiki:
	1. http://www.vtk.org/Wiki/VTK/Examples/Cxx/GeometricObjects/Arrow
	2. http://www.vtk.org/Wiki/VTK/Examples/Cxx/Meshes/AddCell
	3. http://www.vtk.org/Wiki/VTK/Examples/Cxx/Untested/Video/AVI
*/

using namespace std;

int main( int, char** )
{
	// Create an arrow.
	vtkSmartPointer<vtkArrowSource> arrowSource = vtkSmartPointer<vtkArrowSource>::New();
	arrowSource->SetShaftRadius( .02f );
	arrowSource->SetTipLength( .2f );
	arrowSource->SetShaftResolution( 12 );
	arrowSource->SetTipResolution( 12 );
	arrowSource->Update();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( arrowSource->GetOutputPort() );
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	// Create a triangle
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint( 1.0, 0.0, 0.0 );
	points->InsertNextPoint( 0.0, 0.0, 0.0 );
	points->InsertNextPoint( 0.0, 1.0, 0.0 );

	vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
	triangle->GetPointIds()->SetId( 0, 0 );
	triangle->GetPointIds()->SetId( 1, 1 );
	triangle->GetPointIds()->SetId( 2, 2 );

	vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
	triangles->InsertNextCell( triangle );

	// Create a polydata object
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

	// Add the geometry and topology to the polydata
	polyData->SetPoints( points );
	polyData->SetPolys( triangles );

	polyData->GetPolys()->InsertNextCell( triangle );

	vtkSmartPointer< vtkPolyDataMapper > mapper2 = vtkSmartPointer< vtkPolyDataMapper >::New();
	mapper2->SetInputData( polyData );

	vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper( mapper2 );

	// Visualize
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer< vtkRenderWindowInteractor >::New();
	renderWindowInteractor->SetRenderWindow( renderWindow );


	vtkSmartPointer<vtkAssembly> pAssembly = vtkSmartPointer<vtkAssembly>::New();
	pAssembly->AddPart( actor );
	pAssembly->AddPart( actor2 );
	pAssembly->RotateY( -45 );


	{
		// Create a sphere
		vtkSmartPointer<vtkSphereSource> sphereSource =
			vtkSmartPointer<vtkSphereSource>::New();
		sphereSource->Update();

		// Create Scalars
		vtkSmartPointer<vtkDoubleArray> scalars =
			vtkSmartPointer<vtkDoubleArray>::New();
		int numberOfPoints = sphereSource->GetOutput()->GetNumberOfPoints();
		scalars->SetNumberOfTuples( numberOfPoints );

		vtkMath::RandomSeed( 8775070 ); // for reproducibility
		for( vtkIdType i = 0; i < numberOfPoints; ++i )
		{
			scalars->SetTuple1( i, vtkMath::Random( 0.0, 1.0 / 7.0 ) );
		}

		sphereSource->GetOutput()->GetPointData()->SetScalars( scalars );

		vtkSmartPointer<vtkWarpScalar> warpScalar =
			vtkSmartPointer<vtkWarpScalar>::New();
		warpScalar->SetInputConnection( sphereSource->GetOutputPort() );
		warpScalar->SetScaleFactor( 1 ); // use the scalars themselves

		// Required for SetNormal to have an effect
		warpScalar->UseNormalOn();
		warpScalar->SetNormal( 0, 0, 1 );

		warpScalar->Update();

		// Create a mapper and actor
		vtkSmartPointer<vtkPolyDataMapper> mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection( warpScalar->GetOutputPort() );

		vtkSmartPointer<vtkActor> actor =
			vtkSmartPointer<vtkActor>::New();
		actor->SetMapper( mapper );
		pAssembly->AddPart( actor );


	}
	renderer->AddActor( pAssembly );
	renderer->SetBackground( 1, 0, 1 );

	/* -- AVI

	vtkSmartPointer<vtkImageCanvasSource2D> source =
	vtkSmartPointer<vtkImageCanvasSource2D>::New();
	source->SetScalarTypeToUnsignedChar();
	source->SetNumberOfScalarComponents( 3 );
	source->SetExtent( 0, 100, 0, 100, 0, 0 );

	vtkSmartPointer<vtkAVIWriter> writer =
	vtkSmartPointer<vtkAVIWriter>::New();

	vtkSmartPointer<vtkWindowToImageFilter> w2i = vtkSmartPointer<vtkWindowToImageFilter>::New();
	w2i->SetInput( renderWindow );
	writer->SetInputConnection( w2i->GetOutputPort() );
	writer->SetFileName( "test.avi" );
	writer->Start();

	for( unsigned int i = 0; i < 100; i++ )
	{
	source->SetDrawColor( 0, 0, 0, 1 ); //black
	source->FillBox( 0, 100, 0, 100 ); //clear image
	source->SetDrawColor( 255, 0, 0, 1 ); //red
	source->FillBox( i, 20, 10, 20 );
	source->Update();

	renderWindow->Render();
	w2i->Update();

	writer->Write();
	}

	writer->End();


	--- */

	for( int i = 0; i < 360; i++ )
	{
		pAssembly->RotateY( 1.0f );
		renderWindow->Render();

		vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
		windowToImageFilter->SetInput( renderWindow );
		windowToImageFilter->SetMagnification( 1 );
		windowToImageFilter->SetInputBufferTypeToRGBA();
		windowToImageFilter->ReadFrontBufferOff();
		windowToImageFilter->Update();

		vtkSmartPointer< vtkPNGWriter > pExportPNG = vtkSmartPointer< vtkPNGWriter >::New();
		stringstream ss;
		ss << "Exported3DImage_" << i << ".png";
		pExportPNG->SetFileName( ss.str().c_str() );
		pExportPNG->SetInputConnection( windowToImageFilter->GetOutputPort() );
		pExportPNG->Write();

	}

	return 0;
}
