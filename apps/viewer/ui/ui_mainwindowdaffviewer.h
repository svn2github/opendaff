/********************************************************************************
** Form generated from reading UI file 'mainwindowdaffviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOWDAFFVIEWER_H
#define UI_MAINWINDOWDAFFVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DAFFViewer
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QWidget *DAFFCentralWidget;
    QFrame *DAFFFrame;
    QWidget *widget;
    QGroupBox *groupBox;
    QSlider *verticalSlider;
    QSlider *horizontalSlider;
    QSlider *horizontalSlider_2;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QSpinBox *spinBox_3;
    QListView *listView;
    QMenuBar *DAFFMenuBar;
    QMenu *menuFile;
    QToolBar *DAFFToolBar;
    QStatusBar *DAFFStatusBar;

    void setupUi(QMainWindow *DAFFViewer)
    {
        if (DAFFViewer->objectName().isEmpty())
            DAFFViewer->setObjectName(QStringLiteral("DAFFViewer"));
        DAFFViewer->resize(1024, 786);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DAFFViewer->sizePolicy().hasHeightForWidth());
        DAFFViewer->setSizePolicy(sizePolicy);
        DAFFViewer->setAcceptDrops(true);
        DAFFViewer->setAutoFillBackground(false);
        DAFFViewer->setTabShape(QTabWidget::Rounded);
        actionOpen = new QAction(DAFFViewer);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionQuit = new QAction(DAFFViewer);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        DAFFCentralWidget = new QWidget(DAFFViewer);
        DAFFCentralWidget->setObjectName(QStringLiteral("DAFFCentralWidget"));
        DAFFFrame = new QFrame(DAFFCentralWidget);
        DAFFFrame->setObjectName(QStringLiteral("DAFFFrame"));
        DAFFFrame->setGeometry(QRect(50, 10, 771, 561));
        sizePolicy.setHeightForWidth(DAFFFrame->sizePolicy().hasHeightForWidth());
        DAFFFrame->setSizePolicy(sizePolicy);
        DAFFFrame->setFrameShape(QFrame::StyledPanel);
        DAFFFrame->setFrameShadow(QFrame::Raised);
        widget = new QWidget(DAFFFrame);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(290, 50, 341, 231));
        widget->setStyleSheet(QStringLiteral("color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.33 rgba(0, 0, 0, 255), stop:0.34 rgba(255, 30, 30, 255), stop:0.66 rgba(255, 0, 0, 255), stop:0.67 rgba(255, 255, 0, 255), stop:1 rgba(255, 255, 0, 255));"));
        groupBox = new QGroupBox(DAFFFrame);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(40, 30, 221, 251));
        verticalSlider = new QSlider(DAFFFrame);
        verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
        verticalSlider->setGeometry(QRect(660, 50, 20, 241));
        verticalSlider->setOrientation(Qt::Vertical);
        horizontalSlider = new QSlider(DAFFFrame);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(290, 320, 341, 20));
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider_2 = new QSlider(DAFFFrame);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setGeometry(QRect(290, 20, 341, 20));
        horizontalSlider_2->setOrientation(Qt::Horizontal);
        spinBox = new QSpinBox(DAFFFrame);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(350, 290, 43, 22));
        spinBox_2 = new QSpinBox(DAFFFrame);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setGeometry(QRect(430, 290, 43, 22));
        spinBox_3 = new QSpinBox(DAFFFrame);
        spinBox_3->setObjectName(QStringLiteral("spinBox_3"));
        spinBox_3->setGeometry(QRect(510, 290, 43, 22));
        listView = new QListView(DAFFFrame);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(280, 350, 401, 192));
        DAFFViewer->setCentralWidget(DAFFCentralWidget);
        DAFFMenuBar = new QMenuBar(DAFFViewer);
        DAFFMenuBar->setObjectName(QStringLiteral("DAFFMenuBar"));
        DAFFMenuBar->setGeometry(QRect(0, 0, 832, 21));
        menuFile = new QMenu(DAFFMenuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        DAFFViewer->setMenuBar(DAFFMenuBar);
        DAFFToolBar = new QToolBar(DAFFViewer);
        DAFFToolBar->setObjectName(QStringLiteral("DAFFToolBar"));
        DAFFViewer->addToolBar(Qt::TopToolBarArea, DAFFToolBar);
        DAFFStatusBar = new QStatusBar(DAFFViewer);
        DAFFStatusBar->setObjectName(QStringLiteral("DAFFStatusBar"));
        DAFFViewer->setStatusBar(DAFFStatusBar);

        DAFFMenuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionQuit);

        retranslateUi(DAFFViewer);

        QMetaObject::connectSlotsByName(DAFFViewer);
    } // setupUi

    void retranslateUi(QMainWindow *DAFFViewer)
    {
        DAFFViewer->setWindowTitle(QApplication::translate("DAFFViewer", "DAFFViewer", 0));
        actionOpen->setText(QApplication::translate("DAFFViewer", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("DAFFViewer", "Ctrl+O", 0));
        actionQuit->setText(QApplication::translate("DAFFViewer", "Quit", 0));
        groupBox->setTitle(QApplication::translate("DAFFViewer", "GroupBox", 0));
        menuFile->setTitle(QApplication::translate("DAFFViewer", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class DAFFViewer: public Ui_DAFFViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWDAFFVIEWER_H
