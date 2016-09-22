/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *
 *  ------------------------------------------------------------------------------------
 *
 *  Visit the OpenDAFF website: http://www.opendaff.org
 *
 *  ------------------------------------------------------------------------------------
 *
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use the OpenDAFF software package except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#include "QDAFFViewerWindow.h"
#include <QApplication>
#include <QIcon>

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	QCoreApplication::setOrganizationName( "OpenDAFF" );
	QCoreApplication::setOrganizationDomain( "opendaff.org" );
	QCoreApplication::setApplicationName( "DAFFViewer" );
		
    QString sPath;
	if( argc == 2 )
		sPath = argv[ 1 ];

	QIcon icon;
	icon.addFile( QStringLiteral( ":/icons/DAFFViewer_Window_Icon_16x16.gif" ), QSize(), QIcon::Normal, QIcon::On );
	a.setWindowIcon( icon );

	QDAFFViewerWindow w( NULL, sPath );
	w.show();


    return a.exec();
}
