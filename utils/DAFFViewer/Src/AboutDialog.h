#ifndef __ABOUT_DIALOG_H__
#define __ABOUT_DIALOG_H__

#include <fx.h>

class AboutDialog : public FXDialogBox {
FXDECLARE(AboutDialog)
public:
	enum {
		ID_OK = FXDialogBox::ID_LAST,
		ID_LAST
    };

	// -= Nachrichten-Methoden =-

	long onOK(FXObject*, FXSelector, void*);

private:
	AboutDialog() {}

public:
	AboutDialog(FXWindow* owner);
	virtual ~AboutDialog();
};

#endif // __ABOUT_DIALOG_H__
