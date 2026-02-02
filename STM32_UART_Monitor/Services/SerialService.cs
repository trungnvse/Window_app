using System;
using System.IO.Ports;
using System.Threading.Tasks;
using STM32_UART_Monitor.Models;

namespace STM32_UART_Monitor.Services
{
    public class SerialService : IDisposable
    {
        private SerialPort _serialPort;
        private bool _isConnected;

        public event EventHandler<SensorData>? DataReceived;
        public event EventHandler<string>? ErrorOccurred;
        public event EventHandler<bool>? ConnectionChanged;

        public bool IsConnected => _isConnected;

        public SerialService()
        {
            _serialPort = new SerialPort();
            _serialPort.DataReceived += SerialPort_DataReceived;
        }

        public string[] GetAvailablePorts()
        {
            return SerialPort.GetPortNames();
        }

        public async Task<bool> ConnectAsync(string portName, int baudRate = 115200)
        {
            try
            {
                if (_isConnected)
                {
                    await DisconnectAsync();
                }

                _serialPort.PortName = portName;
                _serialPort.BaudRate = baudRate;
                _serialPort.DataBits = 8;
                _serialPort.Parity = Parity.None;
                _serialPort.StopBits = StopBits.One;
                _serialPort.Handshake = Handshake.None;
                _serialPort.ReadTimeout = 500;
                _serialPort.WriteTimeout = 500;

                await Task.Run(() => _serialPort.Open());

                _isConnected = true;
                ConnectionChanged?.Invoke(this, true);
                return true;
            }
            catch (Exception ex)
            {
                ErrorOccurred?.Invoke(this, $"Connection error: {ex.Message}");
                _isConnected = false;
                ConnectionChanged?.Invoke(this, false);
                return false;
            }
        }

        public async Task DisconnectAsync()
        {
            try
            {
                if (_serialPort.IsOpen)
                {
                    await Task.Run(() => _serialPort.Close());
                }
                _isConnected = false;
                ConnectionChanged?.Invoke(this, false);
            }
            catch (Exception ex)
            {
                ErrorOccurred?.Invoke(this, $"Disconnect error: {ex.Message}");
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string data = _serialPort.ReadLine().Trim();

                if (string.IsNullOrEmpty(data))
                    return;

                var sensorData = ParseData(data);
                DataReceived?.Invoke(this, sensorData);
            }
            catch (TimeoutException)
            {
                // Ignore timeout
            }
            catch (Exception ex)
            {
                ErrorOccurred?.Invoke(this, $"Data receive error: {ex.Message}");
            }
        }

        private SensorData ParseData(string rawData)
        {
            var sensorData = new SensorData
            {
                RawData = rawData
            };

            try
            {
                string[] parts = rawData.Split(',');

                foreach (string part in parts)
                {
                    string trimmedPart = part.Trim();

                    if (trimmedPart.StartsWith("ADC:", StringComparison.OrdinalIgnoreCase))
                    {
                        string adcStr = trimmedPart.Substring(4);
                        if (int.TryParse(adcStr, out int adcValue))
                        {
                            sensorData.AdcValue = adcValue;
                        }
                    }
                    else if (trimmedPart.StartsWith("TEMP:", StringComparison.OrdinalIgnoreCase))
                    {
                        string tempStr = trimmedPart.Substring(5);
                        if (double.TryParse(tempStr, out double temperature))
                        {
                            sensorData.Temperature = temperature;
                        }
                    }
                }
            }
            catch
            {
                // Keep default values
            }

            return sensorData;
        }

        public async Task<bool> SendDataAsync(string data)
        {
            try
            {
                if (!_isConnected || !_serialPort.IsOpen)
                    return false;

                await Task.Run(() => _serialPort.WriteLine(data));
                return true;
            }
            catch (Exception ex)
            {
                ErrorOccurred?.Invoke(this, $"Send error: {ex.Message}");
                return false;
            }
        }

        public void Dispose()
        {
            if (_serialPort != null)
            {
                if (_serialPort.IsOpen)
                {
                    _serialPort.Close();
                }
                _serialPort.Dispose();
            }
        }
    }
}