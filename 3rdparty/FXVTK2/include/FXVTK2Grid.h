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
 *    File:			FXVTK2Grid.h
 *
 *    Purpose:		Creates a grid for a plot
 *
 *    Autors:		Jonas Stienen (stienen@akustik.rwth-aachen.de)
 *
 *  ---------------------------------------------------------------------------
 *
 */
// $Id: FXVTK2Grid.h 1722 2011-04-05 08:37:29Z fwefers $

#ifndef __FXVTK2_GRID_H__
#define __FXVTK2_GRID_H__

#include <FXVTK2SGNode.h>

// Forward declarations
class vtkActor;

namespace FXVTK2 {

//! Grid object node
/**
 * This class derived from the scene graph node class creates a wireframe Grid for usage in
 * plots of Cartesian character. It is of size 1 times 1 in the XZ plane, and as defined will
 * create sub cells in each direction, 10 as default
 * 
 * \note Created for the CartesianCoordinateAssistant used in SPAX application by Jonas Stienen.
 */

class Grid : public FXVTK2::SGNode {
public:
	Grid(FXVTK2::SGNode* pParentNode, unsigned int iCellsX=10, unsigned int iCellsZ=10);
	Grid(unsigned int iCellsX=10, unsigned int iCellsZ=10);
	~Grid();

	//! Set visibility
	void SetVisible(const bool bVisible);

	//! Get visibility
	bool IsVisible() const;
	
private:
	vtkActor* m_pActorGrid;

	int m_iCellsX;
	int m_iCellsZ;

	// The initializer generates dynamic objects like source, mapper, actor ...
	void init();
};

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_GRID_H__