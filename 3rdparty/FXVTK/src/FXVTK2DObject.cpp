// $Id: FXVTK2DObject.cpp,v 1.1 2010/02/10 09:57:25 schmoch Exp $

#include "FXVTK2DObject.h"
#include "FXVTKWidget.h"
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkActor2DCollection.h>
#include <vtkCoordinate.h>
#include <vtkProperty.h>


FXVTK2DObject::FXVTK2DObject(void) {
	Actors2D = vtkActor2DCollection::New();
	Actors3D = NULL;
}

FXVTK2DObject::~FXVTK2DObject(void) {
}



// Set- und GetPosition Methoden
void FXVTK2DObject::SetPosition(double x, double y) {
	Actors2D->InitTraversal();
	while (vtkActor2D* actor = Actors2D->GetNextActor2D()) {
		actor->SetPosition(x,y);
		actor->GetPositionCoordinate()->SetCoordinateSystemToViewport();
	}
}
void FXVTK2DObject::SetRelativePosition(double x, double y) {
	Actors2D->InitTraversal();
	while (vtkActor2D* actor = Actors2D->GetNextActor2D()) {
		actor->SetPosition(x,y);
		actor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
	}
}
void FXVTK2DObject::Set3DPosition(double x, double y, double z) {
	Actors2D->InitTraversal();
	while (vtkActor2D* actor = Actors2D->GetNextActor2D()) {
		actor->GetPositionCoordinate()->SetValue(x,y,z);
		actor->GetPositionCoordinate()->SetCoordinateSystemToWorld();
	}
}


void FXVTK2DObject::GetPosition(double &x, double &y) {
	if (vtkActor2D* actor = Actors2D->GetLastActor2D()) {
		double* data = actor->GetPosition();
		x = data[0]; y = data[1];
	}
	else {
		x = 0; y = 0;
	}
}

bool FXVTK2DObject::IsVisibilityOn() {
	if (Actors2D == NULL) return false;
	Actors2D->InitTraversal();
	vtkActor2D* actor = Actors2D->GetNextActor2D();
	if (actor == NULL) return false;

	if (actor->GetVisibility())   return true;
	else   return false;
}


// On default, the first actor of the objectwill be set pickable On or Off, 
void FXVTK2DObject::SetPickableOn() {
	if (Actors2D != NULL) {
		Actors2D->InitTraversal();
		if (vtkActor2D* actor = Actors2D->GetNextActor2D())
			actor->PickableOn();
	}
}

bool FXVTK2DObject::IsPickable() {
	if (Actors2D != NULL) {
		Actors2D->InitTraversal();
		if (vtkActor2D* actor = Actors2D->GetNextActor2D()) {
			if (actor->GetPickable())
				return true;
			else
				return false;
		}
	}
	return false;
}




