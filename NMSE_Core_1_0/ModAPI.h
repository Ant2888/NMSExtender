#pragma once
#include "NMSE_Libs\MemoryManager.h"

enum VERSION{
	GOG,
	STEAM,
	STEAMEXP //NYI
};


struct ModDetails{
	std::string name;
	HANDLE dllHandle;
	VERSION version;
};

// OnStart gets called on your DLL being loaded in. It's your job to fill out the details (you don't have to be it's recommended).
typedef bool(*_OnStart)(HMODULE& mHandle, ModDetails& info);
typedef void(*_UseAllocMemory)(MemoryManager* globalMem, MemoryManager* localMem);

typedef void(*_ScannerListener)(void(*FuncToReg)(uint64_t, int, int, float, uint64_t, char, uint64_t));
typedef void(*_RegisterForScannerEvent)(_ScannerListener RegisterFunction);

//NYI
typedef void(*_ApplyListener)(void(*FuncToReg)());
typedef void(*_RegisterForApplyEvents)(_ApplyListener RegisterFunction);

//NYI
typedef void(*_KeyListener)(void(*FuncToReg)(SHORT));
typedef void(*_RegisterForKeyEvents)(_KeyListener RegisterFunction);