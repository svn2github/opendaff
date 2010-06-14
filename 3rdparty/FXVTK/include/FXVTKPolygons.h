/*=============================================================================/
/						 F X V T K P o l y g o n s							   /
/						  ( L i b :   F X V T K	)							   /
/==========================================================================*//*!

   \class 		FXVTKPolygons

   \brief 		Let you make a list of polygons.
	
   -no description-


   \n
   $Id: FXVTKPolygons.h,v 1.1 2010/02/10 09:57:25 schmoch Exp $

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/

#ifndef __FXVTKPOLYGONS_H__
#define __FXVTKPOLYGONS_H__

#include "FXVTK3DObject.h"

class vtkPoints;
class vtkCellArray;
class vtkFloatArray;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkDepthSortPolyData;
class vtkLookupTable;
class vtkUnstructuredGrid;
class vtkDataSetMapper;
class vtkActor;
class RG_Vector;

class FXVTKPolygons : public FXVTK3DObject {

public:
	/// Constructor 
	FXVTKPolygons(void);
	/// Specified constructor, which also let connect with a widget (see FXVTKObject::connectWidget)
	FXVTKPolygons(FXVTKWidgetBaseClass*);
	/// Destructor
	~FXVTKPolygons(void);

	/// Get name of class of the object
	virtual const char* GetClassname();

	int AddPolygon(unsigned int npts, const double* points, unsigned char mat_id = 0);
	int AddPolygon(unsigned int npts, const float* floatpoints, unsigned char mat_id = 0);
	
	int AddPolygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, unsigned char mat_id = 0);
	int AddPolygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4, unsigned char mat_id = 0);
	int AddPolygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4, double x5, double y5, double z5, unsigned char mat_id = 0);

	void AddPolygon(int id, unsigned int npts, const double* points, unsigned char mat_id = 0);
	void AddPolygon(int id, unsigned int npts, const float* floatpoints, unsigned char mat_id = 0);

	void AddPolygon(int id, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, unsigned char mat_id = 0);
	void AddPolygon(
		int id,
		const std::vector<RG_Vector> *vertices, 
		const unsigned char mat_id = 0 );

	/// Clear the materialtable and the polygons
	void Clear();
	/// Clear the polygons
	void ClearPolygons();
	/// Clear the materialtable
	void ClearMaterialTable();

	void SetMaterial(unsigned char index, double r, double g, double b, double a);
	void SetMaterial(unsigned char index, double r, double g, double b);
	void SetMaterialAlpha(unsigned char index, double alpha);

	void GetMaterial(unsigned char index, double& r, double& g, double& b, double& a);
	void GetMaterial(unsigned char index, double& r, double& g, double& b);
	void GetMaterialAlpha(unsigned char index, double& alpha);
	double GetMaterialAlpha(unsigned char index);


	void SelectPolygon( unsigned int id );
	void DeselectPolygon();

	int _getRealCellID( int id_from_picking );

	/// Set the material of a polygon
	/**
	  \return false if Polygon don't exist.
	*/
    bool SetMaterialOfPolygon(int polygon_id, unsigned char mat_id);

	///  Specify the point to use when sorting. The fastest is to just take the first cell point. Other options are to take the bounding box center or the parametric center of the cell. By default, the first cell point is used. 
	void SetDepthSortModeToFirstPoint();
	void SetDepthSortModeToBoundsCenter();
	void SetDepthSortModeToParametricCenter();

private:
	void Initialization();
	virtual void _update();

	int _addPoint(double x, double y, double z);
	int _addPoint(const double point[]);
	int _addPoint(const float point[]);
	int _findPoint(double x, double y, double z);

private:
	vtkPoints* PolygonPoints;
	vtkCellArray* PolygonCells;
	vtkFloatArray* PolygonMaterialIds;
	vtkPolyData* PolygonsPolyData;
	vtkPolyDataMapper* PolygonsMapper;
	vtkLookupTable* PolygonMaterials;
	vtkDepthSortPolyData* DepthSort;
	vtkActor* PolygonsActor;

	// selected polygon:
	vtkUnstructuredGrid* SelectedPolygonGrid;
	vtkDataSetMapper* SelectedPolygonMapper;
	vtkActor* SelectedPolygonActor;
	unsigned int SelectedPolygonLastId;
	bool IsPolygonSelected;
	bool HasToBuildLinksNew;
};

#endif // __FXVTKPOLYGONS_H__