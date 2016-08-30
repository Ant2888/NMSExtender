#include "AddonManager.h"
#include "TestHooks.h"
#include "NMSE_Libs\MemoryManager.h"
#include "EventManager.h"
#include "OGLHook.h"

HANDLE modHandle;
HHOOK hkeyHook;
static bool isRun = false;

void OnAttach(){
	if (isRun) return;
	isRun = true;

	global_Logger.OpenFile(std::string(RunTimePath()+"\\NMSE\\LOG.txt").c_str(), false);

	WRITEMSG("---- NMSE CORE Hook Init ----\n");
	//aloc mem
	if (!global_Memory->CreateMemory(1024 * 48)){
		MessageBox(0, "Global Mem Failed To Alloc", "NMSE", MB_OK | MB_ICONWARNING);
		ERRORMSG("GMEM Failed To Alloc: ", GetLastError());
		return;
	}

	SUCCESSMSG("Global Mem Alloced");

	if (!local_Memory->CreateMemory(1024 * 48, modHandle)){
		MessageBox(0, "Local Mem Failed To Alloc", "NMSE", MB_OK | MB_ICONWARNING);
		ERRORMSG("LMEM Failed To Alloc: ", GetLastError());
		return;
	}

	SUCCESSMSG("Local Mem Alloced");

	//TestHook();
	modManager.SetMainDLL(modHandle);
	modManager.Init();

	//call last so the hook doesn't start
	HookOGL();
	SUCCESSMSG("NMSE Initialized");
	WRITEMSG("---- NMSE CORE Hook Complete ----\n");
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