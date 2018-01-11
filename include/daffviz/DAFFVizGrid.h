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

#ifndef IW_DAFF_GRID
#define IW_DAFF_GRID

#include <daffviz/DAFFVizSGNode.h>

// Forward declarations
class vtkActor;

namespace DAFFViz
{

	//! Grid object node
	/**
	 * This class derived from the scene graph node class creates a wireframe Grid for usage in
	 * plots of Cartesian character. It is of size 1 times 1 in the XZ plane, and as defined will
	 * create sub cells in each direction, 10 as default
	 * 
	 * \note Created for the CartesianCoordinateAssistant used in SPAX application by Jonas Stienen.
	 */

	class DAFF_API Grid : public DAFFViz::SGNode
	{
	public:
		Grid( DAFFViz::SGNode* pParentNode, unsigned int iCellsX = 10, unsigned int iCellsZ = 10 );
		Grid( unsigned int iCellsX = 10, unsigned int iCellsZ = 10 );
		~Grid();

		//! Set visibility
		void SetVisible( bool bVisible );

		//! Get visibility
		bool IsVisible() const;
	
	private:
		vtkSmartPointer< vtkActor > m_pActorGrid;

		int m_iCellsX;
		int m_iCellsZ;

		// The initializer generates dynamic objects like source, mapper, actor ...
		void init();
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_GRID
