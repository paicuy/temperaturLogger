// URL: http://forum.arduino.cc/index.php?topic=216835.msg1764333#msg1764333


#include <OneWire.h>
#include <DallasTemperature.h>
#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);


#define ONE_WIRE_BUS_1 2

OneWire oneWire_in(ONE_WIRE_BUS_1);

DallasTemperature sensor(&oneWire_in);

float output;

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
 
  delay(500); 

    
}
