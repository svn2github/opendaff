
#include <daffviz/DAFFVizSGNode.h>

#include <daffviz/DAFFVizGlobalLock.h>

#include <cassert>
#include <math.h>
#include <cmath>

// VTK includes
#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>

namespace DAFFViz
{

	static float PI_F = std::acos(-1.0f);

	double grad2rad(double dAngleGrad) {
		return dAngleGrad * PI_F / 180.0F;
	}

	SGNode::SGNode()
		: m_pParentNode( NULL )
	{
		m_pNodeAssembly = vtkSmartPointer< vtkAssembly >::New();
	}

	SGNode::SGNode( DAFFViz::SGNode* pParentNode )
		: m_pParentNode( NULL )
	{
		m_pNodeAssembly = vtkSmartPointer< vtkAssembly >::New();

		assert( pParentNode != NULL );
		if( pParentNode )
			pParentNode->AddChildNode( this );
	}

	SGNode::~SGNode()
	{
		// Disconnect from parent node
		if( m_pParentNode )
			m_pParentNode->RemoveChildNode( this );

		// Delete own subtree
		// Important: delete has side-effects on the vector itself. Therefore we use the while construct.
		//while (!m_vpChildNodes.empty()) delete m_vpChildNodes.front();
		while( !m_vpChildNodes.empty() )
			delete m_vpChildNodes.front();

		// Remove the node assembly
		DAFFVIZ_LOCK_VTK;
		/*
		 *  Very important: All subclasses of SGNode MUST remove their own actors in their destructors
		 *                  If everything went fine, the node assembly is empty here. Otherwise some
		 *                  subclass of SGNode forgot to tidy up. If we would delete the node assembly
		 *                  regardless the fact that it is not empty, we get in trouble (crash).
		 *                  We leave an assertion here, so that programmers are aware of that problem...
		 *					... If you run into that assertion, remind yourself to always RemovePart()
		 *					all the vtkActors in your derived class.
		 */
		assert( m_pNodeAssembly->GetNumberOfPaths() == 0 );
		//m_pNodeAssembly->GlobalWarningDisplayOff();
		//m_pNodeAssembly->Delete(); // smart pointer deletes ...
		DAFFVIZ_UNLOCK_VTK;
	}

	DAFFViz::SGNode* SGNode::GetRootNode() const
	{
		// Recursively traverse to root node
		return ( m_pParentNode ? m_pParentNode->GetRootNode() : NULL );
	}

	DAFFViz::SGNode* SGNode::GetParentNode() const
	{
		return m_pParentNode;
	}

	bool SGNode::HasParentNode() const
	{
		return (m_pParentNode != NULL);
	}

	bool SGNode::IsRoot() const
	{
		return ( m_pParentNode == NULL );
	}

	bool SGNode::HasChildNodes() const
	{
		return ( !m_vpChildNodes.empty() );
	}

	bool SGNode::IsLeaf() const
	{
		return m_vpChildNodes.empty();
	}

	const DAFFViz::SGNode* SGNode::GetChildNode(int iIndex) const
	{
		assert( ( iIndex > 0 ) && ( iIndex < ( int ) m_vpChildNodes.size() ) );

		if( ( iIndex > 0 ) && ( iIndex < ( int ) m_vpChildNodes.size() ) )
			return m_vpChildNodes[ iIndex ];
		else
			return NULL;
	}

	DAFFViz::SGNode* SGNode::GetChildNode( int iIndex )
	{
		assert( ( iIndex > 0 ) && ( iIndex < ( int ) m_vpChildNodes.size() ) );

		if( ( iIndex > 0 ) && ( iIndex < ( int ) m_vpChildNodes.size() ) )
			return m_vpChildNodes[ iIndex ];
		else
			return NULL;
	}

	void SGNode::GetChildNodes( std::vector<const DAFFViz::SGNode*>& vpChildren ) const
	{
		if (m_vpChildNodes.empty()) {
			vpChildren.clear();
			return;
		}

		vpChildren.resize( m_vpChildNodes.size() );
		for (size_t i=0; i<m_vpChildNodes.size(); i++)
			vpChildren[i] = m_vpChildNodes[i];
	}

