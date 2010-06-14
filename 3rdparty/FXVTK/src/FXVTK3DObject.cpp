// $Id: FXVTK3DObject.cpp,v 1.1 2010/02/10 09:57:25 schmoch Exp $

#include "FXVTK3DObject.h"
#include "FXVTKWidget.h"
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkProp3DCollection.h>
#include <vtkProp3D.h>
#include <vtkProperty.h>


FXVTK3DObject::FXVTK3DObject(void) {
	Actors3D = vtkProp3DCollection::New();
	Actors2D = NULL;
}

FXVTK3DObject::~FXVTK3DObject(void) {
	Actors3D->Delete();
	Actors3D = NULL;
}



// Set- und GetPosition Methoden
void FXVTK3DObject::SetPosition(double x, double y, double z) {
	Actors3D->InitTraversal();
	while (vtkProp3D* actor = Actors3D->GetNextProp3D())
		actor->SetPosition(x,y,z);
}
void FXVTK3DObject::SetPosition(double* xyz_array) {
	Actors3D->InitTraversal();
	while (vtkProp3D* actor = Actors3D->GetNextProp3D())
		actor->SetPosition(xyz_array);
}
void FXVTK3DObject::SetPosition(Point &point) {
	Actors3D->InitTraversal();
	while (vtkProp3D* actor = Actors3D->GetNextProp3D())
		actor->SetPosition(point.x,point.y,point.z);
}
void FXVTK3DObject::GetPosition(double &x, double &y, double &z) {
	Actors3D->InitTraversal();
	if ( vtkProp3D* actor = Actors3D->GetNextProp3D() ) {
		double* data = actor->GetPosition();
		x = data[0]; y = data[1]; z = data[2];
	}
	else {
		x = 0; y = 0; z = 0;
	}
}
FXVTK3DObject::Point FXVTK3DObject::GetPosition() {
	Point point;
	GetPosition(point.x,point.y,point.z);
	return point;
}


// Set- und GetOrientation Methoden
void FXVTK3DObject::SetOrientation(double RotateX, double RotateY, double RotateZ) {
	Actors3D->InitTraversal();
	while (vtkProp3D* actor = Actors3D->GetNextProp3D())
		actor->SetOrientation(RotateX,RotateY,RotateZ);
}
void FXVTK3DObject::SetOrientation(Orientation orientation) {
	Actors3D->InitTraversal();
	while (vtkProp3D* actor = Actors3D->GetNextProp3D())
		actor->SetOrientation(orientation.RotX,orientation.RotY,orientation.RotZ);
}
void FXVTK3DObject::GetOrientation(double &RotX, double &RotY, double &RotZ) {
	if (vtkProp3D* actor = Actors3D->GetNextProp3D()) {
		double* rotat = actor->GetOrientation();
		RotX = rotat[0]; RotY = rotat[1]; RotZ = rotat[2];
	}
	else {
		RotX = 0; RotY = 0; RotZ = 0;
	}
}
FXVTK3DObject::Orientation FXVTK3DObject::GetOrientation() {
	Orientation orient;
	if (vtkProp3D* actor = Actors3D->GetNextProp3D()) {
		double* rotat = actor->GetOrientation();
		orient.RotX = rotat[0]; orient.RotY = rotat[1]; orient.RotZ = rotat[2];
	}
	else {
		orient.RotX = 0; orient.RotY = 0; orient.RotZ = 0;
	}
	return orient;
}

void FXVTK3DObject::SetRepresentationToPoints() {
	Actors3D->InitTraversal();
	while (vtkProp3D* prop3d = Actors3D->GetNextProp3D()) {
		vtkActor* actor = dynamic_cast<vtkActor*>( prop3d );
		if ( actor != NULL ) {
			actor->GetProperty()->SetRepresentationToPoints();

			actor->GetProperty()->SetAmbient(0.7);
			actor->GetProperty()->SetDiffuse(0.2);
			actor->GetProperty()->SetSpecular(0.1);
			actor->GetProperty()->SetPointSize(2.0);
		}
	}
}
void FXVTK3DObject::SetRepresentationToSurface() {
	Actors3D->InitTraversal();
	while (vtkProp3D* prop3d = Actors3D->GetNextProp3D()) {
		vtkActor* actor = dynamic_cast<vtkActor*>( prop3d );
		if ( actor != NULL ) {
			actor->GetProperty()->SetRepresentationToSurface();

			actor->GetProperty()->SetAmbient(0.0);
			actor->GetProperty()->SetDiffuse(1.0);
			actor->GetProperty()->SetSpecular(0.0);
		}
	}
}
void FXVTK3DObject::SetRepresentationToWireFrame() {
	Actors3D->InitTraversal();
	while (vtkProp3D* prop3d = Actors3D->GetNextProp3D()) {
		vtkActor* actor = dynamic_cast<vtkActor*>( prop3d );
		if ( actor != NULL ) {
			actor->GetProperty()->SetRepresentationToWireframe();

			actor->GetProperty()->SetAmbient(0.7);
			actor->GetProperty()->SetDiffuse(0.2);
			actor->GetProperty()->SetSpecular(0.1);
		}
	}
}
void FXVTK3DObject::SetRepresentationMode(FXVTK3DObject::REPRESENTATIONMODE mode) {
	switch (mode) {
		case REPRESENTATIONMODE_POINTS:
			SetRepresentationToPoints();
		break;
		case REPRESENTATIONMODE_WIREFRAME:
			SetRepresentationToWireFrame();
		break;
		case REPRESENTATIONMODE_SURFACE:
			SetRepresentationToSurface();
		break;
	}
}
FXVTK3DObject::REPRESENTATIONMODE FXVTK3DObject::GetRepresentationMode() {
	Actors3D->InitTraversal();
	vtkProp3D* prop3d = Actors3D->GetNextProp3D();
	vtkActor* actor;
	if ( prop3d != NULL && ( actor = dynamic_cast<vtkActor*>( prop3d ) ) ) {
		switch ((char) actor->GetProperty()->GetRepresentation()) {
			case VTK_POINTS:
				return REPRESENTATIONMODE_POINTS;
			break;
			case VTK_WIREFRAME:
				return REPRESENTATIONMODE_WIREFRAME;
			break;
			case VTK_SURFACE:
				return REPRESENTATIONMODE_SURFACE;
			break;
		}
	}
	return REPRESENTATIONMODE_POINTS;
}

void FXVTK3DObject::SetScale(double factor) {
	Actors3D->InitTraversal();
	while (vtkProp3D* actor = Actors3D->GetNextProp3D()) {
		actor->SetScale(factor);
	}
}
void FXVTK3DObject::SetScale( double x_scalefactor, double y_scalefactor, double z_scalefactor ) {
	Actors3D->InitTraversal();
	while (vtkProp3D* actor = Actors3D->GetNextProp3D()) {
		actor->SetScale( x_scalefactor, y_scalefactor, z_scalefactor ) ;
	}
}
void FXVTK3DObject::GetScale( double &x_scalefactor, double &y_scalefactor, double &z_scalefactor ) {
	if ( vtkProp3D* actor = Actors3D->GetLastProp3D() ) {
		double* scale = actor->GetScale();
		x_scalefactor = scale[0]; y_scalefactor = scale[1]; z_scalefactor = scale[2];
	}
	else {
		x_scalefactor = 0; y_scalefactor = 0; z_scalefactor = 0;
	}
}


bool FXVTK3DObject::IsVisibilityOn() {
	if (Actors3D == NULL) return false;
	Actors3D->InitTraversal();
	vtkProp3D* actor = Actors3D->GetNextProp3D();
	if (actor == NULL) return false;

	if (actor->GetVisibility())   return true;
	else   return false;
}

// On default, the first actor of the object will be set pickable On or Off
void FXVTK3DObject::SetPickableOn() {
	if (Actors3D != NULL) {
		Actors3D->InitTraversal();
		if ( vtkProp3D* actor = Actors3D->GetNextProp3D() )
			actor->PickableOn();
	}
}

bool FXVTK3DObject::IsPickable() {
	if (Actors3D != NULL) {
		Actors3D->InitTraversal();
		if (vtkProp3D* actor = Actors3D->GetNextProp3D()) {
			if (actor->GetPickable())
				return true;
			else
				return false;
		}
	}
	return false;
}