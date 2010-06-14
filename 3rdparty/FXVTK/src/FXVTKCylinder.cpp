// $Id: FXVTKCylinder.cpp,v 1.2 2009/04/20 14:29:26 giese Exp $

#include "FXVTKCylinder.h"
#include "vtkCylinderSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkSphericalTransform.h"
#include "vtkTransform.h"
// #include <math.h>

#define PI  3.14159265358979323846



FXVTKGetClassnameMacro( FXVTKCylinder )

void FXVTKCylinder::Initialization() {
	source = NULL;
	// Default-values:
	radius = 1;
	height = 1;
	phiResolution = 16;
	// zResolution = 4;
}
FXVTKCylinder::FXVTKCylinder() {
	Initialization();
}
FXVTKCylinder::FXVTKCylinder(FXVTKWidgetBaseClass* widget) : FXVTKSimpleObject(widget) {
	Initialization();
}
FXVTKCylinder::FXVTKCylinder(FXVTKWidgetBaseClass* widget, int userspecificid) : FXVTKSimpleObject(widget) {
	Initialization();
	SetUserSpecificID(userspecificid);
}

FXVTKCylinder::~FXVTKCylinder() {
	if (source != NULL) source->Delete();
}

void FXVTKCylinder::_update() {
	if ( IsUpToDate() ) return;

	if ( source == NULL ) source = vtkCylinderSource::New();

	source->SetRadius(radius);
	source->SetHeight(height);
	source->SetResolution(phiResolution);

	if (Mapper == NULL) {
		Mapper = vtkPolyDataMapper::New();
		Mapper->SetInput( source->GetOutput() );
		Actor->SetMapper(Mapper);
	}

	Mapper->Update();
	SetUpToDate();
}


void FXVTKCylinder::SetRadius(double r) {
	radius = r;
	SetChanged();
}

void FXVTKCylinder::SetHeight(double h) {
	height = h;
	SetChanged();
}


void FXVTKCylinder::SetStartEndPoints(double x1, double y1, double z1, double x2, double y2, double z2) {

	// Set set middle
	SetPosition( (x1+x2)/2, (y1+y2)/2, (z1+z2)/2 );


    // next lines for calculate rations
	double x = x2 - x1;
	double y = y2 - y1;
	double z = z2 - z1;

	
	height = sqrt(x*x + y*y + z*z);
	SetChanged();


	vtkSphericalTransform* transform = vtkSphericalTransform::New();
	double* out = transform->GetInverse()->TransformPoint(x, y, z);

	vtkTransform* transform2 = vtkTransform::New();
	transform2->Identity();						// default cylinder show in y
	transform2->RotateX( 90 );					// to let cylinder show in z
	transform2->RotateY( -(out[2]*180/PI) );	
	transform2->RotateZ( -(out[1]*180/PI) );
	

	double* temp = transform2->GetOrientation();
	SetOrientation( temp[0], temp[1], temp[2] );


	transform->Delete();
	transform2->Delete();

	//// copy and paste from FXVTKPlane::SetNormal(...) 
	//double rotx, roty;
	//double temp = sqrt(z * z + x * x);

	//// if x = y = z = 0, then normal in +y-direction
	//if (temp == 0) { // x = 0, z = 0;
	//	if (y >= 0) {
	//		rotx = -PI/2; roty = 0;
	//	}
	//	else {
	//		rotx = PI/2; roty = 0;
	//	}
	//}
	//// normal case:
	//else {
	//	if (x >= 0)	 roty = acos(z/temp);
	//	else		 roty = 2*PI - acos(z/temp);

	//	rotx = 2*PI - atan(y/temp);
	//}

	//double* old_rot = Actor->GetOrientation();
	//Actor->SetOrientation( rotx*(180/PI), roty*(180/PI), old_rot[2] );

}