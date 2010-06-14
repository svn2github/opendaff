// $Id: FXVTKWidgetBaseClass.cpp,v 1.1 2010/02/10 09:57:25 schmoch Exp $

#include "FXVTKWidgetBaseClass.h"
#include "FXVTKObject.h"
#include "FXVTK3DObject.h"
#include "FXVTK2DObject.h"
#include "FXVTKPolygons.h"

#include <math.h>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
// #include <vtkLightCollection.h>
#include <vtkLight.h>
#include <vtkCamera.h>

#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkPolyData.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty2D.h>
#include <vtkProp.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkProp3DCollection.h>
#include <vtkActor2DCollection.h>
#include <vtkAssemblyPath.h>
#include <vtkCoordinate.h>
#include <vtkViewport.h>
#include <vtkCellPicker.h>
#include <vtkProp3D.h>

#include <vtkWindowToImageFilter.h>
#include <vtkBMPWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>
#include <vtkPostScriptWriter.h>
#include <vtkTIFFWriter.h>
#include <vtkAVIWriter.h>
// #include <vtkMPEG2Writer.h>

#include <vtkVRMLExporter.h>
#include <vtkOBJExporter.h>
#include <vtkGL2PSExporter.h>


//#include <vtkRenderWindowInteractor.h>
//#include <vtkWindowToImageFilter.h>
//#include <vtkBMPWriter.h>


#define PI  3.14159265358979323846


// Constructor
FXVTKWidgetBaseClass::FXVTKWidgetBaseClass() {

	Renderer = vtkRenderer::New();
	RenderWindow = vtkRenderWindow::New();
	RenderWindow->AddRenderer(this->Renderer);

	CameraLight = vtkLight::New();
	CameraLight->SetLightTypeToCameraLight();
	Renderer->AddLight(CameraLight);

	ObjectList = new std::list<FXVTKObject*>;

	CameraLightIsOn_ = true;
	XYZAxesIsOn_ = false;
	XYZAxesRelativePosition[0] = 0.1;
	XYZAxesRelativePosition[1] = 0.1;
	XYZAxesOldRots[0] = 0.0;
	XYZAxesOldRots[1] = 0.0;
	XYZAxesOldRots[2] = 0.0;
	XYZAxesOldWinSize[0] = 0;
	XYZAxesOldWinSize[1] = 0;
	XYZAxesRelativeSizeRadius = 0.03;
	XYZAxes2DActor = NULL;


	Camera = NULL;
	RendererCoordinateTransformer = NULL;
	CellPicker = NULL;
	
	WindowToImageFilter = NULL;
	BMPWriter = NULL;
	JPEGWriter = NULL;
	PNGWriter = NULL;
	TIFFWriter = NULL;
	AVIWriter = NULL;
	//	MPEG2Writer = NULL;
	ExportAsPostScript_DrawBackground = false;
}

// Destructor
FXVTKWidgetBaseClass::~FXVTKWidgetBaseClass(void) {

	if (WindowToImageFilter != NULL) WindowToImageFilter->Delete();
	if (BMPWriter != NULL) BMPWriter->Delete();
	if (JPEGWriter != NULL) JPEGWriter->Delete();
	if (PNGWriter != NULL) PNGWriter->Delete();
	if (TIFFWriter != NULL) TIFFWriter->Delete();
	if (AVIWriter != NULL) AVIWriter->Delete();
	// if (MPEG2Writer != NULL) MPEG2WriterEnd();

	if (XYZAxes2DActor != NULL) {
		SwitchXYZAxesOff();
		XYZAxesZLabel->Delete();
		XYZAxesYLabel->Delete();
		XYZAxesXLabel->Delete();
		vtkPolyDataMapper2D* mapper = static_cast<vtkPolyDataMapper2D*>(XYZAxes2DActor->GetMapper());
		XYZAxes2DActor->Delete();
		vtkPolyData* polydata = mapper->GetInput();
		mapper->Delete();
		polydata->GetCellData()->GetScalars()->Delete();
		polydata->GetLines()->Delete();
		polydata->Delete();
		XYZAxesPoints->Delete();
	}

	DisconnectAllConnectedObjects();

	if (RendererCoordinateTransformer) RendererCoordinateTransformer->Delete();
	if (CameraLight) CameraLight->Delete();
	if (CellPicker) CellPicker->Delete();
	if (Renderer) Renderer->Delete();
	if (RenderWindow) RenderWindow->Delete();	

	delete ObjectList;
}



