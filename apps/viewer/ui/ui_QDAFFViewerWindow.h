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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
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
#include <QtWidgets/QSpinBox>
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
    QAction *actionIncrease_channel;
    QAction *actionDecrease_channel;
    QAction *actionIncrease_Frequency;
    QAction *actionDecrease_Frequency;
    QAction *actionIncrease_phi;
    QAction *actionDecrease_phi;
    QAction *actionIncrease_theta;
    QAction *actionDecrease_theta;
    QAction *actionIncrease_alpha;
    QAction *actionDecrease_alpha;
    QAction *actionIncrease_beta;
    QAction *actionDecrease_beta;
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
    QFrame *frame_3DPlot;
    QGridLayout *gridLayout_2;
    QDAFFVTKWidget *DAFF3DPlot_VTKWidget;
    QHBoxLayout *horizontalLayout_InteractionPanel;
    QGroupBox *groupBox_ObjectView;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *doubleSpinBox_Phi;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_Theta;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_6;
    QGroupBox *groupBox_DataView;
    QGridLayout *gridLayout_4;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_Alpha;
    QDoubleSpinBox *doubleSpinBox_Beta;
    QLabel *label_4;
    QLabel *label_7;
    QLabel *label_8;
    QGroupBox *groupBox_Record;
    QFormLayout *formLayout;
    QLabel *label_9;
    QSpinBox *spinBox_RecordIndex;
    QGroupBox *groupBox_Channel;
    QFormLayout *formLayout_3;
    QLabel *label_12;
    QSpinBox *spinBox_ChannelIndex;
    QGroupBox *groupBox_Frequency;
    QFormLayout *formLayout_4;
    QLabel *label_11;
    QSpinBox *spinBox_FrequencyIndex;
    QLabel *label_10;
    QComboBox *comboBox_FrequencySelector;
    QFrame *frame_2DPlot;
    QGridLayout *gridLayout;
    QDAFF2DPlot *DAFF2DPlot_Frame;
    QMenuBar *DAFFMenuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QMenu *menuContent;
    QMenu *menuObject_view;
    QMenu *menuData_view;
    QMenu *menuEdit;
    QStatusBar *DAFFStatusBar;

    void setupUi(QMainWindow *DAFFViewer)
    {
        if (DAFFViewer->objectName().isEmpty())
            DAFFViewer->setObjectName(QStringLiteral("DAFFViewer"));
        DAFFViewer->resize(1102, 812);
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
        actionIncrease_channel = new QAction(DAFFViewer);
        actionIncrease_channel->setObjectName(QStringLiteral("actionIncrease_channel"));
        actionDecrease_channel = new QAction(DAFFViewer);
        actionDecrease_channel->setObjectName(QStringLiteral("actionDecrease_channel"));
        actionIncrease_Frequency = new QAction(DAFFViewer);
        actionIncrease_Frequency->setObjectName(QStringLiteral("actionIncrease_Frequency"));
        actionDecrease_Frequency = new QAction(DAFFViewer);
        actionDecrease_Frequency->setObjectName(QStringLiteral("actionDecrease_Frequency"));
        actionIncrease_phi = new QAction(DAFFViewer);
        actionIncrease_phi->setObjectName(QStringLiteral("actionIncrease_phi"));
        actionDecrease_phi = new QAction(DAFFViewer);
        actionDecrease_phi->setObjectName(QStringLiteral("actionDecrease_phi"));
        actionIncrease_theta = new QAction(DAFFViewer);
        actionIncrease_theta->setObjectName(QStringLiteral("actionIncrease_theta"));
        actionDecrease_theta = new QAction(DAFFViewer);
        actionDecrease_theta->setObjectName(QStringLiteral("actionDecrease_theta"));
        actionIncrease_alpha = new QAction(DAFFViewer);
        actionIncrease_alpha->setObjectName(QStringLiteral("actionIncrease_alpha"));
        actionDecrease_alpha = new QAction(DAFFViewer);
        actionDecrease_alpha->setObjectName(QStringLiteral("actionDecrease_alpha"));
        actionIncrease_beta = new QAction(DAFFViewer);
        actionIncrease_beta->setObjectName(QStringLiteral("actionIncrease_beta"));
        actionDecrease_beta = new QAction(DAFFViewer);
        actionDecrease_beta->setObjectName(QStringLiteral("actionDecrease_beta"));
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

        horizontalLayout_InteractionPanel = new QHBoxLayout();
        horizontalLayout_InteractionPanel->setSpacing(6);
        horizontalLayout_InteractionPanel->setObjectName(QStringLiteral("horizontalLayout_InteractionPanel"));
        horizontalLayout_InteractionPanel->setContentsMargins(9, -1, 9, -1);
        groupBox_ObjectView = new QGroupBox(DAFFCentralWidget);
        groupBox_ObjectView->setObjectName(QStringLiteral("groupBox_ObjectView"));
        gridLayout_3 = new QGridLayout(groupBox_ObjectView);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        doubleSpinBox_Phi = new QDoubleSpinBox(groupBox_ObjectView);
        doubleSpinBox_Phi->setObjectName(QStringLiteral("doubleSpinBox_Phi"));
        doubleSpinBox_Phi->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_Phi->setDecimals(1);
        doubleSpinBox_Phi->setMaximum(360);
        doubleSpinBox_Phi->setSingleStep(5);

        gridLayout_3->addWidget(doubleSpinBox_Phi, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox_ObjectView);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        doubleSpinBox_Theta = new QDoubleSpinBox(groupBox_ObjectView);
        doubleSpinBox_Theta->setObjectName(QStringLiteral("doubleSpinBox_Theta"));
        doubleSpinBox_Theta->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_Theta->setDecimals(1);
        doubleSpinBox_Theta->setMinimum(-90);
        doubleSpinBox_Theta->setMaximum(90);
        doubleSpinBox_Theta->setSingleStep(5);

        gridLayout_3->addWidget(doubleSpinBox_Theta, 1, 1, 1, 1);

        label = new QLabel(groupBox_ObjectView);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(groupBox_ObjectView);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_3->addWidget(label_5, 0, 2, 1, 1);

        label_6 = new QLabel(groupBox_ObjectView);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 1, 2, 1, 1);


        horizontalLayout_InteractionPanel->addWidget(groupBox_ObjectView);

        groupBox_DataView = new QGroupBox(DAFFCentralWidget);
        groupBox_DataView->setObjectName(QStringLiteral("groupBox_DataView"));
        gridLayout_4 = new QGridLayout(groupBox_DataView);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_3 = new QLabel(groupBox_DataView);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_4->addWidget(label_3, 0, 2, 1, 1);

        doubleSpinBox_Alpha = new QDoubleSpinBox(groupBox_DataView);
        doubleSpinBox_Alpha->setObjectName(QStringLiteral("doubleSpinBox_Alpha"));
        doubleSpinBox_Alpha->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_Alpha->setDecimals(1);
        doubleSpinBox_Alpha->setMaximum(360);
        doubleSpinBox_Alpha->setSingleStep(5);

        gridLayout_4->addWidget(doubleSpinBox_Alpha, 0, 3, 1, 1);

        doubleSpinBox_Beta = new QDoubleSpinBox(groupBox_DataView);
        doubleSpinBox_Beta->setObjectName(QStringLiteral("doubleSpinBox_Beta"));
        doubleSpinBox_Beta->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        doubleSpinBox_Beta->setDecimals(1);
        doubleSpinBox_Beta->setMaximum(180);
        doubleSpinBox_Beta->setSingleStep(5);

        gridLayout_4->addWidget(doubleSpinBox_Beta, 1, 3, 1, 1);

        label_4 = new QLabel(groupBox_DataView);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_4->addWidget(label_4, 1, 2, 1, 1);

        label_7 = new QLabel(groupBox_DataView);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_4->addWidget(label_7, 0, 4, 1, 1);

        label_8 = new QLabel(groupBox_DataView);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_4->addWidget(label_8, 1, 4, 1, 1);


        horizontalLayout_InteractionPanel->addWidget(groupBox_DataView);

        groupBox_Record = new QGroupBox(DAFFCentralWidget);
        groupBox_Record->setObjectName(QStringLiteral("groupBox_Record"));
        formLayout = new QFormLayout(groupBox_Record);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_9 = new QLabel(groupBox_Record);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_9);

        spinBox_RecordIndex = new QSpinBox(groupBox_Record);
        spinBox_RecordIndex->setObjectName(QStringLiteral("spinBox_RecordIndex"));
        spinBox_RecordIndex->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_RecordIndex->setMaximum(64800);

        formLayout->setWidget(0, QFormLayout::FieldRole, spinBox_RecordIndex);


        horizontalLayout_InteractionPanel->addWidget(groupBox_Record);

        groupBox_Channel = new QGroupBox(DAFFCentralWidget);
        groupBox_Channel->setObjectName(QStringLiteral("groupBox_Channel"));
        formLayout_3 = new QFormLayout(groupBox_Channel);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        label_12 = new QLabel(groupBox_Channel);
        label_12->setObjectName(QStringLiteral("label_12"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_12);

        spinBox_ChannelIndex = new QSpinBox(groupBox_Channel);
        spinBox_ChannelIndex->setObjectName(QStringLiteral("spinBox_ChannelIndex"));
        spinBox_ChannelIndex->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_ChannelIndex->setMaximum(12);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, spinBox_ChannelIndex);


        horizontalLayout_InteractionPanel->addWidget(groupBox_Channel);

        groupBox_Frequency = new QGroupBox(DAFFCentralWidget);
        groupBox_Frequency->setObjectName(QStringLiteral("groupBox_Frequency"));
        formLayout_4 = new QFormLayout(groupBox_Frequency);
        formLayout_4->setSpacing(6);
        formLayout_4->setContentsMargins(11, 11, 11, 11);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        label_11 = new QLabel(groupBox_Frequency);
        label_11->setObjectName(QStringLiteral("label_11"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_11);

        spinBox_FrequencyIndex = new QSpinBox(groupBox_Frequency);
        spinBox_FrequencyIndex->setObjectName(QStringLiteral("spinBox_FrequencyIndex"));
        spinBox_FrequencyIndex->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, spinBox_FrequencyIndex);

        label_10 = new QLabel(groupBox_Frequency);
        label_10->setObjectName(QStringLiteral("label_10"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_10);

        comboBox_FrequencySelector = new QComboBox(groupBox_Frequency);
        comboBox_FrequencySelector->setObjectName(QStringLiteral("comboBox_FrequencySelector"));

        formLayout_4->setWidget(1, QFormLayout::FieldRole, comboBox_FrequencySelector);


        horizontalLayout_InteractionPanel->addWidget(groupBox_Frequency);


        layout_Visualization->addLayout(horizontalLayout_InteractionPanel);

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

        gridLayout->addWidget(DAFF2DPlot_Frame, 1, 0, 1, 1);


        layout_Visualization->addWidget(frame_2DPlot);

        layout_Visualization->setStretch(0, 1);
        layout_Visualization->setStretch(2, 1);

        horizontalLayout->addLayout(layout_Visualization);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 6);

        horizontalLayout_2->addLayout(horizontalLayout);

        DAFFViewer->setCentralWidget(DAFFCentralWidget);
        DAFFMenuBar = new QMenuBar(DAFFViewer);
        DAFFMenuBar->setObjectName(QStringLiteral("DAFFMenuBar"));
        DAFFMenuBar->setGeometry(QRect(0, 0, 1102, 21));
        menuFile = new QMenu(DAFFMenuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAbout = new QMenu(DAFFMenuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        menuContent = new QMenu(DAFFMenuBar);
        menuContent->setObjectName(QStringLiteral("menuContent"));
        menuObject_view = new QMenu(menuContent);
        menuObject_view->setObjectName(QStringLiteral("menuObject_view"));
        menuData_view = new QMenu(menuContent);
        menuData_view->setObjectName(QStringLiteral("menuData_view"));
        menuEdit = new QMenu(DAFFMenuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        DAFFViewer->setMenuBar(DAFFMenuBar);
        DAFFStatusBar = new QStatusBar(DAFFViewer);
        DAFFStatusBar->setObjectName(QStringLiteral("DAFFStatusBar"));
        DAFFViewer->setStatusBar(DAFFStatusBar);

        DAFFMenuBar->addAction(menuFile->menuAction());
        DAFFMenuBar->addAction(menuContent->menuAction());
        DAFFMenuBar->addAction(menuEdit->menuAction());
        DAFFMenuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionClose);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuAbout->addAction(actionAboutDAFFViewer);
        menuAbout->addAction(actionAboutOpenDAFF);
        menuAbout->addAction(actionOpenDAFFWebsite);
        menuContent->addSeparator();
        menuContent->addAction(actionIncrease_channel);
        menuContent->addAction(actionDecrease_channel);
        menuContent->addSeparator();
        menuContent->addAction(actionIncrease_Frequency);
        menuContent->addAction(actionDecrease_Frequency);
        menuContent->addSeparator();
        menuContent->addAction(menuObject_view->menuAction());
        menuContent->addAction(menuData_view->menuAction());
        menuObject_view->addAction(actionIncrease_phi);
        menuObject_view->addAction(actionDecrease_phi);
        menuObject_view->addSeparator();
        menuObject_view->addAction(actionIncrease_theta);
        menuObject_view->addAction(actionDecrease_theta);
        menuData_view->addAction(actionIncrease_alpha);
        menuData_view->addAction(actionDecrease_alpha);
        menuData_view->addSeparator();
        menuData_view->addAction(actionIncrease_beta);
        menuData_view->addAction(actionDecrease_beta);
        menuEdit->addAction(actionDownload);
        menuEdit->addAction(actionCreate);

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
        actionIncrease_channel->setText(QApplication::translate("DAFFViewer", "Increase channel", 0));
        actionIncrease_channel->setShortcut(QApplication::translate("DAFFViewer", "PgUp", 0));
        actionDecrease_channel->setText(QApplication::translate("DAFFViewer", "Decrease channel", 0));
        actionDecrease_channel->setShortcut(QApplication::translate("DAFFViewer", "PgDown", 0));
        actionIncrease_Frequency->setText(QApplication::translate("DAFFViewer", "Increase frequency", 0));
        actionIncrease_Frequency->setShortcut(QApplication::translate("DAFFViewer", "+", 0));
        actionDecrease_Frequency->setText(QApplication::translate("DAFFViewer", "Decrease frequency", 0));
        actionDecrease_Frequency->setShortcut(QApplication::translate("DAFFViewer", "-", 0));
        actionIncrease_phi->setText(QApplication::translate("DAFFViewer", "Increase phi", 0));
        actionIncrease_phi->setShortcut(QApplication::translate("DAFFViewer", "Right", 0));
        actionDecrease_phi->setText(QApplication::translate("DAFFViewer", "Decrease phi", 0));
        actionDecrease_phi->setShortcut(QApplication::translate("DAFFViewer", "Left", 0));
        actionIncrease_theta->setText(QApplication::translate("DAFFViewer", "Increase theta", 0));
        actionIncrease_theta->setShortcut(QApplication::translate("DAFFViewer", "Up", 0));
        actionDecrease_theta->setText(QApplication::translate("DAFFViewer", "Decrease theta", 0));
        actionDecrease_theta->setShortcut(QApplication::translate("DAFFViewer", "Down", 0));
        actionIncrease_alpha->setText(QApplication::translate("DAFFViewer", "Increase alpha", 0));
        actionDecrease_alpha->setText(QApplication::translate("DAFFViewer", "Decrease alpha", 0));
        actionIncrease_beta->setText(QApplication::translate("DAFFViewer", "Increase beta", 0));
        actionDecrease_beta->setText(QApplication::translate("DAFFViewer", "Decrease beta", 0));
        groupBox_Reader->setTitle(QApplication::translate("DAFFViewer", "DAFF Reader ", 0));
        labelFileName->setText(QApplication::translate("DAFFViewer", "File name", 0));
        labelVersion->setText(QApplication::translate("DAFFViewer", "Version", 0));
        labelContentType->setText(QApplication::translate("DAFFViewer", "Content type", 0));
        groupBox_Metadata->setTitle(QApplication::translate("DAFFViewer", "DAFF Metadata", 0));
        groupBox_Properties->setTitle(QApplication::translate("DAFFViewer", "DAFF Properties", 0));
        groupBox_ObjectView->setTitle(QApplication::translate("DAFFViewer", "Object view", 0));
        label_2->setText(QApplication::translate("DAFFViewer", "Theta", 0));
        label->setText(QApplication::translate("DAFFViewer", "Phi", 0));
        label_5->setText(QApplication::translate("DAFFViewer", "\302\260", 0));
        label_6->setText(QApplication::translate("DAFFViewer", "\302\260", 0));
        groupBox_DataView->setTitle(QApplication::translate("DAFFViewer", "Data view", 0));
        label_3->setText(QApplication::translate("DAFFViewer", "Alpha", 0));
        label_4->setText(QApplication::translate("DAFFViewer", "Beta", 0));
        label_7->setText(QApplication::translate("DAFFViewer", "\302\260", 0));
        label_8->setText(QApplication::translate("DAFFViewer", "\302\260", 0));
        groupBox_Record->setTitle(QApplication::translate("DAFFViewer", "Record", 0));
        label_9->setText(QApplication::translate("DAFFViewer", "Index", 0));
        groupBox_Channel->setTitle(QApplication::translate("DAFFViewer", "Channel", 0));
        label_12->setText(QApplication::translate("DAFFViewer", "Index", 0));
        groupBox_Frequency->setTitle(QApplication::translate("DAFFViewer", "Sample / Bin / Frequency", 0));
        label_11->setText(QApplication::translate("DAFFViewer", "Index", 0));
        label_10->setText(QApplication::translate("DAFFViewer", "Frequency", 0));
        menuFile->setTitle(QApplication::translate("DAFFViewer", "File", 0));
        menuAbout->setTitle(QApplication::translate("DAFFViewer", "About", 0));
        menuContent->setTitle(QApplication::translate("DAFFViewer", "Edit", 0));
        menuObject_view->setTitle(QApplication::translate("DAFFViewer", "Object view", 0));
        menuData_view->setTitle(QApplication::translate("DAFFViewer", "Data view", 0));
        menuEdit->setTitle(QApplication::translate("DAFFViewer", "Content", 0));
    } // retranslateUi

};

namespace Ui {
    class DAFFViewer: public Ui_DAFFViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDAFFVIEWERWINDOW_H
