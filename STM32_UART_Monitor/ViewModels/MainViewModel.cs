using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using STM32_UART_Monitor.Models;
using STM32_UART_Monitor.Services;

namespace STM32_UART_Monitor.ViewModels
{
    public partial class MainViewModel : ObservableObject, IDisposable
    {
        private readonly SerialService _serialService;

        [ObservableProperty]
        private ObservableCollection<string> _availablePorts;

        [ObservableProperty]
        private string _selectedPort = string.Empty;

        [ObservableProperty]
        private ObservableCollection<int> _baudRates;

        [ObservableProperty]
        private int _selectedBaudRate;

        [ObservableProperty]
        private bool _isConnected;

        [ObservableProperty]
        private string _connectionStatus = "Disconnected";

        [ObservableProperty]
        private string _adcValue = "----";

        [ObservableProperty]
        private string _temperatureValue = "----";

        [ObservableProperty]
        private ObservableCollection<string> _logMessages;

        [ObservableProperty]
        private int _receivedPackets;

        [ObservableProperty]
        private string _connectionButtonText = "Connect";

        public MainViewModel()
        {
            _serialService = new SerialService();

            AvailablePorts = new ObservableCollection<string>();
            BaudRates = new ObservableCollection<int> { 9600, 19200, 38400, 57600, 115200 };
            LogMessages = new ObservableCollection<string>();

            SelectedBaudRate = 115200;

            _serialService.DataReceived += OnDataReceived;
            _serialService.ErrorOccurred += OnErrorOccurred;
            _serialService.ConnectionChanged += OnConnectionChanged;

            RefreshPorts();
        }

        [RelayCommand]
        private void RefreshPorts()
        {
            AvailablePorts.Clear();
            var ports = _serialService.GetAvailablePorts();

            foreach (var port in ports)
            {
                AvailablePorts.Add(port);
            }

            if (AvailablePorts.Any())
            {
                SelectedPort = AvailablePorts[0];
            }

            AddLog($"Found {ports.Length} COM port(s)");
        }

        [RelayCommand]
        private async Task ToggleConnection()
        {
            if (!IsConnected)
            {
                if (string.IsNullOrEmpty(SelectedPort))
                {
                    MessageBox.Show("Please select a COM port!", "Error",
                        MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }

                AddLog($"Connecting to {SelectedPort} at {SelectedBaudRate} baud...");
                bool success = await _serialService.ConnectAsync(SelectedPort, SelectedBaudRate);

                if (success)
                {
                    AddLog("✓ Connected successfully!");
                }
                else
                {
                    AddLog("✗ Connection failed!");
                }
            }
            else
            {
                AddLog("Disconnecting...");
                await _serialService.DisconnectAsync();
                AddLog("✓ Disconnected");
            }
        }

        [RelayCommand]
        private void ClearLog()
        {
            LogMessages.Clear();
            ReceivedPackets = 0;
        }

        private void OnDataReceived(object? sender, SensorData data)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                AdcValue = data.AdcValue.ToString();
                TemperatureValue = $"{data.Temperature:F2}°C";

                AddLog(data.ToString());

                ReceivedPackets++;
            });
        }

        private void OnErrorOccurred(object? sender, string error)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                AddLog($"✗ ERROR: {error}");
            });
        }

        private void OnConnectionChanged(object? sender, bool connected)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                IsConnected = connected;
                ConnectionStatus = connected ? "Connected" : "Disconnected";
                ConnectionButtonText = connected ? "Disconnect" : "Connect";

                if (!connected)
                {
                    AdcValue = "----";
                    TemperatureValue = "----";
                }
            });
        }

        private void AddLog(string message)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                LogMessages.Insert(0, $"[{DateTime.Now:HH:mm:ss}] {message}");

                if (LogMessages.Count > 100)
                {
                    LogMessages.RemoveAt(LogMessages.Count - 1);
                }
            });
        }

        public void Dispose()
        {
            _serialService?.Dispose();
        }
    }
}