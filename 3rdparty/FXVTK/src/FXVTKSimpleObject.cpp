// $Id: FXVTKSimpleObject.cpp,v 1.9 2008/11/21 15:20:05 giese Exp $

#include "FXVTKSimpleObject.h"
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProp3DCollection.h>
#include <vtkProperty.h>


void FXVTKSimpleObject::Initialization() {
	Actor = vtkActor::New();
	Actors3D->AddItem(Actor);
	SetPickableOff();

	Mapper = NULL;
	SetChanged();  // all simpleobjects have all default values, so an object could create after initialization
}

FXVTKSimpleObject::FXVTKSimpleObject(void) {
	Initialization();
}
FXVTKSimpleObject::FXVTKSimpleObject(FXVTKWidgetBaseClass* widget) {
	Initialization();
	ConnectWidget(widget);
}
FXVTKSimpleObject::~FXVTKSimpleObject(void) {
	DisconnectAllWidgets();
	if (Mapper != NULL) Mapper->Delete();
	Actor->Delete();
}

void FXVTKSimpleObject::SetColor(double r, double g, double b) {
	Actor->GetProperty()->SetColor(r,g,b);
}
void FXVTKSimpleObject::GetColor(double &r, double &g, double &b) {
	double rgb[3];
	Actor->GetProperty()->GetColor(rgb);
	r = rgb[0];
	g = rgb[1];
	b = rgb[2];
}

void FXVTKSimpleObject::SetAlpha(double a) {
	Actor->GetProperty()->SetOpacity(a);
}

double FXVTKSimpleObject::GetAlpha(){
	return Actor->GetProperty()->GetOpacity();
}