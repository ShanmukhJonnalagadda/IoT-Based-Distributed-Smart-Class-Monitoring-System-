#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ArduinoMqttClient.h>
#include "secrets.h"
WiFiClient wifi;
MqttClient mqtt(wifi);
Adafruit_SSD1306 oled(128,64,&Wire,-1);
DHT sensor(3,DHT22);
unsigned long lastPublish=0, count=0;
void connectNetwork() {
  if (WiFi.status()!=WL_CONNECTED) {
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    while(WiFi.status()!=WL_CONNECTED) delay(500);
  }
  while(!mqtt.connected()) {
    if(!mqtt.connect(MQTT_HOST,MQTT_PORT)) delay(1000);
  }
}
void setup() {
  Serial.begin(115200); Wire.begin(8,9); sensor.begin();
  if(!oled.begin(SSD1306_SWITCHCAPVCC,0x3C)) while(true) delay(1000);
  oled.setTextColor(SSD1306_WHITE); oled.setTextSize(1);
  mqtt.setId(MQTT_CLIENT_ID);
  if(MQTT_USER[0]) mqtt.setUsernamePassword(MQTT_USER,MQTT_PASSWORD);
  connectNetwork();
}
void loop() {
  connectNetwork(); mqtt.poll();
  if(millis()-lastPublish<2000) return;
  lastPublish=millis();
  float c=sensor.readTemperature(), h=sensor.readHumidity();
  if(isnan(c)||isnan(h)) { Serial.println("Invalid DHT22 reading"); return; }
  float f=c*1.8f+32; long rssi=WiFi.RSSI();
  oled.clearDisplay(); oled.setCursor(0,0);
  oled.print("Temp C: "); oled.println(c);
  oled.print("Temp F: "); oled.println(f);
  oled.print("Humidity: "); oled.println(h);
  oled.print("RSSI: "); oled.println(rssi);
  oled.print("Uptime s: "); oled.println(millis()/1000); oled.display();
  String payload=String("{\"device\":\"esp32c3\",\"tempC\":")+String(c)+",\"tempF\":"+String(f)+",\"humidity\":"+String(h)+",\"rssi\":"+String(rssi)+",\"uptime_seconds\":"+String(millis()/1000)+",\"message_count\":"+String(++count)+"}";
  mqtt.beginMessage(MQTT_TOPIC); mqtt.print(payload); mqtt.endMessage(); Serial.println(payload);
}
