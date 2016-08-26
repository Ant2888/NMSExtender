#include "AddonManager.h"
#include "TestHooks.h"
#include "NMSE_Libs\MemoryManager.h"
#include "EventManager.h"

HANDLE modHandle;
HHOOK hkeyHook;
static bool isRun = false;


void OnAttach(){
	if (isRun) return;
	isRun = true;

	//aloc mem

	if (!global_Memory.CreateMemory(1024 * 48)){
		MessageBox(0, "Global Mem Failed To Alloc", "NMSE", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!local_Memory.CreateMemory(1024 * 48, modHandle)){
		MessageBox(0, "Local Mem Failed To Alloc", "NMSE", MB_OK| MB_ICONWARNING);
		return;
	}


	TestHook();
	modManager.Init();

	
	std::string rtp(RunTimePath()); //save some func calls
	if (CheckFile(rtp + "\\opengl32.dll") || CheckFile(rtp + "\\Opengl32.dll") || CheckFile(rtp + "\\OPENGL32.dll")
		|| CheckFile(rtp + "\\OpenGL32.dll")){ //stat doesn't seem to have a quick and easy way to implment non case-sensitivity
		//so this hack will have to do
		LoadLibrary(std::string(RunTimePath() + "\\opengl32.dll").c_str());
	}
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