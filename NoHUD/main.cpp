#pragma once
#include "NMSE_Core_1_0\ModAPI.h"
#include "GFuncs.h"
#include <stdio.h>
#include <tchar.h>
#include <sstream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

HMODULE localH;
HANDLE hMonitor;
VERSION vers;
bool flag = true;

DWORD WINAPI ThreadProc(LPVOID threadParam){
	//In the future I will create a hook into the games native Key Listener so this hack doesn't need to go on about
	bool keyPressed = false;
	bool m_shwHud = false;
	SHORT btnKeyState;
		
	while (flag){
		btnKeyState = GetAsyncKeyState(VK_F5);
		if (((1 << 16) & btnKeyState) && !keyPressed){
			std::cout << "KEY PRESSED\n";
			keyPressed = true;
			if (vers == GOG){
				ShowHudGOG(m_shwHud);
			}
			else{
				ShowHudSTEAM(m_shwHud);
			}
			m_shwHud = !m_shwHud;
			Sleep(175);
		}
		else if (!((1 << 16) & btnKeyState) && keyPressed){
			std::cout << "KEY NOT PRESSED\n";
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
		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}
};