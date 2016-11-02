#ifndef DAFFDIALOGEXPORT3DPLOTIMAGESERIES_H
#define DAFFDIALOGEXPORT3DPLOTIMAGESERIES_H

#include <QDialog>
#include <QDir>
#include <QString>
#include <QSettings>

namespace Ui {
class QDAFFDialogExport3DPlotImageSeries;
}

class QDAFFDialogExport3DPlotImageSeries : public QDialog
{
    Q_OBJECT

public:

	explicit QDAFFDialogExport3DPlotImageSeries( QWidget *parent, const QDir& oBasePath, const QString& sFileBaseName );
    ~QDAFFDialogExport3DPlotImageSeries();

	QString GetFileBaseName() const;
	QString GetExportPath() const;

	int GetNumFrames() const;
	int GetWidth() const;
	int GetHeight() const;

	bool GetAnimationYaw( double&, double& ) const;
	bool GetAnimationPitch( double&, double& ) const;
	bool GetAnimationFrequencyIndices( int&, int& ) const;
	bool GetAnimationChannelIndices( int&, int& ) const;
	bool GetAnimationElevation( double&, double& ) const;

	void SetAnimationFrequencyIndices( int, int );
	void SetAnimationChannelIndices( int, int );
	void SetAnimationElevationRange( double, double );

private slots:
    void on_pushButton_Export_clicked();
	void on_pushButton_Browse_clicked();

private:
    Ui::QDAFFDialogExport3DPlotImageSeries *ui;
    QDir m_oBasePath;
	QString m_sFileBaseName;
	bool m_bExport;

	void LoadSettings();
	void StoreSettings() const;
};

#endif // DAFFDIALOGEXPORT3DPLOTIMAGESERIES_H
