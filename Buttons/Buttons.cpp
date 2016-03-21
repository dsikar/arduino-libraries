/*
  Buttons.cpp - Select, increase and decrease library.
*/

#include "Arduino.h"
#include "Buttons.h"

// Buttons(int iPin, int iNoise, int iSwitchSelect, int iSwitchInc, int iSwitchDec, int iMax, int iMin, int iStep, int iNodes);
Buttons::Buttons(int ipn, int inz, int iss, int isi, int isd, int imx, int imn, int ist, int inds)
{
  // pinMode(pin, OUTPUT);
  iPin = ipn;
  iNoise = inz;
  iSwitchSelect = iss;
  iSwitchInc = isi;
  iSwitchDec = isd;
  iMax = imx;
  iMin = imn;
  iStep = ist;
  iNodes = inds;
  bButtonPressed = false;
  bChange = false;
  // read temperature from eeprom, NB 1 byte values between 0-255. If a higher range is required, 
  // extra coding will also be required to split value before storing and assembling value after reading.

  // New a loop here to populate our iTemp array
  for(int i = 0; i <= iNodes; i++) {
    iNode[i] = EEPROM.read(i);
  } 

  // get the last array position stored
  iNodePos = EEPROM.read(iNodes + 1);
 
  // eepromTempAddress = 0;
  // iTemp = EEPROM.read(eepromTempAddress); 
} 

int Buttons::initTemp()
{
   return iNode;  
}

void Buttons::checkButtons()
{
  int iRead = 0;
  int iMaxRead = 0;
  int i;
  // ten successive analogue readings, use max
  for(i=0; i<10; i++) {
    iRead = analogRead(pin);
    if (iRead > iMaxRead) {
      iMaxRead = iRead;
    }
  }
  iRead = iMaxRead;

  // was select button pressed?
  bSelect = (iRead <= iSwitchSelect + iNoise && iRead >= iSwitchSelect - iNoise && !bButtonPressed ? true : false);
  // was decrease button pressed?
  bDecrease = (iRead <= iSwitchDec + iNoise && iRead >= iSwitchDec - iNoise && !bButtonPressed ? true : false);
  // was increase button pressed?
  bIncrease = (iRead <= iSwitchInc + iNoise && iRead >= iSwitchInc - iNoise && !bButtonPressed ? true : false);
  
  // need a flag here to let us know in what array position we are
  // CODE GOES HERE
  if(bSelect) {


  }
  // case 1: < button was pressed (decrease speed)
  if(bDecrease) {
	iTemp = (iTemp > iMinTemp ? iTemp - iTempStep : iMinTemp);
    bButtonPressed = true;
  }
  // case 2: > button was pressed (increase speed)
  if(bIncrease) {
	iTemp = (iTemp < iMaxTemp ? iTemp + iTempStep : iMaxTemp);
    bButtonPressed = true;
  }
  
  // check if button was released after it was pressed
  // e.g. if the reading is less than the lowest 
  // interpretable reading, then the button has been released. 
  // ROOM FOR IMPROVEMENT
  // using the (hardcoded) lowest iSwitchSelect value, need to make it dynamic.
  if( iRead < (iSwitchSelect - iNoise) && bButtonPressed ) {
       bChangeTemp = true;
  }
}

bool Buttons::changedTemp()
{
	return bChangeTemp;
}

int Buttons::getTemp()
{
  // reset the button
  bButtonPressed = false;
  // reset change flag
  bChange = false;
  // write to EEPROM
  //EEPROM.write(eepromTempAddress, iTemp);
  for(int i = 0; i <= iNodes; i++) {
    EEPROM.write(i, iNode[i]);
  } 
  // write the array position to eeprom
  EEPROM.write(iNodes + 1, iNodePos);
  // return the array
  return iNode;
}

 /*
 *	In the configuration below, when switch SW1 is shortened
 *  the AD conversion (10 bits, 0 to 1024) is 420.
 *  When SW2 is shortened, the reading is 210.
 *  If the circuit is modified, then the values must be adjusted
 *  accordingly when instantiating the object e.g.
 *  Buttons buttons(0, 30, 420, 210, 220, 0, 5);
 *
 *   Schematic:                         AD conversion
 *
 *   analogPin                 +5 V      
 *      |                         |
 *      |                         \
 *      ----------------          /  
 *                     |          \  10K 
 *                     |          /
 *                     |          \  420 (expected 10 bit analogue read value)
 *                     |____ \____| 
 *                     |   SW1    |         
 *                     |          \
 *                     |          /  
 *                     |          \  10K
 *                     |          /
 *                     |          \
 *                     |____ \____|  210 (expected 10 bit analogue read value)
 *                     |   SW2    |
 *                     \          \
 *                     /          /  
 *                     \  10K     \  10K (pull down resistors)
 *                     /          /
 *                     \          \
 *                     |          |
 *                     |          |
 *                   _____      _____  
 *                    ___        ___     ground
 *                     _          _
 *
 */