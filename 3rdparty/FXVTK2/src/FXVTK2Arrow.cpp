#include <FXVTK2Arrow.h>
#include <FXVTK2GlobalLock.h>

#include <vtkActor.h>
#include <vtkArrowSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>

namespace FXVTK2 {

Arrow::Arrow()
: SGNode(), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
}

Arrow::Arrow(double dTipLength, double dTipRadius, int iTipResolution, double dShaftRadius, int iShaftResolution)
: SGNode(), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
	m_pSource->SetTipLength(dTipLength);
	m_pSource->SetTipRadius(dTipRadius);
	m_pSource->SetTipResolution(iTipResolution);
	m_pSource->SetShaftRadius(dShaftRadius);
	m_pSource->SetShaftResolution(iShaftResolution);
}

Arrow::Arrow(FXVTK2::SGNode* pParentNode, double dTipLength, double dTipRadius, int iTipResolution, double dShaftRadius, int iShaftResolution)
: SGNode(pParentNode), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
	m_pSource->SetTipLength(dTipLength);
	m_pSource->SetTipRadius(dTipRadius);
	m_pSource->SetTipResolution(iTipResolution);
	m_pSource->SetShaftRadius(dShaftRadius);
	m_pSource->SetShaftResolution(iShaftResolution);
}

Arrow::~Arrow() {
	RemoveActor(m_pActor);

	m_pSource->Delete();
	m_pMapper->Delete();

	m_pActor->GlobalWarningDisplayOff();
	m_pActor->Delete();
}

void Arrow::init() {
	m_pSource = vtkArrowSource::New();

	vtkPolyDataNormals* normals = vtkPolyDataNormals::New();
	normals->SetInputConnection(m_pSource->GetOutputPort());
	
	m_pMapper = vtkPolyDataMapper::New();
	m_pMapper->SetInput(normals->GetOutput());

	m_pActor = vtkActor::New();
	m_pActor->SetMapper(m_pMapper);

	m_pActor->GetProperty()->SetInterpolationToGouraud();
	m_pActor->GetProperty()->SetDiffuse(0.9);
	m_pActor->GetProperty()->SetAmbient(0.4);

	AddActor(m_pActor);
}


// --= object related methods =--

double Arrow::GetTipLength() const {
	return m_pSource->GetTipLength();
}

void Arrow::SetTipLength(double dLength) {
	m_pSource->SetTipLength(dLength);
}

double Arrow::GetTipRadius() const {
	return m_pSource->GetTipLength();
}

void Arrow::SetTipRadius(double dRadius) {
	m_pSource->SetTipLength(dRadius);
}

void Arrow::SetTipResolution(int iResolution) {
	m_pSource->SetTipResolution(iResolution);
}

int Arrow::GetTipResolution() const {
	return m_pSource->GetTipResolution();
}

double Arrow::GetShaftRadius() const {
	return m_pSource->GetShaftRadius();
}

void Arrow::SetShaftRadius(double dRadius) {
	m_pSource->SetShaftRadius(dRadius);
}

int Arrow::GetShaftResolution() const {
	return m_pSource->GetShaftResolution();
}

void Arrow::SetShaftResolution(int iResolution) {
	m_pSource->SetShaftResolution(iResolution);
}


// --= general methods =--

void Arrow::SetColor(const double r, const double g, const double b) {
	FXVTK2_LOCK_VTK;
	m_pActor->GetProperty()->SetColor(r, g, b);
	FXVTK2_UNLOCK_VTK;
}

void Arrow::GetColor(double& r, double& g, double& b) {
	m_pActor->GetProperty()->GetColor(r, g, b);
}

double Arrow::GetAlpha() const {
	return m_pActor->GetProperty()->GetOpacity();
}

void Arrow::SetAlpha(const double a) {
	FXVTK2_LOCK_VTK;
	m_pActor->GetProperty()->SetOpacity(a);
	FXVTK2_UNLOCK_VTK;
}

void Arrow::SetVisible(bool bVisible) {
	// keep visibility traversal
	SGNode::SetVisible(bVisible);
	
	FXVTK2_LOCK_VTK;
	if (bVisible)
		m_pActor->VisibilityOn();
	else
		m_pActor->VisibilityOff();
	FXVTK2_UNLOCK_VTK;
}

bool Arrow::IsVisible() const {
	return m_pActor->GetVisibility() > 0 ? true : false;
}

} // End of namespace "FXVTK2"