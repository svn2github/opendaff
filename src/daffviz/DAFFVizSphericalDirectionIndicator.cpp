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

	SphericalDirectionIndicator::SphericalDirectionIndicator()
		: SGNode()
		, m_pDirectionArrow( NULL )
		, m_pDirectionCell( NULL )
	{
		init();
	}

	SphericalDirectionIndicator::SphericalDirectionIndicator( DAFFViz::SGNode* pParentNode )
		: SGNode( pParentNode )
		, m_pDirectionArrow( NULL )
		, m_pDirectionCell( NULL )
	{
		init();
	}

	void SphericalDirectionIndicator::init()
	{
		m_pDirectionArrow = new Arrow( this, 0.02f, 0.02f, 4, 0.01f, 4 );
		m_pDirectionArrow->SetScale( 1.1, 1, 1 );
		m_pDirectionArrow->SetColor( 1, 0, 0 );
		m_pDirectionArrow->SetAlpha( 0.4f );

		m_pDirectionCell = new Plane( this, 0.01f, 0.01f, 1.05f, -0.01f, -0.01f, 1.05f, 0.0f, 0.0f, 1.0f );
		m_pDirectionCell->SetColor( 1, 0, 0 );
		m_pDirectionCell->SetAlpha( 0.4f );

		m_pDirectionCell->SetScale( 10, 1, 2 );
	}

	SphericalDirectionIndicator::~SphericalDirectionIndicator()
	{
	}

	void SphericalDirectionIndicator::SetDirectionObjectView( double dPhi, double dTheta )
	{

	}

	void SphericalDirectionIndicator::SetDirectionDataView( double dPhi, double dTheta )
	{

	}

	void SphericalDirectionIndicator::SetOrientationOfData( const DAFFOrientationYPR& oOrientDeg )
	{

	}

	void SphericalDirectionIndicator::SetColor( const double r, const double g, const double b )
	{
		DAFFVIZ_LOCK_VTK;
		m_pDirectionArrow->SetColor( r, g, b );
		m_pDirectionCell->SetColor( r, g, b );
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
		m_pDirectionCell->SetAlpha( a );
		DAFFVIZ_UNLOCK_VTK;
	}

} // End of namespace "DAFFViz"
