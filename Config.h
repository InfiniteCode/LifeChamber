//Copyright 2014, Alex Khilko.
//This file is part of LifeChamber which is released under MIT.
//See file LICENSE.TXT or go to www.alexkhilko.com for full license details. 

#ifndef CHAMBER_CONFIG

#define DISPLAY_CONTRAST 28
//#define DISPLAY_MOSI 11
//#define DISPLAY_SCK  13
#define DISPLAY_DC		6
#define DISPLAY_CS		5
#define DISPLAY_RST		4

#define RELAY_PIN_LIGHT 2
#define RELAY_PIN_WATER 3
#define RELAY_PIN_HEAT  10
#define TEMP_PIN		A3
#define MOISTURE_PIN	A2

#define CONTROL_LEFT_PIN   9
#define CONTROL_MIDDLE_PIN 8
#define CONTROL_RIGHT_PIN  7

#define PUMP_MIN_WORKTIME 5
#define MOISTURE_INCREMENT 5
#define MOISTURE_MIN 5
#define MOISTURE_MAX 95

#define HEAT_MIN_WORKTIME 5
#define TEMP_INCREMENT 1
#define TEMP_MIN 5
#define TEMP_MAX 40

#define LIGHT_MIN_WORKTIME 5
#define LIGHT_INCREMENT 1
#define LIGHT_MIN 0
#define LIGHT_MAX 24

#endif