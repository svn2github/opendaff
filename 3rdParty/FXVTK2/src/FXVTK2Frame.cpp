// $Id: FXVTK2Frame.cpp 2369 2012-04-05 10:25:03Z stienen $

#include "FXVTK2Frame.h"

#include <FXVTK2GlobalLock.h>
#include <FXVTK2Label.h>
#include <math.h>

#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkPNGWriter.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkWindowToImageFilter.h>

namespace FXVTK2 {

// Map
FXDEFMAP(FXVTK2::Frame) FrameMap[]={
  FXMAPFUNC(SEL_PAINT,					0,						Frame::onPaint),
  FXMAPFUNC(SEL_TIMEOUT,				Frame::ID_TIMEOUT,		Frame::onTimeout),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,		0,						Frame::onLeftBtnPress),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,		0,						Frame::onLeftBtnRelease),
  FXMAPFUNC(SEL_MIDDLEBUTTONPRESS,		0,						Frame::onMiddleBtnPress),
  FXMAPFUNC(SEL_MIDDLEBUTTONRELEASE,	0,						Frame::onMiddleBtnRelease),
  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,		0,						Frame::onRightBtnPress),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,		0,						Frame::onRightBtnRelease),
  FXMAPFUNC(SEL_MOUSEWHEEL,				0,						Frame::onMouseWheelScroll),
  FXMAPFUNC(SEL_MOTION,                 0,                      Frame::onMouseMotion),
  FXMAPFUNC(SEL_LEAVE,					0,                      Frame::onLeave),
  FXMAPFUNC(SEL_ENTER,					0,		                Frame::onEnter),
  FXMAPFUNC(SEL_FOCUSIN,				0,		                Frame::onEnter)
}; 

FXIMPLEMENT(FXVTK2::Frame, FXFrame, FrameMap, ARRAYNUMBER(FrameMap))    

Frame::Frame(FXComposite* p, FXuint opts, FXint x, FXint y, FXint w, FXint h, FXint pl, FXint pr, FXint pt, FXint pb)
 : FXFrame(p,opts,x,y,w,h,pl,pr,pt,pb), m_pSceneRootNode(NULL)
{
	m_pRenderer = vtkRenderer::New();
	m_pRenderWindow = vtkRenderWindow::New();
	m_pRenderWindow->AddRenderer(m_pRenderer);

	m_pRenderWindow->LineSmoothingOn();

	m_pCameraLight = vtkLight::New();
	m_pCameraLight->SetLightTypeToCameraLight();

	m_pRenderer->AddLight(m_pCameraLight);

	m_bCameraLight = true;

	m_iRenderCount = 0;
	m_bRenderInProgress = false;
	m_bDestroyInProgress = false;

	m_pCamera = m_pRenderer->GetActiveCamera();
	
	m_iMouseInteractionMode = Frame::MI_DISABLED;
	m_pFrequencySelector = 0;

	m_bInteractionLeftBtn = false;
	m_bInteractionRightBtn = false;
	m_bInteractionMiddleBtn = false;

	m_dLastRootX = 0;
	m_dLastRootY = 0;
}

Frame::~Frame() {
	getApp()->removeTimeout(this, ID_TIMEOUT);
	detach();

	// Make sure that the rendering is really finished
	m_mxRenderCount.lock();
	m_bDestroyInProgress = true;
	m_mxRenderCount.unlock();

	// Poll until render end is indicated
	bool bFinished;
	do {
		m_mxRenderCount.lock();
		bFinished = !m_bRenderInProgress;
		m_mxRenderCount.unlock();
	} while (!bFinished);

	if (m_pCameraLight != NULL) m_pCameraLight->Delete();
	if (m_pRenderer != NULL) m_pRenderer->Delete();

	/* causes vtk error
	if (m_pRenderWindow != NULL) m_pRenderWindow->Delete();
	*/
}

void Frame::create() { 
	FXFrame::create();

	m_pRenderWindow->SetDisplayId((void*) getApp()->getDisplay());
	m_pRenderWindow->SetParentId((void*) getParent()->id());
	m_pRenderWindow->SetWindowId((void*) id());

	m_pRenderWindow->Start();
	m_pRenderWindow->SetSize(getWidth(), getHeight());
	

	// Initial timeout
	getApp()->addTimeout(this, ID_TIMEOUT, TIME_INTERVAL);
}