	void SGNode::GetChildNodes(std::vector<DAFFViz::SGNode*>& vpChildren) {
		if (m_vpChildNodes.empty()) {
			vpChildren.clear();
			return;
		}

		vpChildren.resize( m_vpChildNodes.size() );
		for (size_t i=0; i<m_vpChildNodes.size(); i++)
			vpChildren[i] = m_vpChildNodes[i];
	}

	bool SGNode::AddChildNode(DAFFViz::SGNode* pChild) {
		/*
		 *  Wichtig: Der Knoten darf nur dann hinzugefügt werden, falls er nicht
		 *           bereits Unterknoten in einem anderen (Teil)baum ist. Dies
		 *           stellt sicher, das jeder Knoten höchstens in einem Teilbaum
		 *           enthalten sein ist. Der Test erfolgt über die Wurzel.
		 */

		assert( pChild != NULL );
		if (pChild == NULL) return false;

		// Ausschließen das man einen Knoten zu sich selbst verknüpft
		assert( pChild != this );
		if (pChild == this) return false;

		assert( pChild->IsRoot() );
		if (!pChild->IsRoot()) return false;

		// Intern den Unterbaum verknüpfen
		m_vpChildNodes.push_back(pChild);

		// Beim Kinderknoten den Vaterknoten vermerken
		pChild->m_pParentNode = this;

		// Assemblies verknüpfen
		DAFFVIZ_LOCK_VTK;
		m_pNodeAssembly->AddPart( pChild->m_pNodeAssembly );
		DAFFVIZ_UNLOCK_VTK;

		return true;
	}

	bool SGNode::AddChildNodes(const std::vector<DAFFViz::SGNode*>& vpChildren) {
		bool bResult = true;
		for (std::vector<DAFFViz::SGNode*>::const_iterator cit=vpChildren.begin(); cit!=vpChildren.end(); ++cit)
			bResult &= AddChildNode(*cit);
		return bResult;
	}

	bool SGNode::RemoveChildNode(DAFFViz::SGNode* pChild) {
		for (std::vector<DAFFViz::SGNode*>::iterator it=m_vpChildNodes.begin(); it!=m_vpChildNodes.end(); ++it)
			if ((*it) == pChild) {
				// Assemblies lösen
				DAFFVIZ_LOCK_VTK;
				m_pNodeAssembly->RemovePart( pChild->m_pNodeAssembly );
				DAFFVIZ_UNLOCK_VTK;

				// Beim Kinderknoten die Vaterschaft entfernen
				pChild->m_pParentNode = NULL;

				// Kinderknoten aus der Liste löschen
				m_vpChildNodes.erase(it);

				return true;
			}

		return false;
	}

	bool SGNode::RemoveChildNodes(const std::vector<DAFFViz::SGNode*>& vpChildren) {
		bool bResult = true;
		for( std::vector<DAFFViz::SGNode*>::const_iterator cit = vpChildren.begin(); cit != vpChildren.end(); ++cit )
			bResult &= RemoveChildNode( *cit );
		return bResult;
	}

	void SGNode::GetPosition( double& x, double& y, double& z ) const
	{
		double* data = m_pNodeAssembly->GetPosition();
		x = data[0];
		y = data[1];
		z = data[2];
	}

	void SGNode::SetPosition( double x, double y, double z )
	{
		DAFFVIZ_LOCK_VTK;
		// [fwe 2011-11-10] Speicherleck hier drin?
		m_pNodeAssembly->SetPosition(x, y, z);
		DAFFVIZ_UNLOCK_VTK;
	}

	void SGNode::GetOrientation( double& rotx_deg, double& roty_deg, double& rotz_deg ) const
	{
		double* data = m_pNodeAssembly->GetOrientation();
		rotx_deg = data[0];
		roty_deg = data[1];
		rotz_deg = data[2];
	}

	void SGNode::SetOrientation( double rotx_deg, double roty_deg, double rotz_deg )
	{
		DAFFVIZ_LOCK_VTK;
		// [fwe 2011-11-10] Speicherleck hier drin?
		m_pNodeAssembly->SetOrientation( rotx_deg, roty_deg, rotz_deg );
		DAFFVIZ_UNLOCK_VTK;
	}

