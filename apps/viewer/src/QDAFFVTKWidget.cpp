#include "QDAFFVTKWidget.h"

#include <QKeyEvent>
#include <QFileInfo>
#include <QDir>

#include <vtkAssembly.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkPNGWriter.h>
#include <vtkAVIWriter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkInteractorStyleTerrain.h>
#include <vtkImageMagnify.h>

#include <iostream>
#include <sstream>


QDAFFVTKWidget::QDAFFVTKWidget( QWidget *parent )
	: QVTKWidget( parent )
	, m_pRenderer( NULL )
	, m_pSGRootNode( NULL )
	, m_pSCA( NULL )
	, m_pDAFFContentBalloon( NULL )
	, m_pCCA( NULL )
	, m_pDAFFContentCarpet( NULL )
	, m_pSDI( NULL )
	, m_bBalloonPhaseColor( false )
	, m_bCarpetWarp( true )
	, m_iCarpetScaling( DAFFViz::CarpetPlot::SCALING_LINEAR )
	, m_iBalloonScaling( DAFFViz::BalloonPlot::SCALING_DECIBEL )
{
	m_pSGRootNode = new DAFFViz::SGNode();
	m_pSCA = new DAFFViz::SphericalCoordinateAssistant( m_pSGRootNode );
	m_pCCA = new DAFFViz::CartesianCoordinateAssistant( 2.0f, 1.0f, 2.0f );
	m_pCCA->SetPosition( -1.0f, 0.0f, -1.0f );

	m_pSDI = new DAFFViz::SphericalDirectionIndicator( m_pSGRootNode );
	m_pSDI->SetVisible( true );

	m_pRenderer = vtkSmartPointer< vtkRenderer >::New();
	m_pRenderer->AddActor( m_pSGRootNode->GetNodeAssembly() );

	m_pSGRootNode->OnSetFollowerCamera( m_pRenderer->GetActiveCamera() );
	m_pRenderer->GetActiveCamera()->SetPosition( 3, 3, -3 );
		
	GetRenderWindow()->AddRenderer( m_pRenderer );

	vtkSmartPointer< vtkInteractorStyleTerrain > pCustomInteractorStyle = vtkSmartPointer< vtkInteractorStyleTerrain >::New();
	GetRenderWindow()->GetInteractor()->SetInteractorStyle( pCustomInteractorStyle );
}

QDAFFVTKWidget::~QDAFFVTKWidget()
{
	if( m_pDAFFContentBalloon )
		delete m_pDAFFContentBalloon;
	if( m_pSCA )
		delete m_pSCA;
	if( m_pDAFFContentCarpet )
		delete m_pDAFFContentCarpet;
	if( m_pCCA )
		delete m_pCCA;

	m_pRenderer->RemoveActor( m_pSGRootNode->GetNodeAssembly() );
	delete m_pSGRootNode;
}

void QDAFFVTKWidget::CloseDAFF()
{
	// Clear current balloon node
	if( m_pDAFFContentBalloon )
	{
		m_pSGRootNode->RemoveChildNode( m_pDAFFContentBalloon );
		delete m_pDAFFContentBalloon;
		m_pDAFFContentBalloon = NULL;

		m_pSGRootNode->RemoveChildNode( m_pSCA );
	}

	// Clear current carpet plot
	if( m_pDAFFContentCarpet )
	{
		m_pSGRootNode->RemoveChildNode( m_pDAFFContentCarpet );
		delete m_pDAFFContentCarpet;
		m_pDAFFContentCarpet = NULL;

		m_pSGRootNode->RemoveChildNode( m_pCCA );
	}

	// Restore default settings
	m_pSGRootNode->AddChildNode( m_pSCA );

	update();
}

