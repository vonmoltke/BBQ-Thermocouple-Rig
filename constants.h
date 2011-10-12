/*
 * Copyright 2011  Wayne Krug
 *
 *  Part of the BBQ Thermocouple Rig project
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
 *
 *  This file defines constants and enumerations used in the thermocouple rig
 *  control program.
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const int8_t RESPONSE_BYTES = 9;
const int8_t RESPONSE_BUFFER_SIZE = 12;
const int8_t ADDRESS_SIZE = 8;
const int8_t TEMP_MSB = 1;
const int8_t TEMP_LSB = 0;
// Minimum execution time of the main loop (ms)
const int LOOP_TIME = 1000;
// 750ms delay required for the ambient temp sensor (12-bit conversion)
const int AMBIENT_SENSOR_DELAY = 750;

enum MESSAGES
{
    END_OF_MESSAGE = 10,
    READ_AMBIENT = 49, // ASCII 1
};

#endif /* CONSTANTS_H_ */
