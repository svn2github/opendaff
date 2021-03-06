#include "QDAFFViewerWindow.h"
#include <ui_QDAFFViewerWindow.h>
#include "QDAFFVTKWidget.h"
#include "QDAFFDialogExport2DPlot.h"
#include "QDAFFDialogExport3DPlotImage.h"
#include "QDAFFDialogExport3DPlotImageSeries.h"
#include "QDAFF2DPlot.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QErrorMessage>
#include <QLayout>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QShortcut>
#include <QPalette>

#include <DAFF.h>

#include <iostream>

QDAFFViewerWindow::QDAFFViewerWindow( QWidget *parent, QString sPath )
	: QMainWindow( parent )
	, ui( new Ui::DAFFViewer )
	, m_pDAFFReader( DAFFReader::create() )
	, m_qSettings( "ITA", "DAFFViewer" )
	, m_iShowChannel( 0 )
	, m_iShowFrequencyIndex( 0 )
	, m_dShowTimeSample( 0.0f )
	, m_dShowAlphaDeg( 0.0f )
	, m_dShowBetaDeg( 90.0f )
	, m_dShowPhiDeg( 0.0f )
	, m_dShowThetaDeg( 0.0f )
	, m_dPhiThetaIncrementDeg( 1.0f )
{
	ui->setupUi( this );

	RestoreWindowSize();

	// Read DAFF header conns
	connect( this, SIGNAL( SignalReadDAFF( const DAFFReader* ) ), ui->groupBox_Reader, SLOT( ReadDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( SignalReadDAFF( const DAFFReader* ) ), ui->tableView_Metadata, SLOT( ReadDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( SignalReadDAFF( const DAFFReader* ) ), ui->tableView_Properties, SLOT( ReadDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( SignalReadDAFF( const DAFFReader* ) ), ui->tableView_Content, SLOT( ReadDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( SignalReadDAFF( const DAFFReader* ) ), ui->graphicsView_2DDAFFPlot, SLOT( ReadDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( SignalReadDAFF( const DAFFReader* ) ), ui->DAFF3DPlot_VTKWidget, SLOT( ReadDAFF( const DAFFReader* ) ) );

	// Close DAFF file conns
	connect( this, SIGNAL( SignalCloseDAFF() ), ui->groupBox_Reader, SLOT( CloseDAFF() ) );
	connect( this, SIGNAL( SignalCloseDAFF() ), ui->tableView_Metadata, SLOT( CloseDAFF() ) );
	connect( this, SIGNAL( SignalCloseDAFF() ), ui->tableView_Properties, SLOT( CloseDAFF() ) );
	connect( this, SIGNAL( SignalCloseDAFF() ), ui->tableView_Content, SLOT( CloseDAFF() ) );
	connect( this, SIGNAL( SignalCloseDAFF() ), ui->graphicsView_2DDAFFPlot, SLOT( CloseDAFF() ) );
	connect( this, SIGNAL( SignalCloseDAFF() ), ui->DAFF3DPlot_VTKWidget, SLOT( CloseDAFF() ) );

	// Load & prepare DAFF content conns
	connect( this, SIGNAL( SignalContentLoaded( const DAFFContent* ) ), this, SLOT( LoadContent( const DAFFContent* ) ) );

	// Frequency index conns
	connect( this, SIGNAL( SignalFrequencyIndexChanged( int ) ), ui->DAFF3DPlot_VTKWidget, SLOT( ChangeFrequencyIndex( int ) ) );
	connect( this, SIGNAL( SignalFrequencyIndexChanged( int ) ), ui->spinBox_FrequencyIndex, SLOT( setValue( int ) ) );
	connect( this, SIGNAL( SignalFrequencyIndexChanged( int ) ), ui->comboBox_FrequencySelector, SLOT( setCurrentIndex( int ) ) );
	connect( this, SIGNAL( SignalFrequencyIndexChanged( int ) ), ui->DAFF3DPlot_VTKWidget, SLOT( ChangeFrequencyIndex( int ) ) );
	connect( ui->spinBox_FrequencyIndex, SIGNAL( valueChanged( int ) ), this, SLOT( ChangeFrequencyIndex( int ) ) );
	connect( ui->comboBox_FrequencySelector, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ChangeFrequencyIndex( int ) ) );
	connect( this, SIGNAL( SignalFrequencyIndexChanged( int ) ), ui->graphicsView_2DDAFFPlot, SLOT( ChangeFrequencyIndex( int ) ) );

	// Channel index conns
	connect( this, SIGNAL( SignalChannelIndexChanged( int ) ), ui->DAFF3DPlot_VTKWidget, SLOT( ChangeChannelIndex( int ) ) );
	connect( this, SIGNAL( SignalChannelIndexChanged( int ) ), ui->spinBox_ChannelIndex, SLOT( setValue( int ) ) );
	connect( this, SIGNAL( SignalChannelIndexChanged( int ) ), ui->graphicsView_2DDAFFPlot, SLOT( ChangeChannelIndex( int ) ) );
	connect( ui->spinBox_ChannelIndex, SIGNAL( valueChanged( int ) ), this, SLOT( ChangeChannelIndex( int ) ) );

	// Alpha and beta conns
	connect( ui->doubleSpinBox_Alpha, SIGNAL( valueChanged( double ) ), this, SLOT( ChangeAlpha( double ) ) );
	connect( this, SIGNAL( SignalAlphaChanged( double ) ), ui->doubleSpinBox_Alpha, SLOT( setValue( double ) ) );
	connect( this, SIGNAL( SignalAlphaChanged( double ) ), ui->DAFF3DPlot_VTKWidget, SLOT( ChangeAlpha( double ) ) );
	connect( ui->doubleSpinBox_Beta, SIGNAL( valueChanged( double ) ), this, SLOT( ChangeBeta( double ) ) );
	connect( this, SIGNAL( SignalBetaChanged( double ) ), ui->doubleSpinBox_Beta, SLOT( setValue( double ) ) );
	connect( this, SIGNAL( SignalBetaChanged( double ) ), ui->DAFF3DPlot_VTKWidget, SLOT( ChangeBeta( double ) ) );
	
	// Record index conns
	connect( this, SIGNAL( SignalRecordIndexChanged( int ) ), ui->spinBox_RecordIndex, SLOT( setValue( int ) ) );
	connect( this, SIGNAL( SignalRecordIndexChanged( int ) ), ui->graphicsView_2DDAFFPlot, SLOT( ChangeRecordIndex( int ) ) );
	connect( ui->spinBox_RecordIndex, SIGNAL( valueChanged( int ) ), this, SLOT( ChangeRecordIndex( int ) ) );

	// Phi and theta conns
	connect( ui->doubleSpinBox_Phi, SIGNAL( valueChanged( double ) ), this, SLOT( ChangePhi( double ) ) );
	connect( this, SIGNAL( SignalPhiChanged( double ) ), ui->doubleSpinBox_Phi, SLOT( setValue( double ) ) );
	connect( this, SIGNAL( SignalPhiChanged( double ) ), ui->DAFF3DPlot_VTKWidget, SLOT( ChangePhi( double ) ) );
	connect( ui->doubleSpinBox_Theta, SIGNAL( valueChanged( double ) ), this, SLOT( ChangeTheta( double ) ) );
	connect( this, SIGNAL( SignalThetaChanged( double ) ), ui->doubleSpinBox_Theta, SLOT( setValue( double ) ) );
	connect( this, SIGNAL( SignalThetaChanged( double ) ), ui->DAFF3DPlot_VTKWidget, SLOT( ChangeTheta( double ) ) );
	connect( this, SIGNAL( SignalPhiAndThetaOutOfBounds( bool ) ), this, SLOT( SetPhiAndThetaOutOfBoundsIndicator( bool ) ) );

	// 2D conns
	// .. nothing further yet.

	// 3D conns
	connect( this, SIGNAL( SignalExportScreenshotPNG( QString ) ), ui->DAFF3DPlot_VTKWidget, SLOT( ExportScreenshotPNG( QString ) ) );
   

	// Menu settings
	ui->action3DSphericalShowArrows->setChecked( m_qSettings.value( "Settings/3DPlot/Spherical/ShowArrows", true ).toBool() );
	ui->action3DSphericalShowPoles->setChecked( m_qSettings.value( "Settings/3DPlot/Spherical/ShowPoles", true ).toBool() );
	ui->action3DSphericalShowGrid->setChecked( m_qSettings.value( "Settings/3DPlot/Spherical/ShowGrid", false ).toBool() );
	ui->action3DSphericalShowEquator->setChecked( m_qSettings.value( "Settings/3DPlot/Spherical/ShowEquator", true ).toBool() );
	ui->action3DSphericalShowMeridians->setChecked( m_qSettings.value( "Settings/3DPlot/Spherical/ShowMeridians", false ).toBool() );
	ui->action3DSphericalUsePhaseColorMap->setChecked( m_qSettings.value( "Settings/3DPlot/Spherical/UsePhaseColorMap", false ).toBool() );
	ui->action3DUseLogScale->setChecked( m_qSettings.value( "Settings/3DPlot/LogScale", true ).toBool() );
	ui->action3DSphericalNormalize->setChecked( m_qSettings.value( "Settings/3DPlot/Spherical/Normalize", false ).toBool() );
	ui->action3DSphericalNormalizeFrequenciesIndividually->setChecked( m_qSettings.value( "Settings/3DPlot/Spherical/NormalizeFreqsIndividually", false ).toBool() );
	ui->action3DCarpetShowWarp->setChecked( m_qSettings.value( "Settings/3DPlot/Carpet/Warp", true ).toBool() );

	ui->DAFF3DPlot_VTKWidget->SetLogScale( ui->action3DUseLogScale->isChecked() );
	ui->DAFF3DPlot_VTKWidget->SetArrowsVisible( ui->action3DSphericalShowArrows->isChecked() );
	ui->DAFF3DPlot_VTKWidget->SetPolesVisible( ui->action3DSphericalShowPoles->isChecked() );
	ui->DAFF3DPlot_VTKWidget->SetGridVisible( ui->action3DSphericalShowGrid->isChecked() );
	ui->DAFF3DPlot_VTKWidget->SetEquatorVisible( ui->action3DSphericalShowEquator->isChecked() );
	ui->DAFF3DPlot_VTKWidget->SetMeridiansVisible( ui->action3DSphericalShowMeridians->isChecked() );
	ui->DAFF3DPlot_VTKWidget->SetPhaseColorMap( ui->action3DSphericalUsePhaseColorMap->isChecked() );
	ui->DAFF3DPlot_VTKWidget->SetNormalize( ui->action3DSphericalNormalize->isChecked() );
	ui->DAFF3DPlot_VTKWidget->SetNormalizeFrequenciesIndividually( ui->action3DSphericalNormalizeFrequenciesIndividually->isChecked() );
	ui->DAFF3DPlot_VTKWidget->SetCarpetPlotWarp( ui->action3DCarpetShowWarp->isChecked() );

	// Other

	ui->DAFFStatusBar->showMessage( "No DAFF file loaded. Shortcut for opening DAFF files: 'o' as in 'open'. 'Esc' will exit DAFFViewer." );

	if( !ui->DAFF3DPlot_VTKWidget->SupportsOpenGL() )
		ui->DAFFStatusBar->showMessage( "Your graphics card does not support OpenGL 4 shader. 3D plot window will be disabled." );

	m_qSettings.setValue( "RequestedPath", sPath );
	m_qSettings.value( "PhiThetaIncrementDeg", m_dPhiThetaIncrementDeg );
	
	if( sPath.isEmpty() == false )
		OpenDAFFFile( sPath, false );

	UpdateRecentFilesActions();

	ChangePhiAndTheta( m_dShowPhiDeg, m_dShowThetaDeg ); // Initially, show default frontal record

}

QDAFFViewerWindow::~QDAFFViewerWindow()
{
	m_qSettings.setValue( "WindowGeometry", saveGeometry() );
	m_qSettings.setValue( "WindowState", saveState() );

	m_qSettings.setValue( "PhiThetaIncrementDeg", m_dPhiThetaIncrementDeg );

	m_qSettings.setValue( "Settings/3DPlot/Spherical/ShowArrows", ui->action3DSphericalShowArrows->isChecked() );
	m_qSettings.setValue( "Settings/3DPlot/Spherical/ShowPoles", ui->action3DSphericalShowPoles->isChecked() );
	m_qSettings.setValue( "Settings/3DPlot/Spherical/ShowGrid", ui->action3DSphericalShowGrid->isChecked() );
	m_qSettings.setValue( "Settings/3DPlot/Spherical/ShowEquator", ui->action3DSphericalShowEquator->isChecked() );
	m_qSettings.setValue( "Settings/3DPlot/Spherical/ShowMeridians", ui->action3DSphericalShowMeridians->isChecked() );
	m_qSettings.setValue( "Settings/3DPlot/Spherical/UsePhaseColorMap", ui->action3DSphericalUsePhaseColorMap->isChecked() );
	m_qSettings.setValue( "Settings/3DPlot/LogScale", ui->action3DUseLogScale->isChecked() );
	m_qSettings.setValue( "Settings/3DPlot/Spherical/Normalize", ui->action3DSphericalNormalize->isChecked() );
	m_qSettings.setValue( "Settings/3DPlot/Spherical/NormalizeFreqsIndividually", ui->action3DSphericalNormalizeFrequenciesIndividually->isChecked() );
	m_qSettings.setValue( "Settings/3DPlot/Carpet/Warp", ui->action3DCarpetShowWarp->isChecked() );

    delete ui;

    if( m_pDAFFReader->isFileOpened() )
        m_pDAFFReader->closeFile();

    delete m_pDAFFReader;
}

void QDAFFViewerWindow::RestoreWindowSize()
{
	restoreGeometry( m_qSettings.value( "WindowGeometry" ).toByteArray() );
	restoreState( m_qSettings.value( "WindowState" ).toByteArray() );
}

void QDAFFViewerWindow::UpdateRecentFilesActions()
{
	QStringList vsRecentFiles = m_qSettings.value( "RecentFiles" ).toStringList();
	QList< QAction* > lActions = ui->menuRecent->actions();

	const int iMaxElems = 9;

	// Clear?
	if( vsRecentFiles.empty() && lActions.size() > 2 )
	{
		// Skip first to elems ('Clear' action and horizontal devider)
		for( size_t i = 2; i < lActions.size(); i++ )
			ui->menuRecent->removeAction( lActions[ int( i ) ] );
	}

	// Cyclic shift with pop front
	for( size_t i = 0; i < std::min( int( iMaxElems ), int( vsRecentFiles.size() ) ); i++ )
	{
		QFileInfo oCurrentFileInfo( vsRecentFiles.at( int( i ) ) );
		QAction* pAction = new QAction( this );

		pAction->setShortcut( Qt::Key_1 + char( i ) );
		pAction->setText( oCurrentFileInfo.fileName() );
		pAction->setData( oCurrentFileInfo.filePath() );
		connect( pAction, SIGNAL( triggered() ), this, SLOT( OpenDAFFFileRecent() ) );

		// Skip 'Clear' action and horizontal devider (+2) and then shift/replace
		if( lActions.size() > i + 2 )
            ui->menuRecent->removeAction( lActions[ int( i + 2 ) ] );

		ui->menuRecent->addAction( pAction );
	}
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
		ui->DAFFStatusBar->showMessage( "Closing current DAFF file " );
		emit SignalCloseDAFF();
		m_pDAFFReader->closeFile();
	}

	ui->DAFFStatusBar->showMessage( "Opening DAFF file " + sPath );

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
		emit SignalReadDAFF( m_pDAFFReader );
		emit SignalContentLoaded( m_pDAFFReader->getContent() );

		QStringList vsRecentFiles = m_qSettings.value( "RecentFiles" ).toStringList();
		QStringList vsNewRecentFiles;
		vsNewRecentFiles.push_back( oPassedFile.filePath() );
		for( int i = 0; i < std::min( 9-1, vsRecentFiles.size() ); i++ )
			if( vsRecentFiles[ i ] != oPassedFile.filePath() )
				vsNewRecentFiles.push_back( vsRecentFiles[ i ] );
		m_qSettings.setValue( "RecentFiles", vsNewRecentFiles );

		UpdateRecentFilesActions();
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
    emit SignalCloseDAFF();
	m_pDAFFReader->closeFile();
}

void QDAFFViewerWindow::on_actionDownload_triggered()
{
    QUrl urlDownloadWebsite( "http://sourceforge.net/projects/opendaff/files/Content" );
    QDesktopServices::openUrl( urlDownloadWebsite );
}

void QDAFFViewerWindow::OpenDAFFFileRecent()
{
	QAction* pSendingAction = qobject_cast< QAction * >( sender() );
	if( pSendingAction )
		OpenDAFFFile( pSendingAction->data().toString() );
}

void QDAFFViewerWindow::IncreaseFrequencyIndex()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	switch( m_pDAFFReader->getContentType() )
	{
	case DAFF_MAGNITUDE_SPECTRUM:
	{
		DAFFContentMS* pC = dynamic_cast< DAFFContentMS* >( m_pDAFFReader->getContent() );
		const std::vector< float >& vFrequencies = pC->getFrequencies();
		if( m_iShowFrequencyIndex < int( vFrequencies.size() ) - 1 )
		{
			m_iShowFrequencyIndex++;
			emit SignalFrequencyIndexChanged( m_iShowFrequencyIndex );
		}
		break;
	}
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
	{
		DAFFContentMPS* pC = dynamic_cast< DAFFContentMPS* >( m_pDAFFReader->getContent() );
		const std::vector< float >& vFrequencies = pC->getFrequencies();
		if( m_iShowFrequencyIndex < int( vFrequencies.size() ) - 1 )
		{
			m_iShowFrequencyIndex++;
			emit SignalFrequencyIndexChanged( m_iShowFrequencyIndex );
		}
		break;
	}
	case DAFF_DFT_SPECTRUM:
	{
		DAFFContentDFT* pC = dynamic_cast< DAFFContentDFT* >( m_pDAFFReader->getContent() );
		if( m_iShowFrequencyIndex < pC->getNumDFTCoeffs() - 1 )
		{
			m_iShowFrequencyIndex++;
			emit SignalFrequencyIndexChanged( m_iShowFrequencyIndex );
		}
		break;
	}
	case DAFF_PHASE_SPECTRUM:
	{
		DAFFContentPS* pC = dynamic_cast< DAFFContentPS* >( m_pDAFFReader->getContent() );
		const std::vector< float >& vFrequencies = pC->getFrequencies();
		if( m_iShowFrequencyIndex < int( vFrequencies.size() ) - 1 )
		{
			m_iShowFrequencyIndex++;
			emit SignalFrequencyIndexChanged( m_iShowFrequencyIndex );
		}
		break;
	}
	}
}

void QDAFFViewerWindow::DecreaseFrequencyIndex()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	if( m_iShowFrequencyIndex > 0 )
	{
		m_iShowFrequencyIndex--;
		emit SignalFrequencyIndexChanged( m_iShowFrequencyIndex );
	}
}

void QDAFFViewerWindow::ChangeFrequencyIndex( int iIndex )
{
	if( !m_pDAFFReader->isFileOpened() || iIndex == -1 )
		return;

	m_iShowFrequencyIndex = iIndex;
	emit SignalFrequencyIndexChanged( iIndex );
}

void QDAFFViewerWindow::IncreaseChannelIndex()
{
	if( m_pDAFFReader->isFileOpened() == false )
		return;

	int N = m_pDAFFReader->getProperties()->getNumberOfChannels();
	if( m_iShowChannel < N - 1 )
	{
		m_iShowChannel++;
        emit SignalChannelIndexChanged( m_iShowChannel );
	}
}

void QDAFFViewerWindow::DecreaseChannelIndex()
{
	if( m_pDAFFReader->isFileOpened() == false )
		return;

	if( m_iShowChannel > 0 )
	{
		m_iShowChannel--;
        emit SignalChannelIndexChanged( m_iShowChannel );
	}
}

void QDAFFViewerWindow::ChangeChannelIndex( int iChannelIndex )
{
	if( m_pDAFFReader->isFileOpened() == false )
		return;

	int N = m_pDAFFReader->getProperties()->getNumberOfChannels();
	if( iChannelIndex >= 0 && iChannelIndex < N )
	{
		m_iShowChannel = iChannelIndex;
		emit SignalChannelIndexChanged( m_iShowChannel );
	}
}

void QDAFFViewerWindow::SetDAFFObjectView()
{
	m_iShowDAFFView = DAFF_OBJECT_VIEW;
}

void QDAFFViewerWindow::SetDAFFDataView()
{
	m_iShowDAFFView = DAFF_DATA_VIEW;
}

void QDAFFViewerWindow::LoadContent( const DAFFContent* pContent )
{
	// Prepare UI objects to fit to this DAFF file props

	// Channels
	int N = pContent->getProperties()->getNumberOfChannels();
	ui->spinBox_ChannelIndex->setMaximum( N - 1 );
	ui->spinBox_ChannelIndex->setValue( 0 );

	// Alpha
	ui->doubleSpinBox_Alpha->setMinimum( pContent->getProperties()->getAlphaStart() );
	ui->doubleSpinBox_Alpha->setMaximum( pContent->getProperties()->getAlphaEnd() );
	ui->doubleSpinBox_Alpha->setSingleStep( pContent->getProperties()->getAlphaResolution() );

	// Beta
	ui->doubleSpinBox_Beta->setMinimum( pContent->getProperties()->getBetaStart() );
	ui->doubleSpinBox_Beta->setMaximum( pContent->getProperties()->getBetaEnd() );
	ui->doubleSpinBox_Beta->setSingleStep( pContent->getProperties()->getBetaResolution() );
		
	switch( pContent->getProperties()->getContentType() )
	{
	case DAFF_MAGNITUDE_SPECTRUM:
	{
		// Frequency
		ui->groupBox_Frequency->setTitle( "Frequency" );
		ui->label_frequency->setText( "Frequency" );
		const DAFFContentMS* pC = dynamic_cast< const DAFFContentMS* >( pContent );
		const std::vector< float >& vfFrequencies = pC->getFrequencies();
		ui->spinBox_FrequencyIndex->setMaximum( int( vfFrequencies.size() ) - 1 );
		ui->comboBox_FrequencySelector->clear();
		for( size_t i = 0; i < vfFrequencies.size(); i++ )
		{
			ui->comboBox_FrequencySelector->addItem( QString::number( vfFrequencies[ i ] ) + " Hz" );
			if( std::abs( vfFrequencies[ i ] - 1000.0f ) < 10.0f )
				ui->comboBox_FrequencySelector->setCurrentIndex( int( i ) );
		}
		break;
	}
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
	{
		ui->groupBox_Frequency->setTitle( "Frequency" );
		ui->label_frequency->setText( "Frequency" );
		const DAFFContentMPS* pC = dynamic_cast< const DAFFContentMPS* >( m_pDAFFReader->getContent() );
		const std::vector< float >& vfFrequencies = pC->getFrequencies();
		ui->spinBox_FrequencyIndex->setMaximum( int( vfFrequencies.size() ) - 1 );
		ui->comboBox_FrequencySelector->clear();
		for( size_t i = 0; i < vfFrequencies.size(); i++ )
		{
			ui->comboBox_FrequencySelector->addItem( QString::number( vfFrequencies[ i ] ) + " Hz" );
			if( std::abs( vfFrequencies[ i ] - 1000.0f ) < 10.0f )
				ui->comboBox_FrequencySelector->setCurrentIndex( int( i ) );
		}
		break;
	}
	case DAFF_DFT_SPECTRUM:
	{
		ui->groupBox_Frequency->setTitle( "Frequency" );
		ui->label_frequency->setText( "Frequency bin" );
		const DAFFContentDFT* pC = dynamic_cast< const DAFFContentDFT* >( m_pDAFFReader->getContent() );
		ui->spinBox_FrequencyIndex->setMaximum( pC->getNumDFTCoeffs() - 1 );
		ui->comboBox_FrequencySelector->clear();
		double dFreqBandwidth = pC->getFrequencyBandwidth();
		for( size_t i = 0; i < pC->getNumDFTCoeffs(); i++ )
		{
			double dFreq = dFreqBandwidth * ( 1 + i );
			ui->comboBox_FrequencySelector->addItem( QString::number( dFreq ) + " Hz" );
			if( std::abs( dFreq - 1000.0f ) < 1.0f )
				ui->comboBox_FrequencySelector->setCurrentIndex( int( i ) );
		}
		break;
	}
	case DAFF_PHASE_SPECTRUM:
	{
		ui->groupBox_Frequency->setTitle( "Frequency" );
		ui->label_frequency->setText( "Frequency" );
		const DAFFContentPS* pC = dynamic_cast< const DAFFContentPS* >( m_pDAFFReader->getContent() );
		const std::vector< float >& vfFrequencies = pC->getFrequencies();
		ui->spinBox_FrequencyIndex->setMaximum( int( vfFrequencies.size() ) - 1 );
		ui->comboBox_FrequencySelector->clear();
		for( size_t i = 0; i < vfFrequencies.size(); i++ )
		{
			ui->comboBox_FrequencySelector->addItem( QString::number( vfFrequencies[ i ] ) + " Hz" );
			if( std::abs( vfFrequencies[ i ] - 1000.0f ) < 10.0f )
				ui->comboBox_FrequencySelector->setCurrentIndex( int( i ) );
		}
		break;
	}
	case DAFF_IMPULSE_RESPONSE:
	{
        ui->groupBox_Frequency->setTitle( "Samples" );
        ui->label_frequency->setText( "Sample pos" );
		const DAFFContentIR* pC = dynamic_cast< const DAFFContentIR* >( m_pDAFFReader->getContent() );
		ui->comboBox_FrequencySelector->clear();
		for( int i = 0; i < pC->getFilterLength(); i++ )
		{
			assert( pC->getSamplerate() > 0 );
			ui->comboBox_FrequencySelector->addItem( QString::number( i / pC->getSamplerate() * 1000.0f * 1000.0f ) + " us" );
		}
		ui->comboBox_FrequencySelector->setCurrentIndex( 0 );
	}
	}

	// Initially set front direction
	ChangePhiAndTheta( 0.0f, 0.0f );
}

void QDAFFViewerWindow::ChangeRecordIndex( int iRecordIndex )
{
	if( m_iShowRecordIndex == iRecordIndex )
		return;

	m_iShowRecordIndex = iRecordIndex;

	if( m_pDAFFReader->isFileOpened() )
	{
		// Record index change not invoked by alpha/beta (data view), so overwrite and emit signal
		float fAlphaDeg, fBetaDeg;
		m_pDAFFReader->getContent()->getRecordCoords( m_iShowRecordIndex, DAFF_DATA_VIEW, fAlphaDeg, fBetaDeg );
		ChangeAlphaAndBeta( fAlphaDeg, fBetaDeg );
		
		// If record index change not invoked by phi/theta (object view) then emit signal
		int iRecordIndexCurrentObjectView;
		m_pDAFFReader->getContent()->getNearestNeighbour( DAFF_OBJECT_VIEW, m_dShowPhiDeg, m_dShowThetaDeg, iRecordIndexCurrentObjectView );
		if( iRecordIndexCurrentObjectView != m_iShowRecordIndex )
		{
			float fPhiDeg, fThetaDeg;
			m_pDAFFReader->getContent()->getRecordCoords( m_iShowRecordIndex, DAFF_OBJECT_VIEW, fPhiDeg, fThetaDeg );
			ChangePhiAndTheta( fPhiDeg, fThetaDeg );
		}
	}

	emit SignalRecordIndexChanged( m_iShowRecordIndex );
}

void QDAFFViewerWindow::SetPhiAndThetaOutOfBoundsIndicator( bool bOutOfBounds )
{
	QString sToolTip, sStyleSheet;

	if( bOutOfBounds )
	{
		sToolTip = "Requested direction is out of bounds (not covered by a record data set)";
		sStyleSheet = "QDoubleSpinBox { background-color: rgb(255, 213, 213); }";
	}
	else
	{
		sToolTip = "Requested direction is within bounds (covered by a record data set)";
		sStyleSheet = "QDoubleSpinBox { background-color: rgb(213, 255, 213); }";
	}

	ui->doubleSpinBox_Phi->setToolTip( sToolTip );
	ui->doubleSpinBox_Theta->setToolTip( sToolTip );
	ui->doubleSpinBox_Phi->setStyleSheet( sStyleSheet );
	ui->doubleSpinBox_Theta->setStyleSheet( sStyleSheet );
}

void QDAFFViewerWindow::IncreaseAlpha()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	assert( m_dShowAlphaDeg >= 0.0f && m_dShowAlphaDeg < 360.0f );
	m_dShowAlphaDeg += fmodf( m_pDAFFReader->getProperties()->getAlphaResolution(), 360.0f );
	ChangeAlpha( m_dShowAlphaDeg );
}

