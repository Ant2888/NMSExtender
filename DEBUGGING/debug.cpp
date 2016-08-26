#include "NMSE_Core_1_0\ModAPI.h"
#include "NMSE_Libs\Hooking.h"
#include "NMSE_Libs\MemoryManager.h"
#include <io.h>
#include <fcntl.h>


HMODULE localH;
HANDLE hMonitor;
VERSION vers; 
HANDLE _wndThread;
bool consActive = false; 
DWORD WINAPI WndThread(LPVOID lpThreadParameter); // thread method

void ApplyFunctionUsed(){
	std::cout << "TEST\n";
}

DWORD WINAPI ThreadProc(LPVOID threadParam){
	_wndThread = CreateThread(0, 0, WndThread, 0, 0, 0);

	while (!consActive) Sleep(1200);

	uint32_t rdx[2];
	int r8;
	uint32_t r9[2];

	//typedef void(*_RechargeFunc)(uint64_t* gameStruct, uint64_t rdxVal, int r8Val, uint64_t r9Val);
	//VAddr<_RechargeFunc> RechargeFunc(0x043C4D0);
	/**bool flag = true;
	while (flag){
		std::cout << "Press anything to generate: ";
		int val = 0;
		std::cin >> val;
		if (!val)
			exit(0);
		uint64_t* rcx = (uint64_t*)(VAManager::baseAddr + 0x1616728);
		//rcx += 0x350;
		uint64_t* fstStep = (uint64_t*)(*rcx + 0x430);
		uint64_t secStep = *(fstStep + 0x28);
		uint64_t thrdStep = secStep + 0x30;
		uint64_t* lifeSupport = (uint64_t*)(thrdStep + 0x18);


		std::cout << "RCXADDR:" << std::hex << *rcx << std::endl;
		std::cout << "LIFESUPPORTVAL: " << lifeSupport << std::endl;
		Sleep(500);
	}**/
	CloseHandle(hMonitor);
	return 0;
}

extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info){
		localH = mHandle;
		info.name = "TESTING PATCHER";
		vers = info.version; // Which game version (GOG or STEAM) this DLL is injected into
		// Do whatever you want from here on -- send out a thread to do constant monitoring. Only do patches or just start a thread for patching
		// If this gets held up the rest of the program will fault
		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}

	void RegisterForApplyEvent(void (*RegFunc)(void(*paramFunc)())){
		(*RegFunc)(ApplyFunctionUsed);
	}
};


DWORD WINAPI WndThread(LPVOID lpThreadParameter){
	AllocConsole();
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
	consActive = true;
	CloseHandle(_wndThread);
	return 0;
}