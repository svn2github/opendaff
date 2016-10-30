#include <daffviz/DAFFVizSphericalDirectionIndicator.h>
#include <daffviz/DAFFVizGlobalLock.h>
#include <daffviz/DAFFVizPlane.h>
#include <daffviz/DAFFVizArrow.h>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPolyDataNormals.h>

namespace DAFFViz
{

	SphericalDirectionIndicator::SphericalDirectionIndicator( DAFFViz::SGNode* pParentNode )
		: SGNode( pParentNode )
		, m_pDirectionArrow( NULL )
		, m_dPhiDeg( 0.0f )
		, m_dThetaDeg( 0.0f )
		, m_dAlphaDeg( 0.0f )
		, m_dBetaDeg( 0.0f )
	{
		Init();
	}

	void SphericalDirectionIndicator::Init()
	{
		m_pDirectionArrow = new Arrow( this, .1f, .04f, 24, .005f, 12 );
		m_pDirectionArrow->SetScale( 1.2f, 1, 1 );
		m_pDirectionArrow->SetColor( 0, 1, 0 );
		m_pDirectionArrow->SetOrientationYPR( 90, 0, 0 );
	}

	void SphericalDirectionIndicator::UpdateOrientation()
	{
		SetOrientationYPR( m_dPhiDeg, m_dThetaDeg, .0f );
	}

	void SphericalDirectionIndicator::SetDirectionDeg( double dPhiDeg, double dThetaDeg )
	{
		m_dPhiDeg = dPhiDeg;
		m_dThetaDeg = dThetaDeg;
		UpdateOrientation();
	}

	void SphericalDirectionIndicator::SetDirectionPhiDeg( double dPhiDeg )
	{
		m_dPhiDeg = dPhiDeg;
		UpdateOrientation();
	}

	void SphericalDirectionIndicator::SetDirectionThetaDeg( double dThetaDeg )
	{
		m_dThetaDeg = dThetaDeg;
		UpdateOrientation();
	}

	void SphericalDirectionIndicator::SetColor( const double r, const double g, const double b )
	{
		DAFFVIZ_LOCK_VTK;
		m_pDirectionArrow->SetColor( r, g, b );
		DAFFVIZ_UNLOCK_VTK;
	}

	void SphericalDirectionIndicator::GetColor( double& r, double& g, double& b )
	{
		m_pDirectionArrow->GetColor( r, g, b );
	}

	double SphericalDirectionIndicator::GetAlpha() const
	{
		return m_pDirectionArrow->GetAlpha();
	}

	void SphericalDirectionIndicator::SetAlpha( double a )
	{
		DAFFVIZ_LOCK_VTK;
		m_pDirectionArrow->SetAlpha( a );
		DAFFVIZ_UNLOCK_VTK;
	}

} // End of namespace "DAFFViz"
