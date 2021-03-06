#include <daffviz/DAFFVizBalloonPlot.h>
#include <DAFFUtils.h>

// Todo: strip required
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkContourFilter.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkFollower.h>
#include <vtkHedgeHog.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyLine.h>
#include <vtkLine.h>
#include <vtkPolygon.h>
#include <vtkProperty.h>
#include <vtkVectorText.h>
#include <vtkWarpScalar.h>
#include <vtkVectorText.h>
#include <vtkScalarBarActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>
#include <vtkWindowedSincPolyDataFilter.h>

#include <DAFF.h>

#include <algorithm>
#include <assert.h>


namespace DAFFViz
{
	BalloonPlot::BalloonPlot( SGNode* pParentNode, const DAFFContent* pContent )
		: SGNode( pParentNode ),
		m_pContent( pContent ),
		m_iFrequency( 0 ),
		m_iNumFrequencies( 0 ),
		m_iScaling( SCALING_DECIBEL ),
		m_dMin( 0.0 ), m_dMax( 1.0 ),
		m_iChannel( 0 ),
		m_bWarp( true ),
		m_bNormalize( false ),
		m_bNormalizeFreqsIndiv( false ),
		m_bUseCustomRange( false ),
		m_bUsePhaseAsColor( false )
	{
		init();
	}

	BalloonPlot::BalloonPlot( const DAFFContent* pContent )
		: SGNode(),
		m_pContent( pContent ),
		m_iFrequency( 0 ),
		m_iNumFrequencies( 0 ),
		m_iScaling( SCALING_DECIBEL ),
		m_dMin( 0.0 ), m_dMax( 1.0 ),
		m_iChannel( 0 ),
		m_bWarp( true ),
		m_bNormalize( false ),
		m_bNormalizeFreqsIndiv( false ),
		m_bUseCustomRange( false ),
		m_bUsePhaseAsColor( false )
	{
		init();
	}

	BalloonPlot::~BalloonPlot()
	{
		RemoveActor( m_pPlotActor );
		RemoveActor( m_pLabel );
		RemoveActor( m_pProbe );
	}

