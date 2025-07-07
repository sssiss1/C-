/********************************************************************************
** Form generated from reading UI file 'CustomGameDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMGAMEDIALOG_H
#define UI_CUSTOMGAMEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_CustomGameDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *rowsSpinBox;
    QLabel *label_2;
    QSpinBox *colsSpinBox;
    QLabel *label_3;
    QSpinBox *minesSpinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CustomGameDialog)
    {
        if (CustomGameDialog->objectName().isEmpty())
            CustomGameDialog->setObjectName("CustomGameDialog");
        CustomGameDialog->resize(250, 180);
        gridLayout = new QGridLayout(CustomGameDialog);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(CustomGameDialog);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        rowsSpinBox = new QSpinBox(CustomGameDialog);
        rowsSpinBox->setObjectName("rowsSpinBox");
        rowsSpinBox->setMinimum(9);
        rowsSpinBox->setMaximum(24);

        gridLayout->addWidget(rowsSpinBox, 0, 1, 1, 1);

        label_2 = new QLabel(CustomGameDialog);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        colsSpinBox = new QSpinBox(CustomGameDialog);
        colsSpinBox->setObjectName("colsSpinBox");
        colsSpinBox->setMinimum(9);
        colsSpinBox->setMaximum(30);

        gridLayout->addWidget(colsSpinBox, 1, 1, 1, 1);

        label_3 = new QLabel(CustomGameDialog);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        minesSpinBox = new QSpinBox(CustomGameDialog);
        minesSpinBox->setObjectName("minesSpinBox");
        minesSpinBox->setMinimum(10);
        minesSpinBox->setMaximum(668);

        gridLayout->addWidget(minesSpinBox, 2, 1, 1, 1);

        buttonBox = new QDialogButtonBox(CustomGameDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 2);


        retranslateUi(CustomGameDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, CustomGameDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, CustomGameDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(CustomGameDialog);
    } // setupUi

    void retranslateUi(QDialog *CustomGameDialog)
    {
        CustomGameDialog->setWindowTitle(QCoreApplication::translate("CustomGameDialog", "\350\207\252\345\256\232\344\271\211\346\270\270\346\210\217", nullptr));
        label->setText(QCoreApplication::translate("CustomGameDialog", "\351\253\230\345\272\246 (9-24):", nullptr));
        label_2->setText(QCoreApplication::translate("CustomGameDialog", "\345\256\275\345\272\246 (9-30):", nullptr));
        label_3->setText(QCoreApplication::translate("CustomGameDialog", "\351\233\267\346\225\260 (10-668):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomGameDialog: public Ui_CustomGameDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMGAMEDIALOG_H
