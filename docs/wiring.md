# Wiring
Pin mappings are derived from the original source, not a verified physical schematic.

| Node | Signal | Pin |
|---|---|---|
| ESP32-C3 | DHT22 data | GPIO 3 |
| ESP32-C3 | OLED SDA | GPIO 8 |
| ESP32-C3 | OLED SCL | GPIO 9 |
| Nano 33 IoT | PIR output | D2 |
| Nano 33 IoT | OLED SDA/SCL | Board-labeled SDA/SCL |

Both displays use I2C address 0x3C. Connect grounds together within each node. Verify the specific sensor module supply and output voltage; both controller boards use 3.3 V logic. Do not apply 5 V signals to GPIO. A bare DHT22 typically needs a data pull-up to the appropriate logic supply; modules may already include one. Verify boot-strapping constraints on the actual ESP32-C3 board before wiring GPIO 8/9. Disconnect power before wiring.
