#include "GFuncs.h"


void ShowHudSTEAM(bool flag){
	byte* hudPtr = (byte*)(VAManager::baseAddr + 0x01594128);
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
	byte* hudPtr = (byte*)(VAManager::baseAddr + 0x015B7168);
	if (flag){
		*hudPtr = 0;
	}
	else{
		*hudPtr = 1;
	}
}