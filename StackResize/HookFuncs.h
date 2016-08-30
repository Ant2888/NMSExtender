#pragma once
#include "NMSE_Libs\VirtualAddress.h"
#include "NMSE_Libs\MemoryManager.h"
#include <stdio.h>
#include <iostream>

struct SizeSettings {
	int DEFAULT_SUBS_SIZE;
	int DEFAULT_ITEM_SIZE;
	int SHIP_MULT;
	int CHARGE_VAL;
};

void WriteHook( SizeSettings, MemoryManager*, MemoryManager* );
uintptr_t findStack();