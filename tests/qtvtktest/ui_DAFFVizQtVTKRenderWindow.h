/********************************************************************************
** Form generated from reading UI file 'DAFFVizQtVTKRenderWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAFFVIZQTVTKRENDERWINDOW_H
#define UI_DAFFVIZQTVTKRENDERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_DAFFVizQtVTKRenderWindow
{
public:
    QAction *actionOpenFile;
    QAction *actionExit;
    QAction *actionPrint;
    QAction *actionHelp;
    QAction *actionSave;
    QWidget *centralwidget;
    QVTKWidget *qvtkWidget;

    void setupUi(QMainWindow *DAFFVizQtVTKRenderWindow)
    {
        if (DAFFVizQtVTKRenderWindow->objectName().isEmpty())
            DAFFVizQtVTKRenderWindow->setObjectName(QStringLiteral("DAFFVizQtVTKRenderWindow"));
        DAFFVizQtVTKRenderWindow->resize(541, 583);
        actionOpenFile = new QAction(DAFFVizQtVTKRenderWindow);
        actionOpenFile->setObjectName(QStringLiteral("actionOpenFile"));
        actionOpenFile->setEnabled(true);
        actionExit = new QAction(DAFFVizQtVTKRenderWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionPrint = new QAction(DAFFVizQtVTKRenderWindow);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        actionHelp = new QAction(DAFFVizQtVTKRenderWindow);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionSave = new QAction(DAFFVizQtVTKRenderWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        centralwidget = new QWidget(DAFFVizQtVTKRenderWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        qvtkWidget = new QVTKWidget(centralwidget);
        qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));
        qvtkWidget->setGeometry(QRect(10, 20, 511, 541));
        DAFFVizQtVTKRenderWindow->setCentralWidget(centralwidget);

        retranslateUi(DAFFVizQtVTKRenderWindow);

        QMetaObject::connectSlotsByName(DAFFVizQtVTKRenderWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DAFFVizQtVTKRenderWindow)
    {
        DAFFVizQtVTKRenderWindow->setWindowTitle(QApplication::translate("DAFFVizQtVTKRenderWindow", "DAFFVizQtVTKRenderWindow", 0));
        actionOpenFile->setText(QApplication::translate("DAFFVizQtVTKRenderWindow", "Open File...", 0));
        actionExit->setText(QApplication::translate("DAFFVizQtVTKRenderWindow", "Exit", 0));
        actionPrint->setText(QApplication::translate("DAFFVizQtVTKRenderWindow", "Print", 0));
        actionHelp->setText(QApplication::translate("DAFFVizQtVTKRenderWindow", "Help", 0));
        actionSave->setText(QApplication::translate("DAFFVizQtVTKRenderWindow", "Save", 0));
    } // retranslateUi

};

namespace Ui {
    class DAFFVizQtVTKRenderWindow: public Ui_DAFFVizQtVTKRenderWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DAFFVIZQTVTKRENDERWINDOW_H
