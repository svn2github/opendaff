// $Id: FXVTKSphere.cpp,v 1.11 2009/04/20 14:29:27 giese Exp $

#include "FXVTKSphere.h"
#include "FXVTKWidgetBaseClass.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkActor2DCollection.h"

#include <string>
#include <sstream>



FXVTKGetClassnameMacro( FXVTKSphere )

void FXVTKSphere::Initialization() {
	source = NULL;
	// Default-values:
	radius = 0.5;
	thetaResolution = 16;
	phiResolution = 8;


	// labels
	Labelactor1 = vtkTextActor::New();
	Labelactor2 = vtkTextActor::New();


	Labelactor1->SetInput("");
	Labelactor2->SetInput("");


	
	if ( Actors2D == NULL ) Actors2D = vtkActor2DCollection::New();
	Actors2D->AddItem( Labelactor1 );
	Actors2D->AddItem( Labelactor2 );


	// default labels off
	Labelactor1->VisibilityOff();
	Labelactor2->VisibilityOff();
	LabelOldFontsize = -1;
	IsLabelsOn = false;

}
FXVTKSphere::FXVTKSphere() {
	Initialization();
}
FXVTKSphere::FXVTKSphere(FXVTKWidgetBaseClass* widget) : FXVTKSimpleObject(widget) {
	Initialization();
	DisconnectAllWidgets();
	ConnectWidget(widget);
}
FXVTKSphere::FXVTKSphere(FXVTKWidgetBaseClass* widget, int userspecificid) : FXVTKSimpleObject(widget) {
	Initialization();
	SetUserSpecificID(userspecificid);
	DisconnectAllWidgets();
	ConnectWidget(widget);
}

FXVTKSphere::~FXVTKSphere() {
	if (source != NULL) source->Delete();

	if ( Labelactor1 != NULL ) Labelactor1->Delete();
	if ( Labelactor2 != NULL ) Labelactor2->Delete();

}

void FXVTKSphere::_update() {

	if ( IsVisibilityOn() && IsLabelsOn == true ) {
		// this muss be do on all rendering, if labels are visible

		if ( LabelOldFontsize = -1 ) {

			// Setting for labels font, once time and on the first 
			vtkTextProperty* textprop;

			textprop = Labelactor1->GetTextProperty();
			textprop->SetJustificationToRight();
			textprop->SetVerticalJustificationToCentered();

			textprop = Labelactor2->GetTextProperty();
			textprop->SetJustificationToLeft();
			textprop->SetVerticalJustificationToCentered();

		}


		double x, y, z, x0, y0, x1, y1, x2, y2, x3, y3, delta_x;
		GetPosition(x, y, z);

		double r = radius;
		
		if ( (WidgetList[0])->GetDisplayValueOfWorldCoorinate( x, y, z, x0, y0) == true ) { 

			(WidgetList[0])->GetDisplayValueOfWorldCoorinate( x - r, y, z, x1, y1);
			(WidgetList[0])->GetDisplayValueOfWorldCoorinate( x, y - r, z, x2, y2);
			(WidgetList[0])->GetDisplayValueOfWorldCoorinate( x, y, z - r, x3, y3);
			

			delta_x = sqrt((x0-x1)*(x0-x1)+(x0-x2)*(x0-x2)+(x0-x3)*(x0-x3));

			// font scaling
			int fontsize = (int)(1.2 * delta_x );
			if ( fontsize > 120 ) fontsize = 120;
			if ( fontsize != LabelOldFontsize ) {
				Labelactor1->GetTextProperty()->SetFontSize( fontsize );
				Labelactor2->GetTextProperty()->SetFontSize( fontsize );
			}
			LabelOldFontsize = fontsize;


			// position of labels
			// Labelactor1->SetPosition( x0-delta_x - 3, y0 );
			// Labelactor2->SetPosition( x0+delta_x + 3, y0 );
			Labelactor1->SetDisplayPosition( x0-delta_x - 3, y0 );
			Labelactor2->SetDisplayPosition( x0+delta_x + 3, y0 );

			Labelactor1->VisibilityOn();
			Labelactor2->VisibilityOn();

		}
		else {

			Labelactor1->VisibilityOff();
			Labelactor2->VisibilityOff();

		}

		
		
		if ( IsUpToDate() ) {
			// Widget think something is change for next Render, so he call _update again on next render.
			SetChanged();
			SetUpToDate();
			return;
		}
		else {
			SetChanged();
		}
	}

	if ( IsUpToDate() ) return;
		
	if ( source == NULL ) source = vtkSphereSource::New();

	source->SetRadius(radius);
	source->SetThetaResolution(thetaResolution);
	source->SetPhiResolution(phiResolution);

	if (Mapper == NULL) {
		Mapper = vtkPolyDataMapper::New();
		Mapper->SetInput( source->GetOutput() );
		Actor->SetMapper( Mapper );
	}



	if ( IsLabelsOn == true ) {
		std::stringstream id_string;
		id_string << GetUserSpecificID() ;
		Labelactor2->SetInput(id_string.str().c_str());
	}


	Mapper->Update();
	SetUpToDate();
}


void FXVTKSphere::SetRadius(double r) {
	radius = r;
	SetChanged();
}


// *** labels ***
void FXVTKSphere::SetLabelsOn() {
	IsLabelsOn = true;
	Labelactor1->VisibilityOn();
	Labelactor2->VisibilityOn();
	SetChanged();
}

void FXVTKSphere::SetLabelsOff() {
	IsLabelsOn = false;
	Labelactor1->VisibilityOff();
	Labelactor2->VisibilityOff();
}

void FXVTKSphere::SetLabel1Text( const char* string ) {
	if ( string != NULL ) Labelactor1->SetInput(string);
	else Labelactor1->SetInput("");
}


