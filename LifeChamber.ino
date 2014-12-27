
#include "Preset.h"
#include "Config.h"

#include <EEPROM.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <dht.h>
#include <Wire.h>
#include <RTClib.h>


#include "ChamberDisplay.h"
#include "ChamberDevices.h"

enum DisplayState
{
	DS_MAINSCREEN,
	DS_MOISTURE_CONFIG,
	DS_TEMP_CONFIG,
	DS_GROWTIME_CONFIG,
	DS_LIGHT_CONFIG,
	DS_CLOCK_CONFIG //Not implemented!
} displayState;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
	Preset.load();
	ChamberDisplay.init();
	ChamberDevices.init();
	displayState = DS_MAINSCREEN;

}

void nextScreen()
{
	switch (displayState)
	{
	case DS_MAINSCREEN: displayState = DS_MOISTURE_CONFIG; break;
	case DS_MOISTURE_CONFIG: displayState = DS_TEMP_CONFIG; break;
	case DS_TEMP_CONFIG: displayState = DS_LIGHT_CONFIG; break;
	case DS_LIGHT_CONFIG: displayState = DS_GROWTIME_CONFIG; break;
	case DS_GROWTIME_CONFIG: displayState = DS_MAINSCREEN; break;
	}
}

void control()
{
	//Control moisture
	uint8_t moistureLevel = ChamberDevices.getMoisture();
	ChamberDevices.switchPump(moistureLevel < Preset.getMoistureGoal());

	//Control temperature
	float temperature = ChamberDevices.getTemp();
	ChamberDevices.switchHeat(temperature < Preset.getTempGoal());

	//Control light
	int lightHours = Preset.getLightHours();
	if (lightHours == LIGHT_MAX) ChamberDevices.switchLight(true); else
	if (lightHours == LIGHT_MIN) ChamberDevices.switchLight(false); else
	{
		uint8_t curHour = ChamberDevices.getTime().hour();
		uint8_t sunrise = 12 - lightHours / 2;
		uint8_t sunset = sunrise + lightHours - 1; 
		ChamberDevices.switchLight(curHour >= sunrise && curHour <= sunset);
	}
}

void loop()
{
	ChamberDevices.update();
	control();

	switch (displayState)
	{
	case DS_MAINSCREEN: 
		{
			if (ChamberDevices.middleButtonClicked()) nextScreen();
			ChamberDisplay.drawMain();
		}break;
	case DS_MOISTURE_CONFIG: 
		{
			if (ChamberDevices.middleButtonClicked()) nextScreen();
			if (ChamberDevices.leftButtonClicked()) Preset.setMoistureGoal(Preset.getMoistureGoal() - MOISTURE_INCREMENT);
			if (ChamberDevices.rightButtonClicked()) Preset.setMoistureGoal(Preset.getMoistureGoal() + MOISTURE_INCREMENT);
			ChamberDisplay.drawMoistureConfig(); break;
		} break;
	case DS_TEMP_CONFIG:
		{
			if (ChamberDevices.middleButtonClicked()) nextScreen();
			if (ChamberDevices.leftButtonClicked()) Preset.setTempGoal(Preset.getTempGoal() - TEMP_INCREMENT);
			if (ChamberDevices.rightButtonClicked()) Preset.setTempGoal(Preset.getTempGoal() + TEMP_INCREMENT);
			ChamberDisplay.drawTempConfig(); break;
	} break;
	case DS_LIGHT_CONFIG:
		{
			if (ChamberDevices.middleButtonClicked()) nextScreen();
			if (ChamberDevices.leftButtonClicked()) Preset.setLightHours(Preset.getLightHours() - LIGHT_INCREMENT);
			if (ChamberDevices.rightButtonClicked()) Preset.setLightHours(Preset.getLightHours() + LIGHT_INCREMENT);
			ChamberDisplay.drawLightConfig(); break;
		} break;
	case DS_GROWTIME_CONFIG:
		{
			if (ChamberDevices.middleButtonClicked()) nextScreen();
			if (ChamberDevices.leftButtonPressed() && ChamberDevices.rightButtonPressed()) Preset.setPlantTime(ChamberDevices.getTime());
			ChamberDisplay.drawAgeConfig(); break;
		} break;

	default: 
		if (ChamberDevices.middleButtonClicked()) nextScreen();
	}

}
