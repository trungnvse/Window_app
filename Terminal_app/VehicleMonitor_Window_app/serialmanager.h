#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

struct VehicleData {
    float tirePressureFL;  // Front Left (PSI)
    float tirePressureFR;  // Front Right (PSI)
    float tirePressureRL;  // Rear Left (PSI)
    float tirePressureRR;  // Rear Right (PSI)
    float engineTemp;      // Celsius
    float batteryVoltage;  // Volts (từ ADC 0-4095 -> 0-16.38V)

    VehicleData() :
        tirePressureFL(32.0f), tirePressureFR(32.0f),
        tirePressureRL(32.0f), tirePressureRR(32.0f),
        engineTemp(90.0f), batteryVoltage(12.6f) {}
};

class SerialManager : public QObject
{
    Q_OBJECT

public:
    explicit SerialManager(QObject *parent = nullptr);
    ~SerialManager();

    // Quản lý kết nối
    bool connectToPort(const QString &portName, qint32 baudRate = 9600);
    void disconnect();
    bool isConnected() const;
    QStringList getAvailablePorts();

    // Lấy dữ liệu
    VehicleData getCurrentData() const { return m_currentData; }

signals:
    void dataUpdated(const VehicleData &data);
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString &error);

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void generateSimulatedData();

private:
    QSerialPort *m_serialPort;
    QTimer *m_simulationTimer;
    VehicleData m_currentData;
    QByteArray m_receiveBuffer;

    void parseReceivedData(const QByteArray &data);
    float adcToBatteryVoltage(uint16_t adcValue);
};

#endif // SERIALMANAGER_H
