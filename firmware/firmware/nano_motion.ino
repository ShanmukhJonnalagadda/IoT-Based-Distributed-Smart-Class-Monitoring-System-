#include <WiFiNINA.h>
#include <MQTTClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "secrets.h"
WiFiClient wifi;
MQTTClient mqtt(256);
Adafruit_SSD1306 oled(128,64,&Wire,-1);
unsigned long lastPublish=0;
void connectNetwork() {
  while(WiFi.status()!=WL_CONNECTED) {
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD); delay(500);
  }
  while(!mqtt.connected()) {
    if(!mqtt.connect(MQTT_CLIENT_ID,MQTT_USER,MQTT_PASSWORD)) delay(1000);
  }
}
void setup() {
  Serial.begin(115200); pinMode(2,INPUT);
  if(!oled.begin(SSD1306_SWITCHCAPVCC,0x3C)) while(true) delay(1000);
  oled.setTextColor(SSD1306_WHITE); oled.setTextSize(1);
  mqtt.begin(MQTT_HOST,MQTT_PORT,wifi); connectNetwork();
}
void loop() {
  connectNetwork(); mqtt.loop();
  if(millis()-lastPublish<3000) return;
  lastPublish=millis(); int pir=digitalRead(2); long rssi=WiFi.RSSI();
  oled.clearDisplay(); oled.setCursor(0,0);
  oled.print("PIR: "); oled.println(pir);
  oled.print("RSSI: "); oled.println(rssi);
  oled.print("Uptime s: "); oled.println(millis()/1000); oled.display();
  String payload=String("{\"device\":\"nano33iot\",\"pir\":")+String(pir)+",\"rssi\":"+String(rssi)+",\"uptime_seconds\":"+String(millis()/1000)+"}";
  mqtt.publish(MQTT_TOPIC,payload); Serial.println(payload);
}
