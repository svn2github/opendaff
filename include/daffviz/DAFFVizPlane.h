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

#ifndef IW_DAFF_PLANE
#define IW_DAFF_PLANE

#include <daffviz/DAFFVizSGNode.h>

// Vorwärtsdeklarationen
class vtkActor;
class vtkPlaneSource;
class vtkPolyDataMapper;

namespace DAFFViz
{

	//! Simple plane object node [TODO Jonas!!]
	/**
	 * This class derived from the scene graph node class creates a plane from VTK.
	 */

	class DAFF_API Plane : public DAFFViz::SGNode {
	public:
		Plane();
		Plane(double x1, double y1, double z1, double x2, double y2, double z2, double n1, double n2, double n3);
		Plane(DAFFViz::SGNode* pParentNode, double x1, double y1, double z1, double x2, double y2, double z2, double n1, double n2, double n3);
		~Plane();
	
		// --= object related methods =--
		// TODO Jonas

		//void SetLengthX(double dLength);
		//double GetLengthX() const;

		//void SetLengthY(double dLength);
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
		void SetVisible(bool bVisible);

		//! Visibility getter
		bool IsVisible() const;

	private:
		vtkPlaneSource*		m_pSource;
		vtkPolyDataMapper*  m_pMapper;
		vtkActor*			m_pActor;

		// The initializer generates dynamic objects like source, mapper, actor ...
		void init();
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_PLANE