	void SGNode::SetOrientationYPR( double yaw, double pitch, double roll )
	{
	
		double* pos = m_pNodeAssembly->GetPosition();
		double r = grad2rad(roll);
		double p = grad2rad(pitch);
		double y = grad2rad(yaw);

		// Translation matrix: to origin
		vtkSmartPointer< vtkMatrix4x4 > mOrigin = vtkSmartPointer< vtkMatrix4x4 >::New();
		mOrigin->Identity();
		mOrigin->SetElement(0,3, -pos[0]);
		mOrigin->SetElement(1,3, -pos[1]);
		mOrigin->SetElement(2,3, -pos[2]);

		// Translation matrix: to object position
		vtkSmartPointer< vtkMatrix4x4 > mPosition = vtkSmartPointer< vtkMatrix4x4 >::New();
		mPosition->Identity();
		mPosition->SetElement(0,3, pos[0]);
		mPosition->SetElement(1,3, pos[1]);
		mPosition->SetElement(2,3, pos[2]);

		/*
		 *  Roll: homogenous rotation matrix for -Z axis
		 *
		 *
		 *  cos(alpha)   sin(alpha)      0       0
		 *  -sin(alpha)  cos(alpha)      0       0
		 *      0            0           1       0
		 *      0            0           0       1
		 */
		vtkSmartPointer< vtkMatrix4x4 > mRoll = vtkSmartPointer< vtkMatrix4x4 >::New();
		mRoll->Identity();
		mRoll->SetElement(0,0, cos(r));
		mRoll->SetElement(0,1, sin(r));
		mRoll->SetElement(1,0, -sin(r));
		mRoll->SetElement(1,1, cos(r));

		/*
		 *  Pitch: homogenous rotation matrix for +X axis
		 *
		 *      1            0            0           0
		 *      0       cos(alpha)  -sin(alpha)       0
		 *      0       sin(alpha)   cos(alpha)       0
		 *      0            0            0           1
		 */
		vtkSmartPointer< vtkMatrix4x4 > mPitch = vtkSmartPointer< vtkMatrix4x4 >::New();
		mPitch->Identity();
		mPitch->SetElement(1,1, cos(p));
		mPitch->SetElement(1,2, -sin(p));
		mPitch->SetElement(2,1, sin(p));
		mPitch->SetElement(2,2, cos(p));

		/*
		 *  Yaw: homogenous rotation matrix for +Y axis
		 *
		 *   cos(alpha)      0       sin(alpha)       0
		 *      0            1            0           0
		 *  -sin(alpha)      0       cos(alpha)       0
		 *      0            0            0           1
		 */
		vtkSmartPointer< vtkMatrix4x4 > mYaw = vtkSmartPointer< vtkMatrix4x4 >::New();
		mYaw->Identity();
		mYaw->SetElement(0,0, cos(y));
		mYaw->SetElement(0,2, sin(y));
		mYaw->SetElement(2,0, -sin(y));
		mYaw->SetElement(2,2, cos(y));


		// Compose transforms
		// (Order: Origin, Roll, Pitch, Yaw, Position)
		vtkSmartPointer< vtkTransform > transform = vtkSmartPointer< vtkTransform >::New();
		transform->Concatenate(mPosition);
		transform->Concatenate(mYaw);
		transform->Concatenate(mPitch);
		transform->Concatenate(mRoll);
		transform->Concatenate(mOrigin);

		// Apply new transformation
		DAFFVIZ_LOCK_VTK;
		m_pNodeAssembly->SetUserTransform( transform );
		DAFFVIZ_UNLOCK_VTK;

		return;
	}

