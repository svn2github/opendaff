/*=============================================================================/
/							F X V T K O b j e c t							   /
/						   ( L i b :   F X V T K )							   /
/==========================================================================*//*!

   \class 		FXVTKObject

   \brief 		Abstract base class for all "objects" in FXVTK. They working 
				with FXVTKWidget.
	
   With the derived classes of FXVTKObject you can easy work with vtk, without 
   using vtk classes. Also you needn't include header-files of vtk. 
   You can also connect objects of this class with "widget-objects" (derived 
   classes of FXVTKWidgetBaseClass), so it can disappear on this widget.

   FXVTKObjects is subdiveded into FXVTK3DObject and FXVTK2DObject. 3D-objects
   have a volume and can be placed over the complete world. 2D objects are "flat",
   have no z-component and normally placed "on the widget". 
   
   More details in  the description of the derived classes.

   \n
   $Id: FXVTKObject.h,v 1.1 2010/02/10 09:57:25 schmoch Exp $

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/





/*! \mainpage FXVTK Main Page
\image html fxvtk.jpg
\image latex fxvtk.eps "" width=8cm

\section intro_sec Introduction
FXVTK is a libary, which has to simplefy you the work with the Visualiuation 
Toolkit and the FOX Toolkit.

FXVTK is shared in two parts, in the "widget-classes" (derived classes of 
FXVTKWidgetBaseClass) and the "object-classes" (FXVTKObject and direved classes).
A "widget-class" is a FOX-Frame, which make it possible to show you pictures of
an VTK Renderer Object or normally an object of an "object-class". 
"object-classes" makes prepared objects. Internal they create VTK Objects, 
VTK Polygons, etc., which can be displayed in the FXVTKWidget.


\n\n
\section install_sec Installation Info
If you want only execute FXVTK-Programm you must only do Step 1. Otherwise you
must do all install steps:

\subsection step1 Step 1: For Execute FXVTK Files
For execute FXVTK-files you need the FOX-Toolkit and the VTK libs and binarys.
Either the libarypath of the dlls of FOX and VTK must set in your 'PATH' 
environment variable or you must copy the files in the bin PATH of the 
execute-file.



\subsection step2 Step 2: MVS project properties - C/C++ settings
First you must set the Include-Path of FXVTK and FOX (optional:VTK) to the list 
of include-paths:

\image html properties-1.gif
\image latex properties-1.eps "" width=14cm

Also you need the to set "FOXDLL" in the prepocessor-definion 
for use the dll-version of FOX-Toolkit:

\image html properties-2.gif
\image latex properties-2.eps "" width=14cm



\subsection step3 Step 3: MVS project properties - Linker settings
You must set the lib paths of FXVTK, FOX and VTK as additional lib path.
In Debug-configuration you can choose either the path "relwithdebinfo" or
the path "debug" for vtk.

\image html properties-3.gif
\image latex properties-3.eps "" width=15.68cm



Now you must load all libs of FXVTK, FOX and VTK as additional dependencies
in your project. Here are the entries: 

FXVTKD.lib FOXDLLD-1.6.lib vtkCommon.lib vtkDICOMParser.lib vtkexoIIc.lib vtkexpat.lib 
vtkFiltering.lib vtkfreetype.lib vtkftgl.lib vtkGenericFiltering.lib vtkGraphics.lib 
vtkHybrid.lib vtkImaging.lib vtkIO.lib vtkjpeg.lib vtkNetCDF.lib vtkpng.lib 
vtkRendering.lib vtksys.lib vtktiff.lib vtkVolumeRendering.lib vtkWidgets.lib vtkzlib.lib

For the Release-version you should choose FXVTK.lib and FOXDLL-1.6.lib instead of FXVTKD.lib FOXDLLD-1.6.lib

\image html properties-4.gif
\image latex properties-4.eps "" width=14cm



As Entry-point you must set "mainCRTStartup" (for FOX):

\image html properties-5.gif
\image latex properties-5.eps "" width=14cm


\n\n
\section autor Autor(s)
cand. ing. Jochen Giese	 (jochen.giese@rwth-aachen.de)

Institute of Technical Acoustics \n
RWTH Aachen University, 2007 \n
http://www.akustik.rwth-aachen.de \n   */



#pragma once
#include <string>
#include <vector>

class vtkActor2DCollection;
class vtkProp3DCollection;
class vtkProp;
class FXVTKWidgetBaseClass;


class FXVTKObject {

	friend class FXVTKWidgetBaseClass;  // Friendship because of widgets can use _update()-method


public:
	/// Constructor 
	FXVTKObject(void);
	/// Destructor
	~FXVTKObject(void);


public:
	/// Point-struct (3D-point)
	struct Point {
		double x;	///< X-value
		double y;	///< Y-value
		double z;	///< Z-value
	};

