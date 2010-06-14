// $Id: FXVTKColorMap.cpp,v 1.1 2010/02/10 09:57:25 schmoch Exp $

#include "FXVTKColorMap.h"
#include "vtkLookupTable.h"
#include "vtkScalarsToColors.h"



FXVTKColorMap::FXVTKColorMap() {
	Lookuptable = NULL;
}

FXVTKColorMap::~FXVTKColorMap() {
}


void FXVTKColorMap::SetRange(double min, double max) {
	if (Lookuptable != NULL) 
		Lookuptable->SetRange(min,max);
}
void FXVTKColorMap::SetRange(double *range) {
	if (Lookuptable != NULL && range != NULL)
		Lookuptable->SetRange(range);
}
void FXVTKColorMap::GetRange(double &min, double &max) {
	if (Lookuptable != NULL) {
		double* pointer = Lookuptable->GetTableRange();
		min = pointer[0];
		max = pointer[1];
	}
	else {
		min = 0;
		max = 0;
	}
}


vtkScalarsToColors* FXVTKColorMap::_getScalarsToColorsObject() {
	return Lookuptable;
}
vtkScalarsToColors* FXVTKColorMap::_getScalarsToColorsObject2() {
	return NULL;
}
