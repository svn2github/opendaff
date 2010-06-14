/*=============================================================================/
/						  F X V T K 2 D O b j e c t							   /
/						   ( L i b :   F X V T K )							   /
/==========================================================================*//*!

   \class 		FXVTK2DObject

   \brief 		Abstract base class for all "objects" in FXVTK. They working 
				with FXVTKWidget.
	
   With the derived classes of FXVTK2DObject you can easy work with vtk,
   without including headerfiles of vtk. You can also connect objects of
   this class with an object of FXVTKWidget, so it can disappear on the widget.


   \n
   $Id: FXVTK2DObject.h,v 1.1 2010/02/10 09:57:25 schmoch Exp $

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


class vtkActor2DCollection;
class vtkActor2D;


class FXVTK2DObject : public FXVTKObject {

public:
	/// Constructor 
	FXVTK2DObject(void);
	/// Destructor
	~FXVTK2DObject(void);


public:

	/// Set the position of the object in the connected widget(s)
	void SetPosition(double x, double y);
	
	/// Set the relativ position of the 2D object [expedient values: (0,0)...(1,1)].
	/*!	The relativ position (1,1) is the point at the upper-right corner of the widget. */
	void SetRelativePosition(double x, double y);

	/// Set the virtuell position in the 3D-world of the widget
	void Set3DPosition(double x, double y, double z);
	
	/// Get the position of the object in the connected widget(s)
	void GetPosition(double &x, double &y);


	virtual bool IsVisibilityOn();

	// description q.v. FXVTKObject
	virtual void SetPickableOn();
	virtual bool IsPickable();


private:
	/// Initialization-method, which called in the constructors of same class
	void Initialization();
};


