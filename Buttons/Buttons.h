/*
  Buttons.cpp - Select, increase and decrease library.
*/
#ifndef Buttons_h
#define Buttons_h

#include "Arduino.h"
#include <EEPROM.h>

class Buttons
{
  public:
    Buttons(int iPin, int iNoise, int iSwitchSelect, int iSwitchInc, int iSwitchDec, int iMax, int iMin, int iStep, int iNodes);
    void checkButtons();
    bool changedTemp();
	int getTemp();
	int initTemp();
  private:
    int iPin;
	int iNoise;
	int iSwitchSelect;
	int iSwitchInc;
	int iSwitchDec;
	int iMax;
	int iMin;
	int iStep;
	bool bButtonPressed;
	bool bSelect;
	bool bDecrease; 
	bool bIncrease;
	bool bChange;
	int iNode[];
	int iNodePos;
	// int eepromTempAddress;

	
/*
// Temperature control LCD constants
#define ERROR_WINDOW 30  // +/- this value means button was pressed and is within expected range
#define SWITCH_INCREASE_TEMP 420
#define SWITCH_DECREASE_TEMP 210
#define MAX_TEMPERATURE 220
#define MIN_TEMPERATURE 0
// change by step
#define CHANGE_TEMP_STEP 5
// button pressed boolean variables
boolean buttonPressed = false;

// analogue read
int iAPin = 0;
int a=0;
int amax=0;
int i=0;
*/	
	
};

#endif
