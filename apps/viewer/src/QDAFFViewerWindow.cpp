#include "QDAFFViewerWindow.h"
#include <ui_QDAFFViewerWindow.h>
#include "QDAFFVTKWidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QErrorMessage>
#include <QLayout>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QShortcut>

#include <DAFF.h>

#include <iostream>

QDAFFViewerWindow::QDAFFViewerWindow( QWidget *parent, QString sPath )
	: QMainWindow( parent )
	, ui( new Ui::DAFFViewer )
	, m_pDAFFReader( DAFFReader::create() )
	, m_qSettings( "ITA", "DAFFVIewer" )
{
    ui->setupUi(this);
    showMaximized();
	
	// Global shortcuts
	m_vShortcuts.push_back( new QShortcut( QKeySequence( Qt::Key_Left ), this ) );
	m_vShortcuts.back()->setContext( Qt::ApplicationShortcut );
	connect( m_vShortcuts.back(), SIGNAL( activated() ), this, SLOT( IncreaseAlpha() ) );
	m_vShortcuts.push_back( new QShortcut( QKeySequence( Qt::Key_Right ), this ) );
	m_vShortcuts.back()->setContext( Qt::ApplicationShortcut );
	connect( m_vShortcuts.back(), SIGNAL( activated() ), this, SLOT( DecreaseAlpha() ) );
	m_vShortcuts.push_back( new QShortcut( QKeySequence( Qt::Key_Up ), this ) );
	m_vShortcuts.back()->setContext( Qt::ApplicationShortcut );
	connect( m_vShortcuts.back(), SIGNAL( activated() ), this, SLOT( IncreaseBeta() ) );
	m_vShortcuts.push_back( new QShortcut( QKeySequence( Qt::Key_Down ), this ) );
	m_vShortcuts.back()->setContext( Qt::ApplicationShortcut );
	connect( m_vShortcuts.back(), SIGNAL( activated() ), this, SLOT( DecreaseBeta() ) );
	m_vShortcuts.push_back( new QShortcut( QKeySequence( Qt::Key_Plus ), this ) );
	m_vShortcuts.back()->setContext( Qt::ApplicationShortcut );
	connect( m_vShortcuts.back(), SIGNAL( activated() ), this, SLOT( IncreaseFrequency() ) );
	m_vShortcuts.push_back( new QShortcut( QKeySequence( Qt::Key_Minus ), this ) );
	m_vShortcuts.back()->setContext( Qt::ApplicationShortcut );
	connect( m_vShortcuts.back(), SIGNAL( activated() ), this, SLOT( DecreaseFrequency() ) );

	connect( this, SIGNAL( readDAFF( const DAFFReader* ) ), ui->groupBox_Reader, SLOT( on_readDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( closeDAFF() ), ui->groupBox_Reader, SLOT( on_closeDAFF() ) );

	connect( this, SIGNAL( readDAFF( const DAFFReader* ) ), ui->DAFF3DPlot_VTKWidget, SLOT( on_readDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( closeDAFF() ), ui->DAFF3DPlot_VTKWidget, SLOT( on_closeDAFF() ) );

	connect( this, SIGNAL( readDAFF( const DAFFReader* ) ), ui->tableView_Metadata, SLOT( on_readDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( closeDAFF() ), ui->tableView_Metadata, SLOT( on_closeDAFF() ) );

	connect( this, SIGNAL( readDAFF( const DAFFReader* ) ), ui->tableView_Properties, SLOT( on_readDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( closeDAFF() ), ui->tableView_Properties, SLOT( on_closeDAFF() ) );

	ui->DAFFStatusBar->showMessage( "No DAFF file loaded." );

	m_qSettings.setValue( "RequestedPath", sPath );

	if( sPath.isEmpty() == false )
		OpenDAFFFile( sPath, false );
}

QDAFFViewerWindow::~QDAFFViewerWindow()
{
    m_qSettings.setValue( "geometry", saveGeometry() );
    m_qSettings.setValue( "windowState", saveState() );

    delete ui;

    if( m_pDAFFReader->isFileOpened() )
        m_pDAFFReader->closeFile();

    delete m_pDAFFReader;

	for( size_t i = 0; i < m_vShortcuts.size(); i++ )
		delete m_vShortcuts[ i ];
}

void QDAFFViewerWindow::on_actionOpen_triggered()
{
    QFileDialog fd;
    fd.setNameFilter( "DAFF files (*.daff)" );
    fd.setViewMode( QFileDialog::Detail );
	fd.setFileMode( QFileDialog::ExistingFile );

	QDir oOpenDialogLastDirectory( m_qSettings.value( "OpenDialogLastDirectory" ).toString() );
	if( oOpenDialogLastDirectory.exists() )
		fd.setDirectory( oOpenDialogLastDirectory );
	else
		fd.setDirectory( QDir( QApplication::applicationDirPath() ) );

    if( fd.exec() )
    {
        QStringList lDAFFFiles = fd.selectedFiles();
        if( lDAFFFiles.empty() == false )
			OpenDAFFFile( lDAFFFiles[ 0 ] );

		QString sOpenDialogLastDirectory = fd.directory().absolutePath();
		m_qSettings.setValue( "OpenDialogLastDirectory", sOpenDialogLastDirectory );
    }
}

void QDAFFViewerWindow::on_actionQuit_triggered()
{
    close();
}

void QDAFFViewerWindow::OpenDAFFFile( QString sPath, bool bQuiet )
{
	if( m_pDAFFReader->isFileOpened() )
	{
		emit closeDAFF();
		m_pDAFFReader->closeFile();
	}

    QFileInfo oPassedFile( sPath );
    int iError = DAFF_NO_ERROR;
    if( ( iError = m_pDAFFReader->openFile( oPassedFile.absoluteFilePath().toStdString() ) ) != DAFF_NO_ERROR )
	{
		QErrorMessage qe;
		QString sError = "Could not open requested file '" + oPassedFile.fileName() + "': " + QString( DAFFUtils::StrError( iError ).c_str() );
		qe.showMessage( sError );
		if( !bQuiet )
			qe.exec();

		ui->DAFFStatusBar->showMessage( sError );

        return;
    }
	else
	{
		QString sMsg = QString( "DAFF file '" + oPassedFile.fileName() + "' sucessfully loaded." );
		ui->DAFFStatusBar->showMessage( sMsg );
		emit readDAFF( m_pDAFFReader );
	}
}

void QDAFFViewerWindow::on_actionOpenDAFFWebsite_triggered()
{
    QUrl urlOpenDAFFWebsite( "http://www.opendaff.org" );
    QDesktopServices::openUrl( urlOpenDAFFWebsite );
}

void QDAFFViewerWindow::on_actionCreate_triggered()
{
	QString msg;
	msg += QString( "To create DAFF files you need Matlab. Find more Information at:\t\n" );
	msg += QString( "http://www.opendaff.org/opendaff_content.html\t\n" );

	QMessageBox d;
	d.setText( msg );
	d.layout()->setSpacing( 9 );
	d.exec();
}

void QDAFFViewerWindow::on_actionAboutOpenDAFF_triggered()
{
    QString msg;
    msg += QString( "OpenDAFF is a free and open source software package for directional audio content like directivities of sound sources (e.g. loudspeakers, musical instruments) and sound receivers (e.g. microphones, head-related transfer functions, HRIRs/HRTFs). OpenDAFF enables a simple exchange, representation and efficient storage of such directional content in form of a single DAFF file (*.DAFF)\t\n" );

    QMessageBox d;
    d.setText( msg );
    d.layout()->setSpacing( 9 );
    d.exec();
}

void QDAFFViewerWindow::on_actionAboutDAFFViewer_triggered()
{
    QString msg;
    msg += QString( "The DAFFViewer application is a grpahical user interface to DAFF content,\t\n" );
    msg += QString( "the Direction Audio File Format.\t\n" );
    msg += QString( "The DAFFViewer is licensed under the Apache License Version 2.0 and can be freely used for" );
    msg += QString( "private, academic and commercial purposes.\t\n\n" );
    msg += QString( "DAFF C++ library version: v1.7\t\n" );
    msg += QString( "DAFFViewer version: v1.7\t\n\n" );

    QMessageBox d;
    d.setText( msg );
    d.layout()->setSpacing( 9 );
    d.exec();

}

void QDAFFViewerWindow::on_actionClose_triggered()
{
    emit closeDAFF();
	m_pDAFFReader->closeFile();
}

void QDAFFViewerWindow::on_actionDownload_triggered()
{
    QUrl urlDownloadWebsite( "http://sourceforge.net/projects/opendaff/files/Content" );
    QDesktopServices::openUrl( urlDownloadWebsite );
}

void QDAFFViewerWindow::IncreaseAlpha()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	assert( m_fShowAlphaDeg >= 0.0f && m_fShowAlphaDeg < 360.0f );
	m_fShowAlphaDeg += fmodf( m_pDAFFReader->getProperties()->getAlphaResolution(), 360.0f );
}

void QDAFFViewerWindow::DecreaseAlpha()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	assert( m_fShowAlphaDeg >= 0.0f && m_fShowAlphaDeg < 360.0f );
	m_fShowAlphaDeg -= m_pDAFFReader->getProperties()->getAlphaResolution();
	if( m_fShowAlphaDeg < 0 )
		m_fShowAlphaDeg += 180.0f;
}

void QDAFFViewerWindow::IncreaseBeta()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	assert( m_fShowBeta >= 0.0f && m_fShowBeta <= 180.0f );
	m_fShowBeta += fmodf( m_pDAFFReader->getProperties()->getBetaResolution(), 180.0f );
}

void QDAFFViewerWindow::DecreaseBeta()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	assert( m_fShowBeta >= 0.0f && m_fShowBeta <= 180.0f );
	m_fShowBeta -= m_pDAFFReader->getProperties()->getAlphaResolution();
	if( m_fShowBeta < 0 )
		m_fShowBeta += 180.0f;
}

void QDAFFViewerWindow::IncreaseFrequency()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	m_fShowFrequency;
}

void QDAFFViewerWindow::DecreaseFrequency()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	m_fShowFrequency;
}

void QDAFFViewerWindow::ShowChannel( int iChannel )
{
	m_iShowChannel = iChannel;
}

void QDAFFViewerWindow::SetDAFFObjectView()
{
	m_iShowDAFFView = DAFF_OBJECT_VIEW;
}

void QDAFFViewerWindow::SetDAFFDataView()
{
	m_iShowDAFFView = DAFF_DATA_VIEW;
}
