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

	LoadSettings();
}

QDAFFDialogExport3DPlotImageSeries::~QDAFFDialogExport3DPlotImageSeries()
{
	StoreSettings();
	//delete ui;
}

void QDAFFDialogExport3DPlotImageSeries::StoreSettings() const
{
	QSettings qs;

	qs.setValue( "Export3DPlotImageSeries/WindowGeometry", saveGeometry() );

	qs.setValue( "Export3DPlotImageSeries/Width", ui->spinBox_Width->value() );
	qs.setValue( "Export3DPlotImageSeries/Height", ui->spinBox_Height->value() );
	qs.setValue( "Export3DPlotImageSeries/Frames", ui->spinBox_frames->value() );

	qs.setValue( "Export3DPlotImageSeries/AnimateYaw", ui->checkBox_yaw->isChecked() );
	qs.setValue( "Export3DPlotImageSeries/AnimatePitch", ui->checkBox_pitch->isChecked() );
	qs.setValue( "Export3DPlotImageSeries/AnimateFrequencies", ui->checkBox_frequencies->isChecked() );
	qs.setValue( "Export3DPlotImageSeries/AnimateChannels", ui->checkBox_channels->isChecked() );
	qs.setValue( "Export3DPlotImageSeries/AnimateElevation", ui->checkBox_ele->isChecked() );

	qs.setValue( "Export3DPlotImageSeries/AnimateYawStart", ui->doubleSpinBox_AnimationYawStart->value() );
	qs.setValue( "Export3DPlotImageSeries/AnimateYawEnd", ui->doubleSpinBox_AnimationYawEnd->value() );
	qs.setValue( "Export3DPlotImageSeries/AnimatePitchStart", ui->doubleSpinBox_AnimationPitchStart->value() );
	qs.setValue( "Export3DPlotImageSeries/AnimatePitchEnd", ui->doubleSpinBox_AnimationPitchEnd->value() );
	qs.setValue( "Export3DPlotImageSeries/AnimateFrequencyIndexStart", ui->spinBox_AnimationFreqStartIndex->value() );
	qs.setValue( "Export3DPlotImageSeries/AnimateFrequencyIndexEnd", ui->spinBox_AnimationFreqEndIndex->value() );
	qs.setValue( "Export3DPlotImageSeries/AnimateChannelIndexStart", ui->spinBox_AnimationChannelStartIndex->value() );
	qs.setValue( "Export3DPlotImageSeries/AnimateChannelIndexEnd", ui->spinBox_AnimationChannelEndIndex->value() );
	qs.setValue( "Export3DPlotImageSeries/AnimateElevationStart", ui->doubleSpinBox_AnimationEleStart->value() );
	qs.setValue( "Export3DPlotImageSeries/AnimateElevationEnd", ui->doubleSpinBox_AnimationEleEnd->value() );

	delete ui;
}

void QDAFFDialogExport3DPlotImageSeries::LoadSettings()
{
	QSettings qs;

	restoreGeometry( qs.value( "Export3DPlotImageSeries/WindowGeometry" ).toByteArray() );

	ui->lineEdit_FileBaseName->setText( m_sFileBaseName );

	if( m_oBasePath.exists() )
	{
		if( !m_oBasePath.isAbsolute() )
			m_oBasePath.makeAbsolute();

		ui->lineEdit_ExportPath->setText( m_oBasePath.absolutePath() );
	}

	ui->spinBox_Width->setValue( qs.value( "Export3DPlotImageSeries/Width", 1920 ).toInt() );
	ui->spinBox_Height->setValue( qs.value( "Export3DPlotImageSeries/Height", 1080 ).toInt() );
	ui->spinBox_frames->setValue( qs.value( "Export3DPlotImageSeries/Frames", 75 ).toInt() );

	ui->checkBox_yaw->setChecked( qs.value( "Export3DPlotImageSeries/AnimateYaw", false ).toBool() );
	ui->checkBox_pitch->setChecked( qs.value( "Export3DPlotImageSeries/AnimatePitch", false ).toBool() );
	ui->checkBox_frequencies->setChecked( qs.value( "Export3DPlotImageSeries/AnimateFrequencies", false ).toBool() );
	ui->checkBox_channels->setChecked( qs.value( "Export3DPlotImageSeries/AnimateChannels", false ).toBool() );
	ui->checkBox_ele->setChecked( qs.value( "Export3DPlotImageSeries/AnimateElevation", false ).toBool() );

	ui->doubleSpinBox_AnimationYawStart->setValue( qs.value( "Export3DPlotImageSeries/AnimateYawStart", 0.0f ).toDouble() );
	ui->doubleSpinBox_AnimationYawEnd->setValue( qs.value( "Export3DPlotImageSeries/AnimateYawEnd", 0.0f ).toDouble() );
	ui->doubleSpinBox_AnimationPitchStart->setValue( qs.value( "Export3DPlotImageSeries/AnimatePitchStart", 0.0f ).toDouble() );
	ui->doubleSpinBox_AnimationPitchEnd->setValue( qs.value( "Export3DPlotImageSeries/AnimatePitchEnd", 0.0f ).toDouble() );
	ui->spinBox_AnimationFreqStartIndex->setValue( qs.value( "Export3DPlotImageSeries/AnimateFrequencyIndexStart", 0 ).toInt() );
	ui->spinBox_AnimationFreqEndIndex->setValue( qs.value( "Export3DPlotImageSeries/AnimateFrequencyIndexEnd", 0 ).toInt() );
	ui->spinBox_AnimationChannelStartIndex->setValue( qs.value( "Export3DPlotImageSeries/AnimateChannelIndexStart", 0 ).toInt() );
	ui->spinBox_AnimationChannelEndIndex->setValue( qs.value( "Export3DPlotImageSeries/AnimateChannelIndexEnd", 0 ).toInt() );
	ui->doubleSpinBox_AnimationEleStart->setValue( qs.value( "Export3DPlotImageSeries/AnimateElevatStart", 0.0f ).toDouble() );
	ui->doubleSpinBox_AnimationEleEnd->setValue( qs.value( "Export3DPlotImageSeries/AnimateElevationEnd", 0.0f ).toDouble() );

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
	return ui->spinBox_frames->value();
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

bool QDAFFDialogExport3DPlotImageSeries::GetAnimationYaw( double& dStart, double& dEnd ) const
{
	dStart = ui->doubleSpinBox_AnimationYawStart->value();
	dEnd = ui->doubleSpinBox_AnimationYawEnd->value();
	return ui->checkBox_yaw->isChecked();
}

bool QDAFFDialogExport3DPlotImageSeries::GetAnimationPitch( double& dStart, double& dEnd ) const
{
	dStart = ui->doubleSpinBox_AnimationPitchStart->value();
	dEnd = ui->doubleSpinBox_AnimationPitchEnd->value();
	return ui->checkBox_pitch->isChecked();
}

bool QDAFFDialogExport3DPlotImageSeries::GetAnimationElevation( double& dStart, double& dEnd ) const
{
	dStart = ui->doubleSpinBox_AnimationEleStart->value();
	dEnd = ui->doubleSpinBox_AnimationEleEnd->value();
	return ui->checkBox_ele->isChecked();
}

bool QDAFFDialogExport3DPlotImageSeries::GetAnimationChannelIndices( int& iStart, int& iEnd ) const
{
	iStart = ui->spinBox_AnimationChannelStartIndex->value();
	iEnd = ui->spinBox_AnimationChannelEndIndex->value();
	return ui->checkBox_channels->isChecked();
}

bool QDAFFDialogExport3DPlotImageSeries::GetAnimationFrequencyIndices( int& iStart, int& iEnd ) const
{
	iStart = ui->spinBox_AnimationFreqStartIndex->value();
	iEnd = ui->spinBox_AnimationFreqEndIndex->value();
	return ui->checkBox_frequencies->isChecked();
}

void QDAFFDialogExport3DPlotImageSeries::SetAnimationFrequencyIndices( int iStart, int iEnd )
{
	ui->spinBox_AnimationFreqStartIndex->setMinimum( iStart );
	ui->spinBox_AnimationFreqStartIndex->setMaximum( iEnd );
	ui->spinBox_AnimationFreqEndIndex->setMinimum( iStart );
	ui->spinBox_AnimationFreqEndIndex->setMaximum( iEnd );

	// Only update initial values
	if( ui->checkBox_frequencies->isChecked() == false )
	{
		ui->spinBox_AnimationFreqStartIndex->setValue( iStart );
		ui->spinBox_AnimationFreqStartIndex->setValue( iEnd );
	}
}
void QDAFFDialogExport3DPlotImageSeries::SetAnimationChannelIndices( int iStart, int iEnd )
{
	ui->spinBox_AnimationChannelStartIndex->setMinimum( iStart );
	ui->spinBox_AnimationChannelStartIndex->setMaximum( iEnd );
	ui->spinBox_AnimationChannelEndIndex->setMinimum( iStart );
	ui->spinBox_AnimationChannelEndIndex->setMaximum( iEnd );

	// Only update initial values
	if( ui->checkBox_channels->isChecked() == false )
	{
		ui->spinBox_AnimationChannelStartIndex->setValue( iStart );
		ui->spinBox_AnimationChannelEndIndex->setValue( iEnd );
	}
}

void QDAFFDialogExport3DPlotImageSeries::SetAnimationElevationRange( double dS, double dE )
{
	ui->doubleSpinBox_AnimationEleStart->setMinimum( dS );
	ui->doubleSpinBox_AnimationEleStart->setMaximum( dE );
	ui->doubleSpinBox_AnimationEleEnd->setMinimum( dS );
	ui->doubleSpinBox_AnimationEleEnd->setMaximum( dE );

	// Only update initial values
	if( ui->checkBox_ele->isChecked() == false )
	{
		ui->doubleSpinBox_AnimationEleStart->setValue( dS );
		ui->doubleSpinBox_AnimationEleEnd->setValue( dE );
	}
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
