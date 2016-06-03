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
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "daffmetadatatableview.h"
#include "daffreadergroupbox.h"
#include "qdaffvtkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_DAFFViewer
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QWidget *DAFFCentralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *DAFFFileInfo;
    DAFFReaderGroupBox *groupBox;
    QFormLayout *formLayout_2;
    QLabel *labelFileName;
    QLineEdit *lineEditFileName;
    QLabel *labelVersion;
    QLineEdit *lineEditVersion;
    QLabel *labelContentType;
    QLineEdit *lineEditContentType;
    QFrame *line;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    DAFFMetadataTableView *tableView;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer;
    QFrame *line_2;
    QVBoxLayout *DAFFVisualization;
    QHBoxLayout *horizontalLayout_ChannelSwitcher;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QDAFFVTKWidget *widget;
    QFrame *line_3;
    QFrame *frame_2;
    QGridLayout *gridLayout;
    QMdiArea *mdiArea;
    QMenuBar *DAFFMenuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DAFFViewer)
    {
        if (DAFFViewer->objectName().isEmpty())
            DAFFViewer->setObjectName(QStringLiteral("DAFFViewer"));
        DAFFViewer->resize(1024, 512);
        DAFFViewer->setAcceptDrops(true);
        DAFFViewer->setAutoFillBackground(false);
        DAFFViewer->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        DAFFViewer->setToolButtonStyle(Qt::ToolButtonIconOnly);
        DAFFViewer->setTabShape(QTabWidget::Rounded);
        actionOpen = new QAction(DAFFViewer);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionQuit = new QAction(DAFFViewer);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        DAFFCentralWidget = new QWidget(DAFFViewer);
        DAFFCentralWidget->setObjectName(QStringLiteral("DAFFCentralWidget"));
        DAFFCentralWidget->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DAFFCentralWidget->sizePolicy().hasHeightForWidth());
        DAFFCentralWidget->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(DAFFCentralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        DAFFFileInfo = new QVBoxLayout();
        DAFFFileInfo->setSpacing(6);
        DAFFFileInfo->setObjectName(QStringLiteral("DAFFFileInfo"));
        groupBox = new DAFFReaderGroupBox(DAFFCentralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(400, 0));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        groupBox->setStyleSheet(QStringLiteral(""));
        formLayout_2 = new QFormLayout(groupBox);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setHorizontalSpacing(12);
        formLayout_2->setVerticalSpacing(12);
        formLayout_2->setContentsMargins(20, 20, 20, 20);
        labelFileName = new QLabel(groupBox);
        labelFileName->setObjectName(QStringLiteral("labelFileName"));
        labelFileName->setStyleSheet(QStringLiteral(""));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, labelFileName);

        lineEditFileName = new QLineEdit(groupBox);
        lineEditFileName->setObjectName(QStringLiteral("lineEditFileName"));
        lineEditFileName->setEnabled(false);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEditFileName);

        labelVersion = new QLabel(groupBox);
        labelVersion->setObjectName(QStringLiteral("labelVersion"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, labelVersion);

        lineEditVersion = new QLineEdit(groupBox);
        lineEditVersion->setObjectName(QStringLiteral("lineEditVersion"));
        lineEditVersion->setEnabled(false);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEditVersion);

        labelContentType = new QLabel(groupBox);
        labelContentType->setObjectName(QStringLiteral("labelContentType"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, labelContentType);

        lineEditContentType = new QLineEdit(groupBox);
        lineEditContentType->setObjectName(QStringLiteral("lineEditContentType"));
        lineEditContentType->setEnabled(false);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, lineEditContentType);


        DAFFFileInfo->addWidget(groupBox);

        line = new QFrame(DAFFCentralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        DAFFFileInfo->addWidget(line);

        scrollArea = new QScrollArea(DAFFCentralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 383, 1018));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableView = new DAFFMetadataTableView(scrollAreaWidgetContents);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setMinimumSize(QSize(0, 1000));

        verticalLayout->addWidget(tableView);

        scrollArea->setWidget(scrollAreaWidgetContents);

        DAFFFileInfo->addWidget(scrollArea);

        pushButton_2 = new QPushButton(DAFFCentralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        DAFFFileInfo->addWidget(pushButton_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DAFFFileInfo->addItem(verticalSpacer);


        horizontalLayout->addLayout(DAFFFileInfo);

        line_2 = new QFrame(DAFFCentralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setLineWidth(3);
        line_2->setMidLineWidth(0);
        line_2->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(line_2);

        DAFFVisualization = new QVBoxLayout();
        DAFFVisualization->setSpacing(6);
        DAFFVisualization->setObjectName(QStringLiteral("DAFFVisualization"));
        horizontalLayout_ChannelSwitcher = new QHBoxLayout();
        horizontalLayout_ChannelSwitcher->setSpacing(6);
        horizontalLayout_ChannelSwitcher->setObjectName(QStringLiteral("horizontalLayout_ChannelSwitcher"));
        pushButton_3 = new QPushButton(DAFFCentralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_ChannelSwitcher->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(DAFFCentralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_ChannelSwitcher->addWidget(pushButton_4);


        DAFFVisualization->addLayout(horizontalLayout_ChannelSwitcher);

        frame = new QFrame(DAFFCentralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        widget = new QDAFFVTKWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(400, 0));
        widget->setStyleSheet(QStringLiteral("background-color: rgb(255, 170, 0);"));

        gridLayout_2->addWidget(widget, 0, 0, 1, 1);

        gridLayout_2->setColumnStretch(0, 1);

        DAFFVisualization->addWidget(frame);

        line_3 = new QFrame(DAFFCentralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShadow(QFrame::Plain);
        line_3->setLineWidth(3);
        line_3->setFrameShape(QFrame::HLine);

        DAFFVisualization->addWidget(line_3);

        frame_2 = new QFrame(DAFFCentralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        mdiArea = new QMdiArea(frame_2);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));

        gridLayout->addWidget(mdiArea, 0, 0, 1, 1);


        DAFFVisualization->addWidget(frame_2);

        DAFFVisualization->setStretch(1, 1);
        DAFFVisualization->setStretch(3, 1);

        horizontalLayout->addLayout(DAFFVisualization);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(2, 6);

        horizontalLayout_2->addLayout(horizontalLayout);

        DAFFViewer->setCentralWidget(DAFFCentralWidget);
        DAFFMenuBar = new QMenuBar(DAFFViewer);
        DAFFMenuBar->setObjectName(QStringLiteral("DAFFMenuBar"));
        DAFFMenuBar->setGeometry(QRect(0, 0, 1024, 21));
        menuFile = new QMenu(DAFFMenuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        DAFFViewer->setMenuBar(DAFFMenuBar);
        statusBar = new QStatusBar(DAFFViewer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DAFFViewer->setStatusBar(statusBar);

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
        actionOpen->setShortcut(QApplication::translate("DAFFViewer", "O", 0));
        actionQuit->setText(QApplication::translate("DAFFViewer", "Quit", 0));
        actionQuit->setShortcut(QApplication::translate("DAFFViewer", "Esc", 0));
        groupBox->setTitle(QApplication::translate("DAFFViewer", "DAFF Reader ", 0));
        labelFileName->setText(QApplication::translate("DAFFViewer", "File name", 0));
        labelVersion->setText(QApplication::translate("DAFFViewer", "Version", 0));
        labelContentType->setText(QApplication::translate("DAFFViewer", "Content type", 0));
        pushButton_2->setText(QApplication::translate("DAFFViewer", "Properties", 0));
        pushButton_3->setText(QApplication::translate("DAFFViewer", "Channel 1", 0));
        pushButton_4->setText(QApplication::translate("DAFFViewer", "Channel 2", 0));
        menuFile->setTitle(QApplication::translate("DAFFViewer", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class DAFFViewer: public Ui_DAFFViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWDAFFVIEWER_H
