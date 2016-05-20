#include <daffviz/DAFFVizGrid.h>
#include <daffviz/DAFFVizGlobalLock.h>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkDataSetMapper.h>
#include <vtkLine.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPolygon.h>
#include <vtkUnstructuredGrid.h>

namespace DAFFViz
{

	Grid::Grid(unsigned int iCellsX, unsigned int iCellsZ)
	: SGNode(), m_iCellsX(iCellsX), m_iCellsZ(iCellsZ)
	{
		if (m_iCellsX == 0)
			m_iCellsX = 10;

		if (m_iCellsZ == 0)
			m_iCellsZ = 10;

		init();
	}

	Grid::Grid(DAFFViz::SGNode* pParentNode, unsigned int iCellsX, unsigned int iCellsZ)
	: SGNode(pParentNode), m_iCellsX(iCellsX), m_iCellsZ(iCellsZ)
	{
		if (m_iCellsX == 0)
			m_iCellsX = 10;

		if (m_iCellsZ == 0)
			m_iCellsZ = 10;

		init();
	}

	Grid::~Grid()
	{
		RemoveActor( m_pActorGrid );
	}

	void Grid::init()
	{
		// Points
		vtkSmartPointer< vtkPoints > pGridPoints = vtkSmartPointer< vtkPoints >::New();

		// Grids
		vtkSmartPointer< vtkUnstructuredGrid > pGrid = vtkSmartPointer< vtkUnstructuredGrid >::New();

		pGrid->SetPoints( pGridPoints );
		pGrid->Allocate( m_iCellsX + m_iCellsZ + 2, 1 );
	
		// Helper
		vtkSmartPointer< vtkLine > pLine = vtkSmartPointer< vtkLine >::New();

		// Create grid
		int k = 0;
		int subk = 0;

		// X direction lines
		for( int i = 0; i <= m_iCellsX; i++ )
		{
			pGridPoints->InsertPoint(k++, i/(double) m_iCellsX, 0, 0);
			pGridPoints->InsertPoint(k++, i/(double) m_iCellsX, 0, 1);
			pLine->GetPointIds()->SetId(0, k-2);
			pLine->GetPointIds()->SetId(1, k-1);
			pGrid->InsertNextCell( pLine->GetCellType(), pLine->GetPointIds() );
		}

		// Z direction lines
		for( int i = 0; i <= m_iCellsZ; i++ )
		{
			pGridPoints->InsertPoint(k++, 0, 0, i/(double) m_iCellsZ);
			pGridPoints->InsertPoint(k++, 1, 0, i/(double) m_iCellsZ);
			pLine->GetPointIds()->SetId(0, k-2);
			pLine->GetPointIds()->SetId(1, k-1);
			pGrid->InsertNextCell( pLine->GetCellType(), pLine->GetPointIds() );
		}

		// Grid actor
		vtkSmartPointer< vtkDataSetMapper > pGridMapper = vtkSmartPointer< vtkDataSetMapper >::New();
		pGridMapper->SetInputData( pGrid );

		m_pActorGrid = vtkSmartPointer< vtkActor >::New();
		m_pActorGrid->SetMapper( pGridMapper );

		DAFFVIZ_LOCK_VTK;
		m_pActorGrid->GetProperty()->SetAmbient(1);
		m_pActorGrid->GetProperty()->SetDiffuse(0);
		m_pActorGrid->GetProperty()->SetSpecular(0);
		double dGridColor = 0.4;
		m_pActorGrid->GetProperty()->SetColor( dGridColor, dGridColor, dGridColor );
		m_pActorGrid->GetProperty()->SetOpacity( 0.8 );
		DAFFVIZ_UNLOCK_VTK;

		AddActor( m_pActorGrid );
	}

	bool Grid::IsVisible() const
	{
		return m_pActorGrid->GetVisibility() ? true : false;
	}

	void Grid::SetVisible( bool bVisible )
	{
		SGNode::SetVisible(bVisible);

		DAFFVIZ_LOCK_VTK;
		bVisible ? m_pActorGrid->VisibilityOn() : m_pActorGrid->VisibilityOff();
		DAFFVIZ_UNLOCK_VTK;
	}

} // End of namespace "DAFFViz"
