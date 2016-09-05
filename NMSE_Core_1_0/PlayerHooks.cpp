#include "PlayerHooks.h"
	
//constVal is typicall a full 32 bit value (FFF...), constXmmVal is typically 185.40
//unk1 is something like a stack frame/global struct and the rest are usually zero and or completely unknown
typedef void(*_ScannerFunc)(uint64_t unk1, int constVal, int zeroVal, float constXmmVal, uint64_t unk5, char unk6, uint64_t unk7);

VAddr<_ScannerFunc> ScannerFunc(MemoryManager::FindPattern("\x48\x2B\x4B\x08\x49\xC1\xFF\x03\x44\x89\x78\x28\x48\x8B\x45\x10",
	"xxxxxxxxxxxxxxxx", 0x20000) -VAManager::baseAddr - 0x39);

_ScannerFunc Scanner_Original = nullptr;

void Scanner_Hook(uint64_t unk1, int constVal, int zeroVal, float constXmmVal, uint64_t unk5, char unk6, uint64_t unk7){
	global_PlayerListener.NotifyListeners(PLAYER_SCAN, unk1, constVal, zeroVal, constXmmVal, unk5, unk6, unk7);
	Scanner_Original(unk1, constVal, zeroVal, constXmmVal, unk5, unk6, unk7);
}

void Scanner_Init(){
	struct ApplyHook : Xbyak::CodeGenerator{
		ApplyHook(void* buffer) : Xbyak::CodeGenerator(4096, buffer){
			Xbyak::Label addrLbl;

			mov(rax, rsp);
			mov(ptr [rax+0x10], edx);

			jmp(ptr[rip + addrLbl]);


			L(addrLbl);
			dq(ScannerFunc.GetUIntPtr() + 6);
		}
	};

	void* addrToWrite = local_Memory->FirstWrittableAddr();
	ApplyHook hook(addrToWrite);
	local_Memory->CalcAllocated(hook.getCurr());

	Scanner_Original = (_ScannerFunc)addrToWrite;

	global_Memory->PatchBranch(ScannerFunc.GetUIntPtr(), (uintptr_t)Scanner_Hook, 6);
}

void Hook_PlayerEvents(){
	Scanner_Init();
}