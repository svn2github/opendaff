#include <daffviz/DAFFVizSphere.h>
#include <daffviz/DAFFVizGlobalLock.h>

#include <vtkActor.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

namespace DAFFViz {

Sphere::Sphere()
: SGNode(), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
}

Sphere::Sphere(double dRadius, int iPhiResolution, int iThetaResolution)
: SGNode(), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
	m_pSource->SetRadius(dRadius);
	m_pSource->SetPhiResolution(iPhiResolution);
	m_pSource->SetThetaResolution(iThetaResolution);
}

Sphere::Sphere(DAFFViz::SGNode* pParentNode, double dRadius, int iPhiResolution, int iThetaResolution)
: SGNode(pParentNode), m_pSource(NULL), m_pMapper(NULL), m_pActor(NULL)
{
	init();
	m_pSource->SetRadius(dRadius);
	m_pSource->SetPhiResolution(iPhiResolution);
	m_pSource->SetThetaResolution(iThetaResolution);
}

Sphere::~Sphere() {
	RemoveActor(m_pActor);

	m_pSource->Delete();
	m_pMapper->Delete();

	m_pActor->GlobalWarningDisplayOff();
	m_pActor->Delete();
}

void Sphere::init() {
	m_pSource = vtkSphereSource::New();
	
	m_pMapper = vtkPolyDataMapper::New();
	m_pMapper->SetInputData(m_pSource->GetOutput());

	m_pActor = vtkActor::New();
	m_pActor->SetMapper(m_pMapper);

	m_pActor->GetProperty()->SetDiffuse(0.9);
	m_pActor->GetProperty()->SetAmbient(0.4);

	AddActor(m_pActor);
}


// --= object related methods =--

double Sphere::GetRadius() const {
	return m_pSource->GetRadius();
}

void Sphere::SetRadius(double dRadius) {
	m_pSource->SetRadius(dRadius);
}

void Sphere::SetPhiResolution(int iResolution) {
	m_pSource->SetPhiResolution(iResolution);
}

void Sphere::SetThetaResolution(int iResolution) {
	m_pSource->SetThetaResolution(iResolution);
}

int Sphere::GetPhiResolution() const {
	return m_pSource->GetPhiResolution();
}

int Sphere::GetThetaResolution() const {
	return m_pSource->GetThetaResolution();
}


// --= general methods =--

void Sphere::SetColor(const double r, const double g, const double b) {
	DAFFVIZ_LOCK_VTK;
	m_pActor->GetProperty()->SetColor(r, g, b);
	DAFFVIZ_UNLOCK_VTK;
}

void Sphere::GetColor(double& r, double& g, double& b) {
	m_pActor->GetProperty()->GetColor(r, g, b);
}

double Sphere::GetAlpha() const {
	return m_pActor->GetProperty()->GetOpacity();
}

void Sphere::SetAlpha(const double a) {
	DAFFVIZ_LOCK_VTK;
	m_pActor->GetProperty()->SetOpacity(a);
	DAFFVIZ_UNLOCK_VTK;
}

bool Sphere::IsVisible() const {
	return m_pActor->GetVisibility() > 0 ? true : false;
}

void Sphere::SetVisible(bool bVisible) {
	SGNode::SetVisible(bVisible);
	
	DAFFVIZ_LOCK_VTK;
	if (bVisible)
		m_pActor->VisibilityOn();
	else
		m_pActor->VisibilityOff();
	DAFFVIZ_UNLOCK_VTK;
}

} // End of namespace "FXVTK2"
