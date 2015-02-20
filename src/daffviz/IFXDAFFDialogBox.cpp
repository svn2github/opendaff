// $Id: Frame.cpp,v 1.8 2010/02/10 11:23:31 stienen Exp $

#include <daffviz/IFXDAFFDialogBox.h>
#include <daffviz/IFXDAFFDialogBoxRes.h>

#include <FXVTK2Frame.h>

FXIMPLEMENT(IFXDAFFDialogBox, FXDialogBox, NULL, 0)

IFXDAFFDialogBox::IFXDAFFDialogBox(FXWindow* owner, const FXString& name, DAFFReader* pDAFF, FXuint opts, FXint x, FXint y, FXint w, FXint h, FXint pl, FXint pr, FXint pt, FXint pb, FXint hs, FXint vs)
 : FXDialogBox(owner, name, opts, x, y, w, h, pl, pr, pt, pb, hs, vs)
{	
	m_pIconDAFF = new FXGIFIcon(owner->getApp(), DAFF_ICON_GIF);
	setIcon(m_pIconDAFF);

	FXVerticalFrame* vframe = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);
	new IFXDAFFFrame(vframe, pDAFF, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);
	new FXButton(vframe, L"OK", NULL, this, FXDialogBox::ID_ACCEPT, LAYOUT_RIGHT | BUTTON_NORMAL | LAYOUT_FIX_WIDTH,  0,0,100,0);
}

IFXDAFFDialogBox::~IFXDAFFDialogBox() {
	delete m_pIconDAFF;
}