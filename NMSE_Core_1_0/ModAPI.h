#pragma once
#include <Windows.h>
#include <string>

struct ModDetails{
	std::string name;
	UINT32 version;
	HMODULE modHandle;
};
// OnStart gets called on your DLL being loaded in. It's your job to fill out the details (you don't have to be it's recommended).
typedef bool(*_OnStart)(HMODULE& mHandle, ModDetails& info);