void QDAFFViewerWindow::DecreaseAlpha()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	assert( m_dShowAlphaDeg >= 0.0f && m_dShowAlphaDeg < 360.0f );
	m_dShowAlphaDeg -= m_pDAFFReader->getProperties()->getAlphaResolution();
	if( m_dShowAlphaDeg < 0 )
		m_dShowAlphaDeg += 180.0f;
	ChangeAlpha( m_dShowAlphaDeg );
}

void QDAFFViewerWindow::ChangeAlpha( double dAlphaDeg )
{
	m_dShowAlphaDeg = fmodf( dAlphaDeg, 360.0f );

	if( m_pDAFFReader->isFileOpened() )
	{
		int iRecordIndex;
		m_pDAFFReader->getContent()->getNearestNeighbour( DAFF_DATA_VIEW, m_dShowAlphaDeg, m_dShowBetaDeg, iRecordIndex );
		ChangeRecordIndex( iRecordIndex );
	}

	emit SignalAlphaChanged( m_dShowAlphaDeg );
}

void QDAFFViewerWindow::IncreaseBeta()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	assert( m_dShowBetaDeg >= 0.0f && m_dShowBetaDeg <= 180.0f );
	m_dShowBetaDeg += fmodf( m_pDAFFReader->getProperties()->getBetaResolution(), 180.0f );
	ChangeBeta( m_dShowBetaDeg );
}

