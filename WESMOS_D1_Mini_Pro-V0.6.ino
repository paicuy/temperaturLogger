#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//EINSTELLUNGEN
const char* ssid = "$SSID";
const char* pswd = "$PASSWORT";
const char* mqtt_server = "$RASPI-IP";
const char* topic = "$LOCATION-$INDEX";    // rhis is the [root topic]

//Initialisierung + Deklaration
long timeBetweenMessages = 1000 * 20 * 1;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;
unsigned int t = 0; // Delay kann nicht in die Vergangenheit reisen
unsigned int n = 0;
// Falls doch, bitte unverzüglich bei mir melden.

int status = WL_IDLE_STATUS;     // start WiFi Antenne

//DS18B20
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
  //Festlegung der baud Rate
  Serial.begin(115200);

  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();

  //1
  //Print device ID
  Serial.print("Temperature for device: ");
  Serial.println(n, DEC);
  //Print device temperature
  float test_tempC = sensors.getTempC(tempAddr);
  Serial.print("Temp C: ");
  Serial.print(test_tempC);
  //

  pinMode(BUILTIN_LED, OUTPUT);     // BUILTIN_LED
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}


/************************
 ********* LOOP *********
 ************************/
void loop() {

  //setze delay auf eine Sekunde
  t = 1000;

  //DS18B20
  sensors.requestTemperatures(); // Send the command to get temperatures

  // Loop through each device, print out temperature data
  for (n = 0; n < numberOfDevices; n++) {
    // Search the wire for address
    if (sensors.getAddress(tempAddr, n)) {
      //2
      //Anzahl an Geräten ausgeben:
      Serial.print("Anzahl an Geräten: ");
      Serial.print(numberOfDevices, DEC);

      //Geräte ID aller angeschlossenen Geräte anzeigen:
      for (n = 0; n < numberOfDevices; n++) { // Search the wire for address
        if (sensors.getAddress(tempAddr, n))
        {
          Serial.print("Found device ");
          Serial.print(n, DEC);
          Serial.print(" with address: ");
          printAddress(tempAddr);
          Serial.println();
        } else {
          Serial.print("Found ghost device at ");
          Serial.print(n, DEC);
          Serial.print(" but could not detect address. Check power and cabling");
        }
      }
      //
      tempC = sensors.getTempC(tempAddr);
      temperature += tempC;
    }
  }

  // Hier muss float beachtet werden - Division
  value = temperature / (float) n;

  //MQTT

  //Verbindungsabfrage
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //Payload zu MQTT Server
  long now = millis();
  if (now - lastMsg > timeBetweenMessages ) {
    lastMsg = now;
    ++value;
    String payload = "{\"micros\":";
    payload += micros();
    payload += ",\"counter\":";
    payload += value;
    payload += ",\"client\":";
    payload += composeClientID();
    payload += "}";
    String pubTopic;
    pubTopic += topic ;
    pubTopic += "/";
    pubTopic += composeClientID();
    pubTopic += "/out";
    Serial.print("Publish topic: ");
    Serial.println(pubTopic);
    Serial.print("Publish message: ");
    Serial.println(payload);
    client.publish( (char*) pubTopic.c_str() , (char*) payload.c_str(), true );
  }

  // 1 second Delay
  delay(t);

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

void devMode1() {

}

//MQTT

void setup_wifi() {
  // Verbinde mit Netzwerk
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    if (t >= 500) {
      t -= 500;                                           //Kann Messungen verhindern
      delay(t);
    }
    else {
      delay(t);
    }
    // Messzeit -(time) ?
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

String composeClientID() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  String clientId;
  clientId += "esp-";
  clientId += macToStr(mac);
  return clientId;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = composeClientID() ;
    clientId += "-";
    clientId += String(micros() & 0xff, 16); // to randomise. sort of

    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic, ("connected " + composeClientID()).c_str() , true );
      // ... and resubscribe
      // topic + clientID + in
      String subscription;
      subscription += topic;
      subscription += "/";
      subscription += composeClientID() ;
      subscription += "/in";
      client.subscribe(subscription.c_str() );
      Serial.print("subscribed to : ");
      Serial.println(subscription);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.print(" wifi=");
      Serial.print(WiFi.status());
      Serial.println(" try again in 5 seconds");
      // Wait 1 seconds before retrying
      delay(1000);                                                                  //critical Timeout
    }
  }
}
