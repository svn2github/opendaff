// $Id: MainWindow.cpp,v 1.8 2010/02/10 11:23:31 schmoch Exp $

#include <vector>
#include <math.h>

#include "MainWindow.h"

#include "AboutDialog.h"
#include "Globals.h"
#include "InfoDialog.h"
#include "../Res/Resources.h"
//#include "DAFFTransformerIR2DFT.h"

#include <DAFF.h>

#include <DAFFTransformerIR2DFT.h>
#include <daffviz/FXVTK2BalloonPlot.h>
#include <daffviz/FXVTK2CarpetPlot.h>
#include <daffviz/IFXDAFFFrame.h>


#include <FXPNGIcon.h>

// Message map
FXDEFMAP(MainWindow) MainWindowMap[]= {
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_MENU_FILE_OPEN,		MainWindow::onCmdFileOpen),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_MENU_FILE_INFO,		MainWindow::onCmdFileInfo),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_MENU_SCREENSHOT,		MainWindow::onCmdSreenshot),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_MENU_FILE_QUIT,		MainWindow::onCmdFileQuit),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_MENU_HELP_ABOUT,		MainWindow::onCmdHelpAbout),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_FREQUENCY,			MainWindow::onCmdFrequency),
	FXMAPFUNC(SEL_MOUSEWHEEL,	MainWindow::ID_FREQUENCY,			MainWindow::onCmdFrequency),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_CHANNEL,				MainWindow::onCmdChannels),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_GRID,				MainWindow::onCmdGrid),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_SCALE,				MainWindow::onCmdScale),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_VECTORS,				MainWindow::onCmdVectors),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_NSPOLE,				MainWindow::onCmdNSPole),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_WARP,				MainWindow::onCmdWarp),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_COLOR,				MainWindow::onCmdColor),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_WIRE,				MainWindow::onCmdWire),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_SCALING,				MainWindow::onCmdScaling),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_FIXED_ANGLE,			MainWindow::onCmdFixedAngle),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_PLOT_TYPE,			MainWindow::onCmdPlotType),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_RANGE,				MainWindow::onCmdRange),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_REFERENCE,			MainWindow::onCmdReference),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_PROBE,				MainWindow::onCmdProbe),
	FXMAPFUNC(SEL_CLOSE,		0,									MainWindow::onCmdFileQuit)
};

FXIMPLEMENT(MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap))

DAFFReader* MainWindow::m_pReader = NULL;
DAFFContentIR* MainWindow::m_pContentIR = NULL;
DAFFContentMS* MainWindow::m_pContentMS = NULL;
DAFFContentDFT* MainWindow::m_pContentDFT = NULL;