void QDAFFViewerWindow::DecreaseBeta()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	assert( m_dShowBetaDeg >= 0.0f && m_dShowBetaDeg <= 180.0f );
	m_dShowBetaDeg -= m_pDAFFReader->getProperties()->getAlphaResolution();
	if( m_dShowBetaDeg < 0 )
		m_dShowBetaDeg += 180.0f;
	ChangeBeta( m_dShowBetaDeg );
}

void QDAFFViewerWindow::ChangeBeta( double dBetaDeg )
{
	m_dShowBetaDeg = fmodf( dBetaDeg, 180.0f );

	if( m_pDAFFReader->isFileOpened() )
	{
		int iRecordIndex;
		m_pDAFFReader->getContent()->getNearestNeighbour( DAFF_DATA_VIEW, m_dShowAlphaDeg, m_dShowBetaDeg, iRecordIndex );
		ChangeRecordIndex( iRecordIndex );
	}

	emit SignalBetaChanged( m_dShowBetaDeg );
}

void QDAFFViewerWindow::ChangeAlphaAndBeta( double dAlphaDeg, double dBetaDeg )
{
	m_dShowAlphaDeg = fmodf( dAlphaDeg, 360.0f );
	m_dShowBetaDeg = fmodf( dBetaDeg, 180.0f );

	if( m_pDAFFReader->isFileOpened() )
	{
		int iRecordIndex;
		m_pDAFFReader->getContent()->getNearestNeighbour( DAFF_DATA_VIEW, m_dShowAlphaDeg, m_dShowBetaDeg, iRecordIndex );
		ChangeRecordIndex( iRecordIndex );
	}

	emit SignalAlphaChanged( m_dShowAlphaDeg );
	emit SignalBetaChanged( m_dShowBetaDeg );
	emit SignalDirectionChanged(m_dShowPhiDeg, m_dShowThetaDeg);
}

