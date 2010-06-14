/*=============================================================================/
/							F X V T K C y l i n d e r  						   /
/						     ( L i b :   F X V T K )						   /
/==========================================================================*//*!

   \class 		FXVTKCylinder

   \brief 		To make a Cylinder.
	
   -no detailed description-


   \n
   $Id: FXVTKCylinder.h,v 1.2 2009/04/20 14:29:26 giese Exp $ 

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

class vtkCylinderSource;

class FXVTKCylinder : public FXVTKSimpleObject {

public:
	/// Constructor
	FXVTKCylinder();
	/// Specified constructor, which also let connect with a widget (see FXVTKObject::connectWidget)
	FXVTKCylinder(FXVTKWidgetBaseClass* widget);
	/// Specified constructor, which also let connect with a widget and set UserSpecificID.
	FXVTKCylinder(FXVTKWidgetBaseClass* widget, int userspecificid);
	/// Destructor
	~FXVTKCylinder();

public:
	/// Get name of class of the object
	virtual const char* GetClassname();

	/// Set the radius of the sphere
	void SetRadius(double r);
	/// Get the radius of the sphere
	inline double GetRadius() {
		return radius;
	}

	/// Set the radius of the sphere
	void SetHeight(double height);
	/// Get the radius of the sphere
	inline double GetHeight() {
		return height;
	}

	void SetStartEndPoints( double x1, double y1, double z1, double x2, double y2, double z2 );




private:
	void Initialization();
protected:
	virtual void _update();

private:
	vtkCylinderSource* source;
	double radius;
	double height;
	int phiResolution;
	// int zResolution;
};

