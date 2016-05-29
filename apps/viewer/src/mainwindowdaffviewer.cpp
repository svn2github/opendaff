#include "mainwindowdaffviewer.h"
#include "ui_mainwindowdaffviewer.h"

#include <qfiledialog.h>
#include <qsettings.h>
#include <iostream>
#include <qerrormessage.h>

#include <DAFF.h>

MainWindowDAFFViewer::MainWindowDAFFViewer( QWidget *parent, QString sPath )
  : QMainWindow( parent )
  , ui( new Ui::DAFFViewer )
  , m_pDAFFReader( DAFFReader::create() )
{
    ui->setupUi(this);
    showMaximized();

    connect( this, SIGNAL( readDAFF(const DAFFReader*)),ui->groupBox,SLOT(on_readDAFF(const DAFFReader*)) );
}

MainWindowDAFFViewer::~MainWindowDAFFViewer()
{
    m_qSettings.setValue( "geometry", saveGeometry() );
    m_qSettings.setValue( "windowState", saveState() );

    delete ui;

    if( m_pDAFFReader->isFileOpened() )
        m_pDAFFReader->closeFile();

    delete m_pDAFFReader;
}

void MainWindowDAFFViewer::on_actionOpen_triggered()
{
    QString sAppDir = QApplication::applicationDirPath().left(1);
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

void MainWindowDAFFViewer::on_actionQuit_triggered()
{
    close();
}

void MainWindowDAFFViewer::OpenDAFFFile( QString sPath, bool bQuiet )
{
    if( m_pDAFFReader->isFileOpened() )
        m_pDAFFReader->closeFile();

    QFileInfo oPassedFile( sPath );
    int iError = DAFF_NO_ERROR;
    if( !bQuiet && ( iError =  m_pDAFFReader->openFile( oPassedFile.absoluteFilePath().toStdString() ) != DAFF_NO_ERROR ) )
    {
        QErrorMessage qe;
        QString sErrors = "DAFF error while opening passed file '" + oPassedFile.fileName() + "': " + QString( DAFFUtils::StrError( iError ).c_str() );
        qe.showMessage( sErrors );
        return;
    }

    emit readDAFF( m_pDAFFReader );
}
