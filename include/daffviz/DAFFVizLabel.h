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

#ifndef IW_DAFF_LABEL
#define IW_DAFF_LABEL

#include <daffviz/DAFFVizSGNode.h>

#include <string>

// Forward declarations
class vtkActor;
class vtkCamera;
class vtkFollower;
class vtkPolyDataMapper;
class vtkVectorText;

namespace DAFFViz
{

	//! Simple arrow object node
	/**
	 * This class derived from the scene graph node class creates an arrow from VTK.
	 */

	class DAFF_API Label : public DAFFViz::SGNode {
	public:
		Label();
		Label(SGNode* pNode);
		Label(SGNode* pNode, std::string sText);
		~Label();


		// --= general methods =--

		//! Color getter
		void GetColor(double& r, double& g, double& b) const;

		//! Color setter
		void SetColor(const double r, const double g, const double b);

		//! Alpha getter
		double GetAlpha() const;

		//! Alpha setter
		void SetAlpha(const double a);

		//! Get visibility
		bool IsVisible() const;

		//! Set visibility
		void SetVisible(const bool bVisible);


		// --= object related methods =--

		//! Label text setter
		void SetText(const std::string& sText);

		//! Label text getter
		std::string GetText() const;

	private:
		vtkVectorText*		m_pLabelText;
		vtkPolyDataMapper*  m_pMapper;
		vtkFollower*		m_pFollower;
		vtkActor*			m_pActor;

		std::string			m_sText;

		// The initializer generates dynamic objects like source, mapper, actor ...
		void init();
	
		// --= object related methods =--

		virtual void OnSetFollowerCamera(vtkCamera* pCamera);

		friend class Frame;
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_LABEL