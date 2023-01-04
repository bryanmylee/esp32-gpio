#include <Arduino.h>
#include <WiFi.h>

void setup()
{
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop()
{
  Serial.println("Start scan");

  // WiFi.scanNetworks will return the number of networks found.
  int networkCount = WiFi.scanNetworks();
  Serial.println("Scan complete");
  if (networkCount == 0)
  {
    Serial.println("No networks found");
  }
  else
  {
    for (int i = 0; i < networkCount; ++i)
    {
      Serial.printf("%d: %s (%d) %s\n",
                    i + 1,
                    WiFi.SSID(i),
                    WiFi.RSSI(i),
                    WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");

  delay(5000);
}