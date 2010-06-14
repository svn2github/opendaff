/*=============================================================================/
/							F X V T K O b j e c t							   /
/						   ( L i b :   F X V T K )							   /
/==========================================================================*//*!

   \class 		FXVTK3DObject

   \brief 		Abstract base class for all "objects" in FXVTK. They working 
				with FXVTKWidget.
	
   With the derived classes of FXVTK3DObject you can easy work with vtk,
   without including headerfiles of vtk. You can also connect objects of
   this class with an object of FXVTKWidget, so it can disappear on the widget.


   \n
   $Id: FXVTK3DObject.h,v 1.1 2010/02/10 09:57:25 schmoch Exp $

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/




#pragma once

#include "FXVTKObject.h"




class FXVTK3DObject : public FXVTKObject {

public:
	/// Constructor 
	FXVTK3DObject(void);
	/// Destructor
	~FXVTK3DObject(void);

public:
	enum REPRESENTATIONMODE {
		REPRESENTATIONMODE_POINTS = 0,
		REPRESENTATIONMODE_WIREFRAME,
		REPRESENTATIONMODE_SURFACE,
	};

public:

	/// Set the position of the object in the connected widget(s)
	virtual void SetPosition(double x, double y, double z);
	virtual void SetPosition(double* xyz_array);
	virtual void SetPosition(Point &point);
	
	/// Get the position of the object in the connected widget(s)
	void GetPosition(double &x, double &y, double &z);
	Point GetPosition();

	/// Set the orientation of the object in the connected widget(s)
	/** orientation order Z X Y **/
	void SetOrientation(double RotateX, double RotateY, double RotateZ);
	void SetOrientation(Orientation orientation);

	/// Get the orientation of the object in the connected widget(s)
	void GetOrientation(double &RotX, double &RotY, double &RotZ);
	Orientation GetOrientation();

	/// 
	void SetRepresentationToPoints();
	void SetRepresentationToSurface();
	void SetRepresentationToWireFrame();
	void SetRepresentationMode(FXVTK3DObject::REPRESENTATIONMODE mode);
	FXVTK3DObject::REPRESENTATIONMODE GetRepresentationMode();

	///
	void SetScale( double factor );
	void SetScale( double x_scalefactor, double y_scalefactor, double z_scalefactor );
	void GetScale( double &x_scalefactor, double &y_scalefactor, double &z_scalefactor );

	virtual bool IsVisibilityOn();

	// description q.v. FXVTKObject
	virtual void SetPickableOn();
	virtual bool IsPickable();

private:
	/// Initialization-method, which called in the constructors of same class
	void Initialization();
};


