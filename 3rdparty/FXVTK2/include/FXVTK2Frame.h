/*
 * ---------------------------------------------------------------------------
 *
 *							FXVTK2 Visualization Library
 *             (c) Copyright Institute of Technical Acoustics (ITA)
 *              RWTH Aachen University, Aachen, Germany, 2011-today
 *
 * ---------------------------------------------------------------------------
 *						    ____  __________  _______
 *						   //  / //__   ___/ //  _   |
 *						  //  /    //  /    //  /_|  |
 *						 //  /    //  /    //  ___   |
 *						//__/    //__/    //__/   |__|
 *
 * ---------------------------------------------------------------------------
 *
 *    File:			FXVTK2Frame.h
 *
 *    Purpose:		FOX frame for visualization of a FXVTK2 scene graph
 *
 *    Autors:		Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de
 *					Jonas Stienen (stienen@akustik.rwth-aachen.de)
 *
 *  ---------------------------------------------------------------------------
 *
 */

// $Id: FXVTK2Frame.h 1853 2011-05-19 13:58:54Z stienen $

#ifndef __FXVTK2_WIDGET_H__
#define __FXVTK2_WIDGET_H__

#include <fx.h>
#include <FXVTK2SGNode.h>
#include <string>

// Forward declarations
class vtkRenderer;
class vtkRenderWindow;
class vtkCamera;
class vtkLight;

namespace FXVTK2 {

// Forward declarations
class Label;

//! FOX frame for visualization of a FXVTK2 scene graph
/** 
 * This class implements a FOX frame that allows you to display a SGNode and its subtree
 * on a screen. You can interact with the camera and change in a certain extend the behaviour
 * and visualization of the VTK scene.
 *
 */

class Frame : public FXFrame {
	FXDECLARE(FXVTK2::Frame)

public:
	//! FOX identifiers
	enum {
		ID_TIMEOUT=FXFrame::ID_LAST,
		ID_INTERACTION,
		ID_LAST,
	};

	//! Mouse interaction modes
	enum {
		MI_DISABLED=0,
		MI_AZIMUTH_ELEVATION,	//!< Left mouse button movement => azimuth and elevation, scroll wheel or right mouse button horizontal movement => zoom
		MI_MOVE_AND_ZOOM,		//!< Left mouse button movement => move x and y direction on the plane, scroll wheel or right mouse button horizontal movement => zoom
		MI_DIRECTIONAL_DATA		//!< Left mouse button movement => azimuth and elevation, right mouse button => zoom, scroll wheel switch frequencies
	};

	//! Camera type
	enum {
		CAMERA_NONE = 0,
		CAMERA_TOP,
		CAMERA_BOTTOM,
		CAMERA_LEFT,
		CAMERA_RIGHT,
		CAMERA_FRONT,
		CAMERA_BACK
	};

	//! Constructor
	Frame(FXComposite* p, FXuint opts=FRAME_NORMAL,
				 FXint x=0, FXint y=0, FXint w=0, FXint h=0,
				 FXint pl=DEFAULT_PAD, FXint pr=DEFAULT_PAD, FXint pt=DEFAULT_PAD, FXint pb=DEFAULT_PAD);

	//! Set the root node which shall represent the scene to be rendered
	/**
	  * Tell the scene with which assembly node he shall begin to draw.
	  */
	void SetSceneGraphRootNode(FXVTK2::SGNode* node);

	//! Activate mouse interaction (default is active)
    void SetMouseInteractionMode(const int iMode);

	//! Get mouse interaction mode
    int GetMouseInteractionMode() const;

	//! Set the FXSlider to which mousewheel events will be forwarded to, when in MI_DIRECTIONAL_DATA mode
	void setFrequencySelector(FXSlider*);

	//! Save a Screenshot to File
	void SaveScreenshot(const std::string& sFilename);

	// --= VTK methods =--

	//! (Re)render the graphics
	/**
	 * Request the (re)rendering of the graphics.
	 * This method is non-blocking and sends a rendering request to
	 * a background timer, which will update the visuals.
	 * Hence it can be called as often as desired, without
	 * leading to computational overhead. The maximum frame rate
	 * (update rate) is give by TODO...
	 */
	virtual void Render();

