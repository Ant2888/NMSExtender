#pragma once
#include "NMSE_Libs\VirtualAddress.h"
#include "NMSE_Libs\MemoryManager.h"
#include <stdio.h>
#include <iostream>

void WriteHook(HANDLE);
uintptr_t findStack();