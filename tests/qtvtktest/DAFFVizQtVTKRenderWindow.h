#ifndef DAFFVIZQTVTKRENDERWINDOW_H
#define DAFFVIZQTVTKRENDERWINDOW_H
 
#include <vtkSmartPointer.h> 
#include <QMainWindow>
 
class Ui_DAFFVizQtVTKRenderWindow;
 
class DAFFVizQtVTKRenderWindow : public QMainWindow
{
  Q_OBJECT
public:
  DAFFVizQtVTKRenderWindow(); 
  ~DAFFVizQtVTKRenderWindow() {};
 
public slots:
 
  virtual void slotExit();
 
private:
  Ui_DAFFVizQtVTKRenderWindow *ui;
};
 
#endif // DAFFVIZQTVTKRENDERWINDOW_H