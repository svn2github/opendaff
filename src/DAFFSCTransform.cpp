#include <DAFFSCTransform.h>

#include "Utils.h"

#include <cmath>

DAFFSCTransform::DAFFSCTransform( const DAFFOrientationYPR& orient )
{
	setOrientation( orient );
}

void DAFFSCTransform::getOrientation( DAFFOrientationYPR& orient ) const
{
	orient = m_orient;
}

void DAFFSCTransform::setOrientation( const DAFFOrientationYPR& orient )
{
	m_orient = orient;
	m_orient_index = 0;
	m_const.init( m_orient.fYawAngleDeg, m_orient.fPitchAngleDeg, m_orient.fRollAngleDeg );
}

/* Transformation functions

	These transformations perform angular conversions between two spherical coordinate systems,
	which are rotated against each other by yaw-pitch-roll convention. So basically what we do here,
	is to express a yaw-pitch-roll rotation within spherical coordinates - this is not trivial.

	The formulas for the rotation can be derived by introducing a cartesian coordinate system
	(e.g. right-handed OpenGL, where Z is up) as an intermediate format. Then rotations are applied
	in the cartesian domainn. Finally we transform this result back into spherical coordinates.

	Many of the terms for the conversion only depend on the rotation angles itself (yaw, pitch and roll).
	Therefore we calculate these constants once and cache them for later reuse.

	For the angular calculations double precision floating points are necessary to maintain
	a low numerical error.


	Operation count:

	4 trigenometric (2 sin, 2 cos)
	2 muls (subexpressions)
	9 muls (calculation)
	6 adds (calculation)
	2 inverse trigenometric (1 atan2, 1 asin)
	-----------------------------------------
	Total: 6 trigenometric + 17 madds

	[Frank Wefers, 2010-02-01]

	*/

void DAFFSCTransform::transformOSC2DSC( float azimuth_in, float elevation_in, float& alpha_out, float& beta_out ) const
{

	// Internally in this method we use radians (not degrees)
	double ai = DAFF::grad2radf( azimuth_in ), ei = DAFF::grad2radf( elevation_in );
	double ao, eo;

	// Base terms
	double sa = sin( ai ), ca = cos( ai );
	double se = sin( ei ), ce = cos( ei );

	// Common subexpressions
	double sa_ce = sa*ce;
	double ca_ce = ca*ce;

	ao = atan2( m_const.t1*sa_ce - m_const.t2*se + m_const.t3*ca_ce,
		m_const.t4*sa_ce - m_const.t5*se + m_const.t6*ca_ce );
	eo = asin( m_const.t7*sa_ce + m_const.t8*se + m_const.t9*ca_ce );

	alpha_out = ( float ) DAFF::rad2grad( ao );
	beta_out = ( float ) DAFF::rad2grad( eo ) + 90.0F;	// Translate into DSC (b=e+90)
}

void DAFFSCTransform::transformDSC2OSC( float alpha_in, float beta_in, float& azimuth_out, float& elevation_out ) const
{
	// Internally in this method we use radians (not degrees)
	double ai = DAFF::grad2radf( alpha_in ), ei = DAFF::grad2radf( beta_in - 90.0F ); // Translate into OSC (e=b-90)
	double ao, eo;

	// Base terms
	double sa = sin( ai ), ca = cos( ai );
	double se = sin( ei ), ce = cos( ei );

	// Common subexpressions
	double sa_ce = sa*ce;
	double ca_ce = ca*ce;

	ao = atan2( m_const.t1*sa_ce + m_const.t7*se + m_const.t4*ca_ce,
		m_const.t3*sa_ce + m_const.t9*se + m_const.t6*ca_ce );
	eo = -asin( m_const.t2*sa_ce - m_const.t8*se + m_const.t5*ca_ce );

	azimuth_out = ( float ) DAFF::rad2grad( ao );
	elevation_out = ( float ) DAFF::rad2grad( eo );
}

void DAFFSCTransform::RotationConstants::init( double yaw, double pitch, double roll )
{

	double y = DAFF::grad2rad( yaw );
	double p = DAFF::grad2rad( pitch );
	double r = DAFF::grad2rad( roll );

	double sy = sin( y ), cy = cos( y );
	double sp = sin( p ), cp = cos( p );
	double sr = sin( r ), cr = cos( r );

	t1 = ( cy*cr - sy*sp*sr );
	t2 = ( cy*sr + sy*sp*cr );
	t3 = sy*cp;
	t4 = ( -sy*cr - cy*sp*sr );
	t5 = ( -sy*sr + cy*sp*cr );
	t6 = cy*cp;
	t7 = cp*sr;
	t8 = cp*cr;
	t9 = sp;
}
