#include "GFuncs.h"

void ShowHud(bool flag, byte* HUDPtr){
	if (flag){
		*HUDPtr = 0;
	}
	else{
		*HUDPtr = 1;
	}
}

uintptr_t findHUD(){
	uintptr_t offset = 0;
	uintptr_t funcAddr
		= MemoryManager::FindPattern("\x90\x48\xBB\xB3\x01\x00\x00\x00\x01\x00\x00\x49\xBC\x25\x23\x22\x84\xE4\x9C\xF2\xCB"
		, "xxxxxxxxxxxxxxxxxxxxx", 0x200000) + 0x15 + 0x2;
	//patternMatchaddr + length till instr + opcodes
	uint8_t ripAddr[4];
	uint8_t* funcAddr8 = (uint8_t*)funcAddr;
	ripAddr[0] = *funcAddr8;
	ripAddr[1] = *(funcAddr8 + 1);
	ripAddr[2] = *(funcAddr8 + 2);
	ripAddr[3] = *(funcAddr8 + 3);

	for (int i = 3; i >= 0; i--){
		offset = (offset << 8) + ripAddr[i];
	}

	//add the offset plus instr count - opcodes we got rid of earlier
	funcAddr += offset + 0x7 - 0x2;
	return funcAddr;
}