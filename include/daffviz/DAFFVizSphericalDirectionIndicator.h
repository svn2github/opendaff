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
		SphericalDirectionIndicator();
		SphericalDirectionIndicator( SGNode* );
		~SphericalDirectionIndicator();

		//! Sets the direction in object view angles
		void SetDirectionObjectView( double dPhi, double dTheta );

		//! Sets the direction in data view angles
		void SetDirectionDataView( double dAlpha, double dBeta );

		//! Sets the orientation of the data object
		void SetOrientationOfData( const DAFFOrientationYPR& oOrientDeg );

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
		Plane* m_pDirectionCell;
		DAFFOrientationYPR m_oDataOrientation;

		//! The initializer generates dynamic objects like source, mapper, actor ...
		void init();
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_SPHERICAL_DIRECTION_INDICATOR