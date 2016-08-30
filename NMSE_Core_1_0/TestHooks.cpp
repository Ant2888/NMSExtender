#include "TestHooks.h"

typedef int64_t(*_ApplyFunc)(char** unk1, int64_t unk2);
VAddr<_ApplyFunc> ApplyFunc(
	MemoryManager::FindPattern("\x0F\x10\x02\x4C\x8B\x01\x40\x32\xF6\x48\x8B\xF9",
	"xxxxxxxxxxxx", 0x300000)
	- 0xf - VAManager::baseAddr); // Yea subtracting the base addr is nasty but later ill fix it
_ApplyFunc ApplyFunc_Original = nullptr;

void ApplyChanges_Hook(char** unk1, int64_t unk2){
	global_ApplyListener.NotifyListeners();
	ApplyFunc_Original(unk1, unk2);
}

void TestHook(){

	struct ApplyHook : Xbyak::CodeGenerator{
		ApplyHook(void* buffer) : Xbyak::CodeGenerator(4096, buffer){
			Xbyak::Label addrLbl;

			mov(ptr [rsp+8], rbx);

			jmp(ptr [rip + addrLbl]);
			

			L(addrLbl);
			dq(ApplyFunc.GetUIntPtr() + 6);
		}
	};

	void* addrToWrite = local_Memory->FirstWrittableAddr();
	ApplyHook hook(addrToWrite);
	local_Memory->CalcAllocated(hook.getCurr());

	ApplyFunc_Original = (_ApplyFunc)addrToWrite;

	global_Memory->PatchBranch(ApplyFunc.GetUIntPtr(), (uintptr_t)ApplyChanges_Hook);
}