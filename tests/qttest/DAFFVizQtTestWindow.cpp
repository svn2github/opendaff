/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

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
