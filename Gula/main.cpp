#include "NMSE_Core_1_0\ModAPI.h"
#include "NMSE_Libs\Hooking.h"
#include "HookFuncs.h"
#include <sstream>
#include <string>
#include <fstream>

HMODULE localH;
HANDLE hMonitor;
VERSION vers;
uintptr_t stackConstant;

const int MAX_AMOUNT = 500;

DWORD WINAPI ThreadProc(LPVOID threadParam){
	//VirtualSet(stackConstant + 0, 0xF4, sizeof(byte));
	//VirtualSet(stackConstant + 1, 0x01, sizeof(byte));

	//Constant Location
	VirtualWrite(stackConstant + 3, (void*)&MAX_AMOUNT, sizeof(int));

	VirtualSet(stackConstant - 2, 0x90, sizeof(byte));
	VirtualSet(stackConstant - 1, 0x90, sizeof(byte));

	CloseHandle(hMonitor);
	return 0;
}


extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info){
		localH = mHandle;
		info.name = "Gula";
		vers = info.version; 

		stackConstant = findStack();
		if (stackConstant == NULL) {
			return false;
		}

		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}
};