void FXVTKWidgetBaseClass::Render() {
	UpdateConnectedObjects();


	if (XYZAxesIsOn_ == true && Camera != NULL) {
		double* orient = Camera->GetOrientation();
		int* rendersize = Renderer->GetSize();
		
		// Reverse rotation of the rotation of the camera (z-components not needed)
		double x_rot = -orient[0]*PI/180;
		double y_rot = -orient[1]*PI/180;
		double z_rot = -orient[2]*PI/180;
		double r = XYZAxesRelativeSizeRadius * rendersize[0];

		if (XYZAxesOldRots[0] != x_rot || XYZAxesOldRots[1] != y_rot || XYZAxesOldRots[2] != z_rot || XYZAxesOldWinSize[0] != rendersize[0] || XYZAxesOldWinSize[1] != rendersize[1]) {

			double x,y;
			

			// x-Achse
			x = r * (sin(x_rot)*sin(y_rot)*sin(z_rot) + cos(y_rot)*cos(z_rot));
			y = r * (sin(x_rot)*sin(y_rot)*cos(z_rot) - cos(y_rot)*sin(z_rot));
			XYZAxesPoints->InsertPoint(1,x,y,0);
			XYZAxesXLabel->SetPosition(XYZAxesRelativePosition[0]*rendersize[0] + 1.3*x, XYZAxesRelativePosition[1]*rendersize[1] + 1.2*y);
			
			
			// y-Achse
			x = r * cos(x_rot)*sin(z_rot);
			y = r * cos(x_rot)*cos(z_rot);
			XYZAxesPoints->InsertPoint(2,x,y,0);
			XYZAxesYLabel->SetPosition(XYZAxesRelativePosition[0]*rendersize[0] + 1.3*x, XYZAxesRelativePosition[1]*rendersize[1] + 1.2*y);


			// z-Achse
			x = r * (sin(x_rot)*cos(y_rot)*sin(z_rot) - sin(y_rot)*cos(z_rot));
			y = r * (sin(x_rot)*cos(y_rot)*cos(z_rot) + sin(y_rot)*sin(z_rot));
			XYZAxesPoints->InsertPoint(3,x,y,0);
			XYZAxesZLabel->SetPosition(XYZAxesRelativePosition[0]*rendersize[0] + 1.3*x, XYZAxesRelativePosition[1]*rendersize[1] + 1.2*y);
			

			/*
			// rotation of camera
			// x-Achse
			// x = cos(z_rot)*cos(y_rot) - sin(z_rot)*sin(x_rot)*sin(y_rot);
			// y = -sin(z_rot)*cos(x_rot);
			// y-Achse
			// x = cos(z_rot)*sin(x_rot)*sin(y_rot) + sin(z_rot)*cos(y_rot);
			// y = cos(z_rot)*cos(x_rot);
			// z-Achse
			// x = cos(x_rot)*sin(y_rot);
			// y = -sin(x_rot);
			*/
		}
		if (XYZAxesOldWinSize[0] != rendersize[0] || XYZAxesOldWinSize[1] != rendersize[1] ) {
			XYZAxesXLabel->GetTextProperty()->SetFontSize(0.025*rendersize[1]);
			XYZAxesYLabel->GetTextProperty()->SetFontSize(0.025*rendersize[1]);
			XYZAxesZLabel->GetTextProperty()->SetFontSize(0.025*rendersize[1]);
			XYZAxes2DActor->SetPosition(XYZAxesRelativePosition[0]*rendersize[0], XYZAxesRelativePosition[1]*rendersize[1]);
		}


		XYZAxesOldRots[0] = x_rot;
		XYZAxesOldRots[1] = y_rot;
		XYZAxesOldRots[2] = z_rot;
		XYZAxesOldWinSize[0] = rendersize[0];
		XYZAxesOldWinSize[1] = rendersize[1];

	}

	//  TEST
	/*
	for (std::list<FXVTKObject*>::iterator iterator = ObjectList->begin(); iterator != ObjectList->end(); iterator++) {
		(*iterator)->mainActor->RenderOpaqueGeometry(Renderer);
	}

	for (std::list<FXVTKObject*>::iterator iterator = ObjectList->begin(); iterator != ObjectList->end(); iterator++) {
		(*iterator)->mainActor->RenderTranslucentGeometry(Renderer);
	}
	*/

	RenderWindow->Render();
	Camera = Renderer->GetActiveCamera();
}



