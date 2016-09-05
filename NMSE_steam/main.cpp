#include <intrin.h>
#include <shlobj.h>
#include "NMSE_Libs\Hooking.h"
#include "NMSE_Libs\VirtualWrite.h"

HANDLE dllHandle;

static void OnAttach(void);
static void HookTables();
static void HookMain(void*);

BOOL WINAPI DllMain(HANDLE pHandle, DWORD reason, LPVOID res) {
	if (reason == DLL_PROCESS_ATTACH) {
		dllHandle = pHandle;
		OnAttach();
	}
}

static void OnAttach(void) {
	//HookTables();
	HookMain(nullptr);

}

bool hooked = false;
static void HookMain(void* retAddr) {
	if (hooked) return;
	hooked = true;
	std::string path = RunTimePath();
	std::string dll(path + "\\NMSE_Core_1_0.dll");
	LoadLibrary(dll.c_str());
}