	//! Set the background color of the scene
	void SetBackground(const double red, const double green, const double blue);

	//! Get the background color of the scene
	void GetBackground(double &red, double &green, double &blue) const; 


	// --= Camera =--

	//! Get camera object
	vtkCamera* getCamera();

	//! Set camera position
	void SetCameraPosition(const double x, const double y, const double z);

	//! Get camera position
	void GetCameraPosition(double& x, double& y, double& z) const;

	//! Set focal point of camera
	void SetCameraFocalPoint(const double x, const double y, const double z);

	//! Get focal point of camera
	void GetCameraFocalPoint(double& x, double& y, double& z) const;

	//! Dolly camera
	/**
	  * Value of 1.0 will lead to not change of camera, 1.1 will dolly in a bit,
	  * 0.9 will dolly out. In general dolly movement ranges around 1.0.
	  */
	void DollyCamera(const double dDolly);

	//! Reset the camera
	//void ResetCamera();

	//! Set camera projection
	void SetParallelProjection();
	void SetPerspectiveProjection();

	//! Set camera to special type (front/left, top/bottom, 3d, see above for IDs)
	void SetCameraType(const int iType);

	//! Set this frame's camera as the target for followers in the scene
	void SetFollowerTarget();

	// --= Lighting =--

	//! Set camera light on
	void SetCameraLightOn();
	
	//! Set camera light off
	void SetCameraLightOff();
	
	//! Returns true if camera light is on
	bool CameraLightIsOn() const;
	
	//! Set camera light position
	void SetCameraLightPosition(const double x, const double y, const double z);
	
	//! Get camera light position
	void GetCameraLightPosition(double &x, double &y, double &z) const;


	// --= FOX event methods =--

	//! Render screen
	long onPaint(FXObject*, FXSelector, void*);

	//! Update method for repainting based on update interval
	long onTimeout(FXObject*, FXSelector, void*);

	//! Mouse interaction methods
	long onLeftBtnPress(FXObject*, FXSelector, void*);
	long onLeftBtnRelease(FXObject*, FXSelector, void*);
	long onMiddleBtnPress(FXObject*, FXSelector, void*);
	long onMiddleBtnRelease(FXObject*, FXSelector, void*);
	long onRightBtnPress(FXObject*, FXSelector, void*);
	long onRightBtnRelease(FXObject*, FXSelector, void*);
	long onMouseWheelScroll(FXObject*, FXSelector, void*);

	//! Interaction methods
	//long onLeave(FXObject*, FXSelector, void*);
	long onMouseMotion(FXObject*, FXSelector, void*);
	long onEnter(FXObject*, FXSelector, void*);

protected:
	//! Constructor 
	Frame() {};

	//! Destructor
	~Frame();

	//! Create frame and connect it with render window
	void create();


	//! Detach frame
	void detach();

private:
	// Update interval [milliseconds]
	enum {
		TIME_INTERVAL = 35
	};

	int m_iMouseInteractionMode;
	FXVTK2::SGNode* m_pSceneRootNode;

	double m_dLastRootX;
	double m_dLastRootY;
	bool m_bInteractionLeftBtn;		// Left mouse button
	bool m_bInteractionMiddleBtn;	// Middle mouse button
	bool m_bInteractionRightBtn;	// Right mouse button

	int m_iRenderCount;			// Update counter for (re)rendering requests
	bool m_bRenderInProgress;	// Flag: Rendering is in progress
	bool m_bDestroyInProgress;	// Flag: Destruction initiated - no more updates
	FXMutex m_mxRenderCount;	// Synchronization of the update counter
	
	bool m_bCameraLight;


	// --= VTK important things =--

	vtkRenderer* m_pRenderer;
	vtkRenderWindow* m_pRenderWindow;
	FXSlider* m_pFrequencySelector;
	vtkCamera* m_pCamera;
	int m_iCameraType;
	vtkLight* m_pCameraLight;

	// Add light to scene
	void AddLight(vtkLight* light);
	
	// Remove light from scene
	void RemoveLight(vtkLight* light);

	// Internal method that performs the rendering of the visuals
	// and resets the update counter.
	// This method is thread save.
	void RenderInternal();
};

} // End of namespace "FXVTK2"

#endif // __FXVTK2_WIDGET_H__
