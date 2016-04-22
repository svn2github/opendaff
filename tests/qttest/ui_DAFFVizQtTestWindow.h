/********************************************************************************
** Form generated from reading UI file 'DAFFVizQtTestWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAFFVIZQTTESTWINDOW_H
#define UI_DAFFVIZQTTESTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DAFFVizQtWindow
{
public:
    QAction *actionQuit;
    QWidget *centralWidget;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DAFFVizQtWindow)
    {
        if (DAFFVizQtWindow->objectName().isEmpty())
            DAFFVizQtWindow->setObjectName(QStringLiteral("DAFFVizQtWindow"));
        DAFFVizQtWindow->resize(422, 270);
        actionQuit = new QAction(DAFFVizQtWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        centralWidget = new QWidget(DAFFVizQtWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(80, 90, 261, 31));
        DAFFVizQtWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DAFFVizQtWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 422, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        DAFFVizQtWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(DAFFVizQtWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DAFFVizQtWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);

        retranslateUi(DAFFVizQtWindow);

        QMetaObject::connectSlotsByName(DAFFVizQtWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DAFFVizQtWindow)
    {
        DAFFVizQtWindow->setWindowTitle(QApplication::translate("DAFFVizQtWindow", "Qt test", 0));
        actionQuit->setText(QApplication::translate("DAFFVizQtWindow", "Quit", 0));
        plainTextEdit->setPlainText(QApplication::translate("DAFFVizQtWindow", "This is the Qt test application from OpenDAFF.", 0));
        menuFile->setTitle(QApplication::translate("DAFFVizQtWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class DAFFVizQtWindow: public Ui_DAFFVizQtWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DAFFVIZQTTESTWINDOW_H
