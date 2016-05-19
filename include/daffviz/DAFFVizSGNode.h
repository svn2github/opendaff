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

#ifndef IW_DAFF_NODE
#define IW_DAFF_NODE

// Includes
#include <DAFFDefs.h>
#include <vector>

// VTK includes
#include <vtkSmartPointer.h>

// Forward declarations
class vtkActor;
class vtkAssembly;
class vtkCamera;

namespace DAFFViz
{

	//! Base class for scene graph nodes
	/**
	 * This class defines the base class for all nodes within an DAFFViz scene graph.
	 * It implements basic functionality, like the scene graph tree and operations.
	 * Internally it projects the DAFFViz scene graph onto a matching VTK scene assembly
	 * making it possible to use VTK objects in the classical scene graph way.
	 * DAFFViz includes subclasses for common objects (cube, cylinder, sphere and so on).
	 * But the SGNode design is flexible to incorporate any VTK object into the DAFFViz hierarchy.
	 *
	 * The tree operations include modification of the scene graph (adding/removing child nodes,
	 * traversal, etc.). Moreover each node implements a common set of transformation operations
	 * (translation, rotation, scaling), which is applicable to all node implementations.
	 *
	 * The usage of a node in your tree structure has strict rules. 
	 * Any scene graph node may only be member of one parent scene graph. Therefore
	 * each scene graph node has a defined identity and may not be cloned/copied during runtime.
	 *
	 * Any transformation on a node will treat the subtree (if node has child nodes) as a fixed
	 * single object. For instance you want to create a table, then you might want to join
	 * the desktop and the legs to form a simple table which then can be placed and rotated within
	 * the scene as one object.
	 * 
	 * Nodes that only group other nodes does not have material properties, e.g. no color. You have
	 * to colorize the leaf nodes which as a rule are derived objects of the SGNode class. Those 
	 * leaves implement the appearing objects like cubes, cylinders and so on. 
	 *
	 * If you want to use the scene graph tree, you normally do it like this:
	 * Create a scene root node. Add children like primitive objects or create your own node by
	 * copying from the provided classes and fabricate any geometric that is possible with VTK.
	 * You can also group DAFFViz primitive  objects to form a more complicated construction and append this
	 * to your scene graph root or any subtree. At the very end you will apply the scene graph
	 * root to your DAFFViz::QtDAFFWidget which will render your own scene and display it in your Qt application.
	 *
	 */

	class DAFF_API SGNode
	{
	public:
		//! Standard constructor
		SGNode();

		//! Linking constructor
		/** 
		 * Generates a node and connects it with the given parent node
		 *
		 * @param [in] pParentNode Parent scene graph node of this child
		 */
		SGNode( DAFFViz::SGNode* pParentNode );

		//! Destructor
		/**
		 * @note This destructor automatically frees all objects within its subtree.
		 */
		virtual ~SGNode();

		// --= Node functionality =--

		//! Traverses up to the root (recursively) and returns the node pointer
		/**
		 * @return Root scene graph node pointer
		 */
		DAFFViz::SGNode* GetRootNode() const;

		//! Returns the parent node pointer
		/**
		* @return Parent scene graph node pointer, NULL if root node
		* @see HasParentNode()
		*/
		DAFFViz::SGNode* GetParentNode() const;

		//! Returns true if the node is appended to a parent node
		/**
		 * @return True, if node has a parent (is not a root node)
		 */
		bool HasParentNode() const;

		//! Returns true if the node is a root node (... has no parent node)
		/**
		* @return True, if node is a root node (has no parent)
		*/
		bool IsRoot() const;

		//! Returns true if the node combines one or more child nodes
		/**
		* @return True, if node has child nodes (is not a leaf node)
		*/
		bool HasChildNodes() const;

		//! Returns true if the node appears as a leaf node (... has no child nodes)
		/**
		* @return True, if node is a leaf nodes (has no child nodes)
		*/
		bool IsLeaf() const;

		//! Returns the pointer to the child node of given index (for const correctness)
		/**
		 * @param [in] iIndex Index number of child node
		 * @return Const pointer to child node
		 */
		const DAFFViz::SGNode* GetChildNode(int iIndex) const;

		//! Returns the pointer to the child node of given index
		/**
		* @param [in] iIndex Index number of child node
		* @return Pointer to child node
		*/
		DAFFViz::SGNode* GetChildNode(int iIndex);

		//! Returns all the pointers to the child nodes (for const correctness,  clears vector if no childs available)
		/**
		* @param [out] vpChildren Vector of const pointers to child nodes
		*/
		void GetChildNodes(std::vector<const DAFFViz::SGNode*>& vpChildren) const;

