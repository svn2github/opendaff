#ifndef QDAFFVTKFRAME_H
#define QDAFFVTKFRAME_H

#include <QFrame>
#include <QVTKWidget.h>
#include <DAFF.h>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <iostream>

class QDAFFVTKFrame : public QFrame
{
    Q_OBJECT

public:
	inline QDAFFVTKFrame( QWidget *parent = Q_NULLPTR )
        : QFrame(parent)
		, m_pRenderer( NULL )
    {

        /* Sphere
        vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->Update();
        vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
        vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
        sphereActor->SetMapper(sphereMapper);
*/
        m_pRenderer = vtkSmartPointer< vtkRenderer >::New();
        //pRenderer->AddActor(sphereActor);

        m_pVTKWidget = new QVTKWidget( this );
        m_pVTKWidget->GetRenderWindow()->AddRenderer( m_pRenderer );
    }

private:
    QVTKWidget* m_pVTKWidget;
    vtkSmartPointer< vtkRenderer > m_pRenderer;
public slots:
    inline void on_readDAFF( const DAFFReader* pReader )
    {
        std::cout << pReader->toString() << std::endl;

    }
};

#endif // QDAFFVTKFRAME_H
