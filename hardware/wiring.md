# LocalLink Hardware Wiring

## DHT11 module → ESP32

| DHT11 pin | ESP32 pin |
|---|---|
| VCC / + | 3V3 |
| DATA / S | GPIO 4 |
| GND / − | GND |

Use the labels on your module; pin order can vary. Most three-pin DHT11 modules include a pull-up resistor. A bare sensor may need one.

Power the ESP32 through USB for this prototype. Keep the relay disconnected in the current build. Do not connect mains voltage. LocalLink is not a certified disaster-warning device.