	void BalloonPlot::init()
	{

		const DAFFProperties* pProps = m_pContent->getProperties();

		switch( pProps->getContentType() )
		{
		case DAFF_DFT_SPECTRUM:
			m_iNumFrequencies = dynamic_cast< const DAFFContentDFT* >( m_pContent )->getNumDFTCoeffs();
			break;

		case DAFF_MAGNITUDE_SPECTRUM:
		case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		case DAFF_PHASE_SPECTRUM:
			m_iNumFrequencies = dynamic_cast< const DAFFContentMS* >( m_pContent )->getNumFrequencies();
			break;
		}

		bool bSouthPole = pProps->getBetaStart() == 0;
		bool bNorthPole = pProps->getBetaEnd() == 180;
		bool bAzimuthWrap = ( pProps->getAlphaStart() == 0 ) && ( pProps->getAlphaEnd() == 360 );



		// --= Sphere grid =--

		vtkSmartPointer< vtkPoints > points = vtkSmartPointer< vtkPoints >::New();
		m_pNormals = vtkSmartPointer< vtkDoubleArray >::New();
		m_pNormals->SetNumberOfComponents( 3 );
		for( int i = 0; i < pProps->getNumberOfRecords(); i++ )
		{
			// TODO: Topologies: Rings, Patches, etc.
			float phi, theta;
			m_pContent->getRecordCoords( i, DAFF_OBJECT_VIEW, phi, theta );

			// Compute Cartesian points
			double x, y, z;
			sph2cart( phi, theta, x, y, z );
			points->InsertPoint( i, x, y, z );
			double n[ 3 ] = { -x, -y, -z }; // define flipped normals for warping
			m_pNormals->InsertNextTuple( n );
		}


		// --= Faces =--

		vtkSmartPointer< vtkCellArray > cells = vtkSmartPointer< vtkCellArray >::New();
		vtkIdType quadface[ 4 ];
		vtkIdType triangleface[ 3 ];

		int iNumPoints = pProps->getNumberOfRecords();
		int iAziPoints = pProps->getAlphaPoints();
		int iElePoints = pProps->getBetaPoints();

		int iBodyEleRings = iElePoints - 1;
		int iAziCount = ( bAzimuthWrap ? iAziPoints : iAziPoints - 1 );
		if( bSouthPole ) iBodyEleRings--;
		if( bNorthPole ) iBodyEleRings--;
		int iBodyOffset = 0;
		if( bSouthPole )
			iBodyOffset = 1;

		// if there is only one azimuth point there would be no surface to draw, so lets draw a line
		if( ( iAziPoints == 1 ) || ( iElePoints == 1 ) )
		{
			if( ( iAziPoints == 1 ) && ( iElePoints == 1 ) )
			{
				// we will not plot a single point
			}
			else
			{
				// add a point at center
				points->InsertPoint( iNumPoints, 0, 0, 0 );

				for( int i = 0; i < iNumPoints; i++ )
				{
					triangleface[ 0 ] = i;
					triangleface[ 1 ] = i + 1;
					triangleface[ 2 ] = iNumPoints;

					for( int k = 0; k < 3; k++ )
						assert( ( triangleface[ k ] >= 0 ) && ( triangleface[ k ] < iNumPoints + 1 ) );

					cells->InsertNextCell( 3, triangleface );
				}

			}
		}
		else
		{
			// draw surfaces
			if( bSouthPole )
			{
				/*
				 *  If there is a south pole, this has index 0.
				 *  The next record, one elevation step above has then index 1.
				 */

				iBodyOffset = 1;
				for( int j = 0; j < iAziCount; j++ )
				{
					triangleface[ 0 ] = 0;
					triangleface[ 1 ] = iBodyOffset + j;
					triangleface[ 2 ] = iBodyOffset + ( j + 1 ) % iAziPoints;

					for( int k = 0; k < 3; k++ )
						assert( ( triangleface[ k ] >= 0 ) && ( triangleface[ k ] < iNumPoints ) );

					cells->InsertNextCell( 3, triangleface );
				}
			};

			// Body
			for( int i = 0; i < iBodyEleRings; i++ )
			{
				for( int j = 0; j < iAziCount; j++ )
				{
					quadface[ 0 ] = iBodyOffset + i*iAziPoints + j;
					quadface[ 1 ] = iBodyOffset + i*iAziPoints + ( j + 1 ) % iAziPoints;
					quadface[ 2 ] = iBodyOffset + ( i + 1 )*iAziPoints + ( j + 1 ) % iAziPoints;
					quadface[ 3 ] = iBodyOffset + ( i + 1 )*iAziPoints + j;

					for( int k = 0; k < 4; k++ )
						assert( ( quadface[ k ] >= 0 ) && ( quadface[ k ] < iNumPoints ) );

					cells->InsertNextCell( 4, quadface );
				}
			}

			// North pole

			if( bNorthPole )
			{
				/*
				 *  If there is a north pole, this has the last index.
				 *  The next record, one elevation step above has then index 1.
				 */

				for( int j = 0; j < iAziCount; j++ )
				{
					triangleface[ 0 ] = iNumPoints - 1;
					triangleface[ 1 ] = iBodyOffset + iBodyEleRings*iAziPoints + j;
					triangleface[ 2 ] = iBodyOffset + iBodyEleRings*iAziPoints + ( j + 1 ) % iAziPoints;

					for( int k = 0; k < 3; k++ )
						assert( ( triangleface[ k ] >= 0 ) && ( triangleface[ k ] < pProps->getNumberOfRecords() ) );

					cells->InsertNextCell( 3, triangleface );
				}
			}
		}


		// --= VTK stuff =--
		m_pPlotPolydata = vtkSmartPointer< vtkPolyData >::New();
		m_pPlotPolydata->SetPoints( points );
		m_pPlotPolydata->SetPolys( cells );

		// Set scalars of polydata
		SetScalars();
		m_pPlotPolydata->GetPointData()->SetActiveScalars( "magnitudes" );

		// Warp sphere
		m_pWarp = vtkSmartPointer< vtkWarpScalar >::New();
		m_pWarp->SetInputData( m_pPlotPolydata );
		m_pWarp->UseNormalOff();

		m_pMapper = vtkSmartPointer< vtkPolyDataMapper >::New();
		m_pMapper->SetScalarModeToUsePointFieldData();

		if( m_bWarp )
			EnableWarp();
		else
			DisableWarp();

		SetUsePhaseAsColor( m_bUsePhaseAsColor );

		m_pPlotActor = vtkSmartPointer< vtkActor >::New();
		m_pPlotActor->SetMapper( m_pMapper );

		m_pPlotActor->GetProperty()->SetInterpolationToGouraud();

		AddActor( m_pPlotActor );

		// Probe

		vtkSmartPointer< vtkPoints > points2 = vtkSmartPointer< vtkPoints >::New();
		points2->SetNumberOfPoints( 2 );
		points2->InsertPoint( 0, 0, 0, 0 );
		points2->InsertPoint( 1, 0, -1, 0 );

		vtkSmartPointer< vtkCellArray >cells2 = vtkSmartPointer< vtkCellArray >::New();
		vtkSmartPointer< vtkLine > line = vtkSmartPointer< vtkLine >::New();
		line->GetPointIds()->SetId( 0, 0 );
		line->GetPointIds()->SetId( 1, 1 );
		cells2->InsertNextCell( line );

		vtkSmartPointer< vtkPolyData > polydata = vtkSmartPointer< vtkPolyData >::New();
		polydata->SetPoints( points2 );
		polydata->SetLines( cells2 );

		// Create a mapper and actor
		vtkSmartPointer< vtkPolyDataMapper > mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
		mapper->SetInputData( polydata );

		m_pProbe = vtkSmartPointer< vtkActor >::New();
		m_pProbe->SetMapper( mapper );
		m_pProbe->VisibilityOff();

		AddActor( m_pProbe );

		// label the probe
		m_pProbeLabel = vtkSmartPointer< vtkVectorText >::New();
		std::ostringstream s;
		s << "test";
		m_pProbeLabel->SetText( s.str().c_str() );

		vtkSmartPointer< vtkPolyDataMapper >mapper2 = vtkSmartPointer< vtkPolyDataMapper >::New();
		mapper2->AddInputConnection( m_pProbeLabel->GetOutputPort() );

		m_pLabel = vtkSmartPointer< vtkFollower >::New();
		m_pLabel->SetMapper( mapper2 );
		m_pLabel->SetScale( 0.03 );
		m_pLabel->SetPosition( 0.02, -1.01, 0 );
		m_pLabel->SetOrientation( -90, 0, -90 );
		m_pLabel->GetProperty()->SetOpacity( 0.8 );
		m_pLabel->VisibilityOff();

		AddActor( m_pLabel );

	}

