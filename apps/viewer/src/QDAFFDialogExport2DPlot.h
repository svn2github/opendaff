#ifndef DAFFDIALOGEXPORT2DPLOT_H
#define DAFFDIALOGEXPORT2DPLOT_H

#include <QDialog>
#include <QDir>
#include <QString>
#include <QSettings>

namespace Ui {
class QDAFFDialogExport2DPlot;
}

class QDAFFDialogExport2DPlot : public QDialog
{
    Q_OBJECT

public:

	enum
	{
		EXPORT_UNKOWN_TYPE = -1,
		EXPORT_TYPE_PNG,
		EXPORT_TYPE_SVG
	};

    explicit QDAFFDialogExport2DPlot(QWidget *parent, const QDir& oBasePath, const QString& sFileBaseName );
    ~QDAFFDialogExport2DPlot();

	QString GetExportPath() const;
	bool ExportAllChannels() const;
	bool ShowDataPointDots() const;
	float GetScalingFactor() const;
	int GetExportType() const;

private slots:
    void on_pushButton_Export_clicked();
	void on_pushButton_Browse_clicked();

private:
    Ui::QDAFFDialogExport2DPlot *ui;
    QDir m_oBasePath;
	QString m_sFileBaseName, m_sTargetFileType;
	QSettings m_qSettings;
	bool m_bExport;
};

#endif // DAFFDIALOGEXPORT2DPLOT_H