void QDAFFViewerWindow::ChangePhiAndTheta( double dPhiDeg, double dThetaDeg )
{
	m_dShowPhiDeg = dPhiDeg;
	m_dShowThetaDeg = dThetaDeg;

	if( m_pDAFFReader->isFileOpened() )
	{
		int iRecordIndex;
		bool bOutOfBounds;
		m_pDAFFReader->getContent()->getNearestNeighbour( DAFF_OBJECT_VIEW, m_dShowPhiDeg, m_dShowThetaDeg, iRecordIndex, bOutOfBounds );
		ChangeRecordIndex( iRecordIndex );
		
		emit SignalPhiAndThetaOutOfBounds( bOutOfBounds );
	}

	emit SignalPhiChanged( m_dShowPhiDeg );
	emit SignalThetaChanged( m_dShowThetaDeg );
}

void QDAFFViewerWindow::on_actionIncrease_Frequency_triggered()
{
    IncreaseFrequencyIndex();
}

void QDAFFViewerWindow::on_actionDecrease_Frequency_triggered()
{
    DecreaseFrequencyIndex();
}

void QDAFFViewerWindow::on_actionIncrease_channel_triggered()
{
    IncreaseChannelIndex();
}

void QDAFFViewerWindow::on_actionDecrease_channel_triggered()
{
    DecreaseChannelIndex();
}