void QDAFFVTKWidget::ReadDAFF( const DAFFReader* pReader )
{
	// Clear current balloon node
	if( m_pDAFFContentBalloon )
	{
		m_pSGRootNode->RemoveChildNode( m_pDAFFContentBalloon );
		delete m_pDAFFContentBalloon;
		m_pDAFFContentBalloon = NULL;
	}

	// Clear current carpet plot
	if( m_pDAFFContentCarpet )
	{
		m_pSGRootNode->RemoveChildNode( m_pDAFFContentCarpet );
		delete m_pDAFFContentCarpet;
		m_pDAFFContentCarpet = NULL;
	}

	if( pReader == nullptr )
		return;

	if( m_pSCA->HasParentNode() )
		m_pSGRootNode->RemoveChildNode( m_pSCA );

	if( m_pCCA->HasParentNode() )
		m_pSGRootNode->RemoveChildNode( m_pCCA );

	switch( pReader->getContentType() )
	{
	case DAFF_IMPULSE_RESPONSE:
	{
		DAFFContentIR* pContentIR = static_cast< DAFFContentIR* >( pReader->getContent() );
		m_pDAFFContentCarpet = new DAFFViz::CarpetPlot( m_pSGRootNode, pContentIR );
		m_pDAFFContentCarpet->SetScaling( m_iCarpetScaling );
		m_pDAFFContentCarpet->SetWarpingEnabled( m_bCarpetWarp );

		m_pSDI->SetVisible( false );

		// Configure cartesian coordinate helper
		m_pCCA->SetMinX( pContentIR->getProperties()->getAlphaStart() );
		m_pCCA->SetMaxX( pContentIR->getProperties()->getAlphaEnd() );
		m_pCCA->SetResolutionX( pContentIR->getProperties()->getAlphaSpan() / 11.0f );

		m_pCCA->SetMinY( -pContentIR->getOverallPeak() );
		m_pCCA->SetResolutionY( 0.25f );
		m_pCCA->SetMaxY( pContentIR->getOverallPeak() );
		m_pCCA->SetOffsetY( 0.0f );

		m_pCCA->SetMinZ( 0.0f );
		m_pCCA->SetResolutionZ( 10 );
		m_pCCA->SetMaxZ( pContentIR->getSamplerate() / double( pContentIR->getFilterLength() ) );

		m_pCCA->UpdateAxes();

		m_pSGRootNode->AddChildNode( m_pCCA );
		break;
	}
	case DAFF_DFT_SPECTRUM:
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
	case DAFF_MAGNITUDE_SPECTRUM:
	case DAFF_PHASE_SPECTRUM:
		m_pDAFFContentBalloon = new DAFFViz::BalloonPlot( m_pSGRootNode, pReader->getContent() );
		m_pDAFFContentBalloon->SetScaling( m_iBalloonScaling );
		m_pDAFFContentBalloon->SetUsePhaseAsColor( m_bBalloonPhaseColor );
		m_pDAFFContentBalloon->SetNormalize(m_bNormalize);
		m_pDAFFContentBalloon->SetNormalizeFrequenciesIndividually(m_bNormalizeFreqsIndiv);
		
		m_pSGRootNode->AddChildNode( m_pSCA );

		break;
	}

	update();
}

void QDAFFVTKWidget::ChangeFrequencyIndex( int iFrequencyIndex )
{
	if( m_pDAFFContentBalloon )
		m_pDAFFContentBalloon->SetSelectedFrequency( iFrequencyIndex );

	update();
}

void QDAFFVTKWidget::ChangeChannelIndex( int iChannelIndex )
{
	if( m_pDAFFContentBalloon )
		m_pDAFFContentBalloon->SetChannel( iChannelIndex );

	if( m_pDAFFContentCarpet )
		m_pDAFFContentCarpet->SetChannel( iChannelIndex );

	update();
}

void QDAFFVTKWidget::ChangeAlpha( double dAlphaDeg )
{
	if( m_pDAFFContentCarpet )
		if( m_pDAFFContentCarpet->getFixedAngle() == DAFFViz::CarpetPlot::ALPHA_FIXED )
			m_pDAFFContentCarpet->SetSelectedAngle( dAlphaDeg );

	update();
}

void QDAFFVTKWidget::ChangeBeta( double dBetaDeg )
{
	if( m_pDAFFContentCarpet )
		if( m_pDAFFContentCarpet->getFixedAngle() == DAFFViz::CarpetPlot::BETA_FIXED )
			m_pDAFFContentCarpet->SetSelectedAngle( dBetaDeg );

	update();
}

void QDAFFVTKWidget::ChangePhi( double dPhiDeg )
{
	if( m_pSDI )
		m_pSDI->SetDirectionPhiDeg( dPhiDeg );
	update();
}

