#include <Windows.h>
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
	HookMain(0);
}

// Hooking the IAT is proving to be a pain in the ass so I'm going to gloss over it for now.
/*typedef WORD (*_glEnable)(void*);
_glEnable glEnable_Original = NULL;

static WORD glEnable_Hook(void* args){
MessageBox(NULL, "TEST", "NMSE STEAM", MB_OK);
HookMain(_ReturnAddress());

return glEnable_Original(args);
}
*/
/*typedef void(__stdcall * _GetSystemTimeAsFileTime)(LPFILETIME * fileTime);

_GetSystemTimeAsFileTime GetSystemTimeAsFileTime_Original = NULL;
_GetSystemTimeAsFileTime * _GetSystemTimeAsFileTime_IAT = NULL;*/

/*void __stdcall GetSystemTimeAsFileTime_Hook(LPFILETIME * fileTime)
{
HookMain(_ReturnAddress());

GetSystemTimeAsFileTime_Original(fileTime);
}*/
/*static void HookTables() {
_glEnable * iat = (_glEnable *)GetIATAddr(GetModuleHandle(NULL), "KERNEL32.dll", "GetSystemTimeAsFileTime");
if (iat){
glEnable_Original = *iat;
VirtualWrite64(uintptr_t(iat), (uint64_t)glEnable_Hook);
}
}*/

bool hooked = false;
static void HookMain(void* retAddr) {
	if (hooked) return;
	hooked = true;
	std::string path = RunTimePath();
	std::string dll(path + "\\NMSE_Core_1_0.dll");
	LoadLibrary(dll.c_str());
}