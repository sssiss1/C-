/********************************************************************************
** Form generated from reading UI file 'HighScoreDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HIGHSCOREDIALOG_H
#define UI_HIGHSCOREDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HighScoreDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTableWidget *highScoreTable;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *clearButton;

    void setupUi(QDialog *HighScoreDialog)
    {
        if (HighScoreDialog->objectName().isEmpty())
            HighScoreDialog->setObjectName("HighScoreDialog");
        HighScoreDialog->resize(500, 400);
        verticalLayout = new QVBoxLayout(HighScoreDialog);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(HighScoreDialog);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        highScoreTable = new QTableWidget(HighScoreDialog);
        if (highScoreTable->columnCount() < 3)
            highScoreTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        highScoreTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        highScoreTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        highScoreTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        highScoreTable->setObjectName("highScoreTable");
        highScoreTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        highScoreTable->setSelectionMode(QAbstractItemView::NoSelection);
        highScoreTable->horizontalHeader()->setStretchLastSection(true);
        highScoreTable->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(highScoreTable);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        clearButton = new QPushButton(HighScoreDialog);
        clearButton->setObjectName("clearButton");

        horizontalLayout->addWidget(clearButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(HighScoreDialog);

        QMetaObject::connectSlotsByName(HighScoreDialog);
    } // setupUi

    void retranslateUi(QDialog *HighScoreDialog)
    {
        HighScoreDialog->setWindowTitle(QCoreApplication::translate("HighScoreDialog", "\346\211\253\351\233\267\346\216\222\350\241\214\346\246\234", nullptr));
        label->setText(QCoreApplication::translate("HighScoreDialog", "\346\234\200\344\275\263\346\227\266\351\227\264\346\216\222\350\241\214\346\246\234", nullptr));
        QTableWidgetItem *___qtablewidgetitem = highScoreTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("HighScoreDialog", "\346\250\241\345\274\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = highScoreTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("HighScoreDialog", "\351\232\276\345\272\246 (\350\241\214x\345\210\227, \351\233\267\346\225\260)", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = highScoreTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("HighScoreDialog", "\346\234\200\344\275\263\346\227\266\351\227\264", nullptr));
        clearButton->setText(QCoreApplication::translate("HighScoreDialog", "\346\270\205\347\251\272\346\216\222\350\241\214\346\246\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HighScoreDialog: public Ui_HighScoreDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HIGHSCOREDIALOG_H
