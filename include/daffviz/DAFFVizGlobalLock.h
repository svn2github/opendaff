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

#ifndef IW_DAFF_GLOBALLOCK
#define IW_DAFF_GLOBALLOCK

#include <DAFFDefs.h>

// Helper macros
#define DAFFVIZ_LOCK_VTK    { DAFFViz::GlobalLock::getInstance()->lock(); }
#define DAFFVIZ_UNLOCK_VTK  { DAFFViz::GlobalLock::getInstance()->unlock(); }

namespace DAFFViz
{

	//! Global lock for synchronization of VTK access
	/**
	 * When drawing a VTK scene by a render process, the mutual exclusion of access to the root node
	 * is indispensable. Any change of the scene graph has to be suppressed to avoid inconsistancy.
	 * Be aware that when writing or deriving a DAFFViz class that crucial access to the members of
	 * e.g. the vtkActor has to be locked! For ease you can use the DAFFVIZ_LOCK_VTK and DAFFVIZ_UNLOCK_VTK
	 * macros at the start and end of your method to provide for constistancy.
	 * If your renderer crashes for some esoteric matter you probably forget to lock the write access
	 * of your node.
	 *
	 */

	class DAFF_API GlobalLock
	{
	public:
		//! Return the singleton instance
		static DAFFViz::GlobalLock* getInstance();
	
		//! Lock globally
		virtual void lock() =0;

		//! Unlock globally
		virtual void unlock() =0;

	protected:
		inline virtual ~GlobalLock() {};
	};

} // End of namespace "DAFFViz"

#endif //  IW_DAFF_GLOBALLOCK
