#include <vtkArrowSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkPolyData.h>
#include <vtkCell.h>

int main( int, char** )
{

	// Create a face
	vtkSmartPointer<vtkPoints> points =	vtkSmartPointer<vtkPoints>::New();
	vtkIdType vPoints[4];
	vPoints[ 0 ] = points->InsertNextPoint( 0.0, 0.0, 0.0 );
	vPoints[ 1 ] = points->InsertNextPoint( 0.0, 0.0, 1.0 );
	vPoints[ 2 ] = points->InsertNextPoint( 1.0, 0.0, 1.0 );
	vPoints[ 3 ] = points->InsertNextPoint( 1.0, 0.0, 0.0 );

	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell( 4, vPoints );

	// Create a polydata object
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

	// Add the geometry and topology to the polydata
	polyData->SetPoints( points );
	polyData->SetPolys( cells );

	std::cout << "There are " << polyData->GetNumberOfCells() << " cells." << std::endl;
	
	vtkSmartPointer< vtkPolyDataMapper > mapper2 = vtkSmartPointer< vtkPolyDataMapper >::New();
	mapper2->SetInputData( polyData );

	vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper( mapper2 );

	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =	vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =	vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow( renderWindow );

	renderer->AddActor( actor2 );
	renderer->SetBackground( 0.52, 1, 1 );

	renderWindow->Render();
	renderWindowInteractor->Start();
	
	return 0;
}
