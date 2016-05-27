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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DAFFViewer
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QWidget *DAFFCentralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QFrame *line;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer;
    QFrame *line_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QMdiArea *mdiArea;
    QMdiArea *mdiArea_2;
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
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(DAFFCentralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(310, 16777215));
        groupBox->setStyleSheet(QStringLiteral(""));
        formLayout_2 = new QFormLayout(groupBox);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setHorizontalSpacing(12);
        formLayout_2->setVerticalSpacing(12);
        formLayout_2->setContentsMargins(20, 20, 20, 20);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral(""));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setEnabled(false);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEdit);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_2);

        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setEnabled(false);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEdit_2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_3);

        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setEnabled(false);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, lineEdit_3);


        verticalLayout->addWidget(groupBox);

        line = new QFrame(DAFFCentralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        pushButton = new QPushButton(DAFFCentralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(DAFFCentralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        line_2 = new QFrame(DAFFCentralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setLineWidth(3);
        line_2->setMidLineWidth(0);
        line_2->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(line_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButton_3 = new QPushButton(DAFFCentralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_3->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(DAFFCentralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_3->addWidget(pushButton_4);


        verticalLayout_3->addLayout(horizontalLayout_3);

        mdiArea = new QMdiArea(DAFFCentralWidget);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));

        verticalLayout_3->addWidget(mdiArea);

        mdiArea_2 = new QMdiArea(DAFFCentralWidget);
        mdiArea_2->setObjectName(QStringLiteral("mdiArea_2"));

        verticalLayout_3->addWidget(mdiArea_2);


        horizontalLayout->addLayout(verticalLayout_3);


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
        actionOpen->setShortcut(QApplication::translate("DAFFViewer", "Ctrl+O", 0));
        actionQuit->setText(QApplication::translate("DAFFViewer", "Quit", 0));
        groupBox->setTitle(QApplication::translate("DAFFViewer", "DAFF Reader ", 0));
        label->setText(QApplication::translate("DAFFViewer", "File name", 0));
        label_2->setText(QApplication::translate("DAFFViewer", "Version", 0));
        label_3->setText(QApplication::translate("DAFFViewer", "Content type", 0));
        pushButton->setText(QApplication::translate("DAFFViewer", "Metadata", 0));
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