		//! Returns all the pointers to the child nodes (clears vector if no childs available)
		/**
		* @param [out] vpChildren Vector of pointers to child nodes
		*/
		void GetChildNodes( std::vector<DAFFViz::SGNode*>& vpChildren );

		//! Appends a single child node
		/**
		 * Will return true if successful, false if the given Child is not a root node.
		 *
		 * @param [in] pChild Adds a child to this node
		 * @return True, if possible, false, if child node already has a parent (is not a root node)
		 *
		 * @see isRoot() hasParentNode()
		 */
		bool AddChildNode(DAFFViz::SGNode* pChild);

		//! Appends a vector child nodes
		/**
		 * Will return true if successful, false if any given Child is not a root node, though
		 * it will continue appending the other given nodes.
		 *
		 * @param [in] vpChildren Adds the children to this node
		 * @return True, if possible for all, false, if any child node already has a parent (is not a root node)
		 *
		 * @see isRoot() hasParentNode()
		 */
		bool AddChildNodes(const std::vector<DAFFViz::SGNode*>& vpChildren);

		//! Removes a single child node
		/**
		 * Will return true if successful, false if the given child node is non existent.
		 *
		 * @param [in] pChild Child node to be removed
		 * @return True on success, false if child is not existent.
		 */
		bool RemoveChildNode(DAFFViz::SGNode* pChild);

		//! Removes a vector of child nodes
		/**
		 * Will return true if successful, false if any given child node is non existent, though
		 * it will continue removing the other given nodes.
		 *
		 * @param [in] vpChildren Vector of child nodes to be removed
		 * @return True on success for all, false if any child is not existent.
		 */
		bool RemoveChildNodes(const std::vector<DAFFViz::SGNode*>& vpChildren);


		// --= Modificators =--

		//! Position setter (unit is meter)
		/**
		 * @param [out] x X axis value
		 * @param [out] y Y axis value
		 * @param [out] z Z axis value
		 *
		 * @see SetPosition()
		 */
		void GetPosition(double& x, double& y, double& z) const;
	
		//! Position getter (unit is meter)
		/**
		* @param [in] x X axis value
		* @param [in] y Y axis value
		* @param [in] z Z axis value
		*
		* @see GetPosition()
		*/
		void SetPosition(double x, double y, double z);

		//! Orientation getter (angles in [&deg;])
		/**
		 * Corresponds to euler angles with first rotation around z axis, then x, then y. Orientations are right-handed.
		 *
		 * @param [out] dRotXDeg X axis rotation value
		 * @param [out] dRotYDeg Y axis rotation value
		 * @param [out] dRotZDeg Z axis rotation value
		 *
		 * @see SetOrientation()
		 */
		void GetOrientation(double& dRotXDeg, double& dRotYDeg, double& dRotZDeg) const;

		//! Sets the orientation of the node assembly based on world coordinate axis (angles in [&deg;])
		/**
		 * Will first rotate around z axis, then x, then y. Orientations are right-handed.
		 *
		 * @param [in] dRotXDeg X axis rotation value
		 * @param [in] dRotYDeg Y axis rotation value
		 * @param [in] dRotZDeg Z axis rotation value
		 *
		 * @see GetOrientation()
		 */
		void SetOrientation(double dRotXDeg, double dRotYDeg, double dRotZDeg);

		//! Sets the orientation of the node assembly based on yaw, pitch and roll angles around own coordinate axis (angles in [&deg;])
		/**
		 * Will first rotate around -z axis (roll), then x (pitch), then y (yaw)
		 *
		 * @param [in] dYawDeg Yaw rotation value (first rotation around Y)
		 * @param [in] dPitchDeg Pitch rotation value (first rotation around X)
		 * @param [in] dRollDeg Roll rotation value (first rotation around -Z)
		 *
		 * @see GetOrientationYPR()
		 */
		void SetOrientationYPR(double dYawDeg, double dPitchDeg, double dRollDeg);

		//! Orientation getter of the node assembly based on yaw, pitch and roll angles around own coordinate axis (angles in [&deg;])
		/**
		* Will first rotate around -z axis (roll), then x (pitch), then y (yaw)
		*
		*
		* @param[out] dYawDeg Yaw rotation value(first rotation around Y)
		* @param[out] dPitchDeg Pitch rotation value(first rotation around X)
		* @param[out] dRollDeg Roll rotation value(first rotation around - Z)
		*
		* @see SetOrientationYPR()
		*/
		void GetOrientationYPR(double& dYawDeg, double& dPitchDeg, double& dRollDeg);

