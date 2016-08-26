#pragma once

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
typedef bool(*_OnStart)(HMODULE& mHandle, ModDetails& info);
typedef void(*_RegisterForApplyEvents)(void (*Function)(void(*paramFunc)()));
typedef void(*_RegisterForKeyEvents)(void(*Function)(void(*paramFunc)(SHORT)));