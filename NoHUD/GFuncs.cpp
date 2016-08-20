#include "GFuncs.h"

#define EXP 0

void ShowHudSTEAM(bool flag){

#if EXP
	byte* hudPtr = (byte*)(VAManager::baseAddr + 0x0);
#else
	byte* hudPtr = (byte*)(VAManager::baseAddr + 0x01594128);
#endif

	if (flag){
		*hudPtr = 0;
	}
	else{
		*hudPtr = 1;
	}
}

void ShowHudGOG(bool flag){
	//byte* hudPtr
	//do stuff
	byte* hudPtr = (byte*)(VAManager::baseAddr + 0x01594168);
	if (flag){
		*hudPtr = 0;
	}
	else{
		*hudPtr = 1;
	}
}