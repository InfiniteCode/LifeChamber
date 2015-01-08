//Copyright 2014, Alex Khilko.
//This file is part of LifeChamber which is released under MIT.
//See file LICENSE.TXT or go to www.alexkhilko.com for full license details. 

#include "ChamberDevices.h"
#include "Config.h"

#include <DHT\DHT.h>
#include <Wire\Wire.h>
#include <RTClib\RTClib.h>

RTC_DS1307 RTC;
DHT tempSensor(TEMP_PIN, DHT11);

#define BUTTON_PROCESSOR(var, pin) { if (var == 2) var = 0; if (digitalRead(pin) == LOW) var = 1; else var = var == 1 ? 2 : 0; }

void ChamberDevicesClass::init()
{
	//Clock has to be initialized always first
	initClock();
	//Then everything else
	initRelay();
	initMoistureProbe();
	initTemp();
	initButtons();
}

void ChamberDevicesClass::initClock()
{
	RTC.begin();

	if (!RTC.isrunning()) {
		Serial.println("RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		// Consider adding functionality to set time and date
		RTC.adjust(DateTime(__DATE__, __TIME__));
	}
	else
		Serial.println("RTC is running.");
}

DateTime ChamberDevicesClass::getTime()
{
	Serial.print(RTC.now().minute());
	Serial.print(":");
	Serial.println(RTC.now().second());
	return RTC.now();
}

void ChamberDevicesClass::initRelay()
{
	pinMode(RELAY_PIN_LIGHT, OUTPUT);
	pinMode(RELAY_PIN_WATER, OUTPUT);
	pinMode(RELAY_PIN_HEAT, OUTPUT);
	digitalWrite(RELAY_PIN_LIGHT, LOW);
	digitalWrite(RELAY_PIN_WATER, LOW);
	digitalWrite(RELAY_PIN_HEAT, LOW);
	pumpOn = false;
	heatOn = false;
	lightOn = false;
	pumpSwitchTime = 0;
	lightSwitchTime = 0;
	heatSwitchTime = 0;
}

bool ChamberDevicesClass::switchPin(bool on, uint8_t pin, bool currentlyOn, uint32_t minWorkTime, uint32_t& outSwitchTime)
{
	if (on == currentlyOn) return on;

	uint32_t curTime = getTime().unixtime();
	if (outSwitchTime != 0 && curTime - outSwitchTime < minWorkTime) return currentlyOn;

	outSwitchTime = curTime;
	digitalWrite(pin, on ? HIGH : LOW);
	return on;
}

void ChamberDevicesClass::switchLight(bool on)
{
	lightOn = switchPin(on, RELAY_PIN_LIGHT, lightOn, LIGHT_MIN_WORKTIME, lightSwitchTime);
}

void ChamberDevicesClass::switchPump(bool on)
{
	pumpOn = switchPin(on, RELAY_PIN_WATER, pumpOn, PUMP_MIN_WORKTIME, pumpSwitchTime);
}

void ChamberDevicesClass::switchHeat(bool on)
{
	heatOn = switchPin(on, RELAY_PIN_HEAT, heatOn, HEAT_MIN_WORKTIME, heatSwitchTime);
}

void ChamberDevicesClass::initButtons()
{
	pinMode(CONTROL_LEFT_PIN, INPUT_PULLUP);
	pinMode(CONTROL_MIDDLE_PIN, INPUT_PULLUP);
	pinMode(CONTROL_RIGHT_PIN, INPUT_PULLUP);

	leftBtnState = 0;
	rightBtnState = 0;
	middleBtnState = 0;
}

void ChamberDevicesClass::initTemp()
{
	tempSensor.begin();
}

float ChamberDevicesClass::getHumidity()
{
	return tempSensor.readHumidity();
}

float ChamberDevicesClass::getTemp()
{
	// Read temperature in Celsius
	return tempSensor.readTemperature();
}

void ChamberDevicesClass::initMoistureProbe()
{
	pinMode(MOISTURE_PIN, INPUT);
}

uint8_t ChamberDevicesClass::getMoisture()
{
	return map(analogRead(MOISTURE_PIN), 1023, 0, 0, 100);
}

bool ChamberDevicesClass::leftButtonClicked()
{
	return leftBtnState == 2;
}

bool ChamberDevicesClass::rightButtonClicked()
{
	return rightBtnState == 2;
}

bool ChamberDevicesClass::middleButtonClicked()
{
	return middleBtnState == 2;
}

bool ChamberDevicesClass::leftButtonPressed()
{
	return leftBtnState == 1;
}

bool ChamberDevicesClass::rightButtonPressed()
{
	return rightBtnState == 1;
}

bool ChamberDevicesClass::middleButtonPressed()
{
	return middleBtnState == 1;
}

void ChamberDevicesClass::update()
{
	BUTTON_PROCESSOR(leftBtnState, CONTROL_LEFT_PIN);
	BUTTON_PROCESSOR(rightBtnState, CONTROL_RIGHT_PIN);
	BUTTON_PROCESSOR(middleBtnState, CONTROL_MIDDLE_PIN);
}

ChamberDevicesClass ChamberDevices;

