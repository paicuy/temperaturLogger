
#include <OneWire.h>
#include <DallasTemperature.h>

// Datenleitung mit ESP8266 an Pin GPIO 4

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Anzahl an Sensoren gefunden:
int numberOfDevices;

// Gerät-Adress-Variable
DeviceAddress gav; 

void setup(){
  // Serieller Port wird überprüft
  Serial.begin(115200);
  
  // Starten der lib
  sensors.begin();
  
  // Anzahl der Geräte wird gesucht
  numberOfDevices = sensors.getDeviceCount();
  
  // Das Gerät wird localisiert
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Zeige die Geräteadresse(n)
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(sensors.getAddress(gav, i)){
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(gav);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}

void loop(){ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(sensors.getAddress(gav, i)){
      // Output the device ID
      Serial.print("Temperature for device: ");
      Serial.println(i,DEC);
      // Print the data
      float tempC = sensors.getTempC(gav);
      Serial.print("Temp C: ");
      Serial.print(tempC);
    }
  }
  delay(30000);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}
