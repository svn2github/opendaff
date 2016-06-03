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

#ifndef IW_DAFF_DIRECTIVITY
#define IW_DAFF_DIRECTIVITY

#include <daffviz/DAFFVizSGNode.h>

#include <vtkSmartPointer.h>

// Forward declarations
class vtkActor;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkWarpScalar;
class vtkDoubleArray;
class vtkVectorText;

class DAFFContent;

namespace DAFFViz
{

	//! Simple directivity object node
	/**
	 * This class derived from the scene graph node class creates a directivity.
	 */

	class DAFF_API BalloonPlot : public DAFFViz::SGNode
	{
	public:
		enum
		{
			SCALING_LINEAR=0,
			SCALING_DECIBEL
		};

		enum
		{
			MODE_SURFACE=0,
			MODE_WIREFRAME,
			MODE_POINT
		};

		BalloonPlot( SGNode* pParentNode, const DAFFContent* pContent );
		BalloonPlot( const DAFFContent* pContent );
		virtual ~BalloonPlot();

		// --= Object related methods =--

		//! Returns the currently selected frequency index
		int GetSelectedFrequency() const;

		//! Sets the currently selected frequency index
		void SetSelectedFrequency( int iFreqIndex );

		//! Get scaling (SCALING_LINEAR | SCALING_DECIBEL)
		int GetScaling() const;

		//! Set scaling (SCALING_LINEAR | SCALING_DECIBEL)
		void SetScaling( int iScaling );

		//! Set minimum of data range (in dB or absolute values depending on currently used scaling)
		double GetRangeMin() const;
	
		//! Set minimum of data range (in dB or absolute values depending on currently used scaling)
		double GetRangeMax() const;

		//! Set data range (in dB or absolute values depending on currently used scaling)
		void SetRange( double dMin, double dMax );

		//! Set whether scalars should be used to color the model
		void SetScalarVisibility( bool bVisible );

		//! Get whether scalars should be used to color the model
		int GetScalarVisibility();

		//! Set display mode (MODE_SURFACE | MODE_WIREFRAME | MODE_POINT)
		void SetDisplayMode( int iMode );

		//! Set active channel
		void SetChannel( int iChannel );

		//! get active channel
		int GetChannel();

		//! Enable/Disable warping (WARP_DISABLE | WARP_ENABLE)
		// \note default: enabled
		void EnableWarp();
		void DisableWarp();

		//! set whether the phase should be used for coloring. If not, the magnitude will be used.
		void SetUsePhaseAsColor( bool bUse );
	
		//! Set probe angles
		void SetProbeAngles( double dAlpha, double dBeta );

		//! Enables/disables probe
		void SetProbeVisible( bool bVisible );

		//! Update probe nodes
		void UpdateProbe();
	private:

		const DAFFContent* m_pContent;
		vtkSmartPointer< vtkWarpScalar > m_pWarp;
		vtkSmartPointer< vtkPolyDataMapper > m_pMapper;
		vtkSmartPointer< vtkActor > m_pPlotActor;
		vtkSmartPointer< vtkPolyData > m_pPlotPolydata;
		vtkSmartPointer< vtkDoubleArray > m_pNormals;
		int m_iFrequency;
		int m_iNumFrequencies;
		int m_iScaling;
		float m_dMin, m_dMax, m_dProbeAlpha, m_dProbeBeta; // linear factors!
		int m_iChannel;
		bool m_bWarp, m_bUsePhaseAsColor;
		vtkSmartPointer< vtkActor > m_pProbe;
		vtkSmartPointer< vtkVectorText > m_pProbeLabel;
		vtkSmartPointer< vtkActor > m_pLabel;

		// The initializer generates dynamic objects like source, mapper, actor ...
		void init();

		// Update scalars (e.g. when selected frequency changed)
		void SetScalars();

		// Convert orientation (phi, theta) into cartesian coordiates (x,y,z)
		void sph2cart( double phi, double theta, double& x, double& y, double& z );

		// Convert a linear value into decibel
		float FactorToDecibel( float x ) const;
	
		// Convert decibel to linear value
		float DecibelToFactor( float x ) const;
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_DIRECTIVITY
