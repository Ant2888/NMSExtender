#include "NMSE_Core_1_0\ModAPI.h"
#include "NMSE_Libs\Hooking.h"
#include "HookFuncs.h"
#include <sstream>
#include <string>
#include <fstream>

HMODULE localH;
VERSION vers;
uintptr_t stack_ptr;
SizeSettings settings;

SizeSettings GetSettings(LPCSTR dir) {
	return {
		(int)GetPrivateProfileInt("SIZE_SETTINGS", "DEFAULT_SUBS_SIZE", 250, dir),
		(int)GetPrivateProfileInt("SIZE_SETTINGS", "DEFAULT_ITEM_SIZE", 1, dir),
		(int)GetPrivateProfileInt("SIZE_SETTINGS", "SHIP_MULT",			2, dir),
		(int)GetPrivateProfileInt("SIZE_SETTINGS", "CHARGE_VAL",		-1, dir)
	};
}

extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info){
		localH = mHandle;
		info.name = "StackResize";
		vers = info.version; 

		settings = GetSettings("./NMSE/StackResize.ini");

		WriteHook(settings);
		return true;
	}

	void GrabVirtualMem(MemoryManager* globalMem, MemoryManager* localMem) {
		global_Memory->Destroy();
		local_Memory->Destroy();

		global_Memory = globalMem;
		local_Memory = localMem;
	}
};