void FXVTKWidgetBaseClass::_addLight(vtkLight* light) {
	Renderer->AddLight(light);
}
void FXVTKWidgetBaseClass::_delLight(vtkLight* light) {
	Renderer->RemoveLight(light);
}



void FXVTKWidgetBaseClass::UpdateConnectedObjects() {
	if (ObjectList->empty() || UpToDate == true) return;
	UpToDate = true; // must set before, because there are some objects, which wanted updated every render
	for (std::list<FXVTKObject*>::iterator iterator = ObjectList->begin(); iterator != ObjectList->end(); iterator++) {
		(*iterator)->_update();
	}
}

void FXVTKWidgetBaseClass::DisconnectAllConnectedObjects() {
	while (! ObjectList->empty() )
		(*ObjectList).front()->DisconnectWidget(this);
}


void FXVTKWidgetBaseClass::_addObject( FXVTKObject* obj, vtkProp3DCollection* actors3d, vtkActor2DCollection* actors2d, vtkProp* oneVtkProp ) {
	
	if (actors3d != NULL) {
		actors3d->InitTraversal();
		while ( vtkProp3D* actor = actors3d->GetNextProp3D() ) {
			Renderer->AddActor(actor);
		}
	}

	if (actors2d != NULL) {
		actors2d->InitTraversal();
		while ( vtkActor2D* actor = actors2d->GetNextActor2D() ) {
			Renderer->AddActor(actor);
		}
	}

	if (oneVtkProp != NULL)
		Renderer->AddActor(oneVtkProp);

	// if obj == NULL, actor has no object;
	if (obj != NULL) ObjectList->push_back(obj);

}
bool FXVTKWidgetBaseClass::_delObject( FXVTKObject* obj, vtkProp3DCollection* actors3d, vtkActor2DCollection* actors2d, vtkProp* oneVtkProp ) {
	
	if (actors3d != NULL) {
		actors3d->InitTraversal();
		while ( vtkProp3D* actor = actors3d->GetNextProp3D() ) {
			Renderer->RemoveActor(actor);
		}
	}

	if (actors2d != NULL) {
		actors2d->InitTraversal();
		while ( vtkActor2D* actor = actors2d->GetNextActor2D() ) {
			Renderer->RemoveActor(actor);
		}
	}

	if (oneVtkProp != NULL)
		Renderer->RemoveActor(oneVtkProp);

	// if obj == NULL, actor has no object;
	if (obj != NULL) {
		for (std::list<FXVTKObject*>::iterator iterator = ObjectList->begin(); iterator != ObjectList->end(); iterator++) {
			if ((*iterator) == obj) {
				ObjectList->erase(iterator);
				return true;
			}
		}
	}
	return false;
}


FXVTKObject* FXVTKWidgetBaseClass::_findObject(vtkProp* prop) {
	if (prop == NULL) return NULL;

	// if prop doesn't point to an vtkActor, dynamic_cast return NULL (VS 6.0)
	if (vtkActor* actor = dynamic_cast<vtkActor*>(prop) ) {
		for (std::list<FXVTKObject*>::iterator iterator = ObjectList->begin(); iterator != ObjectList->end(); iterator++) {
			vtkProp3DCollection* collection = (*iterator)->Actors3D;
			if (collection != NULL) {
				if (collection->IsItemPresent(actor))
					return *iterator;
			}
		}
	}

	// if prop doesn't point to an vtkActor, dynamic_cast return NULL (VS 6.0)
	else if (vtkActor2D* actor = dynamic_cast<vtkActor2D*>(prop) ) {
		for (std::list<FXVTKObject*>::iterator iterator = ObjectList->begin(); iterator != ObjectList->end(); iterator++) {
			vtkActor2DCollection* collection = (*iterator)->Actors2D;
			if (collection != NULL) {
				if (collection->IsItemPresent(actor))
					return *iterator;
			}
		}
	}

	return NULL;
}



