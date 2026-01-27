#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QtCharts>
#include "serialmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDataUpdated(const VehicleData &data);
    void onConnectionStatusChanged(bool connected);
    void onErrorOccurred(const QString &error);
    void onConnectButtonClicked();
    void onRefreshPortsClicked();

private:
    Ui::MainWindow *ui;
    SerialManager *m_serialManager;

    // Chart components
    QChart *m_voltageChart;
    QLineSeries *m_voltageSeries;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    int m_dataPointCount;

    // Throttling để giảm tần suất update chart
    QElapsedTimer m_lastChartUpdate;
    const int MIN_CHART_UPDATE_MS = 50; // Cập nhật tối đa 20 lần/giây

    void setupUI();
    void setupChart();
    void updatePortList();
    void updateTirePressureDisplay(const VehicleData &data);
    void updateEngineTemperature(const VehicleData &data);
    void updateBatteryVoltage(const VehicleData &data);
};

#endif // MAINWINDOW_H
