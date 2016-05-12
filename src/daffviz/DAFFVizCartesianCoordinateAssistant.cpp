#include <daffviz/DAFFVizCartesianCoordinateAssistant.h>

#include <sstream>

#include <daffviz/DAFFVizGlobalLock.h>

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
#include <vtkPolygon.h>

#define PI_F 3.14159265358979323846263383
namespace DAFFViz {

CartesianCoordinateAssistant::CartesianCoordinateAssistant(SGNode* pParentNode, double LengthX, double LengthY, double LengthZ)
: SGNode(pParentNode),
m_bGridVisible(true),
m_bLabelsVisible(true),
m_bAxesVisible(true),
m_bReferenceVisible(false),
m_dMinX(0), 
m_dMaxX(1), 
m_dMinY(0), 
m_dMaxY(1), 
m_dMinZ(0), 
m_dMaxZ(1), 
m_dResolutionX(.1), 
m_dResolutionY(.1), 
m_dResolutionZ(.1), 
m_dLengthX(LengthX), 
m_dLengthY(LengthY), 
m_dLengthZ(LengthZ), 
m_dOffsetY(0)
{
	init();
}

CartesianCoordinateAssistant::CartesianCoordinateAssistant(double LengthX, double LengthY, double LengthZ)
: SGNode(),
m_bGridVisible(true),
m_bLabelsVisible(true),
m_bAxesVisible(true),
m_bReferenceVisible(false),
m_dMinX(0), 
m_dMaxX(1), 
m_dMinY(0), 
m_dMaxY(1), 
m_dMinZ(0), 
m_dMaxZ(1), 
m_dResolutionX(.1), 
m_dResolutionY(.1), 
m_dResolutionZ(.1), 
m_dLengthX(LengthX), 
m_dLengthY(LengthY), 
m_dLengthZ(LengthZ), 
m_dOffsetY(0)
{
	init();
}

CartesianCoordinateAssistant::~CartesianCoordinateAssistant() {
	RemoveActor(m_pReferenceActor);

	for(int i=0; i<(int)m_pGrid.size(); i++) {
		RemoveActor(m_pGrid.at(i));
	}
	m_pGrid.clear();

	for(int i=0; i<(int)m_pLabels.size(); i++) {
		RemoveActor(m_pLabels.at(i));
	}
	m_pLabels.clear();
}

void CartesianCoordinateAssistant::init() {

	// set up reference
	vtkPoints* points =
		vtkPoints::New();
	points->InsertNextPoint(0.0, 0.0, 0.0);
	points->InsertNextPoint(2.0, 0.0, 0.0);
	points->InsertNextPoint(2.0, 0.0, 2.0);
	points->InsertNextPoint(0.0, 0.0, 2.0);

	// Create the  polygon
	vtkPolygon* polygon = vtkPolygon::New();
	polygon->GetPointIds()->SetNumberOfIds(4); //make a quad
	polygon->GetPointIds()->SetId(0, 0);
	polygon->GetPointIds()->SetId(1, 1);
	polygon->GetPointIds()->SetId(2, 2);
	polygon->GetPointIds()->SetId(3, 3);

	// Add the polygon to a list of polygons
	vtkCellArray* polygons =
		vtkCellArray::New();
	polygons->InsertNextCell(polygon);

	// Create a PolyData
	vtkPolyData* polygonPolyData =
		vtkPolyData::New();
	polygonPolyData->SetPoints(points);
	polygonPolyData->SetPolys(polygons);

	// Create a mapper and actor
	vtkPolyDataMapper* mapper =
		vtkPolyDataMapper::New();
	mapper->SetInputData(polygonPolyData);

	m_dReferenceAlpha = 0;
	m_pReferenceActor = vtkActor::New();
	m_pReferenceActor->SetMapper(mapper);
	m_pReferenceActor->GetProperty()->SetOpacity(m_dReferenceAlpha);

	AddActor(m_pReferenceActor);

	UpdateGrid();
}

void CartesianCoordinateAssistant::UpdateAxes() {

	bool bWasVisible = GetAxesVisible(); 

	// TODO: handle case when not (ymin < 0 and ymax > 0)

	// X (yellow) and Y vector (white) and Z vector (red)
	for (int i=0; i<(int)m_pAxes.size(); i++)  {
		this->RemoveChildNode(m_pAxes[i]);
		delete m_pAxes[i];
	}
	m_pAxes.clear();

	//AddChildNode(m_pAxes);
	Arrow* axeX = new Arrow(this, 0.05, 0.03, 36, 0.01, 36);
	axeX->SetColor(1, 1, 0);
	axeX->SetPosition(m_dMinX/(m_dMaxX-m_dMinX),0,0);
	axeX->SetOrientation(0, 0, 0);
	axeX->SetScale(1.1* m_dLengthX,1.0,1.0);
	m_pAxes.push_back(axeX);

	Arrow* axeY = new Arrow(this, 0.05, 0.03, 36, 0.01, 36);
	axeY->SetColor(1, 1, 1);
	axeY->SetPosition(0,(m_dMinY-m_dOffsetY)/(m_dMaxY-m_dMinY),0);
	axeY->SetOrientationYPR(0, 0, -90);
	axeY->SetScale(1.1* m_dLengthY,1.0,1.0);
	m_pAxes.push_back(axeY);

	Arrow* axeZ = new Arrow(this, 0.05, 0.03, 36, 0.01, 36);
	axeZ->SetColor(1, 0, 0);
	axeZ->SetPosition(0,0,m_dMinZ/(m_dMaxZ-m_dMinZ));
	axeZ->SetOrientationYPR(-90,0 , 0);
	axeZ->SetScale(1.1* m_dLengthZ,1.0,1.0);
	m_pAxes.push_back(axeZ);

	if (!bWasVisible)
		SetAxesVisible(false);
	
}

void CartesianCoordinateAssistant::UpdateGrid() {

	//TODO: is it sufficient to delete the actor or do i have to delete polyline/cells/polydata as well?
	bool bWasVisible = GetGridVisible(); 
	for(int i=0; i<(int)m_pGrid.size(); i++) {
		RemoveActor(m_pGrid.at(i));
		m_pGrid.at(i)->GetMapper()->GetInput()->Delete();
	}
	m_pGrid.clear();
	for(int i=0; i<(int)m_pLabels.size(); i++) {
		RemoveActor(m_pLabels.at(i));
		//m_pLabels.at(i)->GetMapper()->GetInput()->Delete();
	}
	m_pLabels.clear();

	// create new grid
	vtkCellArray* cells;
	vtkPolyData* polydata;
	vtkActor* actor;
	vtkPolyDataMapper* mapper;
	vtkLine* line;
	vtkVectorText* pLabel;

	// X-Axe
	double min = ceil(m_dMinX/m_dResolutionX)*m_dResolutionX; // inner gridline
	double max = floor(m_dMaxX/m_dResolutionX)*m_dResolutionX; // outter gridline
	vtkPoints* points = vtkPoints::New();
	int iNumPoints = floor((min-max)/m_dResolutionX);
	points->SetNumberOfPoints(2*iNumPoints);
	cells = vtkCellArray::New();
	int i=0;
	for (double f=min; f<=m_dMaxX; f+=m_dResolutionX) {
		double factor = f*m_dLengthX/(m_dMaxX-m_dMinX);
		points->InsertPoint(2*i, factor, 0, 0);
		points->InsertPoint(2*i+1, factor, 0, -0.05);
		line = vtkLine::New();
		line->GetPointIds()->SetId(0, 2*i);
		line->GetPointIds()->SetId(1, 2*i+1);
		cells->InsertNextCell(line);
		i++;


		//label
		std::ostringstream s;
		s << f;
		pLabel = vtkVectorText::New();
		pLabel->SetText(s.str().c_str());
		mapper = vtkPolyDataMapper::New();
		mapper->SetInputConnection(pLabel->GetOutputPort());

		actor = vtkActor::New(); //vtkFollower::New();
		actor->SetMapper(mapper);
		actor->SetScale(0.025);
		actor->SetPosition(factor, 0, -0.05);
		actor->RotateY(90);
		actor->GetProperty()->SetOpacity(0.4);

		//if (m_pCamera != 0) 
		//	actor->SetCamera(m_pCamera);

		AddActor(actor);
		m_pLabels.push_back(actor);
	}

	polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetLines(cells);

	mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polydata);

