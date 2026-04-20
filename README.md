# Smart-Parking-System-IoT-Blynk-
An IoT-based Smart Parking System built using ESP8266 (NodeMCU) that automates vehicle entry/exit, monitors parking availability in real-time, and provides remote control via the Blynk Cloud dashboard.

📌 Features <br>
🚦 Automatic Gate Control using IR sensors and servo motor <br>
📊 Real-time Slot Monitoring on LCD and Blynk dashboard <br>
📱 Remote Gate Control via Blynk mobile/web app<br>
📈 Gate Activity Graph (Open/Close timing using SuperChart)<br>
🌐 Web Dashboard hosted on ESP8266<br>
🔴🟢 LED Indicators for parking status (Full/Available) <br>

🛠️ Technologies Used <br>
Hardware: ESP8266 (NodeMCU), IR Sensors, Servo Motor, LEDs, LCD (I2C)<br>
Software: Arduino IDE<br>
Cloud Platform: Blynk IoT<br>
Programming Language: C++ (Embedded)<br>

⚙️ Working Principle<br>
IR sensors detect vehicle entry and exit<br>
Available parking slots are updated dynamically<br>
Servo motor controls the gate automatically<br>
Data is displayed on:<br>
LCD (local)<br>
Blynk Dashboard (cloud)<br>
Manual override allows gate control via mobile app<br>
Gate activity is logged and visualized using a graph<br>

📊 Blynk Dashboard<br>
V0: Available Slots<br>
V1: Parking Status<br>
V4: Manual Gate Control (Switch)<br>
V5: Gate Activity Graph<br>

🚀 Setup Instructions<br>
Install required libraries:<br>
Blynk<br>
ESP8266WiFi<br>
LiquidCrystal_I2C<br>
Servo<br>
Configure:<br>
WiFi credentials<br>
Blynk Template ID & Auth Token<br>
Upload code to NodeMCU<br>
Setup Blynk Dashboard:<br>
Create datastreams (V0, V1, V4, V5)<br>
Add widgets (Label, Switch, SuperChart)<br>