	void BalloonPlot::UpdateProbe()
	{
		if( !m_pProbe )
			return;

		double* pos = m_pProbe->GetPosition();
		//double r = grad2rad();
		double p = DAFFUtils::grad2rad( m_dProbeBeta );
		double y = DAFFUtils::grad2rad( m_dProbeAlpha );

		// Translation matrix: to origin
		vtkSmartPointer< vtkMatrix4x4 > mOrigin = vtkSmartPointer< vtkMatrix4x4 >::New();
		mOrigin->Identity();
		mOrigin->SetElement( 0, 3, -pos[ 0 ] );
		mOrigin->SetElement( 1, 3, -pos[ 1 ] );
		mOrigin->SetElement( 2, 3, -pos[ 2 ] );

		// Translation matrix: to object position
		vtkSmartPointer< vtkMatrix4x4 > mPosition = vtkSmartPointer< vtkMatrix4x4 >::New();
		mPosition->Identity();
		mPosition->SetElement( 0, 3, pos[ 0 ] );
		mPosition->SetElement( 1, 3, pos[ 1 ] );
		mPosition->SetElement( 2, 3, pos[ 2 ] );

		/*
		 *  Roll: homogenous rotation matrix for -Z axis
		 *
		 *
		 *  cos(alpha)   sin(alpha)      0       0
		 *  -sin(alpha)  cos(alpha)      0       0
		 *      0            0           1       0
		 *      0            0           0       1
		 */
		//vtkSmartPointer< vtkMatrix4x4 >* mRoll = vtkSmartPointer< vtkMatrix4x4 >::New();
		//mRoll->Identity();
		//mRoll->SetElement(0,0, cos(r));
		//mRoll->SetElement(0,1, sin(r));
		//mRoll->SetElement(1,0, -sin(r));
		//mRoll->SetElement(1,1, cos(r));

		/*
		 *  Pitch: homogenous rotation matrix for +X axis
		 *
		 *      1            0            0           0
		 *      0       cos(alpha)  -sin(alpha)       0
		 *      0       sin(alpha)   cos(alpha)       0
		 *      0            0            0           1
		 */
		vtkSmartPointer< vtkMatrix4x4 > mPitch = vtkSmartPointer< vtkMatrix4x4 >::New();
		mPitch->Identity();
		mPitch->SetElement( 1, 1, cos( p ) );
		mPitch->SetElement( 1, 2, -sin( p ) );
		mPitch->SetElement( 2, 1, sin( p ) );
		mPitch->SetElement( 2, 2, cos( p ) );

		/*
		 *  Yaw: homogenous rotation matrix for +Y axis
		 *
		 *   cos(alpha)      0       sin(alpha)       0
		 *      0            1            0           0
		 *  -sin(alpha)      0       cos(alpha)       0
		 *      0            0            0           1
		 */
		vtkSmartPointer< vtkMatrix4x4 > mYaw = vtkSmartPointer< vtkMatrix4x4 >::New();
		mYaw->Identity();
		mYaw->SetElement( 0, 0, cos( y ) );
		mYaw->SetElement( 0, 2, sin( y ) );
		mYaw->SetElement( 2, 0, -sin( y ) );
		mYaw->SetElement( 2, 2, cos( y ) );


		// Compose transforms
		// (Order: Origin, Roll, Pitch, Yaw, Position)
		vtkSmartPointer< vtkTransform > transform = vtkSmartPointer< vtkTransform >::New();
		transform->Concatenate( mPosition );
		transform->Concatenate( mYaw );
		transform->Concatenate( mPitch );
		//transform->Concatenate(mRoll);
		transform->Concatenate( mOrigin );

		// Apply new transformation
		m_pProbe->SetUserTransform( transform );
		m_pLabel->SetUserTransform( transform );

		// update label
		assert( m_pContent != 0 );
		double val = 0;
		int i;
		m_pContent->getNearestNeighbour( DAFF_DATA_VIEW, m_dProbeAlpha, m_dProbeBeta, i );
		std::ostringstream s;
		const DAFFContentDFT* pContentDFT = NULL;
		const DAFFContentMS* pContentMS = NULL;
		const DAFFContentMPS* pContentMPS = NULL;

		float fMag, fPhase, fReal, fImag;

		switch( m_pContent->getProperties()->getContentType() )
		{
		case DAFF_DFT_SPECTRUM:
			pContentDFT = dynamic_cast< const DAFFContentDFT* >( m_pContent );
			pContentDFT->getDFTCoeff( i, m_iChannel, m_iFrequency, fReal, fImag );
			fMag = sqrt( fReal*fReal + fImag*fImag );
			if( fReal != 0.0 )
			{
				fPhase = atan( fImag / fReal );
				if( fReal < 0.0 )
					if( fImag < 0.0 )
						fPhase -= DAFF::PI_F;
					else
						fPhase += DAFF::PI_F;
			}
			else
			{
				if( fImag < 0.0 )
					fPhase = -DAFF::HALF_PI_F;
				else
					fPhase = DAFF::HALF_PI_F;
			}
			if( m_iScaling == SCALING_DECIBEL )
			{
				fMag = FactorToDecibel( fMag );
				s << "Real: " << fReal << ", Imag: " << fImag << "\n Mag: " << fMag << "db, Phase:" << fPhase;
			}
			else
				s << "Real: " << fReal << ", Imag: " << fImag << "\n Mag: " << fMag << ", Phase:" << fPhase;
			break;

		case DAFF_MAGNITUDE_SPECTRUM:
			pContentMS = dynamic_cast< const DAFFContentMS* >( m_pContent );
			pContentMS->getMagnitude( i, m_iChannel, m_iFrequency, fMag );
			if( m_iScaling == SCALING_DECIBEL )
			{
				fMag = FactorToDecibel( fMag );
				s << fMag << "db";
			}
			else
				s << fMag;
			break;

		case DAFF_MAGNITUDE_PHASE_SPECTRUM:
			pContentMPS = dynamic_cast< const DAFFContentMPS* >( m_pContent );
			pContentMPS->getMagnitude( i, m_iChannel, m_iFrequency, fMag );
			pContentMPS->getPhase( i, m_iChannel, m_iFrequency, fPhase );
			if( m_iScaling == SCALING_DECIBEL )
			{
				fMag = FactorToDecibel( fMag );
				s << "Mag: " << fMag << "db, Phase: " << fPhase;
			}
			else
				s << "Mag: " << fMag << ", Phase: " << fPhase;
			break;
		}
		m_pProbeLabel->SetText( s.str().c_str() );

		return;
	}