	/// Orientation-struct (rotationorder: Z,X,Y)
	struct Orientation {
		double RotX;  ///< Rotationangle around X-axis (rotationorder: Z,X,Y)
		double RotY;  ///< Rotationangle around Y-axis (rotationorder: Z,X,Y)
		double RotZ;  ///< Rotationangle around Z-axis (rotationorder: Z,X,Y)
	};

	/// Enum for select the mode of rounding
	enum ROUNDINGMODE {
		ROUNDINGMODE_NORMAL = 0,  ///< Rounding to XXX0 *10^(n+1)
		ROUNDINGMODE_125,		  ///< Rounding to XXX1 *10^n, XXX2 *10^n or XXX5 *10^n
		ROUNDINGMODE_DOWN,		  ///< Rounding down to XXX0 *10^(n+1)
		ROUNDINGMODE_DOWN_125,	  ///< Rounding down to XXX1 *10^n, XXX2 *10^n or XXX5 *10^n
		ROUNDINGMODE_NONE		  ///< No rounding
	};


public:
	/// Get name of class of the object
	//virtual const char* GetClassname() = 0;
	virtual const char* GetClassname() { return ""; }


	/// Set UserSpecificID (which help user to identify objects)
	void SetUserSpecificID(int id);
	
	/// Get UserSpecificID (which help user to identify objects)
	int GetUserSpecificID();


	// Set UserSpecificName of the object ( string will be copied ).
	void SetUserSpecificName(char* name);

	// Get UserSpecificName of the object, return NULL, if no name is set.
	inline const char* GetUserSpecificName() {
		return UserSpecificName.c_str();
	}


	/// Let connect this "object" with a widget, so it can appear on / used with this "widget".
	virtual void ConnectWidget(FXVTKWidgetBaseClass* widget);

	/// Disconnect the widget and this object, this object will disappear.
	virtual bool DisconnectWidget(FXVTKWidgetBaseClass* widget);

	/// Disconnect all connected widgets, so this object don't used by some "object".
	virtual void DisconnectAllWidgets();


	/// Simplefie the rendering of all "widgets", which are connected with this "object".
	void RenderAllConnectedWidgets();

	/// Get the first connected widget. Return NULL if no Connected Widget;
	FXVTKWidgetBaseClass* GetFirstConnectedWidget();

	/// If "object" is up to date, return value will be true.
	bool IsUpToDate();

	/// Set an object pickable, so it can catched with FXVTKWidget::PickObject(...) and so on.
	virtual void SetPickableOn() = 0;

	/// Set an object non-pickable, so it can't catched with FXVTKWidget::PickObject(...) and so on.
	virtual void SetPickableOff();

	/// Return true, if object is pickable
	virtual bool IsPickable() = 0;

	/// Set the visibility of the object on
	virtual void VisibilityOn();

	/// Set the visibility of the object off
	virtual void VisibilityOff();

	/// Returns true, if object is visible
	virtual bool IsVisibilityOn() = 0;




protected:
	/// Internal method, which calculate new data, if something changed.
	/*! This method will automatically started by FXVTKWidget::Render() and other Render-methods,
		if something changed. Then the calculation of the "3D-object" will be (re)newed, if
		it will be necessary. If you wan't set the time of update manually, and it would make
		sense to to do this, there will be a method named Update(), which call _update(). */
	virtual void _update();

	/// Internal method, which must called, if _update() should be run on next render.
	/*! This internal method should be called, if something changed, which has effects on
		the "object". it shouldn't called for changes of position or orientation in the "widget".
		SetChanged marks the object (UpToDate will set to false) so that it will be run on next
		render or update. */
	void SetChanged();

	/// Internal method, which should be called, if after update-processing "object" is up to date.
	void SetUpToDate();

	/// Initialization-method, which called in the constructors of same class
	void Initialization();


protected:
	/// \<vector\>-List which consist of all widgets, which connected with this "object"
	std::vector<FXVTKWidgetBaseClass*> WidgetList;

	/// A collection of all vtkActor's (3D) which represent the "object".
	vtkProp3DCollection* Actors3D;
	/// A collection of all vtkActor2d's (2D) which represent the "object".
	vtkActor2DCollection* Actors2D;
	/// A prop, if one vtk object can't save in the selections before, e.x. vtkwidget
	vtkProp* SingleVtkProp;



private:
	/// Show you if the "object" is up to date and so update doesn't must called on render.
	bool UpToDate;

	/// ID which set from user, to help the user to identify this object
	/*! UserSpecificID is only for the user, who use FXVTK. The variables don't used by functions of FXVTK. */
	int UserSpecificID;

	/// NAME which set from user, to help the user to identify this object
	std::string UserSpecificName;
};



#define FXVTKGetClassnameMacro( NAME )    \
	const char* NAME::GetClassname() {    \
		static char classname[] = #NAME ;  \
		return classname;                 \
	}