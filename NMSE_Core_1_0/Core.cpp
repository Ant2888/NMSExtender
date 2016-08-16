#include <Windows.h>
#include <iostream>
#include "AddonManager.h"

HANDLE modHandle;
static bool isRun = false;

void OnAttach(){
	if (isRun) return;
	isRun = true;
	modManager.Init();
	FlushInstructionCache(GetCurrentProcess(), NULL, 0);
}

extern "C" {

	void InitNMSE(void){
		OnAttach();
	}

	BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
	{
		switch (dwReason)
		{
		case DLL_PROCESS_ATTACH:
			modHandle = hDllHandle;
			InitNMSE();
			break;
		case DLL_PROCESS_DETACH:
			break;
		};

		return TRUE;
	}

};