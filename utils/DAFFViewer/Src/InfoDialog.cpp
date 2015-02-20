#include "InfoDialog.h"

#include "Globals.h"
#include "MainWindow.h"
#include "../Res/Resources.h"

#include <DAFF.h>

FXIMPLEMENT(InfoDialog, FXDialogBox, NULL, 0)

InfoDialog::InfoDialog(FXWindow* owner)
: FXDialogBox(owner, L"Information", DECOR_TITLE | DECOR_BORDER, 0,0,300,500, 0,0,0,0)
{
	const FXint M = 8;		// Outer margin
	const FXint G = 8;		// Group margin

	// Root container
	FXVerticalFrame* root = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0,0,0,0, M,M,M,M, 0,2*M);

	FXTabBook* tabbook = new FXTabBook(root, this, ID_TAB, LAYOUT_FILL_X | LAYOUT_FILL_Y);
	
	// Properties

	FXTabItem* props_dir = new FXTabItem(tabbook, "&Properties", NULL, TAB_TOP_NORMAL, 0,0,0,0, 4,4,2,2);
	FXHorizontalFrame* props_frame = new FXHorizontalFrame(tabbook, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_FILL_Y , 0,0,0,0, 10,10,10,10);
	
	const DAFFProperties* props = MainWindow::m_pReader->getProperties();

	table = new FXMatrix(props_frame, 2, MATRIX_BY_COLUMNS | LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	

	labelCounter = 0;

	newLabel("Content Type");
	newLabel(DAFFUtils::StrContentType(props->getContentType()).c_str());
	newLabel("File format version");
	newLabel(FXStringFormat("%d", props->getFileFormatVersion()).text()); //TODO
	newLabel("Orientation");
	DAFFOrientationYPR ypr;
	props->getOrientation(ypr);
	newLabel("TODO");
	newLabel("Alpha points");
	newLabel(FXStringFormat("%d", props->getAlphaPoints()).text());
	newLabel("Alpha resolution");
	newLabel(FXStringFormat("%.0f°", props->getAlphaResolution()).text());
	newLabel("Alpha range");
	newLabel(FXStringFormat("[%.0f°, %0.f°]", props->getAlphaStart(), props->getAlphaEnd()).text());
	newLabel("Beta points");
	newLabel(FXStringFormat("%d", props->getBetaPoints()).text());
	newLabel("Beta resolution");
	newLabel(FXStringFormat("%.0f°", props->getBetaResolution()).text());
	newLabel("Beta range");
	newLabel(FXStringFormat("[%.0f°, %0.f°]", props->getBetaStart(), props->getBetaEnd()).text());
	newLabel("Number of Records");
	newLabel(FXStringFormat("%d", props->getNumberOfRecords()).text());
	newLabel("Quantization");
	newLabel(DAFFUtils::StrQuantizationType(props->getQuantization()));
	newLabel("Number of channels");
	newLabel(FXStringFormat("%d", props->getNumberOfChannels()).text());
	for (int i = 1; i <= props->getNumberOfChannels(); i++) {
		if (i==1) {
			newLabel("Channel name(s)");
		} else {
			newLabel("");
		}

		if (props->getChannelLabel(i-1).length() == 0) {
			newLabel("<<no name>>");
		} else {
			newLabel(props->getChannelLabel(i-1));
		}
	}

	//Fill props window
	while (labelCounter < 100) {
		newLabel("");
	}
		
// Metadata
	const DAFFMetadata* pMetadata = MainWindow::m_pReader->getMetadata();
	std::vector<std::string> vsKeys;
	pMetadata->getKeys(vsKeys);

	FXTabItem* md_dir = new FXTabItem(tabbook, "&Metadata", NULL, TAB_TOP_NORMAL, 0,0,0,0, 4,4,2,2);
	FXHorizontalFrame* md_frame = new FXHorizontalFrame(tabbook, FRAME_THICK | FRAME_RAISED, 0,0,0,0, 10,10,10,10);
	
	table = new FXMatrix(md_frame, 2, MATRIX_BY_COLUMNS | LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	
	labelCounter = 0;

	// Enter the data
	for (std::vector<std::string>::const_iterator cit=vsKeys.begin(); cit != vsKeys.end(); ++cit) {
		newLabel((*cit).c_str());
		newLabel(pMetadata->getKeyString(*cit).c_str());
	}

	if (vsKeys.empty()) {
		newLabel("<<no meta data>>");
	}

	//Fill md window
	while (labelCounter < 100) {
		newLabel("");
	}


	new FXButton(root, L"OK", NULL, this, FXDialogBox::ID_ACCEPT, BUTTON_NORMAL | LAYOUT_CENTER_X | LAYOUT_FIX_WIDTH, 0,0,60,0, 4,4,2,2);
}

void InfoDialog::newLabel(std::string sText) {
	FXLabel* tmpLabel;

	tmpLabel = new FXLabel(table, sText.c_str(), NULL, LAYOUT_FILL_X | LAYOUT_FIX_HEIGHT | LAYOUT_FILL_COLUMN, 0, 0, 0, 20);
	tmpLabel->setJustify(JUSTIFY_LEFT);

	if ((labelCounter & 2) == 0) {					//1 and 2 will get one color, 3 and 4 the other one and so on...
		tmpLabel->setBackColor(FXRGB(254,254,254));
	} else {
		tmpLabel->setBackColor(FXRGB(230,230,230));
	}

	labelCounter++;
}

InfoDialog::~InfoDialog() {}
