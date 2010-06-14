// $Id: FXVTKPolygons.cpp,v 1.1 2010/02/10 09:57:25 schmoch Exp $

#include "FXVTKPolygons.h"
#include "FXVTKWidgetBaseClass.h"
#include "vtkPoints.h"
#include "vtkFloatArray.h"
#include "vtkPolyData.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProp3DCollection.h"
#include "vtkLookupTable.h"
#include "vtkProperty.h"
#include "vtkDepthSortPolyData.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkCell.h"
#include "vtkIdList.h"




/*
	Zu lösendes Problem: ?????

	FXVTKPolygons* polygons = new FXVTKPolygons(Widget);

	// Verursacht Fehler:
	polygons->AddPolygon(0, 0.00,0.00,0.00, 1.00,0.00,0.00, 1.00,1.00,0.00);

*/


FXVTKGetClassnameMacro( FXVTKPolygons )

void FXVTKPolygons::Initialization() {
	PolygonPoints = vtkPoints::New();
	PolygonCells = vtkCellArray::New();
	PolygonMaterialIds = vtkFloatArray::New();
	DepthSort = vtkDepthSortPolyData::New();
	DepthSort->SetDepthSortModeToBoundsCenter();


	PolygonMaterials = vtkLookupTable::New();
	PolygonMaterials->Allocate();
	PolygonMaterials->SetRange(0,255);
	// Initialize MaterialTable
	for (int i = 0; i < 255; i++)
		PolygonMaterials->SetTableValue(i,1.0,1.0,1.0,1.0);


	PolygonsPolyData = NULL;
	PolygonsMapper = NULL;

	PolygonsActor = vtkActor::New();
	Actors3D->AddItem(PolygonsActor);

	// selected polygon
	SelectedPolygonMapper = NULL;
	SelectedPolygonGrid = NULL;
	SelectedPolygonActor = vtkActor::New();
	Actors3D->AddItem( SelectedPolygonActor );
	IsPolygonSelected = false;
	HasToBuildLinksNew = true;

	SetPickableOff();
}

FXVTKPolygons::FXVTKPolygons(void) {
	Initialization();
}
FXVTKPolygons::FXVTKPolygons(FXVTKWidgetBaseClass *widget) {
	Initialization();
	ConnectWidget(widget);
}
FXVTKPolygons::~FXVTKPolygons(void) {
	DisconnectAllWidgets();

	PolygonPoints->Delete();
	PolygonCells->Delete();
	PolygonMaterialIds->Delete();
	PolygonMaterials->Delete();
	DepthSort->Delete();
	if (PolygonsPolyData != NULL) PolygonsPolyData->Delete();
	if (PolygonsMapper != NULL) PolygonsMapper->Delete();
	PolygonsActor->Delete();

	if ( SelectedPolygonMapper != NULL ) {
		SelectedPolygonGrid->Delete();
		SelectedPolygonMapper->Delete();
	}
	SelectedPolygonActor->Delete();
}

void FXVTKPolygons::_update() {
	if ( IsUpToDate() ) return;

	if (PolygonsPolyData == NULL) {
		PolygonsPolyData = vtkPolyData::New();

		// We now assign the pieces to the vtkPolyData.
		PolygonsPolyData->SetPoints(PolygonPoints);
		PolygonsPolyData->SetPolys(PolygonCells);
		PolygonsPolyData->GetCellData()->SetScalars(PolygonMaterialIds);

		DepthSort->SetInput(PolygonsPolyData);
				
		PolygonsMapper = vtkPolyDataMapper::New();
		PolygonsMapper->SetInputConnection(DepthSort->GetOutputPort());
		PolygonsMapper->SetLookupTable(PolygonMaterials);
		PolygonsMapper->SetScalarModeToUseCellData();
		PolygonsMapper->SetScalarRange(0,255);
		
		PolygonsActor->SetMapper(PolygonsMapper);

	}
	
	vtkCamera* cam = WidgetList[0]->Camera;
	if (cam != NULL) {
		DepthSort->SetCamera(cam);
		DepthSort->SetDirectionToBackToFront();
		SetUpToDate();
	}
	else {
		DepthSort->SetVector(0,0,-1); 
		DepthSort->SetDirectionToSpecifiedVector();
		SetChanged();
	}

	PolygonsMapper->Update();
}



