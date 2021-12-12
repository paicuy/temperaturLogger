// URL: http://forum.arduino.cc/index.php?topic=216835.msg1764333#msg1764333


#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS_1 2

OneWire oneWire_in(ONE_WIRE_BUS_1);

DallasTemperature sensor(&oneWire_in);

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Dallas Temperature Control Library Demo - TwoPin_DS18B20");

    sensor.begin();
}

void loop(void)
{
    Serial.print("Requesting temperatures...");
    sensor.requestTemperatures();
    Serial.println(" done");

    Serial.print("Temperatur: ");
    Serial.println(sensor.getTempCByIndex(0));
}
