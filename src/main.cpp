#include <Arduino.h>
#include <WiFi.h>

#define LED_PIN 4

const char *SSID = "XXXX";
const char *PASS = "XXXX";

// Initialize the WiFi server.
WiFiServer server(80);

void setup()
{
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);

  delay(10);

  Serial.printf("Connecting to %s\n", SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

int value = 0;

void loop()
{
  // Listen for incoming clients.
  WiFiClient client = server.available();

  // If a client is received.
  if (client)
  {
    Serial.println("New client.");
    // Allocate a String to hold incoming data from the client.
    String currentLine = "";
    // Blocking listen to the connected client.
    while (client.connected())
    {
      // Check if incoming bytes are available from the client.
      if (client.available())
      {
        // Read a single byte.
        char input = client.read();
        Serial.write(input);
        if (input == '\n')
        {
          // If the current line is blank, then we've received two newline
          // characters in a row. That's the end of the HTTP request, so send a
          // response.
          if (currentLine.length() == 0)
          {
            /// HEADER
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            /// CONTENT
            // The content of the HTTP response follows the header.
            client.print("<a href=\"/H\">Turn the LED on</a><br>");
            client.print("<a href=\"/L\">Turn the LED off</a><br>");

            /// END
            client.println();
            break;
          }
          else
          {
            // Otherwise, clear the currentLine if a newline is received.
            currentLine = "";
          }
        }
        else if (input != '\r')
        {
          // Anything else but a carriage return should be added to the end of
          // the currentLine.
          currentLine += input;
        }

        if (currentLine.endsWith("GET /H"))
        {
          digitalWrite(LED_PIN, HIGH);
        }
        else if (currentLine.endsWith("GET /L"))
        {
          digitalWrite(LED_PIN, LOW);
        }
      }
    }
    // Once the client is no longer connected, close the connection.
    client.stop();
    Serial.println("Client disconnected.");
  }
}