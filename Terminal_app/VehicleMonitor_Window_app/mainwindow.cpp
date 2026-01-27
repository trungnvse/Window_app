#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serialManager(new SerialManager(this))
    , m_dataPointCount(0)
{
    ui->setupUi(this);
    setupUI();
    setupChart();

    // Khởi tạo timer cho throttling
    m_lastChartUpdate.start();

    // Kết nối signals
    connect(m_serialManager, &SerialManager::dataUpdated,
            this, &MainWindow::onDataUpdated);
    connect(m_serialManager, &SerialManager::connectionStatusChanged,
            this, &MainWindow::onConnectionStatusChanged);
    connect(m_serialManager, &SerialManager::errorOccurred,
            this, &MainWindow::onErrorOccurred);

    connect(ui->btnConnect, &QPushButton::clicked,
            this, &MainWindow::onConnectButtonClicked);
    connect(ui->btnRefresh, &QPushButton::clicked,
            this, &MainWindow::onRefreshPortsClicked);

    // Load danh sách cổng COM
    updatePortList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle("Vehicle Monitor - S32K144 UART Interface");

    // Thiết lập ComboBox baudrate
    ui->cbBaudRate->addItems({"9600", "19200", "38400", "57600", "115200"});
    ui->cbBaudRate->setCurrentText("9600");

    // Thiết lập style cho các label cảnh báo
    ui->lblEngineTempWarning->setStyleSheet("color: red; font-weight: bold;");
    ui->lblEngineTempWarning->setVisible(false);
}

