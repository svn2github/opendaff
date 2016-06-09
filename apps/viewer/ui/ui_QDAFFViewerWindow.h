/********************************************************************************
** Form generated from reading UI file 'QDAFFViewerWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDAFFVIEWERWINDOW_H
#define UI_QDAFFVIEWERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QDAFF2DPlot.h"
#include "QDAFFHeaderGroupBox.h"
#include "QDAFFMetadataTable.h"
#include "QDAFFPropertiesTable.h"
#include "QDAFFVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_DAFFViewer
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QAction *actionClose;
    QAction *actionOpenDAFFWebsite;
    QAction *actionAboutOpenDAFF;
    QAction *actionAboutDAFFViewer;
    QAction *actionDownload;
    QAction *actionCreate;
    QWidget *DAFFCentralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *layout_StaticInfo;
    QDAFFHeaderGroupBox *groupBox_Reader;
    QFormLayout *formLayout_2;
    QLabel *labelFileName;
    QLineEdit *lineEditFileName;
    QLabel *labelVersion;
    QLineEdit *lineEditVersion;
    QLabel *labelContentType;
    QLineEdit *lineEditContentType;
    QGroupBox *groupBox_Metadata;
    QVBoxLayout *verticalLayout;
    QDAFFMetadataTableView *tableView_Metadata;
    QGroupBox *groupBox_Properties;
    QVBoxLayout *verticalLayout_2;
    QDAFFPropertiesTableView *tableView_Properties;
    QVBoxLayout *layout_Visualization;
    QHBoxLayout *horizontalLayout_ChannelSwitcher;
    QPushButton *pushButton_Channel1;
    QPushButton *pushButton_Channel2;
    QFrame *frame_3DPlot;
    QGridLayout *gridLayout_2;
    QDAFFVTKWidget *DAFF3DPlot_VTKWidget;
    QFrame *frame_2DPlot;
    QGridLayout *gridLayout;
    QDAFF2DPlot *DAFF2DPlot_Frame;
    QMenuBar *DAFFMenuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QMenu *menuContent;
    QStatusBar *DAFFStatusBar;

    void setupUi(QMainWindow *DAFFViewer)
    {
        if (DAFFViewer->objectName().isEmpty())
            DAFFViewer->setObjectName(QStringLiteral("DAFFViewer"));
        DAFFViewer->resize(1024, 747);
        DAFFViewer->setAcceptDrops(true);
        DAFFViewer->setAutoFillBackground(false);
        DAFFViewer->setToolButtonStyle(Qt::ToolButtonIconOnly);
        DAFFViewer->setTabShape(QTabWidget::Rounded);
        actionOpen = new QAction(DAFFViewer);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionQuit = new QAction(DAFFViewer);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionClose = new QAction(DAFFViewer);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionOpenDAFFWebsite = new QAction(DAFFViewer);
        actionOpenDAFFWebsite->setObjectName(QStringLiteral("actionOpenDAFFWebsite"));
        actionAboutOpenDAFF = new QAction(DAFFViewer);
        actionAboutOpenDAFF->setObjectName(QStringLiteral("actionAboutOpenDAFF"));
        actionAboutDAFFViewer = new QAction(DAFFViewer);
        actionAboutDAFFViewer->setObjectName(QStringLiteral("actionAboutDAFFViewer"));
        actionDownload = new QAction(DAFFViewer);
        actionDownload->setObjectName(QStringLiteral("actionDownload"));
        actionCreate = new QAction(DAFFViewer);
        actionCreate->setObjectName(QStringLiteral("actionCreate"));
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
        layout_StaticInfo = new QVBoxLayout();
        layout_StaticInfo->setSpacing(6);
        layout_StaticInfo->setObjectName(QStringLiteral("layout_StaticInfo"));
        groupBox_Reader = new QDAFFHeaderGroupBox(DAFFCentralWidget);
        groupBox_Reader->setObjectName(QStringLiteral("groupBox_Reader"));
        groupBox_Reader->setMinimumSize(QSize(400, 0));
        groupBox_Reader->setMaximumSize(QSize(16777215, 16777215));
        groupBox_Reader->setStyleSheet(QStringLiteral(""));
        formLayout_2 = new QFormLayout(groupBox_Reader);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setHorizontalSpacing(12);
        formLayout_2->setVerticalSpacing(12);
        formLayout_2->setContentsMargins(20, 20, 20, 20);
        labelFileName = new QLabel(groupBox_Reader);
        labelFileName->setObjectName(QStringLiteral("labelFileName"));
        labelFileName->setStyleSheet(QStringLiteral(""));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, labelFileName);

        lineEditFileName = new QLineEdit(groupBox_Reader);
        lineEditFileName->setObjectName(QStringLiteral("lineEditFileName"));
        lineEditFileName->setEnabled(false);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEditFileName);

        labelVersion = new QLabel(groupBox_Reader);
        labelVersion->setObjectName(QStringLiteral("labelVersion"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, labelVersion);

        lineEditVersion = new QLineEdit(groupBox_Reader);
        lineEditVersion->setObjectName(QStringLiteral("lineEditVersion"));
        lineEditVersion->setEnabled(false);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEditVersion);

        labelContentType = new QLabel(groupBox_Reader);
        labelContentType->setObjectName(QStringLiteral("labelContentType"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, labelContentType);

        lineEditContentType = new QLineEdit(groupBox_Reader);
        lineEditContentType->setObjectName(QStringLiteral("lineEditContentType"));
        lineEditContentType->setEnabled(false);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, lineEditContentType);


        layout_StaticInfo->addWidget(groupBox_Reader);

        groupBox_Metadata = new QGroupBox(DAFFCentralWidget);
        groupBox_Metadata->setObjectName(QStringLiteral("groupBox_Metadata"));
        verticalLayout = new QVBoxLayout(groupBox_Metadata);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableView_Metadata = new QDAFFMetadataTableView(groupBox_Metadata);
        tableView_Metadata->setObjectName(QStringLiteral("tableView_Metadata"));
        tableView_Metadata->setMinimumSize(QSize(0, 100));

        verticalLayout->addWidget(tableView_Metadata);


        layout_StaticInfo->addWidget(groupBox_Metadata);

        groupBox_Properties = new QGroupBox(DAFFCentralWidget);
        groupBox_Properties->setObjectName(QStringLiteral("groupBox_Properties"));
        verticalLayout_2 = new QVBoxLayout(groupBox_Properties);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tableView_Properties = new QDAFFPropertiesTableView(groupBox_Properties);
        tableView_Properties->setObjectName(QStringLiteral("tableView_Properties"));

        verticalLayout_2->addWidget(tableView_Properties);


        layout_StaticInfo->addWidget(groupBox_Properties);

        layout_StaticInfo->setStretch(1, 1);
        layout_StaticInfo->setStretch(2, 1);

        horizontalLayout->addLayout(layout_StaticInfo);

        layout_Visualization = new QVBoxLayout();
        layout_Visualization->setSpacing(6);
        layout_Visualization->setObjectName(QStringLiteral("layout_Visualization"));
        horizontalLayout_ChannelSwitcher = new QHBoxLayout();
        horizontalLayout_ChannelSwitcher->setSpacing(6);
        horizontalLayout_ChannelSwitcher->setObjectName(QStringLiteral("horizontalLayout_ChannelSwitcher"));
        horizontalLayout_ChannelSwitcher->setContentsMargins(9, -1, 9, -1);
        pushButton_Channel1 = new QPushButton(DAFFCentralWidget);
        pushButton_Channel1->setObjectName(QStringLiteral("pushButton_Channel1"));

        horizontalLayout_ChannelSwitcher->addWidget(pushButton_Channel1);

        pushButton_Channel2 = new QPushButton(DAFFCentralWidget);
        pushButton_Channel2->setObjectName(QStringLiteral("pushButton_Channel2"));

        horizontalLayout_ChannelSwitcher->addWidget(pushButton_Channel2);


        layout_Visualization->addLayout(horizontalLayout_ChannelSwitcher);

        frame_3DPlot = new QFrame(DAFFCentralWidget);
        frame_3DPlot->setObjectName(QStringLiteral("frame_3DPlot"));
        frame_3DPlot->setFrameShape(QFrame::StyledPanel);
        frame_3DPlot->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_3DPlot);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        DAFF3DPlot_VTKWidget = new QDAFFVTKWidget(frame_3DPlot);
        DAFF3DPlot_VTKWidget->setObjectName(QStringLiteral("DAFF3DPlot_VTKWidget"));
        DAFF3DPlot_VTKWidget->setMinimumSize(QSize(400, 300));
        DAFF3DPlot_VTKWidget->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));

        gridLayout_2->addWidget(DAFF3DPlot_VTKWidget, 0, 0, 1, 1);

        gridLayout_2->setColumnStretch(0, 1);

        layout_Visualization->addWidget(frame_3DPlot);

        frame_2DPlot = new QFrame(DAFFCentralWidget);
        frame_2DPlot->setObjectName(QStringLiteral("frame_2DPlot"));
        frame_2DPlot->setFrameShape(QFrame::StyledPanel);
        frame_2DPlot->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_2DPlot);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        DAFF2DPlot_Frame = new QDAFF2DPlot(frame_2DPlot);
        DAFF2DPlot_Frame->setObjectName(QStringLiteral("DAFF2DPlot_Frame"));
        DAFF2DPlot_Frame->setMinimumSize(QSize(0, 300));
        DAFF2DPlot_Frame->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

        gridLayout->addWidget(DAFF2DPlot_Frame, 0, 0, 1, 1);


        layout_Visualization->addWidget(frame_2DPlot);

        layout_Visualization->setStretch(1, 1);
        layout_Visualization->setStretch(2, 1);

        horizontalLayout->addLayout(layout_Visualization);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 6);

        horizontalLayout_2->addLayout(horizontalLayout);

        DAFFViewer->setCentralWidget(DAFFCentralWidget);
        DAFFMenuBar = new QMenuBar(DAFFViewer);
        DAFFMenuBar->setObjectName(QStringLiteral("DAFFMenuBar"));
        DAFFMenuBar->setGeometry(QRect(0, 0, 1024, 21));
        menuFile = new QMenu(DAFFMenuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAbout = new QMenu(DAFFMenuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        menuContent = new QMenu(DAFFMenuBar);
        menuContent->setObjectName(QStringLiteral("menuContent"));
        DAFFViewer->setMenuBar(DAFFMenuBar);
        DAFFStatusBar = new QStatusBar(DAFFViewer);
        DAFFStatusBar->setObjectName(QStringLiteral("DAFFStatusBar"));
        DAFFViewer->setStatusBar(DAFFStatusBar);

        DAFFMenuBar->addAction(menuFile->menuAction());
        DAFFMenuBar->addAction(menuContent->menuAction());
        DAFFMenuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionClose);
        menuFile->addAction(actionQuit);
        menuAbout->addAction(actionAboutDAFFViewer);
        menuAbout->addAction(actionAboutOpenDAFF);
        menuAbout->addAction(actionOpenDAFFWebsite);
        menuContent->addAction(actionDownload);
        menuContent->addAction(actionCreate);
        menuContent->addSeparator();

        retranslateUi(DAFFViewer);

        QMetaObject::connectSlotsByName(DAFFViewer);
    } // setupUi

    void retranslateUi(QMainWindow *DAFFViewer)
    {
        DAFFViewer->setWindowTitle(QApplication::translate("DAFFViewer", "DAFFViewer", 0));
        actionOpen->setText(QApplication::translate("DAFFViewer", "Open", 0));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("DAFFViewer", "Open a DAFF fike", 0));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("DAFFViewer", "O", 0));
        actionQuit->setText(QApplication::translate("DAFFViewer", "Quit", 0));
#ifndef QT_NO_TOOLTIP
        actionQuit->setToolTip(QApplication::translate("DAFFViewer", "Quit DAFFViewer application", 0));
#endif // QT_NO_TOOLTIP
        actionQuit->setShortcut(QApplication::translate("DAFFViewer", "Esc", 0));
        actionClose->setText(QApplication::translate("DAFFViewer", "Close", 0));
#ifndef QT_NO_TOOLTIP
        actionClose->setToolTip(QApplication::translate("DAFFViewer", "Close current DAFF file", 0));
#endif // QT_NO_TOOLTIP
        actionClose->setShortcut(QApplication::translate("DAFFViewer", "C", 0));
        actionOpenDAFFWebsite->setText(QApplication::translate("DAFFViewer", "OpenDAFF project website", 0));
#ifndef QT_NO_TOOLTIP
        actionOpenDAFFWebsite->setToolTip(QApplication::translate("DAFFViewer", "Open the OpenDAFF project website", 0));
#endif // QT_NO_TOOLTIP
        actionOpenDAFFWebsite->setShortcut(QApplication::translate("DAFFViewer", "F1", 0));
        actionAboutOpenDAFF->setText(QApplication::translate("DAFFViewer", "About OpenDAFF", 0));
#ifndef QT_NO_TOOLTIP
        actionAboutOpenDAFF->setToolTip(QApplication::translate("DAFFViewer", "Show information about the OpenDAFF project", 0));
#endif // QT_NO_TOOLTIP
        actionAboutOpenDAFF->setShortcut(QApplication::translate("DAFFViewer", "A, O", 0));
        actionAboutDAFFViewer->setText(QApplication::translate("DAFFViewer", "About DAFFViewer", 0));
#ifndef QT_NO_TOOLTIP
        actionAboutDAFFViewer->setToolTip(QApplication::translate("DAFFViewer", "Show information about the DAFFViewer application", 0));
#endif // QT_NO_TOOLTIP
        actionAboutDAFFViewer->setShortcut(QApplication::translate("DAFFViewer", "A, D", 0));
        actionDownload->setText(QApplication::translate("DAFFViewer", "Download", 0));
#ifndef QT_NO_TOOLTIP
        actionDownload->setToolTip(QApplication::translate("DAFFViewer", "Download DAFF content online", 0));
#endif // QT_NO_TOOLTIP
        actionDownload->setShortcut(QApplication::translate("DAFFViewer", "D", 0));
        actionCreate->setText(QApplication::translate("DAFFViewer", "Create", 0));
#ifndef QT_NO_TOOLTIP
        actionCreate->setToolTip(QApplication::translate("DAFFViewer", "Create or generate your own DAFF content", 0));
#endif // QT_NO_TOOLTIP
        actionCreate->setShortcut(QApplication::translate("DAFFViewer", "N", 0));
        groupBox_Reader->setTitle(QApplication::translate("DAFFViewer", "DAFF Reader ", 0));
        labelFileName->setText(QApplication::translate("DAFFViewer", "File name", 0));
        labelVersion->setText(QApplication::translate("DAFFViewer", "Version", 0));
        labelContentType->setText(QApplication::translate("DAFFViewer", "Content type", 0));
        groupBox_Metadata->setTitle(QApplication::translate("DAFFViewer", "DAFF Metadata", 0));
        groupBox_Properties->setTitle(QApplication::translate("DAFFViewer", "DAFF Properties", 0));
        pushButton_Channel1->setText(QApplication::translate("DAFFViewer", "Channel 1", 0));
        pushButton_Channel2->setText(QApplication::translate("DAFFViewer", "Channel 2", 0));
        menuFile->setTitle(QApplication::translate("DAFFViewer", "File", 0));
        menuAbout->setTitle(QApplication::translate("DAFFViewer", "About", 0));
        menuContent->setTitle(QApplication::translate("DAFFViewer", "Content", 0));
    } // retranslateUi

};

namespace Ui {
    class DAFFViewer: public Ui_DAFFViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDAFFVIEWERWINDOW_H
