#include <daffviz/DAFFVizSphericalCoordinateAssistant.h>

#include <daffviz/DAFFVizGlobalLock.h>
#include <sstream>

#include <daffviz/DAFFViz.h>

#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyLine.h>
#include <vtkProperty.h>
#include <vtkFollower.h>
#include <vtkLine.h>
#include <vtkVectorText.h>
#include <vtkCamera.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>

namespace DAFFViz
{
	static float PI_F = std::acos( -1.0f );

	SphericalCoordinateAssistant::SphericalCoordinateAssistant( SGNode* pParent, double dMin, double dMax, double dPrec, double dScaleRes )
		: SGNode( pParent ), m_dMin( dMin ), m_dMax( dMax ), m_dPrecision( dPrec ), m_pCamera( 0 ), m_dScaleRes( dScaleRes )
	{
		init();
	}

	void SphericalCoordinateAssistant::init()
	{
		// View (yellow) and up vector (white)
		m_pViewUpVectors = new SGNode( this );
		Arrow* view = new Arrow( m_pViewUpVectors, 0.15, 0.06, 36, 0.02, 36 );
		view->SetColor( 1, 1, 0 );
		view->SetOrientationYPR( 90, 0, 0 );
		view->SetScale( 1.21, 1.0, 1.0 );

		Arrow* up = new Arrow( m_pViewUpVectors, 0.15, 0.06, 36, 0.02, 36 );
		up->SetColor( 1, 1, 1 );
		up->SetOrientationYPR( 0, 0, -90 );
		up->SetScale( 1.21, 1.0, 1.0 );

		// Pole balls
		m_pPoles = new SGNode( this );
		Sphere* northpole = new Sphere( m_pPoles, 0.04, 24, 12 );
		northpole->SetPosition( 0, 1.25, 0 );
		northpole->SetColor( 1, 0, 0 );

		Sphere* southpole = new Sphere( m_pPoles, 0.04, 24, 12 );
		southpole->SetPosition( 0, -1.25, 0 );
		southpole->SetColor( 0, 1, 0 );

		Line* sphereaxis = new Line( m_pPoles, 0, 1.25, 0, 0, -1.25, 0 );
		sphereaxis->SetColor( 0.5, 0.5, 0.5 );
		sphereaxis->SetAlpha( 0.9 );

		// Meridians and Equator
		int res = 18 * 3;

		vtkSmartPointer< vtkPoints > prime_meridian = vtkSmartPointer< vtkPoints >::New();
		vtkSmartPointer< vtkPoints > meridian90 = vtkSmartPointer< vtkPoints >::New();
		vtkSmartPointer< vtkPoints > meridian180 = vtkSmartPointer< vtkPoints >::New();
		vtkSmartPointer< vtkPoints > meridian270 = vtkSmartPointer< vtkPoints >::New();

		prime_meridian->SetNumberOfPoints( res + 1 );
		meridian90->SetNumberOfPoints( res + 1 );
		meridian180->SetNumberOfPoints( res + 1 );
		meridian270->SetNumberOfPoints( res + 1 );

		vtkSmartPointer< vtkPolyLine > polyline = vtkSmartPointer< vtkPolyLine >::New();
		polyline->GetPointIds()->SetNumberOfIds( res + 1 );

		float fAngleRad;
		for( int i = 0; i < res + 1; i++ )
		{
			fAngleRad = i * PI_F / res;
			prime_meridian->InsertPoint( i, 0, -cos( fAngleRad ), sin( fAngleRad ) );
			meridian90->InsertPoint( i, sin( fAngleRad ), -cos( fAngleRad ), 0 );
			meridian180->InsertPoint( i, 0, -cos( fAngleRad ), -sin( fAngleRad ) );
			meridian270->InsertPoint( i, -sin( fAngleRad ), -cos( fAngleRad ), 0 );
			polyline->GetPointIds()->SetId( i, i );
		}

		// Prime meridian
		vtkSmartPointer< vtkCellArray > cells = vtkSmartPointer< vtkCellArray >::New();
		cells->InsertNextCell( polyline );

		vtkSmartPointer< vtkPolyData > polydata = vtkSmartPointer< vtkPolyData >::New();
		polydata->SetPoints( prime_meridian );
		polydata->SetLines( cells );

		vtkSmartPointer< vtkPolyDataMapper > mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
		mapper->SetInputData( polydata );

		vtkSmartPointer< vtkActor > actor = vtkSmartPointer< vtkActor >::New();
		actor->SetMapper( mapper );

		actor->GetProperty()->SetOpacity( 0.4 );

		AddActor( actor );
		m_pMeridians.push_back( actor );

		// 90 degree meridian
		polydata = vtkSmartPointer< vtkPolyData >::New();
		polydata->SetPoints( meridian90 );
		polydata->SetLines( cells );

		mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
		mapper->SetInputData( polydata );

		actor = vtkSmartPointer< vtkActor >::New();
		actor->SetMapper( mapper );

		actor->GetProperty()->SetOpacity( 0.2 );

		AddActor( actor );
		m_pMeridians.push_back( actor );

		// 180 degree meridian
		polydata = vtkSmartPointer< vtkPolyData >::New();
		polydata->SetPoints( meridian180 );
		polydata->SetLines( cells );

		mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
		mapper->SetInputData( polydata );

		actor = vtkSmartPointer< vtkActor >::New();
		actor->SetMapper( mapper );

		actor->GetProperty()->SetOpacity( 0.2 );

		AddActor( actor );
		m_pMeridians.push_back( actor );

		// 270 degree meridian
		polydata = vtkSmartPointer< vtkPolyData >::New();
		polydata->SetPoints( meridian270 );
		polydata->SetLines( cells );

		mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
		mapper->SetInputData( polydata );

		actor = vtkSmartPointer< vtkActor >::New();
		actor->SetMapper( mapper );

		actor->GetProperty()->SetOpacity( 0.2 );

		AddActor( actor );
		m_pMeridians.push_back( actor );

		// Equator
		vtkSmartPointer< vtkPoints > equator = vtkSmartPointer< vtkPoints >::New();
		equator->SetNumberOfPoints( 2 * res );

		polyline = vtkSmartPointer< vtkPolyLine >::New();
		polyline->GetPointIds()->SetNumberOfIds( 2 * res );

		for( int i = 0; i < 2 * res; i++ ) {
			fAngleRad = i * 2 * PI_F / ( 2 * res - 1 );
			equator->InsertPoint( i, sin( fAngleRad ), 0, -cos( fAngleRad ) );
			polyline->GetPointIds()->SetId( i, i );
		}

		cells = vtkSmartPointer< vtkCellArray >::New();
		cells->InsertNextCell( polyline );

		polydata = vtkSmartPointer< vtkPolyData >::New();
		polydata->SetPoints( equator );
		polydata->SetLines( cells );

		mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
		mapper->SetInputData( polydata );

		m_pEquator = vtkSmartPointer< vtkActor >::New();
		m_pEquator->SetMapper( mapper );

		m_pEquator->GetProperty()->SetOpacity( 0.4 );

		AddActor( m_pEquator );

		// Reference

		// create a reference sphere
		m_pSphere = vtkSmartPointer< vtkSphereSource >::New();
		m_pSphere->SetRadius( 0 );
		m_pSphere->SetThetaResolution( res );
		m_pSphere->SetPhiResolution( res );

		// Create a mapper and actor
		mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
		mapper->SetInputData( m_pSphere->GetOutput() );

		m_pReferenceActor = vtkSmartPointer< vtkActor >::New();
		m_pReferenceActor->SetMapper( mapper );
		m_pReferenceActor->GetProperty()->SetOpacity( 0.0 );

		AddActor( m_pReferenceActor );

		// Grid
		UpdateGrid();
	}

