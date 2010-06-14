/*=============================================================================/
/					     F X V T K C o l o r M a p							   /
/						  ( L i b :   F X V T K	)							   /
/==========================================================================*//*!

   \class 		FXVTKColorMap

   \brief 		Abstract base class to define the relationship between scalar value and color
	
   - at the moment no detailed description -   


   \n
   $Id: FXVTKColorMap.h,v 1.1 2010/02/10 09:57:25 schmoch Exp $

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/



#pragma once


class vtkScalarsToColors;
class vtkLookupTable;


class FXVTKColorMap {

public:
	FXVTKColorMap();
	~FXVTKColorMap();

	/// Set the scalar values, which represent the minimum and maximum value-colors of the ColorMap
	virtual void SetRange(double min, double max);
	virtual void SetRange(double* range);

	/// Get the scalar values, which represent the minimum and maximum value-colors of the ColorMap
	virtual void GetRange(double &min, double &max);


	virtual vtkScalarsToColors* _getScalarsToColorsObject();
	virtual vtkScalarsToColors* _getScalarsToColorsObject2();
	

protected:
	vtkLookupTable* Lookuptable;
};
