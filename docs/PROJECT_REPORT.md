# LocalLink — Offline Emergency Information System
## Project Report

**Student:** [Name]  
**Roll number:** [Roll Number]  
**Department:** [Department]  
**Institution:** [College]  
**Guide:** [Guide Name]  
**Academic year:** [Year]

## Abstract
LocalLink is an ESP32-based prototype that makes essential information available through a local Wi-Fi network. The ESP32 operates as an access point and web server; users connect with a phone or laptop and open locally hosted pages without internet access. The interface is intended to provide emergency-number shortcuts, an Odisha district selector, safety guides, and DHT11 temperature/humidity readings. It demonstrates a supplementary information channel during connectivity disruption, not a certified emergency-response system.

## 1. Introduction
During emergencies such as floods, cyclones, and fires, connectivity can be unreliable and online information may be harder to access. LocalLink explores hosting a small information portal directly on a microcontroller so nearby users can access stored guidance over Wi-Fi.

## 2. Problem Statement
How might essential emergency information remain accessible to nearby users when internet connectivity is unavailable or unreliable?

## 3. Objectives
- Configure ESP32 as a Wi-Fi access point and web server.
- Host an offline-first webpage.
- Present emergency-number shortcuts and safety guidance.
- Organize information with an Odisha district selector.
- Read ambient temperature and humidity using DHT11.

## 4. Proposed Solution
The ESP32 creates the network **LocalLink** and serves the website at **http://192.168.4.1**. Sensor endpoint: **/api/sensor**. DHT11 wiring: VCC→3V3, DATA→GPIO4, GND→GND.

## 5. Hardware and Software
Hardware: ESP32 development board, DHT11 module, USB cable, jumper wires. Software: Arduino IDE, C++, ESP32 Wi-Fi/WebServer libraries, DHT library, HTML, CSS, JavaScript.

## 6. Working
1. Power on the ESP32 and start its access point.
2. Connect a phone or laptop to LocalLink.
3. Open the local IP in a browser.
4. ESP32 serves the webpage.
5. Browser requests `/api/sensor`; ESP32 reads DHT11 and returns data.

## 7. Features
Locally hosted webpage, emergency-number shortcuts, Odisha district selector, basic safety guides, sensor display, and English/Odia toggle where translations are available.

## 8. Potential Real-Life Application
A community volunteer or shelter could power the device and share Wi-Fi details, allowing nearby users to read locally stored safety information during an internet outage. This is a proposed use case, not documented field deployment.

## 9. Testing Plan
Record actual results after testing; do not claim success without evidence.

| Test | Procedure | Expected result | Actual result |
|---|---|---|---|
| Access point | Power ESP32 | LocalLink network appears | [Record] |
| Connection | Join from phone | Client connects | [Record] |
| Website | Open 192.168.4.1 | Page loads | [Record] |
| District selector | Select a district | Selection updates | [Record] |
| Sensor API | Open /api/sensor | JSON or unavailable status | [Record] |
| Offline access | Disable mobile data, keep Wi-Fi | Local page remains accessible | [Record] |

## 10. Limitations
DHT11 measures ambient temperature and humidity only; it does not detect fire, predict cyclones, or identify floods. The system does not dispatch responders. Contacts require official verification. Calling may require cellular coverage. Range and uptime depend on power and environment.

## 11. Future Scope
Battery backup, protective enclosure, verified district contacts, reviewed Odia translations, offline documents, field testing, and possible official warning-feed integration when connectivity is available.

## 12. Conclusion
LocalLink demonstrates a local Wi-Fi information portal using ESP32. Further work should focus on trustworthy content, accessibility, reliable power, and testing. It must not replace official emergency services.

## References
- Espressif ESP32: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/
- Arduino: https://docs.arduino.cc/
- India ERSS 112: https://112.gov.in/
- IMD: https://mausam.imd.gov.in/
- NDMA: https://ndma.gov.in/
