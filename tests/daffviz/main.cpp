
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkFollower.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h> 
#include <vtkSphereSource.h>
#include <vtkVectorText.h>
#include <vtkLabeledDataMapper.h>

int main(int argc,char *argv[]) {

	// Text
	vtkVectorText* text = vtkVectorText::New();
	text->SetText("\n\n\t\tSphere 1");

	vtkPolyDataMapper* textmapper = vtkPolyDataMapper::New();
	textmapper->SetInputConnection(text->GetOutputPort());

	vtkFollower* follower = vtkFollower::New();
	follower->SetMapper(textmapper);
	follower->SetOrigin(0, 0, 0);
	follower->GetProperty()->SetColor(0, 1, 0);
	follower->SetScale(0.1, 0.1, 0.1);
	follower->SetPosition(2, 0, 0);
	follower->AddPosition(0, -0.5, 0);

	// Sphere
	vtkSphereSource* sphere = vtkSphereSource::New();

	vtkPolyDataMapper* map = vtkPolyDataMapper::New();
	map->SetInputData(sphere->GetOutput());

	vtkActor* actor = vtkActor::New();
	actor->SetMapper(map);
	actor->GetProperty()->SetOpacity(0.4);
	actor->SetPosition(-2, 0, 0);

	vtkActor* sphere2 = vtkActor::New();
	sphere2->SetMapper(map);
	actor->GetProperty()->SetColor(0, 1, 0);
	sphere2->SetPosition(2, 0, 0);

	// Label
	vtkLabeledDataMapper* labelmapper = vtkLabeledDataMapper::New();


	// VTK stuff
	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renderwindow = vtkRenderWindow::New();
	renderwindow->AddRenderer(renderer);

	vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(renderwindow);

	// Add actors
	renderer->AddActor(actor);
	renderer->AddActor(sphere2);
	renderer->AddActor(follower);

	follower->SetCamera(renderer->GetActiveCamera());

	renderwindow->Render();
	renderer->GetActiveCamera()->Dolly(0.1);
	interactor->Start();

	text->Delete();
	sphere->Delete();
	map->Delete();
	actor->Delete();
	renderer->Delete();
	renderwindow->Delete();
	interactor->Delete(); 

	return 0;
}