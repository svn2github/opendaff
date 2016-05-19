#include "VTKDAFFVizWindow.h"

// DAFFViz includes
#include <daffviz/DAFFVizGlobalLock.h>
#include <daffviz/DAFFVizLabel.h>

// VTK includes
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkPNGWriter.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkWindowToImageFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkProp.h>
#include <vtkAssembly.h>

// STL includes
#include <math.h>


namespace DAFFViz
{

	VTKDAFFVizWindow::VTKDAFFVizWindow()
	{
		/*
		m_pRenderer = vtkSmartPointer< vtkRenderer > ::New();
		m_pRenderer->SetBackground( 0.1f, 0.1f, 1.0f );

		m_pRenderWindow = vtkSmartPointer< vtkRenderWindow >( vtkRenderWindow::New() );
		m_pRenderWindow->AddRenderer( m_pRenderer );

		m_pRenderWindow->LineSmoothingOn();

		m_pCameraLight = vtkSmartPointer<vtkLight>( vtkLight::New() );
		m_pCameraLight->SetLightTypeToCameraLight();

		m_pRenderer->AddLight( m_pCameraLight );

		m_bCameraLight = true;

		m_iRenderCount = 0;
		m_bRenderInProgress = false;
		m_bDestroyInProgress = false;

		m_pCamera = m_pRenderer->GetActiveCamera();

		m_pInteractor = vtkSmartPointer< vtkRenderWindowInteractor >( vtkRenderWindowInteractor::New() );
		m_pInteractor->SetRenderWindow( m_pRenderWindow );
	
		m_bInteractionLeftBtn = false;
		m_bInteractionRightBtn = false;
		m_bInteractionMiddleBtn = false;

		m_dLastRootX = 0;
		m_dLastRootY = 0;
		*/
	}

	VTKDAFFVizWindow::~VTKDAFFVizWindow()
	{	
		// Make sure that the rendering is really finished
		m_mxRenderCount.lock();
		m_bDestroyInProgress = true;
		m_mxRenderCount.unlock();

		// Poll until render end is indicated
		bool bFinished;
		do
		{
			m_mxRenderCount.lock();
			bFinished = !m_bRenderInProgress;
			m_mxRenderCount.unlock();
		} while (!bFinished);
	}

	void VTKDAFFVizWindow::Start()
	{
		m_pRenderWindow->Render();
		//m_pRenderWindow->Start();
		m_pInteractor->Start();	
	}

	void VTKDAFFVizWindow::SetSceneGraphRootNode( DAFFViz::SGNode* node )
	{
		DAFFVIZ_LOCK_VTK;
		m_pRenderer->RemoveAllViewProps();
		if( node != NULL )
		{
			vtkSmartPointer< vtkAssembly > pA = node->GetNodeAssembly();
			m_pRenderer->AddActor( pA );
		
			// Set this frame's camera as the follower object
			node->OnSetFollowerCamera( m_pCamera );
		}

		m_pRenderer->ResetCameraClippingRange();
		DAFFVIZ_UNLOCK_VTK;

		m_pSceneRootNode = node;
	}

	// --= Renderer =-- //

	void VTKDAFFVizWindow::Render()
	{
		// Increment the update counter
		m_mxRenderCount.lock();
		m_iRenderCount++;
		m_mxRenderCount.unlock();
	}

	// --= Screen =-- //

	void VTKDAFFVizWindow::SetBackground( double red, double green, double blue )
	{
		DAFFVIZ_LOCK_VTK;
		m_pRenderer->SetBackground( red, green, blue );
		DAFFVIZ_UNLOCK_VTK;
	}

	void VTKDAFFVizWindow::GetBackground( double &red, double &green, double &blue ) const
	{
		m_pRenderer->GetBackground( red, green, blue );
	}

	void VTKDAFFVizWindow::SaveScreenshot( const std::string& sFilename )
	{
		if (!m_pRenderWindow) return;

		DAFFVIZ_LOCK_VTK;
		RenderInternal();
		DAFFVIZ_UNLOCK_VTK;

		// Window to image filter
		vtkSmartPointer< vtkWindowToImageFilter > screenshot = vtkSmartPointer< vtkWindowToImageFilter >::New();
		screenshot->SetInput(m_pRenderWindow);

		// PNG exporter
		vtkSmartPointer< vtkPNGWriter > pngwriter = vtkSmartPointer< vtkPNGWriter >::New();
		pngwriter->SetFileName(sFilename.c_str());
		pngwriter->SetInputData(screenshot->GetOutput());
		pngwriter->Write();
	}

	// --= Camera =--