void Frame::SetSceneGraphRootNode(FXVTK2::SGNode* node) {
	FXVTK2_LOCK_VTK;
	m_pRenderer->RemoveAllViewProps();
	if (node != NULL) {
		m_pRenderer->AddActor((vtkProp*) node->GetNodeAssembly());
		
		// Set this frame's camera as the follower object
		node->OnSetFollowerCamera(m_pCamera);
	}

	m_pRenderer->ResetCameraClippingRange();
	FXVTK2_UNLOCK_VTK;

	m_pSceneRootNode = node;
}

void Frame::SetMouseInteractionMode(const int iMode) {
	m_iMouseInteractionMode = iMode;
}

int Frame::GetMouseInteractionMode() const {
	return m_iMouseInteractionMode;
}

void Frame::setFrequencySelector(FXSlider* slider) {
	m_pFrequencySelector = slider;
}

void Frame::detach() { 
	FXFrame::detach();
}



// --= Renderer =-- //

long Frame::onTimeout(FXObject*, FXSelector, void*) {
	// Add the next succeeding timeout
	getApp()->addTimeout(this, ID_TIMEOUT, TIME_INTERVAL);
	RenderInternal();

	return 1;
}

long Frame::onPaint(FXObject*, FXSelector, void*) {
	// Request rerendering
	Render();

	return 1;
}

void Frame::Render() {
	// Increment the update counter
	m_mxRenderCount.lock();
	m_iRenderCount++;
	m_mxRenderCount.unlock();
}

// --= Screen =-- //

void Frame::SetBackground(const double red, const double green, const double blue) {
	FXVTK2_LOCK_VTK;
	m_pRenderer->SetBackground(red, green, blue);
	FXVTK2_UNLOCK_VTK;
}

void Frame::GetBackground(double &red, double &green, double &blue) const {
	m_pRenderer->GetBackground(red, green, blue);
}

void Frame::SaveScreenshot(const std::string& sFilename) {
	if (!m_pRenderWindow) return;

	FXVTK2_LOCK_VTK;
	RenderInternal();
	FXVTK2_UNLOCK_VTK;

	// Window to image filter
	vtkWindowToImageFilter* screenshot = vtkWindowToImageFilter::New();
	screenshot->SetInput(m_pRenderWindow);

	// PNG exporter
	vtkPNGWriter* pngwriter = vtkPNGWriter::New();
	pngwriter->SetFileName(sFilename.c_str());
	pngwriter->SetInput(screenshot->GetOutput());
	pngwriter->Write();
}

// --= Camera =--

void Frame::SetCameraPosition(const double x, const double y, const double z ) {
	FXVTK2_LOCK_VTK;

	m_pCamera->SetPosition(x, y, z);
	m_pRenderer->ResetCameraClippingRange();

	FXVTK2_UNLOCK_VTK;
}

void Frame::GetCameraPosition(double& x, double& y, double& z) const {
	m_pCamera->GetPosition(x, y, z);
}

void Frame::SetCameraFocalPoint(const double x, const double y, const double z ) {
	FXVTK2_LOCK_VTK;

	m_pCamera->SetFocalPoint(x, y, z);
	m_pRenderer->ResetCameraClippingRange();

	FXVTK2_UNLOCK_VTK;
}

void Frame::GetCameraFocalPoint(double& x, double& y, double& z) const {
	m_pCamera->GetFocalPoint(x, y, z);
}

void Frame::DollyCamera(const double dDolly) {

	if (m_pCamera->GetParallelProjection())
		m_pCamera->Zoom(dDolly);
	else
		m_pCamera->Dolly(dDolly);

	FXVTK2_LOCK_VTK;

	m_pRenderer->ResetCameraClippingRange();

	FXVTK2_UNLOCK_VTK;

	Render();
}

void Frame::SetParallelProjection() {
	m_pCamera->SetParallelProjection(1);
}

void Frame::SetPerspectiveProjection() {
	m_pCamera->SetParallelProjection(0);
}

