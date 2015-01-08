//Copyright 2014, Alex Khilko.
//This file is part of LifeChamber which is released under MIT.
//See file LICENSE.TXT or go to www.alexkhilko.com for full license details. 

#ifndef _PRESET_h
#define _PRESET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RTClib\RTClib.h>

class PresetClass
{
 protected:
	 void save();
	 uint8_t moistureGoal;
	 uint8_t tempGoal;
	 uint8_t lightHours;
	 DateTime plantTime;

 public:
	void load();

	uint8_t getMoistureGoal() const;
	uint8_t getTempGoal() const;
	uint8_t getLightHours() const;
	const DateTime& getPlantTime() const;

	void setMoistureGoal(int goal);
	void setTempGoal(int goal);
	void setLightHours(int hours);
	void setPlantTime(DateTime time);
};

extern PresetClass Preset;

#endif

