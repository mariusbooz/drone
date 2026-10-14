#include <Arduino.h>
// ============================================================
// Flight Controller
// ============================================================
// ============================================================
// Setup
// ============================================================

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("=================================");
    Serial.println("     DRONE FLIGHT CONTROLLER");
    Serial.println("=================================");
    Serial.println("[Info] Flight Controller gestartet!");
}

// ============================================================
// Main Flight Loop
// ============================================================

void loop()
{
    Serial.println("[Info] ...");

    delay(1000); // vorerst ca. 100 Hz
}