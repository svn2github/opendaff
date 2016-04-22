#include "DAFFVizQtVTKRenderWindow.h"
#include "ui_DAFFVizQtVTKRenderWindow.h"
 
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
 
DAFFVizQtVTKRenderWindow::DAFFVizQtVTKRenderWindow() 
{
  this->ui = new Ui_DAFFVizQtVTKRenderWindow;
  this->ui->setupUi(this);
 
  // Sphere
  vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
  vtkSmartPointer<vtkActor> sphereActor = 
      vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper(sphereMapper);
 
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(sphereActor);
  
  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
 
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
 
}
 
void DAFFVizQtVTKRenderWindow::slotExit() 
{
  qApp->exit();
}