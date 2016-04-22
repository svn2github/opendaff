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

#ifndef IW_DAFF_SPHERICAL_COORDINATE_ASSISTANT
#define IW_DAFF_SPHERICAL_COORDINATE_ASSISTANT

#include <daffviz/DAFFVizSGNode.h>

// Forward declarations
class vtkActor;
class vtkFollower;
class vtkSphereSource;

namespace DAFFViz
{

	class Label;

	//! Spherical coordinate assistant node
	/**
	 * This class derived from the scene graph node class creates axes, arrows and so on
	 * for a better discovery of spherical objects
	 */

	class DAFF_API SphericalCoordinateAssistant : public SGNode
	{
	public:
		//! Constructor
		SphericalCoordinateAssistant(double dMin=0, double dMax=1, double dPrec=.1, double dScaleRes=36);

		//! Desctructor
		~SphericalCoordinateAssistant();

		// --= object related methods =--

		//! Enables/Disables all graphical assistance
		void SetAssistanceVisible(const bool bVisible);

		//! Enables/Disables axes
		void SetAxesVisible(const bool bVisible);

		//! Enables/Disables view/up vector
		void SetViewUpVectorsVisible(const bool bVisible);

		//! Enables/Disables meridians
		void SetMeridiansVisible(const bool bVisible);

		//! Enables/Disables equator
		void SetEquatorVisible(const bool bVisible);

		//! Enables/Disables grid lines
		void SetGridVisible(const bool bVisible);

		//! Set reference level
		void SetReferenceLevel(const double dLevel);
	
		//! Set reference opacity
		void SetReferenceOpacity(const double dOpacity);

		// --= general methods =--

		//! Set visibility
		void SetVisible(const bool bVisible);
	
		bool GetGridVisible() const;
		bool GetAxesVisible() const;
		bool GetViewUpVectorsVisible() const;
		bool GetVisible() const;

		// --= grid get/set methods =--
		void SetMin(double);
		void SetMax(double);
		void SetRResolution(double);
		void SetPhiResolution(double);
		double GetMin() const;
		double GetMax() const;
		double GetRResolution() const;
		double GetPhiResolution() const;
		void UpdateGrid();
		void UpdateReference();

	private:
		virtual void OnSetFollowerCamera(vtkCamera* pCamera);

		SGNode* m_pViewUpVectors;
		SGNode* m_pPoles;
		std::vector<vtkActor*> m_pMeridians;
		std::vector<vtkActor*> m_pGrid;
		std::vector<vtkFollower*> m_pCircles;
		std::vector<vtkFollower*> m_pLabels;
		vtkActor* m_pEquator;
		SGNode* m_pAngleAxes;
		double m_dMin, m_dMax, m_dPrecision, m_dScaleRes, m_dReferenceLevel, m_dReferenceOpacity;
		vtkCamera* m_pCamera;
		vtkActor* m_pReferenceActor;
		vtkSphereSource* m_pSphere;

		//! Initialize objects
		void init();
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_SPHERICAL_COORDINATE_ASSISTANT
