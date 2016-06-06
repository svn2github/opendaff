/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0
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
	 * This class derived from the scene graph node class creates a text label from VTK.
	 */

	class DAFF_API Label : public DAFFViz::SGNode
	{
	public:
		Label();
		Label( SGNode* pNode );
		Label( SGNode* pNode, std::string sText );
		~Label();


		// --= general methods =--

		//! Color getter
		void GetColor( double& r, double& g, double& b ) const;

		//! Color setter
		void SetColor( double r,  double g,  double b );

		//! Alpha getter
		double GetAlpha() const;

		//! Alpha setter
		void SetAlpha( double a );

		//! Get visibility
		bool IsVisible() const;

		//! Set visibility
		void SetVisible( bool bVisible );


		// --= object related methods =--

		//! Label text setter
		void SetText( const std::string& sText );

		//! Label text getter
		std::string GetText() const;

	private:
		vtkSmartPointer< vtkVectorText >		m_pLabelText;
		vtkSmartPointer< vtkPolyDataMapper >  m_pMapper;
		vtkSmartPointer< vtkFollower >		m_pFollower;
		vtkSmartPointer< vtkActor >			m_pActor;

		std::string			m_sText;

		// The initializer generates dynamic objects like source, mapper, actor ...
		void init();
	
		// --= object related methods =--

		virtual void OnSetFollowerCamera( vtkSmartPointer< vtkCamera > pCamera );

		friend class Frame;
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_LABEL