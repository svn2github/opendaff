#include "DAFFVizQtTestWindow.h"
#include "ui_DAFFVizQtTestWindow.h"


DAFFVizQtWindow::DAFFVizQtWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DAFFVizQtWindow)
{
    ui->setupUi(this);
}

DAFFVizQtWindow::~DAFFVizQtWindow()
{
    delete ui;
}

void DAFFVizQtWindow::on_actionQuit_triggered()
{
    close();
}
