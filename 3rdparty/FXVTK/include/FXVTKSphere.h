/*=============================================================================/
/							  F X V T K S p h e r e 						   /
/						     ( L i b :   F X V T K )						   /
/==========================================================================*//*!

   \class 		FXVTKSphere

   \brief 		To make a sphere.
	
   -no detailed description-


   \n
   $Id: FXVTKSphere.h,v 1.10 2009/04/20 14:29:26 giese Exp $ 

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/

#pragma once

#include "FXVTKSimpleObject.h"

class vtkSphereSource;
class vtkTextActor;


class FXVTKSphere : public FXVTKSimpleObject {

public:
	/// Constructor
	FXVTKSphere();
	/// Specified constructor, which also let connect with a widget (see FXVTKObject::connectWidget)
	FXVTKSphere(FXVTKWidgetBaseClass* widget);
	/// Specified constructor, which also let connect with a widget and set UserSpecificID.
	FXVTKSphere(FXVTKWidgetBaseClass* widget, int userspecificid);
	/// Destructor
	~FXVTKSphere();

public:
	/// Get name of class of the object
	virtual const char* GetClassname();

	/// Set the radius of the sphere
	void SetRadius(double r);
	/// Get the radius of the sphere
	inline double GetRadius() {
		return radius;
	}


	/// Set labels on
	void SetLabelsOn();
	/// Set labels off
	void SetLabelsOff();

	/// Set the Text of the left label
	void SetLabel1Text(const char* string);


private:
	void Initialization();
protected:
	virtual void _update();

protected:
	vtkSphereSource* source;
	int thetaResolution;
	int phiResolution;
	double radius;

	// Labels ( left and right )
	vtkTextActor* Labelactor1;
	vtkTextActor* Labelactor2;

	int LabelOldFontsize;
	bool IsLabelsOn;

};

