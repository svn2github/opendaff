// $Id: MainWindow.cpp,v 1.8 2010/02/10 11:23:31 schmoch Exp $

#include <vector>
#include <math.h>

#include "MainWindow.h"

#include "AboutDialog.h"
#include "Globals.h"
#include "InfoDialog.h"
#include "Resources.h"

#include <DAFF.h>
#include <FXPNGIcon.h>

// Message map
FXDEFMAP(MainWindow) MainWindowMap[]= {
	FXMAPFUNC(SEL_DND_DROP,		MainWindow::ID_THESCREEN,			MainWindow::onDrop),

	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_MENU_FILE_OPEN,		MainWindow::onCmdFileOpen),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_MENU_FILE_INFO,		MainWindow::onCmdFileInfo),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_MENU_HELP_ABOUT,		MainWindow::onCmdHelpAbout),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_MENU_FILE_QUIT,		MainWindow::onCmdFileQuit),

	FXMAPFUNC(SEL_CHANGED,		MainWindow::ID_SLIDER_AZIMUTH,		MainWindow::onSlideAzimuth),
	FXMAPFUNC(SEL_MOUSEWHEEL,	MainWindow::ID_SLIDER_AZIMUTH,		MainWindow::onWheelAzimuth),
	FXMAPFUNC(SEL_CHANGED,		MainWindow::ID_SLIDER_ELEVATION,	MainWindow::onSlideElevation),
	FXMAPFUNC(SEL_MOUSEWHEEL,	MainWindow::ID_SLIDER_ELEVATION,	MainWindow::onWheelElevation),
	FXMAPFUNC(SEL_CHANGED,		MainWindow::ID_SLIDER_FREQ,			MainWindow::onSlideFreq),
	FXMAPFUNC(SEL_MOUSEWHEEL,	MainWindow::ID_SLIDER_FREQ,			MainWindow::onWheelFreq),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_LISTBOX_CHANNEL,		MainWindow::onListChannels),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_CHK_GRID,			MainWindow::onChkGrid),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_CHK_VECTORS,			MainWindow::onChkVectors),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_CHK_NSPOLE,			MainWindow::onChkNSPole),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_RADIO_LIN,			MainWindow::onRadLin),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_RADIO_LOG,			MainWindow::onRadLog),
	

	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_SOMETHING,			MainWindow::onSomething),

	//FXMAPFUNC(SEL_DND_ENTER,	0,									MainWindow::onDNDEnter),
	//FXMAPFUNC(SEL_DND_DROP,	0,									MainWindow::onDNDDrop),

	FXMAPFUNC(SEL_CLOSE,		0,									MainWindow::onCmdFileQuit)
};

FXIMPLEMENT(MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap))

DAFFReader* MainWindow::m_pReader = NULL;
DAFFContentIR* MainWindow::m_pContentIR = NULL;
DAFFContentMS* MainWindow::m_pContentMS = NULL;
DAFFContentDFT* MainWindow::m_pContentDFT = NULL;

