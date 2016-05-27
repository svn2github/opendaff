#ifndef MAINWINDOWTEST2_H
#define MAINWINDOWTEST2_H

#include <QMainWindow>
#include <qsettings.h>

class DAFFReader;

namespace Ui {
class DAFFViewer;
}

class MainWindowDAFFViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowDAFFViewer(QWidget *parent = 0, QString sPath = "" );
    ~MainWindowDAFFViewer();

    void OpenDAFFFile( QString sPath, bool bQuiet = false );

private slots:
    void on_actionOpen_triggered();

    void on_actionQuit_triggered();


signals:
    void readDAFF( const DAFFReader* );

private:
    Ui::DAFFViewer *ui;
    QSettings m_qSettings;

    DAFFReader* m_pDAFFReader;
};

#endif // MAINWINDOWTEST2_H
