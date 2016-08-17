#include "GFuncs.h"


void ShowHudSTEAM(bool flag){
	byte* hudPtr = (byte*)(VAManager::baseAddr + 0x01569CC3);
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
	byte* hudPtr = (byte*)(VAManager::baseAddr + 0x01567CC3);
	if (flag){
		*hudPtr = 0;
	}
	else{
		*hudPtr = 1;
	}
}