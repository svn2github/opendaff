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
	, m_qSettings( "ITA", "DAFFViewer" )
	, m_iShowChannel( 0 )
	, m_iShowFrequencyIndex( 0 )
	, m_fShowTimeSample( 0 )
	, m_fShowAlphaDeg( 0 )
	, m_fShowBetaDeg( 90 )
	, m_fShowPhiDeg( 0 )
	, m_fShowTheta( 0 )
{
	ui->setupUi( this );
    showMaximized(); // does not work with QGraphicsView

	connect( this, SIGNAL( ReadDAFF( const DAFFReader* ) ), ui->groupBox_Reader, SLOT( on_readDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( CloseDAFF() ), ui->groupBox_Reader, SLOT( on_closeDAFF() ) );

	connect( this, SIGNAL( ReadDAFF( const DAFFReader* ) ), ui->DAFF3DPlot_VTKWidget, SLOT( on_readDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( CloseDAFF() ), ui->DAFF3DPlot_VTKWidget, SLOT( on_closeDAFF() ) );
	connect( this, SIGNAL( SignalFrequencyIndexChanged( int ) ), ui->DAFF3DPlot_VTKWidget, SLOT( on_changeFrequencyIndex( int ) ) );

	connect( this, SIGNAL( ReadDAFF( const DAFFReader* ) ), ui->tableView_Metadata, SLOT( on_readDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( CloseDAFF() ), ui->tableView_Metadata, SLOT( on_closeDAFF() ) );

	connect( this, SIGNAL( ReadDAFF( const DAFFReader* ) ), ui->tableView_Properties, SLOT( on_readDAFF( const DAFFReader* ) ) );
	connect( this, SIGNAL( CloseDAFF() ), ui->tableView_Properties, SLOT( on_closeDAFF() ) );

	connect( this, SIGNAL( SignalContentLoaded( const DAFFContent* ) ), this, SLOT( LoadContent( const DAFFContent* ) ) );

	// Frequency index conns
	connect( this, SIGNAL( SignalFrequencyIndexChanged( int ) ), ui->spinBox_FrequencyIndex, SLOT( setValue( int ) ) );
	connect( this, SIGNAL( SignalFrequencyIndexChanged( int ) ), ui->comboBox_FrequencySelector, SLOT( setCurrentIndex( int ) ) );
	connect( this, SIGNAL( SignalFrequencyIndexChanged( int ) ), ui->DAFF3DPlot_VTKWidget, SLOT( on_changeFrequencyIndex( int ) ) );
	connect( ui->spinBox_FrequencyIndex, SIGNAL( valueChanged( int ) ), this, SLOT( ChangeFrequencyIndex( int ) ) );
	connect( ui->comboBox_FrequencySelector, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ChangeFrequencyIndex( int ) ) );

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
		emit CloseDAFF();
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
		emit ReadDAFF( m_pDAFFReader );
		emit SignalContentLoaded( m_pDAFFReader->getContent() );
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
    emit CloseDAFF();
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

	assert( m_fShowBetaDeg >= 0.0f && m_fShowBetaDeg <= 180.0f );
	m_fShowBetaDeg += fmodf( m_pDAFFReader->getProperties()->getBetaResolution(), 180.0f );
}

void QDAFFViewerWindow::DecreaseBeta()
{
	if( !m_pDAFFReader->isFileOpened() )
		return;

	assert( m_fShowBetaDeg >= 0.0f && m_fShowBetaDeg <= 180.0f );
	m_fShowBetaDeg -= m_pDAFFReader->getProperties()->getAlphaResolution();
	if( m_fShowBetaDeg < 0 )
		m_fShowBetaDeg += 180.0f;
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
		emit ChangeChannelIndex( m_iShowChannel );
	}
}

void QDAFFViewerWindow::DecreaseChannelIndex()
{
	if( m_pDAFFReader->isFileOpened() == false )
		return;

	if( m_iShowChannel > 0 )
	{
		m_iShowChannel--;
		emit ChangeChannelIndex( m_iShowChannel );
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
		emit ChangeChannelIndex( m_iShowChannel );
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
		const DAFFContentMPS* pC = dynamic_cast< const DAFFContentMPS* >( m_pDAFFReader->getContent() );
		const std::vector< float >& vFrequencies = pC->getFrequencies();
		ui->spinBox_FrequencyIndex->setMaximum( int( vFrequencies.size() ) - 1 );
		break;
	}
	case DAFF_DFT_SPECTRUM:
	{
		ui->groupBox_Frequency->setTitle( "Frequency bin" );
		const DAFFContentDFT* pC = dynamic_cast< const DAFFContentDFT* >( m_pDAFFReader->getContent() );
		ui->spinBox_FrequencyIndex->setMaximum( pC->getNumDFTCoeffs() - 1 );
		break;
	}
	case DAFF_PHASE_SPECTRUM:
	{
		ui->groupBox_Frequency->setTitle( "Frequency" );
		const DAFFContentPS* pC = dynamic_cast< const DAFFContentPS* >( m_pDAFFReader->getContent() );
		const std::vector< float >& vFrequencies = pC->getFrequencies();
		ui->spinBox_FrequencyIndex->setMaximum( int( vFrequencies.size() ) - 1 );
		break;
	}
	case DAFF_IMPULSE_RESPONSE:
	{
		ui->groupBox_Frequency->setTitle( "Sample" );
	}
	}
}

void QDAFFViewerWindow::on_actionIncrease_Frequency_triggered()
{
    IncreaseFrequencyIndex();
}

void QDAFFViewerWindow::on_actionDecrease_Frequency_triggered()
{
    DecreaseFrequencyIndex();
}
