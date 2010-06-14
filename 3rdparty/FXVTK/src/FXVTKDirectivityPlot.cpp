// $Id: FXVTKDirectivityPlot.cpp,v 1.5 2010/02/14 21:05:43 fwefers Exp $

#include "FXVTKDirectivityPlot.h"
#include "FXVTKColorMap.h"
#include <vtkFloatArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkStructuredGrid.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>

#include <DAFF.h>

#define PI  3.14159265358979323846


FXVTKGetClassnameMacro( FXVTKDirectivityPlot )

void FXVTKDirectivityPlot::Initialization() {
	ValueRangeLin[0] = 0;			ValueRangeLin[1] = 1;
	ValueRangeLog[0] = 0.001;		ValueRangeLog[1] = 1;

	//	RadiusoffsetScalefactor = 1;  // old, alt,nicht mehr benutzt
	SphereRadius = 1;
	LogModeIsOn = false;
	NorthpoleValue = 0;
	SouthpoleValue = 0;
	AutosetNorthpoleMethod = APVM_MEDIAN_BORDERED;
	AutosetSouthpoleMethod = APVM_MEDIAN_BORDERED;
	// AutosetRadiusoffsetScalefactorManipulation = ROUNDINGMODE_NONE;
	// AutosetRadiusoffsetScalefactorMethod = ARSM_NONE;
	// AutosetRadiusoffsetScalefactorMethodValue = 0;
}

FXVTKDirectivityPlot::FXVTKDirectivityPlot(void) {
	Initialization();
}
FXVTKDirectivityPlot::FXVTKDirectivityPlot(FXVTKWidgetBaseClass* widget) : FXVTKPlotObject(widget) {
	Initialization();
}
FXVTKDirectivityPlot::~FXVTKDirectivityPlot(void) {
}

void FXVTKDirectivityPlot::SetData(DAFFContentMS* pContentMS) {
	m_pContentMS = pContentMS;

	float sa = pContentMS->getProperties()->getAlphaStart();
	float ra = pContentMS->getProperties()->getAlphaResolution();
	float sb = pContentMS->getProperties()->getBetaStart();
	float rb = pContentMS->getProperties()->getBetaResolution();
	int na = pContentMS->getProperties()->getAlphaPoints();
	int nb = pContentMS->getProperties()->getBetaPoints();
	int nf = pContentMS->getNumFrequencies();

	m_data.clear();
	m_data.resize(nb);
	for (int b=0; b<nb; b++) {
		float fBeta = sb + b*rb;
		m_data[b].resize(na);

		for (int a=0; a<na; a++) {
			float fAlpha = sa + a*ra;

			int iRecordIndex;
			pContentMS->getNearestNeighbour(DAFF_DATA_VIEW, fAlpha, fBeta, iRecordIndex);			

			m_data[b][a].resize(nf);
			float* pfData = &(m_data[b][a][0]);
			pContentMS->getMagnitudes(iRecordIndex, 0, pfData);

			// TODO: Remove this artifical gain!
			for (int i=0; i<nf; i++) pfData[i] *= 0.25/pContentMS->getOverallMagnitudeMaximum();
		}	
	}

	SetValueRangeLin(0, pContentMS->getOverallMagnitudeMaximum());
}

void FXVTKDirectivityPlot::SetSelectedFrequency(int iIndex) {
	SetData(&m_data, iIndex);
}

