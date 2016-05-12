#ifndef DAFFVIZQTVTKRENDERWINDOW_H
#define DAFFVIZQTVTKRENDERWINDOW_H
 
#include <vtkSmartPointer.h> 
#include <QMainWindow>
#include "VTKDAFFVizWindow.h"

class Ui_DAFFVizQtVTKRenderWindow;
 
class DAFFVizQtVTKRenderWindow : public QMainWindow, public DAFFViz::VTKDAFFVizWindow
{
  Q_OBJECT
public:
  DAFFVizQtVTKRenderWindow(); 
  inline ~DAFFVizQtVTKRenderWindow() {};
 
public slots:
 
  virtual void slotExit();
 
private:
  Ui_DAFFVizQtVTKRenderWindow *ui;
};
 
#endif // DAFFVIZQTVTKRENDERWINDOW_H