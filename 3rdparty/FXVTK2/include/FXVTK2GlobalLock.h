/*
 * ---------------------------------------------------------------------------
 *
 *							FXVTK2 Visualization Library
 *            (c) Copyright Institute of Technischal Acoustics (ITA)
 *                  RWTH University, Aachen, Germany, 2008-today
 *
 * ---------------------------------------------------------------------------
 *						    ____  __________  _______
 *						   //  / //__   ___/ //  _   |
 *						  //  /    //  /    //  /_|  |
 *						 //  /    //  /    //  ___   |
 *						//__/    //__/    //__/   |__|
 *
 * ---------------------------------------------------------------------------
 *
 *    File:			FXVTK2GlobalLock.h
 *
 *    Purpose:		Global synchronization lock for mutual VTK access
 *
 *    Autors:		Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de
 *					Jonas Stienen (stienen@akustik.rwth-aachen.de)
 *
 *  ---------------------------------------------------------------------------
 *
 */
// $Id: FXVTK2GlobalLock.h 1722 2011-04-05 08:37:29Z fwefers $

#ifndef __FXVTK2_GLOBALLOCK_H__
#define __FXVTK2_GLOBALLOCK_H__

namespace FXVTK2 {

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
	static FXVTK2::GlobalLock* getInstance();
	
	// Lock
	virtual void lock() const=0;

	// Unlock
	virtual void unlock() const=0;

protected:
	virtual ~GlobalLock();
};

// Helper macros
#define FXVTK2_LOCK_VTK    { FXVTK2::GlobalLock::getInstance()->lock(); }
#define FXVTK2_UNLOCK_VTK  { FXVTK2::GlobalLock::getInstance()->unlock(); }

} // End of namespace "FXVTK2"

#endif //  __FXVTK2_GLOBALLOCK_H__
