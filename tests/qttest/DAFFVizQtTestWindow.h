#ifndef IW_DAFFVIZQTTESTWINDOW_H
#define IW_DAFFVIZQTTESTWINDOW_H

#include <QMainWindow>

namespace Ui {
class DAFFVizQtWindow;
}

class DAFFVizQtWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit DAFFVizQtWindow(QWidget *parent = 0);
    ~DAFFVizQtWindow();

public slots:

private slots:

    void on_actionQuit_triggered();

private:
    Ui::DAFFVizQtWindow *ui;
};

#endif // IW_DAFFVIZQTTESTWINDOW_H
