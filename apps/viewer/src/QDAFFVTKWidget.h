/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef QDAFFVTKWIDGET_H
#define QDAFFVTKWIDGET_H

#include <QObject>
#include <QVTKWidget.h>
#include <QKeyEvent>

#include <DAFF.h>
#include <DAFFViz/DAFFViz.h>

#include <vtkAssembly.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkCamera.h>

#include <iostream>

class QDAFFVTKWidget : public QVTKWidget
{
    Q_OBJECT

public:
	inline QDAFFVTKWidget( QWidget *parent = Q_NULLPTR )
		: QVTKWidget( parent )
		, m_pRenderer( NULL )
		, m_pSGRootNode( NULL )
		, m_pSCA( NULL )
		, m_pDAFFContentBalloon( NULL )
		, m_pCCA( NULL )
		, m_pDAFFContentCarpet( NULL )
    {
		m_pSGRootNode = new DAFFViz::SGNode();
		m_pSCA = new DAFFViz::SphericalCoordinateAssistant( m_pSGRootNode );
		m_pCCA = new DAFFViz::CartesianCoordinateAssistant(  ); // causes dark VTK widget if added to SGNode ... why?

		m_pRenderer = vtkSmartPointer< vtkRenderer >::New();
		m_pRenderer->AddActor( m_pSGRootNode->GetNodeAssembly() );

		m_pSGRootNode->OnSetFollowerCamera( m_pRenderer->GetActiveCamera() );
		m_pRenderer->GetActiveCamera()->SetPosition( 3, 3, -3 );
		
		GetRenderWindow()->AddRenderer( m_pRenderer );

		GetRenderWindow()->GetInteractor();
    }

	inline ~QDAFFVTKWidget()
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

private:
    vtkSmartPointer< vtkRenderer > m_pRenderer;
	DAFFViz::SGNode* m_pSGRootNode;
	DAFFViz::SphericalCoordinateAssistant* m_pSCA;
	DAFFViz::BalloonPlot* m_pDAFFContentBalloon;
	DAFFViz::CartesianCoordinateAssistant* m_pCCA;
	DAFFViz::CarpetPlot* m_pDAFFContentCarpet;

public slots:

inline void on_closeDAFF()
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

	m_pCCA->SetVisible( false );
	m_pSCA->SetVisible( true );

	update();
}

    inline void on_readDAFF( const DAFFReader* pReader )
	{
		// Clear current balloon node
		if( m_pDAFFContentBalloon )
		{
			m_pSGRootNode->RemoveChildNode( m_pDAFFContentBalloon );
			delete m_pDAFFContentBalloon;
			m_pDAFFContentBalloon = NULL;
			m_pSCA->SetVisible( false );
		}

		// Clear current carpet plot
		if( m_pDAFFContentCarpet )
		{
			m_pSGRootNode->RemoveChildNode( m_pDAFFContentCarpet );
			delete m_pDAFFContentCarpet;
			m_pDAFFContentCarpet = NULL;
			m_pCCA->SetVisible( false );
			m_pSCA->SetVisible( true ); // default
		}

		if( pReader == nullptr )
			return;

		switch( pReader->getContentType() )
		{
		case DAFF_IMPULSE_RESPONSE:
		{
			DAFFContentIR* pContentIR = static_cast< DAFFContentIR* >( pReader->getContent() );
			m_pDAFFContentCarpet = new DAFFViz::CarpetPlot( m_pSGRootNode, pContentIR );
			m_pSCA->SetVisible( false );
			m_pCCA->SetVisible( true );
			break;
		}
		case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		case DAFF_MAGNITUDE_SPECTRUM:
		case DAFF_DFT_SPECTRUM:
		case DAFF_PHASE_SPECTRUM:
			m_pDAFFContentBalloon = new DAFFViz::BalloonPlot( m_pSGRootNode, pReader->getContent() );
			m_pSCA->SetVisible( true );
			break;
		}

		update();
	}

	inline void on_changeFrequencyIndex( int iFrequencyIndex )
	{
		if( m_pDAFFContentBalloon )
		{
			m_pDAFFContentBalloon->SetSelectedFrequency( iFrequencyIndex );
		}

		update();
	}

	inline void on_changeChannelIndex( int iChannelIndex )
	{
		if( m_pDAFFContentBalloon )
		{
			m_pDAFFContentBalloon->SetChannel( iChannelIndex );
		}

		if( m_pDAFFContentCarpet )
		{
			m_pDAFFContentCarpet->SetChannel( iChannelIndex );
		}

		update();
	}
};

#endif // QDAFFVTKWIDGET_H