void Frame::SetCameraType(const int iType) {
	FXVTK2_LOCK_VTK;

	// Reset if no type
	if (iType <= 0) {
		m_pCamera->SetPosition(10, 10, 10);
		m_pCamera->SetParallelProjection(0);

		m_iCameraType = CAMERA_NONE;

		FXVTK2_UNLOCK_VTK;
		return;
	}
	
	m_iCameraType = iType;
	m_pCamera->SetParallelProjection(1);

	if (iType == CAMERA_TOP) {
		m_pCamera->SetPosition(0, 20, 0);
		m_pCamera->SetFocalPoint(0.001, 0, 0);
	}
	if (iType == CAMERA_BOTTOM) {
		m_pCamera->SetPosition(0, -20, 0);
		m_pCamera->SetFocalPoint(0, 0, 0);
	}
	if (iType == CAMERA_LEFT) {
		m_pCamera->SetPosition(-20, 0, 0);
		m_pCamera->SetFocalPoint(0, 0, 0);
	}
	if (iType == CAMERA_RIGHT) {
		m_pCamera->SetPosition(20, 0, 0);
		m_pCamera->SetFocalPoint(0, 0, 0);
	}
	if (iType == CAMERA_FRONT) {
		m_pCamera->SetPosition(0, 0, 20);
		m_pCamera->SetFocalPoint(0, 0, 0);
	}
	if (iType == CAMERA_BACK) {
		m_pCamera->SetPosition(0, 0, -20);
		m_pCamera->SetFocalPoint(0, 0, 0);
	}

	m_pCamera->Zoom(1);
	m_pCamera->Dolly(1);

	m_pRenderer->ResetCameraClippingRange();
	FXVTK2_UNLOCK_VTK;
}

void Frame::SetFollowerTarget() {
	if (m_pSceneRootNode != NULL)
		m_pSceneRootNode->OnSetFollowerCamera(m_pCamera);
}


// --= Lighting =--

void Frame::AddLight(vtkLight* light) {
	FXVTK2_LOCK_VTK;
	m_pRenderer->AddLight(light);
	FXVTK2_UNLOCK_VTK;
}

void Frame::RemoveLight(vtkLight* light) {
	FXVTK2_LOCK_VTK;
	m_pRenderer->RemoveLight(light);
	FXVTK2_UNLOCK_VTK;
}

void Frame::SetCameraLightOn() {
	if (m_bCameraLight == true) return;
	m_pCameraLight->SwitchOn();
}

void Frame::SetCameraLightOff() {
	if (m_bCameraLight == false) return;

	FXVTK2_LOCK_VTK;
	m_pCameraLight->SwitchOff();
	FXVTK2_UNLOCK_VTK;

	m_bCameraLight = false;
}

bool Frame::CameraLightIsOn() const { 
	return m_bCameraLight;
}

void Frame::SetCameraLightPosition(double x, double y, double z) {
	FXVTK2_LOCK_VTK;
	m_pCameraLight->SetPosition(x,y,z);
	FXVTK2_UNLOCK_VTK;
}

void Frame::GetCameraLightPosition(double &x, double &y, double &z) const {
	double* pointer = m_pCameraLight->GetPosition();
	x = pointer[0];
	y = pointer[1];
	z = pointer[2];
}

void Frame::RenderInternal() {
	// Important: First reset the update counter.
	//            Stored requested updates during the rendering phase.

	m_mxRenderCount.lock();
	
	if ((m_iRenderCount == 0) || m_bRenderInProgress || m_bDestroyInProgress) {
		m_mxRenderCount.unlock();

		return;
	}

	m_iRenderCount = 0;
	m_bRenderInProgress = true;
	m_mxRenderCount.unlock();	

	// Important: The flags m_bRenderInProgress indicated if a thread is 
	//            currently within the following section (2 lines).
	//            We explicitly avoid reentrance here!

	// Try lock agains scene graph nodes (if root node appended to frame)
	FXVTK2_LOCK_VTK;

	m_pRenderWindow->Render();

	// Unlock agains scene graph nodes (if root node appended to frame)
	FXVTK2_UNLOCK_VTK;

	m_pCamera = m_pRenderer->GetActiveCamera();

	m_mxRenderCount.lock();
	m_bRenderInProgress = false;
	m_mxRenderCount.unlock();
}


// --= Screen interaction =--

long Frame::onLeftBtnPress(FXObject*, FXSelector, void* ptr) {
	grab();
	m_bInteractionLeftBtn = true;

	return 1;
}

long Frame::onLeftBtnRelease(FXObject*, FXSelector, void*) {
	ungrab();
	m_bInteractionLeftBtn = false;

	m_dLastRootX = 0;
	m_dLastRootY = 0;

	return 1;
}

