/*=============================================================================/
/						F X V T K P l o t O b j e c t						   /
/						  ( L i b :   F X V T K	)							   /
/==========================================================================*//*!

   \class 		FXVTKPlotObject

   \brief 		Abstract base class of all plotting objects, which make a
				3D-visualization with help of some two-dimensional data.
	
   A object of these class let you set data and calculate withit a
   visualisation which you can connect with a FXVTKWidget. You can set data
   with the SetData(...)-Methods. The object calculated automatically the
   visualisation, if new data are setted and the object will be used.


   \n
   $Id: FXVTKPlotObject.h,v 1.1 2010/02/10 09:57:25 schmoch Exp $

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/



#pragma once
#include "FXVTK3DObject.h"

class FXVTKColorMap;
class vtkFloatArray;
class vtkStructuredGrid;
class vtkDataSetMapper;
class vtkPoints;
class vtkActor;


class FXVTKPlotObject : public FXVTK3DObject {

public:
	/// Constructor 
	FXVTKPlotObject(void);
	/// Specified constructor, which also let connect with a widget (see FXVTKObject::connectWidget)
	FXVTKPlotObject(FXVTKWidgetBaseClass* widget);
	/// Destructor
	~FXVTKPlotObject(void);
	


public:
	/// Method to overwrite internal data, which will be plotted; Datainput should be two-dimensional.
	virtual void SetData(std::vector<std::vector<float>> *DataToCopy) = 0;

	/// Method to overwrite internal data, which will be plotted. 
	/*! \param DataToCopy  three-ary vector list, where inmost list will be filtered by index
		\param index       filter the value for the plot by the index element of the inmost vector-list.
		\return TRUE	inputdata and index should be okay, data would be set.  
		\return FALSE	data won't set because index not present or something goes wrong. */
	virtual bool SetData(std::vector<std::vector<std::vector<float>>> *DataToCopy, unsigned int index) = 0;

	/// Method to overwrite internal data, which will be plotted; Datainput must a float-array.
	/*! \param floatarray  float-array with minimal (Dim0Site*Dim1Size) elements.
		\param Dim0Size    number of "rows"
		\param Dim1Size    number of "column" */
	virtual void SetData(float *floatarray, unsigned int Dim0Size, unsigned int Dim1Size) = 0;


	/// Clear internal data, so nothing will be plotted.
	virtual void ClearData() = 0;

	/// Method to update manually the object (calculation will be restartet, if it wíll be necessary)
	/*! \sa _update() */
	virtual void Update();

	void SetColorMap(FXVTKColorMap* colormap);

	void GetRange(double &min, double &max);

	double* GetRange();

	/// Set LogartithmicMode on. Logarithmic mode is not used in all PlotObjects.
	void SetLogarithmicModeOn();
	/// Set LogartihnmisMode off. Logarithmic mode is not used in all PlotObjects.
	void SetLogarithmicModeOff();
	/// Returns true, if LogarithmicMode is on.
	bool LogarithmicModeIsOn();

protected:
	virtual void _update() = 0;


protected:
	vtkFloatArray* PlotDataScalars;
	vtkFloatArray* NewPlotDataScalars;
	vtkPoints* PlotDataPoints;
	vtkStructuredGrid* PlotStructuredGrid;
	vtkDataSetMapper* PlotDataSetMapper;
	unsigned int PlotDataDim0Size;
	unsigned int PlotDataDim1Size;
	FXVTKColorMap* ColorMap;
	vtkActor* PlotActor;

	bool LogarithmicMode;

private:
	void Initialization();
};