MainWindow::MainWindow(FXApp* app)
: FXMainWindow(app, APPNAME, NULL, NULL, DECOR_ALL, 10, 10, 1020,700)
{
	sRecentDir="";

	//Drag n Drop
	getApp()->registerDragType("daff");
	this->dropEnable();


	// +---------------------+
	// | Loads the resources |
	// +---------------------+

	//iconWindow = new FXPNGIcon(getApp(), PNG_ICON_WINDOW_16x16);
	iconWindow = new FXGIFIcon(getApp(), DAFF_ICON_GIF);
	
	setIcon(iconWindow);
	// setMiniIcon(icon_window_mini);

	// +--------------+
	// | GUI elements |
	// +--------------+

	const FXint M = 8;		// Outer margin
	const FXint G = 24;		// Group margin

	// Menus

	FXMenuBar* menubar = new FXMenuBar(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);
	
	FXMenuPane* filemenu = new FXMenuPane(this);
    new FXMenuTitle(menubar, L"&File", NULL, filemenu);
	new FXMenuCommand(filemenu, L"&Open...", NULL, this, ID_MENU_FILE_OPEN);
	new FXMenuSeparator(filemenu);
	menuInformation = new FXMenuCommand(filemenu, L"&Information", NULL, this, ID_MENU_FILE_INFO);
	menuInformation->disable();
	new FXMenuSeparator(filemenu);
	new FXMenuCommand(filemenu, L"&Quit", NULL, this, ID_MENU_FILE_QUIT);

	FXMenuPane* helpmenu = new FXMenuPane(this);
	new FXMenuTitle(menubar, L"&Help", NULL, helpmenu);
	new FXMenuCommand(helpmenu, L"&About DAFF Viewer...", NULL, this, ID_MENU_HELP_ABOUT);

	// Statusbar
	statusbar = new FXStatusBar(this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | STATUSBAR_WITH_DRAGCORNER);
	txtStatusBox1 = new FXTextField(statusbar, 1, 0, 0, LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 180, 0);
	txtStatusBox1->disable();
	txtStatusBox1->setTextColor(FXRGB(100,100,100));
	txtStatusBox2 = new FXTextField(statusbar, 1, 0, 0, LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 180, 0);
	txtStatusBox2->disable();
	txtStatusBox2->setTextColor(FXRGB(100,100,100));
	txtStatusBox3 = NULL;
	txtStatusBox3 = new FXTextField(statusbar, 1, 0, 0, LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 180, 0);
	txtStatusBox3->disable();
	txtStatusBox3->setTextColor(FXRGB(100,100,100));


	// Root container
	FXHorizontalFrame* root = new FXHorizontalFrame(this, 
	                                            LAYOUT_FILL_X | LAYOUT_FILL_Y,
								                0, 0, 0, 0, M, M, M, M);

	// +---------------------+
	// | Everything else ... |
	// +---------------------+

	FXGroupBox* gBox;
	FXMatrix* matrix;
	FXVerticalFrame* vFrame;

	//3D-Screen
	theScreen = new FXVTKWidget(root, LAYOUT_FILL_X | LAYOUT_FILL_Y);
	//theScreen->SetMouseCameraInteractionOff();
	theScreen->dropEnable();

	thePlot = new FXVTKDirectivityPlot(theScreen);

	sphNorth = new FXVTKSphere(theScreen);
	sphNorth->SetRadius(0.01);
	sphNorth->SetColor(1,0,0);
	sphNorth->SetPosition(0,0.29,0);
	sphNorth->SetAlpha(0);

	sphSouth = new FXVTKSphere(theScreen);
	sphSouth->SetRadius(0.01);
	sphSouth->SetColor(0,1,0);
	sphSouth->SetPosition(0,-0.29,0);
	sphSouth->SetAlpha(0);

	cylPoleAxis = new FXVTKCylinder(theScreen);
	cylPoleAxis->SetRadius(0.0008);
	cylPoleAxis->SetOrientation(0,0,0);
	cylPoleAxis->SetColor(0.4,0.4,0.4);
	cylPoleAxis->SetHeight(0.58);
	cylPoleAxis->SetPosition(0,0,0);
	cylPoleAxis->SetAlpha(0);

	cylUpVektor = new FXVTKCylinder(theScreen);
	cylUpVektor->SetRadius(0.002);
	cylUpVektor->SetOrientation(0,0,0);
	cylUpVektor->SetColor(1,1,1);
	cylUpVektor->SetHeight(0.26);
	cylUpVektor->SetPosition(0,0.13,0);
	cylUpVektor->SetAlpha(0);

	cylViewVektor = new FXVTKCylinder(theScreen);
	cylViewVektor->SetRadius(0.002);
	cylViewVektor->SetOrientation(90,0,0);
	cylViewVektor->SetColor(1,1,0);
	cylViewVektor->SetHeight(0.26);
	cylViewVektor->SetPosition(0,0,0.13);
	cylViewVektor->SetAlpha(0);

	double a=0.007; //radius for arrow tip
	double b=a/sqrt((double)2);
	double c=0.025; //length of arrow tip

	arrUpVektor = new FXVTKPolygons(theScreen);
	arrUpVektor->AddPolygon(a,0,0,			b,0,b,		0,c,0);
	arrUpVektor->AddPolygon(b,0,b,			0,0,a,		0,c,0);
	arrUpVektor->AddPolygon(0,0,a,			-b,0,b,		0,c,0);
	arrUpVektor->AddPolygon(-b,0,b,			-a,0,0,		0,c,0);
	arrUpVektor->AddPolygon(-a,0,0,			-b,0,-b,	0,c,0);
	arrUpVektor->AddPolygon(-b,0,-b,		0,0,-a,		0,c,0);
	arrUpVektor->AddPolygon(0,0,-a,			b,0,-b,		0,c,0);
	arrUpVektor->AddPolygon(b,0,-b,			a,0,0,		0,c,0);
	arrUpVektor->AddPolygon(a,0,0,	b,0,b,	0,0,a,	0,0,-a,	b,0,-b);
	arrUpVektor->AddPolygon(-a,0,0,	-b,0,-b,	0,0,-a,	0,0,a,	-b,0,b);
	arrUpVektor->SetPosition(0,0.25,0);
	arrUpVektor->SetMaterial(0,1,1,1,0);

	arrViewVektor = new FXVTKPolygons(theScreen);
	arrViewVektor->AddPolygon(a,0,0,		b,0,b,		0,c,0);
	arrViewVektor->AddPolygon(b,0,b,		0,0,a,		0,c,0);
	arrViewVektor->AddPolygon(0,0,a,		-b,0,b,		0,c,0);
	arrViewVektor->AddPolygon(-b,0,b,		-a,0,0,		0,c,0);
	arrViewVektor->AddPolygon(-a,0,0,		-b,0,-b,	0,c,0);
	arrViewVektor->AddPolygon(-b,0,-b,		0,0,-a,		0,c,0);
	arrViewVektor->AddPolygon(0,0,-a,		b,0,-b,		0,c,0);
	arrViewVektor->AddPolygon(b,0,-b,		a,0,0,		0,c,0);
	arrViewVektor->AddPolygon(a,0,0,	b,0,b,	0,0,a,	0,0,-a,	b,0,-b);
	arrViewVektor->AddPolygon(-a,0,0,	-b,0,-b,	0,0,-a,	0,0,a,	-b,0,b);
	arrViewVektor->SetPosition(0,0,0.25);
	arrViewVektor->SetOrientation(90,0,0);
	arrViewVektor->SetMaterial(0,1,1,0,0);


	//Spacer
	new FXLabel(root, "   ");

	vFrame = new FXVerticalFrame(root, LAYOUT_FIX_WIDTH, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0);
	gBox = new FXGroupBox(vFrame, "View", LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
		matrix = new FXMatrix(gBox,	3, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
			new FXLabel(matrix, "Azimuth", NULL, LAYOUT_CENTER_Y);
			slideAzimuth = new FXSlider(matrix, this, ID_SLIDER_AZIMUTH, LAYOUT_CENTER_Y | LAYOUT_FIX_WIDTH, 0, 0, 160, 0);
			slideAzimuth->setRange(0,360);
			slideAzimuth->setIncrement(1);
			slideAzimuth->setValue(0);
			lblAzimuth = new FXLabel(matrix, "0°");
			new FXLabel(matrix, "Elevation", NULL, LAYOUT_CENTER_Y);
			slideElevation = new FXSlider(matrix, this, ID_SLIDER_ELEVATION, LAYOUT_CENTER_Y | LAYOUT_FIX_WIDTH, 0, 0, 160, 0);
			slideElevation->setRange(-90,90);
			slideElevation->setIncrement(1);
			slideElevation->setValue(0);
			lblElevation = new FXLabel(matrix, "0°");
	gBox = new FXGroupBox(vFrame, "Frequency", LAYOUT_FILL_X | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
		matrix = new FXMatrix(gBox,	3, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
			new FXLabel(matrix, "f =", NULL, LAYOUT_CENTER_Y);
			slideFrequency = new FXSlider(matrix, this, ID_SLIDER_FREQ, LAYOUT_CENTER_Y | LAYOUT_FILL_X | LAYOUT_FIX_WIDTH, 0, 0, 192, 0);
			//slideFrequency is initialized in initWidgets!
			lblFrequency = new FXLabel(matrix, "x Hz");
	gBox = new FXGroupBox(vFrame, "Channels", LAYOUT_FILL_X | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
			listChannel = new FXListBox(gBox, this, ID_LISTBOX_CHANNEL, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
			listChannel->setNumVisible(1);
	gBox = new FXGroupBox(vFrame, "Settings", LAYOUT_FILL_X | FRAME_GROOVE, 0, 0, 0, 0, M, M, M, M, M, M);
		FXHorizontalFrame* hFrame = new FXHorizontalFrame(gBox, LAYOUT_FILL_X);
			matrix = new FXMatrix(hFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
				chkGrid = new FXCheckButton(matrix, "Axes", this, ID_CHK_GRID);
				chkVectors = new FXCheckButton(matrix, "Vectors", this, ID_CHK_VECTORS);
				chkNSPole = new FXCheckButton(matrix, "North/South pole", this, ID_CHK_NSPOLE);
			matrix = new FXMatrix(hFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
				radLin = new FXRadioButton(matrix, "linear", this, ID_RADIO_LIN);
				radLin->setCheck();
				radLog = new FXRadioButton(matrix, "logarithmic", this, ID_RADIO_LOG);
}

MainWindow::~MainWindow() {

	// Konfiguration sichern
	//storeConfig();

	getApp()->exit(0);
}

void MainWindow::create() {
    FXMainWindow::create();
	show(PLACEMENT_SCREEN);

	initWidgets();

	// TODO: Enable Drag'n'drop
	// dropEnable();
}

void MainWindow::initWidgets() {
	txtStatusBox1->setText("x Channel(s)");
	txtStatusBox2->setText("Resolution (A, B): xxx");
	txtStatusBox3->setText("Quantization: xxx");

	slideFrequency->setRange(0,0);
	slideFrequency->setIncrement(1);
	slideFrequency->setValue(0);

	theScreen->SetCameraPosition(1.2, 0, 0);
	theScreen->SetMouseCameraInteractionOff();
}

/*
   +--------------------+
   |                    |
   |  Event handlers    |
   |                    |
   +--------------------+
*/

long MainWindow::onDrop(FXObject*, FXSelector, void*) {
	FXMessageBox::information(this, MBOX_OK, "Info", "Yeah!");

	return 1;
}


long MainWindow::onCmdFileOpen(FXObject*, FXSelector, void*) {
	FXFileDialog fd(this, L"Open file", 0);
	
	if (sRecentDir != "") {
		fd.setDirectory(sRecentDir.c_str());
	}

	fd.setPatternList("OpenDAFF files (*.daff)\nAll files (*)\n");
	fd.setCurrentPattern(0);

	if (fd.execute() != 0) load(fd.getFilename().text());

	sRecentDir = fd.getDirectory().text();

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

long MainWindow::onSlideAzimuth(FXObject*, FXSelector, void*) {
	lblAzimuth->setText(   FXStringFormat("%d", slideAzimuth->getValue()) + "°"   );

	//the coordinates are:	x points to the camera, when both angles are 0 degree
	//						y then points upwards
	//						z then points to the left of the viewscreen
	//						az (0..360) is counted counterclockwise when seen from y>0
	//						el (-90..90) is counted from the x-z-plain, positive values mean y>0

	double az = (double)slideAzimuth->getValue() * PI/180;		//catch angles from sliders
	double el = (double)slideElevation->getValue() * PI/180;	
	double* CamCoords = theScreen->GetCameraPosition();			//catch camera coordinates
	double x = CamCoords[0];
	double y = CamCoords[1];
	double z = CamCoords[2];
	double r = 1.2;				//r stays constant as determined

	x = r * cos(el) * cos(az);
	z = - r * cos(el) * sin(az);
								//y stays constant, because only azimuth is changend

	theScreen->SetCameraPosition(x, y, z);

	theScreen->update();
	return 1;
}

long MainWindow::onWheelAzimuth(FXObject*, FXSelector, void*) {
	onSlideAzimuth(NULL,0,NULL);
	return 1;
}

long MainWindow::onSlideElevation(FXObject*, FXSelector, void*) {
	lblElevation->setText(   FXStringFormat("%d", slideElevation->getValue()) + "°"   );

	//the coordinates are:	x points to the camera, when both angles are 0 degree
	//						y then points upwards
	//						z then points to the left of the viewscreen
	//						az (0..360) is counted counterclockwise when seen from y>0
	//						el (-90..90) is counted from the x-z-plain, positive values mean y>0

	double az = (double)slideAzimuth->getValue() * PI/180;		//catch angles from sliders
	double el = (double)slideElevation->getValue() * PI/180;	
	double* CamCoords = theScreen->GetCameraPosition();			//catch camera coordinates
	double x = CamCoords[0];
	double y = CamCoords[1];
	double z = CamCoords[2];
	double r = 1.2;				//r stays constant as determined

	x = r * cos(el) * cos(az);
	y = r * sin(el);
	z = - r * cos(el) * sin(az);

	theScreen->SetCameraPosition(x, y, z);

	theScreen->update();
	return 1;
}

long MainWindow::onWheelElevation(FXObject*, FXSelector, void*) {
	onSlideElevation(NULL,0,NULL);
	return 1;
}

long MainWindow::onSlideFreq(FXObject*, FXSelector, void*) {
	thePlot->SetSelectedFrequency( slideFrequency->getValue() );

	//FXMessageBox::information(this, MBOX_OK, "Info", FXStringFormat("%d", fAvailableFreq[1]) );

	lblFrequency->setText( FXStringFormat("%.0f", fAvailableFreq[ slideFrequency->getValue() ]) + " Hz" );

	theScreen->update();
	return 1;
}

long MainWindow::onWheelFreq(FXObject*, FXSelector, void*) {
	onSlideFreq(NULL,0,NULL);
	return 1;
}

long MainWindow::onListChannels(FXObject*, FXSelector, void*) {
	//TODO

	return 1;
}

long MainWindow::onChkGrid(FXObject*, FXSelector, void*) {
	if (chkGrid->getCheck()) {
		theScreen->SwitchXYZAxesOn();
	} else {
		theScreen->SwitchXYZAxesOff();
	}

	theScreen->update();

	return 1;
}

long MainWindow::onChkVectors(FXObject*, FXSelector, void*) {
	if (chkVectors->getCheck()) {
		cylUpVektor->SetAlpha(1);
		arrUpVektor->SetMaterial(0,1,1,1,1);
		cylViewVektor->SetAlpha(1);
		arrViewVektor->SetMaterial(0,1,1,0,1);
	} else {
		cylUpVektor->SetAlpha(0);
		arrUpVektor->SetMaterial(0,1,1,1,0);
		cylViewVektor->SetAlpha(0);
		arrViewVektor->SetMaterial(0,1,1,0,0);
	}

	theScreen->update();

	return 1;
}

long MainWindow::onChkNSPole(FXObject*, FXSelector, void*) {
	if (chkNSPole->getCheck()) {
		sphNorth->SetAlpha(1);
		sphSouth->SetAlpha(1);
		cylPoleAxis->SetAlpha(1);
	} else {
		sphNorth->SetAlpha(0);
		sphSouth->SetAlpha(0);
		cylPoleAxis->SetAlpha(0);
	}

	theScreen->update();

	return 1;
}

long MainWindow::onRadLin(FXObject*, FXSelector, void*) {
	radLog->setCheck(false);

	thePlot->SetLogModeOff();

	theScreen->update();

	return 1;
}

long MainWindow::onRadLog(FXObject*, FXSelector, void*) {
	radLin->setCheck(false);

	thePlot->SetLogModeOn();

	theScreen->update();

	//TODO Scale is wrong for viewing

	return 1;
}

long MainWindow::onSomething(FXObject* sender, FXSelector sel, void* param) {
	FXMessageBox::information(this, MBOX_OK, "Info", "Something happened");
	return 1;
}

long MainWindow::onDNDEnter(FXObject* sender, FXSelector sel, void* param) {
	FXMessageBox::information(this, MBOX_OK, "Info", "DND Enter");
	return 1;
}

long MainWindow::onDNDDrop(FXObject* sender, FXSelector sel, void* param) {
	FXMessageBox::information(this, MBOX_OK, "Info", "DND Drop");
	return 1;
}

/*
   +--------------------+
   |                    |
   |  Other methods     |
   |                    |
   +--------------------+
*/

void MainWindow::load(const std::string& sFilename) {
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
	
	if (iError == 0) {
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
			
			slideFrequency->setRange(0, m_pContentMS->getNumFrequencies()-1);
			slideFrequency->setValue(0);

			fAvailableFreq = m_pContentMS->getFrequencies();

			thePlot->SetData(m_pContentMS);
			thePlot->SetSelectedFrequency(0);

			const DAFFProperties* props = m_pReader->getProperties();
			txtStatusBox1->setText(FXStringFormat("%d", props->getNumberOfChannels()) + " Channel(s)");
			txtStatusBox2->setText("Resolution (A, B): " + FXStringFormat("%.0f°", props->getAlphaResolution()) + ", " + FXStringFormat("%.0f°", props->getBetaResolution()));
			txtStatusBox3->setText(DAFFUtils::StrQuantizationType(props->getQuantization()).c_str());
			
			
			lblFrequency->setText( FXStringFormat("%d", m_pContentMS->getFrequencies()[0]) + " Hz");

			listChannel->clearItems();
			for (int i=1; i <= props->getNumberOfChannels(); i++) {
				if( props->getChannelLabel(i-1).length() == 0) {
					listChannel->appendItem("Channel " + FXStringFormat("%d", i));
				} else {
					listChannel->appendItem("Ch. " + FXStringFormat("%d", i) + " - " + props->getChannelLabel(i-1).c_str());
				}
			}
			listChannel->setNumVisible(props->getNumberOfChannels());

			menuInformation->enable();
		}
		
		if (pNewReader->getContentType() == DAFF_DFT_SPECTRUM) {
			m_pContentIR = NULL;
			m_pContentMS = NULL;
			m_pContentDFT = dynamic_cast<DAFFContentDFT*>( m_pReader->getContent() );
			
			slideFrequency->setRange(0, m_pContentDFT->getNumDFTCoeffs()-1);
			slideFrequency->setValue(0);

			for (int i=0;i<m_pContentDFT->getNumDFTCoeffs();i++)
				fAvailableFreq.push_back(i);

			thePlot->SetData(m_pContentDFT);
			thePlot->SetSelectedFrequency(0);

			const DAFFProperties* props = m_pReader->getProperties();
			txtStatusBox1->setText(FXStringFormat("%d", props->getNumberOfChannels()) + " Channel(s)");
			txtStatusBox2->setText("Resolution (A, B): " + FXStringFormat("%.0f°", props->getAlphaResolution()) + ", " + FXStringFormat("%.0f°", props->getBetaResolution()));
			txtStatusBox3->setText(DAFFUtils::StrQuantizationType(props->getQuantization()).c_str());
			
			
			lblFrequency->setText( FXStringFormat("%d", 0) + "TODO Hz");

			listChannel->clearItems();
			for (int i=1; i <= props->getNumberOfChannels(); i++) {
				if( props->getChannelLabel(i-1).length() == 0) {
					listChannel->appendItem("Channel " + FXStringFormat("%d", i));
				} else {
					listChannel->appendItem("Ch. " + FXStringFormat("%d", i) + " - " + props->getChannelLabel(i-1).c_str());
				}
			}
			listChannel->setNumVisible(props->getNumberOfChannels());

			menuInformation->enable();
		}

	} else {
		delete pNewReader;
		FXMessageBox::error(this, MBOX_OK, "Error", DAFFUtils::StrError(iError).c_str());
	}
}