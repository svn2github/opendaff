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
 *    File:			FXVTK2Line.h
 *
 *    Purpose:		Creates a plane
 *
 *    Autors:		Jonas Stienen (stienen@akustik.rwth-aachen.de)
 *
 *  ---------------------------------------------------------------------------
 *
 */
// $Id: FXVTK2Plane.h 2333 2012-03-05 14:21:39Z stienen $

#ifndef __FXVTK2_PLANE_H__
#define __FXVTK2_PLANE_H__

#include <FXVTK2SGNode.h>

// Vorwärtsdeklarationen
class vtkActor;
class vtkPlaneSource;
class vtkPolyDataMapper;

namespace FXVTK2 {

//! Simple plane object node [TODO Jonas!!]
/**
 * This class derived from the scene graph node class creates a plane from VTK.
 */

class Plane : public FXVTK2::SGNode {
public:
	Plane();
	Plane(double x1, double y1, double z1, double x2, double y2, double z2, double n1, double n2, double n3);
	Plane(FXVTK2::SGNode* pParentNode, double x1, double y1, double z1, double x2, double y2, double z2, double n1, double n2, double n3);
	~Plane();
	
	// --= object related methods =--
	// TODO Jonas

	//void SetLengthX(const double dLength);
	//double GetLengthX() const;

	//void SetLengthY(const double dLength);
	//double GetLengthY() const;


	// --= general methods =--

	//! Color getter
	void GetColor(double& r, double& g, double& b);

	//! Color setter
	void SetColor(const double r, const double g, const double b);

	//! Alpha getter
	double GetAlpha() const;

	//! Alpha setter
	void SetAlpha(const double a);

	//! Visibility setter
	void SetVisible(const bool bVisible);

	//! Visibility getter
	bool IsVisible() const;

private:
	vtkPlaneSource*		m_pSource;
	vtkPolyDataMapper*  m_pMapper;
	vtkActor*			m_pActor;

	// The initializer generates dynamic objects like source, mapper, actor ...
	void init();
};

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_PLANE_H__