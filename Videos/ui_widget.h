/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label_title;
    QPushButton *button_load;
    QLabel *label_2;
    QLabel *label_3;
    QSlider *horizontalSlider;
    QPushButton *button_play_stop;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_tipo_defecto;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(563, 504);
        label_title = new QLabel(Widget);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setGeometry(QRect(170, 10, 181, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        label_title->setFont(font);
        button_load = new QPushButton(Widget);
        button_load->setObjectName(QString::fromUtf8("button_load"));
        button_load->setGeometry(QRect(40, 460, 61, 26));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(210, 460, 41, 32));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(480, 460, 41, 32));
        horizontalSlider = new QSlider(Widget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(210, 450, 301, 16));
        horizontalSlider->setOrientation(Qt::Horizontal);
        button_play_stop = new QPushButton(Widget);
        button_play_stop->setObjectName(QString::fromUtf8("button_play_stop"));
        button_play_stop->setGeometry(QRect(100, 460, 61, 26));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(33, 50, 491, 361));
        label->setStyleSheet(QString::fromUtf8("background-color: #000\n"
""));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 420, 111, 17));
        label_tipo_defecto = new QLabel(Widget);
        label_tipo_defecto->setObjectName(QString::fromUtf8("label_tipo_defecto"));
        label_tipo_defecto->setGeometry(QRect(160, 420, 171, 17));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        label_title->setText(QApplication::translate("Widget", "Video Defectos", 0, QApplication::UnicodeUTF8));
        button_load->setText(QApplication::translate("Widget", "Cargar", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Widget", "0:00", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Widget", "0:00", 0, QApplication::UnicodeUTF8));
        button_play_stop->setText(QApplication::translate("Widget", "Play", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_4->setText(QApplication::translate("Widget", "<html><head/><body><p><span style=\" font-weight:600;\">Tipo de defecto:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_tipo_defecto->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
