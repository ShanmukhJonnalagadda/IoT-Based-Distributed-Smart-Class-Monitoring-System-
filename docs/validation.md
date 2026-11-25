# Validation plan
Original log inspection: consumer records contain RSSI, message counters and elapsed-time fields. Some counters reset. No packet-loss or sensor-accuracy metric is established here.

| Test | Procedure | Expected outcome | Revised firmware status |
|---|---|---|---|
| Build | Compile each sketch for its board | No compiler errors | Not run |
| OLED | Boot each node | Local readings shown | Not run |
| DHT22 | Compare with reference thermometer/hygrometer | Record measured difference | Not run |
| PIR | Move in sensor field after warm-up | Digital state changes | Not run |
| MQTT | Subscribe to both configured topics | Valid JSON at nominal intervals | Not run |
| Invalid DHT reading | Disconnect sensor with power off then reboot | No invalid JSON measurement | Not run |
| Network interruption | Disconnect and restore Wi-Fi | Document recovery behavior | Not run |

Capture serial output, subscriber output and hardware photos without personal data. Record board/library versions and actual test outcomes; do not mark tests passed until executed.
