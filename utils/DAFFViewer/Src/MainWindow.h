// $Id: MainWindow.h,v 1.7 2010/02/10 11:23:31 schmoch Exp $

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <fx.h>
#include <string>

#include "FXVTKWidget.h"
#include "FXVTKDirectivityPlot.h"
#include "FXVTKSphere.h"
#include "FXVTKCylinder.h"
#include "FXVTKPolygons.h"

class DAFFReader;
class DAFFContentDFT;
class DAFFContentIR;
class DAFFContentMS;

class MainWindow : public FXMainWindow {
	FXDECLARE(MainWindow)

public:
	// Messages
	enum {
		ID_SOMETHING = FXMainWindow::ID_LAST,

		ID_MENU_HELP_ABOUT,
		ID_MENU_FILE_OPEN,
		ID_MENU_FILE_INFO,
		ID_MENU_FILE_QUIT,

		ID_THESCREEN,

		ID_SLIDER_AZIMUTH,
		ID_SLIDER_ELEVATION,
		ID_SLIDER_FREQ,

		ID_LISTBOX_CHANNEL,
		ID_CHK_GRID,
		ID_CHK_VECTORS,
		ID_CHK_NSPOLE,
		ID_RADIO_LIN,
		ID_RADIO_LOG,

		ID_LAST
	};

	MainWindow(FXApp* a);
    virtual void create();

	// -= Event handler =-

	long onDrop(FXObject*, FXSelector, void*);

	long onCmdHelpAbout(FXObject*, FXSelector, void*);
	long onCmdFileQuit(FXObject*, FXSelector, void*);
	long onCmdFileOpen(FXObject*, FXSelector, void*);
	long onCmdFileInfo(FXObject*, FXSelector, void*);

	long onSlideAzimuth(FXObject*, FXSelector, void*);
	long onWheelAzimuth(FXObject*, FXSelector, void*);
	long onSlideElevation(FXObject*, FXSelector, void*);
	long onWheelElevation(FXObject*, FXSelector, void*);
	long onSlideFreq(FXObject*, FXSelector, void*);
	long onWheelFreq(FXObject*, FXSelector, void*);

	long onListChannels(FXObject*, FXSelector, void*);
	long onChkGrid(FXObject*, FXSelector, void*);
	long onChkVectors(FXObject*, FXSelector, void*);
	long onChkNSPole(FXObject*, FXSelector, void*);
	long onRadLin(FXObject*, FXSelector, void*);
	long onRadLog(FXObject*, FXSelector, void*);

	long onSomething(FXObject*, FXSelector, void*);

	long onDNDEnter(FXObject*, FXSelector, void*);
	long onDNDDrop(FXObject*, FXSelector, void*);

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
	FXTextField* txtStatusBox1;
	FXTextField* txtStatusBox2;
	FXTextField* txtStatusBox3;

	//Menu
	FXMenuCommand* menuInformation;

	// 3D Plot
	FXVTKWidget* theScreen;
	FXVTKDirectivityPlot* thePlot;
	FXVTKSphere* sphNorth;
	FXVTKSphere* sphSouth;
	FXVTKCylinder* cylPoleAxis;
	FXVTKCylinder* cylUpVektor;
	FXVTKCylinder* cylViewVektor;
	FXVTKPolygons* arrUpVektor;
	FXVTKPolygons* arrViewVektor;

	// Controls
	FXSlider* slideAzimuth;
	FXSlider* slideElevation;
	FXSlider* slideFrequency;

	FXLabel* lblAzimuth;
	FXLabel* lblElevation;
	FXLabel* lblFrequency;

	FXListBox* listChannel;
	FXCheckButton* chkGrid;
	FXCheckButton* chkVectors;
	FXCheckButton* chkNSPole;

	FXRadioButton* radLin;
	FXRadioButton* radLog;

	std::string sRecentDir;

	std::vector<float> fAvailableFreq;

	// Loads a DAFF file
	void load(const std::string& sFilename);

	//Initiates alle Widgets (called from Constructor)
	void initWidgets();

	// Update the states of GUI elements (after file loading)
	void updateElements();
};

#endif // __MAINWINDOW_H__