void FXVTKWidgetBaseClass::SetBackgroundColor(const double red, const double green, const double blue) {
	Renderer->SetBackground(red, green, blue);
}
void FXVTKWidgetBaseClass::GetBackgroundColor(double &red, double &green, double &blue) {
	Renderer->GetBackground(red, green, blue);
}


void FXVTKWidgetBaseClass::GetWidgetSize(int &x, int &y) {
	int* a = Renderer->GetSize();
	x = a[0]; x = a[1];
}
int* FXVTKWidgetBaseClass::GetWidgetSize() {
	return Renderer->GetSize();
}


bool FXVTKWidgetBaseClass::GetDisplayValueOfWorldCoorinate( const double x, const double y, const double z, double &display_x, double &display_y ) {
	
	if ( Renderer == NULL ) return false;

	/*
	if ( RendererCoordinateTransformer == NULL )  {
		RendererCoordinateTransformer = vtkCoordinate::New();
		RendererCoordinateTransformer->SetCoordinateSystemToWorld();
	}
	
	// 1000000 times the next 2 lines take 22 seconds
	RendererCoordinateTransformer->SetValue( x,  y,  z );
	double* temp = RendererCoordinateTransformer->GetComputedDoubleDisplayValue(Renderer);
	*/



	// 1000000 times the next 3 lines take 22 seconds
	Renderer->SetWorldPoint( x, y, z, 1.0000000000 );
	Renderer->WorldToDisplay();
	double* temp2 = Renderer->GetDisplayPoint();

	display_x = temp2[0];
	display_y = temp2[1];
	// is outside viewing planes
	if (temp2[2] > 1.00 ) return false;

	return true;
}


//  cameraposition
void FXVTKWidgetBaseClass::SetCameraPosition( const double x, const double y, const double z ) {
	Camera = Renderer->GetActiveCamera();
	Camera->SetPosition(x,y,z);
}
double* FXVTKWidgetBaseClass::GetCameraPosition() {
	if (Camera == NULL) return NULL;
	
	return Camera->GetPosition();
}

void FXVTKWidgetBaseClass::SetCameraRoll( double roll ) {
	Camera = Renderer->GetActiveCamera();
	Camera->SetRoll(roll);	
}
double FXVTKWidgetBaseClass::GetCameraRoll() {
	if (Camera == NULL) return NULL;
	
	return Camera->GetRoll();
}

void FXVTKWidgetBaseClass::RollCamera( double angle ) {
	if ( Camera == NULL ) Camera = Renderer->GetActiveCamera();
	Camera->Roll(angle);
}

void FXVTKWidgetBaseClass::DollyCamera( double distance ) {
	if ( Camera == NULL ) Camera = Renderer->GetActiveCamera();
	Camera->Dolly( distance );
}


void FXVTKWidgetBaseClass::ResetCamera() {
	Renderer->ResetCamera();
}


//  Lights
void FXVTKWidgetBaseClass::SetCameraLightOn() {
	if (CameraLightIsOn_ == true) return;
	CameraLight->SwitchOn();
}
void FXVTKWidgetBaseClass::SetCameraLightOff() {
	if (CameraLightIsOn_ == false) return;
	CameraLight->SwitchOff();
	CameraLightIsOn_ = false;
}
void FXVTKWidgetBaseClass::SetCameraLightPosition(double x, double y, double z) {
	CameraLight->SetPosition(x,y,z);
}
void FXVTKWidgetBaseClass::GetCameraLightPosition(double &x, double &y, double &z) {
	double* pointer = CameraLight->GetPosition();
	x = pointer[0];  y = pointer[1];  z = pointer[2];
}


