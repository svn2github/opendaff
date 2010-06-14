/*=============================================================================/
/					F X V T K W i d g e t B a s e C l a s s					   /
/						   ( L i b :   F X V T K )							   /
/==========================================================================*//*!

   \class 		FXVTKWidgetBaseClass

   \brief 		Let you make a VTK Window as a Frame in FOX-Toolkit. FXVTKObjects
				can displayed on it.
	
   - temporary no discription -													   


   \n
   $Id: FXVTKWidgetBaseClass.h,v 1.1 2010/02/10 09:57:25 schmoch Exp $

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/

#pragma once
#define _FXVTKWIDGETBASECLASS_H


#define FXVTKCompile3DTestMethods 0		// Methoden zum Testen werden mit Wert 1 kompiliert


#include <list>

class vtkActor;
class vtkActor2D;
class vtkProp3DCollection;
class vtkActor2DCollection;
class vtkRenderer;
class vtkRenderWindow;
class vtkProp;
class vtkCamera;
class vtkPoints;
class vtkTextActor;
class vtkLight;
class vtkCoordinate;

class vtkWindowToImageFilter;
class vtkBMPWriter;
class vtkJPEGWriter;
class vtkPNGWriter;
class vtkPostScriptWriter;
class vtkTIFFWriter;
class vtkAVIWriter;
// class vtkMPEG2Writer;
// class vtkRenderWindowInteractor;

class FXVTKObject;
class FXVTK3DObject;
class FXVTK2DObject;
class vtkRenderer;
class vtkCellPicker;


class FXVTKWidgetBaseClass {

	friend class FXVTKObject;  // friendship because of objects can use _addObject _delObject function
	friend class FXVTKInfoBox;  // friendship because of objects can use _addObject _delObject function
	friend class FXVTKRingAxis;   // friendship because of objects can get WidgetCamera
	friend class FXVTKPolygons;   // friendship because of objects can get WidgetCamera
	friend class FXVTKLight;   // friendship because of dummyobject of a Light and use _addLight _delLight function


protected:
	/// Constructor 
	FXVTKWidgetBaseClass();
	/// Destructor
	~FXVTKWidgetBaseClass();



public:
	
	/// Let render the widget
	virtual void Render();

	/// Return false if the next rendering is the first.
	inline bool IsRenderedBefore() {
		if (Camera == NULL) 
			return false;
		else 
			return true;
	}

	/// Let all connected objects updating (automatically started before every rendering of the widget) 
	void UpdateConnectedObjects();

	/// Disconnect all connected objects, after this method (and render), no FXVTK object will be in the widget
	void DisconnectAllConnectedObjects();



	/// Set the Backgroundcolor of the widget
	void SetBackgroundColor(const double red, const double green, const double blue); 
	/// Get the Backgroundcolor of the widget
	void GetBackgroundColor(double &red, double &green, double &blue); 

	/// Get the size of the widget (in pixel);
	void GetWidgetSize(int &x, int &y);
	/// Get the size of the widget (in pixel);
	int* GetWidgetSize();

	
	///
	bool GetDisplayValueOfWorldCoorinate(const double x, const double y, const double z, double &display_x, double &display_y );



	// *********************** Camera ************************
	/// Set the Position of the Camera
	void SetCameraPosition(const double x, const double y, const double z);
	/// Get the Position of the Camera
	/** Return NULL if at the moment there is no camera and so a camera-position. 
	    A camera will automatically create on first render or if you set the camera-position */
	double* GetCameraPosition();

	void SetCameraRoll(double roll);

	double GetCameraRoll();

	void RollCamera( double angle );

	void DollyCamera( double distance );

	/// Reset the Camera
	void ResetCamera();


	
	// ********************* CameraLight *********************
	/// Switch CameraLight on (default)
	void SetCameraLightOn();
	/// Switch CameraLight off
	/** If no Light is switched on, a default light will created by VTK */
	void SetCameraLightOff();
	/// Return true if CameraLight is switched on
	bool CameraLightIsOn() const {
		return CameraLightIsOn_;
	}
	/// Set the relative position to the camera, if camera was on distance of 1 to middle of world
	void SetCameraLightPosition(double x, double y, double z);
	/// Get the relative position to the camera, if camera was on distance of 1 to middle of world
	void GetCameraLightPosition(double &x, double &y, double &z);


	/// Pick an FXVTK3DObject in the window (objects must be pickable).
	/** Return NULL if no FXVTK3DObject was picked. 	*/
	FXVTK3DObject* Pick3DObject(double selectionX, double selectionY);

	/// Pick an FXVTK2DObject in the window (objects must be pickable).
	/** Return NULL if no FXVTK2DObject was picked. 	*/
	FXVTK2DObject* Pick2DObject(double selectionX, double selectionY);

	/// Pick an FXVTK2DObject in the window (objects must be pickable).
	/** Return NULL if no FXVTK2DObject was picked. 	*/
	FXVTKObject* PickObject(double selectionX, double selectionY);


	int FXVTKWidgetBaseClass::PickCellFrom3DObject(double selectionX, double selectionY, FXVTK3DObject* object);



	// *********************** XYZ-Axes ***********************
	/// Let you show XYZ-Axes (2D Object, in forderground)
	void SwitchXYZAxesOn();
	/// Disable the XYZ-Axes
	void SwitchXYZAxesOff();
	/// Set the relative Position of the centre of the Axes-object (suggestive values: 0 ... 1)
	void SetXYZAxesRelativePosition(double relpos_x, double relpos_y);
	/// Set the relative size of the virtuell circle-radius, in there the axes displayed (suggestive values: 0 ... 0.5)
	void SetXYZAxesRelativeSizeRadius(double relradiussize);
	/// Return true if the XYZ is switched on
	bool XYZAxesIsOn() const {
		return XYZAxesIsOn_;
	}

	// PLEASE DON'T USE
	// Erzeugt VTK Fenster Interaktion, welche zur Kamerasteuerung beträgt
	// void CreateInteractorStyleTrackballCamera(); 	


	// be careful with use this 
	inline vtkRenderer* __GetVtkRenderer() {
		return Renderer;
	}



	// ********************** ImageWriter **********************
	/// Save Image as BMP
	void SaveImageAsBMP(const std::string filename);
	/// Save Image as JPEG
	void SaveImageAsJPEG(const std::string filename);
	/// Save Image as PNG
	void SaveImageAsPNG(const std::string filename);
	/// Save Image as TIFF
	void SaveImageAsTIFF(const std::string filename);

	/// Export as VRML 2.0 File (based on the VRML 2.0 draft #3 but it should be pretty stable since we aren't using any of the newer features)
	void ExportAsVRML(std::string filename);
	/// Export as Wavefront .OBJ File
	void ExportAsWavefrontOBJ(std::string filename);
	/// Export as Encapsulated PostScript (EPS) File
	void ExportAsPostscript_EPS(std::string filename);
	/// Export as PostScript (PS) File
	void ExportAsPostscript_PS(std::string filename);
	/// Export as PostScript-format in a PDF-File
	void ExportAsPostscript_PDF(std::string filename);
	/// Export as PostScript-format in a TEX-File
	void ExportAsPostscript_TEX(std::string filename);


	// ********************** MovieWriter **********************
	/// Prepare to capture frames to captured it to an avi-video
	void AVIWriterStart(std::string filename);
	/// Render Widget and capture the frame to the avi-video
	void AVIWriterCaptureFrame();
	/// Set the end of the capturing from the avi-video
	void AVIWriterEnd();


