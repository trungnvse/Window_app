using System;

namespace STM32_UART_Monitor.Models
{
    public class SensorData
    {
        public DateTime Timestamp { get; set; }
        public int AdcValue { get; set; }
        public double Temperature { get; set; }
        public string RawData { get; set; } = string.Empty;

        public SensorData()
        {
            Timestamp = DateTime.Now;
        }

        public override string ToString()
        {
            return $"[{Timestamp:HH:mm:ss}] ADC: {AdcValue}, Temp: {Temperature:F2}°C";
        }
    }
}