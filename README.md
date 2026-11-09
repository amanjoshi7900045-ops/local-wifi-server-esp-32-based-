# LocalLink — Offline Emergency Information System

LocalLink is an ESP32-based prototype hosting an emergency-information webpage over its own Wi-Fi network. The local website can be opened without internet access. A DHT11 demonstrates ambient temperature and humidity monitoring.

> **Safety:** Educational prototype only. It does not detect disasters, dispatch responders, or guarantee emergency calls. Verify safety information and contacts with official sources.

## Features
- ESP32 Wi-Fi access point and local web server
- Local website at `http://192.168.4.1`
- Odisha district selector (30 districts)
- Emergency-number shortcuts and safety guides
- DHT11 sensor endpoint at `/api/sensor`
- English/Odia interface option (translation completeness must be verified)

## Hardware
ESP32 development board, DHT11 module, USB power, and jumper wires.

| DHT11 | ESP32 |
|---|---|
| VCC | 3V3 |
| DATA | GPIO 4 |
| GND | GND |

## Run
1. Install Arduino IDE and ESP32 board support.
2. Install the DHT sensor library (and Adafruit Unified Sensor if requested).
3. Open the project's `.ino` sketch in `firmware/`.
4. Select the matching board and port; upload.
5. Join Wi-Fi **LocalLink**, password **LocalLink123**.
6. Open `http://192.168.4.1`.

These are demonstration credentials; change them before deployment. See `hardware/wiring.md`, `docs/PROJECT_REPORT.md`, and `docs/PRESENTATION.md`.

## Limitations
DHT11 measures only ambient temperature and humidity. It is not a fire, flood, or cyclone detector. Calls may require cellular coverage. Wi-Fi range and uptime depend on environment and power.

## References
- https://docs.espressif.com/projects/esp-idf/en/stable/esp32/
- https://docs.arduino.cc/
- https://112.gov.in/
- https://ndma.gov.in/
