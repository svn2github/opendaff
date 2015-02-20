#include <FXVTK2Grid.h>
#include <FXVTK2GlobalLock.h>

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

namespace FXVTK2 {

Grid::Grid(unsigned int iCellsX, unsigned int iCellsZ)
: SGNode(), m_iCellsX(iCellsX), m_iCellsZ(iCellsZ)
{
	if (m_iCellsX == 0)
		m_iCellsX = 10;

	if (m_iCellsZ == 0)
		m_iCellsZ = 10;

	init();
}

Grid::Grid(FXVTK2::SGNode* pParentNode, unsigned int iCellsX, unsigned int iCellsZ)
: SGNode(pParentNode), m_iCellsX(iCellsX), m_iCellsZ(iCellsZ)
{
	if (m_iCellsX == 0)
		m_iCellsX = 10;

	if (m_iCellsZ == 0)
		m_iCellsZ = 10;

	init();
}

Grid::~Grid() {
	RemoveActor(m_pActorGrid);

	m_pActorGrid->GlobalWarningDisplayOff();
	m_pActorGrid->Delete();
}

void Grid::init() {
	// Points
	vtkPoints* pGridPoints = vtkPoints::New();

	// Grids
	vtkUnstructuredGrid* pGrid = vtkUnstructuredGrid::New();

	pGrid->SetPoints(pGridPoints);
	pGrid->Allocate(m_iCellsX + m_iCellsZ + 2, 1);
	
	// Helper
	vtkLine* pLine = vtkLine::New(); 

	// Create grid
	int k = 0;
	int subk = 0;

	// X direction lines
	for (int i=0; i<=m_iCellsX; i++) {
		pGridPoints->InsertPoint(k++, i/(double) m_iCellsX, 0, 0);
		pGridPoints->InsertPoint(k++, i/(double) m_iCellsX, 0, 1);
		pLine->GetPointIds()->SetId(0, k-2);
		pLine->GetPointIds()->SetId(1, k-1);
		pGrid->InsertNextCell(pLine->GetCellType(), pLine->GetPointIds());
	}

	// Z direction lines
	for (int i=0; i<=m_iCellsZ; i++) {
		pGridPoints->InsertPoint(k++, 0, 0, i/(double) m_iCellsZ);
		pGridPoints->InsertPoint(k++, 1, 0, i/(double) m_iCellsZ);
		pLine->GetPointIds()->SetId(0, k-2);
		pLine->GetPointIds()->SetId(1, k-1);
		pGrid->InsertNextCell(pLine->GetCellType(), pLine->GetPointIds());
	}

	pLine->Delete();
	
	// Grid actor
	vtkDataSetMapper* pGridMapper = vtkDataSetMapper::New();
	pGridMapper->SetInput(pGrid);

	m_pActorGrid = vtkActor::New();
	m_pActorGrid->SetMapper(pGridMapper);

	FXVTK2_LOCK_VTK;
	m_pActorGrid->GetProperty()->SetAmbient(1);
	m_pActorGrid->GetProperty()->SetDiffuse(0);
	m_pActorGrid->GetProperty()->SetSpecular(0);
	double dGridColor = 0.4;
	m_pActorGrid->GetProperty()->SetColor(dGridColor, dGridColor, dGridColor);
	m_pActorGrid->GetProperty()->SetOpacity(0.8);
	FXVTK2_UNLOCK_VTK;

	AddActor(m_pActorGrid);
	
	// Cleanup
	pGridMapper->Delete();
	pGrid->Delete();
	pGridPoints->Delete();

}

bool Grid::IsVisible() const {
	return m_pActorGrid->GetVisibility() ? true : false;
}

void Grid::SetVisible(bool bVisible) {
	SGNode::SetVisible(bVisible);

	FXVTK2_LOCK_VTK;
	bVisible ? m_pActorGrid->VisibilityOn() : m_pActorGrid->VisibilityOff();
	FXVTK2_UNLOCK_VTK;
}

} // End of namespace "FXVTK2"