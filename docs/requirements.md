# Requirements
- Arduino IDE 2.x (recommended; exact original version unknown).
- ESP32 board package by Espressif Systems; select the actual ESP32-C3 board.
- Arduino SAMD Boards package; select Arduino Nano 33 IoT.
- ESP32 sketch: ArduinoMqttClient, Adafruit GFX Library, Adafruit SSD1306, DHT sensor library by Adafruit and Adafruit Unified Sensor dependency. WiFi and Wire come from board support.
- Nano sketch: WiFiNINA, MQTT by Joel Gaehwiler (256dpi), Adafruit GFX Library, Adafruit SSD1306. Wire comes from board support.
- Two boards, DHT22, PIR sensor, two 128x64 I2C SSD1306 displays, suitable supplies, USB cables and wiring.
- Compatible Wi-Fi network and accessible MQTT broker; optional Mosquitto subscriber.

Library versions are not pinned because the original versions are unknown. Record exact versions after a successful compile. Python and requirements.txt are not required.
