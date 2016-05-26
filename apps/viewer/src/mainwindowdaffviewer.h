#ifndef MAINWINDOWTEST2_H
#define MAINWINDOWTEST2_H

#include <QMainWindow>

namespace Ui {
class DAFFViewer;
}

class MainWindowDAFFViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowDAFFViewer(QWidget *parent = 0, QString sPath = "" );
    ~MainWindowDAFFViewer();

private slots:
    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

private:
    Ui::DAFFViewer *ui;
};

#endif // MAINWINDOWTEST2_H
