# MQTT configuration
Copy the example header in each sketch folder to secrets.h. Set Wi-Fi name/password, broker host, port, username/password, unique client ID and topic. The templates use placeholder hostnames and example topics. Use a private lab broker, not a public test broker for sensitive data. Plain MQTT is unencrypted; use TLS and access controls for production.

Example subscriber on a trusted test network (replace host):
```sh
mosquitto_sub -h YOUR_BROKER -p 1883 -t 'classroom/#' -v
```
If authentication is enabled use your local subscriber configuration. Do not share credentials in screenshots.

Environmental fields: device, tempC, tempF, humidity, rssi, uptime_seconds, message_count. Motion fields: device, pir, rssi, uptime_seconds. Uptime is elapsed device time, not Unix time. The revised schema deliberately omits MAC, IP and student identifiers. Update downstream subscribers accordingly.
