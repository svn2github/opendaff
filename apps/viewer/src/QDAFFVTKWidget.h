#ifndef QDAFFVTKWIDGET_H
#define QDAFFVTKWIDGET_H

#include <QObject>
#include <QVTKWidget.h>

#include <DAFF.h>
#include <DAFFViz/DAFFViz.h>

#include <vtkAssembly.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

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
    {
		m_pSGRootNode = new DAFFViz::SGNode();
		m_pSCA = new DAFFViz::SphericalCoordinateAssistant( m_pSGRootNode );

		m_pRenderer = vtkSmartPointer< vtkRenderer >::New();
		m_pRenderer->AddActor( m_pSGRootNode->GetNodeAssembly() );

		GetRenderWindow()->AddRenderer( m_pRenderer );
    }

	inline ~QDAFFVTKWidget()
	{

		if( m_pDAFFContentBalloon )
			delete m_pDAFFContentBalloon;
		if( m_pSCA )
			delete m_pSCA;

		m_pRenderer->RemoveActor( m_pSGRootNode->GetNodeAssembly() );
		delete m_pSGRootNode;
	}

private:
    vtkSmartPointer< vtkRenderer > m_pRenderer;
	DAFFViz::SGNode* m_pSGRootNode;
	DAFFViz::SphericalCoordinateAssistant* m_pSCA;
	DAFFViz::BalloonPlot* m_pDAFFContentBalloon;

public slots:

    inline void on_readDAFF( const DAFFReader* pReader )
    {
        std::cout << "!" << std::endl;
		// Clear current balloon node
		if( m_pDAFFContentBalloon )
		{
			m_pSGRootNode->RemoveChildNode( m_pDAFFContentBalloon );
			delete m_pDAFFContentBalloon;
			m_pDAFFContentBalloon = NULL;
		}

		if( pReader == nullptr )
			return;

		m_pDAFFContentBalloon = new DAFFViz::BalloonPlot( m_pSGRootNode, pReader->getContent() );
    }
};

#endif // QDAFFVTKWIDGET_H
