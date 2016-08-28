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


	//TestHook();
	modManager.SetMainDLL(modHandle);
	modManager.Init();
	
	Sleep(1000);
	std::string rtp(RunTimePath()); //save some func calls
	if (CheckFile(rtp + "\\opengl32.dll")){ //stat doesn't seem to have a quick and easy way to implment non case-sensitivity
		//so this hack will have to do
		LoadLibrary(std::string(RunTimePath() + "\\opengl32.dll").c_str());
	}
	else if (CheckFile(rtp + "\\Opengl32.dll")){ //stat doesn't seem to have a quick and easy way to implment non case-sensitivity
		//so this hack will have to do
		LoadLibrary(std::string(rtp + "\\Opengl32.dll").c_str());
	}
	else if (CheckFile(rtp + "\\OPENGL32.dll")){ //stat doesn't seem to have a quick and easy way to implment non case-sensitivity
		//so this hack will have to do
		LoadLibrary(std::string(rtp + "\\OPENGL32.dll").c_str());
	}
	else if (CheckFile(rtp + "\\OpenGL32.dll")){ //stat doesn't seem to have a quick and easy way to implment non case-sensitivity
		//so this hack will have to do
		LoadLibrary(std::string(rtp + "\\OpenGL32.dll").c_str());
	}

	if (CheckFile(rtp + "\\xinput9_1_0.dll")){
		LoadLibrary(std::string(rtp + "\\xinput9_1_0.dll").c_str());
	}
	if (CheckFile(rtp + "\\opengl32.dll")){ //stat doesn't seem to have a quick and easy way to implment non case-sensitivity
		//so this hack will have to do
		std::cout << "YOU HAVE THE OPENGL32 DLL INSTALLED\n";
	}
	else{
		std::cout << "No opengl32.dll Found! \n";
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