void QDAFFViewerWindow::on_actionIncrease_alpha_triggered()
{
    IncreaseAlpha();
}

void QDAFFViewerWindow::on_actionDecrease_alpha_triggered()
{
    DecreaseAlpha();
}

void QDAFFViewerWindow::on_actionIncrease_beta_triggered()
{
    IncreaseBeta();
}

void QDAFFViewerWindow::on_actionDecrease_beta_triggered()
{
    DecreaseBeta();
}

void QDAFFViewerWindow::on_actionIncrease_phi_triggered()
{
    IncreasePhi();
}

void QDAFFViewerWindow::on_actionDecrease_phi_triggered()
{
    DecreasePhi();
}

void QDAFFViewerWindow::on_actionIncrease_theta_triggered()
{
    IncreaseTheta();
}

void QDAFFViewerWindow::on_actionDecrease_theta_triggered()
{
    DecreaseTheta();
}

void QDAFFViewerWindow::IncreasePhi()
{
	ChangePhi( m_dShowPhiDeg + m_dPhiThetaIncrementDeg );
}

void QDAFFViewerWindow::DecreasePhi()
{
	ChangePhi( m_dShowPhiDeg - m_dPhiThetaIncrementDeg );
}

void QDAFFViewerWindow::ChangePhi( double dPhiDeg )
{
	m_dShowPhiDeg = dPhiDeg;
	emit SignalPhiChanged( dPhiDeg );

	if( m_pDAFFReader->isFileOpened() )
	{
		int iRecordIndex;
		bool bOutOfBounds;
		m_pDAFFReader->getContent()->getNearestNeighbour( DAFF_OBJECT_VIEW, m_dShowPhiDeg, m_dShowThetaDeg, iRecordIndex, bOutOfBounds );
		ChangeRecordIndex( iRecordIndex );

		emit SignalPhiAndThetaOutOfBounds( bOutOfBounds );
	}
}