void FXVTKDirectivityPlot::SetData(std::vector<std::vector<float>> *DataToCopy) {
	
	std::vector<std::vector<float>>::iterator Dim0Iterator = DataToCopy->begin();
	std::vector<float>::iterator Dim1Iterator = (*Dim0Iterator).begin();

	PlotDataDim0Size = (unsigned int)DataToCopy->size();
	PlotDataDim1Size = (unsigned int)(*Dim0Iterator).size();
	
	if (NewPlotDataScalars != NULL) NewPlotDataScalars->Delete();
	NewPlotDataScalars = vtkFloatArray::New();
	NewPlotDataScalars->Allocate( (PlotDataDim0Size+2)*(PlotDataDim1Size+1) );

	
	// First row (northpole) will be filled by zeros.
	for( unsigned int i = 0; i < PlotDataDim1Size+1; i++ ) NewPlotDataScalars->InsertNextValue(0.00f);

	// Next rows will be transfered with the datainput (first element of row will repeated at the end of the row)
	for( unsigned int dim0=0; dim0 < PlotDataDim0Size; dim0++ ) {
		Dim1Iterator = (*Dim0Iterator).begin();
		float firstRowElement = *Dim1Iterator;
		for( unsigned int dim1=0; dim1 < PlotDataDim1Size; dim1++ ) {
			if(Dim1Iterator != (*Dim0Iterator).end()) {
				NewPlotDataScalars->InsertNextValue(*Dim1Iterator);
				Dim1Iterator++;
			}
			else NewPlotDataScalars->InsertNextValue(0.00f);
		}
		NewPlotDataScalars->InsertNextValue(firstRowElement);
		Dim0Iterator++;
	}

	// Last row (southpole) will be filled by zeros.
	for( unsigned int i = 0; i < PlotDataDim1Size+1; i++) NewPlotDataScalars->InsertNextValue(0.00f);

	SetChanged();
}


bool FXVTKDirectivityPlot::SetData(std::vector<std::vector<std::vector<float>>> *DataToCopy, unsigned int index) {

	std::vector<std::vector<std::vector<float>>>::iterator Dim0Iterator = DataToCopy->begin();
	std::vector<std::vector<float>>::iterator Dim1Iterator = Dim0Iterator->begin();

	std::vector<std::vector<std::vector<float>>>::size_type asdf = (*DataToCopy).size();

	PlotDataDim0Size = (unsigned int)(*DataToCopy).size();
	PlotDataDim1Size = (unsigned int)(*Dim0Iterator).size();

	// If index is greater-equal than the Size of the elements the update-function aborted with false
	if (index >= (*Dim1Iterator).size()) return false;

	
	if (NewPlotDataScalars != NULL) NewPlotDataScalars->Delete();
	NewPlotDataScalars = vtkFloatArray::New();
	NewPlotDataScalars->Allocate( (PlotDataDim0Size+2)*(PlotDataDim1Size+1) );
		
	// First row (northpole) will be filled by zeros.
	for( unsigned int i = 0; i < PlotDataDim1Size+1; i++) NewPlotDataScalars->InsertNextValue(0.00f);

	// Next rows will be transfered with the datainput (first element of row will repeated at the end of the row)
	for( unsigned int dim0 = 0; dim0 < PlotDataDim0Size; dim0++ ) {
		Dim1Iterator = (*Dim0Iterator).begin();
		float firstRowElement = (*Dim1Iterator)[index];
		for( unsigned int dim1=0; dim1 < PlotDataDim1Size; dim1++) {
			if(Dim1Iterator != (*Dim0Iterator).end()) {
				NewPlotDataScalars->InsertNextValue((*Dim1Iterator)[index]);
				Dim1Iterator++;
			}
			else NewPlotDataScalars->InsertNextValue(0.00f);
		}
		NewPlotDataScalars->InsertNextValue(firstRowElement);
		Dim0Iterator++;
	}

	// Last row (southpole) will be filled by zeros.
	for( unsigned int i = 0; i < PlotDataDim1Size+1; i++) NewPlotDataScalars->InsertNextValue(0.00f);

	
	SetChanged();
	return true;
}


void FXVTKDirectivityPlot::SetData(float *floatarray, unsigned int Dim0Size, unsigned int Dim1Size) {
	
	if (NewPlotDataScalars != NULL) NewPlotDataScalars->Delete();
	NewPlotDataScalars = vtkFloatArray::New();
	NewPlotDataScalars->Allocate((Dim0Size+2)*(Dim1Size+1));

	PlotDataDim0Size = Dim0Size; PlotDataDim1Size = Dim1Size;
	
	
	// First row (northpole) will be filled by zeros.
	for( unsigned int i = 0; i < PlotDataDim1Size+1; i++) NewPlotDataScalars->InsertNextValue(0.00f);

	// Next rows will be transfered with the datainput (first element of row will repeated at the end of the row)
	for( unsigned int dim0=0; dim0 < Dim0Size; dim0++) {
		unsigned int start = dim0*Dim1Size;    unsigned int end=((dim0+1)*Dim1Size)-1;
		for( unsigned int i=start; i <= end; i++) {
			NewPlotDataScalars->InsertNextValue(floatarray[i]);
		}
		NewPlotDataScalars->InsertNextValue(floatarray[dim0*Dim1Size]);
	}

	// Last row (southpole) will be filled by zeros.
	for(unsigned int i = 0; i < Dim1Size+1; i++) NewPlotDataScalars->InsertNextValue(0.00f);

	
	SetChanged();
}