	void BalloonPlot::SetProbeAngles( double dAlpha, double dBeta )
	{
		m_dProbeAlpha = dAlpha;
		m_dProbeBeta = dBeta;
	}

	void BalloonPlot::SetProbeVisible( bool bVisible )
	{
		m_pProbe->SetVisibility( bVisible );
		m_pLabel->SetVisibility( bVisible );
	}

	void BalloonPlot::SetSelectedFrequency( int iFreq )
	{
		m_iFrequency = iFreq;
		SetScalars();

		return;
	}

	int BalloonPlot::GetSelectedFrequency() const
	{
		return m_iFrequency;
	}

	void BalloonPlot::SetScaling( int iScaling )
	{
		m_iScaling = iScaling;
		SetScalars();

		return;
	}

	int BalloonPlot::GetScaling() const
	{
		return m_iScaling;
	}

	void BalloonPlot::SetRange( double dMin, double dMax )
	{
		assert( m_pContent != NULL );
		assert( dMin < dMax );
		if( m_iScaling == SCALING_LINEAR ) {
			// Convert from linear to decibel
			m_dMin = dMin;
			m_dMax = dMax;
		}
		else
		{
			m_dMin = DecibelToFactor( dMin );
			m_dMax = DecibelToFactor( dMax );
		}
		SetScalars();
	}

