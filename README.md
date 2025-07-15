
##### 🌊 ESP32 Dual-Point Water Level Monitoring System

This project is a complete water level monitoring solution using ESP32 microcontrollers, ultrasonic sensors, and a SIM800L GSM module. The system monitors water levels at two different points (inner and outer), communicates locally using ESP-NOW, and sends the data remotely via GSM to a middle server, which then forwards it to a Glitch server for visualization.

## 🚰 System Overview

The system architecture is designed to ensure reliable monitoring and reporting from two critical points:

- **Inner Point (Transmitter):** Measures its own water level and sends the data to the outer point.
- **Outer Point (Receiver):** Measures its own water level, receives data from the inner point, combines both, and sends the data to a central server via GSM.

## ⚙️ Detailed System Structure

### 📍 Inner Point (Transmitter)

- **Components:**
  - ESP32-DEVKIT-V1
  - Ultrasonic sensor (HC-SR04)
  - 5V voltage regulator circuit (L7805CV)
  - TVS diode and basic protections
  - Custom PCB design

- **Functionality:**
  - Continuously measures local water level.
  - Communicates with the outer point using ESP-NOW (Wi-Fi peer-to-peer protocol).
  - Does not connect directly to the internet.

### 📍 Outer Point (Receiver)

- **Components:**
  - ESP32-DEVKIT-V1
  - Ultrasonic sensor (HC-SR04)
  - SIM800L GSM module
  - Voltage regulators (L7805CV for 5V, LM317 configured to 4.2V for GSM)
  - TVS diodes, capacitors, and zener diodes for protection
  - Custom PCB design

- **Functionality:**
  - Measures its own local water level.
  - Receives water level data from the inner point using ESP-NOW.
  - Combines both readings into one data packet.
  - Uses the SIM800L module to send data via GSM to a middle server.
  - Middle server forwards the data to a Glitch server for real-time monitoring.

## 🔄 Data Flow and Communication

Inner Point (ESP32 + Sensor)
⬇️ ESP-NOW
Outer Point (ESP32 + Sensor + GSM)
⬇️ GSM (SIM800L)
Middle Server
⬇️ HTTP/API
Glitch Server (final visualization)

- **Local communication (ESP-NOW):** Low-power, direct, fast data transfer between ESP32 modules without needing Wi-Fi infrastructure.
- **Remote communication (SIM800L):** Mobile data network to reach the middle server anywhere.

## ⚡ Power Management

- Both points include voltage regulation to ensure stable operation.
- **5V Regulator (L7805CV):** Provides 5V to ESP32 and sensors.
- **4.2V Regulator (LM317 with Zener reference):** Supplies stable power to SIM800L GSM module, preventing resets and drops.
- TVS diodes and capacitors added for noise filtering and surge protection.

## 💡 Features

✅ Dual point monitoring system (inner + outer)  
✅ ESP-NOW wireless local communication  
✅ GSM remote data transmission via SIM800L  
✅ Custom-designed PCBs with protections  
✅ Fully autonomous, no external Wi-Fi or network needed locally  
✅ Modular design for easy maintenance
