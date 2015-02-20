// $Id: MainWindow.h,v 1.7 2010/02/10 11:23:31 schmoch Exp $

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <fx.h>
#include <string>

#include <daffviz/FXVTK2BalloonPlot.h>
#include <daffviz/FXVTK2CarpetPlot.h>

#include <FXVTK2.h>

class DAFFReader;
class DAFFContentDFT;
class DAFFContentIR;
class DAFFContentMS;

class MainWindow : public FXMainWindow {
	FXDECLARE(MainWindow)

public:
	// Messages
	enum {
		ID_DUMMY = FXMainWindow::ID_LAST,

		ID_MENU_FILE_OPEN,
		ID_MENU_FILE_INFO,
		ID_MENU_SCREENSHOT,
		ID_MENU_FILE_QUIT,

		ID_MENU_HELP_ABOUT,

		ID_THESCREEN,

		ID_FREQUENCY,
		ID_CHANNEL,

		ID_SCALING,
		ID_FIXED_ANGLE,
		ID_PLOT_TYPE,
		ID_GRID,
		ID_SCALE,
		ID_VECTORS,
		ID_NSPOLE,

		ID_NORMALIZE,
		ID_WARP,
		ID_COLOR,
		ID_WIRE,
		ID_RANGE,

		ID_REFERENCE,
		ID_PROBE,

		ID_LAST
	};

	MainWindow(FXApp* a);
    virtual void create();

	// -= Event handler =-

	long onCmdHelpAbout(FXObject*, FXSelector, void*);
	long onCmdFileQuit(FXObject*, FXSelector, void*);
	long onCmdFileOpen(FXObject*, FXSelector, void*);
	long onCmdFileInfo(FXObject*, FXSelector, void*);

	long onCmdFrequency(FXObject*, FXSelector, void*);

	long onCmdChannels(FXObject*, FXSelector, void*);
	long onCmdGrid(FXObject*, FXSelector, void*);
	long onCmdScale(FXObject*, FXSelector, void*);
	long onCmdVectors(FXObject*, FXSelector, void*);
	long onCmdNSPole(FXObject*, FXSelector, void*);
	long onCmdWarp(FXObject*, FXSelector, void*);
	long onCmdColor(FXObject*, FXSelector, void*);
	long onCmdWire(FXObject*, FXSelector, void*);
	long onCmdScaling(FXObject*, FXSelector, void*);
	long onCmdRange(FXObject*, FXSelector, void*);
	long onCmdFixedAngle(FXObject*, FXSelector, void*);
	long onCmdPlotType(FXObject*, FXSelector, void*);
	long onCmdSreenshot(FXObject* sender, FXSelector, void*);
	long onCmdReference(FXObject* sender, FXSelector, void*);
	long onCmdProbe(FXObject* sender, FXSelector, void*);

	// -= Others methods =-

	// Global access to the DAFF reader
	static DAFFReader* m_pReader;
	static DAFFContentIR* m_pContentIR;
	static DAFFContentMS* m_pContentMS;
	static DAFFContentDFT* m_pContentDFT;

protected:
    MainWindow() {};
	~MainWindow();

private:
	// -= GUI elements =-
	
	// Icons
	FXIcon* iconWindow;
	FXStatusBar* statusbar;
	FXTextField* m_pTxtStatusBox1;
	FXTextField* m_pTxtStatusBox2;
	FXTextField* m_pTxtStatusBox3;

	// Menu
	FXMenuCommand* m_pMenuInformation;
	FXMenuCommand* m_pMenuScreenshot;

	// 3D plot
	FXVTK2::Frame* m_pScreen;
	FXVTK2::SGNode* m_pRootNode;
	FXVTK2::SphericalCoordinateAssistant* m_pSphericalCoordAssist;
	FXVTK2::CartesianCoordinateAssistant* m_pCartesianCoordAssist;
	FXVTK2::BalloonPlot* m_pBalloonPlotNode;
	FXVTK2::CarpetPlot* m_pCarpetPlotNode;

	// Controls
	FXVerticalFrame* m_pControllerSidebar;
	FXGroupBox* m_pSliderGroupBox;
	FXSlider* m_pFrequencySlider;
	FXTextField* m_pFrequencyIndicator;

	FXListBox* m_pChannelList;
	FXCheckButton* m_pScaleCheckButton;
	FXCheckButton* m_pGridCheckButton;
	FXCheckButton* m_pVectorsCheckButton;
	FXCheckButton* m_pNSPoleCheckButton;
	FXCheckButton* m_pWarpCheckButton;
	FXCheckButton* m_pColorCheckButton;
	FXCheckButton* m_pPhaseColorCheckButton;
	FXCheckButton* m_pWireCheckButton;
	FXCheckButton* m_pReferenceCheckButton;
	FXCheckButton* m_pProbeCheckButton;

	FXRadioButton* m_pLinearRadioButton;
	FXRadioButton* m_pLogRadioButton;
	FXRadioButton* m_pFixedAlphaRadioButton;
	FXRadioButton* m_pFixedBetaRadioButton;
	FXRadioButton* m_pBalloonPlotRadioButton;
	FXRadioButton* m_pCarpetPlotRadioButton;

	FXLabel* m_pMinLabel, *m_pMaxLabel, *m_pRResLabel, *m_pPhiResLabel, 
			*m_pOpacityLabel, *m_pLevelLabel, *m_pProbeFirstLabel, *m_pProbeSecondLabel;
	FXString m_sBalloonLinResText, m_sBalloonLogResText, m_sBalloonPhiResText, 
			m_sCarpetBetaResText, m_sCarpetAlphaResText, m_sCarpetYLinResText, 
			m_sCarpetYLogResText, m_sLevelLinText, m_sLevelLogText, 
			m_sMinLinText, m_sMaxLinText, m_sMinLogText, m_sMaxLogText,
			m_sFrequencyText, m_sAlphaAngleText, m_sBetaAngleText, m_sSampleText;
	FXTextField* m_pMinTextField, *m_pMaxTextField, *m_pRResTextField, *m_pPhiResTextField;
	FXButton* m_pResetButton;

	FXSlider* m_pOpacitySlider, *m_pLevelSlider, *m_pProbeFirstSlider, 
			*m_pProbeSecondSlider;
	FXTextField* m_pOpacityIndicator, *m_pProbeFirstTextField, *m_pLevelTextField, 
			*m_pProbeSecondTextField;

	std::string m_sRecentDir;
	std::string m_sInputFile;

	std::vector<float> m_vFrequencies;

	// Loads a DAFF file
	void load(const std::string& sFilename, bool bQuiet=false);

	//Initiates alle Widgets (called from Constructor)
	void initWidgets();

	// update vtk directivity or carpetplot nodes
	void updatePlot();
};

#endif // __MAINWINDOW_H__
