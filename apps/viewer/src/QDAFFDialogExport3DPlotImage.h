#ifndef DAFFDIALOGEXPORT3DPLOTIMAGE_H
#define DAFFDIALOGEXPORT3DPLOTIMAGE_H

#include <QDialog>
#include <QDir>
#include <QString>
#include <QSettings>

namespace Ui
{
	class QDAFFDialogExport3DPlotImage;
}

class QDAFFDialogExport3DPlotImage : public QDialog
{
    Q_OBJECT

public:

	explicit QDAFFDialogExport3DPlotImage( QWidget *parent, const QDir& oBasePath, const QString& sFileBaseName );
    ~QDAFFDialogExport3DPlotImage();

	QString GetExportPath() const;
	int GetWidth() const;
	int GetHeight() const;

private slots:
    void on_pushButton_Export_clicked();
	void on_pushButton_Browse_clicked();

private:
    Ui::QDAFFDialogExport3DPlotImage *ui;
    QDir m_oBasePath;
	QString m_sFileBaseName;
	QSettings m_qSettings;
	bool m_bExport;
};

#endif // DAFFDIALOGEXPORT3DPLOTIMAGE_H
