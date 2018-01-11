/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016-2018 Institute of Technical Acoustics (ITA), RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef IW_DAFF_SPHERE
#define IW_DAFF_SPHERE

#include <daffviz/DAFFVizSGNode.h>

// Forward declaration
class vtkActor;
class vtkSphereSource;
class vtkPolyDataMapper;

namespace DAFFViz
{

	//! Simple sphere object node
	/**
	 * This class derived from the scene graph node class creates a sphere from VTK.
	 */

	class DAFF_API Sphere : public DAFFViz::SGNode
	{
	public:
		Sphere();
		Sphere(double dRadius, int iPhiResolution, int iThetaResolution);
		Sphere(DAFFViz::SGNode* pParentNode, double dRadius, int iPhiResolution, int iThetaResolution);
		virtual ~Sphere();

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
		void SetVisible(bool bVisible);

		//! Get visibility
		bool IsVisible() const;

	private:
		vtkSmartPointer< vtkSphereSource >	m_pSource;
		vtkSmartPointer< vtkPolyDataMapper >  m_pMapper;
		vtkSmartPointer< vtkActor >			m_pActor;

		// The initializer generates dynamic objects like source, mapper, actor ...
		void init();
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_SPHERE