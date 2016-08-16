#include "GFuncs.h"


int* hudPtr = (int*)(VAManager::baseAddr + 0x0156CD40);

typedef void(*APPLY)(void*, void*);
VAddr <APPLY> applyCmd(0x0BF6930);

void ShowHud(bool flag){
	//a bit redundant but I'm just going to leave it :P
	void* rcxVal = (void*)(VAManager::baseAddr + 0x015ab100);
	void** rcx = &rcxVal;
	void* rdxVal = (void*)(VAManager::baseAddr + 0x0156CD00);
	void** rdx = &rdxVal;
	if (flag){
		*hudPtr = 0;
	}
	else{
		*hudPtr = 1;
	}
	applyCmd(*rcx, *rdx);
}