void QDAFFViewerWindow::IncreaseTheta()
{
	ChangeTheta( m_dShowThetaDeg + m_dPhiThetaIncrementDeg );
}

void QDAFFViewerWindow::DecreaseTheta()
{
	ChangeTheta( m_dShowThetaDeg - m_dPhiThetaIncrementDeg );
}

void QDAFFViewerWindow::ChangeTheta( double dThetaDeg )
{
	m_dShowThetaDeg = dThetaDeg;

	if( m_pDAFFReader->isFileOpened() )
	{
		int iRecordIndex;
		bool bOutOfBounds;
		m_pDAFFReader->getContent()->getNearestNeighbour( DAFF_OBJECT_VIEW, m_dShowPhiDeg, m_dShowThetaDeg, iRecordIndex, bOutOfBounds );
		ChangeRecordIndex( iRecordIndex );

		emit SignalPhiAndThetaOutOfBounds( bOutOfBounds );
	}

	emit SignalThetaChanged( m_dShowThetaDeg );
}

void QDAFFViewerWindow::on_actionRecent_Clear_triggered()
{
    m_qSettings.setValue( "RecentFiles", QVariant() );
    UpdateRecentFilesActions();
}

void QDAFFViewerWindow::on_actionExport2DPlot_triggered()
{
	if( m_pDAFFReader->isFileOpened() == false )
	{
		QErrorMessage qe;
		QString sError = "Please load a DAFF file before trying to export plots";
		qe.showMessage( sError );
		ui->DAFFStatusBar->showMessage( sError );
		qe.exec();
		return;
	}

	QFileInfo oFile( m_pDAFFReader->getFilename().c_str() );
	QDAFFDialogExport2DPlot dialog( this, oFile.dir(), oFile.baseName() );

	if( dialog.exec() )
	{
		QString sExportFilePath = dialog.GetExportPath();
		bool bIncludeAllChannels = dialog.ExportAllChannels();
		bool bShowDataPointDots = dialog.ShowDataPointDots();
		float fScalingFactor = dialog.GetScalingFactor();
		if( dialog.GetExportType() == QDAFFDialogExport2DPlot::EXPORT_TYPE_PNG )
		{
			ui->graphicsView_2DDAFFPlot->ExportImagePNG( sExportFilePath, fScalingFactor, bIncludeAllChannels, bShowDataPointDots );
			ui->DAFFStatusBar->showMessage( "Exported plot to " + sExportFilePath );
		}
		else if( dialog.GetExportType() == QDAFFDialogExport2DPlot::EXPORT_TYPE_SVG )
		{
			ui->graphicsView_2DDAFFPlot->ExportImageSVG( sExportFilePath, fScalingFactor, bIncludeAllChannels, bShowDataPointDots );
			ui->DAFFStatusBar->showMessage( "Exported plot to " + sExportFilePath );
		}
		else
		{
			QErrorMessage qe;
			QString sError = "Unkown export type, supported types are: svg, png";
			qe.showMessage( sError );
			ui->DAFFStatusBar->showMessage( sError );
			qe.exec();
			return;
		}
	}
	else
	{
		ui->DAFFStatusBar->showMessage( "Export aborted." );
	}
}

