/********************************************************************************
** Form generated from reading UI file 'second.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECOND_H
#define UI_SECOND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_second
{
public:
    QPushButton *pushButton;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLabel *label_7;

    void setupUi(QWidget *second)
    {
        if (second->objectName().isEmpty())
            second->setObjectName(QString::fromUtf8("second"));
        second->setEnabled(true);
        second->resize(800, 480);
        second->setContextMenuPolicy(Qt::DefaultContextMenu);
        second->setStyleSheet(QString::fromUtf8(""));
        pushButton = new QPushButton(second);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(700, 450, 93, 28));
        layoutWidget = new QWidget(second);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(70, 50, 351, 191));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);


        horizontalLayout_2->addLayout(horizontalLayout);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);


        horizontalLayout_3->addLayout(horizontalLayout_4);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_3->addWidget(label_6);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);


        horizontalLayout_5->addLayout(horizontalLayout_6);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_5->addWidget(label_7);


        verticalLayout->addLayout(horizontalLayout_5);


        retranslateUi(second);

        QMetaObject::connectSlotsByName(second);
    } // setupUi

    void retranslateUi(QWidget *second)
    {
        second->setWindowTitle(QApplication::translate("second", "Form", nullptr));
        pushButton->setText(QApplication::translate("second", "\350\277\224\345\233\236", nullptr));
        label->setText(QApplication::translate("second", "\347\216\257\345\242\203\345\205\211\345\274\272\345\272\246", nullptr));
        label_3->setText(QString());
        label_4->setText(QApplication::translate("second", "\346\216\245\350\277\221\350\267\235\347\246\273", nullptr));
        label_6->setText(QString());
        label_5->setText(QApplication::translate("second", "\347\272\242\345\244\226\345\274\272\345\272\246", nullptr));
        label_7->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class second: public Ui_second {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECOND_H
