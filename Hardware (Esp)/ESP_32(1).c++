 #include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HTTPClient.h>

const char* ssid = "Steurzentrale";
const char* password = "STM-b/84729635";

int bluePins[] = {15, 2, 4, 16, 17};
#define DIFFUSER_PIN 13
#define FAN_PIN 14
#define SPEAKER1_PIN 25
#define SPEAKER2_PIN 26
#define SS_PIN 5
#define RST_PIN 22

WebServer server(80);
MFRC522 rfid(SS_PIN, RST_PIN);
String letzteUID = "Noch nichts gescannt";
bool diffuserStatus = false;
bool fanStatus = false;
bool speakerStatus = false;

void alleBlauenAn() {
  for (int i = 0; i < sizeof(bluePins) / sizeof(int); i++) {
    digitalWrite(bluePins[i], HIGH);
  }
}

void alleBlauenAus() {
  for (int i = 0; i < sizeof(bluePins) / sizeof(int); i++) {
    digitalWrite(bluePins[i], LOW);
  }
}

void schalteBlaueLED(int pin) {
  for (int i = 0; i < sizeof(bluePins) / sizeof(int); i++) {
    if (bluePins[i] == pin) {
      digitalWrite(pin, LOW);
      return;
    }
  }
}

void diffuserAn() { diffuserStatus = true; digitalWrite(DIFFUSER_PIN, HIGH); }
void diffuserAus() { diffuserStatus = false; digitalWrite(DIFFUSER_PIN, LOW); }
void fanAn() { fanStatus = true; digitalWrite(FAN_PIN, HIGH); }
void fanAus() { fanStatus = false; digitalWrite(FAN_PIN, LOW); }
void speakerAn() { speakerStatus = true; digitalWrite(SPEAKER1_PIN, HIGH); }
void speakerAus() { speakerStatus = false; digitalWrite(SPEAKER1_PIN, LOW); }

void soundEffekt() {
  tone(SPEAKER1_PIN, 880, 150);
  delay(160);
  tone(SPEAKER2_PIN, 660, 150);
  delay(160);
  noTone(SPEAKER1_PIN);
  noTone(SPEAKER2_PIN);
}

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html><html><head><meta charset='utf-8'>
    <style>
      body { background-color: #121212; color: #eee; font-family: sans-serif; }
      h1 { color: #4af; }
      button { margin: 4px; padding: 10px; font-size: 16px; background: #333; color: white; border: none; cursor: pointer; }
      button:hover { background: #555; }
    </style>
    <title>Kuehlsteuerzentralle</title></head><body>
    <h1>Kuehlsteuerzentralle</h1>
    <p>RFID UID: )rawliteral" + letzteUID + R"rawliteral(</p>
    <h3>Diffuser</h3>
    <a href='/diffuseran'><button>Diffuser AN</button></a>
    <a href='/diffuseraus'><button>Diffuser AUS</button></a>
    <h3>Ventilator</h3>
    <a href='/fanan'><button>Ventilator AN</button></a>
    <a href='/fanaus'><button>Ventilator AUS</button></a>
    <h3>Lautsprecher</h3>
    <a href='/speakeran'><button>Lautsprecher AN</button></a>
    <a href='/speakeraus'><button>Lautsprecher AUS</button></a>
    <a href='/sound'><button>Soundeffekt</button></a>
    <h3>Reaktor Module</h3>
    <button>Reaktor 1 - Modul A</button>
    <a href='/reaktor1_modulB'><button>Reaktor 1 - Modul B</button></a>
    <button>Reaktor 1 - Modul C</button><br>
    <button>Reaktor 2 - Modul A</button><button>Reaktor 2 - Modul B</button><button>Reaktor 2 - Modul C</button><br>
    <button>Reaktor 3 - Modul A</button><button>Reaktor 3 - Modul B</button><button>Reaktor 3 - Modul C</button><br>
    <button>Reaktor 4 - Modul A</button><button>Reaktor 4 - Modul B</button><button>Reaktor 4 - Modul C</button><br>
    <h3>Blaue LED schalten</h3>
    <form action='/blau'>
      <input type='number' name='pin' placeholder='Blau-Pin (z.B. 21)'>
      <input type='submit' value='Blau setzen'>
    </form>
    <a href='/blauall'><button>Alle Blau AN</button></a>
    <a href='/blauoff'><button>Alle Blau AUS</button></a>
  )rawliteral";
  server.send(200, "text/html", html);
}

void handleBlau() {
  if (server.hasArg("pin")) {
    int p = server.arg("pin").toInt();
    schalteBlaueLED(p);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleUID() {
  server.send(200, "text/plain", letzteUID);
}

void handleReaktor1ModulB() {
  fanAn();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleSound() {
  soundEffekt();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleBlauAll() {
  alleBlauenAn();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleBlauOff() {
  alleBlauenAus();
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFi.config(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  SPI.begin();
  rfid.PCD_Init();

  pinMode(DIFFUSER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(SPEAKER1_PIN, OUTPUT);
  pinMode(SPEAKER2_PIN, OUTPUT);
  diffuserAus(); fanAus(); speakerAus();

  for (int i = 0; i < sizeof(bluePins) / sizeof(int); i++) {
    pinMode(bluePins[i], OUTPUT);
    digitalWrite(bluePins[i], HIGH);
  }

  server.on("/", handleRoot);
  server.on("/blau", handleBlau);
  server.on("/blauall", handleBlauAll);
  server.on("/blauoff", handleBlauOff);
  server.on("/diffuseran", []() { diffuserAn(); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/diffuseraus", []() { diffuserAus(); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/fanan", []() { fanAn(); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/fanaus", []() { fanAus(); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/speakeran", []() { speakerAn(); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/speakeraus", []() { speakerAus(); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/sound", handleSound);
  server.on("/reaktor1_modulB", handleReaktor1ModulB);
  server.on("/uid", handleUID);

  server.begin();
}

void loop() {
  server.handleClient();
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    letzteUID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      if (rfid.uid.uidByte[i] < 0x10) letzteUID += "0";
      letzteUID += String(rfid.uid.uidByte[i], HEX);
    }
    letzteUID.toUpperCase();
    delay(1000);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    handleSound();
  }
}