void QDAFFViewerWindow::on_actionExport3DPlot_triggered()
{
	if( m_pDAFFReader->isFileOpened() == false )
	{
		QErrorMessage qe;
		QString sError = "Please load a DAFF file before trying to export plots";
		qe.showMessage( sError );
		ui->DAFFStatusBar->showMessage( sError );
		qe.exec();
		return;
	}

	QFileInfo oFile( m_pDAFFReader->getFilename().c_str() );
	QDAFFDialogExport3DPlotImage dialog( this, oFile.dir(), oFile.baseName() );

	if( dialog.exec() )
	{
		QString sExportFilePath = dialog.GetExportPath();		
		ui->DAFF3DPlot_VTKWidget->ExportScreenshotPNG( sExportFilePath, dialog.GetWidth(), dialog.GetHeight() );
		ui->DAFFStatusBar->showMessage( "Exported plot to " + sExportFilePath );
	}
	else
	{
		ui->DAFFStatusBar->showMessage( "Export aborted." );
	}
}

void QDAFFViewerWindow::on_actionExport3DPlotImageSeries_triggered()
{
	if( m_pDAFFReader->isFileOpened() == false )
	{
		QErrorMessage qe;
		QString sError = "Please load a DAFF file before trying to export plots";
		qe.showMessage( sError );
		ui->DAFFStatusBar->showMessage( sError );
		qe.exec();
		return;
	}

	QFileInfo oFile( m_pDAFFReader->getFilename().c_str() );
	QDAFFDialogExport3DPlotImageSeries dialog( this, oFile.dir(), oFile.baseName() );

	dialog.SetAnimationChannelIndices( 0, m_pDAFFReader->getProperties()->getNumberOfChannels() - 1 );

	switch( m_pDAFFReader->getContentType() )
	{
	case DAFF_DFT_SPECTRUM:
	{
		const DAFFContentDFT* pC = dynamic_cast< const DAFFContentDFT* >( m_pDAFFReader->getContent() );
		dialog.SetAnimationFrequencyIndices( 0, pC->getNumDFTCoeffs() - 1 );
		break;
	}
	case DAFF_MAGNITUDE_SPECTRUM:
	{
		const DAFFContentMS* pC = dynamic_cast< const DAFFContentMS* >( m_pDAFFReader->getContent() );
		dialog.SetAnimationFrequencyIndices( 0, pC->getNumFrequencies() - 1 );
		break;
	}
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
	{
		const DAFFContentMPS* pC = dynamic_cast< const DAFFContentMPS* >( m_pDAFFReader->getContent() );
		dialog.SetAnimationFrequencyIndices( 0, pC->getNumFrequencies() - 1 );
		break;
	}
	case DAFF_PHASE_SPECTRUM:
	{
		const DAFFContentPS* pC = dynamic_cast< const DAFFContentPS* >( m_pDAFFReader->getContent() );
		dialog.SetAnimationFrequencyIndices( 0, pC->getNumFrequencies() - 1 );
		break;
	}
	default:
	{
		dialog.SetAnimationFrequencyIndices( 0, 0 );
	}
	}

	double dBetaStart = m_pDAFFReader->getProperties()->getBetaStart();
	double dBetaEnd = m_pDAFFReader->getProperties()->getBetaEnd();
	int iR;
	m_pDAFFReader->getContent()->getNearestNeighbour( DAFF_DATA_VIEW, 0.0f, dBetaStart, iR );
	float fEleStart, fEleEnd, fTemp;
	m_pDAFFReader->getContent()->getRecordCoords( iR, DAFF_OBJECT_VIEW, fTemp, fEleStart );
	m_pDAFFReader->getContent()->getNearestNeighbour( DAFF_DATA_VIEW, 0.0f, dBetaEnd, iR );
	m_pDAFFReader->getContent()->getRecordCoords( iR, DAFF_OBJECT_VIEW, fTemp, fEleEnd );
	dialog.SetAnimationElevationRange( double( fEleStart ), double( fEleEnd ) );

	if( dialog.exec() )
	{
		QString sExportDirectory = dialog.GetExportPath();
		QString sFileBaseName = dialog.GetFileBaseName();

		if( sFileBaseName.isEmpty() )
			sFileBaseName = oFile.baseName();

		QDAFFVTKWidget::CAnimation oAnimation;

		oAnimation.iNumFrames = dialog.GetNumFrames();
		oAnimation.iWidth = dialog.GetWidth();
		oAnimation.iHeight = dialog.GetHeight();

		oAnimation.bYaw = dialog.GetAnimationYaw( oAnimation.dYawStart, oAnimation.dYawEnd );
		oAnimation.bPitch = dialog.GetAnimationPitch( oAnimation.dPitchStart, oAnimation.dPitchEnd );

		oAnimation.bChannels = dialog.GetAnimationChannelIndices( oAnimation.iChannelIdxStart, oAnimation.iChannelIdxEnd );
		oAnimation.bFrequencies = dialog.GetAnimationFrequencyIndices( oAnimation.iFreqIdxStart, oAnimation.iFreqIdxEnd );
		oAnimation.bElevation = dialog.GetAnimationElevation( oAnimation.dEleStart, oAnimation.dEleEnd );
		
		ui->DAFFStatusBar->showMessage( "Starting screenshot series, this may take a while." );
		ui->DAFF3DPlot_VTKWidget->ExportScrenshotSeriesPNG( sExportDirectory, sFileBaseName, oAnimation );
		ui->DAFFStatusBar->showMessage( "Exported screenshot series to folder " + oFile.absolutePath() );
	}
	else
	{
		ui->DAFFStatusBar->showMessage( "Export aborted." );
	}
}

