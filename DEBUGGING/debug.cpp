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
	//while (consActive) Sleep(1000);
	uintptr_t offset = 0;
	uintptr_t funcAddr
		= MemoryManager::FindPattern("\x90\x48\xBB\xB3\x01\x00\x00\x00\x01\x00\x00\x49\xBC\x25\x23\x22\x84\xE4\x9C\xF2\xCB"
		, "xxxxxxxxxxxxxxxxxxxxx", 0x200000) + 0x15 + 0x2;
	//patternMatchaddr + length till instr + opcodes
	uint8_t ripAddr[4];
	uint8_t* funcAddr8 = (uint8_t*)funcAddr;
	ripAddr[0] = *funcAddr8;
	ripAddr[1] = *(funcAddr8 + 1);
	ripAddr[2] = *(funcAddr8 + 2);
	ripAddr[3] = *(funcAddr8 + 3);

	for (int i = 3; i >= 0; i--){
		offset = (offset << 8) + ripAddr[i];
	}

	//add the offset plus instr count - opcodes we got rid of earlier
	funcAddr += offset + 0x7 - 0x2;
	//seems to be 4 away

	std::cout << "OFF: " << std::hex << offset;
	std::cout << "VAL: " << std::hex << funcAddr;
	
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
		// If this gets helded up the rest of the program will fault
		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}

	void RegisterForApplyEvent(void (*RegFunc)(void(*param)())){
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