	SphericalCoordinateAssistant::~SphericalCoordinateAssistant()
	{
		RemoveActor( m_pReferenceActor );
		RemoveActor( m_pEquator );

		for( size_t i = 0; i < m_pGrid.size(); i++ )
		{
			RemoveActor( m_pGrid.at( i ) );
		}
		m_pGrid.clear();

		for( size_t i = 0; i < m_pLabels.size(); i++ )
		{
			RemoveActor( m_pLabels.at( i ) );
		}
		m_pLabels.clear();

		for( size_t i = 0; i < m_pMeridians.size(); i++ )
		{
			RemoveActor( m_pMeridians.at( i ) );
		}
		m_pMeridians.clear();
		for( size_t i = 0; i < m_pCircles.size(); i++ )
		{
			RemoveActor( m_pCircles.at( i ) );
		}
		m_pCircles.clear();

		//delete m_pViewUpVectors;
		//delete m_pPoles;
	}

	void SphericalCoordinateAssistant::SetAssistanceVisible( bool bVisible )
	{
		SetEquatorVisible( bVisible );
		SetMeridiansVisible( bVisible );
		SetGridVisible( bVisible );
	}

	void SphericalCoordinateAssistant::UpdateGrid() {
		bool bWasVisible = GetGridVisible();
		int res = 18 * 3;
		float fAngleRad;
		// Delete old Grid
		// TODO: is it sufficient to delete the actor or do i have to delete polyline/cells/polydata as well?
		for( int i = 0; i < ( int ) m_pGrid.size(); i++ ) {
			RemoveActor( m_pGrid.at( i ) );
			m_pGrid.at( i )->GetMapper()->GetInput()->Delete();
		}
		m_pGrid.clear();
		for( int i = 0; i < ( int ) m_pCircles.size(); i++ ) {
			RemoveActor( m_pCircles.at( i ) );
			m_pCircles.at( i )->GetMapper()->GetInput()->Delete();
		}
		m_pCircles.clear();
		for( int i = 0; i < ( int ) m_pLabels.size(); i++ ) {
			RemoveActor( m_pLabels.at( i ) );
			//m_pLabels.at(i)->GetMapper()->GetInput()->Delete();
		}
		m_pLabels.clear();

		// create new grid
		vtkSmartPointer< vtkCellArray> cells;
		vtkSmartPointer< vtkPolyData> polydata;
		vtkSmartPointer< vtkPolyLine> polyline;
		vtkSmartPointer< vtkFollower> actor;
		vtkSmartPointer< vtkPolyDataMapper> mapper;
		vtkSmartPointer< vtkVectorText> pLabel;

		double min = ceil( m_dMin / m_dPrecision )*m_dPrecision; // inner gridline
		double max = floor( m_dMax / m_dPrecision )*m_dPrecision; // outter gridline
		//int gridRes = (max-min)/m_dPrecision; // total number of gridlines
		for( double f = min; f <= m_dMax; f += m_dPrecision ) {
			vtkSmartPointer< vtkPoints> gridLine = vtkSmartPointer< vtkPoints>::New();
			gridLine->SetNumberOfPoints( 2 * res );

			polyline = vtkSmartPointer< vtkPolyLine>::New();
			polyline->GetPointIds()->SetNumberOfIds( 2 * res );

			double factor = ( f - m_dMin ) / ( m_dMax - m_dMin );

			for( int i = 0; i < 2 * res; i++ ) {
				fAngleRad = i * 2 * PI_F / ( 2 * res - 1 );
				gridLine->InsertPoint( i, sin( fAngleRad )*factor, 0, -cos( fAngleRad )*factor );
				polyline->GetPointIds()->SetId( i, i );
			}

			cells = vtkSmartPointer< vtkCellArray>::New();
			cells->InsertNextCell( polyline );

			polydata = vtkSmartPointer< vtkPolyData>::New();
			polydata->SetPoints( gridLine );
			polydata->SetLines( cells );

			mapper = vtkSmartPointer< vtkPolyDataMapper>::New();
			mapper->SetInputData( polydata );

			actor = vtkSmartPointer< vtkFollower>::New();
			actor->SetMapper( mapper );
			actor->RotateX( 90 );
			if( m_pCamera != 0 )
				actor->SetCamera( m_pCamera );


			actor->GetProperty()->SetOpacity( 0.15 );

			AddActor( actor );
			m_pCircles.push_back( actor );

			// label
			pLabel = vtkSmartPointer< vtkVectorText>::New();
			std::ostringstream s;
			s << f;
			pLabel->SetText( s.str().c_str() );

			mapper = vtkSmartPointer< vtkPolyDataMapper>::New();
			mapper->SetInputConnection( pLabel->GetOutputPort() );

			actor = vtkSmartPointer< vtkFollower>::New();
			actor->SetMapper( mapper );
			actor->SetScale( 0.025 );
			actor->SetPosition( 0.02, factor + .01, 0 );
			actor->GetProperty()->SetOpacity( 0.2 );

			if( m_pCamera != 0 )
				actor->SetCamera( m_pCamera );

			AddActor( actor );
			m_pLabels.push_back( actor );
		}

		// Scale

		vtkSmartPointer< vtkPoints> scale = vtkSmartPointer< vtkPoints>::New();
		scale->SetNumberOfPoints( 6 * m_dScaleRes );

		cells = vtkSmartPointer< vtkCellArray>::New();
		vtkSmartPointer< vtkLine > line;

		for( int i = 0; i < m_dScaleRes; i++ ) {
			fAngleRad = i * 2 * PI_F / m_dScaleRes;
			if( fmod( ( double ) i / m_dScaleRes, .25 ) == 0.0 ) // skip  0° 90° 180° 270° lines
				continue;
			// equator
			scale->InsertPoint( 6 * i, 0.975*sin( fAngleRad ), 0, -0.975*cos( fAngleRad ) );
			scale->InsertPoint( 6 * i + 1, 1.025*sin( fAngleRad ), 0, -1.025*cos( fAngleRad ) );
			line = vtkSmartPointer< vtkLine>::New();
			line->GetPointIds()->SetId( 0, 6 * i );
			line->GetPointIds()->SetId( 1, 6 * i + 1 );
			cells->InsertNextCell( line );
			// prime meridian
			scale->InsertPoint( 6 * i + 2, 0, -0.975*cos( fAngleRad ), 0.975*sin( fAngleRad ) );
			scale->InsertPoint( 6 * i + 3, 0, -1.025*cos( fAngleRad ), 1.025*sin( fAngleRad ) );
			line = vtkSmartPointer< vtkLine>::New();
			line->GetPointIds()->SetId( 0, 6 * i + 2 );
			line->GetPointIds()->SetId( 1, 6 * i + 3 );
			cells->InsertNextCell( line );
			// meridian 90 degree
			scale->InsertPoint( 6 * i + 4, 0.975*sin( fAngleRad ), -0.975*cos( fAngleRad ), 0 );
			scale->InsertPoint( 6 * i + 5, 1.025*sin( fAngleRad ), -1.025*cos( fAngleRad ), 0 );
			line = vtkSmartPointer< vtkLine>::New();
			line->GetPointIds()->SetId( 0, 6 * i + 4 );
			line->GetPointIds()->SetId( 1, 6 * i + 5 );
			cells->InsertNextCell( line );
		}

		polydata = vtkSmartPointer< vtkPolyData>::New();
		polydata->SetPoints( scale );
		polydata->SetLines( cells );

		mapper = vtkSmartPointer< vtkPolyDataMapper>::New();
		mapper->SetInputData( polydata );

		actor = vtkSmartPointer< vtkFollower>::New();
		actor->SetMapper( mapper );

		actor->GetProperty()->SetOpacity( 0.2 );

		AddActor( actor );
		m_pGrid.push_back( actor );


		// draw thicker 0° 90° 180° 270° lines
		scale = vtkSmartPointer< vtkPoints >::New();
		scale->SetNumberOfPoints( 8 );

		cells = vtkSmartPointer< vtkCellArray>::New();

		for( int i = 0; i < 4; i++ )
		{
			line = vtkSmartPointer< vtkLine>::New();
			fAngleRad = i * 2 * PI_F / 4;
			scale->InsertPoint( 2 * i, .975*sin( fAngleRad ), 0, -.975*cos( fAngleRad ) );
			scale->InsertPoint( 2 * i + 1, 1.075*sin( fAngleRad ), 0, -1.075*cos( fAngleRad ) );
			line->GetPointIds()->SetId( 0, 2 * i );
			line->GetPointIds()->SetId( 1, 2 * i + 1 );
			cells->InsertNextCell( line );
		}

		polydata = vtkSmartPointer< vtkPolyData>::New();
		polydata->SetPoints( scale );
		polydata->SetLines( cells );

		mapper = vtkSmartPointer< vtkPolyDataMapper>::New();
		mapper->SetInputData( polydata );

		actor = vtkSmartPointer< vtkFollower>::New();
		actor->SetMapper( mapper );

		actor->GetProperty()->SetOpacity( 0.4 );

		AddActor( actor );
		m_pGrid.push_back( actor );

		if( !bWasVisible )
			SetGridVisible( false );
		UpdateReference();
	}

