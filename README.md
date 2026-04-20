# Smart-Parking-System-IoT-Blynk-
An IoT-based Smart Parking System built using ESP8266 (NodeMCU) that automates vehicle entry/exit, monitors parking availability in real-time, and provides remote control via the Blynk Cloud dashboard.

📌 Features 
🚦 Automatic Gate Control using IR sensors and servo motor
📊 Real-time Slot Monitoring on LCD and Blynk dashboard
📱 Remote Gate Control via Blynk mobile/web app
📈 Gate Activity Graph (Open/Close timing using SuperChart)
🌐 Web Dashboard hosted on ESP8266
🔴🟢 LED Indicators for parking status (Full/Available)

🛠️ Technologies Used
Hardware: ESP8266 (NodeMCU), IR Sensors, Servo Motor, LEDs, LCD (I2C)
Software: Arduino IDE
Cloud Platform: Blynk IoT
Programming Language: C++ (Embedded)

⚙️ Working Principle
IR sensors detect vehicle entry and exit
Available parking slots are updated dynamically
Servo motor controls the gate automatically
Data is displayed on:
LCD (local)
Blynk Dashboard (cloud)
Manual override allows gate control via mobile app
Gate activity is logged and visualized using a graph

📊 Blynk Dashboard
V0: Available Slots
V1: Parking Status
V4: Manual Gate Control (Switch)
V5: Gate Activity Graph

🚀 Setup Instructions
Install required libraries:
Blynk
ESP8266WiFi
LiquidCrystal_I2C
Servo
Configure:
WiFi credentials
Blynk Template ID & Auth Token
Upload code to NodeMCU
Setup Blynk Dashboard:
Create datastreams (V0, V1, V4, V5)
Add widgets (Label, Switch, SuperChart)
