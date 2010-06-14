#include "AboutDialog.h"

#include "MainWindow.h"
#include "Resources.h"

#include <FXJPGIcon.h>

FXIMPLEMENT(AboutDialog, FXDialogBox, NULL, 0)

AboutDialog::AboutDialog(FXWindow* owner)
: FXDialogBox(owner, L"About DAFF Viewer", DECOR_TITLE | DECOR_BORDER)
{
	const FXint M = 8;		// Outer margin
	const FXint G = 8;		// Group margin
	const FXint B = 12;		// Banner margin

	// Root container
	FXVerticalFrame* root = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0,0,0,0, M,M,M,M, 0,2*M);

	FXVerticalFrame* top = new FXVerticalFrame(root, FRAME_THICK | FRAME_SUNKEN, 0,0,0,0, B,B,B,B);
	top->setBackColor(FXRGB(0xFF,0xFF,0xFF));
	FXIcon* banner = new FXGIFIcon(getApp(), DAFF_LOGO_GIF);
	new FXLabel(top, "", banner, 0, 0,0,0,0, 0,0,0,0);
	
	FXVerticalFrame* sub = new FXVerticalFrame(top, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0,0,0,0, M,M,M,M, 0,M);
	sub->setBackColor(FXRGB(0xFF,0xFF,0xFF));

	FXLabel* label;

	label = new FXLabel(sub, L"A 2D/3D viewer of DAFF directional audio files", NULL, LABEL_NORMAL | JUSTIFY_LEFT);
	label->setBackColor(FXRGB(0xFF,0xFF,0xFF));

	label = new FXLabel(sub, L"Version 0.1", NULL, LABEL_NORMAL | JUSTIFY_LEFT);
	label->setBackColor(FXRGB(0xFF,0xFF,0xFF));

	label = new FXLabel(sub, L"(c) Copyright Institute of Technical Acoustics (ITA), RWTH Aachen, 2009-2010\n", NULL, LABEL_NORMAL | JUSTIFY_LEFT);
	label->setBackColor(FXRGB(0xFF,0xFF,0xFF));

	label = new FXLabel(sub, L"This software is distributed under the terms of the GNU Lesser Public License (LGPL)", NULL, LABEL_NORMAL | JUSTIFY_LEFT);
	label->setBackColor(FXRGB(0xFF,0xFF,0xFF));
		
	label = new FXLabel(sub, L"DAFF Viewer was written by Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de) and\n"\
		                     L"Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de). Portions contributed by others.", NULL, LABEL_NORMAL | JUSTIFY_LEFT);
	label->setBackColor(FXRGB(0xFF,0xFF,0xFF));
		
	label = new FXLabel(sub, L"DAFF Viewer is a part OpenDAFF,\n"\
		                     L"a free and open-source software package for directional audio data", NULL, LABEL_NORMAL | JUSTIFY_LEFT);
	label->setBackColor(FXRGB(0xFF,0xFF,0xFF));

	label = new FXLabel(sub, L"Visit the OpenDAFF homepage: http://www.opendaff.org", NULL, LABEL_NORMAL | JUSTIFY_LEFT);
	label->setBackColor(FXRGB(0xFF,0xFF,0xFF));

	// Separator
	new FXSeparator(root);

	new FXButton(root, L"OK", NULL, this, FXDialogBox::ID_ACCEPT, BUTTON_NORMAL | LAYOUT_CENTER_X | LAYOUT_FIX_WIDTH, 0,0,60,0, 4, 4, 2, 2);
}

AboutDialog::~AboutDialog() {}
