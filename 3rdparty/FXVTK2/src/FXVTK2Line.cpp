#include <FXVTK2Line.h>
#include <FXVTK2GlobalLock.h>

#include <vtkActor.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

namespace FXVTK2 {

Line::Line()
: SGNode(), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
}

Line::Line(double x1, double y1, double z1, double x2, double y2, double z2)
: SGNode(), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
	m_pSource->SetPoint1(x1,y1,z1);
	m_pSource->SetPoint1(x1,y1,z1);
}

Line::Line(FXVTK2::SGNode* pParentNode, double x1, double y1, double z1, double x2, double y2, double z2)
: SGNode(pParentNode), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
	m_pSource->SetPoint1(x1,y1,z1);
	m_pSource->SetPoint2(x2,y2,z2);
}

Line::~Line() {
	RemoveActor(m_pActor);

	m_pSource->Delete();
	m_pMapper->Delete();

	m_pActor->GlobalWarningDisplayOff();
	m_pActor->Delete();
}

void Line::init() {
	m_pSource = vtkLineSource::New();
	
	m_pMapper = vtkPolyDataMapper::New();
	m_pMapper->SetInput(m_pSource->GetOutput());

	m_pActor = vtkActor::New();
	m_pActor->SetMapper(m_pMapper);

	AddActor(m_pActor);
}


// --= object related methods =--

void Line::GetPoint1(double& x, double& y, double& z) const {
	double* data = m_pSource->GetPoint1();
	x = data[0];
	y = data[1];
	z = data[2];
}

void Line::SetPoint1(double x, double y, double z) {
	m_pSource->SetPoint1(x,y,z);
}

void Line::GetPoint2(double& x, double& y, double& z) const {
	double* data = m_pSource->GetPoint2();
	x = data[0];
	y = data[1];
	z = data[2];
}

void Line::SetPoint2(double x, double y, double z) {
	m_pSource->SetPoint2(x,y,z);
}


// --= general methods =--

void Line::SetColor(const double r, const double g, const double b) {
	FXVTK2_LOCK_VTK;
	m_pActor->GetProperty()->SetColor(r, g, b);
	FXVTK2_UNLOCK_VTK;
}

void Line::GetColor(double& r, double& g, double& b) {
	m_pActor->GetProperty()->GetColor(r, g, b);
}

double Line::GetAlpha() const {
	return m_pActor->GetProperty()->GetOpacity();
}

void Line::SetAlpha(const double a) {
	FXVTK2_LOCK_VTK;
	m_pActor->GetProperty()->SetOpacity(a);
	FXVTK2_UNLOCK_VTK;
}

bool Line::IsVisible() const {
	return m_pActor->GetVisibility() > 0 ? true : false;
}

void Line::SetVisible(bool bVisible) {
	SGNode::SetVisible(bVisible);
	
	FXVTK2_LOCK_VTK;
	if (bVisible)
		m_pActor->VisibilityOn();
	else
		m_pActor->VisibilityOff();
	FXVTK2_UNLOCK_VTK;
}

} // End of namespace "FXVTK2"