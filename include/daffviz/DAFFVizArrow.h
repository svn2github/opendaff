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

#ifndef IW_DAFF_ARROW
#define IW_DAFF_ARROW

#include <DAFFDefs.h>
#include <daffviz/DAFFVizSGNode.h>

// Forward declarations
class vtkActor;
class vtkArrowSource;
class vtkPolyDataMapper;

namespace DAFFViz {

//! Simple arrow object node
/**
 * This class derived from the scene graph node class creates an arrow from VTK.
 */

class DAFF_API Arrow : public DAFFViz::SGNode {
public:
	Arrow();
	Arrow(double dTipLength, double dTipRadius, int iTipResolution, double dShaftRadius, int iShaftResolution);
	Arrow(DAFFViz::SGNode* pParentNode, double dTipLength, double dTipRadius, int iTipResolution, double dShaftRadius, int iShaftResolution);
	~Arrow();

	// --= object related methods =--

	//! Tip length getter
	double GetTipLength() const;

	//! Tip length setter
	void SetTipLength(double dLength);

	//! Tip radius getter
	double GetTipRadius() const;

	//! Tip radius setter
	void SetTipRadius(double dRadius);

	//! Tip resolution getter
	int GetTipResolution() const;

	//! Tip resolution setter
	void SetTipResolution(int iResolution);

	//! Shaft radius getter
	double GetShaftRadius() const;

	//! Shaft radius setter
	void SetShaftRadius(double dRadius);

	//! Shaft resolution getter
	int GetShaftResolution() const;

	//! Shaft resolution setter
	void SetShaftResolution(int iResolution);


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
	vtkArrowSource*		m_pSource;
	vtkPolyDataMapper*  m_pMapper;
	vtkActor*			m_pActor;

	// The initializer generates dynamic objects like source, mapper, actor ...
	void init();
};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_ARROW