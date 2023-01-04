/**
 * This example shows how to use the hardware timer in ESP32.
 */
#include <Arduino.h>

#define BTN_STOP_PIN 2
#define LED_PIN 4

hw_timer_t *timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMutex = portMUX_INITIALIZER_UNLOCKED;

volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;

// IRAM_ATTR forces the value to be in IRAM instead of flash.
// Interrupt Service Routine (ISR) is a function that is invoked every time an interrupt occurs.
void IRAM_ATTR onTimer()
{
  // Lock the mutex to guarantee atomicity.
  portENTER_CRITICAL_ISR(&timerMutex);
  // Increment the counter and set the time of ISR.
  isrCounter++;
  // Get the current time in ms
  lastIsrAt = millis();
  // Unlock the mutex.
  portEXIT_CRITICAL_ISR(&timerMutex);

  // Increment the semaphore to indicate a timer tick has occurred.
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
}

void setup()
{
  Serial.begin(9600);

  pinMode(BTN_STOP_PIN, INPUT);

  // Create the semaphore that informs us when the timer has fired.
  timerSemaphore = xSemaphoreCreateBinary();

  // Use the 1st timer out of 4 hardware timers.
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual).
  timer = timerBegin(0, 80, true);

  // Attach the timer callback function to each interrupt.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set the alarm to call onTimer every second (value in microseconds).
  // Repeat the alarm.
  timerAlarmWrite(timer, 1000000, true);

  // Start the alarm
  timerAlarmEnable(timer);
}

void loop()
{
  // Check if the timer has fired.
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE)
  {
    uint32_t localIsrCount = 0;
    uint32_t localLastIsrAt = 0;

    // Read the interrupt count and time.
    portENTER_CRITICAL(&timerMutex);
    localIsrCount = isrCounter;
    localLastIsrAt = lastIsrAt;
    portEXIT_CRITICAL(&timerMutex);

    Serial.printf("onTimer no. %d at %d ms\n", localIsrCount, localLastIsrAt);
  }

  // Check if the button is pressed.
  if (digitalRead(BTN_STOP_PIN) == LOW)
  {
    Serial.println("Pressed button");
    // If the timer is still running.
    if (timer)
    {
      timerEnd(timer);
      timer = NULL;
    }
  }
}