// Smart Parking System (FINAL WITH GRAPH + MANUAL CONTROL)

#define BLYNK_TEMPLATE_ID "TMPL3SS5rI3IZ"
#define BLYNK_TEMPLATE_NAME "Smart Parking System"
#define BLYNK_AUTH_TOKEN "qNzweLs-eF3OIWCXn5HM3plccEMZK_Rh"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- Pins ----------------
#define IR_ENTRY D5
#define IR_EXIT  D6
#define SERVO_PIN D4
#define GREEN_LED D7
#define RED_LED   D8
#define Blue_LED  D3

Servo barrierServo;

// ---------------- WiFi ----------------
const char* ssid = "Sachin";
const char* password = "13032002";

ESP8266WebServer server(80);

// ---------------- Variables ----------------
int totalSlots = 4;
int availableSlots = 4;

int lastEntryState = HIGH;
int lastExitState  = HIGH;

String gateStatus = "Closed";
bool manualMode = false;

// ---------------- BLYNK TIMER ----------------
BlynkTimer timer;

// ---------------- LCD ----------------
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Slots:");
  lcd.print(availableSlots);
  lcd.print("/");
  lcd.print(totalSlots);

  lcd.setCursor(0, 1);
  lcd.print("Gate:");
  lcd.print(gateStatus);
}

// ---------------- BLYNK UPDATE ----------------
void sendToBlynk() {
  Blynk.virtualWrite(V0, availableSlots);

  if (availableSlots > 0)
    Blynk.virtualWrite(V1, "Available");
  else
    Blynk.virtualWrite(V1, "FULL");
}

// ---------------- MANUAL CONTROL ----------------
BLYNK_WRITE(V4) {
  int value = param.asInt();

  if (value == 1) {
    manualMode = true;

    barrierServo.write(0);
    gateStatus = "Manual Open";
    Blynk.virtualWrite(V5, 1);   // GRAPH OPEN

    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  } 
  else {
    manualMode = false;

    barrierServo.write(90);
    gateStatus = "Closed";
    Blynk.virtualWrite(V5, 0);   // GRAPH CLOSE

    digitalWrite(GREEN_LED, LOW);
  }

  updateLCD();
  sendToBlynk();
}

// ---------------- FULL MESSAGE ----------------
void showFullMessage() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Parking FULL!");

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  sendToBlynk();

  delay(2000);
  updateLCD();
}

// ---------------- JSON ----------------
void handleData() {
  String json = "{";
  json += "\"available\":" + String(availableSlots) + ",";
  json += "\"occupied\":" + String(totalSlots - availableSlots) + ",";
  json += "\"gate\":\"" + gateStatus + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

// ---------------- HTML ----------------
String getHTML() {
  String html = "<!DOCTYPE html><html><body>";
  html += "<h1>Smart Parking</h1>";
  html += "<h2>Available: <span id='a'>0</span></h2>";
  html += "<h2>Occupied: <span id='o'>0</span></h2>";
  html += "<h2>Gate: <span id='g'>Closed</span></h2>";

  html += "<script>";
  html += "setInterval(async ()=>{";
  html += "let r=await fetch('/data'); let d=await r.json();";
  html += "a.innerText=d.available;";
  html += "o.innerText=d.occupied;";
  html += "g.innerText=d.gate;";
  html += "},1000);";
  html += "</script>";

  html += "</body></html>";
  return html;
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(IR_ENTRY, INPUT);
  pinMode(IR_EXIT, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(Blue_LED, OUTPUT);

  Wire.begin(D2, D1);

  lcd.init();
  lcd.backlight();

  barrierServo.attach(SERVO_PIN);
  barrierServo.write(90);

  lcd.print("Connecting WiFi");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  lcd.clear();
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  updateLCD();
  sendToBlynk();

  timer.setInterval(1000L, sendToBlynk);

  server.on("/", []() {
    server.send(200, "text/html", getHTML());
  });

  server.on("/data", handleData);

  server.begin();
}

// ---------------- ENTRY ----------------
void handleEntry() {
  if (availableSlots <= 0) {
    showFullMessage();
    return;
  }

  Serial.println("ENTRY DETECTED");

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  delay(300);

  gateStatus = "Open";
  Blynk.virtualWrite(V5, 1);   // GRAPH OPEN
  updateLCD();

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  barrierServo.write(0);
  delay(2000);

  barrierServo.write(90);

  availableSlots--;

  gateStatus = "Closed";
  Blynk.virtualWrite(V5, 0);   // GRAPH CLOSE
  digitalWrite(GREEN_LED, LOW);

  updateLCD();
  sendToBlynk();
}

// ---------------- EXIT ----------------
void handleExit() {
  if (availableSlots >= totalSlots) return;

  Serial.println("EXIT DETECTED");

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  delay(300);

  gateStatus = "Open";
  Blynk.virtualWrite(V5, 1);
  updateLCD();

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  barrierServo.write(0);
  delay(2000);

  barrierServo.write(90);

  availableSlots++;

  gateStatus = "Closed";
  Blynk.virtualWrite(V5, 0);
  digitalWrite(GREEN_LED, LOW);

  updateLCD();
  sendToBlynk();
}

// ---------------- LOOP ----------------
void loop() {
  Blynk.run();
  timer.run();
  server.handleClient();

  if (manualMode) return;

  int entryState = digitalRead(IR_ENTRY);
  int exitState  = digitalRead(IR_EXIT);

  if (lastEntryState == HIGH && entryState == LOW) {
    handleEntry();
  }

  if (lastExitState == HIGH && exitState == LOW) {
    handleExit();
  }

  lastEntryState = entryState;
  lastExitState  = exitState;
}