	void SphericalCoordinateAssistant::SetVisible( bool bVisible )
	{
		SGNode::SetVisible( bVisible );

		//m_pViewUpVectors->SetVisible(bVisible);
		//m_pPoles->SetVisible(bVisible);

		if( bVisible )
		{
			m_pEquator->VisibilityOn();
			for( size_t i = 0; i < m_pMeridians.size(); i++ )
				m_pMeridians[ i ]->VisibilityOn();
			for( size_t i = 0; i < ( int ) m_pGrid.size(); i++ )
				m_pGrid[ i ]->VisibilityOn();
			for( size_t i = 0; i < ( int ) m_pCircles.size(); i++ )
				m_pCircles[ i ]->VisibilityOn();
			for( size_t i = 0; i < ( int ) m_pLabels.size(); i++ )
				m_pLabels[ i ]->VisibilityOn();
		}
		else
		{
			m_pEquator->VisibilityOff();
			for( size_t i = 0; i < m_pMeridians.size(); i++ )
				m_pMeridians[ i ]->VisibilityOff();
			for( size_t i = 0; i < m_pGrid.size(); i++ )
				m_pGrid[ i ]->VisibilityOff();
			for( size_t i = 0; i < ( int ) m_pCircles.size(); i++ )
				m_pCircles[ i ]->VisibilityOff();
			for( size_t i = 0; i < m_pLabels.size(); i++ )
				m_pLabels[ i ]->VisibilityOff();
		}
	}

