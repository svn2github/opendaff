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
	, m_bBalloonPlotPhaseColor( false )
{
	m_pSGRootNode = new DAFFViz::SGNode();
	m_pSCA = new DAFFViz::SphericalCoordinateAssistant( m_pSGRootNode );
	m_pCCA = new DAFFViz::CartesianCoordinateAssistant(); // causes dark VTK widget if added to SGNode ... why?

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
		m_pDAFFContentCarpet->SetScaling( DAFFViz::CarpetPlot::SCALING_DECIBEL );
		m_pSGRootNode->AddChildNode( m_pCCA );
		SetDirectionIndicatorVisible(false);
		break;
	}
	case DAFF_DFT_SPECTRUM:
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
	case DAFF_MAGNITUDE_SPECTRUM:
	case DAFF_PHASE_SPECTRUM:
		m_pDAFFContentBalloon = new DAFFViz::BalloonPlot( m_pSGRootNode, pReader->getContent() );
		m_pDAFFContentBalloon->SetScaling( DAFFViz::CarpetPlot::SCALING_LINEAR );
		m_pDAFFContentBalloon->SetUsePhaseAsColor( m_bBalloonPlotPhaseColor );
		
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

void QDAFFVTKWidget::ExportScrenshotSeriesPNG( QString sFileBasePath, QString sFileBaseName, int iNumFrames, int iWidth, int iHeight )
{
	if( iNumFrames < 0 )
		return;

	vtkSmartPointer<vtkRenderer> pImageRenderer = vtkSmartPointer<vtkRenderer>::New();
	pImageRenderer->AddActor( m_pSGRootNode->GetNodeAssembly() );
	pImageRenderer->SetActiveCamera( m_pRenderer->GetActiveCamera() );

	vtkSmartPointer<vtkRenderWindow> pImageRenderWin = vtkSmartPointer<vtkRenderWindow>::New();
	pImageRenderWin->SetWindowName( sFileBaseName.toStdString().c_str() );
	pImageRenderWin->SetSize( iWidth, iHeight );
	pImageRenderWin->AddRenderer( pImageRenderer );
	pImageRenderWin->Render();
			
	vtkSmartPointer<vtkWindowToImageFilter> pFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	pFilter->SetInputBufferTypeToRGBA();
	pFilter->ReadFrontBufferOff();
	pFilter->SetInput( pImageRenderWin );

	vtkSmartPointer< vtkPNGWriter > pExportPNG = vtkSmartPointer< vtkPNGWriter >::New();
	pExportPNG->SetInputConnection( pFilter->GetOutputPort() );

	double dAngularResolutionDegree = 360.0f / double( iNumFrames );
	for( int i = 0; i < iNumFrames; i++ )
	{
		m_pSGRootNode->SetOrientationYPR( i * dAngularResolutionDegree, 0.0f, 0.0f );
		pImageRenderWin->Render();

		// This is required to update the filter, otherwise you have a still image
		pFilter->Modified();

		std::stringstream ss;
		ss << sFileBaseName.toStdString() << "_f" << std::setfill( '0' ) << std::setw( 4 ) << i << ".png";

		QDir oDir( sFileBasePath );
		QFileInfo oFile( oDir, ss.str().c_str() );

		pExportPNG->SetFileName( oFile.absoluteFilePath().toStdString().c_str() );
		pExportPNG->Write();
	}
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
	if (m_pSDI)
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
	m_bBalloonPlotPhaseColor = bEnabled;
	if( m_pDAFFContentBalloon )
		m_pDAFFContentBalloon->SetUsePhaseAsColor( bEnabled );

	update();
}
