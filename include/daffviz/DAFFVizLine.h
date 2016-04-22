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

#ifndef IW_DAFF_Line
#define IW_DAFF_Line

#include <daffviz/DAFFVizSGNode.h>

// Forward declarations
class vtkActor;
class vtkLineSource;
class vtkPolyDataMapper;

namespace DAFFViz
{

	//! Simple line object node
	/**
	 * This class derived from the scene graph node class creates a line from VTK.
	 */

	class DAFF_API Line : public DAFFViz::SGNode {
	public:
		Line();
		Line(double x1, double y1, double z1, double x2, double y2, double z2);
		Line(DAFFViz::SGNode* pParentNode, double x1, double y1, double z1, double x2, double y2, double z2);
		~Line();

		// --= object related methods =--

		void GetPoint1(double& x, double& y, double& z) const;
		void SetPoint1(double x, double y, double z);

		void GetPoint2(double& x, double& y, double& z) const;
		void SetPoint2(double x, double y, double z);


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
		vtkLineSource*		m_pSource;
		vtkPolyDataMapper*  m_pMapper;
		vtkActor*			m_pActor;

		// The initializer generates dynamic objects like source, mapper, actor ...
		void init();
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_Line