void QDAFFViewerWindow::on_action3DSphericalShowArrows_triggered( bool bChecked )
{
	ui->DAFF3DPlot_VTKWidget->SetArrowsVisible( bChecked );
}

void QDAFFViewerWindow::on_action3DSphericalShowPoles_triggered( bool bChecked )
{
	ui->DAFF3DPlot_VTKWidget->SetPolesVisible( bChecked );
}

void QDAFFViewerWindow::on_action3DSphericalShowGrid_triggered( bool bChecked )
{
	ui->DAFF3DPlot_VTKWidget->SetGridVisible( bChecked );
}

void QDAFFViewerWindow::on_action3DSphericalShowMeridians_triggered( bool bChecked )
{
	ui->DAFF3DPlot_VTKWidget->SetMeridiansVisible( bChecked );
}

void QDAFFViewerWindow::on_action3DSphericalShowEquator_triggered( bool bChecked )
{
	ui->DAFF3DPlot_VTKWidget->SetEquatorVisible( bChecked );
}

void QDAFFViewerWindow::on_action2DShowAllChannels_triggered( bool bChecked )
{
    ui->graphicsView_2DDAFFPlot->SetAllChannelsVisible( bChecked );
}

void QDAFFViewerWindow::on_action3DShowDirectionIndicator_triggered( bool bChecked )
{
    ui->DAFF3DPlot_VTKWidget->SetDirectionIndicatorVisible( bChecked );
}

void QDAFFViewerWindow::on_action3DSphericalUsePhaseColorMap_triggered( bool bChecked )
{
    ui->DAFF3DPlot_VTKWidget->SetPhaseColorMap( bChecked );
}

void QDAFFViewerWindow::on_action3DUseLogScale_triggered( bool bChecked )
{
	ui->DAFF3DPlot_VTKWidget->SetLogScale( bChecked );
}

void QDAFFViewerWindow::on_action3DSphericalNormalize_triggered(bool bChecked)
{
	ui->DAFF3DPlot_VTKWidget->SetNormalize(bChecked);
}

void QDAFFViewerWindow::on_action3DSphericalNormalizeFrequenciesIndividually_triggered(bool bChecked)
{
	ui->DAFF3DPlot_VTKWidget->SetNormalizeFrequenciesIndividually(bChecked);
}

void QDAFFViewerWindow::on_action3DCarpetShowWarp_triggered(bool bChecked)
{
	ui->DAFF3DPlot_VTKWidget->SetCarpetPlotWarp(bChecked);
}
