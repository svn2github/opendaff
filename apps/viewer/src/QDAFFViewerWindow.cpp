#include "QDAFFViewerWindow.h"
#include <ui_QDAFFViewerWindow.h>
#include "QDAFFVTKWidget.h"

#include <QFileDialog>
#include <qsettings.h>
#include <iostream>
#include <qerrormessage.h>

#include <DAFF.h>

QDAFFViewerWindow::QDAFFViewerWindow( QWidget *parent, QString sPath )
  : QMainWindow( parent )
  , ui( new Ui::DAFFViewer )
  , m_pDAFFReader( DAFFReader::create() )
{
    ui->setupUi(this);
    showMaximized();

    connect( this, SIGNAL( readDAFF( const DAFFReader* ) ), ui->groupBox_Reader, SLOT( on_readDAFF( const DAFFReader* ) ) );
    connect( this, SIGNAL( readDAFF( const DAFFReader* ) ), ui->DAFF3DPlot_VTKWidget, SLOT( on_readDAFF( const DAFFReader* ) ) );
    connect( this, SIGNAL( readDAFF( const DAFFReader* ) ), ui->tableView_Metadata, SLOT( on_readDAFF( const DAFFReader* ) ) );
    connect( this, SIGNAL( readDAFF( const DAFFReader* ) ), ui->tableView_Properties, SLOT( on_readDAFF( const DAFFReader* ) ) );

	ui->DAFFStatusBar->showMessage( QString( "No DAFF file loaded." ) );

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
}

void QDAFFViewerWindow::on_actionOpen_triggered()
{
	QString sAppDir = QApplication::applicationDirPath().left( 1 );
    QString sOpenDialogLastDirectory = m_qSettings.value( "OpenDialogLastDirectory", sAppDir ).toString();
    QStringList lHistory = m_qSettings.value( "OpenFileHistory" ).toStringList();

    QDir oOpenDialogLastDirectory( sOpenDialogLastDirectory );
    if( oOpenDialogLastDirectory.exists() == false )
        oOpenDialogLastDirectory.setCurrent( sAppDir );

    QFileDialog fd;
    fd.setNameFilter( "DAFF files (*.daff)" );
    fd.setViewMode( QFileDialog::Detail );
    fd.setFileMode( QFileDialog::ExistingFile );
    fd.setHistory( lHistory );
    fd.setDirectory( oOpenDialogLastDirectory.absolutePath() );
    if( fd.exec() )
    {
        QStringList lDAFFFiles = fd.selectedFiles();
        if( lDAFFFiles.empty() == false )
            OpenDAFFFile( lDAFFFiles[0] );
    }
}

void QDAFFViewerWindow::on_actionQuit_triggered()
{
    close();
}

void QDAFFViewerWindow::OpenDAFFFile( QString sPath, bool bQuiet )
{
    if( m_pDAFFReader->isFileOpened() )
        m_pDAFFReader->closeFile();

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
