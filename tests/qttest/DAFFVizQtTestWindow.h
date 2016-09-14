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

#ifndef IW_DAFFVIZQTTESTWINDOW
#define IW_DAFFVIZQTTESTWINDOW

#include <vtkSmartPointer.h> 
#include <vtkCamera.h>
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkChartXY.h"
#include "vtkPlot.h"
#include "vtkTable.h"
#include "vtkFloatArray.h"

#include <QMainWindow>
#include<DAFF.h>
#include <DAFFReader.h>
#include <vtkAssembly.h>
#include <daffviz/DAFFVizSGNode.h>
#include <daffviz/DAFFVizBalloonPlot.h>
#include <daffviz/DAFFVizCarpetPlot.h>
#include <daffviz/DAFFVizArrow.h>
#include <daffviz/DAFFVizGrid.h>
#include <daffviz/DAFFVizLabel.h>
#include <daffviz/DAFFVizLine.h>
#include <daffviz/DAFFVizCartesianCoordinateAssistant.h>
#include <daffviz/DAFFVizSphericalCoordinateAssistant.h>

namespace Ui {
class DAFFVizQtWindow;
}

class DAFFVizQtWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit DAFFVizQtWindow(QWidget *parent = 0);
    ~DAFFVizQtWindow();

public slots:

private slots :
void on_actionQuit_triggered();
/*
	void on_actionOpen_triggered();
	//void on_lineEditFrequency_returnPressed();
	//void on_lineEditPhi_returnPressed();
	//void on_lineEditTheta_returnPressed();
*/
private:
    Ui::DAFFVizQtWindow *ui;
	DAFFReader *reader;
	DAFFViz::SGNode *node, *nodeCarpet;
	vtkSmartPointer<vtkRenderer> renderer, rendererPlot;
	vtkCamera *camera;
	vtkSmartPointer<vtkContextView> view;
	vtkSmartPointer<vtkTable> table;
	vtkSmartPointer<vtkFloatArray> dataX, dataY;
	vtkSmartPointer<vtkChartXY> chart;


	DAFFViz::SphericalCoordinateAssistant* spherical;
	DAFFViz::CartesianCoordinateAssistant* cartesian;
	DAFFViz::Arrow* balloonUpArrow, *balloonFrontArrow, *carpetXArrow, *carpetYArrow, *carpetZArrow;
	DAFFViz::Grid *grid;
	DAFFViz::Label *label;
	DAFFViz::Line *line;
	DAFFViz::BalloonPlot *balloonPlot;
	DAFFViz::CarpetPlot *carpetPlot;
	void drawGraph();
};

#endif // IW_DAFFVIZQTTESTWINDOW