void FXVTKDirectivityPlot::ClearData() {

	if ( PlotDataSetMapper != NULL && PlotStructuredGrid != NULL ) {
		PlotActor->SetMapper(NULL);
		PlotDataSetMapper->Delete();
		PlotDataSetMapper = NULL;
		PlotStructuredGrid->SetDimensions(0,0,0);
		PlotActor->Modified();
	}	
	if ( NewPlotDataScalars != NULL ) {
		NewPlotDataScalars->Delete();
		NewPlotDataScalars = NULL;
	}
	if ( PlotDataScalars != NULL ) {
		PlotDataScalars->Delete();
		PlotDataScalars = NULL;
	}
}


void FXVTKDirectivityPlot::_update() {

	if ( IsUpToDate() ) return;
	
	// if no new scalars exist, but new calculation must do, Pointer will takeover
	if (NewPlotDataScalars == NULL) {
		NewPlotDataScalars = PlotDataScalars; 
		PlotDataScalars = NULL;
	}

	if ( NewPlotDataScalars == NULL ) return;

	float currentNorthpoleValue, currentSouthpoleValue; 
	float sum; vtkIdType min_offs, max_offs; 

	// Methods to set the Values of Northpole and Southpole automatically calculated
	switch (AutosetNorthpoleMethod) { 
		//   take the Median of all bordered values, here the second row (first row of data)
		case APVM_MEDIAN_BORDERED: 
			sum = 0; min_offs = PlotDataDim1Size+1;  max_offs = PlotDataDim1Size*2; 
			vtkFloatArray* scalars;
			if (NewPlotDataScalars != NULL) scalars = NewPlotDataScalars;
			else scalars = PlotDataScalars;

			for (vtkIdType offs=min_offs; offs <= max_offs; offs++) 
				sum += scalars->GetValue(offs);
			currentNorthpoleValue = sum / PlotDataDim1Size;

			break;
		//   set the pole point in the middle of the sphere, value is something << 0
		case APVM_MINUS_RADIUS:
			currentNorthpoleValue = ValueRangeLin[0];
			break;
		//   set the pole values to zero, point has radius of SphereRadius
		case APVM_ZERO:
			currentNorthpoleValue = 0;
			break;
		case APVM_NONE:
		default:
			currentNorthpoleValue = NorthpoleValue;
			break;
	}
	switch (AutosetSouthpoleMethod) {
		case APVM_MEDIAN_BORDERED:
			sum = 0; min_offs = (PlotDataDim1Size+1)*PlotDataDim0Size;  max_offs = (PlotDataDim1Size+1)*(PlotDataDim0Size+1)-2;
			vtkFloatArray* scalars;
			if (NewPlotDataScalars != NULL) scalars = NewPlotDataScalars;
			else scalars = PlotDataScalars;

			for (vtkIdType offs=min_offs; offs <= max_offs; offs++)
				sum += scalars->GetValue(offs);
			currentSouthpoleValue = sum / PlotDataDim1Size;
			break;
		case APVM_MINUS_RADIUS:
			currentSouthpoleValue = ValueRangeLin[0];
			break;
		case APVM_ZERO:
			currentSouthpoleValue = 0;
			break;
		case APVM_NONE:
		default:
			currentSouthpoleValue = SouthpoleValue;
			break;
	}


	/*
	float scalefactor;
	if (AutosetRadiusoffsetScalefactorMethod != ARSM_NONE) {
		double range[2];
		PlotDataScalars->GetRange(range);
		switch (AutosetRadiusoffsetScalefactorMethod) {
//			case ARSM_MIN_TO_VALUE:
//				// scalefactor = AutosetRadiusoffsetScalefactorMethodValue / range[0];
//				break;
//			case ARSM_MIN_ABS_TO_VALUE:
//				
//				break;
			case ARSM_MAX_TO_VALUE:
				scalefactor = AutosetRadiusoffsetScalefactorMethodValue / range[1];
				break;
			case ARSM_MAX_ABS_TO_VALUE:
				if (range[0] > 0 || (-range[0]) < range[1]) {
					scalefactor = AutosetRadiusoffsetScalefactorMethodValue / range[1];
				}
				else {
					scalefactor = AutosetRadiusoffsetScalefactorMethodValue /(-range[0]);
				}
				break;
//			case ARSM_MEDIAN_ABS_TO_VALUE:
//				
//				break;
//			case ARSM_LIMITMIN_AT_VALUE:
//
//				break;
//			case ARSM_LIMITMAX_AT_VALUE:
//				//
//				break;
//			case ARSM_LIMIT_ABS_AT_VALUE:
//				//
//				break;
		}
	}
	else scalefactor=RadiusoffsetScalefactor;
	*/





	vtkPoints *NewPoints = vtkPoints::New();
	NewPoints->Allocate( (PlotDataDim0Size+2) * (PlotDataDim1Size+1) );

	if ( LogModeIsOn == false ) {  // !!!  muss in false geändert werden 
		// *** linear deformation ***
	
		vtkIdType offset;
		float r;

		// northpole
		r = ( currentNorthpoleValue - ValueRangeLin[0] ) / ( ValueRangeLin[1] - ValueRangeLin[0] );
		for ( offset=0; offset < (vtkIdType)( PlotDataDim1Size+1 ); offset++) {
			NewPoints->InsertPoint( offset, 0, r, 0 );
			NewPlotDataScalars->SetValue(offset, currentNorthpoleValue);
		}

		// mainpart
		for ( unsigned int polar=1; polar<PlotDataDim0Size+1; polar++) {
			float polarwinkel = polar * PI / (PlotDataDim0Size+1);
			
			for ( unsigned int azimuth=0; azimuth<PlotDataDim1Size+1; azimuth++ ) {
				float azimuthwinkel = azimuth*2*PI/PlotDataDim1Size;
				float value = NewPlotDataScalars->GetValue(offset);
				
				
				if ( value < ValueRangeLin[0] ) 
					r = 0;
				else if ( value > ValueRangeLin[1] )
					r = 1;
				else 
					r = ( value - ValueRangeLin[0]) / ( ValueRangeLin[1] - ValueRangeLin[0] );
				
				// Z - Achse Rotationsachse
				// float x = r * sin(polarwinkel) * cos(azimuthwinkel);
				// float y = r * sin(polarwinkel) * sin(azimuthwinkel);
				// float z = r * cos(polarwinkel);
				
				// Y - Achse Rotationsachse
				float z = r * sin(polarwinkel) * cos(azimuthwinkel);
				float x = r * sin(polarwinkel) * sin(azimuthwinkel);
				float y = r * cos(polarwinkel);

				NewPoints->InsertPoint(offset++,x,y,z);
			}
		}

		// northpole
		r = -1 * ( currentNorthpoleValue - ValueRangeLin[0] ) / ( ValueRangeLin[1] - ValueRangeLin[0] );
		for ( offset; offset < (vtkIdType)((PlotDataDim0Size+2)*(PlotDataDim1Size+1)); offset++) {
			NewPoints->InsertPoint( offset, 0, r, 0 );
			NewPlotDataScalars->SetValue(offset, currentSouthpoleValue);
		}
	}


	else {
		// *** logarithmic deformation ***

		vtkIdType offset;
		float r;
		float log_rangemin = log10f( ValueRangeLog[0] );
		float log_rangemax = log10f( ValueRangeLog[1] );

		// northpole
		r = ( log10f ( currentNorthpoleValue ) - log_rangemin ) / ( log_rangemax - log_rangemin );
		for ( offset=0; offset < (vtkIdType)(PlotDataDim1Size+1); offset++) {
			NewPoints->InsertPoint( offset, 0, r, 0 );
			NewPlotDataScalars->SetValue(offset, currentNorthpoleValue);
		}

		// mainpart
		for ( unsigned int polar=1; polar<PlotDataDim0Size+1; polar++) {
			float polarwinkel = polar * PI / (PlotDataDim0Size+1);
			
			for ( unsigned int azimuth=0; azimuth<PlotDataDim1Size+1; azimuth++ ) {
				float azimuthwinkel = azimuth*2*PI/PlotDataDim1Size;
				float value = NewPlotDataScalars->GetValue(offset);
				
				
				if ( value < ValueRangeLin[0] ) 
					r = 0;
				else if ( value > ValueRangeLin[1] )
					r = 1;
				else 
					r = ( log10f( value ) - log_rangemin ) / ( log_rangemax - log_rangemin );
				
				// Z - Achse Rotationsachse
				// float x = r * sin(polarwinkel) * cos(azimuthwinkel);
				// float y = r * sin(polarwinkel) * sin(azimuthwinkel);
				// float z = r * cos(polarwinkel);
				
				// Y - Achse Rotationsachse
				float z = r * sin(polarwinkel) * cos(azimuthwinkel);
				float x = r * sin(polarwinkel) * sin(azimuthwinkel);
				float y = r * cos(polarwinkel);

				NewPoints->InsertPoint(offset++,x,y,z);
			}
		}

		// northpole
		r = -1 * ( log10f( currentSouthpoleValue ) - log_rangemin ) / ( log_rangemax - log_rangemin );
		for ( offset; offset < (vtkIdType)( (PlotDataDim0Size+2)*(PlotDataDim1Size+1) ); offset++) {
			NewPoints->InsertPoint( offset, 0, r, 0 );
			NewPlotDataScalars->SetValue(offset, currentSouthpoleValue);
		}
	}




	// StructuredGrid will be made, if it not exist
	if (PlotStructuredGrid == NULL) {
		PlotStructuredGrid = vtkStructuredGrid::New();
		PlotStructuredGrid->SetDimensions( PlotDataDim1Size+1, 1, PlotDataDim0Size+2 );
	}
	//  If Dimensions changed, new structured grid must be created;
	else {
		int *OldDims = PlotStructuredGrid->GetDimensions();
		if (OldDims[0] != (PlotDataDim1Size+1) || OldDims[2] != (PlotDataDim0Size+2)) { // ca. 7% faster if you use old grid, if Dims not changed
			vtkStructuredGrid* NewSGrid = vtkStructuredGrid::New();
			NewSGrid->SetDimensions(PlotDataDim1Size+1,1,PlotDataDim0Size+2);
			if ( PlotDataSetMapper != NULL ) PlotDataSetMapper->SetInput( NewSGrid );
			PlotStructuredGrid->Delete();
			PlotStructuredGrid = NewSGrid;
		}
	}
		
	PlotStructuredGrid->SetPoints(NewPoints);
	PlotStructuredGrid->GetPointData()->SetScalars(NewPlotDataScalars);

	
	if (PlotDataSetMapper == NULL) {
		PlotDataSetMapper = vtkDataSetMapper::New();
		PlotDataSetMapper->SetInput(PlotStructuredGrid);	
		PlotDataSetMapper->ImmediateModeRenderingOn();
		if (ColorMap != NULL) {
			PlotDataSetMapper->SetLookupTable(ColorMap->_getScalarsToColorsObject());
			PlotDataSetMapper->UseLookupTableScalarRangeOn();
		}
		PlotActor->SetMapper(PlotDataSetMapper);
	}
	
	if (ColorMap == NULL) {    // If ColorMap is set ColorRange don't set automatically
		double range[2];
		PlotStructuredGrid->GetScalarRange(range);
		PlotDataSetMapper->SetScalarRange(range);
		// PlotDataSetMapper->SetColorModeToDefault();
	}

	// VTK should update here mapper and all with-mapper-above-connected object mapper, so old dataobjects can be deleted
	PlotDataSetMapper->Update();


	// Old points and scalars will be deleted and new of them will be takeover
	if (PlotDataScalars != NULL) PlotDataScalars->Delete();
	PlotDataScalars = NewPlotDataScalars;
	NewPlotDataScalars = NULL;
	if (PlotDataPoints != NULL) PlotDataPoints->Delete();
	PlotDataPoints = NewPoints;
	NewPoints = NULL;


	SetUpToDate();
}


