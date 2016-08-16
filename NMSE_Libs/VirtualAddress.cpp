#include "VirtualAddress.h"

//Credit to the SE team

//Helps support pointers in the relocated binaries.

#pragma warning(disable: 4073)
#pragma init_seg(lib)

static VAManager s_vaMgr;

uintptr_t VAManager::baseAddr = 0;

VAManager::VAManager(){
	baseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
}
