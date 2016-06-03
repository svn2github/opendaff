#ifndef QDAFFVTKWIDGET_H
#define QDAFFVTKWIDGET_H

#include <QVTKWidget.h>
#include <DAFF.h>
#include <DAFFViz/DAFFViz.h>
#include <DAFFViz/DAFFVizSGNode.h>

#include <vtkAssembly.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

#include <iostream>

class QDAFFVTKWidget : public QVTKWidget
{
public:
	inline QDAFFVTKWidget( QWidget *parent = Q_NULLPTR )
		: QVTKWidget( parent )
		, m_pRenderer( NULL )
		, m_pSGRootNode( NULL )
		, m_pSCA( NULL )
    {

        /* Sphere
        vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->Update();
        vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
        vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
        sphereActor->SetMapper(sphereMapper);
*/

		m_pSGRootNode = new DAFFViz::SGNode();
		m_pSCA = new DAFFViz::SphericalCoordinateAssistant();
		m_pSGRootNode->AddChildNode( m_pSCA );

		m_pRenderer = vtkSmartPointer< vtkRenderer >::New();
		m_pRenderer->AddActor( m_pSGRootNode->GetNodeAssembly() );

		GetRenderWindow()->AddRenderer( m_pRenderer );
    }

	inline ~QDAFFVTKWidget()
	{
		m_pRenderer->RemoveActor( m_pSGRootNode->GetNodeAssembly() );
		m_pSGRootNode->RemoveChildNode( m_pSCA );
		delete m_pSGRootNode;
		delete m_pSCA;
	}

private:
    vtkSmartPointer< vtkRenderer > m_pRenderer;
	DAFFViz::SGNode* m_pSGRootNode;
	DAFFViz::SphericalCoordinateAssistant* m_pSCA;
public slots:
    inline void on_readDAFF( const DAFFReader* pReader )
    {
        std::cout << pReader->toString() << std::endl;

    }
};

#endif // QDAFFVTKWIDGET_H
