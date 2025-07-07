/********************************************************************************
** Form generated from reading UI file 'StartWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTWINDOW_H
#define UI_STARTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QSpacerItem *verticalSpacer;
    QPushButton *simpleButton;
    QPushButton *normalButton;
    QPushButton *hardButton;
    QPushButton *campaignButton;
    QPushButton *customButton;
    QPushButton *highScoreButton;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *StartWindow)
    {
        if (StartWindow->objectName().isEmpty())
            StartWindow->setObjectName("StartWindow");
        StartWindow->resize(300, 450);
        verticalLayout = new QVBoxLayout(StartWindow);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(StartWindow);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(28);
        font.setBold(true);
        titleLabel->setFont(font);
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        simpleButton = new QPushButton(StartWindow);
        simpleButton->setObjectName("simpleButton");
        simpleButton->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(simpleButton);

        normalButton = new QPushButton(StartWindow);
        normalButton->setObjectName("normalButton");
        normalButton->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(normalButton);

        hardButton = new QPushButton(StartWindow);
        hardButton->setObjectName("hardButton");
        hardButton->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(hardButton);

        campaignButton = new QPushButton(StartWindow);
        campaignButton->setObjectName("campaignButton");
        campaignButton->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(campaignButton);

        customButton = new QPushButton(StartWindow);
        customButton->setObjectName("customButton");
        customButton->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(customButton);

        highScoreButton = new QPushButton(StartWindow);
        highScoreButton->setObjectName("highScoreButton");
        highScoreButton->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(highScoreButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(StartWindow);

        QMetaObject::connectSlotsByName(StartWindow);
    } // setupUi

    void retranslateUi(QWidget *StartWindow)
    {
        StartWindow->setWindowTitle(QCoreApplication::translate("StartWindow", "\346\211\253\351\233\267 - \351\200\211\346\213\251\346\250\241\345\274\217", nullptr));
        titleLabel->setText(QCoreApplication::translate("StartWindow", "\346\211\253\351\233\267", nullptr));
        simpleButton->setText(QCoreApplication::translate("StartWindow", "\347\256\200\345\215\225\346\250\241\345\274\217 (9x9, 10\351\233\267)", nullptr));
        normalButton->setText(QCoreApplication::translate("StartWindow", "\346\231\256\351\200\232\346\250\241\345\274\217 (16x16, 40\351\233\267)", nullptr));
        hardButton->setText(QCoreApplication::translate("StartWindow", "\345\233\260\351\232\276\346\250\241\345\274\217 (16x30, 99\351\233\267)", nullptr));
        campaignButton->setText(QCoreApplication::translate("StartWindow", "\351\227\257\345\205\263\346\250\241\345\274\217", nullptr));
        customButton->setText(QCoreApplication::translate("StartWindow", "\350\207\252\345\256\232\344\271\211\346\250\241\345\274\217", nullptr));
        highScoreButton->setText(QCoreApplication::translate("StartWindow", "\346\216\222\350\241\214\346\246\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StartWindow: public Ui_StartWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTWINDOW_H