void MainWindow::setupChart()
{
    // Tạo chart cho điện áp pin
    m_voltageChart = new QChart();
    m_voltageChart->setTitle("Battery Voltage Monitor");
    m_voltageChart->setAnimationOptions(QChart::NoAnimation); // TẮT animation
    m_voltageChart->legend()->setVisible(false); // Ẩn legend để gọn hơn

    m_voltageSeries = new QLineSeries();
    m_voltageSeries->setName("Voltage (V)");

    // Tối ưu rendering
    m_voltageSeries->setUseOpenGL(true); // Dùng OpenGL để vẽ mượt hơn

    m_voltageChart->addSeries(m_voltageSeries);

    // Thiết lập trục
    m_axisX = new QValueAxis();
    m_axisX->setTitleText("Time (s)");
    m_axisX->setRange(0, 60);
    m_axisX->setLabelFormat("%d");
    m_axisX->setTickCount(7); // Giảm số tick để gọn hơn
    m_voltageChart->addAxis(m_axisX, Qt::AlignBottom);
    m_voltageSeries->attachAxis(m_axisX);

    m_axisY = new QValueAxis();
    m_axisY->setTitleText("Voltage (V)");
    m_axisY->setRange(9, 16);
    m_axisY->setLabelFormat("%.1f");
    m_axisY->setTickCount(8);
    m_voltageChart->addAxis(m_axisY, Qt::AlignLeft);
    m_voltageSeries->attachAxis(m_axisY);

    // Thêm chart vào chartView
    ui->chartView->setChart(m_voltageChart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    // TỐI ƯU HÓA: Giảm quality khi update nhanh
    ui->chartView->setRenderHint(QPainter::SmoothPixmapTransform, false);
}

void MainWindow::updatePortList()
{
    ui->cbComPort->clear();
    QStringList ports = m_serialManager->getAvailablePorts();

    if (ports.isEmpty()) {
        ui->cbComPort->addItem("No ports available");
    } else {
        ui->cbComPort->addItems(ports);
    }
}

void MainWindow::onConnectButtonClicked()
{
    if (m_serialManager->isConnected()) {
        m_serialManager->disconnect();
    } else {
        QString portName = ui->cbComPort->currentText();
        int baudRate = ui->cbBaudRate->currentText().toInt();

        if (portName == "No ports available") {
            QMessageBox::warning(this, "Error", "Không có cổng COM khả dụng!");
            return;
        }

        m_serialManager->connectToPort(portName, baudRate);
    }
}

void MainWindow::onRefreshPortsClicked()
{
    updatePortList();
}

void MainWindow::onConnectionStatusChanged(bool connected)
{
    if (connected) {
        ui->btnConnect->setText("Disconnect");
        ui->lblStatus->setText("✓ Connected");
        ui->lblStatus->setStyleSheet("color: green; font-weight: bold;");
        ui->cbComPort->setEnabled(false);
        ui->cbBaudRate->setEnabled(false);
    } else {
        ui->btnConnect->setText("Connect");
        ui->lblStatus->setText("✗ Disconnected");
        ui->lblStatus->setStyleSheet("color: red;");
        ui->cbComPort->setEnabled(true);
        ui->cbBaudRate->setEnabled(true);
    }
}

void MainWindow::onErrorOccurred(const QString &error)
{
    QMessageBox::critical(this, "Serial Port Error", error);
}

void MainWindow::onDataUpdated(const VehicleData &data)
{
    updateTirePressureDisplay(data);
    updateEngineTemperature(data);
    updateBatteryVoltage(data);
}

void MainWindow::updateTirePressureDisplay(const VehicleData &data)
{
    // Cập nhật LCD hoặc Label cho từng lốp
    ui->lcdTireFL->display(QString::number(data.tirePressureFL, 'f', 1));
    ui->lcdTireFR->display(QString::number(data.tirePressureFR, 'f', 1));
    ui->lcdTireRL->display(QString::number(data.tirePressureRL, 'f', 1));
    ui->lcdTireRR->display(QString::number(data.tirePressureRR, 'f', 1));

    // Cảnh báo nếu áp suất thấp (< 28 PSI)
    bool anyLowPressure = (data.tirePressureFL < 28.0f ||
                           data.tirePressureFR < 28.0f ||
                           data.tirePressureRL < 28.0f ||
                           data.tirePressureRR < 28.0f);

    if (anyLowPressure) {
        ui->lblTireWarning->setText("⚠ Low Tire Pressure!");
        ui->lblTireWarning->setStyleSheet("color: red; font-weight: bold;");
    } else {
        ui->lblTireWarning->setText("✓ Tire Pressure OK");
        ui->lblTireWarning->setStyleSheet("color: green;");
    }
}

void MainWindow::updateEngineTemperature(const VehicleData &data)
{
    ui->progressEngineTemp->setValue(static_cast<int>(data.engineTemp));
    ui->lblEngineTempValue->setText(QString::number(data.engineTemp, 'f', 1) + " °C");

    // Cảnh báo nếu nhiệt độ cao (> 105°C)
    if (data.engineTemp > 105.0f) {
        ui->lblEngineTempWarning->setText("⚠ ENGINE OVERHEAT!");
        ui->lblEngineTempWarning->setVisible(true);
        ui->progressEngineTemp->setStyleSheet("QProgressBar::chunk { background-color: red; }");
    } else if (data.engineTemp > 100.0f) {
        ui->lblEngineTempWarning->setText("⚠ High Temperature");
        ui->lblEngineTempWarning->setVisible(true);
        ui->progressEngineTemp->setStyleSheet("QProgressBar::chunk { background-color: orange; }");
    } else {
        ui->lblEngineTempWarning->setVisible(false);
        ui->progressEngineTemp->setStyleSheet("QProgressBar::chunk { background-color: #05B8CC; }");
    }
}

void MainWindow::updateBatteryVoltage(const VehicleData &data)
{
    // Luôn cập nhật label ngay lập tức
    ui->lblBatteryVoltage->setText(QString::number(data.batteryVoltage, 'f', 2) + " V");

    // Throttling: Chỉ cập nhật chart nếu đã qua 50ms từ lần update trước
    if (m_lastChartUpdate.elapsed() < MIN_CHART_UPDATE_MS) {
        return; // Bỏ qua lần update này
    }
    m_lastChartUpdate.restart();

    // Cập nhật chart - KHÔNG TẠO LẠI, CHỈ THÊM ĐIỂM MỚI
    m_voltageSeries->append(m_dataPointCount, data.batteryVoltage);
    m_dataPointCount++;

    // Tự động điều chỉnh trục Y để luôn thấy rõ dữ liệu
    static float minVoltage = data.batteryVoltage;
    static float maxVoltage = data.batteryVoltage;

    minVoltage = qMin(minVoltage, data.batteryVoltage);
    maxVoltage = qMax(maxVoltage, data.batteryVoltage);

    // Thêm padding 0.5V ở 2 đầu
    m_axisY->setRange(qMax(9.0f, minVoltage - 0.5f), qMin(16.0f, maxVoltage + 0.5f));

    // Giới hạn số điểm hiển thị (60 điểm gần nhất)
    if (m_voltageSeries->count() > 60) {
        m_voltageSeries->removePoints(0, m_voltageSeries->count() - 60);
        m_axisX->setRange(m_dataPointCount - 60, m_dataPointCount);

        // Reset min/max sau khi xóa điểm cũ
        minVoltage = data.batteryVoltage;
        maxVoltage = data.batteryVoltage;
        for (int i = 0; i < m_voltageSeries->count(); i++) {
            float val = m_voltageSeries->at(i).y();
            minVoltage = qMin(minVoltage, val);
            maxVoltage = qMax(maxVoltage, val);
        }
    }

    // Cảnh báo điện áp thấp (< 11.5V)
    if (data.batteryVoltage < 11.5f) {
        ui->lblBatteryStatus->setText("⚠ Low Battery!");
        ui->lblBatteryStatus->setStyleSheet("color: red; font-weight: bold;");
    } else if (data.batteryVoltage < 12.0f) {
        ui->lblBatteryStatus->setText("⚠ Battery Weak");
        ui->lblBatteryStatus->setStyleSheet("color: orange; font-weight: bold;");
    } else {
        ui->lblBatteryStatus->setText("✓ Battery OK");
        ui->lblBatteryStatus->setStyleSheet("color: green;");
    }
}