void FXVTKWidgetBaseClass::SwitchXYZAxesOn() {
	// if XYZAxes is on, nothing must to do here
	if (XYZAxesIsOn_ == true) return;

	if (XYZAxes2DActor == NULL) {
		XYZAxesPoints = vtkPoints::New();
		XYZAxesPoints->InsertNextPoint(0,0,0);
		XYZAxesPoints->InsertNextPoint(0,0,0);
		XYZAxesPoints->InsertNextPoint(0,0,0);
		XYZAxesPoints->InsertNextPoint(0,0,0);

		vtkCellArray* lines = vtkCellArray::New();
		lines->InsertNextCell(2);
		lines->InsertCellPoint(0);
		lines->InsertCellPoint(1);
		lines->InsertNextCell(2);
		lines->InsertCellPoint(0);
		lines->InsertCellPoint(2);
		lines->InsertNextCell(2);
		lines->InsertCellPoint(0);
		lines->InsertCellPoint(3);

		vtkUnsignedCharArray* CellColors = vtkUnsignedCharArray::New();
		CellColors->SetNumberOfComponents(3);
		CellColors->InsertNextTuple3(255,0,0);
		CellColors->InsertNextTuple3(0,255,0);
		CellColors->InsertNextTuple3(0,0,255);

		vtkPolyData* polydata = vtkPolyData::New();
		polydata->SetPoints(XYZAxesPoints);
		polydata->SetLines(lines);
		polydata->GetCellData()->SetScalars(CellColors);

		vtkPolyDataMapper2D* mapper = vtkPolyDataMapper2D::New();
		mapper->SetInput(polydata);
		mapper->SetScalarModeToUseCellData();

		XYZAxes2DActor = vtkActor2D::New();
		XYZAxes2DActor->SetMapper(mapper);
		XYZAxes2DActor->PickableOff();

		XYZAxesXLabel = vtkTextActor::New();
		XYZAxesXLabel->SetTextScaleModeToNone();  // in older vtk Versions here ScaledTextOff()
		XYZAxesXLabel->GetTextProperty()->SetFontSize(12);
		XYZAxesXLabel->GetTextProperty()->SetColor(1,0,0);
		XYZAxesXLabel->GetTextProperty()->SetJustificationToCentered();
		XYZAxesXLabel->GetTextProperty()->SetVerticalJustificationToCentered();
		XYZAxesXLabel->SetInput("X");

		XYZAxesYLabel = vtkTextActor::New();
		XYZAxesYLabel->SetTextScaleModeToNone();  // in older vtk Versions here ScaledTextOff()
		XYZAxesYLabel->GetTextProperty()->SetFontSize(12);
		XYZAxesYLabel->GetTextProperty()->SetColor(0,1,0);
		XYZAxesYLabel->GetTextProperty()->SetJustificationToCentered();
		XYZAxesYLabel->GetTextProperty()->SetVerticalJustificationToCentered();
		XYZAxesYLabel->SetInput("Y");

		XYZAxesZLabel = vtkTextActor::New();
		XYZAxesZLabel->SetTextScaleModeToNone();  // in older vtk Versions here ScaledTextOff()
		XYZAxesZLabel->GetTextProperty()->SetFontSize(12);
		XYZAxesZLabel->GetTextProperty()->SetColor(0,0,1);
		XYZAxesZLabel->GetTextProperty()->SetJustificationToCentered();
		XYZAxesZLabel->GetTextProperty()->SetVerticalJustificationToCentered();
		XYZAxesZLabel->SetInput("Z");
	}

	Renderer->AddActor2D(XYZAxes2DActor);
	Renderer->AddActor2D(XYZAxesXLabel);
	Renderer->AddActor2D(XYZAxesYLabel);
	Renderer->AddActor2D(XYZAxesZLabel);

	XYZAxesIsOn_ = true;
}

