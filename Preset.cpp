//Copyright 2014, Alex Khilko.
//This file is part of LifeChamber which is released under MIT.
//See file LICENSE.TXT or go to www.alexkhilko.com for full license details. 

#include "Preset.h"
#include "Config.h"

#include "EEPROM\EEPROM.h"

void PresetClass::load()
{
	moistureGoal = constrain(EEPROM.read(0), MOISTURE_MIN, MOISTURE_MAX);
	tempGoal = constrain(EEPROM.read(1), TEMP_MIN, TEMP_MAX);
	lightHours = constrain(EEPROM.read(2), LIGHT_MIN, LIGHT_MAX);

	uint32_t timestamp; uint8_t* timestampBytes = (uint8_t*)&timestamp;
	timestampBytes[0] = EEPROM.read(3);
	timestampBytes[1] = EEPROM.read(4);
	timestampBytes[2] = EEPROM.read(5);
	timestampBytes[3] = EEPROM.read(6);
	plantTime = DateTime(timestamp);
}

void PresetClass::save()
{
	EEPROM.write(0, moistureGoal);
	EEPROM.write(1, tempGoal);
	EEPROM.write(2, lightHours);

	uint32_t timestamp = plantTime.unixtime(); uint8_t* timestampBytes = (uint8_t*)&timestamp;
	EEPROM.write(3, timestampBytes[0]);
	EEPROM.write(4, timestampBytes[1]);
	EEPROM.write(5, timestampBytes[2]);
	EEPROM.write(6, timestampBytes[3]);
}

uint8_t PresetClass::getMoistureGoal() const
{
	return moistureGoal;
}

uint8_t PresetClass::getTempGoal() const
{
	return tempGoal;
}

uint8_t PresetClass::getLightHours() const
{
	return lightHours;
}

const DateTime& PresetClass::getPlantTime() const
{
	return plantTime;
}

void PresetClass::setPlantTime(DateTime time)
{
	plantTime = time;
	save();
}

void PresetClass::setMoistureGoal(int goal)
{
	moistureGoal = constrain(goal, MOISTURE_MIN, MOISTURE_MAX);
	save();
}

void PresetClass::setTempGoal(int goal)
{
	tempGoal = constrain(goal, TEMP_MIN, TEMP_MAX);
	save();
}

void PresetClass::setLightHours(int hours)
{
	lightHours = constrain(hours, LIGHT_MIN, LIGHT_MAX); 
	save();
}


PresetClass Preset;

