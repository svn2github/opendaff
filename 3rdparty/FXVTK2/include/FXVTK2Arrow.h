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
 *    File:			FXVTK2Arrow.h
 *
 *    Purpose:		Creates an arrow
 *
 *    Autors:		Jonas Stienen (stienen@akustik.rwth-aachen.de)
 *
 *  ---------------------------------------------------------------------------
 *
 */
// $Id: FXVTK2Arrow.h 1722 2011-04-05 08:37:29Z fwefers $

#ifndef __FXVTK2_ARROW_H__
#define __FXVTK2_ARROW_H__

#include <FXVTK2SGNode.h>

// Forward declarations
class vtkActor;
class vtkArrowSource;
class vtkPolyDataMapper;

namespace FXVTK2 {

//! Simple arrow object node
/**
 * This class derived from the scene graph node class creates an arrow from VTK.
 */

class Arrow : public FXVTK2::SGNode {
public:
	Arrow();
	Arrow(double dTipLength, double dTipRadius, int iTipResolution, double dShaftRadius, int iShaftResolution);
	Arrow(FXVTK2::SGNode* pParentNode, double dTipLength, double dTipRadius, int iTipResolution, double dShaftRadius, int iShaftResolution);
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

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_ARROW_H__