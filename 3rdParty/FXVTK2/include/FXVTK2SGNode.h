/*
   +------------------------------------------------------------------------+
   |																		|
   |  FXVTK2 Visualization Library											|
   |																		|
   |  (c) Copyright Institute of Technical Acoustics (ITA)					|
   |      Aachen University of Technology (RWTH), 2010						|
   |																		|
   +------------------------------------------------------------------------+
   |																		|
   |  File:     FXVTK2SGNode.h												|
   |  Purpose:  Scene graph node base class									|
   |  Authors:  Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de			|
   |            Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)		|
   |																		|
   +------------------------------------------------------------------------+
*/
// $Id: FXVTK2SGNode.h 1756 2011-04-08 12:59:26Z stienen $

#ifndef __FXVTK2_NODE_H__
#define __FXVTK2_NODE_H__

// Includes
#include <vector>

// Forward declarations
class vtkActor;
class vtkAssembly;
class vtkCamera;

namespace FXVTK2 {

//! Base class for scene graph nodes
/**
 * This class defines the base class for all nodes within an FXVTK2 scene graph.
 * It implements basic functionality, like the scene graph tree and operations.
 * Internally it projects the FXVTK2 scene graph onto a matching VTK scene assembly
 * making it possible to use VTK objects in the classical scene graph way.
 * FXVTK2 includes subclasses for common objects (cube, cylinder, sphere and so on).
 * But the SGNode design is flexible to incorporate any VTK object into the FXVTK2 hierarchy.
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
 * You can also group FXVTK2 primitive  objects to form a more complicated construction and append this
 * to your scene graph root or any subtree. At the very end you will apply the scene graph
 * root to your FXVTK2::Frame which will render your own scene and display it in your FOX application.
 *
 */

class SGNode {
public:
	//! Standard constructor
	SGNode();

	//! Linking constructor
	/** 
	 * Generates a node and connects it with the given parent node
	 */
	SGNode(FXVTK2::SGNode* pParentNode);

	//! Destructor
	/**
	 * \note This destructor automatically frees all objects within its subtree.
	 */
	virtual ~SGNode();

	// --= Node functionality =--

	//! Traverses up to the root (recursively) and returns the node pointer
	FXVTK2::SGNode* GetRootNode() const;

	//! Returns the parent node pointer
	FXVTK2::SGNode* GetParentNode() const;

	//! Returns true if the node is appended to a parent node
	bool HasParentNode() const;

	//! Returns true if the node is a root node (... has no parent node)
	bool IsRoot() const;

	//! Returns true if the node combines one or more child nodes
	bool HasChildNodes() const;

	//! Returns true if the node appears as a leaf node (... has no child nodes)
	bool IsLeaf() const;

	//! Returns the pointer to the child node of given index (for const correctness)
	const FXVTK2::SGNode* GetChildNode(int iIndex) const;

	//! Returns the pointer to the child node of given index
	FXVTK2::SGNode* GetChildNode(int iIndex);

	//! Returns all the pointers to the child nodes (for const correctness,  clears vector if no childs available)
	void GetChildNodes(std::vector<const FXVTK2::SGNode*>& vpChildren) const;

	//! Returns all the pointers to the child nodes (clears vector if no childs available)
	void GetChildNodes(std::vector<FXVTK2::SGNode*>& vpChildren);

	//! Appends a single child node
	/**
	 * Will return true if successful, false if the given Child is not a root node.
	 */
	bool AddChildNode(FXVTK2::SGNode* pChild);

	//! Appends a vector child nodes
	/**
	 * Will return true if successful, false if any given Child is not a root node, though
	 * it will continue appending the other given nodes.
	 */
	bool AddChildNodes(const std::vector<FXVTK2::SGNode*>& vpChildren);

	//! Removes a single child node
	/**
	 * Will return true if successful, false if the given child node is non existent.
	 */
	bool RemoveChildNode(FXVTK2::SGNode* pChild);

