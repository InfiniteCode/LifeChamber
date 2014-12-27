// ChamberDisplay.h

#ifndef _CHAMBERDISPLAY_h
#define _CHAMBERDISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class ChamberDisplayClass
{
 protected:
	 int calcMoistureFillWidth(int moisture);
	 void drawMoisture(int at);
	 void drawTemp(int at);
	 void drawAge(int at);
	 void drawDateTime(int at);
	 void drawLight(int at);
	 void drawChangeInfo(int at, bool bothToReset = false);

 public:
	void init();

	void drawMain();
	void drawMoistureConfig();
	void drawTempConfig();
	void drawLightConfig();
	void drawAgeConfig();
};

extern ChamberDisplayClass ChamberDisplay;

#endif

