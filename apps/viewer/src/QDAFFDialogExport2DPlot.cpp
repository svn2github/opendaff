#include "QDAFFDialogExport2DPlot.h"
#include "ui_QDAFFDialogExport2DPlot.h"

#include <QFileDialog>

QDAFFDialogExport2DPlot::QDAFFDialogExport2DPlot(QWidget *parent, const QDir& oBasePath, const QString& sFileBaseName )
    : QDialog(parent),
    ui(new Ui::QDAFFDialogExport2DPlot)
  , m_oBasePath( oBasePath )
  , m_sFileBaseName( sFileBaseName )
  , m_bExport( false )
{
    ui->setupUi(this);

	QStringList vsSupportedFileTypes = { "png", "svg", "jpg" };
	int iExportType = m_qSettings.value( "Export2DPlot/FileType", EXPORT_TYPE_SVG ).toInt();

	Qt::CheckState iState = Qt::CheckState( m_qSettings.value( "Export2DPlot/IncludeAllChannels", Qt::Checked ).toInt() );
	ui->checkBox_IncludeAllChannels->setCheckState( iState );

	iState = Qt::CheckState( m_qSettings.value( "Export2DPlot/ShowDataPointDots", Qt::Checked ).toInt() );
	ui->checkBox_ShowDataPointDots->setCheckState( iState );

	float fScalingFactor = m_qSettings.value( "Export2DPlot/ScalingFactor", 1.0f ).toFloat();
	ui->doubleSpinBox_ScalingFactor->setValue( fScalingFactor );

    if( m_oBasePath.exists() )
    {
		if( !m_oBasePath.isAbsolute() )
			m_oBasePath.makeAbsolute();

		QString sSuffix = "svg";
		if( iExportType == EXPORT_TYPE_PNG )
			sSuffix = "png";

		QFile oFile;
		oFile.setFileName( m_oBasePath.absolutePath() + QDir::separator() + m_sFileBaseName + "." + sSuffix );

        ui->lineEdit_ExportPath->setText( oFile.fileName() );
    }
}

QDAFFDialogExport2DPlot::~QDAFFDialogExport2DPlot()
{
	m_qSettings.setValue( "Export2DPlot/IncludeAllChannels", ui->checkBox_IncludeAllChannels->checkState() );
	m_qSettings.setValue( "Export2DPlot/ShowDataPointDots", ui->checkBox_ShowDataPointDots->checkState() );
	m_qSettings.setValue( "Export2DPlot/ScalingFactor", ui->doubleSpinBox_ScalingFactor->value() );
	m_qSettings.setValue( "Export2DPlot/FileType", GetExportType() );
    delete ui;
}

QString QDAFFDialogExport2DPlot::GetExportPath() const
{
	QFileInfo oFilePath( ui->lineEdit_ExportPath->text() );

	if( m_bExport )
		return oFilePath.absoluteFilePath();
	else
		return "";
}

bool QDAFFDialogExport2DPlot::ExportAllChannels() const
{
	Qt::CheckState iState = ui->checkBox_IncludeAllChannels->checkState();
	if( iState == Qt::Unchecked )
		return false;
	else
		return true;
}

bool QDAFFDialogExport2DPlot::ShowDataPointDots() const
{
	Qt::CheckState iState = ui->checkBox_ShowDataPointDots->checkState();
	if( iState == Qt::Unchecked )
		return false;
	else
		return true;
}

float QDAFFDialogExport2DPlot::GetScalingFactor() const
{
	return ui->doubleSpinBox_ScalingFactor->value();
}

int QDAFFDialogExport2DPlot::GetExportType() const
{
	QFileInfo oFilePath( ui->lineEdit_ExportPath->text() );

	if( oFilePath.suffix() == "png" )
		return EXPORT_TYPE_PNG;
	else if( oFilePath.suffix() == "svg" )
		return EXPORT_TYPE_SVG;
	else
		return EXPORT_UNKOWN_TYPE;
}

void QDAFFDialogExport2DPlot::on_pushButton_Export_clicked()
{
	m_bExport = true;
	accept();
	close();
}

void QDAFFDialogExport2DPlot::on_pushButton_Browse_clicked()
{

	QFileDialog fd;
	fd.setNameFilter( "Image files (*.png, *.svg)" );
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
