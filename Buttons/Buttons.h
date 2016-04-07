/*
  Buttons.h - Select node, increase and decrease node value library.
*/
#ifndef Buttons_h
#define Buttons_h
#include "Arduino.h"

class Buttons
{
  public:
    Buttons(int iPin, int iNoise, int iSwitchSelect, int iSwitchInc, int iSwitchDec, int iNodes);
    void checkButtons();
    void addNode(int iMn, int iMx, int iSp, int iDx);
    bool changed();
	int getNodeVal(int iNd);
	int getNodePos();
  	int setNodeVal();
  private:
    int iPin;
	int iNoise;
	int iSwitchSelect;
	int iSwitchInc;
	int iSwitchDec;
	int iMax[3];
	int iMin[3];
	int iStep[3];
	int iNode[3];
	int iNodePos;
	int iNodes;	
	int iStoredValSizeInBytes;
	int iLowestADReading;		
	bool bButtonPressed;
	bool bSelect;
	bool bDecrease; 
	bool bIncrease;
	bool bChange;

};

#endif
