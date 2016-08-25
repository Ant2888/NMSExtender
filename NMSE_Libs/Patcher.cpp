#include "Patcher.h"



uint8_t* Patcher::PatchJmpIMMI(uintptr_t src, uintptr_t dest, const size_t buffer = 6){
	/*
	if (buffer < 6) return NULL;

	ptrdiff_t ripModifier = src - dest;
	uint8_t bytesTW[6];

	bytesTW[0] = 0xff;
	bytesTW[1] = 0x25;
	*((sint32_t*)&bytesTW[2]) = (sint32_t)ripModifier;

	uint8_t* bytesTR = 0;

	for (size_t i = 0; i < buffer; i++){
		
	}
	*/
	return NULL;
};

