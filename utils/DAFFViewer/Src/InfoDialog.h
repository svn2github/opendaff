#ifndef __INFO_DIALOG_H__
#define __INFO_DIALOG_H__

#include <fx.h>
#include <string>

class InfoDialog : public FXDialogBox {
FXDECLARE(InfoDialog)
public:
	enum {
		ID_OK = FXDialogBox::ID_LAST,
		ID_TAB,
		ID_LAST
    };

	// -= Nachrichten-Methoden =-

	long onOK(FXObject*, FXSelector, void*);

private:
	InfoDialog() {}
	
	FXMatrix* table;
	unsigned int labelCounter;
	void newLabel(std::string sText);

public:
	InfoDialog(FXWindow* owner);
	virtual ~InfoDialog();
};

#endif // __INFO_DIALOG_H__