void FXVTKWidgetBaseClass::SwitchXYZAxesOff() {
	// if XYZAxes is off, nothing must to do here
	if (XYZAxesIsOn_ == false) return;

	Renderer->RemoveActor2D(XYZAxes2DActor);
	Renderer->RemoveActor2D(XYZAxesXLabel);
	Renderer->RemoveActor2D(XYZAxesYLabel);
	Renderer->RemoveActor2D(XYZAxesZLabel);

	XYZAxesIsOn_ = false;
}

void FXVTKWidgetBaseClass::SetXYZAxesRelativePosition(double relpos_x, double relpos_y) {
	XYZAxesRelativePosition[0] = relpos_x;
	XYZAxesRelativePosition[1] = relpos_y;
}
void FXVTKWidgetBaseClass::SetXYZAxesRelativeSizeRadius(double relradiussize) {
	XYZAxesRelativeSizeRadius = relradiussize;
}




// ********************** ImageWriter **********************	

void FXVTKWidgetBaseClass::SaveImageAsBMP(const std::string filename) {

	if (WindowToImageFilter == NULL) {
		WindowToImageFilter = vtkWindowToImageFilter::New();
		WindowToImageFilter->SetInput(RenderWindow);
	}
	if (BMPWriter == NULL) {
		BMPWriter = vtkBMPWriter::New();
		BMPWriter->SetInputConnection(WindowToImageFilter->GetOutputPort());
	}

	BMPWriter->SetFileName(filename.c_str());
	Render();
	WindowToImageFilter->Modified();
	BMPWriter->Write();
}

void FXVTKWidgetBaseClass::SaveImageAsJPEG(const std::string filename) {

	if (WindowToImageFilter == NULL) {
		WindowToImageFilter = vtkWindowToImageFilter::New();
		WindowToImageFilter->SetInput(RenderWindow);
	}
	if (JPEGWriter == NULL) {
		JPEGWriter = vtkJPEGWriter::New();
		JPEGWriter->SetInputConnection(WindowToImageFilter->GetOutputPort());
	}

	JPEGWriter->SetFileName(filename.c_str());
	Render();
	WindowToImageFilter->Modified();
	JPEGWriter->Write();
}

void FXVTKWidgetBaseClass::SaveImageAsPNG(const std::string filename) {

	if (WindowToImageFilter == NULL) {
		WindowToImageFilter = vtkWindowToImageFilter::New();
		WindowToImageFilter->SetInput(RenderWindow);
	}
	if (PNGWriter == NULL) {
		PNGWriter = vtkPNGWriter::New();
		PNGWriter->SetInputConnection(WindowToImageFilter->GetOutputPort());
	}

	PNGWriter->SetFileName(filename.c_str());
	Render();
	WindowToImageFilter->Modified();
	PNGWriter->Write();
}

void FXVTKWidgetBaseClass::SaveImageAsTIFF(const std::string filename) {

	if (WindowToImageFilter == NULL) {
		WindowToImageFilter = vtkWindowToImageFilter::New();
		WindowToImageFilter->SetInput(RenderWindow);
	}
	if (TIFFWriter == NULL) {
		TIFFWriter = vtkTIFFWriter::New();
		TIFFWriter->SetInputConnection(WindowToImageFilter->GetOutputPort());
	}

	TIFFWriter->SetFileName(filename.c_str());
	Render();
	WindowToImageFilter->Modified();
	TIFFWriter->Write();
}


// ********************** MovieWriter **********************	
//  AVIWriter
void FXVTKWidgetBaseClass::AVIWriterStart(std::string filename) {
	
	if (WindowToImageFilter == NULL) {
		WindowToImageFilter = vtkWindowToImageFilter::New();
		WindowToImageFilter->SetInput(RenderWindow);
	}
	if (AVIWriter != NULL) {
		AVIWriter->End();
		AVIWriter->Delete();	
	}
	AVIWriter = vtkAVIWriter::New();

	AVIWriter->SetInputConnection(WindowToImageFilter->GetOutputPort());
	AVIWriter->SetFileName(filename.c_str());
	AVIWriter->Start();
}
void FXVTKWidgetBaseClass::AVIWriterCaptureFrame() {
	Render();
	WindowToImageFilter->Modified();
	AVIWriter->Write();
}
void FXVTKWidgetBaseClass::AVIWriterEnd() {
	if (AVIWriter == NULL) return;

	AVIWriter->End();
	AVIWriter->Delete();
	AVIWriter = NULL;
}


