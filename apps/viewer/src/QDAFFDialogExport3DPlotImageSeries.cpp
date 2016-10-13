#include "QDAFFDialogExport3DPlotImageSeries.h"
#include "ui_QDAFFDialogExport3DPlotImageSeries.h"

#include <QFileDialog>

QDAFFDialogExport3DPlotImageSeries::QDAFFDialogExport3DPlotImageSeries( QWidget *parent, const QDir& oBasePath, const QString& sFileBaseName )
	: QDialog( parent ),
	ui( new Ui::QDAFFDialogExport3DPlotImageSeries )
	, m_oBasePath( oBasePath )
	, m_sFileBaseName( sFileBaseName )
	, m_bExport( false )
{
	ui->setupUi( this );

	QStringList vsSupportedFileTypes;
	vsSupportedFileTypes << "png";

	int iWidth = m_qSettings.value( "Export3DPlotImage/Width", 1920 ).toInt();
	int iHeight = m_qSettings.value( "Export3DPlotImage/Height", 1080 ).toInt();

	ui->spinBox_Width->setValue( iWidth );
	ui->spinBox_Height->setValue( iHeight );
	
	ui->lineEdit_FileBaseName->setText( sFileBaseName );

	if( m_oBasePath.exists() )
	{
		if( !m_oBasePath.isAbsolute() )
			m_oBasePath.makeAbsolute();

		ui->lineEdit_ExportPath->setText( m_oBasePath.absolutePath() );
	}
}

QDAFFDialogExport3DPlotImageSeries::~QDAFFDialogExport3DPlotImageSeries()
{
	m_qSettings.setValue( "Export3DPlotImageSeries/Width", ui->spinBox_Width->value() );
	m_qSettings.setValue( "Export3DPlotImageSeries/Height", ui->spinBox_Height->value() );
    delete ui;
}

QString QDAFFDialogExport3DPlotImageSeries::GetExportPath() const
{
	QFileInfo oFilePath( ui->lineEdit_ExportPath->text() );

	if( m_bExport )
		return oFilePath.absoluteFilePath();
	else
		return "";
}

int QDAFFDialogExport3DPlotImageSeries::GetNumFrames() const
{
	return ui->spinBox->value();
}

int QDAFFDialogExport3DPlotImageSeries::GetHeight() const
{
	return ui->spinBox_Height->value();
}

int QDAFFDialogExport3DPlotImageSeries::GetWidth() const
{
	return ui->spinBox_Width->value();
}

QString QDAFFDialogExport3DPlotImageSeries::GetFileBaseName() const
{
	return ui->lineEdit_FileBaseName->text();
}

void QDAFFDialogExport3DPlotImageSeries::on_pushButton_Export_clicked()
{
	m_bExport = true;
	accept();
	close();
}

void QDAFFDialogExport3DPlotImageSeries::on_pushButton_Browse_clicked()
{
	QFileDialog fd;
	fd.setNameFilter( "Export folder" );
	fd.setFileMode( QFileDialog::Directory );
	fd.setOption( QFileDialog::ShowDirsOnly );

	if( m_oBasePath.exists() )
		fd.setDirectory( m_oBasePath );
	else
		fd.setDirectory( QDir( QApplication::applicationDirPath() ) );

	if( fd.exec() )
	{
		QStringList lFiles = fd.selectedFiles();
		if( lFiles.empty() == false )
			ui->lineEdit_ExportPath->setText( lFiles[ 0 ] );
	}
}