		//! Sets the orientation of the node assembly based on view- and up-vectors
		/**
		 * @param [in] vx X axis view vector value
		 * @param [in] vy Y axis view vector value
		 * @param [in] vz Z axis view vector value
		 * @param [in] ux X axis up vector value
		 * @param [in] uy Y axis up vector value
		 * @param [in] uz Z axis up vector value
		 *
		 * @see GetOrientationVU()
		 */
		void SetOrientationVU(double vx, double vy, double vz,
							  double ux, double uy, double uz);

		//! Returns the orientation of the node assembly based on view- and up-vectors
		/**
		 * @param [out] vx X axis view vector value
		 * @param [out] vy Y axis view vector value
		 * @param [out] vz Z axis view vector value
		 * @param [out] ux X axis up vector value
		 * @param [out] uy Y axis up vector value
		 * @param [out] uz Z axis up vector value
		 *
		 * @see SetOrientationVU()
		 */
		void GetOrientationVU(double& vx, double& vy, double& vz,
							  double& ux, double& uy, double& uz);

		//! Scale getter
		/**
		 * @param [out] sx X axis scale value
		 * @param [out] sy Y axis scale value
		 * @param [out] sz Z axis scale value
		 *
		 * @see SetScale()
		 */
		void GetScale(double& sx, double& sy, double& sz) const;
	
		//! Scale setter
		/**
		 * @param [in] sx X axis scale value
		 * @param [in] sy Y axis scale value
		 * @param [in] sz Z axis scale value
		 *
		 * @see SetScale()
		 */
		void SetScale(double sx, double sy, double sz);

		//! Set visibility
		/**
		 * The method will traverse the subtree and set the visibility of all nodes.
		 * If you want to hide a leaf node or a subtree, you have to manually hide it afterwards.
		 *
		 *  @param [in] bVisible True for visible, false for hidden
		 *
		 * @see IsVisible()
		 */
		virtual void SetVisible( bool bVisible);

		//! Returns true if the node is visible
		/**
		 * If this node is visible, the method will return true. Note, that this does not actually
		 * mean, that the child nodes are visible, too.
		 *
		 *  @return True if visible, false if hidden
		 *
		 * @see SetVisible()
		 */
		virtual bool IsVisible() const;

	protected:

		// --= Modificators for subclasses =--

		//! Add a VTK actor to the node
		/**
		 * Will assert if the pointer is NULL in debug mode.
		 *
		 * @param [in] pActor Pointer to VTK actor
		 */
		void AddActor( vtkSmartPointer< vtkActor > pActor );
	
		//! Remove a VTK actor from the assembly of the node
		/**
		 * Will assert if the pointer is NULL in debug mode.
		 *
		 * @param [in] pActor Pointer to VTK actor
		 */
		void RemoveActor( vtkSmartPointer< vtkActor > pActor );
	
		//! Add a VTK assembly to the node
		/**
		 * Will assert if the pointer is NULL in debug mode.
		 *
		 * @param [in] pAssembly Pointer to VTK assembly
		 */
		void AddAssembly( vtkSmartPointer< vtkAssembly > pAssembly );

		//! Release an assembly from the assembly of the node
		/**
		 * Will assert if the pointer is NULL in debug mode.
		 *
		 * @param [in] pAssembly Pointer to VTK assembly
		 */
		void RemoveAssembly( vtkSmartPointer< vtkAssembly > pAssembly );

		// --= Event handlers =--

		//! Set active camera for followers
		/** 
		 *
		 * @param [in] pCamera VTK camera followers shall face
		 */
		virtual void OnSetFollowerCamera( vtkSmartPointer< vtkCamera > pCamera );

	private:
		DAFFViz::SGNode*				m_pParentNode;		//!@ Parent scene graph node (NULL => root node)
		std::vector< DAFFViz::SGNode* >	m_vpChildNodes;		//!@ Scene graph child nodes list
		vtkSmartPointer< vtkAssembly >	m_pNodeAssembly;	//!@ Internal VTK assembly of the node

		bool _debug; //!@ Debug switch for internal use only

		// --= Uncopyable =--

		//! [Deny for all] Copy constructor
		inline SGNode( const SGNode& ) {};

		//! [Deny for all] Assignment operator
		inline SGNode& operator=( const SGNode& ) { return *this; };

		//! [Only friends] Returns the assembly of this node
		/**
		 * @return VTK assembly pointer
		 */
		vtkSmartPointer< vtkAssembly > GetNodeAssembly();
	
		// The display class needs access to the node assembly
		friend class VTKDAFFVizWindow;
		friend class QtDAFFWidget;
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_NODE