long Frame::onMiddleBtnPress(FXObject*, FXSelector, void*) {
	grab();
	m_bInteractionMiddleBtn = true;

	return 1;
}

long Frame::onMiddleBtnRelease(FXObject*, FXSelector, void*) {
	ungrab();
	m_bInteractionMiddleBtn = false;

	return 1;
}

long Frame::onRightBtnPress(FXObject*, FXSelector, void*) {
	grab();
	m_bInteractionRightBtn = true;

	return 1;
}

long Frame::onRightBtnRelease(FXObject*, FXSelector, void*) {
	ungrab();
	m_bInteractionRightBtn = false;

	return 1;
}

long Frame::onMouseWheelScroll(FXObject* obj, FXSelector sel, void* ptr) {
	FXEvent *pEvent = (FXEvent*) ptr;

	double dx = (double) pEvent->code / 1200;

	if ((m_iMouseInteractionMode == MI_AZIMUTH_ELEVATION) ||
		(m_iMouseInteractionMode == MI_MOVE_AND_ZOOM)) {

		double dDeltaDolly = 1.0f - 2 * dx;

		m_pCamera->Dolly(dDeltaDolly);

		if (m_pCamera->GetParallelProjection())
			m_pCamera->Zoom(dDeltaDolly);

		FXVTK2_LOCK_VTK;

		m_pRenderer->ResetCameraClippingRange();

		FXVTK2_UNLOCK_VTK;

		Render();
	} else if ((m_iMouseInteractionMode == MI_DIRECTIONAL_DATA) &&
		(m_pFrequencySelector != 0)) 
			m_pFrequencySelector->onMouseWheel(obj, sel, ptr);

	return 1;
}

