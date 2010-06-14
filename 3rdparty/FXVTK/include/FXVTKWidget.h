/*=============================================================================/
/							F X V T K W i d g e t							   /
/						   ( L i b :   F X V T K )							   /
/==========================================================================*//*!

   \class 		FXVTKWidget

   \brief 		Let you make a VTK Window as a Frame in FOX-Toolkit. FXVTKObjects
				can displayed on it.
	
   - temporary no discription -													   


   \n
   $Id: FXVTKWidget.h,v 1.1 2010/02/10 09:57:25 schmoch Exp $

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/

#pragma once

#include "FXVTKWidgetBaseClass.h"
#include <fx.h>


class FXVTKWidget : public FXVTKWidgetBaseClass, public FXFrame {

	// ***************************** Methods for FOX *****************************
public:
	FXDECLARE(FXVTKWidget)	

protected:
	/// Constructor 
	FXVTKWidget() {};
	/// Destructor
	~FXVTKWidget(void);

	virtual void create();
	virtual void detach();
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();

public:
	/// Public constructor
	FXVTKWidget(FXComposite* p,FXuint opts=FRAME_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,
		FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD, FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

	long onPaint(FXObject*,FXSelector,void *ptr);
	long onTimeout(FXObject*,FXSelector,void *ptr);
	long onLeftBtnPress(FXObject*,FXSelector,void*);
	long onLeftBtnRelease(FXObject*,FXSelector,void*);
	long onMiddleBtnPress(FXObject*,FXSelector,void*);
	long onMiddleBtnRelease(FXObject*,FXSelector,void*);
	long onRightBtnPress(FXObject*,FXSelector,void*);
	long onRightBtnRelease(FXObject*,FXSelector,void*);
	long onMouseMotion(FXObject*,FXSelector,void*);
	long onLeave(FXObject*,FXSelector,void*);
// *************************** END: Methods for FOX ***************************




	// enum { TIME_INTERVAL = 50000000,}; // <-- 50ms entspricht max 20Hz. (fuer fox-version: 1.7.X)
	enum { TIME_INTERVAL = 5,}; // <-- 5ms entspricht max 200Hz (praktisch meistens so bei 90Hz).

	enum {
		FXVTKIS_NOTHING=0,		// <-- ??? Name? 
		FXVTKIS_ROTATE,
		FXVTKIS_DOLLY,
		FXVTKIS_PAN,
		FXVTKIS_SPIN,
	};


public:
	enum{
		ID_TIMEOUT=FXFrame::ID_LAST,
		ID_LAST,
	};

// *************************** END: Methods for FOX ***************************


public:

	void Dolly(int, int);
	void Pan(int, int);
	void Rotate(int, int);
	void Spin(int, int);
	void StopAnimate(void);

	void SetMouseMotionFunction( void (*pt2Func)(void *savedPtr, FXVTKWidget*), void* savedPtr );

	inline void SetMouseCameraInteractionOn() {
		MouseCameraInteraction = true;
	}
	inline void SetMouseCameraInteractionOff() {
		StopAnimate();
		MouseCameraInteraction = false;
	}
	inline bool IsMouseCameraInteraction() {
		return MouseCameraInteraction;
	}


private:
	FXTimer *timer;                     // Timer for spinning box

	void (*mouseMotionFunction)(void*,FXVTKWidget*);   // function which is called, is mouse over widget and moved
	void* mouseMotionFunctionSavedPtr;
	int state; // <-- ??? Name? 

	bool MouseCameraInteraction;
};