int FXVTKPolygons::AddPolygon(unsigned int npts, const double *points, unsigned char mat_id) {
	if (npts < 3) return -1;	// a polygon has minimal three corners
	if (npts >= 32) npts = 32;  // npts is limited to 32 per definition
	
	int* point_ids = new int[npts];
	for (unsigned int i = 0; i < npts; i++)
		point_ids[i] = _addPoint(&points[3*i]);

	int cell_id = PolygonCells->InsertNextCell(npts, point_ids);
	PolygonMaterialIds->InsertTuple1(cell_id, mat_id);

	SetChanged();
	HasToBuildLinksNew = true;

	delete point_ids;
	return cell_id;
}

int FXVTKPolygons::AddPolygon(unsigned int npts, const float *floatpoints, unsigned char mat_id) {
	if (npts < 3) return -1;	// a polygon has minimal three corners
	if (npts >= 32) npts = 32;  // npts will limited arbitraliry (~deutsch: willkürlich) to 32

	int* point_ids = new int[npts];
	for (unsigned int i = 0; i < npts; i++)
		point_ids[i] = _addPoint(&floatpoints[3*i]);

	int cell_id = PolygonCells->InsertNextCell(npts, point_ids);
	PolygonMaterialIds->InsertTuple1(cell_id, mat_id);

	SetChanged();
	HasToBuildLinksNew = true;

	delete [] point_ids;
	return cell_id;
}

int FXVTKPolygons::AddPolygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, unsigned char mat_id) {
	double points[9] = {x1,y1,z1,x2,y2,z2,x3,y3,z3};
	return AddPolygon(3, points, mat_id);

/*  alternative:
	int points[3];
	points[0] = _addPoint(x1,y1,z1);
	points[1] = _addPoint(x2,y2,z2);
	points[2] = _addPoint(x3,y3,z3);
		
	int cell_id = PolygonCells->InsertNextCell(3,points);
	PolygonMaterialIds->InsertTuple1(cell_id, mat_id);

	SetChanged();
	HasToBuildLinksNew = true;
	return cell_id;
*/
}

int FXVTKPolygons::AddPolygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4, unsigned char mat_id) {
	double points[12] = {x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4};
	return AddPolygon(4, points, mat_id);
}

int FXVTKPolygons::AddPolygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4, double x5, double y5, double z5, unsigned char mat_id) {
	double points[15] = {x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4,x5,y5,z5};
	return AddPolygon(5, points, mat_id);
}



// "AddPolygon"-functions which replace or add with passed(uebergebenen) id :

void FXVTKPolygons::AddPolygon(int id, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, unsigned char mat_id) {
	int points[3];
	points[0] = _addPoint(x1,y1,z1);
	points[1] = _addPoint(x2,y2,z2);
	points[2] = _addPoint(x3,y3,z3);
		
	PolygonCells->ReplaceCell(id,3,points);
	PolygonMaterialIds->InsertTuple1(id, mat_id);

	SetChanged();
	HasToBuildLinksNew = true;
}

void FXVTKPolygons::AddPolygon(int id, unsigned int npts, const double *points, unsigned char mat_id) {
	if (npts < 3) return;		// a polygon has minimal three corners
	if (npts >= 32) npts = 32;  // npts is limited to 32 per definition
	
	int* point_ids = new int[npts];
	for (unsigned int i = 0; i < npts; i++)
		point_ids[i] = _addPoint(&points[3*i]);

	PolygonCells->ReplaceCell(id, npts, point_ids);
	PolygonMaterialIds->InsertTuple1(id, mat_id);

	SetChanged();
	HasToBuildLinksNew = true;

	delete point_ids;
}

void FXVTKPolygons::AddPolygon(int id, unsigned int npts, const float *floatpoints, unsigned char mat_id) {
	if (npts < 3) return;	// a polygon has minimal three corners
	if (npts >= 32) npts = 32;  // npts will limited arbitraliry (~deutsch: willkürlich) to 32

	int* point_ids = new int[npts];
	for (unsigned int i = 0; i < npts; i++)
		point_ids[i] = _addPoint(&floatpoints[3*i]);

	PolygonCells->ReplaceCell(id, npts, point_ids);
	PolygonMaterialIds->InsertTuple1(id, mat_id);

	SetChanged();
	HasToBuildLinksNew = true;

	delete [] point_ids;
}



