# ROBO-PET

The **Pet Robot** is a smart ESP32-based autonomous robot designed to explore surfaces safely while displaying expressive OLED eyes. It avoids obstacles using an ultrasonic sensor and ensures it doesn’t fall off edges using an IR surface detection sensor. Its behavior mimics that of a curious, cautious pet!

---

## Features

- **Obstacle Avoidance** using Ultrasonic Sensor  
- **Surface Edge Detection** using IR Sensor  
- **Animated OLED Eyes** that respond to motion and environment  
- **Smooth Motor Control** with PWM (adjustable RPM)  
- **Smart Navigation Logic** with intelligent turn and retry paths

---

## Components Used

| Component              | Quantity |
|------------------------|----------|
| ESP32 Dev Board        | 1        |
| L298N Motor Driver     | 1        |
| Ultrasonic Sensor (HC-SR04) | 1    |
| IR Sensor Module       | 2        |
| BO Motors              | 2        |
| OLED Display (SSD1306) | 1        |
| Jumper Wires           | As needed |
| Power Supply (7.4V–12V Battery) | 1 |

---

## Wiring Connection

| Component              | Connect To ESP32 Pin |
|-----------------------|----------------------|
| **ENA (L298N)**        | D14                 |
| **IN1 (L298N)**        | D27                 |
| **IN2 (L298N)**        | D26                 |
| **IN3 (L298N)**        | D25                 |
| **IN4 (L298N)**        | D33                 |
| **ENB (L298N)**        | D12                 |
| **TRIG (Ultrasonic)**  | D5                  |
| **ECHO (Ultrasonic)**  | D18                 |
| **IR Surface Sensor**  | D4                  |
| **IR Eye Sensor**      | D34                 |
| **OLED SDA**           | D21                 |
| **OLED SCL**           | D22                 |
| **GND (All Modules)**  | GND                 |
| **VCC (All Modules)**  | 5V (or External)    |

> ⚠Ensure all grounds (ESP32, sensors, motor driver) are connected together.

---

## Logic Flow

1. **Forward Motion** until an obstacle or surface edge is detected.
2. **If Obstacle Detected:**
   - Stop → Reverse → Turn Left → Check surface.
   - If surface not found → Turn Right → Move forward if clear.
3. **If No Surface Detected (Edge):**
   - Stop → Reverse → Try turning left and right until surface is found.
4. **OLED Eyes:**
   - Show idle animation by default.
   - Change eyes based on IR motion detection.

---

## 🛠Libraries Required

Install these libraries via Arduino Library Manager:

- `NewPing` — For ultrasonic sensor
- `U8g2` — For OLED animations


---

## Adjustments

- You can tune the motor speed by changing `speedValue` in the code (0–255).
- Adjust the delay timings in the navigation logic for smoother or faster reaction.
- OLED expressions can be customized to match your robot's personality.









