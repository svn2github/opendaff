/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAFFVIZQTTESTWINDOW_H
#define UI_DAFFVIZQTTESTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QVTKWidget.h>

QT_BEGIN_NAMESPACE

class Ui_QtDAFFVizTestWindow
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QWidget *centralWidget;
    QTextBrowser *textFileInfo;
    QTextBrowser *textBrowser_2;
    QWidget *widgetPlot;
    QWidget *widgetPlotDisplay;
    QLabel *labelFrequency;
    QLabel *labelPhi;
    QLineEdit *lineEditFrequency;
    QLineEdit *lineEditPhi;
    QLabel *labelTheta;
    QLineEdit *lineEditTheta;
    QWidget *widgetBalloon;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

	QVTKWidget *vtkWidgetBalloon;
	QVTKWidget *vtkWidgetPlot;

	void setupUi(QMainWindow *DAFFVizQtWindow)
    {
		if (DAFFVizQtWindow->objectName().isEmpty())
			DAFFVizQtWindow->setObjectName(QStringLiteral("DAFFVizQtWindow"));
		DAFFVizQtWindow->resize(842, 615);
		actionOpen = new QAction(DAFFVizQtWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
		actionQuit = new QAction(DAFFVizQtWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
		centralWidget = new QWidget(DAFFVizQtWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        textFileInfo = new QTextBrowser(centralWidget);
        textFileInfo->setObjectName(QStringLiteral("textFileInfo"));
        textFileInfo->setGeometry(QRect(10, 10, 301, 271));
        textBrowser_2 = new QTextBrowser(centralWidget);
        textBrowser_2->setObjectName(QStringLiteral("textBrowser_2"));
        textBrowser_2->setGeometry(QRect(10, 290, 301, 241));
		widgetPlot = new QWidget(centralWidget);
		widgetPlot->setObjectName(QStringLiteral("widgetCarpet"));
		widgetPlot->setGeometry(QRect(320, 290, 511, 231));
		widgetPlotDisplay = new QWidget(widgetPlot);
		widgetPlotDisplay->setObjectName(QStringLiteral("widgetCarpetDisplay"));
		widgetPlotDisplay->setGeometry(QRect(10, 60, 491, 161));
		vtkWidgetPlot = new QVTKWidget(widgetPlotDisplay);
		vtkWidgetPlot->setObjectName(QStringLiteral("vtkWidgetCarpet"));
		vtkWidgetPlot->setGeometry(QRect(0, 0, 491, 161));
		labelFrequency = new QLabel(widgetPlot);
        labelFrequency->setObjectName(QStringLiteral("labelFrequency"));
        labelFrequency->setGeometry(QRect(10, 11, 61, 31));
		labelPhi = new QLabel(widgetPlot);
        labelPhi->setObjectName(QStringLiteral("labelPhi"));
        labelPhi->setGeometry(QRect(170, 20, 21, 16));
		lineEditFrequency = new QLineEdit(widgetPlot);
        lineEditFrequency->setObjectName(QStringLiteral("lineEditFrequency"));
		lineEditFrequency->setGeometry(QRect(70, 20, 91, 22));
		lineEditPhi = new QLineEdit(widgetPlot);
        lineEditPhi->setObjectName(QStringLiteral("lineEditPhi"));
		lineEditPhi->setGeometry(QRect(190, 20, 113, 22));
		labelTheta = new QLabel(widgetPlot);
        labelTheta->setObjectName(QStringLiteral("labelTheta"));
        labelTheta->setGeometry(QRect(310, 20, 41, 16));
		lineEditTheta = new QLineEdit(widgetPlot);
        lineEditTheta->setObjectName(QStringLiteral("lineEditTheta"));
        lineEditTheta->setGeometry(QRect(350, 20, 113, 22));
        widgetBalloon = new QWidget(centralWidget);
        widgetBalloon->setObjectName(QStringLiteral("widgetBalloon"));
        widgetBalloon->setGeometry(QRect(320, 10, 511, 271));
		vtkWidgetBalloon = new QVTKWidget(widgetBalloon);
		vtkWidgetBalloon->setObjectName(QStringLiteral("vtkWidgetBalloon"));
		vtkWidgetBalloon->setGeometry(QRect(0, 0, 511, 271));
		DAFFVizQtWindow->setCentralWidget(centralWidget);
		menuBar = new QMenuBar(DAFFVizQtWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 842, 26));
        menuBar->setDefaultUp(true);
        menuBar->setNativeMenuBar(false);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
		DAFFVizQtWindow->setMenuBar(menuBar);
		mainToolBar = new QToolBar(DAFFVizQtWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
		DAFFVizQtWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
		statusBar = new QStatusBar(DAFFVizQtWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
		DAFFVizQtWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionQuit);

		retranslateUi(DAFFVizQtWindow);

		QMetaObject::connectSlotsByName(DAFFVizQtWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        labelFrequency->setText(QApplication::translate("MainWindow", "Frequency", 0));
        labelPhi->setText(QApplication::translate("MainWindow", "Phi", 0));
        labelTheta->setText(QApplication::translate("MainWindow", "Theta", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
	class QtDAFFVizTestWindow : public Ui_QtDAFFVizTestWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
