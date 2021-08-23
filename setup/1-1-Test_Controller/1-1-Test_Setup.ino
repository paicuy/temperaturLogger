/*
 * TEST - Microcontroller
 * Author: Patrick Stucky
 * 
 * Einstellung:
 * BOARD: LOLIN(WESMOS)D1 Mini Pro
 * Bautrate: 115200 
 * (einstellen unter "Werkzeug" > "Upload Speed"
 * und beim aufrufen des "Serial Monitor")
 * 
 * 
 */

void setup() {
    pinMode(BUILTIN_LED, OUTPUT);
    Serial.begin(115200);
}
 
void loop() {
    digitalWrite(BUILTIN_LED, LOW);
    Serial.println("LED: ON");
    delay(1000);
    digitalWrite(BUILTIN_LED, HIGH);
    Serial.println("LED: OFF");
    delay(1000);
}
