// URL: http://forum.arduino.cc/index.php?topic=216835.msg1764333#msg1764333


#include <OneWire.h>
#include <DallasTemperature.h>
#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

//Festlegung des Datenpins für Sensor
#define ONE_WIRE_BUS_1 2
OneWire oneWire_in(ONE_WIRE_BUS_1);
DallasTemperature sensor(&oneWire_in);

//Temperaturmessung
float output;

// WiFi
const char* ssid = "ssid";
const char* wifi_password = "password";

// MQTT
const char* mqtt_server = "ip";
const char* mqtt_username = "username";
const char* mqtt_password = "password";
const char* topic = "test";
const char* clientID = "";

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
  Serial.println();
  Serial.print("Verbunden mit der IP-Adresse: ");
  Serial.println(WiFi.localIP());
  Serial.println();

    if (client.connect(clientID, mqtt_username, mqtt_password)) {
      Serial.println("Verbunden mit MQTT Broker!");
    }
    else {
      Serial.println("Verbindung mit MQTT Broker fehlgeschlagen...");
  }
}

void setup(void)
{
    Serial.begin(9600);
    u8g2.begin();
    Serial.println("Dallas Temperature Control Library Demo - TwoPin_DS18B20");

    
    sensor.begin();
}

void loop(void)
{
  //Sensor
  Serial.println("-------------------------------------------");
  Serial.print("Requesting temperatures...");
  sensor.requestTemperatures();
  Serial.println(" done");

  Serial.print("Temperatur: ");
  output = sensor.getTempCByIndex(0);
  Serial.println(output);

  //Display
  u8g2.firstPage();
  do {
    //keep this the same as it pages through the data generating the screen
    u8g2.setFont(u8g2_font_ncenB14_tr);
    //u8g2.drawStr(0,20,"2.10   C");
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0, 15);
    u8g2.print(output);
    
    
  } while ( u8g2.nextPage() );

  //MQTT
  connect_MQTT();
  Serial.setTimeout(500);
  
  Serial.print("Nachricht: ");
  Serial.print(output);

  if (client.publish(topic, String(output).c_str())) {
    Serial.println(" gesendet!");
    Serial.println();
  }

  else {
    Serial.println("Verbindungsabbruch mit MQTT Broker");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(topic, String(output).c_str()); 

  }
}
