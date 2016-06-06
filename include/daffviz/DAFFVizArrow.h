/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
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

namespace DAFFViz
{

	//! Simple arrow object node
	/**
	 * This class derived from the scene graph node class creates an arrow from VTK.
	 */

	class DAFF_API Arrow : public DAFFViz::SGNode
	{
	public:
		Arrow();
		Arrow( double dTipLength, double dTipRadius, int iTipResolution, double dShaftRadius, int iShaftResolution );
		Arrow( DAFFViz::SGNode* pParentNode, double dTipLength, double dTipRadius, int iTipResolution, double dShaftRadius, int iShaftResolution );
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
		void SetColor( double r, double g, double b);

		//! Alpha getter
		double GetAlpha() const;

		//! Alpha setter
		void SetAlpha( double a);

		//! Set visibility
		void SetVisible( bool bVisible);

		//! Get visibility
		bool IsVisible() const;

	private:
		vtkSmartPointer< vtkArrowSource >		m_pSource;
		vtkSmartPointer< vtkPolyDataMapper >	m_pMapper;
		vtkSmartPointer< vtkActor >				m_pActor;

		//! The initializer generates dynamic objects like source, mapper, actor ...
		void init();
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_ARROW