#ifndef VTK_DAFF_VIZ_TEST_WINDOW_
#define VTK_DAFF_VIZ_TEST_WINDOW_

#include <daffviz/DAFFVizSGNode.h>
#include <string>
#include <mutex>


// Forward declarations
class vtkRenderer;
class vtkRenderWindow;
class vtkCamera;
class vtkLight;
class vtkRenderWindowInteractor;

namespace DAFFViz
{
	// Forward declarations
	class Label;

	//! VTK windows for quick visualization of a DAFFViz scene graph node
	/**
	* This class implements a FOX frame that allows you to display a SGNode and its subtree
	* on a screen. You can interact with the camera and change in a certain extend the behaviour
	* and visualization of the VTK scene.
	*
	*/

	class VTKDAFFVizWindow
	{

	public:

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
		VTKDAFFVizWindow();

		//! Destructor
		~VTKDAFFVizWindow();

		//! Set the root node which shall represent the scene to be rendered
		/**
		* Tell the scene with which assembly node he shall begin to draw.
		*/
		void SetSceneGraphRootNode(DAFFViz::SGNode* node);
		
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
		void SetBackground( double red, double green, double blue);

		//! Get the background color of the scene
		void GetBackground(double &red, double &green, double &blue) const;


		// --= Camera =--

		//! Get camera object
		vtkCamera* getCamera();

		//! Set camera position
		void SetCameraPosition(double x, double y, double z);

		//! Get camera position
		void GetCameraPosition(double& x, double& y, double& z) const;

		//! Set focal point of camera
		void SetCameraFocalPoint( double x,  double y,  double z);

		//! Get focal point of camera
		void GetCameraFocalPoint(double& x, double& y, double& z) const;

		//! Dolly camera
		/**
		* Value of 1.0 will lead to not change of camera, 1.1 will dolly in a bit,
		* 0.9 will dolly out. In general dolly movement ranges around 1.0.
		*/
		void DollyCamera( double dDolly);

		//! Reset the camera
		//void ResetCamera();

		//! Set camera projection
		void SetParallelProjection();
		void SetPerspectiveProjection();

		//! Set camera to special type (front/left, top/bottom, 3d, see above for IDs)
		void SetCameraType( int iType);

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
		void SetCameraLightPosition( double x,  double y,  double z);

		//! Get camera light position
		void GetCameraLightPosition(double &x, double &y, double &z) const;

		//! Start rendering
		void Start();

	private:
		// Update interval [milliseconds]
		enum
		{
			TIME_INTERVAL = 35
		};

		DAFFViz::SGNode* m_pSceneRootNode;

		double m_dLastRootX;
		double m_dLastRootY;
		bool m_bInteractionLeftBtn;		// Left mouse button
		bool m_bInteractionMiddleBtn;	// Middle mouse button
		bool m_bInteractionRightBtn;	// Right mouse button

		int m_iRenderCount;			// Update counter for (re)rendering requests
		bool m_bRenderInProgress;	// Flag: Rendering is in progress
		bool m_bDestroyInProgress;	// Flag: Destruction initiated - no more updates
		std::mutex m_mxRenderCount;	// Synchronization of the update counter

		bool m_bCameraLight;


		// --= VTK important things =--

		vtkRenderer* m_pRenderer;
		vtkRenderWindow* m_pRenderWindow;
		vtkCamera* m_pCamera;
		int m_iCameraType;
		vtkLight* m_pCameraLight;
		vtkRenderWindowInteractor* m_pInteractor;

		// Add light to scene
		void AddLight(vtkLight* light);

		// Remove light from scene
		void RemoveLight(vtkLight* light);

		// Internal method that performs the rendering of the visuals
		// and resets the update counter.
		// This method is thread save.
		void RenderInternal();
	};

} // End of namespace "DAFFViz"

#endif // VTK_DAFF_VIZ_TEST_WINDOW_
