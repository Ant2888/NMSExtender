#include "NMSE_Core_1_0\ModAPI.h"
#include "NMSE_Libs\Hooking.h"
#include "GFuncs.h"
#include <sstream>
#include <string>
#include <fstream>

HMODULE localH;
HANDLE hMonitor;
VERSION vers;
bool flag = true;
int KEYCHK = VK_F5;
uintptr_t hudStuff;

DWORD WINAPI ThreadProc(LPVOID threadParam){
	//In the future I will create a hook into the games native Key Listener so this hack doesn't need to go on about
	bool keyPressed = false;
	bool m_shwHud = false;
	SHORT btnKeyState;
		

	while (flag){
		btnKeyState = GetAsyncKeyState(KEYCHK);
		if (((1 << 16) & btnKeyState) && !keyPressed){
			keyPressed = true;
			ShowHud(m_shwHud, (byte*)hudStuff);
			m_shwHud = !m_shwHud;
			Sleep(175);
		}
		else if (!((1 << 16) & btnKeyState) && keyPressed){
			keyPressed = false;
		}
		Sleep(50);
	}
	CloseHandle(hMonitor);
	return 0;
}

extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info){
		localH = mHandle;
		info.name = "No HUD";
		vers = info.version; // Which game version (GOG or STEAM) this DLL is injected into
		// Do whatever you want from here on -- send out a thread to do constant monitoring. Only do patches or just start a thread for patching
		// If this gets help up the rest of the program will fault

		hudStuff = findHUD();
		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}
};