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
#include "constants.h"

#define DEBUG

OneWire g_temp_sensor(12);
// Flag used to indcate setup failed and the loop cannot run
boolean g_setup_failed = true;
byte g_address[ADDRESS_SIZE]; // Address of the temp sensor

#ifdef DEBUG
float inline c_to_f(float p_celsius)
{
    return(((p_celsius * 9.0f) / 5.0f) + 32.0f);
}
#endif

static void f_request_ambient_temp(byte *p_address)
{
    g_temp_sensor.reset(); // Reset the device
    g_temp_sensor.select(p_address); // Select the temp sensor
    g_temp_sensor.write(0x44); // Initiate the temperature reading
}

static int16_t f_read_ambient_temp(byte *p_address)
{
    byte data[RESPONSE_BUFFER_SIZE]; // Buffer for the temperature reading

    g_temp_sensor.reset(); // Reset the device
    g_temp_sensor.select(p_address); // Select the temp sensor
    g_temp_sensor.write(0xBE); // Issue the read command

    // Read the temp value from the scratchpad
    for (int i = 0; i < RESPONSE_BYTES; ++i)
    {
        data[i] = g_temp_sensor.read();
    }

    // Create the binary representation of the temperature
    int16_t temperature = data[TEMP_MSB];
    temperature = temperature << 8;
    temperature += data[TEMP_LSB];

    return(temperature);
}

void setup()
{
#ifdef DEBUG
    // Make pin 13 an output
    pinMode(13, OUTPUT);
#endif
    Serial.begin(9600);

    // Get the address of the ambient temperature sensor
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
    }
    else
    {
#ifdef DEBUG
        Serial.print("Could not find temp sensor");
#endif
        // Abort setup if the sensor isn't there
        return;
    }

    g_setup_failed = false;
}

void loop()
{
    // Abort the loop if setup failed
    if(g_setup_failed) return;

    static unsigned long ambient_temp_request_time;
    static unsigned long ambient_temp_finish_time;
    static boolean request_ambient_temp = false;
    static boolean request_pending = false;

    static int16_t binary_temperature = 0;

    // Check the serial bus for messages
    if(Serial.available() > 0)
    {
        // Read the message
        int message = Serial.read();

#ifdef DEBUG
        Serial.print("Message received: ");
        Serial.println(message, DEC);
#endif

        // Only do something if the message is valid
        switch(message)
        {
        case READ_AMBIENT:
            request_ambient_temp = true;
            break;
        case END_OF_MESSAGE:
        default:
            break;
        }
    }

    if(request_ambient_temp)
    {
        // Get an ambient temperature reading.
        f_request_ambient_temp(g_address);
        ambient_temp_request_time = millis();
        ambient_temp_finish_time = ambient_temp_request_time + AMBIENT_SENSOR_DELAY;
        request_ambient_temp = false;
        request_pending = true;
#ifdef DEBUG
        digitalWrite(13, HIGH);
#endif
    }

    // Read the temperature if the sensor is ready
    if(request_pending && (ambient_temp_finish_time < millis()))
    {
        binary_temperature = f_read_ambient_temp(g_address);
        request_pending = false;
#ifdef DEBUG
        digitalWrite(13, LOW);
        Serial.print("Binary temperature = ");
        Serial.print(binary_temperature);
        Serial.print("\n");

        float temperature = (float) binary_temperature * 0.0625;
        Serial.print("Temperature (C) = ");
        Serial.print(temperature);
        Serial.print("\n");
        Serial.print("Temperature (F) = ");
        Serial.print(c_to_f(temperature));
        Serial.print("\n");
#endif
    }
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