void QDAFFVTKWidget::ChangeTheta( double dThetaDeg )
{
	if( m_pSDI )
		m_pSDI->SetDirectionThetaDeg( dThetaDeg );
	update();
}

void QDAFFVTKWidget::ExportScreenshotPNG( QString sFilePath, int iWidth, int iHeight )
{
	vtkSmartPointer< vtkRenderer > pImageRenderer = vtkSmartPointer< vtkRenderer >::New();
	pImageRenderer->AddActor( m_pSGRootNode->GetNodeAssembly() );
	pImageRenderer->SetActiveCamera( m_pRenderer->GetActiveCamera() );

	vtkSmartPointer< vtkRenderWindow > pImageRenderWin = vtkSmartPointer< vtkRenderWindow >::New();
	pImageRenderWin->SetSize( iWidth, iHeight );
	pImageRenderWin->AddRenderer( pImageRenderer );
	pImageRenderWin->Render();

	vtkSmartPointer< vtkWindowToImageFilter > pFilter = vtkSmartPointer< vtkWindowToImageFilter >::New();
	pFilter->SetInput( pImageRenderWin );
	pFilter->SetInputBufferTypeToRGBA();
	pFilter->ReadFrontBufferOff();
	pFilter->Update();

	vtkSmartPointer< vtkPNGWriter > pExportPNG = vtkSmartPointer< vtkPNGWriter >::New();
	pExportPNG->SetFileName( sFilePath.toStdString().c_str() );
	pExportPNG->SetInputConnection( pFilter->GetOutputPort() );
    pExportPNG->Write();
}

void QDAFFVTKWidget::ExportScrenshotSeriesPNG( QString sFileBasePath, QString sFileBaseName, const CAnimation& oA )
{
	if( oA.iNumFrames < 0 )
		return;

	vtkSmartPointer<vtkRenderer> pImageRenderer = vtkSmartPointer<vtkRenderer>::New();
	pImageRenderer->AddActor( m_pSGRootNode->GetNodeAssembly() );
	pImageRenderer->SetActiveCamera( m_pRenderer->GetActiveCamera() );

	vtkSmartPointer<vtkRenderWindow> pImageRenderWin = vtkSmartPointer<vtkRenderWindow>::New();
	pImageRenderWin->SetWindowName( sFileBaseName.toStdString().c_str() );
	pImageRenderWin->SetSize( oA.iWidth, oA.iHeight );
	pImageRenderWin->AddRenderer( pImageRenderer );
	pImageRenderWin->Render();
			
	vtkSmartPointer<vtkWindowToImageFilter> pFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	pFilter->SetInputBufferTypeToRGBA();
	pFilter->ReadFrontBufferOff();
	pFilter->SetInput( pImageRenderWin );

	vtkSmartPointer< vtkPNGWriter > pExportPNG = vtkSmartPointer< vtkPNGWriter >::New();
	pExportPNG->SetInputConnection( pFilter->GetOutputPort() );

	// Initial vals
	double dInitYaw, dInitPitch, dInitRoll;
	m_pSGRootNode->GetOrientationYPR( dInitYaw, dInitPitch, dInitRoll );

	// Increments
	double dYawRes = ( oA.dYawEnd - oA.dYawStart ) / double( oA.iNumFrames );
	double dPitchRes = ( oA.dPitchEnd - oA.dPitchStart ) / double( oA.iNumFrames );
	double dEleRes = ( oA.dEleEnd - oA.dEleStart ) / double( oA.iNumFrames );

	for( int i = 0; i < oA.iNumFrames; i++ )
	{
		// Manipulate plot
		double dYaw = oA.bYaw ? oA.dYawStart + i * dYawRes : 0.0f;
		double dPitch = oA.bPitch ? oA.dPitchStart + i* dPitchRes : 0.0f;
		m_pSGRootNode->SetOrientationYPR( dYaw, dPitch, 0.0f );
		if( oA.bElevation && m_pDAFFContentCarpet )
		{
			m_pDAFFContentCarpet->SetSelectedAngle( oA.dEleStart + i * dEleRes );
		}

		if( oA.bChannels )
		{
			int iIdx = oA.iChannelIdxStart + int( std::round( ( oA.iChannelIdxEnd - oA.iChannelIdxStart )  * double( i ) / double( oA.iNumFrames ) ) );
			ChangeChannelIndex( iIdx );
		}

		if( oA.bFrequencies )
		{
			int iIdx = oA.iFreqIdxStart + int( std::round( ( oA.iFreqIdxEnd - oA.iFreqIdxStart )  * double( i ) / double( oA.iNumFrames ) ) );
			ChangeFrequencyIndex( iIdx );
		}

		// Render
		pImageRenderWin->Render();

		// This is required to update the filter, otherwise you have a still image
		pFilter->Modified();

		// Export render image
		std::stringstream ss;
		ss << sFileBaseName.toStdString() << "_f" << std::setfill( '0' ) << std::setw( 4 ) << i << ".png";

		QDir oDir( sFileBasePath );
		QFileInfo oFile( oDir, ss.str().c_str() );

		pExportPNG->SetFileName( oFile.absoluteFilePath().toStdString().c_str() );
		pExportPNG->Write();
	}

	m_pSGRootNode->SetOrientationYPR( 0,0,0 );
}

