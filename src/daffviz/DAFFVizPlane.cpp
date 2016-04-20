#include <daffviz/DAFFVizPlane.h>
#include <daffviz/DAFFVizGlobalLock.h>

#include <vtkActor.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

namespace DAFFViz {

Plane::Plane()
: SGNode(), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
}

Plane::Plane(double x1, double y1, double z1, double x2, double y2, double z2, double n1, double n2, double n3)
: SGNode(), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
	m_pSource->SetPoint1(x1, y1, z1);
	m_pSource->SetPoint2(x2, y2, z2);
	m_pSource->SetNormal(n1, n2, n3);
}

Plane::Plane(DAFFViz::SGNode* pParentNode, double x1, double y1, double z1, double x2, double y2, double z2, double n1, double n2, double n3)
: SGNode(pParentNode), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
	m_pSource->SetPoint1(x1, y1, z1);
	m_pSource->SetPoint2(x2, y2, z2);
	m_pSource->SetNormal(n1, n2, n3);
}

Plane::~Plane() {
	RemoveActor(m_pActor);

	m_pSource->Delete();
	m_pMapper->Delete();

	m_pActor->GlobalWarningDisplayOff();
	m_pActor->Delete();
}

void Plane::init() {
	m_pSource = vtkPlaneSource::New();
	
	m_pMapper = vtkPolyDataMapper::New();
	m_pMapper->SetInputData(m_pSource->GetOutput());

	m_pActor = vtkActor::New();
	m_pActor->SetMapper(m_pMapper);

	AddActor(m_pActor);
}

// --= object related methods =--


// --= general methods =--

void Plane::SetColor(const double r, const double g, const double b) {
	DAFFVIZ_LOCK_VTK;
	m_pActor->GetProperty()->SetColor(r, g, b);
	DAFFVIZ_UNLOCK_VTK;
}

void Plane::GetColor(double& r, double& g, double& b) {
	m_pActor->GetProperty()->GetColor(r, g, b);
}

double Plane::GetAlpha() const {
	return m_pActor->GetProperty()->GetOpacity();
}

void Plane::SetAlpha(const double a) {
	DAFFVIZ_LOCK_VTK;
	m_pActor->GetProperty()->SetOpacity(a);
	DAFFVIZ_UNLOCK_VTK;
}

bool Plane::IsVisible() const {
	return m_pActor->GetVisibility() > 0 ? true : false;
}

void Plane::SetVisible(bool bVisible) {
	SGNode::SetVisible(bVisible);
	
	DAFFVIZ_LOCK_VTK;
	if (bVisible)
		m_pActor->VisibilityOn();
	else
		m_pActor->VisibilityOff();
	DAFFVIZ_UNLOCK_VTK;
}

} // End of namespace "FXVTK2"