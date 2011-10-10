/*
 * Copyright 2011  Wayne Krug
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  OneWire.cpp obtained from <http://www.pjrc.com/teensy/td_libs_OneWire.html>
 *  and written by Jim Studt and others.  See OneWire.cpp for full copyright
 *  details.
 */

#include <WProgram.h>
#include <HardwareSerial.h>
#include "OneWire.h"

#define DEBUG

const int8_t RESPONSE_BYTES = 9;
const int8_t RESPONSE_BUFFER_SIZE = 12;
const int8_t ADDRESS_SIZE = 8;
const int8_t TEMP_MSB = 1;
const int8_t TEMP_LSB = 0;

OneWire g_temp_sensor(12);

// Indicates the controller found a valid address sensor on the bus
boolean g_found_sensor = false;
byte g_address[ADDRESS_SIZE]; // Address of the temp sensor

float inline c_to_f(float p_celsius)
{
    return(((p_celsius * 9.0f) / 5.0f) + 32.0f);
}

void setup()
{
    // Make pin 13 an output
    pinMode(13, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    // Illuminate the port 13 LED during operations
    digitalWrite(13, HIGH);

    // Issue the search command to get the device address.
    // This command must always precede the function commands below.
    if(!g_found_sensor)
    {
        if (g_temp_sensor.search(g_address))
        {
#ifdef DEBUG
            Serial.print("R=");
            for (int i = 0; i < ADDRESS_SIZE; ++i)
            {
                Serial.print(g_address[i], HEX);
                Serial.print(" ");
            }
            Serial.print("\n");
#endif
            g_temp_sensor.reset_search();
            g_found_sensor = true;
        }
#ifdef DEBUG
        else
        {
            Serial.print("Could not find temp sensor");
        }
#endif
    }

    if(g_found_sensor)
    {
        byte data[RESPONSE_BUFFER_SIZE]; // Buffer for the temperature reading

        // Already found the sensor.  Get a temperature reading.
        g_temp_sensor.reset(); // Reset the device
        g_temp_sensor.select(g_address); // Select the temp sensor
        g_temp_sensor.write(0x44); // Initiate the temperature reading
        delay(750); // Wait 750ms (required for 12-bit conversion)
        g_temp_sensor.reset(); // Reset the device
        g_temp_sensor.select(g_address); // Select the temp sensor
        g_temp_sensor.write(0xBE); // Issue the read command

        // Read the temp value from the scratchpad
        for (int i = 0; i < RESPONSE_BYTES; ++i)
        {
            data[i] = g_temp_sensor.read();
        }

        // Create the binary representation of the temperature
        int16_t binary_temperature = data[1];
        binary_temperature = binary_temperature << 8;
        binary_temperature += data[0];
#ifdef DEBUG
        Serial.print("Binary temperature = ");
        Serial.print(binary_temperature);
        Serial.print("\n");
#endif

        float temperature = (float)binary_temperature * 0.0625;
        Serial.print("Temperature (C) = ");
        Serial.print(temperature);
        Serial.print("\n");
        Serial.print("Temperature (F) = ");
        Serial.print(c_to_f(temperature));
        Serial.print("\n");
    }

    digitalWrite(13, LOW);
    delay(1000);
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
