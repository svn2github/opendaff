#include "mainwindowdaffviewer.h"
#include "ui_mainwindowdaffviewer.h"

#include <qfiledialog.h>
#include <qsettings.h>
#include <iostream>
#include <qerrormessage.h>

#include <DAFF.h>

MainWindowDAFFViewer::MainWindowDAFFViewer(QWidget *parent, QString sPath ) :
    QMainWindow(parent),
    ui(new Ui::DAFFViewer)
{
    ui->setupUi(this);

    DAFFReader* m_pDAFFReader = DAFFReader::create();

    QFileInfo oPassedFile( sPath );
    int iError = DAFF_NO_ERROR;
    if( ( iError =  m_pDAFFReader->openFile( oPassedFile.absoluteFilePath().toStdString() ) != DAFF_NO_ERROR ) )
    {
        QErrorMessage qe;
        QString sErrors = "DAFF error while opening passed file '" + oPassedFile.fileName() + "': " + QString( DAFFUtils::StrError( iError ).c_str() );
        qe.showMessage( sErrors );
    }
    else
    {
        // SIGNAL EVENT FILE OPENED
    }
}

MainWindowDAFFViewer::~MainWindowDAFFViewer()
{
    delete ui;
}

void MainWindowDAFFViewer::on_actionOpen_triggered()
{

    QSettings settings;

    QString sAppDir = QApplication::applicationDirPath().left(1);
    QString sOpenDialogLastDirectory = settings.value( "OpenDialogLastDirectory", sAppDir ).toString();

    QDir oOpenDialogLastDirectory( sOpenDialogLastDirectory );
    if( oOpenDialogLastDirectory.exists() == false )
        oOpenDialogLastDirectory.setCurrent( sAppDir );

    QFileDialog fd;
    fd.setNameFilter( "DAFF files (*.daff)" );
    fd.setViewMode( QFileDialog::Detail );
    fd.setFileMode( QFileDialog::ExistingFile );
    fd.setDirectory( oOpenDialogLastDirectory.absolutePath() );
    if( fd.exec() )
    {
        QStringList lDAFFFiles = fd.selectedFiles();

        if( lDAFFFiles.empty() == false)
            std::cout << lDAFFFiles[0].toStdString() << std::endl;
    }
}

void MainWindowDAFFViewer::on_actionQuit_triggered()
{
    close();
}