void QDAFFVTKWidget::SetCoordinateAssistanceVisible( bool bVisible )
{
	if( m_pSCA )
		m_pSCA->SetAssistanceVisible( bVisible );

	if( m_pCCA )
		m_pCCA->SetAxesVisible( bVisible );

	update();
}

void QDAFFVTKWidget::SetDirectionIndicatorVisible( bool bVisible )
{
	if( m_pSDI )
		m_pSDI->SetVisible(bVisible);

	update();
}

void QDAFFVTKWidget::SetArrowsVisible( bool bVisible )
{
	if( m_pSCA )
		m_pSCA->SetViewUpVectorsVisible( bVisible );

	update();
}


void QDAFFVTKWidget::SetPolesVisible( bool bVisible )
{
	if( m_pSCA )
		m_pSCA->SetPolesVisible( bVisible );

	update();
}

void QDAFFVTKWidget::SetGridVisible( bool  bVisible )
{
	if( m_pSCA )
		m_pSCA->SetGridVisible( bVisible );

	update();
}

void QDAFFVTKWidget::SetEquatorVisible( bool bVisible )
{
	if( m_pSCA )
		m_pSCA->SetEquatorVisible( bVisible );

	update();
}

void QDAFFVTKWidget::SetMeridiansVisible( bool bVisible )
{
	if( m_pSCA )
		m_pSCA->SetMeridiansVisible( bVisible );

	update();
}

void QDAFFVTKWidget::SetPhaseColorMap( bool bEnabled )
{
	m_bBalloonPhaseColor = bEnabled;
	if( m_pDAFFContentBalloon )
		m_pDAFFContentBalloon->SetUsePhaseAsColor( bEnabled );

	update();
}

void QDAFFVTKWidget::SetCarpetPlotWarp( bool bEnabled )
{
	m_bCarpetWarp = bEnabled;
	if( m_pDAFFContentCarpet )
		m_pDAFFContentCarpet->SetWarpingEnabled( m_bCarpetWarp );

	update();
}

void QDAFFVTKWidget::SetLogScale( bool bEnabled )
{
	m_iBalloonScaling = bEnabled ? DAFFViz::BalloonPlot::SCALING_DECIBEL : DAFFViz::BalloonPlot::SCALING_LINEAR;
	if( m_pDAFFContentBalloon )
	{
		m_pDAFFContentBalloon->SetScaling( m_iBalloonScaling );
	}

	m_iCarpetScaling = bEnabled ? DAFFViz::CarpetPlot::SCALING_DECIBEL : DAFFViz::CarpetPlot::SCALING_LINEAR;
	if( m_pDAFFContentCarpet )
	{
		m_pDAFFContentCarpet->SetScaling( m_iCarpetScaling );
	}

	update();
}

void QDAFFVTKWidget::SetNormalizeFrequenciesIndividually( bool bEnabled )
{
	m_bNormalizeFreqsIndiv = bEnabled;
	if (m_pDAFFContentBalloon)
	{
		m_pDAFFContentBalloon->SetNormalizeFrequenciesIndividually( bEnabled );
	}

	update();
}

void QDAFFVTKWidget::SetNormalize( bool bEnabled )
{
	m_bNormalize = bEnabled;
	if (m_pDAFFContentBalloon)
	{
		m_pDAFFContentBalloon->SetNormalize( bEnabled );
	}

	update();
}
