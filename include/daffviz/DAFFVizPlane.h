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

#ifndef __FXVTK2_PLANE_H__
#define __FXVTK2_PLANE_H__

#include <daffviz/DAFFVizSGNode.h>

// Vorwärtsdeklarationen
class vtkActor;
class vtkPlaneSource;
class vtkPolyDataMapper;

namespace DAFFViz {

//! Simple plane object node [TODO Jonas!!]
/**
 * This class derived from the scene graph node class creates a plane from VTK.
 */

class Plane : public DAFFViz::SGNode {
public:
	Plane();
	Plane(double x1, double y1, double z1, double x2, double y2, double z2, double n1, double n2, double n3);
	Plane(DAFFViz::SGNode* pParentNode, double x1, double y1, double z1, double x2, double y2, double z2, double n1, double n2, double n3);
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