/*
//  MPEG2Writer
void FXVTKWidgetBaseClass::MPEG2WriterStart(std::string filename) {
	if (MPEG2Writer != NULL) return;

	MPEG2Writer = vtkMPEG2Writer::New();

	if (WindowToImageFilter == NULL) {
		WindowToImageFilter = vtkWindowToImageFilter::New();
		WindowToImageFilter->SetInput(RenderWindow)
	}

	MPEG2Writer->SetInputConnection(WindowToImageFilter->GetOutputPort());
	MPEG2Writer->SetFileName("output.mpg");
	MPEG2Writer->Start();
}
void FXVTKWidgetBaseClass::MPEG2WriterCaptureFrame() {
	Render();
	WindowToImageFilter->Modified();
	MPEG2Writer->Write();
}
void FXVTKWidgetBaseClass::MPEG2WriterEnd() {
	if (MPEG2Writer == NULL) return;

	MPEG2Writer->End();

	MPEG2Writer->Delete();
	WindowToImageFilter->Delete();

	MPEG2Writer = NULL;
	WindowToImageFilter = NULL;
}
*/

void FXVTKWidgetBaseClass::ExportAsVRML(std::string filename) {
	
	vtkVRMLExporter* vrml = vtkVRMLExporter::New();
	vrml->SetInput(RenderWindow);
	vrml->SetFileName(filename.c_str());

	vrml->Write();
	vrml->Delete();
}
void FXVTKWidgetBaseClass::ExportAsWavefrontOBJ(std::string fileprefix) {
	
	vtkOBJExporter* obj = vtkOBJExporter::New();
	obj->SetInput(RenderWindow);
	obj->SetFilePrefix(fileprefix.c_str());

	obj->Write();
	obj->Delete();
}
void FXVTKWidgetBaseClass::ExportAsPostscript_EPS(std::string fileprefix) {
	
	vtkGL2PSExporter* exporter = vtkGL2PSExporter::New();
	exporter->SetInput(RenderWindow);
	exporter->SetFileFormatToEPS();
	exporter->CompressOff();
	if (ExportAsPostScript_DrawBackground == true) 
		exporter->DrawBackgroundOn();
	else exporter->DrawBackgroundOff();
	exporter->SetFilePrefix(fileprefix.c_str());
	
	exporter->Write();
	exporter->Delete();
}
void FXVTKWidgetBaseClass::ExportAsPostscript_PS(std::string fileprefix) {
	
	vtkGL2PSExporter* exporter = vtkGL2PSExporter::New();
	exporter->SetInput(RenderWindow);
	exporter->SetFileFormatToPS();
	exporter->CompressOff();
	if (ExportAsPostScript_DrawBackground == true) 
		exporter->DrawBackgroundOn();
	else exporter->DrawBackgroundOff();
	exporter->SetFilePrefix(fileprefix.c_str());
	
	exporter->Write();
	exporter->Delete();
}
void FXVTKWidgetBaseClass::ExportAsPostscript_PDF(std::string fileprefix) {
	
	vtkGL2PSExporter* exporter = vtkGL2PSExporter::New();
	exporter->SetInput(RenderWindow);
	exporter->SetFileFormatToPDF();
	if (ExportAsPostScript_DrawBackground == true) 
		exporter->DrawBackgroundOn();
	else exporter->DrawBackgroundOff();
	exporter->SetFilePrefix(fileprefix.c_str());
	
	exporter->Write();
	exporter->Delete();
}
void FXVTKWidgetBaseClass::ExportAsPostscript_TEX(std::string fileprefix) {
	
	vtkGL2PSExporter* exporter = vtkGL2PSExporter::New();
	exporter->SetInput(RenderWindow);
	exporter->SetFileFormatToTeX();
	if (ExportAsPostScript_DrawBackground == true) 
		exporter->DrawBackgroundOn();
	else exporter->DrawBackgroundOff();
	exporter->SetFilePrefix(fileprefix.c_str());
	
	exporter->Write();
	exporter->Delete();
}


