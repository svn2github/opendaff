#include "FXVTK2Label.h"

#include <FXVTK2GlobalLock.h>

#include <vtkCamera.h>
#include <vtkFollower.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkVectorText.h>

namespace FXVTK2 {

Label::Label()
: SGNode()
{
	m_sText = "No label text set";
	init();
}

Label::Label(SGNode* pNode)
: SGNode(pNode)
{
	m_sText = "No label text set";
	init();
}

Label::Label(SGNode* pNode, std::string sText)
: SGNode(pNode)
{
	m_sText = sText;
	init();
}


Label::~Label() {
	RemoveActor(m_pFollower);

	m_pLabelText->Delete();
	m_pMapper->Delete();

	m_pFollower->GlobalWarningDisplayOff();
	m_pFollower->Delete();
}

void Label::init() {
	m_pLabelText = vtkVectorText::New();
	m_pLabelText->SetText(m_sText.c_str());
	
	m_pMapper = vtkPolyDataMapper::New();
	m_pMapper->SetInputConnection(m_pLabelText->GetOutputPort());

	m_pFollower = vtkFollower::New();
	m_pFollower->SetMapper(m_pMapper);

	m_pFollower->SetScale(0.1, 0.1, 0.1);

	AddActor(m_pFollower);
}

// --= general methods =--

void Label::GetColor(double& r, double& g, double& b) const {
	m_pFollower->GetProperty()->GetColor(r, g, b);
}

void Label::SetColor(const double r, const double g, const double b) {
	FXVTK2_LOCK_VTK;
	m_pFollower->GetProperty()->SetColor(r, g, b);
	FXVTK2_UNLOCK_VTK;
}

double Label::GetAlpha() const {
	return m_pFollower->GetProperty()->GetOpacity();
}

void Label::SetAlpha(const double a) {
	FXVTK2_LOCK_VTK;
	m_pFollower->GetProperty()->SetOpacity(a);
	FXVTK2_UNLOCK_VTK;
}

void Label::SetVisible(bool bVisible) {
	// keep visibility traversal
	SGNode::SetVisible(bVisible);
	FXVTK2_LOCK_VTK;
	if (bVisible)
		m_pFollower->VisibilityOn();
	else
		m_pFollower->VisibilityOff();
	FXVTK2_UNLOCK_VTK;
}

bool Label::IsVisible() const {
	return m_pFollower->GetVisibility() > 0 ? true : false;
}


// --= object related methods =--

void Label::SetText(const std::string& s) {
	FXVTK2_LOCK_VTK;
	m_pLabelText->SetText(s.c_str());
	FXVTK2_UNLOCK_VTK;
}

void Label::OnSetFollowerCamera(vtkCamera* pCamera) {
	FXVTK2_LOCK_VTK;
	//m_pFollower->SetCamera(pCamera);
	FXVTK2_UNLOCK_VTK;

	// Delegate
	SGNode::OnSetFollowerCamera(pCamera);
}

} // End of namespace "FXVTK2"
