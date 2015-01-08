//Copyright 2014, Alex Khilko.
//This file is part of LifeChamber which is released under MIT.
//See file LICENSE.TXT or go to www.alexkhilko.com for full license details. 

#ifndef _SENSORS_h
#define _SENSORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RTClib\RTClib.h>

class ChamberDevicesClass
{
 protected:
	void initMoistureProbe();
	void initTemp();
	void initRelay();
	void initClock();
	void initButtons();	
	bool switchPin(bool on, uint8_t pin, bool currentlyOn, uint32_t minWorkTime, uint32_t& outSwitchTime);

	uint8_t leftBtnState, rightBtnState, middleBtnState;
	bool	pumpOn, lightOn, heatOn;
	uint32_t pumpSwitchTime, lightSwitchTime, heatSwitchTime;

 public:
	void init();
	void update();

	float getHumidity();
	float getTemp();
	uint8_t getMoisture();	//0 - 100
	DateTime getTime();
	void switchPump(bool on);
	void switchLight(bool on);
	void switchHeat(bool on);

	bool leftButtonPressed();
	bool rightButtonPressed();
	bool middleButtonPressed();
	bool leftButtonClicked();
	bool rightButtonClicked();
	bool middleButtonClicked();
};

extern ChamberDevicesClass ChamberDevices;

#endif

