#pragma once
#include "NMSE_Libs\VirtualAddress.h"
#include "NMSE_Libs\MemoryManager.h"
#include <stdio.h>
#include <iostream>

void ChangeStack(bool, byte*);
uintptr_t findStack();