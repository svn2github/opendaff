/*=============================================================================/
/						F X V T K S i m p l e O b j e c t					   /
/						     ( L i b :   F X V T K )						   /
/==========================================================================*//*!

   \class 		FXVTKSimpleObject

   \brief 		Abstract base class of all simple 3D bodys and 
				others simple 3D-objects.
	
   -no detailed description-


   \n
   $Id: FXVTKSimpleObject.h,v 1.8 2008/10/27 15:34:22 giese Exp $ 

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/


#pragma once
#include "FXVTK3DObject.h"

class vtkPolyDataMapper;
class vtkActor;


class FXVTKSimpleObject : public FXVTK3DObject {

public:
	/// Constructor
	FXVTKSimpleObject(void);
	/// Specified constructor, which also let connect with a widget (see FXVTKObject::connectWidget)
	FXVTKSimpleObject(FXVTKWidgetBaseClass*);
	/// Destructor
	~FXVTKSimpleObject(void);

public:
	/// Set color of the object in RGB-code (0.0 ... 1.0)
	virtual void SetColor(double r, double g, double b);
	/// Get color of the object in RGB-code (0.0 ... 1.0)
	void GetColor(double &r, double &g, double &b);

	/// Set the opacity (alpha-value) (0.0 ... 1.0)
	void SetAlpha(double a);
	/// Get the opacity (alpha-value) (0.0 ... 1.0)
	double GetAlpha();


protected:
	virtual void _update() = 0;

private:
	void Initialization();


protected:
	vtkPolyDataMapper* Mapper;
	vtkActor* Actor;
};