	//! Removes a vector of child nodes
	/**
	 * Will return true if successful, false if any given child node is non existent, though
	 * it will continue removing the other given nodes.
	 */
	bool RemoveChildNodes(const std::vector<FXVTK2::SGNode*>& vpChildren);


	// --= Modificators =--

	//! Position setter
	void GetPosition(double& x, double& y, double& z) const;
	
	//! Position getter
	void SetPosition(double x, double y, double z);

	//! Orientation getter (angles in [�])
	void GetOrientation(double& rotx_deg, double& roty_deg, double& rotz_deg) const;

	//! Sets the orientation of the node assembly based on world coordinate axis (angles in [�])
	/**
	 * Will first rotate around z axis, then x, then y. Orientations are right-handed.
	 */
	void SetOrientation(double rotx_deg, double roty_deg, double rotz_deg);

	//! Sets the orientation of the node assembly based on yaw, pitch and roll angles around own coordinate axis (angles in [�])
	/**
	 * Will first rotate around -z axis (roll), then x (pitch), then y (yaw)
	 */
	void SetOrientationYPR(double yaw, double pitch, double roll);

	//! Oorientation getter of the node assembly based on yaw, pitch and roll angles around own coordinate axis (angles in [�])
	void GetOrientationYPR(double& yaw, double& pitch, double& roll);

	//! Sets the orientation of the node assembly based on view- and up-vectors
	void SetOrientationVU(double vx, double vy, double vz,
		                  double ux, double uy, double uz);

	//! Returns the orientation of the node assembly based on view- and up-vectors
	void GetOrientationVU(double& vx, double& vy, double& vz,
		                  double& ux, double& uy, double& uz);

	//! Scale getter
	void GetScale(double& sx, double& sy, double& sz) const;
	
	//! Scale setter
	void SetScale(double sx, double sy, double sz);

	//! Set visibility
	/**
	 * The method will traverse the subtree and set the visibility of all nodes.
	 * If you want to hide a leaf node or a subtree, you have to manually hide it afterwards.
	 */
	virtual void SetVisible(const bool bVisible);

	//! Returns true if the node is visible
	/**
	 * If this node is visible, the method will return true. Note, that this does not actually
	 * mean, that the child nodes are visible, too.
	 */
	virtual bool IsVisible() const;

protected:

	// --= Modificators for subclasses =--

	//! Add a VTK actor to the node
	/**
	 * Will assert if the pointer is NULL in debug mode.
	 */
	void AddActor(vtkActor* pActor);
	
	//! Remove a VTK actor from the assembly of the node
	/**
	 * Will assert if the pointer is NULL in debug mode.
	 */
	void RemoveActor(vtkActor* pActor);
	
	//! Add a VTK assembly to the node
	/**
	 * Will assert if the pointer is NULL in debug mode.
	 */
	void AddAssembly(vtkAssembly* pAssembly);

	//! Release an assembly from the assembly of the node
	/**
	 * Will assert if the pointer is NULL in debug mode.
	 */
	void RemoveAssembly(vtkAssembly* pAssembly);

	// --= Event handlers =--

	//! Set active camera for followers
	/** 
	 * ...
	 */
	virtual void OnSetFollowerCamera(vtkCamera* pCamera);

private:
	FXVTK2::SGNode*				 m_pParentNode;		// Parent scene graph node (NULL => root node)
	std::vector<FXVTK2::SGNode*> m_vpChildNodes;	// Scene graph child nodes list
	vtkAssembly*				 m_pNodeAssembly;	// Internal VTK assembly of the node

	bool _debug;

	// --= Uncopyable =--

	// [Deny for all] Copy constructor
	SGNode(const SGNode&) {}

	// [Deny for all] Assignment operator
	SGNode& operator=(const SGNode&) { return *this; }

	// [Only friends] Returns the assembly of this node
	vtkAssembly* GetNodeAssembly();
	
	// The display class needs access to the node assembly
	friend class Frame;
};

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_NODE_H__