#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ArduinoMqttClient.h>

const char gNumber = "TEAM_02";
const char deviceName = "esp32c3";

const char ssid[] = "YOUR_WIFI_SSID";
const char pass[] = "YOUR_WIFI_PASSWORD";
const char mqtt_server = "test.mosquitto.org";
const int   mqtt_port   = 1883;
const char mqtt_topic  = "gmu/ece508/G5594/nano33iot";
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define DHT22_PIN 3
DHT dht22(DHT22_PIN, DHT22);
unsigned long startMillis;
long messageCount = 0;
void getMacWifiShieldMacRouterSS(char *macCombined) {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  sprintf(macCombined, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}
String getUptime() {
  unsigned long sec = (millis() - startMillis) / 1000;
  char buf[16];
  sprintf(buf, "%02lu:%02lu:%02lu",
          (sec / 3600) % 24,
          (sec / 60) % 60,
          sec % 60);
  return String(buf);
}
void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  dht22.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed!");
    while (1);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  Serial.print("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println(" connected!");
  mqttClient.setId("esp32c3Client");
  Serial.print("Connecting MQTT...");
  if (!mqttClient.connect(mqtt_server, mqtt_port)) {
    Serial.print(" failed: ");
    Serial.println(mqttClient.connectError());
    while (true);
  }
  Serial.println(" connected!");
  startMillis = millis();
}
void loop() {
  float tempC = dht22.readTemperature();
  float tempF = (tempC * 9.0 / 5.0) + 32.0;
  float hum   = dht22.readHumidity();
  long rssi = WiFi.RSSI();
  char macCombined[30];
  getMacWifiShieldMacRouterSS(macCombined);
  messageCount++;
  long epoch = millis() / 1000;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("gnumber:");
  display.println(gNumber);
  display.print("TempF:");
  display.println(tempF);
  display.print("TempC:");
  display.println(tempC);
  display.print("Humidity:");
  display.println(hum);
  display.print("RSSI:");
  display.println(rssi);
  display.print("Uptime:");
  display.println(getUptime());
  display.display();
  String payload =
    "{"
      "\"measurement\":\"ece508meas\","
      "\"device\":\"esp32c3module\","
      "\"mac\":\"" + String(macCombined) + "\","
      "\"gnumber\":\"" + String(gNumber) + "\","
      "\"nmrMqttMessages\":" + String(messageCount) + ","
      "\"rssi\":" + String(rssi) + ","
      "\"epoch\":" + String(epoch) + ","
      "\"tempF\":" + String(tempF) + ","
      "\"tempC\":" + String(tempC) + ","
      "\"humidity\":" + String(hum) +
    "}";
  mqttClient.beginMessage(mqtt_topic);
  mqttClient.print(payload);
  mqttClient.endMessage();
  Serial.println("--- MQTT SENT ---");
  Serial.println(payload);
  Serial.println();
  delay(2000);
}

