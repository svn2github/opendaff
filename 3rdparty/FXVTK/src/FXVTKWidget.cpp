// $Id: FXVTKWidget.cpp,v 1.1 2010/02/10 09:57:25 schmoch Exp $

#include "FXVTKWidget.h"

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
#include <vtkActor2D.h>
#include <vtkProperty2D.h>

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


// Map
FXDEFMAP(FXVTKWidget) FXVTKWidgetMap[]={
  FXMAPFUNC(SEL_PAINT,					0,						FXVTKWidget::onPaint              ),
  //FXMAPFUNC(SEL_UPDATE,				0,						FXVTKWidget::onPaint              ),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,		0,						FXVTKWidget::onLeftBtnPress       ),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,		0,						FXVTKWidget::onLeftBtnRelease     ),
  FXMAPFUNC(SEL_MIDDLEBUTTONPRESS,		0,						FXVTKWidget::onMiddleBtnPress     ),
  FXMAPFUNC(SEL_MIDDLEBUTTONRELEASE,	0,						FXVTKWidget::onMiddleBtnRelease   ),
  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,		0,						FXVTKWidget::onRightBtnPress      ),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,		0,						FXVTKWidget::onRightBtnRelease    ),
  FXMAPFUNC(SEL_MOTION,                 0,                      FXVTKWidget::onMouseMotion        ),
  FXMAPFUNC(SEL_TIMEOUT,				FXVTKWidget::ID_TIMEOUT, FXVTKWidget::onTimeout            ),
  FXMAPFUNC(SEL_LEAVE,					0,                      FXVTKWidget::onLeave              ),
};

// Object implementation
FXIMPLEMENT(FXVTKWidget,FXFrame,FXVTKWidgetMap,ARRAYNUMBER(FXVTKWidgetMap))    

// Constructor
FXVTKWidget::FXVTKWidget(FXComposite* p, FXuint opts, FXint x, FXint y, FXint w, FXint h, FXint pl, FXint pr, FXint pt, FXint pb) :
	FXFrame(p,opts,x,y,w,h,pl,pr,pt,pb) {

	mouseMotionFunction = NULL;
	MouseCameraInteraction = true;
}

// Destructor
FXVTKWidget::~FXVTKWidget(void) {
	detach();
}


// Create window
void FXVTKWidget::create() { 
	FXFrame::create();

	this->RenderWindow->SetDisplayId((void*)getApp()->getDisplay());
	this->RenderWindow->SetParentId((void*)getParent()->id());
	this->RenderWindow->SetWindowId((void*)this->id());
	this->RenderWindow->Start();
}


// Detach window
void FXVTKWidget::detach() { 
  FXFrame::detach();
}

// Get minimum width
FXint FXVTKWidget::getDefaultWidth() {
  return 400+padleft+padright+border;  
}

// Get minimum height
FXint FXVTKWidget::getDefaultHeight() {
  return 300+padtop+padbottom+border;  
}



void FXVTKWidget::Dolly(int ,int y) {
   y = this->Renderer->GetSize()[1] - y;
   float c1 = this->Renderer->GetCenter()[1];
   double dyf = 0.5 * (double)(y - c1) / (double)(c1);
   double zoomFactor = pow((double)1.1, dyf);
   if (zoomFactor < 0.5 ) zoomFactor = 0.5;
   if (zoomFactor > 1.5 ) zoomFactor = 1.5;

   if ( Camera == NULL ) Camera = Renderer->GetActiveCamera();
   Camera->Dolly(zoomFactor);
   Renderer->ResetCameraClippingRange();
  
   // vtkCamera* CurrentCamera = this->Renderer->GetActiveCamera();
   // if (CurrentCamera->GetParallelProjection())  {
   //    CurrentCamera->SetParallelScale(CurrentCamera->GetParallelScale()/zoomFactor);
   // }
   // else {
   //    CurrentCamera->Dolly(zoomFactor);
   //    this->Renderer->ResetCameraClippingRange();
   // }
}

