/*
* -------------------------------------------------------------------------------------
*
*  OpenDAFF - A free, open source software package for directional audio data
*  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
*
*  Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
*
*  ------------------------------------------------------------------------------------
*
*/

#ifndef IW_DAFF_CARPETPLOT
#define IW_DAFF_CARPETPLOT

#include <daffviz/DAFFVizSGNode.h>

// Forward declarations
class vtkActor;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkWarpScalar;
class vtkVectorText;

class DAFFContentIR;

namespace DAFFViz
{
	//! Simple carpet plot object node
	/**
	 * This class derived from the scene graph node class creates a carpet plot visualization.
	 */

	class DAFF_API CarpetPlot : public DAFFViz::SGNode
	{
	public:
		enum
		{
			SCALING_LINEAR=0,
			SCALING_DECIBEL
		};

		enum
		{
			BETA_FIXED=0,
			ALPHA_FIXED
		};

		enum
		{
			MODE_SURFACE=0,
			MODE_WIREFRAME,
			MODE_POINT
		};

		CarpetPlot( const DAFFContentIR* pContentIR );
		virtual ~CarpetPlot();

		// --= Object related methods =--

		//! Returns the currently selected angle in degree (interval [-90° 90°])
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
		void SetProbeAngles(double dAlpha, double dBeta);

		//! Enables/disables probe
		void SetProbeVisible(bool bVisible);

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

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_CARPETPLOT
