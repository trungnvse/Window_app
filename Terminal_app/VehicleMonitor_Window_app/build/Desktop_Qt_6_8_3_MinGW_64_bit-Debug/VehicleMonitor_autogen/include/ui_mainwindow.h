/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *cbComPort;
    QPushButton *btnRefresh;
    QLabel *label_2;
    QComboBox *cbBaudRate;
    QPushButton *btnConnect;
    QLabel *lblStatus;
    QSpacerItem *horizontalSpacer;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLCDNumber *lcdTireFL;
    QLabel *label_4;
    QLCDNumber *lcdTireFR;
    QLabel *label_5;
    QLCDNumber *lcdTireRL;
    QLabel *label_6;
    QLCDNumber *lcdTireRR;
    QLabel *lblTireWarning;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QProgressBar *progressEngineTemp;
    QLabel *lblEngineTempValue;
    QLabel *lblEngineTempWarning;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *lblBatteryVoltage;
    QLabel *lblBatteryStatus;
    QChartView *chartView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        cbComPort = new QComboBox(groupBox);
        cbComPort->setObjectName("cbComPort");
        cbComPort->setMinimumWidth(120);

        horizontalLayout->addWidget(cbComPort);

        btnRefresh = new QPushButton(groupBox);
        btnRefresh->setObjectName("btnRefresh");
        btnRefresh->setMaximumWidth(80);

        horizontalLayout->addWidget(btnRefresh);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        cbBaudRate = new QComboBox(groupBox);
        cbBaudRate->setObjectName("cbBaudRate");
        cbBaudRate->setMinimumWidth(100);

        horizontalLayout->addWidget(cbBaudRate);

        btnConnect = new QPushButton(groupBox);
        btnConnect->setObjectName("btnConnect");
        btnConnect->setMinimumWidth(100);

        horizontalLayout->addWidget(btnConnect);

        lblStatus = new QLabel(groupBox);
        lblStatus->setObjectName("lblStatus");

        horizontalLayout->addWidget(lblStatus);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(groupBox);

        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setMinimumWidth(300);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        lcdTireFL = new QLCDNumber(groupBox_2);
        lcdTireFL->setObjectName("lcdTireFL");
        lcdTireFL->setDigitCount(4);
        lcdTireFL->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(lcdTireFL, 0, 1, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        lcdTireFR = new QLCDNumber(groupBox_2);
        lcdTireFR->setObjectName("lcdTireFR");
        lcdTireFR->setDigitCount(4);
        lcdTireFR->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(lcdTireFR, 1, 1, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        lcdTireRL = new QLCDNumber(groupBox_2);
        lcdTireRL->setObjectName("lcdTireRL");
        lcdTireRL->setDigitCount(4);
        lcdTireRL->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(lcdTireRL, 2, 1, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 3, 0, 1, 1);

        lcdTireRR = new QLCDNumber(groupBox_2);
        lcdTireRR->setObjectName("lcdTireRR");
        lcdTireRR->setDigitCount(4);
        lcdTireRR->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(lcdTireRR, 3, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        lblTireWarning = new QLabel(groupBox_2);
        lblTireWarning->setObjectName("lblTireWarning");
        lblTireWarning->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lblTireWarning);

        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName("groupBox_4");
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setObjectName("verticalLayout_4");
        progressEngineTemp = new QProgressBar(groupBox_4);
        progressEngineTemp->setObjectName("progressEngineTemp");
        progressEngineTemp->setMinimum(0);
        progressEngineTemp->setMaximum(120);
        progressEngineTemp->setValue(90);
        progressEngineTemp->setTextVisible(true);

        verticalLayout_4->addWidget(progressEngineTemp);

        lblEngineTempValue = new QLabel(groupBox_4);
        lblEngineTempValue->setObjectName("lblEngineTempValue");
        lblEngineTempValue->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(lblEngineTempValue);

        lblEngineTempWarning = new QLabel(groupBox_4);
        lblEngineTempWarning->setObjectName("lblEngineTempWarning");
        lblEngineTempWarning->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(lblEngineTempWarning);


        verticalLayout_2->addWidget(groupBox_4);


        horizontalLayout_2->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(widget);
        groupBox_3->setObjectName("groupBox_3");
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        lblBatteryVoltage = new QLabel(groupBox_3);
        lblBatteryVoltage->setObjectName("lblBatteryVoltage");
        lblBatteryVoltage->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lblBatteryVoltage);

        lblBatteryStatus = new QLabel(groupBox_3);
        lblBatteryStatus->setObjectName("lblBatteryStatus");
        lblBatteryStatus->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lblBatteryStatus);

        chartView = new QChartView(groupBox_3);
        chartView->setObjectName("chartView");
        chartView->setMinimumHeight(300);

        verticalLayout_3->addWidget(chartView);


        horizontalLayout_2->addWidget(groupBox_3);


        verticalLayout->addWidget(widget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Vehicle Monitor", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "UART Connection", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "COM Port:", nullptr));
        btnRefresh->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Baud Rate:", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        lblStatus->setText(QCoreApplication::translate("MainWindow", "\342\234\227 Disconnected", nullptr));
        lblStatus->setStyleSheet(QCoreApplication::translate("MainWindow", "color: red;", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Tire Pressure (PSI)", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Front Left:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Front Right:", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Rear Left:", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Rear Right:", nullptr));
        lblTireWarning->setText(QCoreApplication::translate("MainWindow", "Status: OK", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Engine Temperature", nullptr));
        progressEngineTemp->setFormat(QCoreApplication::translate("MainWindow", "%v\302\260C", nullptr));
        lblEngineTempValue->setText(QCoreApplication::translate("MainWindow", "90.0 \302\260C", nullptr));
        lblEngineTempValue->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18pt; font-weight: bold;", nullptr));
        lblEngineTempWarning->setText(QCoreApplication::translate("MainWindow", "\342\232\240 WARNING", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Battery Voltage Monitor", nullptr));
        lblBatteryVoltage->setText(QCoreApplication::translate("MainWindow", "12.6 V", nullptr));
        lblBatteryVoltage->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24pt; font-weight: bold; color: #05B8CC;", nullptr));
        lblBatteryStatus->setText(QCoreApplication::translate("MainWindow", "Status: OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