void FXVTKWidget::Pan(int x,int y) {
   //y = this->Interactor->GetSize()[1] - y;
   y = this->Renderer->GetSize()[1] - y;
   vtkCamera* CurrentCamera = this->Renderer->GetActiveCamera();
   double ViewFocus[4];
   CurrentCamera->GetFocalPoint(ViewFocus);
   this->Renderer->SetWorldPoint(ViewFocus[0],ViewFocus[1] , ViewFocus[2], 1.0);
   this->Renderer->WorldToDisplay();
   this->Renderer->GetDisplayPoint(ViewFocus);
   double focalDepth = ViewFocus[2];
   double NewPickPoint[4];

   this->Renderer->SetDisplayPoint((float)x, (float)y, focalDepth);
   this->Renderer->DisplayToWorld();
   this->Renderer->GetWorldPoint(NewPickPoint);
   if (NewPickPoint[3]) {
      NewPickPoint[0] /= NewPickPoint[3];
      NewPickPoint[1] /= NewPickPoint[3];
      NewPickPoint[2] /= NewPickPoint[3];
      NewPickPoint[3] = 1.0;
   }
   CurrentCamera->GetFocalPoint(ViewFocus);
   double *ViewPoint = CurrentCamera->GetPosition();

   double MotionVector[3];
   MotionVector[0] =  0.1*(ViewFocus[0] - NewPickPoint[0]);
   MotionVector[1] =  0.1*(ViewFocus[1] - NewPickPoint[1]);
   MotionVector[2] =  0.1*(ViewFocus[2] - NewPickPoint[2]);

   CurrentCamera->SetFocalPoint(
         MotionVector[0] + ViewFocus[0],
         MotionVector[1] + ViewFocus[1],
         MotionVector[2] + ViewFocus[2]);
   CurrentCamera->SetPosition(
         MotionVector[0] + ViewPoint[0],
         MotionVector[1] + ViewPoint[1],
         MotionVector[2] + ViewPoint[2]);
}
  
void FXVTKWidget::Rotate(int x,int y)
{
//   int *Size = this->Interactor->GetSize();
   int* Size = this->Renderer->GetSize();

   double* vp = this->Renderer->GetViewport();
       
   float DeltaElevation = -20.0/((vp[3] - vp[1])*Size[1]);
   float DeltaAzimuth   = -20.0/((vp[2] - vp[0])*Size[0]);

   float c0 = this->Renderer->GetCenter()[0];
   float c1 = this->Renderer->GetCenter()[1];
   double rxf = (double)(x - c0) * DeltaAzimuth;
   double ryf = (double)(Size[1] - y - c1) * DeltaElevation;

   vtkCamera* CurrentCamera = this->Renderer->GetActiveCamera();

   CurrentCamera->Azimuth(rxf);
   CurrentCamera->Elevation(ryf);
   CurrentCamera->OrthogonalizeViewUp();
   this->Renderer->ResetCameraClippingRange();
}

/*
void FXVTKWidget::Rotate(double ryf)
{
   vtkCamera* CurrentCamera = this->Renderer->GetActiveCamera();
   CurrentCamera->Elevation(ryf);
   CurrentCamera->OrthogonalizeViewUp();
   this->Renderer->ResetCameraClippingRange();
   this->CheckLightFollowCamera();
}
*/

void FXVTKWidget::Spin(int, int y)
{
   y = this->Renderer->GetSize()[1] - y;
   float c1 = this->Renderer->GetCenter()[1];
   double yf = (double)(y - c1)/ (double)(c1);
   if (yf > 1)  yf = 1;
   if (yf < -1) yf = -1;
   double newAngle = asin(yf) * 180.0 / 3.1415926;

   vtkCamera* CurrentCamera = this->Renderer->GetActiveCamera();
   CurrentCamera->Roll(newAngle);
   CurrentCamera->OrthogonalizeViewUp();
}


static struct _c {
	FX::FXint win_x;
	FX::FXint win_y;
	_c() : win_x(0), win_y(0) {}
} __c;


