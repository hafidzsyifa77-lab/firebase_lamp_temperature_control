| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

Smart Home IoT: DHT11 Sensor Monitoring & Dual-Lamp Control via Firebase

A professional-grade Internet of Things (IoT) Smart Home prototype built using the **ESP32** microcontroller and the official **ESP-IDF Framework v5.3.1**. This project demonstrates real-time, non-blocking multi-tasking architecture to monitor temperature/humidity levels and control multiple appliances instantly via Firebase Realtime Database.

Key Features

- **Multi-Tasking Architecture (FreeRTOS):** Decoupled sensor acquisition and hardware control into independent, concurrent FreeRTOS tasks. This ensures the lamp actuation remains instantly responsive (*low-latency*) without being blocked by the DHT11's 5-second sampling delay.
- **Event-Driven HTTP Client:** Implemented the native ESP-IDF HTTP Event Handler (`HTTP_EVENT_ON_DATA`). This robust mechanism catches incoming JSON data stream-by-stream directly from network buffers, completely eliminating data loss or empty buffer crashes.
- **Storage & Bandwidth Optimization (Partial Updates):** Utilizes the `HTTP PATCH` method instead of `PUT`. Environmental metrics are only transmitted to the cloud when a value change is detected (*Event-Driven State Change*), significantly reducing database storage overhead and WiFi bandwidth consumption.
- **Dual-Lamp Parsing Engine:** Parses raw configuration payloads directly inside the ESP32 to independently drive `GPIO 5` and `GPIO 18` based on key-value extractions.

Hardware & Tech Stack

Hardware Components
- **ESP32** Development Board
- **DHT11** Temperature & Humidity Sensor
- **2x Physical LEDs** & Current-Limiting Resistors ($220\ \Omega$)
- Breadboard & Jumper Wires

Software & Cloud Architecture
- **ESP-IDF v5.3.1** (Espressif IoT Development Framework)
- **FreeRTOS** (Real-Time Embedded Operating System)
- **Firebase Realtime Database** (Google NoSQL Cloud Database)
- Language: **C**

---

Firmware Directory Structure

The project strictly follows a modular architecture, segregating low-level drivers and application logic to ensure clean, high-maintainability code:

```text
├── main/
│   ├── main.c                  # Core Scheduler (Task initialization & prioritization)
│   ├── wifi_smart.c / .h       # Network Layer (WiFi Station Mode configuration)
│   ├── dht11_firebase.c / .h   # Telemetry Mod (DHT11 driver & dynamic PATCH payload builder)
│   └── lamp_firebase.c / .h    # Actuator Mod (HTTP Event Handler & physical GPIO switching)
└── CMakeLists.txt              # Project Build Configuration

## Getting Started
1. Ensure your ESP-IDF environment is properly configured.
2. Clone this repository:
   ```bash
   git clone https://github.com/hafidzsyifa77-lab/firebase_lamp_temperature_control/tree/main