	void SphericalCoordinateAssistant::SetViewUpVectorsVisible( bool bVisible )
	{
		m_pViewUpVectors->SetVisible( bVisible );
	}

	void SphericalCoordinateAssistant::SetMeridiansVisible( bool bVisible )
	{
		if( bVisible )
			for( int i = 0; i < ( int ) m_pMeridians.size(); i++ )
				m_pMeridians[ i ]->VisibilityOn();
		else
			for( int i = 0; i < ( int ) m_pMeridians.size(); i++ )
				m_pMeridians[ i ]->VisibilityOff();
	}

	void SphericalCoordinateAssistant::SetGridVisible( bool bVisible )
	{
		if( bVisible )
		{
			for( int i = 0; i < ( int ) m_pGrid.size(); i++ )
				m_pGrid[ i ]->VisibilityOn();
			for( int i = 0; i < ( int ) m_pCircles.size(); i++ )
				m_pCircles[ i ]->VisibilityOn();
			for( int i = 0; i < ( int ) m_pLabels.size(); i++ )
				m_pLabels[ i ]->VisibilityOn();
		}
		else
		{
			for( int i = 0; i < ( int ) m_pGrid.size(); i++ )
				m_pGrid[ i ]->VisibilityOff();
			for( int i = 0; i < ( int ) m_pCircles.size(); i++ )
				m_pCircles[ i ]->VisibilityOff();
			for( int i = 0; i < ( int ) m_pLabels.size(); i++ )
				m_pLabels[ i ]->VisibilityOff();
		}
	}