/*  // Zur Zeit wegen Rechtlichen Gründen nicht in Benutzung, unteranderem gibt es auch Probleme, eine DLL von vtkMPEG2Encode zu erstellen

	/// Prepare to capture frames to captured it to an mpeg2-video
	void MPEG2WriterStart(std::string filename);
	/// Render Widget and capture the frame to the mpeg2-video
	void MPEG2WriterCaptureFrame();
	/// Set the end of the capturing from the mpeg2-video
	void MPEG2WriterEnd();

*/






	// T E S T M E T H O D E N          ( Definitionen befinden sich in der Datei MyFXVWidgetTestMethods )
	// if FXVTKCompileTestMethods == 1

	//void CreateTestPolyData();
	//void CreateVTKTestObject();			// Erzeugt ein VTK TestObjekt
	//void CreateVTKTestCone();			// Erzeugt ein weiteres VTK TestObjekt (Cone)
	//void CreateTestPlane();
	//void CreateHeightfieldTestPlot();
	//void CreateDirectivityPlot();
	//void CreateMyCallback();			// Erzeugt eine Command Konsole ???

	// endif


private:

	void _addObject( FXVTKObject* obj, vtkProp3DCollection* actors3d, vtkActor2DCollection* actors2d, vtkProp* oneVtkProp = NULL );
	bool _delObject( FXVTKObject* obj, vtkProp3DCollection* actors3d, vtkActor2DCollection* actors2d, vtkProp* oneVtkProp = NULL );
	FXVTKObject* _findObject(vtkProp* prop);

	void _addLight(vtkLight* light);
	void _delLight(vtkLight* light);

	

	// **************** Image & Movie - Writer ****************
	vtkWindowToImageFilter* WindowToImageFilter;
	vtkBMPWriter* BMPWriter;
	vtkJPEGWriter* JPEGWriter;
	vtkPNGWriter* PNGWriter;
	vtkTIFFWriter* TIFFWriter;
	vtkAVIWriter* AVIWriter;
// 	vtkMPEG2Writer* MPEG2Writer;

	bool ExportAsPostScript_DrawBackground;



	// *********************** XYZ-Axes ***********************
	bool XYZAxesIsOn_;
	double XYZAxesRelativePosition[2];
	double XYZAxesRelativeSizeRadius;
	vtkActor2D* XYZAxes2DActor;
	vtkPoints* XYZAxesPoints;
	vtkTextActor* XYZAxesXLabel;
	vtkTextActor* XYZAxesYLabel;
	vtkTextActor* XYZAxesZLabel;
	double XYZAxesOldRots[3];
	int XYZAxesOldWinSize[2];

//	vtkRenderWindowInteractor *Interactor;



protected:
	std::list<FXVTKObject*>* ObjectList;
	bool UpToDate;

	vtkRenderer* Renderer;
	vtkRenderWindow* RenderWindow;
	vtkCamera* Camera;
	vtkLight* CameraLight;
	vtkCoordinate* RendererCoordinateTransformer;
	vtkCellPicker* CellPicker;

	bool CameraLightIsOn_;


};