	void BalloonPlot::SetUseCustomRange( bool bChecked )
	{
		m_bUseCustomRange = bChecked;
		SetScalars();
	}

	double BalloonPlot::GetRangeMin() const
	{
		if( m_iScaling == SCALING_LINEAR )
		{
			return m_dMin;
		}
		else
		{
			return FactorToDecibel( m_dMin );
		}
	}

	double BalloonPlot::GetRangeMax() const
	{
		if( m_iScaling == SCALING_LINEAR ) {
			return m_dMax;
		}
		else
			return FactorToDecibel( m_dMax );
	}

	void BalloonPlot::EnableWarp()
	{
		m_pPlotPolydata->GetPointData()->SetNormals( m_pNormals );
		m_pMapper->SetInputConnection( m_pWarp->GetOutputPort() );
		m_bWarp = true;
	}

	void BalloonPlot::DisableWarp()
	{
		m_pPlotPolydata->GetPointData()->SetNormals( 0 );
		m_pMapper->SetInputData( m_pPlotPolydata );
		m_bWarp = false;
	}

	void BalloonPlot::SetNormalize( bool bChecked )
	{
		m_bNormalize = bChecked;
		SetScalars();
	}

	void BalloonPlot::SetNormalizeFrequenciesIndividually( bool bChecked )
	{
		m_bNormalizeFreqsIndiv = bChecked;
		SetScalars();
	}