MainWindow::MainWindow(FXApp* app)
: FXMainWindow(app, APPNAME, NULL, NULL, DECOR_ALL, 10, 10, 1280,1020),
  m_pScreen(NULL), m_pRootNode(NULL), m_pBalloonPlotNode(NULL), m_pCarpetPlotNode(NULL), m_pSphericalCoordAssist(NULL), m_pCartesianCoordAssist(NULL)
{
	// Drag'n'Drop
	//getApp()->registerDragType("daff");
	//dropEnable();

	// Open file by first command argument
	if (getApp()->getArgc() > 1) {
		FXString argv1 = getApp()->getArgv()[1];
		m_sInputFile = argv1.text();
	}

	// +---------------------+
	// | Loads the resources |
	// +---------------------+

	iconWindow = new FXGIFIcon(getApp(), DAFF_ICON_GIF);
	
	setIcon(iconWindow);
	// setMiniIcon(icon_window_mini);

	// +--------------+
	// | GUI elements |
	// +--------------+

	const FXint M = 8;		// Outer margin
	const FXint G = 24;		// Group margin

	// Menus

	FXMenuBar* pMenubar = new FXMenuBar(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);
	
	FXMenuPane* pFilemenu = new FXMenuPane(this);
    new FXMenuTitle(pMenubar, L"&File", NULL, pFilemenu);
	new FXMenuCommand(pFilemenu, L"&Open...", NULL, this, ID_MENU_FILE_OPEN);
	m_pMenuScreenshot = new FXMenuCommand(pFilemenu, L"&Save Screenshot", NULL, this, ID_MENU_SCREENSHOT);
	m_pMenuScreenshot->disable();
	new FXMenuSeparator(pFilemenu);
	m_pMenuInformation = new FXMenuCommand(pFilemenu, L"&Information", NULL, this, ID_MENU_FILE_INFO);
	m_pMenuInformation->disable();
	new FXMenuSeparator(pFilemenu);
	new FXMenuCommand(pFilemenu, L"&Quit", NULL, this, ID_MENU_FILE_QUIT);

	FXMenuPane* helperMenu = new FXMenuPane(this);
	new FXMenuTitle(pMenubar, L"&Help", NULL, helperMenu);
	new FXMenuCommand(helperMenu, L"&About DAFF Viewer...", NULL, this, ID_MENU_HELP_ABOUT);

	// Statusbar
	statusbar = new FXStatusBar(this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | STATUSBAR_WITH_DRAGCORNER);
	m_pTxtStatusBox1 = new FXTextField(statusbar, 1, 0, 0, LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 180, 0);
	m_pTxtStatusBox1->disable();
	m_pTxtStatusBox1->setTextColor(FXRGB(100,100,100));
	m_pTxtStatusBox2 = new FXTextField(statusbar, 1, 0, 0, LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 180, 0);
	m_pTxtStatusBox2->disable();
	m_pTxtStatusBox2->setTextColor(FXRGB(100,100,100));
	m_pTxtStatusBox3 = NULL;
	m_pTxtStatusBox3 = new FXTextField(statusbar, 1, 0, 0, LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 180, 0);
	m_pTxtStatusBox3->disable();
	m_pTxtStatusBox3->setTextColor(FXRGB(100,100,100));

	// Root container
	FXHorizontalFrame* root = new FXHorizontalFrame(this, 
	                                            LAYOUT_FILL_X | LAYOUT_FILL_Y,
								                0, 0, 0, 0, M, M, M, M);

	// +---------------------+
	// | Everything else ... |
	// +---------------------+
	// define Label Texts
	m_sMinLinText = "Min:";
	m_sMaxLinText = "Max:";
	m_sMinLogText = "Min: [db]";
	m_sMaxLogText = "Max: [db]";
	m_sBalloonLinResText = "r-Resolution:",
	m_sBalloonLogResText = "r-Resolution: [db]",
	m_sBalloonPhiResText = "phi-Resolution: [°]",
	m_sCarpetBetaResText = "Beta-Resolution: [°]",
	m_sCarpetAlphaResText = "Alpha-Resolution: [°]",
	m_sCarpetYLinResText = "Y-Resolution:",
	m_sCarpetYLogResText = "Y-Resolution: [db]",
	m_sLevelLinText = "Level:",
	m_sLevelLogText = "Level: [db]";
	m_sFrequencyText = "Frequency:";
	m_sAlphaAngleText = "Alpha-Angle:";
	m_sBetaAngleText = "Beta-Angle:";
	m_sSampleText = "Sample Number:";


	FXGroupBox* helperGroupBox;
	FXMatrix* helperMatrix;
	FXVerticalFrame* helperVerticalFrame;
	FXHorizontalFrame* helperHorizontalFrame;

	// Visualization
	m_pScreen = new FXVTK2::Frame(root, FRAME_GROOVE | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	m_pScreen->SetMouseInteractionMode(FXVTK2::Frame::MI_DIRECTIONAL_DATA);
	m_pScreen->SetCameraPosition(+5, +2.5, -5);
	m_pScreen->SetParallelProjection();
	m_pScreen->DollyCamera(0.6);	
	

	m_pRootNode = new FXVTK2::SGNode();
	m_pScreen->SetSceneGraphRootNode(m_pRootNode);

	m_pSphericalCoordAssist = new FXVTK2::SphericalCoordinateAssistant();
	m_pCartesianCoordAssist = new FXVTK2::CartesianCoordinateAssistant();
	m_pRootNode->AddChildNode(m_pSphericalCoordAssist);

	// --= Controller sidebar =--
	m_pControllerSidebar = new FXVerticalFrame(root, LAYOUT_FIX_WIDTH, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0);

	// Frequency control
	m_pSliderGroupBox = new FXGroupBox(m_pControllerSidebar, m_sFrequencyText, LAYOUT_FILL_X | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
	helperVerticalFrame = new FXVerticalFrame(m_pSliderGroupBox, LAYOUT_FILL_X);
	m_pFrequencySlider = new FXSlider(helperVerticalFrame, this, ID_FREQUENCY, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pFrequencySlider->disable();
	m_pFrequencySlider->setRange(0,0);
	m_pFrequencySlider->setIncrement(1);
	m_pFrequencySlider->setValue(0);
	m_pScreen->setFrequencySelector(m_pFrequencySlider);

	m_pFrequencyIndicator = new FXTextField(helperVerticalFrame, 80, NULL, ID_FREQUENCY, TEXTFIELD_NORMAL | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
	m_pFrequencyIndicator->disable();
	m_pFrequencyIndicator->setBackColor(FXRGB(0, 0, 0));
	m_pFrequencyIndicator->setTextColor(FXRGB(0, 200, 0));
	

	// Channel control
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Channels", LAYOUT_FILL_X | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
	m_pChannelList = new FXListBox(helperGroupBox, this, ID_CHANNEL, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pChannelList->setNumVisible(1);

	// Settings control
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Settings", LAYOUT_FILL_X | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	helperMatrix = new FXMatrix(helperHorizontalFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
	m_pGridCheckButton = new FXCheckButton(helperMatrix, "Grid", this, ID_GRID);
	m_pGridCheckButton->setCheck(1);
	m_pScaleCheckButton = new FXCheckButton(helperMatrix, "Scale", this, ID_SCALE);
	m_pScaleCheckButton->setCheck(1);
	m_pVectorsCheckButton = new FXCheckButton(helperMatrix, "Directions", this, ID_VECTORS);
	m_pVectorsCheckButton->setCheck(1);
	m_pNSPoleCheckButton = new FXCheckButton(helperMatrix, "Poles", this, ID_NSPOLE);
	m_pNSPoleCheckButton->setCheck(1);
	/*m_pNormalizeCheckButton = new FXCheckButton(helperMatrix, "Normalized", this, ID_NORMALIZE);
	m_pNormalizeCheckButton->setCheck(0);*/
	m_pWarpCheckButton = new FXCheckButton(helperMatrix, "Warp", this, ID_WARP);
	m_pWarpCheckButton->setCheck(1);
	m_pColorCheckButton = new FXCheckButton(helperMatrix, "Color", this, ID_COLOR);
	m_pColorCheckButton->setCheck(1);
	m_pPhaseColorCheckButton = new FXCheckButton(helperMatrix, "Use Phase as Color", this, ID_COLOR);
	m_pPhaseColorCheckButton->setCheck(0);
	m_pWireCheckButton = new FXCheckButton(helperMatrix, "Wireframe", this, ID_WIRE);
	m_pWireCheckButton->setCheck(0);

	helperMatrix = new FXMatrix(helperHorizontalFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
	m_pLinearRadioButton = new FXRadioButton(helperMatrix, "Linear", this, ID_SCALING);
	m_pLinearRadioButton->setCheck(1);
	m_pLogRadioButton = new FXRadioButton(helperMatrix, "Decibel", this, ID_SCALING);
	m_pLogRadioButton->setCheck(0);
	m_pFixedAlphaRadioButton = new FXRadioButton(helperMatrix, "Alpha fixed", this, ID_FIXED_ANGLE);
	m_pFixedAlphaRadioButton->setCheck(0);
	m_pFixedBetaRadioButton = new FXRadioButton(helperMatrix, "Beta fixed", this, ID_FIXED_ANGLE);
	m_pFixedBetaRadioButton->setCheck(1);
	m_pBalloonPlotRadioButton = new FXRadioButton(helperMatrix, "Balloon Plot", this, ID_PLOT_TYPE);
	m_pBalloonPlotRadioButton->setCheck(0);
	m_pCarpetPlotRadioButton = new FXRadioButton(helperMatrix, "Carpet Plot", this, ID_PLOT_TYPE);
	m_pCarpetPlotRadioButton->setCheck(1);

	// Range control
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Data Range", LAYOUT_FILL_X | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	helperMatrix = new FXMatrix(helperHorizontalFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
	m_pMinLabel = new FXLabel(helperMatrix, m_sMinLinText);
	m_pMaxLabel = new FXLabel(helperMatrix, m_sMaxLinText);
	m_pRResLabel = new FXLabel(helperMatrix, m_sBalloonLinResText);
	m_pPhiResLabel = new FXLabel(helperMatrix, m_sBalloonPhiResText);
	m_pResetButton = new FXButton(helperMatrix, "Reset", 0, this, ID_RANGE, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 0, 0, M, M, M, M);
	m_pResetButton->disable();
	helperMatrix = new FXMatrix(helperHorizontalFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
	m_pMinTextField = new FXTextField(helperMatrix, 15, this, ID_RANGE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pMinTextField->setText("0.0");
	m_pMinTextField->disable();
	m_pMaxTextField = new FXTextField(helperMatrix, 15, this, ID_RANGE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pMaxTextField->setText("1.0");
	m_pMaxTextField->disable();
	m_pRResTextField = new FXTextField(helperMatrix, 15, this, ID_RANGE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pRResTextField->setText("0.1");
	m_pRResTextField->disable();
	m_pPhiResTextField = new FXTextField(helperMatrix, 15, this, ID_RANGE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pPhiResTextField->setText("10");
	m_pPhiResTextField->disable();

	
	// Reference
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Reference", LAYOUT_FILL_X | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
	helperVerticalFrame = new FXVerticalFrame(helperGroupBox, LAYOUT_FILL_X);
	
	m_pReferenceCheckButton = new FXCheckButton(helperVerticalFrame, "Enabled", this, ID_REFERENCE);
	m_pReferenceCheckButton->setCheck(1);
	
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	m_pOpacityLabel = new FXLabel(helperHorizontalFrame, "Opacity:");
	
	helperVerticalFrame = new FXVerticalFrame(helperHorizontalFrame, LAYOUT_FILL_X);
	m_pOpacitySlider = new FXSlider(helperVerticalFrame, this, ID_REFERENCE, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pOpacitySlider->disable();
	m_pOpacitySlider->setRange(0,100);
	m_pOpacitySlider->setIncrement(1);
	m_pOpacitySlider->setValue(0);

	m_pOpacityIndicator = new FXTextField(helperVerticalFrame, 80, NULL, ID_REFERENCE, TEXTFIELD_NORMAL | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
	m_pOpacityIndicator->disable();
	m_pOpacityIndicator->setBackColor(FXRGB(0, 0, 0));
	m_pOpacityIndicator->setTextColor(FXRGB(0, 200, 0));
	m_pOpacityIndicator->setText("0%");

	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	m_pLevelLabel = new FXLabel(helperHorizontalFrame, m_sLevelLinText);

	helperVerticalFrame = new FXVerticalFrame(helperHorizontalFrame, LAYOUT_FILL_X);
	m_pLevelSlider = new FXSlider(helperVerticalFrame, this, ID_REFERENCE, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pLevelSlider->disable();
	m_pLevelSlider->setRange(0,100);
	m_pLevelSlider->setIncrement(1);
	m_pLevelSlider->setValue(0);
	m_pLevelTextField = new FXTextField(helperVerticalFrame, 15, this, ID_REFERENCE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pLevelTextField->setText("0.0");
	m_pLevelTextField->disable();
	
	
	// Probe
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Probe", LAYOUT_FILL_X | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
	helperVerticalFrame = new FXVerticalFrame(helperGroupBox, LAYOUT_FILL_X);
	
	m_pProbeCheckButton = new FXCheckButton(helperVerticalFrame, "Enabled", this, ID_PROBE);
	m_pProbeCheckButton->setCheck(1);
	
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	m_pProbeFirstLabel = new FXLabel(helperHorizontalFrame, m_sAlphaAngleText);

	helperVerticalFrame = new FXVerticalFrame(helperHorizontalFrame, LAYOUT_FILL_X);
	m_pProbeFirstSlider = new FXSlider(helperVerticalFrame, this, ID_PROBE, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pProbeFirstSlider->disable();
	m_pProbeFirstSlider->setRange(0,0);
	m_pProbeFirstSlider->setIncrement(1);
	m_pProbeFirstTextField = new FXTextField(helperVerticalFrame, 15, this, ID_PROBE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pProbeFirstTextField->setText("0.0");
	m_pProbeFirstTextField->disable();
	
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	m_pProbeSecondLabel = new FXLabel(helperHorizontalFrame, m_sBetaAngleText);

	helperVerticalFrame = new FXVerticalFrame(helperHorizontalFrame, LAYOUT_FILL_X);
	m_pProbeSecondSlider = new FXSlider(helperVerticalFrame, this, ID_PROBE, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pProbeSecondSlider->disable();
	m_pProbeSecondSlider->setRange(0,0);
	m_pProbeSecondSlider->setIncrement(1);
	m_pProbeSecondTextField = new FXTextField(helperVerticalFrame, 15, this, ID_PROBE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pProbeSecondTextField->setText("0.0");
	m_pProbeSecondTextField->disable();


	// Load input file
	if (!m_sInputFile.empty())
		load(m_sInputFile, true);
}

MainWindow::~MainWindow() {
	getApp()->exit(0);
}

void MainWindow::create() {
    FXMainWindow::create();
	show(PLACEMENT_SCREEN);

	initWidgets();

	// dropEnable();
}

void MainWindow::initWidgets() {
	m_pTxtStatusBox1->setText("x Channel(s)");
	m_pTxtStatusBox2->setText("Resolution (A, B): xxx");
	m_pTxtStatusBox3->setText("Quantization: xxx");
}

/*
   +--------------------+
   |                    |
   |  Event handlers    |
   |                    |
   +--------------------+
*/

long MainWindow::onCmdFileOpen(FXObject*, FXSelector, void*) {
	FXFileDialog fd(this, L"Open file", 0);
	
	if (m_sRecentDir != "") {
		fd.setDirectory(m_sRecentDir.c_str());
	}

	fd.setPatternList("OpenDAFF files (*.daff)\nAll files (*)\n");
	fd.setCurrentPattern(0);

	if (fd.execute() != 0) load(fd.getFilename().text());

	m_sRecentDir = fd.getDirectory().text();

	return 1;
}

long MainWindow::onCmdFileInfo(FXObject*, FXSelector, void*) {
	if (m_pReader == NULL) {
		FXMessageBox::error(this, MBOX_OK, "Error", "No file loaded!");
		return 0;
	}

	InfoDialog* dlg = new InfoDialog(this);
	dlg->execute();
	return 1;
}

long MainWindow::onCmdHelpAbout(FXObject*, FXSelector, void*) {
	AboutDialog* dlg = new AboutDialog(this);
	dlg->execute();
	return 1;
}

long MainWindow::onCmdFileQuit(FXObject*, FXSelector, void*) {
	getApp()->exit(0);	
	return 1;
}

long MainWindow::onCmdFrequency(FXObject*, FXSelector, void*) {
	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetSelectedFrequency( m_pFrequencySlider->getValue() );
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + " Hz" );
	} else {
		m_pCarpetPlotNode->SetSelectedAngle( m_vFrequencies[ m_pFrequencySlider->getValue() ] );
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + "°" );
	}
	m_pScreen->Render();

	return 1;
}

/*
long MainWindow::onWheelFreq(FXObject*, FXSelector, void*) {
	onSlideFreq(NULL,0,NULL);
	return 1;
}
*/

long MainWindow::onCmdChannels(FXObject*, FXSelector, void*) {
	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetChannel(m_pChannelList->getCurrentItem());
		m_pScreen->Render();
	}
	if (m_pCarpetPlotNode) {
		m_pCarpetPlotNode->SetChannel(m_pChannelList->getCurrentItem());
		m_pScreen->Render();
	}

	return 1;
}

long MainWindow::onCmdScale(FXObject*, FXSelector, void*) {
	bool bEnabled = m_pScaleCheckButton->getCheck() > 0 ? true : false;
	m_pSphericalCoordAssist->SetGridVisible(bEnabled);
	m_pCartesianCoordAssist->SetGridVisible(bEnabled);
	m_pScreen->Render();
	return 1;
}

long MainWindow::onCmdGrid(FXObject*, FXSelector, void*) {
	bool bEnabled = m_pGridCheckButton->getCheck() > 0 ? true : false;
	m_pSphericalCoordAssist->SetEquatorVisible(bEnabled);
	m_pSphericalCoordAssist->SetMeridiansVisible(bEnabled);
	m_pCartesianCoordAssist->SetAxesVisible(bEnabled);
	m_pScreen->Render();
	return 1;
}

long MainWindow::onCmdVectors(FXObject*, FXSelector, void*) {
	bool bEnabled = m_pVectorsCheckButton->getCheck() > 0 ? true : false;
	m_pSphericalCoordAssist->SetViewUpVectorsVisible(bEnabled);
	m_pScreen->Render();
	return 1;
}

long MainWindow::onCmdNSPole(FXObject*, FXSelector, void*) {
	bool bEnabled = m_pNSPoleCheckButton->getCheck() > 0 ? true : false;
	m_pSphericalCoordAssist->SetAxesVisible(bEnabled);
	m_pScreen->Render();
	return 1;
}

long MainWindow::onCmdWarp(FXObject* sender, FXSelector, void*) {
	if (m_pBalloonPlotNode) 
		if (m_pWarpCheckButton->getCheck())
			m_pBalloonPlotNode->EnableWarp();
		else
			m_pBalloonPlotNode->DisableWarp();
	else if (m_pCarpetPlotNode) 
		if (m_pWarpCheckButton->getCheck()) {
			m_pCarpetPlotNode->EnableWarp();
			/*if (m_pLogRadioButton->getCheck()) // we have to move the plot-node, because for the negative db-scale the warping goes to the other direction
				m_pCarpetPlotNode->SetPosition(0,1.0,0);
			else
				m_pCarpetPlotNode->SetPosition(0,0.0,0);*/
		} else {
			m_pCarpetPlotNode->DisableWarp();
			m_pCarpetPlotNode->SetPosition(0,0.0,0);
		}

	m_pScreen->Render();
	return 1;
}

long MainWindow::onCmdColor(FXObject* sender, FXSelector, void*) {
	bool bEnabled = (m_pColorCheckButton->getCheck() > 0 ? true : false);
	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetScalarVisibility(bEnabled);
		m_pBalloonPlotNode->SetUsePhaseAsColor(m_pPhaseColorCheckButton->getCheck() > 0 ? true : false);
	}
	if (m_pCarpetPlotNode)
		m_pCarpetPlotNode->SetScalarVisibility(bEnabled);
	m_pScreen->Render();
	return 1;
}

long MainWindow::onCmdWire(FXObject* sender, FXSelector, void*) {
	if (m_pBalloonPlotNode) {
		int iMode = m_pWireCheckButton->getCheck() > 0 ? FXVTK2::BalloonPlot::MODE_WIREFRAME : FXVTK2::BalloonPlot::MODE_SURFACE;
		m_pBalloonPlotNode->SetDisplayMode(iMode);
	}
	if (m_pCarpetPlotNode) {
		int iMode = m_pWireCheckButton->getCheck() > 0 ? FXVTK2::CarpetPlot::MODE_WIREFRAME : FXVTK2::CarpetPlot::MODE_SURFACE;
		m_pCarpetPlotNode->SetDisplayMode(iMode);
	}
	m_pScreen->Render();
	return 1;
}

long MainWindow::onCmdScaling(FXObject* sender, FXSelector, void*) {
	float prec=1.0;
	// Invert states
	if ((FXRadioButton*) sender == m_pLinearRadioButton)
		m_pLogRadioButton->setCheck(!m_pLinearRadioButton->getCheck());
	else
		m_pLinearRadioButton->setCheck(!m_pLogRadioButton->getCheck());

	// which directivity is present?
	if (m_pBalloonPlotNode) {
		// Delegate scaling option
		if (m_pLinearRadioButton->getCheck()) {
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
			m_pMinLabel->setText(m_sMinLinText);
			m_pMaxLabel->setText(m_sMaxLinText);
			m_pRResLabel->setText(m_sBalloonLinResText);
			//m_pPhiResLabel->setText(m_sBallonPhiResText);
			m_pLevelLabel->setText(m_sLevelLinText);
		} else {
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
			m_pMinLabel->setText(m_sMinLogText);
			m_pMaxLabel->setText(m_sMaxLogText);
			m_pRResLabel->setText(m_sBalloonLogResText);
			//m_pPhiResLabel->setText(m_sBallonPhiResText);
			m_pLevelLabel->setText(m_sLevelLogText);
		}
	}
	if (m_pCarpetPlotNode) {
		// Delegate scaling option
		if (m_pLinearRadioButton->getCheck()){
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
			m_pMinLabel->setText(m_sMinLinText);
			m_pMaxLabel->setText(m_sMaxLinText);
			m_pRResLabel->setText(m_sCarpetYLinResText);
			if (m_pFixedAlphaRadioButton->getCheck())
				m_pPhiResLabel->setText(m_sCarpetBetaResText);
			else
				m_pPhiResLabel->setText(m_sCarpetAlphaResText);
			m_pLevelLabel->setText(m_sLevelLinText);
		}else{
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
			m_pMinLabel->setText(m_sMinLogText);
			m_pMaxLabel->setText(m_sMaxLogText);
			m_pRResLabel->setText(m_sCarpetYLogResText);
			if (m_pFixedAlphaRadioButton->getCheck())
				m_pPhiResLabel->setText(m_sCarpetBetaResText);
			else
				m_pPhiResLabel->setText(m_sCarpetAlphaResText);
			m_pLevelLabel->setText(m_sLevelLogText);
		}
	}
	
	float level = FXDoubleVal(m_pLevelTextField->getText());
	if (m_pLinearRadioButton->getCheck()) {
		m_pLevelTextField->setText(FXStringVal(pow(10, 0.1*level)));
	} else {
		m_pLevelTextField->setText(FXStringVal(10*log(level)/log(10.0)));
	}
	
	onCmdRange(m_pResetButton, 0, 0);
	onCmdWarp(m_pWarpCheckButton, 0, 0);

	m_pScreen->Render();

	return 1;
}

long MainWindow::onCmdFixedAngle(FXObject* sender, FXSelector, void*) {
	// Invert states
	if ((FXRadioButton*) sender == m_pFixedBetaRadioButton)
		m_pFixedAlphaRadioButton->setCheck(!m_pFixedBetaRadioButton->getCheck());
	else
		m_pFixedBetaRadioButton->setCheck(!m_pFixedAlphaRadioButton->getCheck());

	if (m_pCarpetPlotNode) {
		if (m_pFixedBetaRadioButton->getCheck()) {
			m_pCarpetPlotNode->SetFixedAngle(FXVTK2::CarpetPlot::BETA_FIXED);
			//adjust frequency slider
			m_pFrequencySlider->setRange(0, m_pContentIR->getProperties()->getBetaPoints()-1);
			m_pFrequencySlider->setValue(m_pContentIR->getProperties()->getBetaPoints()/2);
			m_pFrequencySlider->enable();
			m_pProbeFirstSlider->setRange(m_pContentIR->getProperties()->getAlphaStart(), m_pContentIR->getProperties()->getAlphaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentIR->getProperties()->getAlphaResolution());
			m_pProbeFirstLabel->setText(m_sAlphaAngleText);
			

			m_vFrequencies.clear();
			for (int i=0; i<m_pContentIR->getProperties()->getBetaPoints(); i++) 
				m_vFrequencies.push_back(m_pContentIR->getProperties()->getBetaStart()+ i*m_pContentIR->getProperties()->getBetaResolution());
			
			m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + "°" );
			m_pSliderGroupBox->setText(m_sBetaAngleText);
			m_pCarpetPlotNode->SetSelectedAngle( m_vFrequencies[ m_pFrequencySlider->getValue() ] );

			m_pCartesianCoordAssist->SetMinX(m_pContentIR->getProperties()->getAlphaStart());
			m_pCartesianCoordAssist->SetMaxX(m_pContentIR->getProperties()->getAlphaEnd());
			m_pCartesianCoordAssist->SetResolutionX(20);
		} else {
			m_pCarpetPlotNode->SetFixedAngle(FXVTK2::CarpetPlot::ALPHA_FIXED);
			//adjust frequency slider
			m_pFrequencySlider->setRange(0, m_pContentIR->getProperties()->getAlphaPoints()-1);
			m_pFrequencySlider->setValue(m_pContentIR->getProperties()->getAlphaPoints()/2);
			m_pFrequencySlider->enable();
			m_pProbeFirstSlider->setRange(m_pContentIR->getProperties()->getBetaStart(), m_pContentIR->getProperties()->getBetaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentIR->getProperties()->getBetaResolution());
			m_pProbeFirstLabel->setText(m_sBetaAngleText);

			m_vFrequencies.clear();
			for (int i=0; i<m_pContentIR->getProperties()->getAlphaPoints(); i++) 
				m_vFrequencies.push_back(m_pContentIR->getProperties()->getAlphaStart()+ i*m_pContentIR->getProperties()->getAlphaResolution());
			
			m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + "°" );
			m_pSliderGroupBox->setText(m_sAlphaAngleText);
			m_pCarpetPlotNode->SetSelectedAngle( m_vFrequencies[ m_pFrequencySlider->getValue() ] );

			m_pCartesianCoordAssist->SetMinX(m_pContentIR->getProperties()->getBetaStart());
			m_pCartesianCoordAssist->SetMaxX(m_pContentIR->getProperties()->getBetaEnd());
			m_pCartesianCoordAssist->SetResolutionX(10);
		}
	}
	// Das müsste jetzt nicht mehr nötig sein!
	// m_pCartesianCoordAssist->UpdateGrid();
	m_pScreen->Render();
	return 1;
}


long MainWindow::onCmdPlotType(FXObject* sender, FXSelector, void*) {
	
	// Invert states
	if ((FXRadioButton*) sender == m_pBalloonPlotRadioButton){
		m_pCarpetPlotRadioButton->setCheck(!m_pBalloonPlotRadioButton->getCheck());
		if (m_pLinearRadioButton->getCheck()) 
			m_pRResLabel->setText(m_sBalloonLinResText);
		else
			m_pRResLabel->setText(m_sBalloonLogResText);

		m_pPhiResLabel->setText(m_sBalloonPhiResText);
	}else{
		m_pBalloonPlotRadioButton->setCheck(!m_pCarpetPlotRadioButton->getCheck());
		if (m_pLinearRadioButton->getCheck()) 
			m_pRResLabel->setText(m_sCarpetYLinResText);
		else 
			m_pRResLabel->setText(m_sCarpetYLogResText);
		if (m_pFixedAlphaRadioButton->getCheck())
			m_pPhiResLabel->setText(m_sCarpetAlphaResText);
		else
			m_pPhiResLabel->setText(m_sCarpetBetaResText);
	}
	updatePlot();
	m_pScreen->Render();

	return 1;
}

long MainWindow::onCmdSreenshot(FXObject* sender, FXSelector, void*) {  
	int i=2;
	std::stringstream sFilename;
	sFilename << "Screenshot.jpg";
	FILE * file;
	while (file = fopen(sFilename.str().c_str(), "r")) {
		sFilename.str("");
		sFilename << "Screenshot" << i++ << ".jpg";
		fclose(file);
	}

	m_pScreen->SaveScreenshot(sFilename.str());
	std::stringstream message;
	message << "Screenshot has been saved to " << sFilename.str();
	FXMessageBox::information(this, MBOX_OK, "Screenshot saved", message.str().c_str());
	return 1;
}

long MainWindow::onCmdRange(FXObject* sender, FXSelector, void*) {
	float min = 0.0f, max = 1.0f, prec = 1.0f;
	double res = 10;
	if (sender == m_pResetButton) {
		// try to find good values
		if (m_pBalloonPlotNode) {
			min = 0.0;
			if (m_pContentDFT)
				max = m_pContentDFT->getOverallMagnitudeMaximum();
			if (m_pContentMS)
				max = m_pContentMS->getOverallMagnitudeMaximum();
		}
		if (m_pCarpetPlotNode) {
			if (m_pContentIR) {
				max = m_pContentIR->getOverallPeak();
				min = -max;
			}
		}
		if (m_pLogRadioButton->getCheck() > 0) {
			// convert to log:
			min = -30; 
			max = 10*log(max)/log((double) 10);
		}
		prec = fabs(max-min);
		float n=1;
		while (int(prec/n)!=0)
			n*=2;
		prec = n/10;
		m_pRResTextField->setText(FXStringVal(prec));
		m_pPhiResTextField->setText(FXStringVal(res));
		m_pMaxTextField->setText(FXStringVal(max));
		m_pMinTextField->setText(FXStringVal(min));
	} else {
		min = FXFloatVal(m_pMinTextField->getText());
		max = FXFloatVal(m_pMaxTextField->getText());
		prec = FXFloatVal(m_pRResTextField->getText());
		res = FXIntVal(m_pPhiResTextField->getText());

		if (fmod(360.0, res) != 0) 
			m_pPhiResTextField->setText(FXStringVal(res - fmod(res, 360.0)));

		if (min > max) {
			if ((FXTextField*) sender == m_pMinTextField)
				m_pMaxTextField->setText(m_pMinTextField->getText());
			else
				m_pMinTextField->setText(m_pMaxTextField->getText());
		}
	}

	// Update Plot
	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetRange(min, max);
		m_pSphericalCoordAssist->SetMin(min);
		m_pSphericalCoordAssist->SetMax(max);
		m_pSphericalCoordAssist->SetRResolution(prec);
		m_pSphericalCoordAssist->SetPhiResolution(res);
	
		if (fmod(360.0, res) != 0) 
			m_pPhiResTextField->setText(FXStringVal(m_pSphericalCoordAssist->GetPhiResolution()));
		m_pSphericalCoordAssist->UpdateGrid();
	} else if (m_pCarpetPlotNode) {
		m_pCarpetPlotNode->SetRange(min, max);
		m_pCartesianCoordAssist->SetMinY(min);
		m_pCartesianCoordAssist->SetMaxY(max);
		m_pCartesianCoordAssist->SetResolutionY(prec);
		if ((m_pLinearRadioButton->getCheck()) && ((min < 0) && (max > 0)))
			m_pCartesianCoordAssist->SetOffsetY(0.0);
		else
			m_pCartesianCoordAssist->SetOffsetY(min);

		// Das müsste jetzt nicht mehr nötig sein
		//m_pCartesianCoordAssist->UpdateGrid();

	}
	
	onCmdReference(m_pLevelTextField, 0, 0);
	m_pScreen->Render();

	return 1;
}

long MainWindow::onCmdReference(FXObject* sender, FXSelector, void*) {
	double min = FXDoubleVal(m_pMinTextField->getText()),
		max = FXDoubleVal(m_pMaxTextField->getText());
	if (sender == m_pLevelSlider){
		m_pLevelTextField->setText(FXStringVal((double)m_pLevelSlider->getValue()/100.0*(max-min)+min));
	} else if (sender == m_pLevelTextField) {
		double val = FXDoubleVal(m_pLevelTextField->getText());
		m_pLevelSlider->setValue((val-min)/(max-min)*100);
	}
	m_pOpacityIndicator->setText( FXStringFormat("%d", m_pOpacitySlider->getValue()) + " %" );

	if (m_pCarpetPlotNode) {
		m_pCartesianCoordAssist->SetReferenceLevel(FXDoubleVal(m_pLevelTextField->getText()));
		m_pCartesianCoordAssist->SetReferenceOpacity((double)m_pOpacitySlider->getValue()/100.0);
		
	}
	if (m_pBalloonPlotNode) {
		m_pSphericalCoordAssist->SetReferenceLevel(FXDoubleVal(m_pLevelTextField->getText()));
		m_pSphericalCoordAssist->SetReferenceOpacity((double)m_pOpacitySlider->getValue()/100.0);
		
	}
	
	if (m_pReferenceCheckButton->getCheck()) {
		m_pLevelTextField->enable();
		m_pOpacitySlider->enable();
	} else {
		m_pSphericalCoordAssist->SetReferenceOpacity(0.0);
		m_pCartesianCoordAssist->SetReferenceOpacity(0.0);
		m_pLevelTextField->disable();
		m_pOpacitySlider->disable();
	}
	
	m_pSphericalCoordAssist->UpdateReference();
	// m_pCartesianCoordAssist->UpdateReference(); // Das müsste nicht mehr nötig sein
	m_pScreen->Render();
	return 1;
}

long MainWindow::onCmdProbe(FXObject* sender, FXSelector, void*) {
	if (sender == m_pProbeFirstSlider){
		m_pProbeFirstTextField->setText(FXStringVal((double)m_pProbeFirstSlider->getValue()));
	} else if (sender == m_pProbeFirstTextField) {
		double val = FXDoubleVal(m_pProbeFirstTextField->getText());
		m_pProbeFirstSlider->setValue(val);
	}
	if (sender == m_pProbeSecondSlider){
		m_pProbeSecondTextField->setText(FXStringVal((double)m_pProbeSecondSlider->getValue()));
	} else if (sender == m_pProbeSecondTextField) {
		double val = FXDoubleVal(m_pProbeSecondTextField->getText());
		m_pProbeSecondSlider->setValue(val);
	}
	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetProbeAngles(m_pProbeFirstSlider->getValue(), m_pProbeSecondSlider->getValue());
		m_pBalloonPlotNode->SetProbeVisible(m_pProbeCheckButton->getCheck() > 0 ? true : false);
		m_pBalloonPlotNode->UpdateProbe();
	}
	if (m_pCarpetPlotNode) {
		m_pCarpetPlotNode->SetProbeAngles(m_pProbeFirstSlider->getValue(), m_pProbeSecondSlider->getValue());
		m_pCarpetPlotNode->SetProbeVisible(m_pProbeCheckButton->getCheck() > 0 ? true : false);
		m_pCarpetPlotNode->UpdateProbe();
	}
	m_pScreen->Render();
	return 0;
}

/*
   +------------------+
   |                  |
   |  Other methods   |
   |                  |
   +------------------+
*/
void MainWindow::updatePlot() {
	// file open?
	if (m_pReader==0)
		return;
	
	// reset gui
	m_pFrequencySlider->disable();
	m_pFrequencyIndicator->disable();
	m_pMaxTextField->disable();
	m_pMinTextField->disable();
	m_pWarpCheckButton->disable();
	m_pRResTextField->disable();
	m_pPhiResTextField->disable();
	m_pLinearRadioButton->disable();
	m_pCarpetPlotRadioButton->disable();
	m_pBalloonPlotRadioButton->disable();
	m_pLogRadioButton->disable();
	m_pMenuInformation->disable();
	m_pMenuScreenshot->disable();
	m_pChannelList->clearItems();
	m_pChannelList->disable();
	m_pGridCheckButton->disable();
	m_pScaleCheckButton->disable();
	m_pNSPoleCheckButton->disable();
	m_pVectorsCheckButton->disable();
	m_pColorCheckButton->disable();
	m_pWireCheckButton->disable();
	m_pFixedAlphaRadioButton->disable();
	m_pFixedBetaRadioButton->disable();
	m_pLevelTextField->disable();
	m_pOpacityIndicator->disable();
	m_pOpacitySlider->disable();
	m_pPhaseColorCheckButton->disable();
	m_pProbeCheckButton->disable();
	m_pProbeFirstTextField->disable();
	m_pProbeSecondTextField->disable();
	m_pProbeFirstSlider->disable();
	m_pProbeSecondSlider->disable();
	
	m_pMenuScreenshot->enable();
	m_pWarpCheckButton->setCheck();
	m_pColorCheckButton->setCheck();
	m_pWireCheckButton->setCheck(false); // TODO?
	m_pPhaseColorCheckButton->setCheck(false); // TODO?
	m_pProbeCheckButton->setCheck(false);

	// Remove old plot child nodes
	if (m_pBalloonPlotNode) {
		m_pRootNode->RemoveChildNode(m_pBalloonPlotNode);
		delete m_pBalloonPlotNode;
		m_pBalloonPlotNode=0;
	}
	if (m_pCarpetPlotNode){
		m_pRootNode->RemoveChildNode(m_pCarpetPlotNode);
		delete m_pCarpetPlotNode;
		m_pCarpetPlotNode = 0;
	}
	if (m_pSphericalCoordAssist) m_pRootNode->RemoveChildNode(m_pSphericalCoordAssist);
	if (m_pCartesianCoordAssist) m_pRootNode->RemoveChildNode(m_pCartesianCoordAssist);
	m_vFrequencies.clear();
	//...
	
	const DAFFProperties* props = m_pReader->getProperties();

	// create new plot node and reset coordinate system
	if (m_pContentIR != NULL) {
		if (m_pCarpetPlotRadioButton->getCheck()) { // no transform
			m_pCarpetPlotNode = new FXVTK2::CarpetPlot( m_pContentIR );
			m_pRootNode->AddChildNode(m_pCarpetPlotNode);
			m_pRootNode->AddChildNode(m_pCartesianCoordAssist);
			m_pCartesianCoordAssist->SetPosition(-1, 0, -1);
			m_pCartesianCoordAssist->SetAxesLength(2, 1, 2);
			m_pCartesianCoordAssist->SetMinZ(0);
			m_pCartesianCoordAssist->SetMaxZ(m_pContentIR->getFilterLength());
			m_pCartesianCoordAssist->SetResolutionZ(10);
			m_pCartesianCoordAssist->SetGridVisible(m_pGridCheckButton->getCheck() > 0 ? true : false);
			m_pCarpetPlotNode->SetSelectedAngle( 90 );
		} else if (m_pBalloonPlotRadioButton->getCheck() > 0 ? true : false) { // transform to directivity
			DAFFTransformerIR2DFT* transformer = new DAFFTransformerIR2DFT(m_pContentIR);
			m_pContentDFT = transformer->getOutputContent();
			m_pBalloonPlotNode = new FXVTK2::BalloonPlot( m_pContentDFT );
			m_pRootNode->AddChildNode(m_pBalloonPlotNode);
			m_pRootNode->AddChildNode(m_pSphericalCoordAssist);
			m_pBalloonPlotNode->SetSelectedFrequency(0);
			m_pBalloonPlotNode->SetScale(1.0,1.0,1.0);
			m_pBalloonPlotNode->SetRange(0, m_pContentDFT->getOverallMagnitudeMaximum());
		}
		// enable transformation radio buttons
		m_pCarpetPlotRadioButton->enable();
		m_pBalloonPlotRadioButton->enable();
	} else if (m_pContentMS != NULL) {
		m_pBalloonPlotNode = new FXVTK2::BalloonPlot( m_pContentMS );
		m_pRootNode->AddChildNode(m_pBalloonPlotNode);
		m_pRootNode->AddChildNode(m_pSphericalCoordAssist);
		m_pBalloonPlotNode->SetSelectedFrequency(0);
		m_pBalloonPlotNode->SetScale(1.0,1.0,1.0);
		m_pBalloonPlotNode->SetRange(0, m_pContentMS->getOverallMagnitudeMaximum());
	} else if (m_pContentDFT != NULL) {
		m_pBalloonPlotNode = new FXVTK2::BalloonPlot( m_pContentDFT );
		m_pRootNode->AddChildNode(m_pBalloonPlotNode);
		m_pRootNode->AddChildNode(m_pSphericalCoordAssist);
		m_pBalloonPlotNode->SetSelectedFrequency(0);
		m_pBalloonPlotNode->SetScale(1.0,1.0,1.0);
		m_pBalloonPlotNode->SetRange(0, m_pContentDFT->getOverallMagnitudeMaximum());
	}

	
	// update GUI
	if (m_pCarpetPlotNode) {
		// Update Range TextFields and checkboxes
		m_pMaxTextField->setText(FXStringVal(m_pCarpetPlotNode->GetRangeMax()));
		m_pMinTextField->setText(FXStringVal(m_pCarpetPlotNode->GetRangeMin()));
		m_pFixedAlphaRadioButton->enable();
		m_pFixedBetaRadioButton->enable();
		
		//m_pFixedBetaRadioButton->setCheck(1);
		if (m_pFixedBetaRadioButton->getCheck()) {
			m_pCarpetPlotNode->SetFixedAngle(FXVTK2::CarpetPlot::BETA_FIXED);
			m_pCartesianCoordAssist->SetMinX(m_pContentIR->getProperties()->getAlphaStart());
			m_pCartesianCoordAssist->SetMaxX(m_pContentIR->getProperties()->getAlphaEnd());
			m_pCartesianCoordAssist->SetResolutionX(20);
			m_pFrequencySlider->setRange(0, m_pContentIR->getProperties()->getBetaPoints()-1);
			m_pFrequencySlider->setValue(m_pContentIR->getProperties()->getBetaPoints()/2);
			m_pSliderGroupBox->setText(m_sBetaAngleText);
			m_pProbeFirstSlider->setRange(m_pContentIR->getProperties()->getAlphaStart(), m_pContentIR->getProperties()->getAlphaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentIR->getProperties()->getAlphaResolution());
			m_pProbeFirstLabel->setText(m_sAlphaAngleText);
		} else {
			m_pCarpetPlotNode->SetFixedAngle(FXVTK2::CarpetPlot::ALPHA_FIXED);
			m_pCartesianCoordAssist->SetMinX(m_pContentIR->getProperties()->getBetaStart());
			m_pCartesianCoordAssist->SetMaxX(m_pContentIR->getProperties()->getBetaEnd());
			m_pCartesianCoordAssist->SetResolutionX(10);
			m_pFrequencySlider->setRange(0, m_pContentIR->getProperties()->getAlphaPoints()-1);
			m_pFrequencySlider->setValue(m_pContentIR->getProperties()->getAlphaPoints()/2);
			m_pSliderGroupBox->setText(m_sAlphaAngleText);
			m_pProbeFirstSlider->setRange(m_pContentIR->getProperties()->getBetaStart(), m_pContentIR->getProperties()->getBetaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentIR->getProperties()->getBetaResolution());	
			m_pProbeFirstLabel->setText(m_sBetaAngleText);
		}
		m_pProbeSecondSlider->setRange(0, m_pContentIR->getFilterLength()-1);
		m_pProbeSecondSlider->setIncrement(1);
		m_pProbeSecondLabel->setText(m_sSampleText);

		for (int i=0; i<m_pContentIR->getProperties()->getBetaPoints(); i++) 
			m_vFrequencies.push_back(m_pContentIR->getProperties()->getBetaStart()+ i*m_pContentIR->getProperties()->getBetaResolution());
		
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + "°" );

		if (m_pLinearRadioButton->getCheck()) 
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
		else
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
		
		m_pProbeCheckButton->enable();
		m_pProbeFirstTextField->enable();
		m_pProbeSecondTextField->enable();
		m_pProbeFirstSlider->enable();
		m_pProbeSecondSlider->enable();
	}

	if (m_pBalloonPlotNode) {
		// do content specific stuff first
		if (m_pContentMS) {
			m_pBalloonPlotNode->SetRange(0.0, m_pContentMS->getOverallMagnitudeMaximum());
			m_vFrequencies = m_pContentMS->getFrequencies();
			m_pFrequencySlider->setRange(0, m_pContentMS->getNumFrequencies()-1);
			m_pProbeFirstSlider->setRange(m_pContentMS->getProperties()->getAlphaStart(), m_pContentMS->getProperties()->getAlphaEnd());
			m_pProbeSecondSlider->setRange(m_pContentMS->getProperties()->getBetaStart(), m_pContentMS->getProperties()->getBetaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentMS->getProperties()->getAlphaResolution());
			m_pProbeSecondSlider->setIncrement(m_pContentMS->getProperties()->getBetaResolution());
		} else if (m_pContentDFT) {
			m_pBalloonPlotNode->SetRange(0.0, m_pContentDFT->getOverallMagnitudeMaximum());
			for (int i=0; i<m_pContentDFT->getNumDFTCoeffs(); i++)
				m_vFrequencies.push_back( i*m_pContentDFT->getFrequencyBandwidth() );
			m_pFrequencySlider->setRange(0, m_pContentDFT->getNumDFTCoeffs()-1);
			m_pProbeFirstSlider->setRange(m_pContentDFT->getProperties()->getAlphaStart(), m_pContentDFT->getProperties()->getAlphaEnd());
			m_pProbeSecondSlider->setRange(m_pContentDFT->getProperties()->getBetaStart(), m_pContentDFT->getProperties()->getBetaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentDFT->getProperties()->getAlphaResolution());
			m_pProbeSecondSlider->setIncrement(m_pContentDFT->getProperties()->getBetaResolution());
			m_pPhaseColorCheckButton->enable();

		}
		m_pSliderGroupBox->setText(m_sFrequencyText);
		m_pFrequencySlider->setValue(0);
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + " Hz" );
		
		m_pMaxTextField->setText(FXStringVal(m_pBalloonPlotNode->GetRangeMax()));
		m_pMinTextField->setText(FXStringVal(m_pBalloonPlotNode->GetRangeMin()));
		
		// update scaling
		if (m_pLinearRadioButton->getCheck()) 
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
		else
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
		
		m_pProbeFirstLabel->setText(m_sAlphaAngleText);
		m_pProbeSecondLabel->setText(m_sBetaAngleText);
		m_pNSPoleCheckButton->enable();
		m_pVectorsCheckButton->enable();
		m_pRResTextField->enable();
		m_pProbeCheckButton->enable();
		m_pProbeFirstTextField->enable();
		m_pProbeSecondTextField->enable();
		m_pProbeFirstSlider->enable();
		m_pProbeSecondSlider->enable();
	}
	
		
	// finally do all plot-node independant stuff


	// update textfields and checkboxes
	m_pWarpCheckButton->enable();
	m_pRResTextField->enable();
	m_pPhiResTextField->enable();
	m_pResetButton->enable();
	m_pGridCheckButton->enable();
	m_pScaleCheckButton->enable();
	m_pColorCheckButton->enable();
	m_pWireCheckButton->enable();
	m_pLinearRadioButton->enable();
	m_pLogRadioButton->enable();
	m_pMaxTextField->enable();
	m_pMinTextField->enable();
	m_pLevelTextField->enable();
	m_pLevelSlider->enable();
	m_pOpacitySlider->enable();

	// Adjust frequency slider
	m_pFrequencySlider->enable();
		
	// update statusboxes
	m_pTxtStatusBox1->setText(FXStringFormat("%d", props->getNumberOfChannels()) + " Channel(s)");
	m_pTxtStatusBox2->setText("Resolution (A, B): " + FXStringFormat("%.0f°", props->getBetaResolution()) + ", " + FXStringFormat("%.0f°", props->getBetaResolution()));
	m_pTxtStatusBox3->setText(DAFFUtils::StrQuantizationType(props->getQuantization()).c_str());
		
	// update channel list
	for (int i=1; i <= props->getNumberOfChannels(); i++) {
		if( props->getChannelLabel(i-1).length() == 0) {
			m_pChannelList->appendItem("Channel " + FXStringFormat("%d", i));
		} else {
			m_pChannelList->appendItem("Ch. " + FXStringFormat("%d", i) + " - " + props->getChannelLabel(i-1).c_str());
		}
	}
	m_pChannelList->setNumVisible(props->getNumberOfChannels());
	m_pChannelList->enable();

	// update menu
	m_pMenuInformation->enable();
	
	// trigger range, probe und warp reset
	onCmdRange(m_pResetButton, 0, 0);
	onCmdReference(m_pReferenceCheckButton, 0, 0);
	onCmdWarp(m_pWarpCheckButton, 0, 0);
}

void MainWindow::load(const std::string& sFilename, bool bQuiet) {
	/*
	 *  We do it like to following:
	 *
	 *  - Try to open the new file, but preserve the old one
	 *  - If the reading was successfull, we discard the old one
	 *  - If it failed, we stick to the old one
	 *
	 */

	DAFFReader* pNewReader = DAFFReader::create();
	int iError = pNewReader->openFile(sFilename);
	
	if (iError != 0) {
		delete pNewReader;
		if(!bQuiet) FXMessageBox::error(this, MBOX_OK, "Error", DAFFUtils::StrError(iError).c_str());
	} else if (pNewReader->getContent()->getProperties()->getNumberOfRecords() <= 1) {
		delete pNewReader;
		if(!bQuiet) FXMessageBox::error(this, MBOX_OK, "Error", "The choosen file contains only one or zero records.");
	} else {
		// Discard the old data
		delete m_pReader;
		m_pReader = pNewReader;

		setTitle(APPNAME + FXStringFormat(" [%s]", sFilename.c_str()));
		statusbar->getStatusLine()->setText("Wow!");

		if (pNewReader->getContentType() == DAFF_IMPULSE_RESPONSE) {
			m_pContentIR = dynamic_cast<DAFFContentIR*>( m_pReader->getContent() );
			m_pContentMS = NULL;
			m_pContentDFT = NULL;
		}
		if (pNewReader->getContentType() == DAFF_MAGNITUDE_SPECTRUM) {
			m_pContentIR = NULL;
			m_pContentMS = dynamic_cast<DAFFContentMS*>( m_pReader->getContent() );
			m_pContentDFT = NULL;
		}

		if (pNewReader->getContentType() == DAFF_DFT_SPECTRUM) {
			m_pContentIR = NULL;
			m_pContentDFT = dynamic_cast<DAFFContentDFT*>( m_pReader->getContent() );
			m_pContentMS = NULL;
		}

		updatePlot();
	}
}