long Frame::onMouseMotion(FXObject*, FXSelector, void* ptr) {
	if(!grabbed()) return 1;

	FXEvent* pEvent = (FXEvent*) ptr;
	double width = m_pRenderer->GetSize()[0];
	double height = m_pRenderer->GetSize()[1];

	if (m_iMouseInteractionMode == MI_AZIMUTH_ELEVATION) {

		if (m_bInteractionLeftBtn) {

			if (m_dLastRootX == 0) m_dLastRootX = pEvent->root_x;
			if (m_dLastRootY == 0) m_dLastRootY = pEvent->root_y;

			double dAzimuthScale = 0.8f * 360 / width;
			double dElevationScale = 180 / height;
			double dDeltaAzimuth = dAzimuthScale * (m_dLastRootX - pEvent->root_x);
			double dDeltaElevation =  - dElevationScale * (m_dLastRootY - pEvent->root_y);

			m_pCamera->Azimuth(dDeltaAzimuth);
			double* d = m_pCamera->GetOrientation();

			m_pCamera->Elevation(dDeltaElevation);
			if (fabs(d[0] + dDeltaElevation) > 90)
				m_pCamera->Azimuth(180);

			m_dLastRootX = pEvent->root_x;
			m_dLastRootY = pEvent->root_y;
			
			Render();
		}
		
		if (m_bInteractionMiddleBtn || m_bInteractionRightBtn) {
			double dDeltaDolly = 1.0f - 2*((pEvent->last_x - pEvent->win_x)/width);

			m_pCamera->Dolly(dDeltaDolly);

			if (m_pCamera->GetParallelProjection())
				m_pCamera->Zoom(dDeltaDolly);

			// Lock on scene graph
			FXVTK2_LOCK_VTK;

			m_pRenderer->ResetCameraClippingRange();

			// Unlock on scene graph
			FXVTK2_UNLOCK_VTK;

			Render();
		}

	}

	if (m_iMouseInteractionMode == MI_MOVE_AND_ZOOM) {

		double* pdCamPos = m_pCamera->GetPosition();
		double dDistance = m_pCamera->GetDistance();

		if (m_bInteractionLeftBtn) {
			double dx = 0.1 * dDistance * (pEvent->last_x - pEvent->win_x) / width;
			double dy = 0.1 * dDistance * (pEvent->last_y - pEvent->win_y) / height;

			if (m_iCameraType == CAMERA_FRONT) {
				m_pCamera->SetPosition(pdCamPos[0] + dx, pdCamPos[1] - dy, pdCamPos[2]);
				m_pCamera->SetFocalPoint(pdCamPos[0]+ dx, pdCamPos[1] - dy, 0);
			}
			if (m_iCameraType == CAMERA_BACK) {
				m_pCamera->SetPosition(pdCamPos[0] - dx, pdCamPos[1] - dy, pdCamPos[2]);
				m_pCamera->SetFocalPoint(pdCamPos[0] - dx, pdCamPos[1] - dy, 0);
			}

			if (m_iCameraType == CAMERA_LEFT) {
				m_pCamera->SetPosition(pdCamPos[0], pdCamPos[1] - dy, pdCamPos[2] + dx);
				m_pCamera->SetFocalPoint(0, pdCamPos[1] - dy, pdCamPos[2] + dx);
			}
			if (m_iCameraType == CAMERA_RIGHT) {
				m_pCamera->SetPosition(pdCamPos[0], pdCamPos[1] - dy, pdCamPos[2] - dx);
				m_pCamera->SetFocalPoint(0, pdCamPos[1] - dy, pdCamPos[2] - dx);
			}

			if (m_iCameraType == CAMERA_TOP) {
				m_pCamera->SetPosition(pdCamPos[0]-dy, pdCamPos[1], pdCamPos[2]+dx);
				m_pCamera->SetFocalPoint(pdCamPos[0]+0.0001, 0, pdCamPos[2]);
			}
			if (m_iCameraType == CAMERA_BOTTOM) {
				m_pCamera->SetPosition(pdCamPos[0]+dy, pdCamPos[1], pdCamPos[2]+dx);
				m_pCamera->SetFocalPoint(pdCamPos[0]+0.0001, 0, pdCamPos[2]);
			}

			// Lock on scene graph
			FXVTK2_LOCK_VTK;

			m_pRenderer->ResetCameraClippingRange();

			// Unlock on scene graph
			FXVTK2_UNLOCK_VTK;

			Render();
		}
		
		if (m_bInteractionMiddleBtn || m_bInteractionRightBtn) {
			double dDolly = 1.0f - 2*((pEvent->last_x - pEvent->win_x)/width);
			
			m_pCamera->Dolly(dDolly);

			if (m_pCamera->GetParallelProjection())
				m_pCamera->Zoom(dDolly);

			// Lock on scene graph
			FXVTK2_LOCK_VTK;

			m_pRenderer->ResetCameraClippingRange();

			// Unlock on scene graph
			FXVTK2_UNLOCK_VTK;

			Render();
		}

	}

	// --= Azimuth/elevation and zoom for directional datasets =--

	if (m_iMouseInteractionMode == MI_DIRECTIONAL_DATA) {

		if (m_bInteractionLeftBtn) {

			if (m_dLastRootX == 0) m_dLastRootX = pEvent->root_x;
			if (m_dLastRootY == 0) m_dLastRootY = pEvent->root_y;

			double dAzimuthScale = 0.8f * 360 / width;
			double dElevationScale = 180 / height;
			double dDeltaAzimuth = dAzimuthScale * (m_dLastRootX - pEvent->root_x);
			double dDeltaElevation =  - dElevationScale * (m_dLastRootY - pEvent->root_y);

			m_pCamera->Azimuth(dDeltaAzimuth);
			double* d = m_pCamera->GetOrientation();

			m_pCamera->Elevation(dDeltaElevation);
			if (fabs(d[0] + dDeltaElevation) > 90)
				m_pCamera->Azimuth(180);

			m_dLastRootX = pEvent->root_x;
			m_dLastRootY = pEvent->root_y;
			
			Render();
		}
		
		if (m_bInteractionRightBtn) {
			double dDeltaDolly = 1.0f - 2*((pEvent->last_x - pEvent->win_x)/width + (pEvent->last_y - pEvent->win_y)/height);

			m_pCamera->Dolly(dDeltaDolly);

			if (m_pCamera->GetParallelProjection())
				m_pCamera->Zoom(dDeltaDolly);

			// Lock on scene graph
			FXVTK2_LOCK_VTK;

			m_pRenderer->ResetCameraClippingRange();

			// Unlock on scene graph
			FXVTK2_UNLOCK_VTK;

			Render();
		}

		if (m_bInteractionMiddleBtn) {

			Render();
		}

	}


	return 1;
}

long Frame::onEnter(FXObject*, FXSelector, void*) {
	SetFollowerTarget();

	Render();

	return 1;
}

} // End of namespace "FXVTK2"