	void BalloonPlot::SetScalars()
	{
		assert( m_iFrequency >= 0 && m_iFrequency < m_iNumFrequencies );
		assert( m_pContent != NULL );
		assert( m_dMin < m_dMax );

		const DAFFContentDFT* pContentDFT = NULL;
		const DAFFContentMS* pContentMS = NULL;
		const DAFFContentMPS* pContentMPS = NULL;

		switch( m_pContent->getProperties()->getContentType() )
		{
		case DAFF_DFT_SPECTRUM:
			pContentDFT = dynamic_cast< const DAFFContentDFT* >( m_pContent );
			break;

		case DAFF_MAGNITUDE_SPECTRUM:
			pContentMS = dynamic_cast< const DAFFContentMS* >( m_pContent );
			break;

		case DAFF_MAGNITUDE_PHASE_SPECTRUM:
			pContentMPS = dynamic_cast< const DAFFContentMPS* >( m_pContent );
			break;
		}

		vtkSmartPointer< vtkFloatArray > pMagArray = vtkSmartPointer< vtkFloatArray >::New();

		pMagArray->SetName( "magnitudes" );
		vtkSmartPointer< vtkFloatArray > pPhArray = vtkSmartPointer< vtkFloatArray >::New();
		pPhArray->SetName( "phases" );

		float fMag = 0.0f;
		float fPhase = 0.0f;
		float fAbsoluteMax = 0.0f;
		double dMax = 1.0f;

		//get the normalization range
		if( pContentDFT )
		{
			if( m_iScaling == SCALING_DECIBEL )
			{
				if( m_bNormalizeFreqsIndiv )
					fAbsoluteMax = getMagnitudeMaximum();
				else
					fAbsoluteMax = pContentDFT->getOverallMagnitudeMaximum();
			}
			else
			{
				if( m_bNormalizeFreqsIndiv )
					dMax = getMagnitudeMaximum();
				else
					dMax = pContentDFT->getOverallMagnitudeMaximum();
			}

		}
		else if( pContentMS )
		{
			if( m_bNormalizeFreqsIndiv )
				dMax = getMagnitudeMaximum();
			else
				dMax = pContentMS->getOverallMagnitudeMaximum();
		}
		else if( pContentMPS )
		{
			if( m_bNormalizeFreqsIndiv )
				dMax = getMagnitudeMaximum();
			else
				dMax = pContentMPS->getOverallMagnitudeMaximum();
		}

		for( int i = 0; i < m_pContent->getProperties()->getNumberOfRecords(); i++ )
		{
			// Get the magnitude value

			if( pContentDFT ) // ----------------
			{
				float fReal, fImag;
				pContentDFT->getDFTCoeff( i, m_iChannel, m_iFrequency, fReal, fImag );
				fMag = sqrt( fReal*fReal + fImag*fImag );
				if( fReal != 0.0 ) {
					fPhase = atan( fImag / fReal );
					if( fReal < 0.0 )
						if( fImag < 0.0 )
							fPhase -= DAFF::PI_F;
						else
							fPhase += DAFF::PI_F;
				}
				else
				{
					if( fImag < 0.0 )
						fPhase = -DAFF::HALF_PI_F;
					else
						fPhase = DAFF::HALF_PI_F;
				}

				// Check weather decibel scaling is activated
				if( m_iScaling == SCALING_DECIBEL )
				{
					assert( fAbsoluteMax != NULL );
					fMag = FactorToDecibel( fMag / fAbsoluteMax );

					float DECIBEL_LOWER;
					float DECIBEL_UPPER;

					// Decibel boundaries
					//float DECIBEL_DELTA = 30; // FIXME: should not be hard coded -> GUI
					if( m_bUseCustomRange )
					{
						DECIBEL_LOWER = FactorToDecibel( m_dMin );
						DECIBEL_UPPER = FactorToDecibel( m_dMax );
					}
					else
					{
						DECIBEL_LOWER = FactorToDecibel( 0.0 );
						DECIBEL_UPPER = FactorToDecibel( fAbsoluteMax );
					}

					// Limit the lower boundary
					fMag = std::max( fMag, DECIBEL_LOWER );
					fMag = std::min( fMag, DECIBEL_UPPER );

					// Normalize the range into the interval [0,1]
					fMag = 1 / ( DECIBEL_UPPER - DECIBEL_LOWER )*fMag + DECIBEL_LOWER / ( DECIBEL_LOWER - DECIBEL_UPPER );
				}
				else
				{
					if( m_bNormalize || m_bNormalizeFreqsIndiv )
					{
						assert( dMax != NULL && fMag <= dMax );
						// Normalize the range into the interval [0, 1]
						fMag = fMag / dMax;
					}
					else if( m_bUseCustomRange )
					{
						fMag = std::max( fMag, m_dMin );
						fMag = std::min( fMag, m_dMax );
						// Normalize the range into the interval [0,1]
						fMag = ( fMag - m_dMin ) / ( m_dMax - m_dMin );
					}
				}

			}
			else if( pContentMS ) // ----------------
			{
				pContentMS->getMagnitude( i, m_iChannel, m_iFrequency, fMag );

				if( m_bNormalize || m_bNormalizeFreqsIndiv )
				{
					assert( dMax != NULL && fMag <= dMax );
					// Normalize the range into the interval [0, 1]
					fMag = fMag / dMax;
				}
				else if( m_bUseCustomRange )
				{
					fMag = std::max( fMag, m_dMin );
					fMag = std::min( fMag, m_dMax );
					// Normalize the range into the interval [0,1]
					fMag = ( fMag - m_dMin ) / ( m_dMax - m_dMin );
				}
			}

			else if( pContentMPS ) {
				pContentMPS->getMagnitude( i, m_iChannel, m_iFrequency, fMag );
				pContentMPS->getPhase( i, m_iChannel, m_iFrequency, fPhase );

				if( m_bNormalize )
				{
					assert( dMax != NULL && fMag <= dMax );
					// Normalize the range into the interval [0, 1]
					fMag = fMag / dMax;
				}
				else if( m_bUseCustomRange )
				{
					fMag = std::max( fMag, m_dMin );
					fMag = std::min( fMag, m_dMax );
					// Normalize the range into the interval [0,1]
					fMag = ( fMag - m_dMin ) / ( m_dMax - m_dMin );
				}
			}

			// Store (inverted) value into scalars array
			pMagArray->InsertNextTuple1( 1.0 - fMag );
			pPhArray->InsertNextTuple1( fPhase );
		}

		// Assign scalars to points
		//m_pPlotPolydata->GetPointData()->SetScalars(pMagArray);
		m_pPlotPolydata->GetPointData()->AddArray( pMagArray );
		m_pPlotPolydata->GetPointData()->AddArray( pPhArray );

		UpdateProbe();
		return;
	}

