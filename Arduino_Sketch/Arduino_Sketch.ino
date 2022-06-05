/*  Projekt: Temperaturlogger by paicuy
 *  Version: 1.02
 *  
 *  Beschreibung:
 *  Ein Microcontroller mit einem DS18B20 Sensor misst alle 500ms die Temperatur, zeigt Sie an einem i2c Display und sendet Sie mittels MQTT alle 5s an einen MQTT-Broker
 * 
 *  Anwender Einstellungen (Geräteeinstellungen (Sensoren Eichung), Wifi und MQTT) finden Sie ab dem 4. Absatz
 */


#include <OneWire.h>              // by Paul Soffregen, Version 2.3.7
#include <DallasTemperature.h>    // by Miles Burton, Version 3.9.0
#include <PubSubClient.h>         // by Nick O'Leary, Version 2.8.0
#include <U8g2lib.h>              // by Oliver, Version 2.32.15
#include <ESP8266WiFi.h>          // by ESP8266 Community (Board lib), Version 3.0.2

//Festlegung des Daten-Pins für Sensor
#define ONE_WIRE_BUS_1 2
OneWire oneWire_in(ONE_WIRE_BUS_1);
DallasTemperature sensor(&oneWire_in);

//Festlegung des Displays
//(https://github.com/olikraus/u8g2/wiki/u8g2setupcpp)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);



//Benutzereinstellungen

    /******************************
     * Beispiel
     * 
     *    Geraetename = "strawberry";       // Der Wert für den Gerätenamen und Raum muss in Anführunszeichen stehen (String)!
     *    Raum =        "DB11.100.1.10";    // Semikolon darf nicht nicht vergeßen werden!
     *    Abweichung =  0.0;                // Dieser Wert ist eine Gleitkommazahl. Hier keine Anführungszeichen und bitte mit "." für die Nachkommastelle verwenden!
     *    
    */

  //Geräteeinstellungen
  String Name = "Strawberry";
  String Raum = "Beispielraum"; 
  float Abweichung = 0.0;

  // WiFi-Zugriff Einstellungen
  const char* ssid = "WLAN_SSID";
  const char* wifi_password = "WLAN_PASSWD";

  // MQTT-Zugriff Einstellungen
  const char* mqtt_server = "192.168.1.2";
  const char* topic = "temperature";

  const char* mqtt_username = "";
  const char* mqtt_password = "";
  const char* clientID = "";

//Ender der Benutzereinstellungen










//Initialisierung der Var
float temperature;
bool stateWifi;
bool stateMQTT;
String payload;

// Initialisierung WiFi + MQTT
WiFiClient wifiClient;
// Listener Port: 1883
PubSubClient client(mqtt_server, 1883, wifiClient); 

// Verbindungsaufbau MQTT
void connect_MQTT(){

  // W-LAN Verbindung aufbauen
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {

  //Funktionsaufruf: Sensor, Display (Wifi False, MQTT False)
  readSensor();
  drawScreen(false,false);
  
  delay(500);
  }

  //Debug: IP-Adresse des Gerätes?
  Serial.println();
  Serial.print("Verbunden mit der IP-Adresse: ");
  Serial.println(WiFi.localIP());
  Serial.println();

    if (client.connect(clientID, mqtt_username, mqtt_password)) {             //client.connect > CAN DELAY BLOCK if FAILED
      Serial.println("Verbunden mit MQTT Broker!");
      readSensor();
      drawScreen(true,true);
    }
    else {
      Serial.println("Verbindung mit MQTT Broker fehlgeschlagen...");
      readSensor();
      drawScreen(true,false);
  }
}

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Setup start");
    
    u8g2.begin();  
    sensor.begin();
}

void loop(void)
{

  //Funktionsaufruf: MQTT
  connect_MQTT();
  
  
  //Payload in Json umwandeln
  payload = payloadJson(Name,Raum,temperature);

      //publish payload zu topic
  if (client.publish(topic,String(payload).c_str())) {                   //client.publish DELAY BLOCK
    // Warte 5 Sekunden bis nächste Nachricht gesendet wird
    // Lese und zeige Temperatur an
    for (int i = 0; i <= 4; i++) {
      readSensor();
      drawScreen(true,true);  
      Serial.setTimeout(500);
    }
  }

  else {
    Serial.println("Verbindungsabbruch mit MQTT Broker");
    client.connect(clientID, mqtt_username, mqtt_password);
    readSensor();
    drawScreen(true,false);
    client.publish(topic,String(payload).c_str());
  }
}

//Funktion 1: OLED Anzeigescript
void drawScreen(bool stateWifi,bool stateMQTT) {
  u8g2.firstPage();
  do {
    // Schriftart
    u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);
    // Ausgabe von String (x,y,String)
    u8g2.drawStr(2,9,"Temperatur");
    u8g2.setFont(u8g2_font_freedoomr25_tn);
    // Position von variable
    u8g2.setCursor(2,46);
    // Ausgabe der Variable Float, 2 Nachkommastellen
    u8g2.print(temperature,2);
    u8g2.setFont(u8g2_font_inb16_mf);
    // Ausgabe des Symbols °
    u8g2.drawGlyph(92,36,0x00b0); // degree
    u8g2.drawStr(104,36,"C");
    u8g2.drawLine(1,56,127,56);
    u8g2.setFont(u8g2_font_u8glib_4_tf);

    //Wifi /& MQTT Working? > Debug
    if (stateWifi) {
      if (stateMQTT) {
        u8g2.drawStr(2,64,"O WIFI");
        u8g2.drawStr(100,64,"O MQTT");
      }
      else {
        u8g2.drawStr(2,64,"O WIFI");
        u8g2.drawStr(100,64,"X MQTT");
      }
    }
    else {
      u8g2.drawStr(2,64,"X WIFI");
      u8g2.drawStr(100,64,"X MQTT");
    }
  } while ( u8g2.nextPage() );
}

//Funktion 2: Temperatur ablesen
void readSensor() {
  sensor.requestTemperatures();
  // Schreibe Temperatur in Variable temperature
  temperature = sensor.getTempCByIndex(0);
  // Eichwert (durch +/- Abweichung)
  temperature = temperature + Abweichung;
  Serial.print("Temperatur: ");
  Serial.println(temperature);
}

//Funktion 3: Payload zu Jason
String payloadJson(String device, String room, float value) {

  String row1 = "\"device\": " ;
  String row2 = "\"room\": ";
  String row3 = "\"temperature\": ";
  String semi = ",";

  row1 = row1 + device + semi;
  row2 = row2 + room + semi;
  row3 = row3 + String(value,3);

  String payload = "{\"device\": \"" + device + "\",\"room\": \"" + room + "\",\"temperature\": "+ value +"}";

  Serial.println(payload);
  return payload;
}
