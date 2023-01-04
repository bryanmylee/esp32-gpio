#include <Arduino.h>
#include <BluetoothSerial.h>

#define LED_PIN 4

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

BluetoothSerial SerialBT;

void setup()
{
  Serial.begin(9600);
  SerialBT.begin("ESP32");
  Serial.println("The device started, you can now pair it with Bluetooth!");
}

void loop()
{
  // Check if there are any incoming bytes from the serial.
  if (Serial.available())
  {
    SerialBT.write(Serial.read());
  }
  // Check if there are any incoming bytes from Bluetooth.
  if (SerialBT.available())
  {
    Serial.write(SerialBT.read());
  }
  delay(20);
}