// $Id: FXVTKObject.cpp,v 1.1 2010/02/10 09:57:25 schmoch Exp $

#include "FXVTKObject.h"
#include "FXVTKWidget.h"
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkProp3DCollection.h>
#include <vtkActor2DCollection.h>
#include <vtkProp.h>
#include <vtkProperty.h>
#include <string>

void FXVTKObject::Initialization() {
	UpToDate = true;
	SingleVtkProp = NULL;
	UserSpecificID = -1;
}
FXVTKObject::FXVTKObject(void) {
	Initialization();
}

FXVTKObject::~FXVTKObject(void) {
	DisconnectAllWidgets();
	if (Actors3D != NULL) Actors3D->Delete();
	if (Actors2D != NULL) Actors2D->Delete();
}


void FXVTKObject::SetUserSpecificID(int id) {
	UserSpecificID = id;
}
int FXVTKObject::GetUserSpecificID() {
	return UserSpecificID;
}

void FXVTKObject::SetUserSpecificName(char* name) {
	UserSpecificName = name;
}


void FXVTKObject::ConnectWidget(FXVTKWidgetBaseClass* widget) {
	if ( Actors2D == NULL && Actors3D == NULL && SingleVtkProp == NULL ) return;

	widget->_addObject(this, Actors3D, Actors2D, SingleVtkProp );
	WidgetList.push_back(widget);
	
	if (UpToDate == false) widget->UpToDate = false;
}

bool FXVTKObject::DisconnectWidget(FXVTKWidgetBaseClass* widget) {
	for (std::vector<FXVTKWidgetBaseClass*>::iterator iterator = WidgetList.begin(); iterator != WidgetList.end(); iterator++) {
		if ((*iterator) == widget) {
			WidgetList.erase(iterator);
			widget->_delObject(this, Actors3D, Actors2D, SingleVtkProp);
			return true;
		}
	}
	return false;
}

void FXVTKObject::DisconnectAllWidgets() {
	if (!(WidgetList.empty())) {
		for (std::vector<FXVTKWidgetBaseClass*>::iterator iterator = WidgetList.begin(); iterator != WidgetList.end(); iterator++) {
			(*iterator)->_delObject(this, Actors3D, Actors2D, SingleVtkProp);
		}
		WidgetList.clear();
	}
}

void FXVTKObject::RenderAllConnectedWidgets() {
	for (std::vector<FXVTKWidgetBaseClass*>::iterator iterator = WidgetList.begin(); iterator != WidgetList.end(); iterator++) {
		(*iterator)->Render();
	}
}

FXVTKWidgetBaseClass* FXVTKObject::GetFirstConnectedWidget() {
	if (WidgetList.empty()) return NULL;
	return WidgetList[0];
}

void FXVTKObject::_update() {
	return;
}

void FXVTKObject::SetChanged() {
	UpToDate = false;
	if (WidgetList.empty()) return;
	for (std::vector<FXVTKWidgetBaseClass*>::iterator iterator = WidgetList.begin(); iterator != WidgetList.end(); iterator++) {
		(*iterator)->UpToDate = false;
	}
}

void FXVTKObject::SetUpToDate() {
	UpToDate = true;
}

bool FXVTKObject::IsUpToDate() {
	return UpToDate;
}



void FXVTKObject::VisibilityOn() {
	if (Actors3D != NULL) {
		Actors3D->InitTraversal();
		while (vtkProp3D* actor = Actors3D->GetNextProp3D())
			actor->VisibilityOn();
	}
	if (Actors2D != NULL) {
		Actors2D->InitTraversal();
		while (vtkActor2D* actor = Actors2D->GetNextActor2D())
			actor->VisibilityOn();
	}
	if (SingleVtkProp != NULL) {
		SingleVtkProp->VisibilityOn();
	}
}

void FXVTKObject::VisibilityOff() {
	if (Actors3D != NULL) {
		Actors3D->InitTraversal();
		while (vtkProp3D* actor = Actors3D->GetNextProp3D())
			actor->VisibilityOff();
	}
	if (Actors2D != NULL) {
		Actors2D->InitTraversal();
		while (vtkActor2D* actor = Actors2D->GetNextActor2D())
			actor->VisibilityOff();
	}
	if (SingleVtkProp != NULL) {
		SingleVtkProp->VisibilityOff();
	}
}



// The definition SetPickableOn() and IsPickable() will be find in the derived classes,
// so it is possible to specifie which actors will be pickable.
// With SetPickableOff(), all actors will be set to no-pickable.
void FXVTKObject::SetPickableOff() {
	if (Actors3D != NULL) {
		Actors3D->InitTraversal();
		while (vtkProp3D* actor = Actors3D->GetNextProp3D())
			actor->PickableOff();
	}
	if (Actors2D != NULL) {
		Actors2D->InitTraversal();
		while (vtkActor2D* actor = Actors2D->GetNextActor2D())
			actor->PickableOff();
	}
	if (SingleVtkProp != NULL) {
		SingleVtkProp->PickableOff();
	}
}



