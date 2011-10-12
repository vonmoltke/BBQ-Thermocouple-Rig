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
 * Original version date: Oct 11, 2011
 *
 * This header defines the structure and related constants that the controller
 * uses to report ambient and thermocouple temperatures.
 *
 */
#ifndef REPORT_H_
#define REPORT_H_

#include <inttypes.h>

static const int MAX_NUM_TC = 8;

struct Temperature_Report
{
    float ambient_temperature;
    float thermocouple_temperatures[MAX_NUM_TC];
};

#endif /* REPORT_H_ */