	void VTKDAFFVizWindow::SetCameraPosition( double x,  double y,  double z )
	{
		DAFFVIZ_LOCK_VTK;

		m_pCamera->SetPosition(x, y, z);
		m_pRenderer->ResetCameraClippingRange();

		DAFFVIZ_UNLOCK_VTK;
	}

	void VTKDAFFVizWindow::GetCameraPosition( double& x, double& y, double& z ) const
	{
		m_pCamera->GetPosition(x, y, z);
	}

	void VTKDAFFVizWindow::SetCameraFocalPoint( double x,  double y,  double z )
	{
		DAFFVIZ_LOCK_VTK;

		m_pCamera->SetFocalPoint(x, y, z);
		m_pRenderer->ResetCameraClippingRange();

		DAFFVIZ_UNLOCK_VTK;
	}

	void VTKDAFFVizWindow::GetCameraFocalPoint( double& x, double& y, double& z ) const
	{
		m_pCamera->GetFocalPoint(x, y, z);
	}

	void VTKDAFFVizWindow::DollyCamera( double dDolly )
	{

		if( m_pCamera->GetParallelProjection() )
			m_pCamera->Zoom(dDolly);
		else
			m_pCamera->Dolly(dDolly);

		DAFFVIZ_LOCK_VTK;

		m_pRenderer->ResetCameraClippingRange();

		DAFFVIZ_UNLOCK_VTK;

		Render();
	}

	void VTKDAFFVizWindow::SetParallelProjection()
	{
		m_pCamera->SetParallelProjection(1);
	}

	void VTKDAFFVizWindow::SetPerspectiveProjection()
	{
		m_pCamera->SetParallelProjection(0);
	}

	void VTKDAFFVizWindow::SetCameraType( int iType )
	{
		DAFFVIZ_LOCK_VTK;

		// Reset if no type
		if( iType <= 0 )
		{
			m_pCamera->SetPosition(10, 10, 10);
			m_pCamera->SetParallelProjection(0);

			m_iCameraType = CAMERA_NONE;

			DAFFVIZ_UNLOCK_VTK;
			return;
		}
	
		m_iCameraType = iType;
		m_pCamera->SetParallelProjection(1);

		if( iType == CAMERA_TOP )
		{
			m_pCamera->SetPosition(0, 20, 0);
			m_pCamera->SetFocalPoint(0.001, 0, 0);
		}
		if( iType == CAMERA_BOTTOM )
		{
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
		DAFFVIZ_UNLOCK_VTK;
	}

	void VTKDAFFVizWindow::SetFollowerTarget()
	{
		if( m_pSceneRootNode != NULL )
			m_pSceneRootNode->OnSetFollowerCamera( m_pCamera );
	}


	// --= Lighting =--

	void VTKDAFFVizWindow::AddLight(vtkLight* light)
	{
		DAFFVIZ_LOCK_VTK;
		m_pRenderer->AddLight(light);
		DAFFVIZ_UNLOCK_VTK;
	}

	void VTKDAFFVizWindow::RemoveLight(vtkLight* light)
	{
		DAFFVIZ_LOCK_VTK;
		m_pRenderer->RemoveLight(light);
		DAFFVIZ_UNLOCK_VTK;
	}

	void VTKDAFFVizWindow::SetCameraLightOn()
	{
		if( m_bCameraLight == true )
			return;
		m_pCameraLight->SwitchOn();
	}

	void VTKDAFFVizWindow::SetCameraLightOff()
	{
		if (m_bCameraLight == false) return;

		DAFFVIZ_LOCK_VTK;
		m_pCameraLight->SwitchOff();
		DAFFVIZ_UNLOCK_VTK;

		m_bCameraLight = false;
	}

	bool VTKDAFFVizWindow::CameraLightIsOn() const
	{ 
		return m_bCameraLight;
	}

	void VTKDAFFVizWindow::SetCameraLightPosition(double x, double y, double z)
	{
		DAFFVIZ_LOCK_VTK;
		m_pCameraLight->SetPosition(x,y,z);
		DAFFVIZ_UNLOCK_VTK;
	}

	void VTKDAFFVizWindow::GetCameraLightPosition(double &x, double &y, double &z) const
	{
		double* pointer = m_pCameraLight->GetPosition();
		x = pointer[0];
		y = pointer[1];
		z = pointer[2];
	}

	void VTKDAFFVizWindow::RenderInternal()
	{
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
		DAFFVIZ_LOCK_VTK;

		m_pRenderWindow->Render();

		// Unlock agains scene graph nodes (if root node appended to frame)
		DAFFVIZ_UNLOCK_VTK;

		m_pCamera = m_pRenderer->GetActiveCamera();

		m_mxRenderCount.lock();
		m_bRenderInProgress = false;
		m_mxRenderCount.unlock();
	}

} // End of namespace "DAFFViz"
