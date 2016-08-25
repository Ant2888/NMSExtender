#pragma once
#include "MemoryManager.h"


class Patcher{
	public:
		Patcher();
		~Patcher();

		//NYI NYI NYI NYI _ DO NOT USE
		//Writes a JMP to DEST at SRC. It will overwrite buffer bytes (6 default -- Do not go lower) and return them in a uint8_t* with the value in buffer
		//This will return null on fail i.e- no space left. At the moment make sure dest is customily allocated mem!
		static uint8_t* PatchJmpIMMI(uintptr_t src, uintptr_t dest, const size_t buffer = 6);
		
};

