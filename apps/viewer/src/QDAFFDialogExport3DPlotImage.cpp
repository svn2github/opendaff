#include "QDAFFDialogExport3DPlotImage.h"
#include "ui_QDAFFDialogExport3DPlotImage.h"

#include <QFileDialog>

QDAFFDialogExport3DPlotImage::QDAFFDialogExport3DPlotImage(QWidget *parent, const QDir& oBasePath, const QString& sFileBaseName )
    : QDialog(parent),
    ui(new Ui::QDAFFDialogExport3DPlotImage)
  , m_oBasePath( oBasePath )
  , m_sFileBaseName( sFileBaseName )
  , m_bExport( false )
{
    ui->setupUi(this);

	QStringList vsSupportedFileTypes;
	vsSupportedFileTypes <<"png";

	int iWidth = m_qSettings.value( "Export3DPlotImage/Width", 1920 ).toInt();
	int iHeight = m_qSettings.value( "Export3DPlotImage/Height", 1080 ).toInt();

	ui->spinBox_Width->setValue( iWidth );
	ui->spinBox_Height->setValue( iHeight );

    if( m_oBasePath.exists() )
    {
		if( !m_oBasePath.isAbsolute() )
			m_oBasePath.makeAbsolute();

		QString sSuffix = "png";

		QFile oFile;
		oFile.setFileName( m_oBasePath.absolutePath() + QDir::separator() + m_sFileBaseName + "." + sSuffix );

        ui->lineEdit_ExportPath->setText( oFile.fileName() );
    }
}

QDAFFDialogExport3DPlotImage::~QDAFFDialogExport3DPlotImage()
{
	m_qSettings.setValue( "Export3DPlotImage/Width", ui->spinBox_Width->value() );
	m_qSettings.setValue( "Export3DPlotImage/Height", ui->spinBox_Height->value() );
    delete ui;
}

QString QDAFFDialogExport3DPlotImage::GetExportPath() const
{
	QFileInfo oFilePath( ui->lineEdit_ExportPath->text() );

	if( m_bExport )
		return oFilePath.absoluteFilePath();
	else
		return "";
}

int QDAFFDialogExport3DPlotImage::GetWidth() const
{
	return ui->spinBox_Width->value();
}

int QDAFFDialogExport3DPlotImage::GetHeight() const
{
	return ui->spinBox_Height->value();
}

void QDAFFDialogExport3DPlotImage::on_pushButton_Export_clicked()
{
	m_bExport = true;
	accept();
	close();
}

void QDAFFDialogExport3DPlotImage::on_pushButton_Browse_clicked()
{
	QFileDialog fd;
	fd.setNameFilter( "PNG image files (*.png)" );
	fd.setViewMode( QFileDialog::Detail );

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
