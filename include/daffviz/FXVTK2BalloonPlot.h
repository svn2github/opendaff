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
   |  Purpose:  Creates a BalloonPlot Plot									|
   |  Authors:  Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)		|
   |            Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)		    |
   |																		|
   +------------------------------------------------------------------------+
*/
// $Id: $

#ifndef __FXVTK2_DIRECTIVITY_H__
#define __FXVTK2_DIRECTIVITY_H__

#include <FXVTK2SGNode.h>

// Forward declarations
class vtkActor;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkWarpScalar;
class vtkDoubleArray;
class vtkVectorText;

class DAFFContent;

namespace FXVTK2 {

//! Simple directivity object node
/**
 * This class derived from the scene graph node class creates a directivity.
 */

class BalloonPlot : public FXVTK2::SGNode {
public:
	enum {
		SCALING_LINEAR=0,
		SCALING_DECIBEL
	};

	enum {
		MODE_SURFACE=0,
		MODE_WIREFRAME,
		MODE_POINT
	};

	BalloonPlot(SGNode* pParentNode, const DAFFContent* pContent);
	BalloonPlot(const DAFFContent* pContent);
	virtual ~BalloonPlot();

	// --= Object related methods =--

	//! Returns the currently selected frequency index
	int GetSelectedFrequency() const;

	//! Sets the currently selected frequency index
	void SetSelectedFrequency(int iFreqIndex);

	//! Get scaling (SCALING_LINEAR | SCALING_DECIBEL)
	int GetScaling() const;

	//! Set scaling (SCALING_LINEAR | SCALING_DECIBEL)
	void SetScaling(int iScaling);

	//! Set minimum of data range (in dB or absolute values depending on currently used scaling)
	double GetRangeMin() const;
	
	//! Set minimum of data range (in dB or absolute values depending on currently used scaling)
	double GetRangeMax() const;

	//! Set data range (in dB or absolute values depending on currently used scaling)
	void SetRange(double dMin, double dMax);

	//! Set whether scalars should be used to color the model
	void SetScalarVisibility(bool bVisible);

	//! Get whether scalars should be used to color the model
	int GetScalarVisibility();

	//! Set display mode (MODE_SURFACE | MODE_WIREFRAME | MODE_POINT)
	void SetDisplayMode(int iMode);

	//! Set active channel
	void SetChannel(int iChannel);

	//! get active channel
	int GetChannel();

	//! Enable/Disable warping (WARP_DISABLE | WARP_ENABLE)
	// \note default: enabled
	void EnableWarp();
	void DisableWarp();

	//! set whether the phase should be used for coloring. If not, the magnitude will be used.
	void SetUsePhaseAsColor(bool bUse);
	
	//! Set probe angles
	void SetProbeAngles(const double dAlpha, const double dBeta);

	//! Enables/disables probe
	void SetProbeVisible(const bool bVisible);

	//! Update probe nodes
	void UpdateProbe();
private:

	const DAFFContent* m_pContent;
	vtkWarpScalar* m_pWarp;
	vtkPolyDataMapper* m_pMapper;
	vtkActor* m_pPlotActor;
	vtkPolyData* m_pPlotPolydata;
	vtkDoubleArray* m_pNormals;
	int m_iFrequency;
	int m_iNumFrequencies;
	int m_iScaling;
	float m_dMin, m_dMax, m_dProbeAlpha, m_dProbeBeta; // linear factors!
	int m_iChannel;
	bool m_bWarp, m_bUsePhaseAsColor;
	vtkActor* m_pProbe;
	vtkVectorText *m_pProbeLabel;
	vtkActor* m_pLabel;

	// The initializer generates dynamic objects like source, mapper, actor ...
	void init();

	// Update scalars (e.g. when selected frequency changed)
	void SetScalars();

	// Convert orientation (phi, theta) into cartesian coordiates (x,y,z)
	void sph2cart(double phi, double theta, double& x, double& y, double& z);

	// Convert a linear value into decibel
	double factor2decibel(double x) const;
	
	// Convert decibel to linear value
	double decibel2factor(double x) const;
};

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_DIRECTIVITY_H__
