#include <Arduino.h>

// Pin 4 refers to GPIO4.
#define LED_PIN 4

void setup()
{
  // Configures the pin to behave either as an input or an output.
  //
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  // If the pin is configured as OUTPUT, its voltage will be set to 5V or 3.3V
  // on 3.3V boards (HIGH) or 0V (LOW).
  //
  // If the pin is configured as INPUT, this enables (HIGH) or disables (LOW)
  // the internal pullup.
  //
  // An internal pull-up allows the hardware to reliably use, for instance, a
  // switch between the pin and ground without an external resistor.
  digitalWrite(LED_PIN, HIGH);

  delay(1000);

  // Subsequently, set the voltage at GPIO4 to LOW.
  digitalWrite(LED_PIN, LOW);

  delay(1000);
}