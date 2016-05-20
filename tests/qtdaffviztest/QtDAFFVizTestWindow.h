﻿#ifndef IW_DAFFVIZQTTESTWINDOW
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
	class QtDAFFVizTestWindow;
}

class QtDAFFVizTestWindow : public QMainWindow
{
    Q_OBJECT


public:
	explicit QtDAFFVizTestWindow( QWidget *parent = 0 );
	~QtDAFFVizTestWindow();

public slots:

private slots:

    void on_actionQuit_triggered();
	void on_actionOpen_triggered();
	void on_lineEditFrequency_returnPressed();
	void on_lineEditPhi_returnPressed();
	void on_lineEditTheta_returnPressed();

private:
	Ui::QtDAFFVizTestWindow *ui;
	DAFFReader *reader;
	DAFFViz::SGNode *node, *nodeCarpet;
	vtkSmartPointer<vtkRenderer> renderer, rendererPlot;
	vtkSmartPointer< vtkCamera >camera;
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
