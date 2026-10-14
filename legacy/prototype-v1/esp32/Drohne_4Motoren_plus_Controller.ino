#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// ===== WLAN-DATEN =====
const char* ssid     = "Autobots";
const char* password = "XYyKdKyFe3vw8fFR";

// ===== UDP ===== -> Ändern zu Tcp
WiFiUDP Udp;
const unsigned int localUdpPort = 4210;
char incomingPacket[512];

IPAddress pcIP;
unsigned int pcPort = 0;
bool pcConnected = false;

// ===== SENSOR MPU6050 =====
Adafruit_MPU6050 mpu;
bool mpuConnected = false;
sensors_event_t accel, gyro, temp;

// ===== AKTUELLE PIN-BELEGUNG (OHNE DISPLAY) =====
const int M1_IN1 = 12; const int M1_IN2 = 13; // Motor 1: Vorne Links
const int M2_IN1 = 14; const int M2_IN2 = 27; // Motor 2: Vorne Rechts
const int M3_IN1 = 26; const int M3_IN2 = 25; // Motor 3: Hinten Links
const int M4_IN1 = 32; const int M4_IN2 = 33; // Motor 4: Hinten Rechts

bool motorsActive = false;
int speedM1 = 0, speedM2 = 0, speedM3 = 0, speedM4 = 0;
int baseThrust = 0;

// Controller-Werte
float lx = 0, ly = 0, rx = 0, ry = 0;
float lt = 0, rt = 0;
int btnA = 0, btnB = 0, btnX = 0, btnY = 0;
int btnLB = 0, btnRB = 0, btnBACK = 0, btnSTART = 0;
int hat_x = 0, hat_y = 0;

// Timing Intervalle
unsigned long lastTelemetrySend = 0;
const unsigned long telemetryInterval = 50; // Telemetrie alle 50ms an den PC (20 Hz)

void stopAllMotors() {
  speedM1 = speedM2 = speedM3 = speedM4 = 0;
  analogWrite(M1_IN1, 0); analogWrite(M2_IN1, 0);
  analogWrite(M3_IN1, 0); analogWrite(M4_IN1, 0);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // 1. Motor-Pins definieren
  pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT); pinMode(M2_IN2, OUTPUT);
  pinMode(M3_IN1, OUTPUT); pinMode(M3_IN2, OUTPUT);
  pinMode(M4_IN1, OUTPUT); pinMode(M4_IN2, OUTPUT);

  // Motoren auf Vorwärtsrichtung schalten & stoppen
  digitalWrite(M1_IN2, LOW); digitalWrite(M2_IN2, LOW);
  digitalWrite(M3_IN2, LOW); digitalWrite(M4_IN2, LOW);
  stopAllMotors();

  // 2. I2C für MPU-6050 an Pin 21 (SDA) und 22 (SCL) initialisieren
  Wire.begin(21, 22);
  mpuConnected = mpu.begin();
  if (mpuConnected) {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU6050 erfolgreich gestartet!");
  } else {
    Serial.println("WARNUNG: MPU6050 nicht gefunden!");
  }

  // 3. WLAN verbinden
  WiFi.begin(ssid, password);
  Serial.print("Verbinde mit WLAN...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nVerbunden!");
  Serial.print("ESP32 IP-Adresse: ");
  Serial.println(WiFi.localIP());

  // 4. UDP starten
  Udp.begin(localUdpPort);
  Serial.printf("UDP lauscht auf Port %d\n", localUdpPort);
}

void loop() {
  // A) UDP-PAKETE VOM PC EMPFANGEN
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    pcIP = Udp.remoteIP();
    pcPort = Udp.remotePort();
    pcConnected = true;

    int len = Udp.read(incomingPacket, sizeof(incomingPacket) - 1);
    if (len > 0) incomingPacket[len] = '\0';

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, incomingPacket);
    if (!error) {
      lx = doc["lx"] | 0.0f; ly = doc["ly"] | 0.0f;
      rx = doc["rx"] | 0.0f; ry = doc["ry"] | 0.0f;
      lt = doc["lt"] | 0.0f; rt = doc["rt"] | 0.0f;

      btnA = doc["A"] | 0;       btnB = doc["B"] | 0;
      btnX = doc["X"] | 0;       btnY = doc["Y"] | 0;
      btnLB = doc["LB"] | 0;     btnRB = doc["RB"] | 0;
      btnBACK = doc["BACK"] | 0; btnSTART = doc["START"] | 0;
      hat_x = doc["hat_x"] | 0; hat_y = doc["hat_y"] | 0;

      // Start / Stop Logik
      if (btnSTART == 1) motorsActive = true;
      if (btnBACK == 1) {
        motorsActive = false;
        stopAllMotors();
      }
    }
  }

  // B) SENSORDATEN LESEN
  if (mpuConnected) {
    mpu.getEvent(&accel, &gyro, &temp);
  }

  // C) MOTOREN STEUERN
  if (motorsActive) {
    // Rechter Trigger (RT) bestimmt den Schub (0 bis 255 PWM)
    baseThrust = (int)(rt * 255.0f);

    // Neigungsausgleich über MPU6050 ???
    int diffRoll  = mpuConnected ? (int)(accel.acceleration.x * 8.0) : 0;
    int diffPitch = mpuConnected ? (int)(accel.acceleration.y * 8.0) : 0;

    // Berechnung aller 4 Motoren
    speedM1 = constrain(baseThrust + diffRoll + diffPitch, 0, 255); // Vorne Links
    speedM2 = constrain(baseThrust - diffRoll + diffPitch, 0, 255); // Vorne Rechts
    speedM3 = constrain(baseThrust + diffRoll - diffPitch, 0, 255); // Hinten Links
    speedM4 = constrain(baseThrust - diffRoll - diffPitch, 0, 255); // Hinten Rechts

    // Signale an die Pins ausgeben
    analogWrite(M1_IN1, speedM1);
    analogWrite(M2_IN1, speedM2);
    analogWrite(M3_IN1, speedM3);
    analogWrite(M4_IN1, speedM4);
  } else {
    stopAllMotors();
  }

  // D) TELEMETRIE ZURÜCK AN DEN PC SENDEN (50ms Interval)
  unsigned long now = millis();
  if (pcConnected && (now - lastTelemetrySend >= telemetryInterval)) {
    lastTelemetrySend = now;

    StaticJsonDocument<256> outDoc;
    outDoc["run"] = motorsActive;
    outDoc["ax"] = mpuConnected ? round(accel.acceleration.x * 10.0) / 10.0 : 0;
    outDoc["ay"] = mpuConnected ? round(accel.acceleration.y * 10.0) / 10.0 : 0;
    outDoc["az"] = mpuConnected ? round(accel.acceleration.z * 10.0) / 10.0 : 0;
    outDoc["m1"] = speedM1;
    outDoc["m2"] = speedM2;
    outDoc["m3"] = speedM3;
    outDoc["m4"] = speedM4;

    Udp.beginPacket(pcIP, pcPort);
    serializeJson(outDoc, Udp);
    Udp.endPacket();
  }
}