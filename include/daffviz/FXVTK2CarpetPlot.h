/*
   +------------------------------------------------------------------------+
   |																		|
   |  FXVTK2 Visualization Library											|
   |																		|
   |  (c) Copyright Institute of Technical Acoustics (ITA)					|
   |      Aachen University of Technology (RWTH), 2010						|
   |																		|
   +------------------------------------------------------------------------+
   |																		|
   |  File:     FXVTK2BalloonPlot.h											|
   |  Purpose:  Creates a Carpet Plot										|
   |  Authors:  Hendrik Vorwerk (Hendrik.Vorwerk@akustik.rwth-aachen.de)	|
   |            Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)		    |
   |																		|
   +------------------------------------------------------------------------+
*/
// $Id: $

#ifndef __FXVTK2_CARPETPLOT_H__
#define __FXVTK2_CARPETPLOT_H__

#include <FXVTK2SGNode.h>

// Forward declarations
class vtkActor;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkWarpScalar;
class vtkVectorText;

class DAFFContentIR;

namespace FXVTK2 {

//! Simple directivity object node
/**
 * This class derived from the scene graph node class creates a directivity.
 */

class CarpetPlot : public FXVTK2::SGNode {
public:
	enum {
		SCALING_LINEAR=0,
		SCALING_DECIBEL
	};
	enum {
		BETA_FIXED=0,
		ALPHA_FIXED
	};
	enum {
		MODE_SURFACE=0,
		MODE_WIREFRAME,
		MODE_POINT
	};


	CarpetPlot(const DAFFContentIR* pContentIR);
	virtual ~CarpetPlot();

	// --= Object related methods =--

	//! Returns the currently selected angle in degree (interval [-90� 90�])
	float GetSelectedAngle() const;

	//! Sets the currently selected angle in degree
	void SetSelectedAngle(float fAngle);

	//! Get scaling (SCALING_LINEAR | SCALING_DECIBEL)
	int GetScaling() const;

	//! Set scaling (SCALING_LINEAR | SCALING_DECIBEL)
	void SetScaling(int iScaling);

	//! Set fixed angle (ALPHA_FIXED | BETA_FIXED)
	void SetFixedAngle(int iFixedAngle);

	//! Get fixed angle (ALPHA_FIXED | BETA_FIXED)
	int getFixedAngle();

	//! Set whether scalars should be used to color the model
	void SetScalarVisibility(bool bVisible);

	//! Get whether scalars should be used to color the model
	int getScalarVisibility();

	//! Set display mode (MODE_SURFACE | MODE_WIREFRAME | MODE_POINT)
	void SetDisplayMode(int iMode);

	//! Set minimum of data range (in dB or absolute values depending on currently used scaling)
	double GetRangeMin() const;
	
	//! Set minimum of data range (in dB or absolute values depending on currently used scaling)
	double GetRangeMax() const;

	//! Set data range (in dB or absolute values depending on currently used scaling)
	void SetRange(double dMin, double dMax);

	//! Set active channel
	void SetChannel(int iChannel);

	//! get active channel
	int GetChannel();

	//! Enable/Disable warping (WARP_DISABLE | WARP_ENABLE)
	// \note default: enabled
	void EnableWarp();
	void DisableWarp();
	
	//! Set probe angles
	void SetProbeAngles(const double dAlpha, const double dBeta);

	//! Enables/disables probe
	void SetProbeVisible(const bool bVisible);

	//! Update probe nodes
	void UpdateProbe();

private:

	const DAFFContentIR* m_pContentIR;
	vtkWarpScalar* m_pWarp;
	vtkPolyDataMapper* m_pMapper;
	vtkActor* m_pPlotActor;
	vtkPolyData* m_pPlotPolydata;
	float m_fAngle;
	int m_iScaling;
	int m_iFixedAngle;
	float m_dMin, m_dMax; // linear factors!
	int m_iChannel;
	bool m_bWarp;
	vtkActor* m_pProbe;
	vtkVectorText *m_pProbeLabel;
	vtkActor* m_pLabel;
	float m_dProbeX, m_dProbeY;

	// The initializer generates dynamic objects like source, mapper, actor ...
	void init();

	// Update scalars (e.g. when selected angle changed)
	void SetScalars();

	// Convert a linear value into decibel
	double factor2decibel(double x) const;
	
	// Convert decibel to linear value
	double decibel2factor(double x) const;

	// moves the whole plot to fit to the coordinate axes
	void updatePlotOffset();
};

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_CARPETPLOT_H__