FXVTK3DObject* FXVTKWidgetBaseClass::Pick3DObject(double selectionX, double selectionY) {
	vtkAssemblyPath* assemblypath = Renderer->PickProp(selectionX, selectionY);
	if (assemblypath == NULL) return NULL;

	// TODO: Wenn möglich den assemblypath noch sortieren lassen, nach treffer reienfolge.
	// denn bei mehreren Objekten hintereinander wird nicht unbedingt das Objekt im Vordergrund genommen
	vtkAssemblyNode* assemblynode = assemblypath->GetFirstNode();
	
	while(assemblynode) {
		vtkProp* prop = assemblynode->GetViewProp();	

		// if obj doesn't point to a FXVTK3DObject, dynamic_cast return NULL (VS 6.0) and loop resume
		if (FXVTK3DObject* obj = dynamic_cast<FXVTK3DObject*> ( _findObject(prop)) )
			return obj;

		assemblynode = assemblypath->GetNextNode();
	}
	 	
	return NULL;
}

FXVTK2DObject* FXVTKWidgetBaseClass::Pick2DObject(double selectionX, double selectionY) {
	vtkAssemblyPath* assemblypath = Renderer->PickProp(selectionX, selectionY);
	if (assemblypath == NULL) return NULL;

	vtkAssemblyNode* assemblynode = assemblypath->GetFirstNode();
	
	while(assemblynode) {
		vtkProp* prop = assemblynode->GetViewProp();	

		// if obj doesn't point to a FXVTK2DObject, dynamic_cast return NULL (VS 6.0) and loop resume
		if (FXVTK2DObject* obj = dynamic_cast<FXVTK2DObject*> ( _findObject(prop)) )
			return obj;

		assemblynode = assemblypath->GetNextNode();
	}
	 	
	return NULL;
}

FXVTKObject* FXVTKWidgetBaseClass::PickObject(double selectionX, double selectionY) {
	vtkAssemblyPath* assemblypath = Renderer->PickProp(selectionX, selectionY);
	if (assemblypath == NULL) return NULL;

	vtkAssemblyNode* assemblynode = assemblypath->GetFirstNode();
	
	while(assemblynode) {
		vtkProp* prop = assemblynode->GetViewProp();	

		// if obj doesn't point to a FXVTK2DObject, dynamic_cast return NULL (VS 6.0) and loop resume
		if ( FXVTKObject* obj = _findObject(prop) )
			return obj;

		assemblynode = assemblypath->GetNextNode();
	}
	 	
	return NULL;
}


int FXVTKWidgetBaseClass::PickCellFrom3DObject(double selectionX, double selectionY, FXVTK3DObject* object) {
	if ( CellPicker == NULL ) { 
		CellPicker = vtkCellPicker::New();
		CellPicker->SetTolerance( 0.0025 );  // default from vtk 0.025
	}
	object->Actors3D->InitTraversal();
	vtkProp3D* prop = object->Actors3D->GetNextProp3D();
	if ( prop == NULL ) return -1;

	CellPicker->InitializePickList();
	CellPicker->AddPickList( prop );
	CellPicker->PickFromListOn(); // war vorher off???

	int cellid;

	if ( CellPicker->Pick( selectionX, selectionY, 0, Renderer ) ) {
		// something is picked
		cellid = CellPicker->GetCellId();
	}
	else return -1;

	if ( strcmp ( object->GetClassname(), "FXVTKPolygons" ) == 0 ) {
		// TODO: 
		FXVTKPolygons* polygonsobj = dynamic_cast<FXVTKPolygons*> ( object );
		cellid = polygonsobj->_getRealCellID( cellid );

		return cellid;
	}
	else return cellid;
}


/*
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballCamera.h>

void FXVTKWidgetBaseClass::CreateInteractorStyleTrackballCamera() {
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	vtkRenderWindow* renWin = RenderWindow;
	iren->SetRenderWindow(renWin);

	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	iren->Initialize();
	iren->Start();
}
*/