void FXVTKPolygons::Clear() {
	ClearPolygons();
	ClearMaterialTable();
}
void FXVTKPolygons::ClearPolygons() {
	if ( IsPolygonSelected == true ) DeselectPolygon();
	PolygonCells->Reset();
	PolygonPoints->Initialize();
	PolygonMaterialIds->Initialize();

	SetChanged();
	HasToBuildLinksNew = true;
}
void FXVTKPolygons::ClearMaterialTable() {
	// Initialize MaterialTable
	for (int i = 0; i < 255; i++)
		PolygonMaterials->SetTableValue(i,1.0,1.0,1.0,1.0);
}


int FXVTKPolygons::_addPoint(double x, double y, double z) {
	int temp = _findPoint(x,y,z);
	if (temp < 0) 
		return PolygonPoints->InsertNextPoint(x,y,z);
	else return temp;
}
int FXVTKPolygons::_addPoint(const double point[]) {
	int temp = _findPoint(point[0],point[1],point[2]);
	if (temp < 0) 
		return PolygonPoints->InsertNextPoint(point);
	else return temp;
}
int FXVTKPolygons::_addPoint(const float point[]) {
	int temp = _findPoint(point[0],point[1],point[2]);
	if (temp < 0) 
		return PolygonPoints->InsertNextPoint(point);
	else return temp;
}
int FXVTKPolygons::_findPoint(double x, double y, double z) {
	return -1;

	int n = PolygonPoints->GetNumberOfPoints();
	if (n == 0) return -1;
	const double epsilon = 0.000001;
	double x_min = x - epsilon; double x_max = x + epsilon;
	double y_min = y - epsilon; double y_max = y + epsilon;
	double z_min = z - epsilon; double z_max = z + epsilon;
	double* points;
	for(int i=0; i < n; i++) {
		points = PolygonPoints->GetPoint(i);
		if ( points[0] > x_min && points[0] < x_max &&
			 points[1] > y_min && points[1] < y_max && 
			 points[2] > z_min && points[2] < z_max )     return i;
	}
	return -1;
}


void FXVTKPolygons::SetMaterial(unsigned char index, double r, double g, double b, double alpha) {
	PolygonMaterials->SetTableValue(index, r, g, b, alpha);
}
void FXVTKPolygons::SetMaterial(unsigned char index, double r, double g, double b){
	PolygonMaterials->SetTableValue(index, r, g, b, GetMaterialAlpha(index));
}
void FXVTKPolygons::SetMaterialAlpha(unsigned char index, double alpha) {
	double *pointer = PolygonMaterials->GetTableValue(index);
	PolygonMaterials->SetTableValue(index, pointer[0], pointer[1], pointer[2], alpha);
}

void FXVTKPolygons::GetMaterial(unsigned char index, double& r, double& g, double& b, double& alpha) {
	double *pointer = PolygonMaterials->GetTableValue(index);
	r = pointer[0]; g = pointer[1]; b = pointer[2]; alpha = pointer[3];
}
void FXVTKPolygons::GetMaterial(unsigned char index, double& r, double& g, double& b) {
	double *pointer = PolygonMaterials->GetTableValue(index);
	r = pointer[0]; g = pointer[1]; b = pointer[2];
}
void FXVTKPolygons::GetMaterialAlpha(unsigned char index, double& alpha) {
	double *pointer = PolygonMaterials->GetTableValue(index);
	alpha = pointer[3];
}
double FXVTKPolygons::GetMaterialAlpha(unsigned char index) {
	double *pointer = PolygonMaterials->GetTableValue(index);
	return pointer[3];
}




