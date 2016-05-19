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
#include "vtkAssembly.h"

/* The source code has been taken from the vtk.org/Wiki:
	1. http://www.vtk.org/Wiki/VTK/Examples/Cxx/GeometricObjects/Arrow
	2. http://www.vtk.org/Wiki/VTK/Examples/Cxx/Meshes/AddCell
*/

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
	vtkSmartPointer<vtkPoints> points =	vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint( 1.0, 0.0, 0.0 );
	points->InsertNextPoint( 0.0, 0.0, 0.0 );
	points->InsertNextPoint( 0.0, 1.0, 0.0 );

	vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
	triangle->GetPointIds()->SetId( 0, 0 );
	triangle->GetPointIds()->SetId( 1, 1 );
	triangle->GetPointIds()->SetId( 2, 2 );

	vtkSmartPointer<vtkCellArray> triangles = 	vtkSmartPointer<vtkCellArray>::New();
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
	vtkSmartPointer<vtkRenderer> renderer =	vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =	vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =	vtkSmartPointer< vtkRenderWindowInteractor >::New();
	renderWindowInteractor->SetRenderWindow( renderWindow );


	vtkSmartPointer<vtkAssembly> pAssembly = vtkSmartPointer<vtkAssembly>::New();
	pAssembly->AddPart( actor );
	pAssembly->AddPart( actor2 );
	pAssembly->RotateY( -45 );

	//renderer->AddActor( pAssembly );
	renderer->SetBackground( 1, 0, 1 );


	renderWindow->Render();
	renderWindowInteractor->Start();

	return 0;
}
