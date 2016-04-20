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

#ifndef __FXVTK2_GLOBALLOCK_H__
#define __FXVTK2_GLOBALLOCK_H__

namespace DAFFViz {

//! Global lock for synchronization of VTK access
/**
 * When drawing a VTK scene by a render process, the mutual exclusion of access to the root node
 * is indispensable. Any change of the scene graph has to be suppressed to avoid inconsistancy.
 * Be aware that when writing or deriving a FXVTK2 class that crucial access to the members of
 * e.g. the vtkActor has to be locked! For ease you can use the FXVTK2_LOCK_VTK and FXVTK2_UNLOCK_VTK
 * macros at the start and end of your method to provide for constistancy.
 * If your renderer crashes for some esoteric matter you probably forget to lock the write access
 * of your node.
 *
 */

class GlobalLock {
public:
	// Return the singleton instance
	static DAFFViz::GlobalLock* getInstance();
	
	// Lock
	virtual void lock() =0;

	// Unlock
	virtual void unlock() =0;

protected:
	virtual ~GlobalLock();
};

// Helper macros
#define DAFFVIZ_LOCK_VTK    { DAFFViz::GlobalLock::getInstance()->lock(); }
#define DAFFVIZ_UNLOCK_VTK  { DAFFViz::GlobalLock::getInstance()->unlock(); }

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_GLOBALLOCK_H__