	bool SphericalCoordinateAssistant::GetGridVisible() const
	{
		return ( m_pGrid.size()>0 ) ? m_pGrid[ 0 ]->GetVisibility() != 0 : true;
	}
	
	bool SphericalCoordinateAssistant::GetAxesVisible() const
	{
		return ( m_pPoles ) ? m_pPoles->IsVisible() : true;
	}
	
	bool SphericalCoordinateAssistant::GetViewUpVectorsVisible() const
	{
		return ( m_pViewUpVectors ) ? m_pViewUpVectors->IsVisible() : true;
	}

	void SphericalCoordinateAssistant::SetEquatorVisible( bool bVisible )
	{
		if( bVisible )
			m_pEquator->VisibilityOn();
		else
			m_pEquator->VisibilityOff();
	}

	void SphericalCoordinateAssistant::SetPolesVisible( bool bVisible )
	{
		m_pPoles->SetVisible( bVisible );
	}

	void SphericalCoordinateAssistant::SetReferenceLevel( double dLevel ){ m_dReferenceLevel = dLevel; }

	void SphericalCoordinateAssistant::SetReferenceOpacity( double dOpacity ) { m_dReferenceOpacity = dOpacity; }

	void SphericalCoordinateAssistant::SetMin( double min ) { m_dMin = min; }

	void SphericalCoordinateAssistant::SetMax( double max ) { m_dMax = max; }

	void SphericalCoordinateAssistant::SetRResolution( double prec ) { m_dPrecision = prec; }

	void SphericalCoordinateAssistant::SetPhiResolution( double res ) { m_dScaleRes = 360 / res; }

	double SphericalCoordinateAssistant::GetMin() const { return m_dMin; }

	double SphericalCoordinateAssistant::GetMax() const { return m_dMax; }

	double SphericalCoordinateAssistant::GetRResolution() const { return m_dPrecision; }

	double SphericalCoordinateAssistant::GetPhiResolution() const { return 360 / m_dScaleRes; }

	void SphericalCoordinateAssistant::OnSetFollowerCamera( vtkCamera* pCamera )
	{
		//DAFFVIZ_LOCK_VTK;
		for( int i = 0; i < ( int ) m_pCircles.size(); i++ )
		{
			m_pCircles[ i ]->SetCamera( pCamera );
		}
		for( int i = 0; i < ( int ) m_pLabels.size(); i++ )
		{
			m_pLabels[ i ]->SetCamera( pCamera );
		}
		m_pCamera = pCamera;
		//DAFFVIZ_UNLOCK_VTK;

		// Delegate
		SGNode::OnSetFollowerCamera( pCamera );
	}

	void SphericalCoordinateAssistant::UpdateReference()
	{
		double factor = ( m_dReferenceLevel - m_dMin ) / ( m_dMax - m_dMin );
		m_pSphere->SetRadius( factor );
		m_pReferenceActor->GetProperty()->SetOpacity( m_dReferenceOpacity );
	}

} // End of namespace "DAFFViz"