void FXVTKDirectivityPlot::SetValueRangeLin(const float min, const float max) {
	ValueRangeLin[0] = min;
	ValueRangeLin[1] = max;
}
float* FXVTKDirectivityPlot::GetValueRangeLin() {
	return ValueRangeLin;
}
void FXVTKDirectivityPlot::SetValueRangeLog(const float min, const float max) {
	ValueRangeLog[0] = min;
	ValueRangeLog[1] = max;
}
float* FXVTKDirectivityPlot::GetValueRangeLog() {
	return ValueRangeLog;
}
void FXVTKDirectivityPlot::SetLogModeOn() {
	if ( LogModeIsOn == true ) return;
	LogModeIsOn = true;
	SetChanged();
}
void FXVTKDirectivityPlot::SetLogModeOff() {
	if ( LogModeIsOn == false ) return;
	LogModeIsOn = false;
	SetChanged();
}
bool FXVTKDirectivityPlot::IsLogModeOn() {
	return LogModeIsOn;
}




void FXVTKDirectivityPlot::SetNorthpoleValue(float value) {
	AutosetNorthpoleMethod = APVM_NONE;
	NorthpoleValue = value;
}
void FXVTKDirectivityPlot::SetSouthpoleValue(float value) {
	AutosetSouthpoleMethod = APVM_NONE;
	SouthpoleValue = value;
}
float FXVTKDirectivityPlot::GetNorthpoleValue() {
	return NorthpoleValue;
}
float FXVTKDirectivityPlot::GetSouthpoleValue() {
	return SouthpoleValue;
}
void FXVTKDirectivityPlot::SetAutosetNorthpoleValueMethod(_AutosetPoleValueMethod method) {
	AutosetNorthpoleMethod = method;
}
void FXVTKDirectivityPlot::SetAutosetSouthpoleValueMethod(_AutosetPoleValueMethod method) {
	AutosetSouthpoleMethod = method;
}
void FXVTKDirectivityPlot::SetAutosetPoleValueMethod(_AutosetPoleValueMethod method) {
	AutosetNorthpoleMethod = method;
	AutosetSouthpoleMethod = method;
}
FXVTKDirectivityPlot::_AutosetPoleValueMethod FXVTKDirectivityPlot::GetAutosetNorthpoleValueMethod() {
	return AutosetNorthpoleMethod;
}
FXVTKDirectivityPlot::_AutosetPoleValueMethod FXVTKDirectivityPlot::GetAutosetSouthpoleValueMethod() {
	return AutosetNorthpoleMethod;
}


/*
void FXVTKDirectivityPlot::SetAutosetRadiusoffsetScalefactorMethod(FXVTKDirectivityPlot::_AutosetRadiusoffsetScalefactorMethod method) {
	AutosetRadiusoffsetScalefactorMethod = method;
}
void FXVTKDirectivityPlot::SetAutosetRadiusoffsetScalefactorMethod(_AutosetRadiusoffsetScalefactorMethod method, float MethodArgument) {
	AutosetRadiusoffsetScalefactorMethod = method;
	AutosetRadiusoffsetScalefactorMethodValue = MethodArgument;
}
FXVTKDirectivityPlot::_AutosetRadiusoffsetScalefactorMethod FXVTKDirectivityPlot::GetAutosetRadiusoffsetScalefactorMethod() {
	return AutosetRadiusoffsetScalefactorMethod;
}
void FXVTKDirectivityPlot::SetAutosetRadiusoffsetScalefactorMethodValue(float MethodValue) {
	AutosetRadiusoffsetScalefactorMethodValue = MethodValue;
}
float FXVTKDirectivityPlot::GetAutosetRadiusoffsetScalefactorMethodValue() {
	return AutosetRadiusoffsetScalefactorMethodValue;
}
*/