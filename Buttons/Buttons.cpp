/*
  Buttons.cpp - Select node, increase and decrease node value library.
*/

#include "Arduino.h"
#include "Buttons.h"

// Buttons(int iPin, int iNoise, int iSwitchSelect, int iSwitchInc, int iSwitchDec, int iNodes);
Buttons::Buttons(int ipn, int inz, int iss, int isi, int isd, int inds) 
{
  // pinMode(pin, OUTPUT);
  // the analog input pin reading tension as a byte volue (0-255) on the resistor ladded (see diagram)
  iPin = ipn;
  // +- iNoise are considered noise. NB, value needs to be chosen such that it agrees with analogue reading
  // e.g. if the resistor ladder produces AD conversions around 210 and 420. Observe variations due to noise.
  // In this example, noise is not expected to be greater than (420 - 210) / 2 = 100 as upper and lower limits
  // would overlap e.g.  if noise was reached 101 max in the circuit; 210 + 101 = 311 and 420 - 101 = 311
  // The algorithm would produce an "increase button pressed" and a "decrease value button pressed" result, messing
  // up the logic.
  iNoise = inz;
  // The analogue reading expected for a switch button press
  iSwitchSelect = iss;
  // The analogue reading expected for a decrease button press
  iSwitchInc = isi;
  // The analogue reading expected for an increase button press
  iSwitchDec = isd;
  //iMax = imx;
  //iMin = imn;
  //iStep = ist;
  // Number of nodes base 1
  iNodes = inds;
  // Change to base 0
  iNodes--;
  // button press flag
  bButtonPressed = false;
  // button release flag, TODO find descriptive name
  bChange = false;
  // set the size of stored value in bytes ~ 2 bytes 
  iStoredValSizeInBytes = 2;

  // Populate the node array with 0's since the due has no EEPROM
  for(int i = 0; i <= iNodes; i++) {
    iNode[i] = 0;
  } 

  // set the node position to 0 - first node
  iNodePos = 0;

  // Get the lowest expected AD reading;
  iLowestADReading = (iSwitchSelect < iSwitchInc ? iSwitchSelect : iSwitchInc);
  iLowestADReading = (iLowestADReading < iSwitchDec ? iLowestADReading : iSwitchDec);
 
} 

/*
void Buttons::addNode(int iMn, int iMx, int iSp, int iDx)

Add a node with attributes minimum value, maximum value, step and index (array position)
Note, iDx is base 1 and changed here to base 0
*/
void Buttons::addNode(int iMn, int iMx, int iSp, int iDx)
{
  iDx--;
  iMin[iDx] = iMn;
  iMax[iDx] = iMx;
  iStep[iDx] = iSp;
}

/*
  Application will call this member function to set the initial node
  selection on the user interface.
*/
int Buttons::getNodePos()
{
  // convert to base 1
  return iNodePos + 1;  
}

/*
void Buttons::checkButtons()

This member function is placed in the main loop
and checks if any temporary switches placed in the resistor
ladder are pressed.
*/
void Buttons::checkButtons()
{
  int iRead = 0;
  int iMaxRead = 0;
  int i;
  // ten successive analogue readings, use max
  for(i=0; i<10; i++) {
    iRead = analogRead(iPin);
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
  
  // case 1: select button was pressed (change node)
  if(bSelect) {
    iNodePos = (iNodePos + 1 > iNodes ? 0 : iNodePos + 1);
    bButtonPressed = true;
  }
  // case 2: descrease button was pressed (decrease value)
  if(bDecrease) {
	iNode[iNodePos] = (iNode[iNodePos] > iMin[iNodePos] ? iNode[iNodePos] - iStep[iNodePos] : iMin[iNodePos]);
    bButtonPressed = true;
  }
  // case 2: increase button was pressed (increase vlue)
  if(bIncrease) {
	iNode[iNodePos] = (iNode[iNodePos] < iMax[iNodePos] ? iNode[iNodePos] + iStep[iNodePos] : iMax[iNodePos]);
    bButtonPressed = true;
  }
  
  // check if button was released after it was pressed
  // e.g. if the AD reading is less than the lowest 
  // interpretable AD reading, then the button has been released. 
  if( iRead < (iLowestADReading - iNoise) && bButtonPressed ) {
       bChange = true;
  }
}

/*
  bool Buttons::changed()
  Application uses this member function in main loop to check
  for temporary switch presses.
*/
bool Buttons::changed()
{
	return bChange;
}

/*
  int getNodeVal(int iNd)
  Get the value stored in a given node.
*/
int Buttons::getNodeVal(int iNd)
{
  // convert index to base 0
  iNd--;
  return iNode[iNd];
}

int Buttons::setNodeVal()
{
  // reset the button
  bButtonPressed = false;
  // reset change flag
  bChange = false;
  // return the changed value NB application needs to call the node position
  // to deal with result
  return iNode[iNodePos];
}

 /*
 *	In the configuration below, when switch INC is shortened
 *  the AD conversion (10 bits, 0 to 1024) is 440 approx.
 *  When DEC is shortened, the reading is 260 approx.
 *  When INC is shortened, the reading is 160 approx.
 *  If the resistor ladded is modified, values must be adjusted
 *  accordingly when instantiating the object e.g.
 *  Buttons buttons(iPin, iNoise, iSwitchSelect, iSwitchInc, iSwitchDec, iNodes);
 *
 *   Schematic:                         AD conversion
 *
 *   analogPin                 +5 V      
 *      |                         |
 *      |                         \
 *      ----------------          /  
 *                     |          \  10K 
 *                     |          /
 *                     |          \  440 (expected 10 bit analogue read value)
 *                     |____ \____| 
  *                    |   INC    |         
 *                     |          \
 *                     |          /  
 *                     |          \  10K
 *                     |          /
 *                     |          \
 *                     |____ \____|  260 (expected 10 bit analogue read value
 *                     |   DEC    |         
 *                     |          \
 *                     |          /  
 *                     |          \  10K
 *                     |          /
 *                     |          \
 *                     |____ \____|  160 (expected 10 bit analogue read value)
 *                     |   SEL    |
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