void FXVTKPolygons::SelectPolygon( unsigned int id ) {
	
	if ( IsPolygonSelected == true && SelectedPolygonLastId == id )
		// polygon is yet selected
		return;


	vtkIdList* list = vtkIdList::New();
	list->DeepCopy( PolygonsPolyData->GetCell( id )->GetPointIds() );
		
	if ( list->GetNumberOfIds() == 0 ) {  // cell with id don't exist
		if ( IsPolygonSelected == true )
			DeselectPolygon();
		return;
	}

	list->InsertNextId( list->GetId(0) );

	// here: pointids are in "list" ( in a loop so last id is the same as the first id )


	if ( SelectedPolygonMapper == NULL ) {
		SelectedPolygonGrid = vtkUnstructuredGrid::New();
		SelectedPolygonGrid->Allocate( 1, 1 );
		SelectedPolygonMapper = vtkDataSetMapper::New();

		SelectedPolygonGrid->SetPoints( PolygonPoints );
		SelectedPolygonMapper->SetInput( SelectedPolygonGrid );
		SelectedPolygonActor->SetMapper( SelectedPolygonMapper );
		
		SelectedPolygonActor->GetProperty()->SetColor( 1,0.2,0.2 );
		SelectedPolygonActor->GetProperty()->SetLineWidth( 1.5 );
	}

	// if ( SelectedPolygonGrid->GetNumberOfCells() > 0 ) {
	//	SelectedPolygonGrid->Initialize();
	// }

	SelectedPolygonGrid->Initialize();
	SelectedPolygonGrid->Allocate(1,1);
	SelectedPolygonGrid->InsertNextCell( VTK_POLY_LINE, list );
	SelectedPolygonGrid->SetPoints( PolygonPoints );

	IsPolygonSelected = true;
	SelectedPolygonLastId = id;
	

	list->Delete();
	SetChanged();

}


void FXVTKPolygons::DeselectPolygon() {
	
	if ( IsPolygonSelected == false ) return;

	if ( SelectedPolygonGrid == NULL ) {
		IsPolygonSelected = false;
		return;
	}

	
	SelectedPolygonGrid->Initialize();
	// SelectedPolygonGrid->Allocate(1,1);
	SelectedPolygonGrid->SetPoints( PolygonPoints );

	SelectedPolygonLastId = 0;
	IsPolygonSelected = false;


	//SelectedPolygonGrid->GetCells()->Initialize();
	//SelectedPolygonMapper->Update();
	//SelectedPolygonActor->Modified();

}


int FXVTKPolygons::_getRealCellID( int id_from_picking ) {
	
	vtkIdList* pointidlist = DepthSort->GetOutput()->GetCell( id_from_picking )->GetPointIds();
	vtkIdType npts = pointidlist->GetNumberOfIds();
	unsigned short ncells;
	vtkIdType* cellids;

	if ( HasToBuildLinksNew == true ) {
		PolygonsPolyData->BuildLinks();
		HasToBuildLinksNew = false;
	}
	PolygonsPolyData->GetPointCells( pointidlist->GetId(0), ncells, cellids );
	
	for ( vtkIdType i = 0; i < ncells; i++ ) {

		vtkIdList* pointidlist2 = PolygonsPolyData->GetCell( cellids[i] )->GetPointIds();
		if ( npts == pointidlist2->GetNumberOfIds() ) {

			bool test = true;

			for ( int j = 0; j < npts; j++ ) {
				if ( pointidlist2->IsId( pointidlist->GetId(j) ) < 0 ) {
					// id not in other list
					test = false;
					break;
				}
			}

			if ( test == true ) return cellids[i];
		}
	}

	return -1;
}


bool FXVTKPolygons::SetMaterialOfPolygon(int polygon_id, unsigned char mat_id) {

	vtkIdType npts;
	vtkIdType* pts;
	PolygonCells->GetCell(polygon_id, npts, pts);

	// If npts < 1, polygon don't exist
	if (npts < 1) return false;

	PolygonMaterialIds->InsertTuple1(polygon_id, mat_id);
	return true;

}






void FXVTKPolygons::SetDepthSortModeToFirstPoint() {
	DepthSort->SetDepthSortModeToFirstPoint();
}
void FXVTKPolygons::SetDepthSortModeToBoundsCenter() {
	DepthSort->SetDepthSortModeToBoundsCenter();
}
void FXVTKPolygons::SetDepthSortModeToParametricCenter() {
	DepthSort->SetDepthSortModeToParametricCenter();
}