	void SGNode::SetOrientationVU(double vx, double vy, double vz,
								  double ux, double uy, double uz)
	{
		double* pos = m_pNodeAssembly->GetPosition();

		// Translation matrix: to origin
		vtkSmartPointer< vtkMatrix4x4 > mOrigin = vtkSmartPointer< vtkMatrix4x4 >::New();
		mOrigin->Identity();
		mOrigin->SetElement(0,3, -pos[0]);
		mOrigin->SetElement(1,3, -pos[1]);
		mOrigin->SetElement(2,3, -pos[2]);

		// Translation matrix: to object position
		vtkSmartPointer< vtkMatrix4x4 > mPosition = vtkSmartPointer< vtkMatrix4x4 >::New();
		mPosition->Identity();
		mPosition->SetElement(0,3, pos[0]);
		mPosition->SetElement(1,3, pos[1]);
		mPosition->SetElement(2,3, pos[2]);

		// Cross product
		double zx = uz*vy - uy*vz;
		double zy = ux*vz - uz*vx;
		double zz = uy*vx - ux*vy;

		vtkSmartPointer< vtkMatrix4x4 > mViewUp = vtkSmartPointer< vtkMatrix4x4 >::New();
		mViewUp->Identity();
		mViewUp->SetElement(0, 0,  zx);
		mViewUp->SetElement(1, 0,  zy);
		mViewUp->SetElement(2, 0,  zz);
		mViewUp->SetElement(0, 1,  ux);
		mViewUp->SetElement(1, 1,  uy);
		mViewUp->SetElement(2, 1,  uz);
		mViewUp->SetElement(0, 2, -vx);
		mViewUp->SetElement(1, 2, -vy);
		mViewUp->SetElement(2, 2, -vz);

		// Compose transforms
		// (Order: Origin, Roll, Pitch, Yaw, Position)
		vtkSmartPointer< vtkTransform > transform = vtkSmartPointer< vtkTransform >::New();
		transform->Concatenate(mPosition);
		transform->Concatenate(mViewUp);
		transform->Concatenate(mOrigin);

		// Apply new transformation
		DAFFVIZ_LOCK_VTK;
		m_pNodeAssembly->SetUserTransform(transform);
		DAFFVIZ_UNLOCK_VTK;

		return;
	}


	void SGNode::GetScale( double& sx, double& sy, double& sz ) const
	{
		double* data = m_pNodeAssembly->GetScale();
		sx = data[0];
		sy = data[1];
		sz = data[2];
	}

	void SGNode::SetScale( double sx, double sy, double sz )
	{
		DAFFVIZ_LOCK_VTK;
		m_pNodeAssembly->SetScale( sx, sy, sz );
		DAFFVIZ_UNLOCK_VTK;
	}

	bool SGNode::IsVisible() const
	{
		return m_pNodeAssembly->GetVisibility() > 0 ? true : false;
	}

	void SGNode::SetVisible( bool bVisible )
	{
		// Traverse the subtree, lock has to be applied in child nodes, so no lock here!
		for( std::vector<SGNode*>::const_iterator cit = m_vpChildNodes.begin(); cit != m_vpChildNodes.end(); ++cit )
			( *cit )->SetVisible( bVisible );
	
		DAFFVIZ_LOCK_VTK;
		if( bVisible )
			m_pNodeAssembly->VisibilityOn();
		else
			m_pNodeAssembly->VisibilityOff();
		DAFFVIZ_UNLOCK_VTK;
	}

	void SGNode::AddActor( vtkSmartPointer< vtkActor > pActor )
	{
		assert ( pActor != NULL );

		DAFFVIZ_LOCK_VTK;
		m_pNodeAssembly->AddPart( pActor );
		DAFFVIZ_UNLOCK_VTK;
	}

	void SGNode::RemoveActor( vtkSmartPointer< vtkActor > pActor )
	{
		assert ( pActor != NULL );
	
		DAFFVIZ_LOCK_VTK;
		m_pNodeAssembly->RemovePart( pActor );
		//m_pNodeAssembly->
		DAFFVIZ_UNLOCK_VTK;
	}

	void SGNode::AddAssembly( vtkSmartPointer< vtkAssembly > pAssembly )
	{
		assert ( pAssembly != NULL );

		DAFFVIZ_LOCK_VTK;
		m_pNodeAssembly->AddPart( pAssembly );
		DAFFVIZ_UNLOCK_VTK;
	}

	void SGNode::RemoveAssembly( vtkSmartPointer< vtkAssembly> pAssembly )
	{
		assert ( pAssembly != NULL );

		DAFFVIZ_LOCK_VTK;
		m_pNodeAssembly->RemovePart( pAssembly );
		DAFFVIZ_UNLOCK_VTK;
	}

	void SGNode::OnSetFollowerCamera( vtkSmartPointer< vtkCamera > pCamera )
	{
		// Lock has to be applied in child nodes, so no lock here!
		for (std::vector<SGNode*>::const_iterator cit=m_vpChildNodes.begin(); cit!=m_vpChildNodes.end(); ++cit)
			(*cit)->OnSetFollowerCamera(pCamera);
	}

	vtkSmartPointer< vtkAssembly > SGNode::GetNodeAssembly()
	{
		return m_pNodeAssembly;
	}

} // End of namespace "DAFFViz"
