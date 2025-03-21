# 🎓 Hardware Device Configuration — Computer Science Degree Project  

This repository is part of my final project submitted for my **Computer Science degree at the Federal Rural University of the Semi-arid Region (UFERSA)**.  

## 📚 Project Overview  

In this project, I developed an application responsible for scheduling tasks and managing communication with hardware devices like **ESP32** and **ESP8266**. Users can interact with these devices through a mobile application (also developed by me), enabling them to control smart devices using their smartphones.  

The long-term goal is to create a foundation for building **intelligent devices** that can be easily controlled — for example, turning devices on or off and interacting with them remotely.  

> **⚠ Note:** This repository is dedicated only to the configuration and development of the hardware devices using **Arduino IDE** and **C++**.  

## ⚙️ Key Features of the Hardware Configuration:  
- Connection and communication setup between ESP32/ESP8266 and the backend API.  
- Task scheduling features where devices receive and execute instructions at specified times.  
- Wi-Fi connectivity configuration for real-time control via mobile app.  
- Setup of MQTT or HTTP requests for task execution.  
- Device status reporting and response handling.  

## 🛠️ Technologies Used:  
- **ESP32 / ESP8266** microcontrollers  
- **Arduino IDE** for firmware development  
- **C++** as the programming language  
- Wi-Fi libraries and MQTT/HTTP protocols for communication

## 🚀 How It Works:  
1. ESP32/ESP8266 connects to the configured Wi-Fi network.  
2. The device regularly communicates with the backend API to receive scheduled tasks.  
3. Upon receiving commands, the device executes actions (like turning on or off lights or devices).  
4. The user can send commands through the mobile application, and the device responds in real time.  

## 🔮 Future Improvements:  
- Integration with voice assistants (Google Assistant, Alexa).  
- Expansion of device control types (temperature control, smart sensors).  
- Automatic firmware update mechanism for devices.  
- Improved security for device communication and authentication.  

## 📧 Contact:  
If you have questions or would like to learn more about this project, feel free to reach out to me at [bruno.fariasd1@gmail.com].
