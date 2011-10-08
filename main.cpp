#include <WProgram.h>
#include <HardwareSerial.h>
#include "OneWire.h"

OneWire temp_sensor(12);

void setup()
{
    // Make pin 13 an output
    //pinMode(13, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    byte address[8]; // Address of the temp sensor
    boolean found_sensor = false;

    digitalWrite(13, HIGH);

    if (!found_sensor)
    {
        if (temp_sensor.search(address))
        {
            Serial.print("R=");
            for (int i = 0; i < 8; ++i)
            {
                Serial.print(address[i], HEX);
                Serial.print(" ");
            }
            Serial.print("\n");

            temp_sensor.reset_search();
        }
        else
        {
            Serial.print("Could not find temp sensor");
        }
    }

    digitalWrite(13, LOW);
    delay(2000);
}

int main(void)
{
    init();

    setup();

    for (;;)
    {
        loop();
    }

    return (0);
}
