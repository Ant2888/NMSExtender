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

const int MAX_AMOUNT = 500;

DWORD WINAPI ThreadProc(LPVOID threadParam){
	Sleep(10000); //For running debug attach
	WriteHook(hMonitor);

	CloseHandle(hMonitor);
	return 0;
}


extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info){
		localH = mHandle;
		info.name = "Gula";
		vers = info.version; 

		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}
};