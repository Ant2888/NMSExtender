#pragma once
#include "NMSE_Libs\MemoryManager.h"

enum VERSION{
	GOG,
	STEAM,
	STEAMEXP //NYI
};

struct ModDetails{
	std::string name;
	VERSION version;
};

// OnStart gets called on your DLL being loaded in. It's your job to fill out the details (you don't have to be it's recommended).
typedef bool(*_OnStart)(HMODULE& mHandle, ModDetails& info, MemoryManager& local, MemoryManager& global);
typedef void(*_RegisterForApplyEvents)(void (*Function)(void(*param)()));