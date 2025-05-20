 #include <WiFi.h>
#include <WebServer.h>

// WLAN-Zugangsdaten
// In ESP2
const char* ssid = "Steurzentrale";
const char* password = "STM-b/84729635";
// Webserver
WebServer server(80);

// GPIO-Listen nach Farbe
const int greenPins[] = {16, 17, 19, 21, 13, 14, 25, 33};
const int redPins[]   = {32, 5, 18, 22, 23, 27, 26, 15};

const int greenCount = sizeof(greenPins) / sizeof(greenPins[0]);
const int redCount   = sizeof(redPins)   / sizeof(redPins[0]);

void setPins(const int* pins, int count, int level) {
  for (int i = 0; i < count; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], level);
  }
}

void handleGreenOn() {
  setPins(greenPins, greenCount, HIGH);
  Serial.println("Grüne LEDs AN");
  server.send(200, "text/plain", "Grün an");
}

void handleGreenOff() {
  setPins(greenPins, greenCount, LOW);
  Serial.println("Grüne LEDs AUS");
  server.send(200, "text/plain", "Grün aus");
}

void handleRedOn() {
  setPins(redPins, redCount, HIGH);
  Serial.println("Rote LEDs AN");
  server.send(200, "text/plain", "Rot an");
}

void handleRedOff() {
  setPins(redPins, redCount, LOW);
  Serial.println("Rote LEDs AUS");
  server.send(200, "text/plain", "Rot aus");
}

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html><html><head><meta charset='utf-8'>
    <style>
      body { background-color: #1a1a1a; color: #eee; font-family: sans-serif; }
      h1 { color: #4af; }
      button { margin: 4px; padding: 10px; font-size: 16px; background: #333; color: white; border: none; cursor: pointer; }
      button:hover { background: #555; }
    </style>
    <title>ESP2 Steuerung</title></head><body>
    <h1>ESP2 LED-Steuerung</h1>
    <a href='/green-on'><button>Grün AN</button></a>
    <a href='/green-off'><button>Grün AUS</button></a><br>
    <a href='/red-on'><button>Rot AN</button></a>
    <a href='/red-off'><button>Rot AUS</button></a>
    </body></html>
  )rawliteral";

  server.send(200, "text/html", html);
}


void setup() {
  Serial.begin(115200);

  WiFi.config(IPAddress(192,168,4,2), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.print("Verbinde mit WLAN");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nVerbunden mit IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  

  server.on("/green-on",  handleGreenOn);
  server.on("/green-off", handleGreenOff);
  server.on("/red-on",    handleRedOn);
  server.on("/red-off",   handleRedOff);
  server.begin();
  Serial.println("HTTP-Server gestartet");
  handleGreenOff();
  handleRedOff();
  
}

void loop() {
  server.handleClient();
}

