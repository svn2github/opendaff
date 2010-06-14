// $Id: FXVTKPlotObject.cpp,v 1.1 2010/02/10 09:57:25 schmoch Exp $

#include "FXVTKPlotObject.h"
#include "FXVTKColorMap.h"
#include <vtkStructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkProp3DCollection.h>
#include <vtkFloatArray.h>

void FXVTKPlotObject::Initialization() {
	PlotActor = vtkActor::New();
	Actors3D->AddItem(PlotActor);
	SetPickableOff();

	PlotDataScalars = NULL;
	NewPlotDataScalars = NULL;
	PlotStructuredGrid = NULL;
	PlotDataPoints = NULL;
	PlotDataSetMapper = NULL;
	PlotDataDim0Size = 0;
	PlotDataDim1Size = 0;
	ColorMap = NULL;
	LogarithmicMode = false;
}	

FXVTKPlotObject::FXVTKPlotObject(void) {
	Initialization();
}

FXVTKPlotObject::FXVTKPlotObject(FXVTKWidgetBaseClass *widget)  {
	Initialization();
	ConnectWidget(widget);
}

FXVTKPlotObject::~FXVTKPlotObject(void) {
	DisconnectAllWidgets();

	if (NewPlotDataScalars != NULL) NewPlotDataScalars->Delete();
	if (PlotDataScalars != NULL) PlotDataScalars->Delete();
	if (PlotDataPoints != NULL) PlotDataPoints->Delete();
	if (PlotStructuredGrid != NULL) PlotStructuredGrid->Delete();
	if (PlotDataSetMapper != NULL) PlotDataSetMapper->Delete();
	PlotActor->Delete();
}

void FXVTKPlotObject::Update() {
	_update();
}

void FXVTKPlotObject::SetColorMap(FXVTKColorMap *colormap) {
	ColorMap = colormap;
	if (PlotDataSetMapper != NULL) {
		PlotDataSetMapper->SetLookupTable(colormap->_getScalarsToColorsObject());
		PlotDataSetMapper->UseLookupTableScalarRangeOn();
	}
}

void FXVTKPlotObject::GetRange(double &min, double &max) {
	double* pointer;
	if (NewPlotDataScalars != NULL) pointer = NewPlotDataScalars->GetRange();
	else if (PlotDataScalars != NULL) pointer = PlotDataScalars->GetRange();
	else {
		min = 0; max = 0; return;
	}
	min = pointer[0]; max = pointer[1];
}

double* FXVTKPlotObject::GetRange() {
	if (NewPlotDataScalars != NULL) return NewPlotDataScalars->GetRange();
	else if (PlotDataScalars != NULL) return PlotDataScalars->GetRange();
	else return NULL;
}

void FXVTKPlotObject::SetLogarithmicModeOn() {
	if ( LogarithmicMode == true )  return;
	LogarithmicMode = true;
	SetChanged();
}
void FXVTKPlotObject::SetLogarithmicModeOff() {
	if ( LogarithmicMode == false )  return;
	LogarithmicMode = false;
	SetChanged();
}
bool FXVTKPlotObject::LogarithmicModeIsOn() {
	return LogarithmicMode;
}