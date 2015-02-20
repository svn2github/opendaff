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
 *    File:			FXVTK2Label.h
 *
 *    Purpose:		Creates an text label for a node
 *
 *    Autors:		Jonas Stienen (stienen@akustik.rwth-aachen.de)
 *
 *  ---------------------------------------------------------------------------
 *
 */
// $Id: FXVTK2Arrow.h 1722 2011-04-05 08:37:29Z fwefers $

#ifndef __FXVTK2_LABEL_H__
#define __FXVTK2_LABEL_H__

#include <FXVTK2SGNode.h>

#include <string>

// Forward declarations
class vtkActor;
class vtkCamera;
class vtkFollower;
class vtkPolyDataMapper;
class vtkVectorText;

namespace FXVTK2 {

//! Simple arrow object node
/**
 * This class derived from the scene graph node class creates an arrow from VTK.
 */

class Label : public FXVTK2::SGNode {
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

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_LABEL_H__