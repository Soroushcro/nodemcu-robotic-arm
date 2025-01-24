# Robotic Arm Control System

A sophisticated robotic arm system powered by NodeMCU ESP8266 microcontroller and controlled via a web interface. This system allows precise manipulation of a 7-servo robotic arm with real-time feedback through an OLED display.

## Features

- **7-Servo Control**: Includes servos for the base, shoulder (2), elbow, wrist, wrist rotation, and gripper.
- **Web Interface**: Control the arm's movements using sliders or by specifying XYZ coordinates.
- **OLED Display**: Displays real-time system status, including coordinates, arm height, and WiFi access point IP.
- **Forward and Inverse Kinematics**: Supports XYZ coordinate inputs for automatic joint angle calculations.

## Hardware Components

- **7 Servos**:
  - Base
  - 2 Shoulders
  - Elbow
  - Wrist
  - Wrist Rotation
  - Gripper
- **Microcontroller**: NodeMCU ESP8266
- **Display**: OLED Display (SSD1306)

## Power Requirements

- **Input Voltage**: 5V
- **Current**: 3A

> ⚠️ **Ensure strict adherence to these specifications to prevent damage to the hardware.**

## OLED Display Features

The OLED display provides key real-time information:
- Current arm height
- X, Y, Z coordinates
- WiFi Access Point IP address
- System status messages

## Control Interface

### Sliders Tab
- Individual servo controls:
  - **Base Rotation**: 0°–180°
  - **Shoulder**: 0°–180°
  - **Elbow**: 30°–180°
  - **Wrist**: 0°–180°
  - **Wrist Rotation**: Left/Right buttons
  - **Gripper**: Open/Close/Grip buttons

### XYZ Position Tab
- Input desired X, Y, Z coordinates (in mm) for the robotic arm.
- The system calculates and executes the necessary joint movements.

## Example Movements

Here are some examples of the robotic arm in action:

![Example Movement 1](/images/arm%20(1).jpg)
![Example Movement 2](/images/arm%20(2).jpg)
![Example Movement 3](/images/arm%20(3).jpg)

## ARM MODEL

The Model used for this project is available here! [Robotic Arm With 7 Servos by jjshortcut](https://github.com/Soroushcro/nodemcu-robotic-arm)

### Get Started
1. Clone the repository:
   ```bash
   git clone https://github.com/Soroushcro/nodemcu-robotic-arm.git
   ```
2. Follow the documentation to assemble the hardware and configure the web interface.

Feel free to contribute by opening issues or pull requests!
