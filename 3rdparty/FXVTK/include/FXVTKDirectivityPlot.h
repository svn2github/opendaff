/*=============================================================================/
/					F X V T K D i r e c t i v i t y P l o t					   /
/						  ( L i b :   F X V T K	)							   /
/==========================================================================*//*!

   \class 		FXVTKDirectivityPlot

   \brief 		Let you make a directivity plot.
	
   A directivity plot is a deformed sphere, whose deformation is calculated of
   data, which setted with the SetData(...)-methods. Every points of the grid, 
   which consisted by the globe, has a radius and a color, which represent a 
   value of the data. The data-input should represent a 2D-array.


   \n
   $Id: FXVTKDirectivityPlot.h,v 1.3 2010/02/10 11:23:31 schmoch Exp $

*//*===========================================================================/
/  Author(s):																   /
/  cand. ing. Jochen Giese				mailto:jochen.giese@rwth-aachen.de	   /
/																		       /
/  Institute of Technical Acoustics											   /
/  RWTH Aachen University, 2007												   /
/  http://www.akustik.rwth-aachen.de										   /
/=============================================================================*/



#pragma once
#include "FXVTKPlotObject.h"

class DAFFContentDFT;
class DAFFContentMS;

class FXVTKDirectivityPlot : public FXVTKPlotObject {

public:
	/// Constructor
	FXVTKDirectivityPlot(void);
	/// Specified constructor, which also let connect with a widget (see FXVTKObject::connectWidget)
	FXVTKDirectivityPlot(FXVTKWidgetBaseClass* widget);
	/// Destructor
	~FXVTKDirectivityPlot(void);


public:
	enum _AutosetPoleValueMethod {
		// APVM_MINIMUM_ALL, APVM_MAXIMUM_ALL, APVM_MEDIAN_ALL, APVM_MINIMUM_BORDERED, APVM_MAXIMUM_BORDERED, 
		APVM_MEDIAN_BORDERED, APVM_MINUS_RADIUS ,APVM_ZERO ,APVM_NONE
	};

/*
	enum _AutosetRadiusoffsetScalefactorMethod {
//		ARSM_MIN_TO_VALUE,
//		ARSM_MIN_ABS_TO_VALUE,
		ARSM_MAX_TO_VALUE,
		ARSM_MAX_ABS_TO_VALUE,
//		ARSM_MEDIAN_ABS_TO_VALUE,
//		ARSM_LIMITMIN_AT_VALUE,
//		ARSM_LIMITMAX_AT_VALUE,
//		ARSM_LIMIT_ABS_AT_VALUE,
		ARSM_NONE
	};
*/


public:
	/// Get name of class of the object
	virtual const char* GetClassname();

	// Set the data source (DAFF content object)
	virtual void SetData(DAFFContentMS* pContent);
	virtual void SetData(DAFFContentDFT* pContent);

	//int GetSelectedFrequency() const;
	void SetSelectedFrequency(int iIndex);

	/// Set radius for max value (same as SetScale(...))
	inline void SetSphereRadius(float radius) {
		SetScale(radius);
	}

	void SetValueRangeLin(float min, float max);
	float* GetValueRangeLin();
	void SetValueRangeLog(float min, float max);
	float* GetValueRangeLog();

	/// Switch to logarithmic deformation
	void SetLogModeOn();
	/// Switch to linear deformation
	void SetLogModeOff();
	/// Return true, if logarithmic deformation is used
	bool IsLogModeOn();



	/// Set/get value if northpole or southpole
	void SetNorthpoleValue(float value);
	void SetSouthpoleValue(float value);
	float GetNorthpoleValue();
	float GetSouthpoleValue();

	// methods in ALPHA-phase
	void SetAutosetNorthpoleValueMethod(_AutosetPoleValueMethod method);
	void SetAutosetSouthpoleValueMethod(_AutosetPoleValueMethod method);
	void SetAutosetPoleValueMethod(_AutosetPoleValueMethod method);
	_AutosetPoleValueMethod GetAutosetNorthpoleValueMethod();
	_AutosetPoleValueMethod GetAutosetSouthpoleValueMethod();

	/*
	// methods in ALPHA-phase
	void SetAutosetRadiusoffsetScalefactorMethod(_AutosetRadiusoffsetScalefactorMethod method);
	void SetAutosetRadiusoffsetScalefactorMethod(_AutosetRadiusoffsetScalefactorMethod method, float MethodValue);
	_AutosetRadiusoffsetScalefactorMethod GetAutosetRadiusoffsetScalefactorMethod();
	void SetAutosetRadiusoffsetScalefactorMethodValue(float MethodValue);
	float GetAutosetRadiusoffsetScalefactorMethodValue();
	*/

private:
	void Initialization();
	virtual void _update();

private:
	virtual void SetData(std::vector<std::vector<float>> *DataToCopy);
	virtual bool SetData(std::vector<std::vector<std::vector<float>>> *DataToCopy, unsigned int index);
	virtual void SetData(float *floatarray, unsigned int Dim0Size, unsigned int Dim1Size);
	virtual void ClearData();

	float SphereRadius;
	float ValueRangeLin[2];
	float ValueRangeLog[2];
	bool LogModeIsOn;

	// float RadiusoffsetScalefactor;
	float NorthpoleValue;
	float SouthpoleValue;
	// float AutosetRadiusoffsetScalefactorMethodValue;
	_AutosetPoleValueMethod AutosetNorthpoleMethod;
	_AutosetPoleValueMethod AutosetSouthpoleMethod;
	/*
	_AutosetRadiusoffsetScalefactorMethod AutosetRadiusoffsetScalefactorMethod;
	ROUNDINGMODE AutosetRadiusoffsetScalefactorManipulation;
	*/

	DAFFContentMS* m_pContentMS;
	DAFFContentDFT* m_pContentDFT;
	std::vector<std::vector<std::vector<float>>> m_data;
};
