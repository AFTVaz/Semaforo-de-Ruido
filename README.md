
# Semaforo de Ruido (Noise Traffic Light)

# Autors
  - AFTVaz
  - Jawahall

# Summary
The embedded system “Semáforo de Ruído” (Noise Traffic Light) was developed as part of the Final Course Project to monitor ambient noise in 
laboratories at DEETC (Departamento de Engenharia Eletrónica e Telecomunicações e de Computadores) at ISEL (Instituto Superior de Engenharia de Lisboa).  
This Embedded system aims at real-time monitoring of noise levels in unmonitored and not supervised laboratories and classrooms. The device uses an LPC1769 microcontroller, a MAX9814 sensor for audio capture, local traffic-light-style signaling (green/yellow/red), an alarm buzzer, and communicates via ESP-01 (ESP8266) with the ThingsBoard platform using MQTT. There is also a web/mobile application (Ionic + Angular) for real-time visualization and historical analysis.

## Main Features
- Audio acquisition with 12-bit ADC and processing to calculate dB SPL.
- Local indication with 3 levels (green / yellow / red) and audible alarm (buzzer).
- Telemetry sent to ThingsBoard via MQTT, with timestamps synchronized using NTP.
- Web/mobile dashboard (Ionic + Angular) with traffic-light visualization and historical charts.
- Implementation in C with FreeRTOS (tasks, queues, mutexes, notifications).

## Motivation
Noise in educational environments impacts health and learning. This project aims to provide immediate feedback 
and historical data to enable interventions and policies to improve the academic environment.

## Architecture
- **Sensor**: MAX9814 (microphone with AGC) → analog output to LPC1769 ADC.  
- **Local actuation**: LEDs (traffic light) + buzzer.  
- **Connectivity**: ESP-01 via UART → Wi-Fi → NTP / MQTT → ThingsBoard.  
- **Application**: Web/mobile dashboard (Ionic/Angular) consuming data from ThingsBoard.

## Hardware
- **Board**: LPCXpresso-compatible with microcontroller LPC1769 (ARM Cortex-M3)  
- **Sound sensor**: MAX9814 
- **Wi-Fi module**: ESP-01 (ESP8266)
- **Buzzer**: KY-012 (or equivalent)  
- **Traffic light**: 3-color LED module (or 3 separate LEDs)

## Software / Tools
- **IDE**: MCUXpresso IDE (Eclipse-based projects)  
- **RTOS**: FreeRTOS 
- **Frontend**: Ionic + Angular (Noise Monitoring App)  
- **Dashboard/Backend**: ThingsBoard (via MQTT)  
- **Communication**: MQTT, NTP, AT commands for ESP-01

## How to Import the Project in MCUXpresso
1. Open MCUXpresso IDE.  
2. "File" → "Import..." → "Existing Projects into Workspace".  
3. Select the repository root / local clone and import the project.  
4. Check the toolchain / build settings (adjust the SDK path if needed).
