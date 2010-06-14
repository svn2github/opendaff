/*
 *  OpenDAFF - A free, open-source software package for directional audio data
 *  
 *  File:		Main.cpp
 *  Purpose:	DAFF viewer main file
 *
 */

// $Id: Main.cpp,v 1.2 2010/02/08 18:55:49 fwefers Exp $

#include <fx.h>
#include "Globals.h"
#include "MainWindow.h"

int main(int argc, char* argv[]) {
	// Make the application
	FXApp app(APPNAME);

	// Initialize it
	app.init(argc,argv);

	// Create the main window
	MainWindow* pMainWindow = new MainWindow(&app);

	// Create the application
	app.create();

	//Load Configuration and Preferences from INI-File
	//pMainWindow->init();
	//pMainWindow->loadConfig();

	// Go for it!
	return app.run();
}