	void BalloonPlot::sph2cart( double phi, double theta, double& x, double& y, double& z )
	{
		x = -sin( ( theta + 90 )*DAFF::PI_F / 180.0 ) * sin( phi*DAFF::PI_F / 180.0 );
		y = -cos( ( theta + 90 )*DAFF::PI_F / 180.0 );
		z = -sin( ( theta + 90 )*DAFF::PI_F / 180.0 ) * cos( phi*DAFF::PI_F / 180.0 );
	}

	float BalloonPlot::FactorToDecibel( float x ) const
	{
		assert( x >= 0 );

		// Lower boundary is -100 dB
		if( x <= 0.0000000001 )
			return -100.0f;

		return 10.0f * log( x ) / log( 10.0f );
	}

	float BalloonPlot::DecibelToFactor( float x ) const
	{
		return pow( 10.0, .1*x );
	}

	float BalloonPlot::getMagnitudeMaximum() const
	{
		assert( m_iFrequency >= 0 && m_iFrequency < m_iNumFrequencies );
		assert( m_pContent != NULL );
		assert( m_dMin < m_dMax );

		const DAFFContentDFT* pContentDFT = NULL;
		const DAFFContentMS* pContentMS = NULL;
		const DAFFContentMPS* pContentMPS = NULL;

		switch( m_pContent->getProperties()->getContentType() )
		{
		case DAFF_DFT_SPECTRUM:
			pContentDFT = dynamic_cast< const DAFFContentDFT* >( m_pContent );
			break;

		case DAFF_MAGNITUDE_SPECTRUM:
			pContentMS = dynamic_cast< const DAFFContentMS* >( m_pContent );
			break;

		case DAFF_MAGNITUDE_PHASE_SPECTRUM:
			pContentMPS = dynamic_cast< const DAFFContentMPS* >( m_pContent );
			break;
		}

		float fMax = std::numeric_limits<float>::min();
		float fMag;
		if( pContentDFT )
		{
			float fReal, fImag;
			for( int i = 0; i < pContentDFT->getNumDFTCoeffs(); i++ )
			{
				pContentDFT->getDFTCoeff( i, m_iChannel, m_iFrequency, fReal, fImag );
				fMag = std::sqrtf( std::powf( fReal, 2 ) + std::powf( fImag, 2 ) );
				if( fMag > fMax )
					fMax = fMag;
			}
		}
		else if( pContentMS )
		{
			for( int i = 0; i < pContentMS->getProperties()->getNumberOfRecords(); i++ )
			{
				pContentMS->getMagnitude( i, m_iChannel, m_iFrequency, fMag );
				if( fMag > fMax )
					fMax = fMag;
			}
		}
		else if( pContentMPS )
		{
			for( int i = 0; i < pContentMPS->getProperties()->getNumberOfRecords(); i++ )
			{
				pContentMPS->getMagnitude( i, m_iChannel, m_iFrequency, fMag );
				if( fMag > fMax )
					fMax = fMag;
			}
		}

		return fMax;
	}

