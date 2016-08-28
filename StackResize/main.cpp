#include "NMSE_Core_1_0\ModAPI.h"
#include "NMSE_Libs\Hooking.h"
#include "HookFuncs.h"
#include <sstream>
#include <string>
#include <fstream>

HMODULE localH;
HANDLE hMonitor;
VERSION vers;
uintptr_t stack_ptr;
SizeSettings settings;

DWORD WINAPI ThreadProc(LPVOID threadParam){
	WriteHook(settings);

	CloseHandle(hMonitor);
	return 0;
}

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
		info.name = "Gula";
		vers = info.version; 

		settings = GetSettings("./NMSE/StackResize.ini");

		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}

	bool GrabVirtualMem(MemoryManager* globalMem, MemoryManager* localMem) {
		global_Memory = *globalMem;
		local_Memory = *localMem;

		return globalMem != NULL && localMem != NULL;
	}
};