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
	//Sleep(10000);

	WriteHook(settings);

	CloseHandle(hMonitor);
	return 0;
}

SizeSettings GetSettings() {
	return {
		(int)GetPrivateProfileInt("SIZE_SETTINGS", "DEFAULT_SUBS_SIZE", 100, "Gula.ini"),
		(int)GetPrivateProfileInt("SIZE_SETTINGS", "DEFAULT_ITEM_SIZE", 10, "Gula.ini"),
		(int)GetPrivateProfileInt("SIZE_SETTINGS", "SHIP_MULT", 10, "Gula.ini"),
		(int)GetPrivateProfileInt("SIZE_SETTINGS", "CHARGE_MULT", 2, "Gula.ini")
	};
}

extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info, MemoryManager& local, MemoryManager& global){
		localH = mHandle;
		info.name = "Gula";
		vers = info.version; 
		local_Memory = local;
		global_Memory = global;

		settings = GetSettings();

		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}
};