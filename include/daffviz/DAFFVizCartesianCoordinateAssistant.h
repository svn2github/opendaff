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
		//! Constructor
		CartesianCoordinateAssistant(SGNode* pParentNode, double LengthX = 1.0f, double LengthY = 1.0f, double LengthZ = 1.0f);
	
		//! Constructor
		CartesianCoordinateAssistant(double LengthX = 1.0f, double LengthY = 1.0f, double LengthZ = 1.0f);

		//! Destructor
		~CartesianCoordinateAssistant();


		// --= Object related methods =--

		//! Enables/Disables all graphical assistance
		//void setAssistanceVisible(const bool bVisible);

		//! Set axes visibility
		bool GetAxesVisible() const;

		//! Enables/Disables axes
		void SetAxesVisible(const bool bVisible);

		//! Set grid visibility
		bool GetGridVisible() const;

		//! Enables/Disables grid line visiblity
		void SetGridVisible(const bool bVisible);

		//! Set grid visibility
		bool GetLabelsVisible() const;

		//! Enables/Disables grid line visiblity
		void SetLabelsVisible(const bool bVisible);

		//! Set Length of axes arrows
		void SetAxesLength(const double x, const double y, const double z);

		//! Get probe level
		double GetReferenceLevel() const;

		//! Set probe level
		void SetReferenceLevel(const double dLevel);

		//! Get probe opacity
		double GetReferenceOpacity() const;

		//! Set probe opacity
		void SetReferenceOpacity(const double dOpacity);


		// --= General methods =--
	
		//! Get visibility
		bool getVisible() const;

		//! Set visibility
		void SetVisible(const bool bVisible);


		// --= Grid getter/setter =--

		//! Minimum value getter for X-axis
		double GetMinX() const;

		//! Minimum value setter for X-axis
		void SetMinX(double);

		//! Maximum value getter for X-axis
		double GetMaxX() const;

		//! Maximum value setter for X-axis
		void SetMaxX(double);

		//! Minimum value getter for Y-axis
		double GetMinY() const;

		//! Minimum value setter for Y-axis
		void SetMinY(double);

		//! Maximum value getter for Y-axis
		double GetMaxY() const;

		//! Maximum value setter for Y-axis
		void SetMaxY(double);

		//! Minimum value getter for Z-axis
		double GetMinZ() const;

		//! Minimum value setter for Z-axis
		void SetMinZ(double);

		//! Maximum value getter for Z-axis
		double GetMaxZ() const;

		//! Maximum value setter for Z-axis
		void SetMaxZ(double);

		//! Resolution getter for X-axis
		double GetResolutionX() const;

		//! Resolution setter for X-axis
		void SetResolutionX(double);

		//! Resolution getter for Y-axis
		double GetResolutionY() const;

		//! Resolution setter for Y-axis
		void SetResolutionY(double);

		//! Resolution getter for Z-axis
		double GetResolutionZ() const;

		//! Resolution setter for Z-axis
		void SetResolutionZ(double);

		//! Offset level getter, at which value the XZ plane intersects with the Y-axis
		double GetOffsetY() const;

		//! Offset level setter, at which value the XZ plane intersects with the Y-axis
		void SetOffsetY(double); 

		//! Update axes node
		void UpdateAxes();

		//! Update grid node
		void UpdateGrid();

		//! Update reference node
		void UpdateReference();

	private:
		std::vector<vtkActor*> m_pGrid;
		std::vector<vtkActor*> m_pLabels;
		std::vector<SGNode*> m_pAxes;
		vtkActor* m_pReferenceActor;
	
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
