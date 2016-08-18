#include "NMSE_Core_1_0\ModAPI.h"

HMODULE localH;
HANDLE hMonitor;
bool flag = true;

DWORD WINAPI ThreadProc(LPVOID threadParam){

	CloseHandle(hMonitor);
	return 0;
}

extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info){
		localH = mHandle;
		info.name = "Stop Wasting Time";
		// Do whatever you want from here on -- send out a thread to do constant monitoring. Only do patches or just start a thread for patching
		// If this gets held up the rest of the program will fault
		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}
};