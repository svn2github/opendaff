/* ---------------------------------------------------------------------------
 *
 *							IFX - ITA FOX Extensions
 *            (c) Copyright Institute of Technical Acoustics (ITA)
 *                  RWTH University, Aachen, Germany, 2008-today
 *
 * ---------------------------------------------------------------------------
 *
 *    File:			IFXDAFFDialogBox.h
 *
 *    Purpose:		Dialog box opening a DAFF Frame
 *
 *    Autors:		Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)
 *
 *  ---------------------------------------------------------------------------
 */
// $Id: IFXDAFFDialogBox.h,v 1.2 2006-05-21 14:26:44 stienen Exp $

#ifndef __IFXDAFFDIALOGBOX_H__
#define __IFXDAFFDIALOGBOX_H__

#include <fx.h>
#include <string>

#include <DAFF.h>

#include "IFXDAFFFrame.h"

class IFXDAFFDialogBox : public FXDialogBox {
	FXDECLARE(IFXDAFFDialogBox)
public:
	//! FOX identifiers
	enum {
		ID_ACCEPT = FXDialogBox::ID_LAST,
		ID_LAST
    };

	IFXDAFFFrame* m_pDAFFFrame;

	//! Constructor
	IFXDAFFDialogBox(FXWindow* owner, const FXString& name, DAFFReader* pDAFF, FXuint opts=0, FXint x=0, FXint y=0, FXint w=0, FXint h=0, FXint pl=DEFAULT_PAD, FXint pr=DEFAULT_PAD, FXint pt=DEFAULT_PAD, FXint pb=DEFAULT_PAD, FXint hs=4, FXint vs=4);

protected:
	//! Constructor 
	IFXDAFFDialogBox() {};

	//! Destructor
	~IFXDAFFDialogBox();

private:
	FXIcon* m_pIconDAFF;
};

#endif // __IFXDAFFDIALOGBOX_H__