long FXVTKWidget::onTimeout(FXObject*,FXSelector,void *ptr)
{  
   //FXEvent *ev = (FXEvent*)ptr;
	_c *ev = &__c;
   switch(state) {
      case FXVTKIS_ROTATE: 
         this->Rotate(ev->win_x, ev->win_y); 
         getApp()->addTimeout(this, ID_TIMEOUT, TIME_INTERVAL);
         break;
      case FXVTKIS_PAN:
         this->Pan(ev->win_x, ev->win_y); 
         getApp()->addTimeout(this, ID_TIMEOUT, TIME_INTERVAL);
         break;
      case FXVTKIS_DOLLY:
         this->Dolly(ev->win_x, ev->win_y); 
         getApp()->addTimeout(this, ID_TIMEOUT, TIME_INTERVAL);
         break;
      case FXVTKIS_SPIN:
         this->Spin(ev->win_x,  ev->win_y); 
         getApp()->addTimeout(this, ID_TIMEOUT, TIME_INTERVAL);
         break;
      default:
         break;
   }
   this->Render();
   return 1;
}

void FXVTKWidget::StopAnimate(void) {
	if(timer){
		getApp()->removeTimeout(this, ID_TIMEOUT);
		timer=NULL;
	}
	state = FXVTKIS_NOTHING;  
	Render();
}


void FXVTKWidget::SetMouseMotionFunction( void (*pt2Func)(void* savedPtr, FXVTKWidget*), void* savedPtr ) {
	mouseMotionFunction = pt2Func;
	mouseMotionFunctionSavedPtr = savedPtr;
}

// Canvas is an object drawn by another
long FXVTKWidget::onPaint(FXObject*,FXSelector,void *ptr) {
	Render();
	return 1;
}


long FXVTKWidget::onLeftBtnRelease(FXObject*,FXSelector,void *ptr) {  
   this->StopAnimate();
   return 1;
}

long FXVTKWidget::onLeftBtnPress(FXObject*,FXSelector,void *ptr) {  
	
	// nothing to do if follow var = false
	if ( MouseCameraInteraction == false ) return 1;

   FXEvent *ev = (FXEvent*)ptr;
   if( ev->state&SHIFTMASK) 
      state = FXVTKIS_PAN;
   else
      state = FXVTKIS_ROTATE;  

   __c.win_x = ev->win_x;
   __c.win_y = ev->win_y;

   getApp()->addTimeout(this, ID_TIMEOUT, TIME_INTERVAL);

   return 1;
}



long FXVTKWidget::onMiddleBtnRelease(FXObject*,FXSelector,void *ptr) {  
   this->StopAnimate();
   return 1;
}

long FXVTKWidget::onMiddleBtnPress(FXObject*,FXSelector,void *ptr) {  

	// nothing to do if follow var = false
	if ( MouseCameraInteraction == false ) return 1;

   FXEvent *ev = (FXEvent*)ptr;

   __c.win_x = ev->win_x;
   __c.win_y = ev->win_y;

   state = FXVTKIS_PAN;  
   getApp()->addTimeout(this, ID_TIMEOUT, TIME_INTERVAL);
   return 1;
}

long FXVTKWidget::onRightBtnRelease(FXObject*,FXSelector,void *ptr) {  
   this->StopAnimate();
   return 1;
}

long FXVTKWidget::onRightBtnPress(FXObject*,FXSelector,void *ptr) {  

	// nothing to do if follow var = false
	if ( MouseCameraInteraction == false ) return 1;

   state = FXVTKIS_DOLLY;  
   FXEvent *ev = (FXEvent*)ptr;

   __c.win_x = ev->win_x;
   __c.win_y = ev->win_y;


   getApp()->addTimeout(this, ID_TIMEOUT, TIME_INTERVAL);
   return 1;
}

long FXVTKWidget::onMouseMotion(FXObject*,FXSelector,void *ptr) {  
	// is called if mouse motion on widget
	if (mouseMotionFunction != NULL)
		(*mouseMotionFunction)(mouseMotionFunctionSavedPtr, this);
	
	return 1;
}

long FXVTKWidget::onLeave(FXObject*,FXSelector,void *ptr) {  
   this->StopAnimate();
   return 1;
}


