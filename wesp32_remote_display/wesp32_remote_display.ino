#include "ETH.h"
#include "WebServer.h"
#include "ESPmDNS.h"
#include "WiFi.h"
#include "wesp32_logo.h"
#include "Adafruit_ILI9341.h"

// TFT pins
// SCLK = 18, MISO = 32, MOSI = 23, SS = 5, DC = 14, RST = 13
#define TFT_SS 5
#define TFT_DC 14
#define TFT_RST 13
#define TFT_SCK 18
#define TFT_MOSI 23
#define TFT_MISO 32

// Initialize TFT object
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_SS, TFT_DC, TFT_RST);

// Web server
WebServer server(80);


// HTTP handlers

void handleRoot() {
  server.send(200, "text/plain", "Hello from wESP32!\n");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void showMessage() {
  for (uint8_t i=0; i<server.args(); i++) {
    if (server.argName(i) == "text") {
      TFTMessage(server.arg(i));
      server.send(200, "text/plain", "Message is displayed\n");
      return;
    }
  }
  server.send(200, "text/plain", "Please provide a message in 'text'\n");
}

// TFT message
void TFTMessage(String message) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.println(message);
}

// Ethernet event handler
void handleEvent(WiFiEvent_t event){
  String message;
  switch(event) {
    case ARDUINO_EVENT_ETH_START:
      ETH.setHostname("wesp32");
      Serial.println("ETH started");
      Serial.print("ETH MAC: ");
      Serial.println(ETH.macAddress());
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH connected");
      TFTMessage("Ethernet connected\nWaiting for IP...");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("ETH IPv4: ");
      Serial.println(ETH.localIP());
      TFTMessage("Ethernet connected");
      tft.print("IP address: ");
      tft.println(ETH.localIP());
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH disconnected");
      TFTMessage("Waiting for Ethernet\nconnection...");
      break;
    default:
      break;
  }
}

void setup(){
  // Start the serial console 
  Serial.begin(115200);
  // Attach the callback event handler
  WiFi.onEvent(handleEvent);
  // Start the ethernet (Arduino-ESP32 3.x on rev 7 hardware)
  ETH.begin(ETH_PHY_RTL8201, 0, 16, 17, -1, ETH_CLOCK_GPIO0_IN);

  // Start the TFT display and initialize
  tft.begin(32000000);
  tft.setRotation(3);
  tft.drawRGBBitmap(0, 0, (uint16_t *)wesp32_logo_rgb, 320, 240);

  // Set up MDNS responder
  if (!MDNS.begin("wesp32")) {
    Serial.println("Error setting up MDNS responder!");
  }
        
  // Bind HTTP handlers
  server.on("/", handleRoot);
  server.on("/message", showMessage);
  server.on("/inline", [](){
    server.send(200, "text/plain", "This works as well");
  });
  server.onNotFound(handleNotFound);

  // Start the Ethernet web server
  server.begin();
  Serial.println("HTTP server started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
  Serial.println("MDNS server started");
}

void loop(){
  // Handle client requests
  server.handleClient();
}
