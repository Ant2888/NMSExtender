#include "OGLHook.h"

typedef void* (*_LoadOGL)(int a1, int a2, uintptr_t a3);
VAddr<_LoadOGL> LoadOGL(
	MemoryManager::FindPattern("\x8B\xF2\x44\x8B\xF1\x85\xC9",
	"xxxxxxx", 0x300000) - VAManager::baseAddr - 0x17); // Yea subtracting the base addr is nasty but later ill fix it
_LoadOGL LoadOGL_Original = nullptr;


//Loads the shader if it's present
void LoadOGL_Hook(int a1, int a2, uintptr_t a3){
	if (CheckFile(RunTimePath() + "\\ReShade64.dll")){
		LoadLibrary(std::string(RunTimePath() + "\\ReShade64.dll").c_str());
	}
	else if (CheckFile(RunTimePath() + "\\reshade64.dll")){
		LoadLibrary(std::string(RunTimePath() + "\\reshade64.dll").c_str());
	}
	LoadOGL_Original(a1, a2, a3);
	//else do nothing. haven't developed any ogl hooks yet
}

void HookOGL(){
	struct ApplyHook : Xbyak::CodeGenerator{
		ApplyHook(void* buffer) : Xbyak::CodeGenerator(4096, buffer){
			Xbyak::Label addrLbl;

			mov(rax, rsp);
			mov(ptr[rax + 0x18], rsi);

			jmp(ptr[rip + addrLbl]);


			L(addrLbl);
			dq(LoadOGL.GetUIntPtr() + 7);
		}
	};

	void* addrToWrite = local_Memory->FirstWrittableAddr();
	ApplyHook hook(addrToWrite);
	local_Memory->CalcAllocated(hook.getCurr());

	LoadOGL_Original = (_LoadOGL)addrToWrite;

	global_Memory->PatchBranch(LoadOGL.GetUIntPtr(), (uintptr_t)LoadOGL_Hook, 7);
}