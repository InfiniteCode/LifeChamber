// 
// 
// 

#include "Config.h"
#include "ChamberDisplay.h"
#include "ChamberDevices.h"
#include "Preset.h"

#include <stdio.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display(DISPLAY_DC, DISPLAY_CS, DISPLAY_RST);
#define PREZERO(a) if(a < 10) display.print("0");

void ChamberDisplayClass::init()
{
	display.begin();
	display.setContrast(DISPLAY_CONTRAST);
	display.clearDisplay();
	display.setRotation(0); 
	display.setTextSize(1);
	display.setTextColor(BLACK);
}

void ChamberDisplayClass::drawMoisture(int at)
{
	if (at != -1) display.setCursor(0, at); else at = 0;
	display.setTextSize(1);
	display.println(F("Moisture:"));
	display.drawRoundRect(0, at + 9, LCDWIDTH, 10, 5, BLACK);
	display.fillRoundRect(0, at + 9, calcMoistureFillWidth(ChamberDevices.getMoisture()), 10, 5, BLACK);
	display.setCursor(map(Preset.getMoistureGoal(), 0, 100, 2, 78), at + 18); //min 2, max 78
	display.write(30);
}

void ChamberDisplayClass::drawTemp(int at)
{
	if (at != -1) display.setCursor(0, at); else at = 0;
	char str[20];
	display.print(F("Temp: "));
	dtostrf(ChamberDevices.getTemp(), 1, 1, str);
	display.print(str);
	display.write(9);
	display.println("C");
}

void ChamberDisplayClass::drawAge(int at)
{
	if (at != -1) display.setCursor(0, at); else at = 0;
	DateTime currentTime = ChamberDevices.getTime();
	if (currentTime.unixtime() == 0)
	{
		display.println("Age: Unknown");
		return;
	} else
		display.print("Age: ");

	TimeSpan sincePlanted = currentTime - Preset.getPlantTime();
	if (sincePlanted.days() > 0)
	{
		display.print(sincePlanted.days());
		display.print(F("d "));
		display.print(sincePlanted.hours());
		display.println(F("h"));
	} else
	if (sincePlanted.hours() > 0)
	{
		display.print(sincePlanted.hours());
		display.print(F("h "));
		display.print(sincePlanted.minutes());
		display.println(F("m"));
	} else
	{
		display.print(sincePlanted.minutes());
		display.print(F("m "));
		display.print(sincePlanted.seconds());
		display.println(F("s"));
	}
}

void ChamberDisplayClass::drawLight(int at)
{
	if (at != -1) display.setCursor(0, at); else at = 0;
	int timeHours = Preset.getLightHours();
	display.print(F("Light amount: "));
	if (timeHours == LIGHT_MIN)
	{
		display.println(F("  Always OFF"));
		return;
	} else
	if (timeHours == LIGHT_MAX)
	{
		display.println(F("  Always ON"));
		return;
	}

	display.print("  ");
	display.print(timeHours);
	display.println(F("h per day"));
}


void ChamberDisplayClass::drawDateTime(int at)
{
	if (at != -1) display.setCursor(0, at); else at = 0;
	DateTime dt = ChamberDevices.getTime();
	display.print(dt.hour()); display.print(":");
	PREZERO(dt.minute()); display.print(dt.minute()); display.print("    ");

	PREZERO(dt.day()); display.print(dt.day()); display.print("/");
	PREZERO(dt.month()); display.println(dt.month());
}

void ChamberDisplayClass::drawMain()
{
	display.clearDisplay();
	
	drawMoisture(0);
	drawTemp(25);
	drawAge(-1);
	drawDateTime(-1);

	display.display();
}

int ChamberDisplayClass::calcMoistureFillWidth(int moisture)
{
	return map(moisture, 0, 100, 0, LCDWIDTH - 1);
}

void ChamberDisplayClass::drawMoistureConfig()
{
	display.clearDisplay();
	display.setCursor(0, 0);
	drawMoisture(0);
	drawChangeInfo(24);
	display.display();
}

void ChamberDisplayClass::drawTempConfig()
{
	display.clearDisplay();
	display.setCursor(0, 0);
	drawTemp(0);
	display.print(F("Desired: "));
	int tempGoal = Preset.getTempGoal();
	display.print(tempGoal);
	display.print(" ");
	display.write(9);
	if (tempGoal < 10)
		display.println("C");
	else
		display.print("C");
	
	drawChangeInfo(-1);

	display.display();
}

void ChamberDisplayClass::drawLightConfig()
{
	display.clearDisplay();
	display.setCursor(0, 0);
	drawLight(0);
	drawChangeInfo(-1);
	display.display();
}

void ChamberDisplayClass::drawChangeInfo(int at, bool bothToReset)
{
	if (at != -1) display.setCursor(0, at); else at = 0;

	display.print(F("--------------"));
	display.print(F("Press "));
	display.write(17);
	display.print(bothToReset == true ? F(" and ") : F(" or "));
	display.write(16);
	display.println(bothToReset == true ? F(" to reset.") : F("  to change."));
	display.write(8);
	display.println(F(" to continue."));
}

void ChamberDisplayClass::drawAgeConfig()
{
	display.clearDisplay();
	display.setCursor(0, 0);
	drawAge(0);

	display.println("");
	drawChangeInfo(-1, true);
	display.display();
}


ChamberDisplayClass ChamberDisplay;