	actor = vtkActor::New();
	actor->SetMapper(mapper);

	actor->GetProperty()->SetOpacity(0.4);

	AddActor(actor);
	m_pGrid.push_back(actor);

	// Y-axe
	min = ceil(m_dMinY/m_dResolutionY)*m_dResolutionY; // inner gridline
	max = floor(m_dMaxY/m_dResolutionY)*m_dResolutionY; // outter gridline
	points = vtkPoints::New();
	iNumPoints = floor((min-max)/m_dResolutionY);
	points->SetNumberOfPoints(2*iNumPoints);
	cells = vtkCellArray::New();
	i=0;
	for (double f=min; f<=m_dMaxY; f+=m_dResolutionY) {
		double factor = (f*m_dLengthY-m_dOffsetY)/(m_dMaxY-m_dMinY);
		points->InsertPoint(2*i, 0, factor, 0);
		points->InsertPoint(2*i+1, -0.05/sqrt(2.0f), factor, -0.05/sqrt(2.0f));
		line = vtkLine::New();
		line->GetPointIds()->SetId(0, 2*i);
		line->GetPointIds()->SetId(1, 2*i+1);
		cells->InsertNextCell(line);
		i++;

		//label
		std::ostringstream s;
		s << f;
		pLabel = vtkVectorText::New();
		pLabel->SetText(s.str().c_str());
		mapper = vtkPolyDataMapper::New();
		mapper->SetInputConnection(pLabel->GetOutputPort());

		actor = vtkActor::New(); //vtkFollower::New();
		actor->SetMapper(mapper);
		actor->SetScale(0.025);
		actor->SetPosition(-0.05/sqrt(2.0f), factor, -0.05/sqrt(2.0f));
		actor->RotateY(135);
		actor->GetProperty()->SetOpacity(0.4);

		//if (m_pCamera != 0) 
		//	actor->SetCamera(m_pCamera);

		AddActor(actor);
		m_pLabels.push_back(actor);
	}

	polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetLines(cells);

	mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polydata);

	actor = vtkActor::New();
	actor->SetMapper(mapper);

	actor->GetProperty()->SetOpacity(0.4);

	AddActor(actor);
	m_pGrid.push_back(actor);

	// Z-axe
	min = ceil(m_dMinZ/m_dResolutionZ)*m_dResolutionZ; // inner gridline
	max = floor(m_dMaxZ/m_dResolutionZ)*m_dResolutionZ; // outter gridline
	points = vtkPoints::New();
	iNumPoints = floor((min-max)/m_dResolutionZ);
	points->SetNumberOfPoints(2*iNumPoints);
	cells = vtkCellArray::New();
	i=0;
	for (double f=min; f<=m_dMaxZ; f+=m_dResolutionZ) {
		double factor = f*m_dLengthZ/(m_dMaxZ-m_dMinZ);
		points->InsertPoint(2*i, 0, 0, factor);
		points->InsertPoint(2*i+1, -0.05, 0, factor);
		line = vtkLine::New();
		line->GetPointIds()->SetId(0, 2*i);
		line->GetPointIds()->SetId(1, 2*i+1);
		cells->InsertNextCell(line);
		i++;

		//label
		std::ostringstream s;
		s << f;
		pLabel = vtkVectorText::New();
		pLabel->SetText(s.str().c_str());
		mapper = vtkPolyDataMapper::New();
		mapper->SetInputConnection(pLabel->GetOutputPort());

		actor = vtkActor::New(); //vtkFollower::New();
		actor->SetMapper(mapper);
		actor->SetScale(0.025);
		actor->SetPosition(-0.05, 0, factor);
		actor->RotateY(180);
		actor->GetProperty()->SetOpacity(0.4);

		//if (m_pCamera != 0) 
		//	actor->SetCamera(m_pCamera);

		AddActor(actor);
		m_pLabels.push_back(actor);
	}


	polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetLines(cells);

	mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polydata);

	actor = vtkActor::New();
	actor->SetMapper(mapper);

	actor->GetProperty()->SetOpacity(0.4);

	AddActor(actor);
	m_pGrid.push_back(actor);

	// //labels
	//
	//for (int i=0; i<=iNumPoints; i++) {
	//	std::ostringstream s;
	//	s << (double)i*m_dResolutionX;
	//	pLabel = vtkVectorText::New();
	//	pLabel->SetText(s.str().c_str());
	//	mapper = vtkPolyDataMapper::New();
	//	mapper->SetInputConnection(pLabel->GetOutputPort());

	//	actor = vtkFollower::New();
	//	actor->SetMapper(mapper);
	//	actor->SetScale(0.025);
	//	actor->SetPosition(i*m_dResolutionX, 0, 0);
	//	actor->GetProperty()->SetOpacity(0.2);

	//	//if (m_pCamera != 0) 
	//	//	actor->SetCamera(m_pCamera);
	//	
	//	AddActor(actor);
	//	m_pLabels.push_back(actor);
	//}
	UpdateAxes();	
	UpdateReference();
	if (!bWasVisible)
		SetGridVisible(false);
}

