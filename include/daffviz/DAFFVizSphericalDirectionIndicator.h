/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef IW_DAFF_SPHERICAL_DIRECTION_INDICATOR
#define IW_DAFF_SPHERICAL_DIRECTION_INDICATOR

#include <daffviz/DAFFVizSGNode.h>
#include <DAFF.h>

// Forward declaration
class vtkActor;
class vtkPolyDataMapper;

namespace DAFFViz
{
	class Arrow;
	class Plane;

	//! Direction indicator for spherical objects
	/**
	 * This class derived from the scene graph node class creates a direction indicator in a spherical context
	 */

	class DAFF_API SphericalDirectionIndicator : public DAFFViz::SGNode
	{
	public:
		SphericalDirectionIndicator( SGNode* = NULL );
		inline ~SphericalDirectionIndicator() {};

		//! Sets the direction in object view angles
		void SetDirectionDeg( double dPhiDeg, double dThetaDeg );

		//! Sets the phi direction in object view angles
		void SetDirectionPhiDeg( double dPhiDeg );

		//! Sets the theta direction in object view angles
		void SetDirectionThetaDeg( double dThetaDeg );

		//! Color getter
		void GetColor( double& r, double& g, double& b );

		//! Color setter
		void SetColor( double r, double g, double b );

		//! Alpha getter
		double GetAlpha() const;

		//! Alpha setter
		void SetAlpha( double a );

	private:
		Arrow*  m_pDirectionArrow;
		DAFFSCTransform m_oTransform;
		double m_dPhiDeg;
		double m_dThetaDeg;
		double m_dAlphaDeg;
		double m_dBetaDeg;

		//! The initializer generates dynamic objects like source, mapper, actor ...
		void Init();
		void UpdateOrientation();
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_SPHERICAL_DIRECTION_INDICATOR