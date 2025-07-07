/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *campaignStatusLabel;
    QVBoxLayout *topPanelLayout;
    QHBoxLayout *topRowLayout;
    QLabel *statusLabel;
    QSpacerItem *topRowSpacer;
    QLabel *timeLabel;
    QHBoxLayout *bottomRowLayout;
    QLabel *flagsLabel;
    QSpacerItem *bottomRowSpacer;
    QPushButton *newGameButton;
    QPushButton *homeButton;
    QWidget *grids;
    QGridLayout *gridLayout;
    QHBoxLayout *bottomPanelLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *hintButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(430, 380);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        campaignStatusLabel = new QLabel(centralwidget);
        campaignStatusLabel->setObjectName("campaignStatusLabel");
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        campaignStatusLabel->setFont(font);
        campaignStatusLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(campaignStatusLabel);

        topPanelLayout = new QVBoxLayout();
        topPanelLayout->setObjectName("topPanelLayout");
        topRowLayout = new QHBoxLayout();
        topRowLayout->setObjectName("topRowLayout");
        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");

        topRowLayout->addWidget(statusLabel);

        topRowSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topRowLayout->addItem(topRowSpacer);

        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName("timeLabel");

        topRowLayout->addWidget(timeLabel);


        topPanelLayout->addLayout(topRowLayout);

        bottomRowLayout = new QHBoxLayout();
        bottomRowLayout->setObjectName("bottomRowLayout");
        flagsLabel = new QLabel(centralwidget);
        flagsLabel->setObjectName("flagsLabel");

        bottomRowLayout->addWidget(flagsLabel);

        bottomRowSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomRowLayout->addItem(bottomRowSpacer);

        newGameButton = new QPushButton(centralwidget);
        newGameButton->setObjectName("newGameButton");

        bottomRowLayout->addWidget(newGameButton);

        homeButton = new QPushButton(centralwidget);
        homeButton->setObjectName("homeButton");

        bottomRowLayout->addWidget(homeButton);


        topPanelLayout->addLayout(bottomRowLayout);


        verticalLayout->addLayout(topPanelLayout);

        grids = new QWidget(centralwidget);
        grids->setObjectName("grids");
        gridLayout = new QGridLayout(grids);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(grids);

        bottomPanelLayout = new QHBoxLayout();
        bottomPanelLayout->setObjectName("bottomPanelLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomPanelLayout->addItem(horizontalSpacer);

        hintButton = new QPushButton(centralwidget);
        hintButton->setObjectName("hintButton");

        bottomPanelLayout->addWidget(hintButton);


        verticalLayout->addLayout(bottomPanelLayout);

        verticalLayout->setStretch(2, 1);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\211\253\351\233\267", nullptr));
        campaignStatusLabel->setText(QString());
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\345\207\206\345\244\207\345\274\200\345\247\213", nullptr));
        timeLabel->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264: 0", nullptr));
        flagsLabel->setText(QCoreApplication::translate("MainWindow", "\346\227\227\345\270\234: 0", nullptr));
        newGameButton->setText(QCoreApplication::translate("MainWindow", "\351\207\215\346\226\260\345\274\200\345\247\213", nullptr));
        homeButton->setText(QCoreApplication::translate("MainWindow", "\345\233\236\345\210\260\344\270\273\351\241\265", nullptr));
        hintButton->setText(QCoreApplication::translate("MainWindow", "\346\217\220\347\244\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
