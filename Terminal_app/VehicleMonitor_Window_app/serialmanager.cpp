#include "serialmanager.h"
#include <QDebug>
#include <QRandomGenerator>

SerialManager::SerialManager(QObject *parent)
    : QObject(parent)
    , m_serialPort(new QSerialPort(this))
    , m_simulationTimer(new QTimer(this))
{
    // Kết nối signals
    connect(m_serialPort, &QSerialPort::readyRead,
            this, &SerialManager::readData);
    connect(m_serialPort, &QSerialPort::errorOccurred,
            this, &SerialManager::handleError);

    // Timer để tạo dữ liệu giả lập (tire pressure & engine temp)
    connect(m_simulationTimer, &QTimer::timeout,
            this, &SerialManager::generateSimulatedData);
    m_simulationTimer->start(200); // Cập nhật mỗi 200ms (nhanh hơn)
}

SerialManager::~SerialManager()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

QStringList SerialManager::getAvailablePorts()
{
    QStringList portList;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        portList << info.portName();
    }
    return portList;
}

bool SerialManager::connectToPort(const QString &portName, qint32 baudRate)
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Connected to" << portName << "at" << baudRate << "baud";
        emit connectionStatusChanged(true);
        return true;
    } else {
        qDebug() << "Failed to open port:" << m_serialPort->errorString();
        emit errorOccurred("Không thể kết nối: " + m_serialPort->errorString());
        return false;
    }
}

void SerialManager::disconnect()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        emit connectionStatusChanged(false);
        qDebug() << "Disconnected from serial port";
    }
}

bool SerialManager::isConnected() const
{
    return m_serialPort->isOpen();
}

void SerialManager::readData()
{
    QByteArray data = m_serialPort->readAll();
    m_receiveBuffer.append(data);

    // Tìm ký tự kết thúc (\n hoặc \r\n)
    int newlineIndex = m_receiveBuffer.indexOf('\n');
    if (newlineIndex != -1) {
        QByteArray completeMessage = m_receiveBuffer.left(newlineIndex);
        m_receiveBuffer.remove(0, newlineIndex + 1);

        parseReceivedData(completeMessage);
    }
}

void SerialManager::parseReceivedData(const QByteArray &data)
{
    // Format từ S32K144: chỉ là số điện áp, ví dụ: "4.9987\r\n" hoặc "0.0000\r\n"

    QString dataStr = QString::fromLatin1(data).trimmed();

    // Loại bỏ các ký tự không mong muốn
    dataStr = dataStr.remove('\r').remove('\n').trimmed();

    if (dataStr.isEmpty()) {
        return;
    }

    // Chuyển đổi string sang float
    bool ok;
    float voltageFromADC = dataStr.toFloat(&ok);

    if (ok) {
        // S32K144 gửi điện áp trực tiếp (0-5V từ ADC)
        // Chuyển đổi sang điện áp pin thực tế
        // Giả sử: 0V ADC = 10V pin, 5V ADC = 15V pin
        m_currentData.batteryVoltage = 10.0f + (voltageFromADC / 5.0f) * 5.0f;

        // Hoặc nếu bạn muốn hiển thị giá trị ADC trực tiếp:
        // m_currentData.batteryVoltage = voltageFromADC;

        // BỎ debug log để tăng hiệu suất
        // qDebug() << "ADC Voltage:" << voltageFromADC << "V -> Battery:"
        //          << m_currentData.batteryVoltage << "V";

        emit dataUpdated(m_currentData);
    }
}

float SerialManager::adcToBatteryVoltage(uint16_t adcValue)
{
    // Hàm này không còn dùng nữa vì S32K144 đã gửi điện áp rồi
    // Nhưng giữ lại để tương thích
    float voltage = 10.0f + (adcValue / 4095.0f) * 5.0f;
    return voltage;
}

void SerialManager::generateSimulatedData()
{
    // Giả lập áp suất lốp (28-35 PSI) với biến động nhỏ
    auto *rng = QRandomGenerator::global();

    m_currentData.tirePressureFL = 30.0f + (rng->bounded(100) / 100.0f) * 5.0f;
    m_currentData.tirePressureFR = 30.0f + (rng->bounded(100) / 100.0f) * 5.0f;
    m_currentData.tirePressureRL = 29.0f + (rng->bounded(100) / 100.0f) * 5.0f;
    m_currentData.tirePressureRR = 29.0f + (rng->bounded(100) / 100.0f) * 5.0f;

    // Giả lập nhiệt độ động cơ (70-110°C) với xu hướng tăng dần
    static float engineTempTrend = 90.0f;
    engineTempTrend += (rng->bounded(200) - 100) / 100.0f; // ±1°C

    // Giới hạn trong khoảng hợp lý
    if (engineTempTrend < 70.0f) engineTempTrend = 70.0f;
    if (engineTempTrend > 110.0f) engineTempTrend = 110.0f;

    m_currentData.engineTemp = engineTempTrend;

    emit dataUpdated(m_currentData);
}

void SerialManager::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError && error != QSerialPort::TimeoutError) {
        QString errorMsg = m_serialPort->errorString();
        qDebug() << "Serial port error:" << errorMsg;
        emit errorOccurred(errorMsg);
    }
}
