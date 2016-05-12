#include "DAFFVizQtVTKRenderWindow.h"
#include "ui_DAFFVizQtVTKRenderWindow.h"
 
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include "vtkCamera.h"
 
DAFFVizQtVTKRenderWindow::DAFFVizQtVTKRenderWindow() 
{
  this->ui = new Ui_DAFFVizQtVTKRenderWindow;
  this->ui->setupUi(this);



  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(m_pRenderer);
 
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
 
}
 
void DAFFVizQtVTKRenderWindow::slotExit() 
{
  qApp->exit();
}