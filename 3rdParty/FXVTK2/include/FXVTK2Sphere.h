/*
 * ---------------------------------------------------------------------------
 *
 *							FXVTK2 Visualization Library
 *             (c) Copyright Institute of Technical Acoustics (ITA)
 *              RWTH Aachen University, Aachen, Germany, 2011-today
 *
 * ---------------------------------------------------------------------------
 *						    ____  __________  _______
 *						   //  / //__   ___/ //  _   |
 *						  //  /    //  /    //  /_|  |
 *						 //  /    //  /    //  ___   |
 *						//__/    //__/    //__/   |__|
 *
 * ---------------------------------------------------------------------------
 *
 *    File:			FXVTK2Sphere.h
 *
 *    Purpose:		Creates a sphere
 *
 *    Autors:		Jonas Stienen (stienen@akustik.rwth-aachen.de)
 *
 *  ---------------------------------------------------------------------------
 *
 */
// $Id: FXVTK2Sphere.h 1722 2011-04-05 08:37:29Z fwefers $

#ifndef __FXVTK2_SPHERE_H__
#define __FXVTK2_SPHERE_H__

#include <FXVTK2SGNode.h>

// Forward declaration
class vtkActor;
class vtkSphereSource;
class vtkPolyDataMapper;

namespace FXVTK2 {

//! Simple sphere object node
/**
 * This class derived from the scene graph node class creates a sphere from VTK.
 */

class Sphere : public FXVTK2::SGNode {
public:
	Sphere();
	Sphere(double dRadius, int iPhiResolution, int iThetaResolution);
	Sphere(FXVTK2::SGNode* pParentNode, double dRadius, int iPhiResolution, int iThetaResolution);
	~Sphere();

	// --= object related methods =--

	double GetRadius() const;
	void SetRadius(double dRadius);

	int GetPhiResolution() const;
	void SetPhiResolution(int iResolution);
	
	int GetThetaResolution() const;
	void SetThetaResolution(int iResolution);


	// --= general methods =--

	//! Color getter
	void GetColor(double& r, double& g, double& b);

	//! Color setter
	void SetColor(const double r, const double g, const double b);

	//! Alpha getter
	double GetAlpha() const;

	//! Alpha setter
	void SetAlpha(const double a);

	//! Set visibility
	void SetVisible(const bool bVisible);

	//! Get visibility
	bool IsVisible() const;

private:
	vtkSphereSource*	m_pSource;
	vtkPolyDataMapper*  m_pMapper;
	vtkActor*			m_pActor;

	// The initializer generates dynamic objects like source, mapper, actor ...
	void init();
};

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_SPHERE_H__