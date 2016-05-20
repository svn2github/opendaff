#ifndef DAFFVIZQTVTKRENDERWINDOW_H
#define DAFFVIZQTVTKRENDERWINDOW_H
 
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <QMainWindow>

class Ui_DAFFVizQtVTKRenderWindow;
 
class DAFFVizQtVTKRenderWindow : public QMainWindow
{
  Q_OBJECT
public:
  DAFFVizQtVTKRenderWindow(); 
  inline ~DAFFVizQtVTKRenderWindow()
  {
	  m_pRenderer = vtkSmartPointer<vtkRenderer>::New();
	  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	  renderWindow->AddRenderer( m_pRenderer );

  };
 
public slots:
 
  virtual void slotExit();
 
private:
  Ui_DAFFVizQtVTKRenderWindow *ui;
  vtkSmartPointer< vtkRenderer > m_pRenderer;
};
 
#endif // DAFFVIZQTVTKRENDERWINDOW_H