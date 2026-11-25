#include <WiFiNINA.h>
#include <MQTTClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- OLED SETTINGS ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- PIR SENSOR ----------
#define PIR_PIN 2

// ---------- WIFI ----------
const char ssid = "YOUR_WIFI_SSID";
const char password = "YOUR_WIFI_PASSWORD";
const char gNumber = "TEAM_02";

// ---------- LOCAL SHIFTR.IO MQTT ----------
const char mqttHost[] = "   ";
const int  mqttPort   = 2883;
const char mqttUser[] = "YOUR_MQTT_USERNAME";
const char mqttPass[] = "YOUR_MQTT_PASSWORD";

WiFiClient wifiClient;
MQTTClient mqttClient(256);

// ---------- GLOBAL ----------
unsigned long lastPublish = 0;
char gNumber[] = "G5594,G4402";

// ---------- Convert uptime ----------
String getUptime() {
  unsigned long ms = millis();
  unsigned long sec = ms / 1000;
  unsigned long min = sec / 60;
  unsigned long hr  = min / 60;

  sec = sec % 60;
  min = min % 60;

  char buffer[20];
  sprintf(buffer, "%02lu:%02lu:%02lu", hr, min, sec);
  return String(buffer);
}

// ---------- OLED DISPLAY ----------
void showOLED(int pirValue, int rssi) {

  String ipStr = WiFi.localIP().toString();   // <-- Convert IP to simple string

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);   
  display.setCursor(0, 0);
  display.print(gNumber);
  display.setCursor(0, 10);
  display.print("IP: ");
  display.print(ipStr);    
  display.setCursor(0, 20);
  display.print("Uptime: ");
  display.print(getUptime());
  display.setCursor(0, 30);
  display.print("PIR: ");
  display.print(pirValue);
  display.setCursor(0, 40);
  display.print("RSSI: ");
  display.print(rssi);
  display.setCursor(0, 52);
  display.print("ClassMonitoringSystem");
  display.display();
}
void connectWiFi() {
  Serial.print("Connecting WiFi...");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");
}

// ---------- MQTT ----------
void connectMQTT() {
  Serial.print("Connecting to shiftr.io ");
  while (!mqttClient.connect("nano33iot", mqttUser, mqttPass)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" connected!");
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(PIR_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED FAILED");
    while (1);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Starting...");
  display.display();

  connectWiFi();
  mqttClient.begin(mqttHost, mqttPort, wifiClient);
  connectMQTT();
}

// ---------- LOOP ----------
void loop() {

  mqttClient.loop();

  if (!mqttClient.connected()) {
    connectMQTT();
  }

  if (millis() - lastPublish >= 3000) {
    lastPublish = millis();

    int pirValue = digitalRead(PIR_PIN);
    int rssi = WiFi.RSSI();

    String payload = "{";
    payload += "\"gnumber\":\"" + String(gNumber) + "\",";
    payload += "\"device\":\"nano33iot\",";
    payload += "\"pir\":" + String(pirValue) + ",";
    payload += "\"rssi\":" + String(rssi) + ",";
    payload += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    payload += "\"uptime\":\"" + getUptime() + "\"";
    payload += "}";

    mqttClient.publish("gmu/ece508/Gxxxx5594/pir", payload);

    Serial.print("Published: ");
    Serial.println(payload);

    showOLED(pirValue, rssi);
  }
}

