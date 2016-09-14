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
 
  // Sphere
  vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
  vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper(sphereMapper);
 
  vtkSmartPointer< vtkRenderer > pRenderer = vtkSmartPointer< vtkRenderer >::New();
  pRenderer->AddActor(sphereActor);

  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(pRenderer);
 
  connect( this->ui->actionExit, SIGNAL( triggered() ), this, SLOT( slotExit() ) ); 
}
 
void DAFFVizQtVTKRenderWindow::slotExit() 
{
  qApp->exit();
}