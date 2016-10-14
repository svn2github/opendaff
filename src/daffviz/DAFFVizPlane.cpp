#include <daffviz/DAFFVizPlane.h>
#include <daffviz/DAFFVizGlobalLock.h>

#include <vtkActor.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

namespace DAFFViz
{

	Plane::Plane( SGNode* pParentNode )
		: SGNode( pParentNode )
		, m_pSource( NULL )
		, m_pMapper( NULL )
		, m_pActor( NULL )
	{
		init();
	}

    Plane::Plane( SGNode* pParentNode, double x1, double y1, double z1, double x2, double y2, double z2, double n1, double n2, double n3 )
		: SGNode( pParentNode )
		, m_pSource( NULL )
		, m_pMapper( NULL )
		, m_pActor( NULL )
	{
		init();
		m_pSource->SetCenter( 1.0, 0.0, 0.0 );
		m_pSource->SetNormal( 1.0, 0.0, 1.0 );
		m_pSource->Update();
	}

	Plane::~Plane()
	{
		RemoveActor( m_pActor );
	}

	void Plane::init()
	{
		m_pSource = vtkSmartPointer< vtkPlaneSource >::New();
	
		m_pMapper = vtkSmartPointer< vtkPolyDataMapper >::New();
		m_pMapper->SetInputData( m_pSource->GetOutput() );

		m_pActor = vtkSmartPointer< vtkActor >::New();
		m_pActor->SetMapper( m_pMapper );

		AddActor( m_pActor );
	}

	// --= object related methods =--


	// --= general methods =--

	void Plane::SetColor( double r, double g, double b )
	{
		DAFFVIZ_LOCK_VTK;
		m_pActor->GetProperty()->SetColor( r, g, b );
		DAFFVIZ_UNLOCK_VTK;
	}

	void Plane::GetColor( double& r, double& g, double& b )
	{
		m_pActor->GetProperty()->GetColor( r, g, b );
	}

	double Plane::GetAlpha() const
	{
		return m_pActor->GetProperty()->GetOpacity();
	}

	void Plane::SetAlpha( double a )
	{
		DAFFVIZ_LOCK_VTK;
		m_pActor->GetProperty()->SetOpacity( a );
		DAFFVIZ_UNLOCK_VTK;
	}

	bool Plane::IsVisible() const
	{
		return m_pActor->GetVisibility() > 0 ? true : false;
	}

	void Plane::SetVisible( bool bVisible )
	{
		SGNode::SetVisible( bVisible );
	
		DAFFVIZ_LOCK_VTK;
		if( bVisible )
			m_pActor->VisibilityOn();
		else
			m_pActor->VisibilityOff();
		DAFFVIZ_UNLOCK_VTK;
	}

} // End of namespace "DAFFViz"
