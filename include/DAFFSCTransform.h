/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016-2018 Institute of Technical Acoustics (ITA), RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef IW_DAFF_SC_TRANSFORM
#define IW_DAFF_SC_TRANSFORM

#include <DAFFDefs.h>

/**
 * This library-internal class realizes transformations of spherical coordinates.
 * It implements the rather complex rotation yaw-pitch-roll convention
 * expressed in spherical coordinates.
 */

class DAFF_API DAFFSCTransform
{
public:
	//! Constructor
	inline DAFFSCTransform() {};

	//! Constructor with initial orientation
	DAFFSCTransform( const DAFFOrientationYPR& orient );

	//! Return the OSC->DSC orientation (yaw-pitch-roll)
	void getOrientation( DAFFOrientationYPR& orient ) const;

	//! Sets the OSC->DSC orientation (yaw-pitch-roll)
	void setOrientation( const DAFFOrientationYPR& orient );

	//! Transform coordinates from OSC -> DSC (including coordinate system rotation)
	void transformOSC2DSC( float azimuth_in, float elevation_in,
		float& alpha_out, float& beta_out ) const;

	//! Transform coordinates from DSC -> OSC (including coordinate system rotation)
	void transformDSC2OSC( float alpha_in, float beta_in,
		float& azimuth_out, float& elevation_out ) const;

	//! Transform coordinates from OSC -> DSC (including coordinate system rotation)
	void transformOSC2DSC( double azimuth_in, double elevation_in,
		double& alpha_out, double& beta_out ) const;

	//! Transform coordinates from DSC -> OSC (including coordinate system rotation)
	void transformDSC2OSC( double alpha_in, double beta_in,
		double& azimuth_out, double& elevation_out ) const;

private:
	//! Cached trigenometric terms of yaw-pitch-roll angles
	class RotationConstants
	{
	public:
		double t1, t2, t3, t4, t5, t6, t7, t8, t9;

		//! Initialize constants (Angles in degrees [&deg;])
		void Init( double yaw, double pitch, double roll );
	};

	DAFFOrientationYPR m_orient;	//!@ Orientation
	RotationConstants m_const;		//!@ Rotation constants
	int m_orient_index;				//!@ Index of the orientation for simple cases, -1 otherwise
};

#endif // IW_DAFF_SC_TRANSFORM