void CartesianCoordinateAssistant::UpdateReference() {
	double factor = (m_dReferenceLevel*m_dLengthY-m_dOffsetY)/(m_dMaxY-m_dMinY);
	m_pReferenceActor->SetPosition(0,factor,0);
	m_pReferenceActor->GetProperty()->SetOpacity(m_dReferenceAlpha);
}

void CartesianCoordinateAssistant::SetAxesLength(const double x, const double y, const double z) {
	m_dLengthX = x;
	m_dLengthY = y;
	m_dLengthZ = z;
	UpdateAxes();
	UpdateGrid();
}

bool CartesianCoordinateAssistant::GetAxesVisible() const {	

	return (m_pAxes.size()>0) ? m_pAxes[0]->IsVisible() : true;
}

void CartesianCoordinateAssistant::SetAxesVisible(bool bVisible) {
	for (int i=0; i<(int)m_pAxes.size(); i++)
		m_pAxes[i]->SetVisible(bVisible);
}

bool CartesianCoordinateAssistant::GetGridVisible() const {	
	return (m_pGrid.size()>0) ? m_pGrid[0]->GetVisibility()!=0 : true;
}


void CartesianCoordinateAssistant::SetGridVisible(bool bVisible) {
	if (bVisible) {
		for (int i=0; i<(int)m_pGrid.size(); i++)
			m_pGrid[i]->VisibilityOn();
		for (int i=0; i<(int)m_pLabels.size(); i++)
			m_pLabels[i]->VisibilityOn();
	} else {
		for (int i=0; i<(int)m_pGrid.size(); i++)
			m_pGrid[i]->VisibilityOff();
		for (int i=0; i<(int)m_pLabels.size(); i++)
			m_pLabels[i]->VisibilityOff();
	}
}
double CartesianCoordinateAssistant::GetReferenceLevel() const { return m_dReferenceLevel; }

void CartesianCoordinateAssistant::SetReferenceLevel(double dLevel) { 
	m_dReferenceLevel = dLevel; 
	UpdateReference();
}

double CartesianCoordinateAssistant::GetReferenceOpacity() const { return m_dReferenceAlpha; }

void CartesianCoordinateAssistant::SetReferenceOpacity(double dOpacity) { 
	m_dReferenceAlpha = dOpacity;
	UpdateReference();
}

bool CartesianCoordinateAssistant::getVisible() const {
	return GetAxesVisible() || GetGridVisible();
}

void CartesianCoordinateAssistant::SetVisible(bool bVisible) {
	SetAxesVisible(bVisible);
	SetGridVisible(bVisible);
}

void CartesianCoordinateAssistant::SetMinX(double min) { m_dMinX = min; }
	
void CartesianCoordinateAssistant::SetMaxX(double max) { m_dMaxX = max; }

void CartesianCoordinateAssistant::SetMinY(double min) { m_dMinY = min; }
	
void CartesianCoordinateAssistant::SetMaxY(double max) { m_dMaxY = max; }

void CartesianCoordinateAssistant::SetMinZ(double min) { m_dMinZ = min; }
	
void CartesianCoordinateAssistant::SetMaxZ(double max) { m_dMaxZ = max; }

void CartesianCoordinateAssistant::SetResolutionX(double res) { 
	m_dResolutionX = res; 
	UpdateGrid();
}

void CartesianCoordinateAssistant::SetResolutionY(double res) { m_dResolutionY = res; }

void CartesianCoordinateAssistant::SetResolutionZ(double res) { 
	m_dResolutionZ = res;
	UpdateGrid();
}

void CartesianCoordinateAssistant::SetOffsetY(double res) { 
	m_dOffsetY = res;
	UpdateAxes();
}

double CartesianCoordinateAssistant::GetMinX() const { return m_dMinX; }

double CartesianCoordinateAssistant::GetMaxX() const { return m_dMaxX; }

double CartesianCoordinateAssistant::GetMinY() const { return m_dMinY; }

double CartesianCoordinateAssistant::GetMaxY() const { return m_dMaxY; }

double CartesianCoordinateAssistant::GetMinZ() const { return m_dMinZ; }

double CartesianCoordinateAssistant::GetMaxZ() const { return m_dMaxZ; }

double CartesianCoordinateAssistant::GetResolutionX() const { return m_dResolutionX; }

double CartesianCoordinateAssistant::GetResolutionY() const { return m_dResolutionY; }

double CartesianCoordinateAssistant::GetResolutionZ() const { return m_dResolutionZ; }

double CartesianCoordinateAssistant::GetOffsetY() const { return m_dOffsetY; }

} // End of namespace "DAFFViz"
