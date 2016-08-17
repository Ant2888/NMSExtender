#pragma once
#include "NMSE_Core_1_0\ModAPI.h"
#include <stdio.h>
#include <iostream>
#include <tchar.h>

HMODULE localH;
HANDLE hMonitor;
bool flag = true;

void foregroundFix(HWND hwnd){
	WINDOWPLACEMENT place;
	memset(&place, 0, sizeof(WINDOWPLACEMENT));
	place.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd, &place);
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	SetForegroundWindow(hwnd);
}

DWORD WINAPI ThreadProc(LPVOID threadParam){
	HWND nmsHwnd(nullptr);
	int antCount = 0;
	while (!(nmsHwnd = FindWindow(NULL, _T("Application"))) && antCount < 12) {
		// Search 12 times for the application
		antCount++;
		Sleep(750);
	}
	if (antCount >= 12) {
		// couldnt find app
		CloseHandle(hMonitor);
		return 0;
	}
	Sleep(1000);
	DWORD nmsProcID = GetWindowThreadProcessId(nmsHwnd, 0);
	int closeCount = 0;
	bool isMax = false;
	while (flag) {
		if (closeCount >= 50) {
			// Note this is subject to change. It's kind of dumb they named it this.
			// Might be whatever they used to develop this though.
			if (!(nmsHwnd = FindWindow(NULL, _T("Application")))) {
				flag = false;
				CloseHandle(hMonitor);
				return 0;
			}
			closeCount = 0;
		}
		DWORD active = GetWindowThreadProcessId(GetForegroundWindow(), 0);
		if (active != nmsProcID) {
			isMax = false;
		}
		else if (active == nmsProcID && !isMax) {
			foregroundFix(nmsHwnd);
			isMax = true;
		}
		closeCount++;
		Sleep(200);
	}
	CloseHandle(hMonitor);
	return 0;
}

extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info){
		localH = mHandle;
		info.name = "Tabbing Fix";
		//if (info.version == GOG); // DO something
		//if (info.version == STEAM) //Do something else etc etc
		// Do whatever you want from here on -- send out a thread to do constant monitoring. Only do patches or just start a thread for patching
		// If this gets help up the rest of the program will fault
		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}
};