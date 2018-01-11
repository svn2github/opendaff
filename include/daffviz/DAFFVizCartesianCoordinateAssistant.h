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

#ifndef IW_DAFF_CARTESIAN_COORDINATE_ASSISTANT
#define IW_DAFF_CARTESIAN_COORDINATE_ASSISTANT

#include <daffviz/DAFFVizSGNode.h>
#include <daffviz/DAFFVizArrow.h>
#include <daffviz/DAFFVizGrid.h>
#include <daffviz/DAFFVizPlane.h>

// Forward declarations
class vtkActor;

namespace DAFFViz
{

	class Label;

	//! Cartesian coordinate assistant node
	/*
	 * This class derived from the scene graph node class creates axes, arrows and some other
	 * graphical assistance to support visualization of data using the Cartesian coordinate system.
	 *
	 */
	class DAFF_API CartesianCoordinateAssistant : public SGNode
	{
	public:
		CartesianCoordinateAssistant( SGNode* pParentNode, double LengthX = 1.0f, double LengthY = 1.0f, double LengthZ = 1.0f );
		CartesianCoordinateAssistant( double LengthX = 1.0f, double LengthY = 1.0f, double LengthZ = 1.0f );
		~CartesianCoordinateAssistant();


		// --= Object related methods =--

		//! Enables/Disables all graphical assistance
		//void setAssistanceVisible(bool bVisible);

		//! Set axes visibility
		bool GetAxesVisible() const;

		//! Enables/Disables axes
		void SetAxesVisible( bool bVisible );

		//! Set grid visibility
		bool GetGridVisible() const;

		//! Enables/Disables grid line visiblity
		void SetGridVisible( bool bVisible );

		//! Set grid visibility
		bool GetLabelsVisible() const;

		//! Enables/Disables grid line visiblity
		void SetLabelsVisible(bool bVisible);

		//! Set Length of axes arrows
		void SetAxesLength( double x, double y, double z );

		//! Get probe level
		double GetReferenceLevel() const;

		//! Set probe level
		void SetReferenceLevel( double dLevel );

		//! Get probe opacity
		double GetReferenceOpacity() const;

		//! Set probe opacity
		void SetReferenceOpacity( double dOpacity );


		// --= General methods =--
	
		//! Get visibility
		bool getVisible() const;

		//! Set visibility
		void SetVisible( bool bVisible );


		// --= Grid getter/setter =--

		//! Minimum value getter for X-axis
		double GetMinX() const;

		//! Minimum value setter for X-axis
		void SetMinX( double dMin );

		//! Maximum value getter for X-axis
		double GetMaxX() const;

		//! Maximum value setter for X-axis
		void SetMaxX( double dMax );

		//! Minimum value getter for Y-axis
		double GetMinY() const;

		//! Minimum value setter for Y-axis
		void SetMinY( double dMin );

		//! Maximum value getter for Y-axis
		double GetMaxY() const;

		//! Maximum value setter for Y-axis
		void SetMaxY( double dMax );

		//! Minimum value getter for Z-axis
		double GetMinZ() const;

		//! Minimum value setter for Z-axis
		void SetMinZ( double dMin );

		//! Maximum value getter for Z-axis
		double GetMaxZ() const;

		//! Maximum value setter for Z-axis
		void SetMaxZ( double dMax );

		//! Resolution getter for X-axis
		double GetResolutionX() const;

		//! Resolution setter for X-axis
		void SetResolutionX( double dRes );

		//! Resolution getter for Y-axis
		double GetResolutionY() const;

		//! Resolution setter for Y-axis
		void SetResolutionY( double dRes );

		//! Resolution getter for Z-axis
		double GetResolutionZ() const;

		//! Resolution setter for Z-axis
		void SetResolutionZ( double dRes );

		//! Offset level getter, at which value the XZ plane intersects with the Y-axis
		double GetOffsetY() const;

		//! Offset level setter, at which value the XZ plane intersects with the Y-axis
		void SetOffsetY( double dOffset );

		//! Update axes node
		void UpdateAxes();

		//! Update grid node
		void UpdateGrid();

		//! Update reference node
		void UpdateReference();

	private:
		std::vector< vtkSmartPointer< vtkActor > > m_pGrid;
		std::vector<vtkSmartPointer< vtkActor > > m_pLabels;
		std::vector< SGNode* > m_pAxes;
		vtkSmartPointer< vtkActor > m_pReferenceActor;
	
		double m_dMinX;
		double m_dMaxX;
		double m_dMinY;
		double m_dMaxY;
		double m_dMinZ;
		double m_dMaxZ;

		double m_dResolutionX;
		double m_dResolutionY;
		double m_dResolutionZ;

		double m_dLengthX;
		double m_dLengthY;
		double m_dLengthZ;

		double m_dOffsetY;

		double m_dReferenceLevel;
		double m_dReferenceAlpha;

		bool m_bAxesVisible;
		bool m_bGridVisible;
		bool m_bLabelsVisible;
		bool m_bReferenceVisible;

		//! Initialized all nodes
		void init();
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_CARTESIAN_COORDINATE_ASSISTANT
