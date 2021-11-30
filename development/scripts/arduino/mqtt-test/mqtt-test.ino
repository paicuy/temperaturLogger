#include "PubSubClient.h"
#include "ESP8266WiFi.h"

// Testvariable
float x=0;
int tick=0;

// WiFi
const char* ssid = "yourSSID";
const char* wifi_password = "yourPASSWD";

// MQTT
const char* mqtt_server = "yourMQTTserver";
const char* mqtt_username = "client";
const char* mqtt_password = "password";
const char* topic = "test";
const char* clientID = "client0";

// Initialise WiFi + MQTT
WiFiClient wifiClient;
// Listener Port: 1883
PubSubClient client(mqtt_server, 1883, wifiClient); 


// Verbindungsaufbau
void connect_MQTT(){
  Serial.print("WLAN: ");
  Serial.println(ssid);
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Serial Monitor Output
  Serial.print("Verbunden mit der IP-Adresse: ");
  Serial.println(WiFi.localIP());

    if (client.connect(clientID, mqtt_username, mqtt_password)) {
      Serial.println("Verbunden mit MQTT Broker!");
    }
    else {
      Serial.println("Verbindung mit MQTT Broker fehlgeschlagen...");
  }
}


void setup() {
  Serial.begin(9600);
}

void loop() {
  connect_MQTT();
  Serial.setTimeout(500);
  
  x = random(-4,7);

  Serial.print("Nachricht: ");
  Serial.print(x);

  if (client.publish(topic, String(x).c_str())) {
    Serial.println(" gesendet!");
    Serial.println();
  }

  else {
    Serial.println("Verbindungsabbruch mit MQTT Broker");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(topic, String(x).c_str());
  }
}