	void BalloonPlot::SetScalarVisibility( bool bVisible )
	{
		m_pMapper->SetScalarVisibility( bVisible );
	}

	int BalloonPlot::GetScalarVisibility()
	{
		return m_pMapper->GetScalarVisibility();
	}

	void BalloonPlot::SetDisplayMode( int iMode )
	{
		if( iMode == MODE_SURFACE )
			m_pPlotActor->GetProperty()->SetRepresentationToSurface();
		else if( iMode == MODE_WIREFRAME )
			m_pPlotActor->GetProperty()->SetRepresentationToWireframe();
		else if( iMode == MODE_POINT )
			m_pPlotActor->GetProperty()->SetRepresentationToPoints();
	}

	void BalloonPlot::SetChannel( int iChannel )
	{
		m_iChannel = iChannel;
		SetScalars();
	}

	int BalloonPlot::GetChannel()
	{
		return m_iChannel;
	}

	void BalloonPlot::SetUsePhaseAsColor( bool bUse )
	{
		if( bUse )
		{
			m_pMapper->SelectColorArray( "phases" );
			vtkSmartPointer< vtkColorTransferFunction > colors = vtkSmartPointer< vtkColorTransferFunction >::New();
			colors->AddRGBPoint( -DAFF::PI_F, 0, 0, 1 );
			colors->AddRGBPoint( 0, 1, 1, 1 );
			colors->AddRGBPoint( DAFF::PI_F, 1, 0, 0 );
			m_pMapper->SetLookupTable( colors );
		}
		else
		{
			m_pMapper->SelectColorArray( "magnitudes" );
			m_pMapper->SetLookupTable( 0 );
		}
		m_bUsePhaseAsColor = bUse;
	}

} // End of namespace "DAFFViz"
