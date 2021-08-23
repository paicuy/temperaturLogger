/*
 * Author: Patrick Stucky
 * 
 * Einstellung:
 * baut: 115200
 * Board: Lolin(Wesmos) D1 Mini Pro
 * Bibliothek: OneWire, DallasTemperature
 * 
 */



#include <OneWire.h>
#include <DallasTemperature.h>


//Wahl des Pins
const int PIN = 4;
int numberOfDevices;

float tempC;
float temperature;

OneWire oneWire(PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress tempAddr;


/************************
 ******** SETUP *********
 ************************/
void setup() {
  Serial.begin(115200);

  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();

  //Anzahl an Geräten ausgeben:
  Serial.print("Anzahl an Geräten: ");
  Serial.print(numberOfDevices, DEC);

  //Geräte ID aller angeschlossenen Geräte anzeigen:
  for (int i = 0; i < numberOfDevices; i++) { // Search the wire for address
    if (sensors.getAddress(tempAddr, i))
    {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempAddr);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}


/************************
 ********* LOOP *********
 ************************/
void loop() {

  sensors.requestTemperatures(); // Send the command to get temperatures

  // Loop through each device, print out temperature data
  for (int i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if (sensors.getAddress(tempAddr, i)) {

      //Print device ID
      Serial.print("Temperature for device: ");
      Serial.println(i, DEC);
      //Print device temperature
      float test_tempC = sensors.getTempC(tempAddr);
      Serial.print("Temp C: ");
      Serial.print(test_tempC);

      tempC = sensors.getTempC(tempAddr);
      temperature = temperature + tempC;
    }
  }

  temperature = temperature / 2.0;

  // 1 second delay
  delay(1000);

}


/***************************
 ******** Functions ********
 ***************************/
// Funktion Ausgabe Geräte-Adresse
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
