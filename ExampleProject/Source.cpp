#include "NMSE_Core_1_0\ModAPI.h"

HMODULE modHandle;

extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info){
		modHandle = mHandle;
		info.name = "Example Project";
		// Please don't keep this in when posting actual mods!
		MessageBox(0, "Hello World!", info.name.c_str(), MB_OK);
		return true;
	}
};