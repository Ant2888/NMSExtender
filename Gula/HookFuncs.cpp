#include "HookFuncs.h"
#include "xbyak/xbyak.h"

typedef void(*_FuncImHooking)(void); //if you know the params you can just put void*'s for as many as there are
_FuncImHooking WhereToJumpBack = nullptr; //need this so we can go back
VAddr<_FuncImHooking> AreaToPatchJMP(
	MemoryManager::FindPattern("\x85\xC0\x75\x0B\x69\x47\x78\xFA\x00\x00\x00", "xxxxxxxxxxx", 0x200000)
	- VAManager::baseAddr); //I recommend you look into FindPattern in MemoryManager

void Function_Hook(/*you would mimic the params here*/){
	//Since you are writting all your code in x64 you can just do this one thing
	WhereToJumpBack();
}

void WriteHook(HANDLE hdl){
	//you dont need the exit label you can just jump to retLbl
	//The patch also replaces the next ATLEAST 5 byte instruction so 
	//If there are two 3 byte instructions it will replace them so YOU have to write them back
	//Im still testing it though, sometimes it does sometimes it doesnt you'll have to look how 
	//many it actually takes in cheat engine. Oddly enough the jump is 6 bytes :P
	struct HookCode : Xbyak::CodeGenerator{
		HookCode(void* buffer) : Xbyak::CodeGenerator(4096, buffer){
			Xbyak::Label retLbl, storetestLbl, isprodLbl, issubLbl;
			//So when we're here we are at the beginning of new mem you can replace the code you patched out here
			
			// At this point, eax contains ENUM stack.type
			// If eax is 1, then stack type is Equipment, we don't want to change the charge size, thus return
			test(rax, 1);
			jne(&retLbl);
			// If eax is 0, then stack type is Substance, we want to change this to whatever amount is set in issubLbl
			test(rax, rax);
			je(issubLbl);

			// If eax is 2, then stack type is Product, we want to change this to whatever amount is set in isprodLbl
			L(isprodLbl);
			mov(rax, 0x64);

			// Then we test what the ENUM storage.type is
			jmp(&storetestLbl); //not 100% how to do only near jumps atm. havent played with it alot

			L(issubLbl);
			mov(rax, 0x1F4);

			// We check if the storage type is spaceship, if so, we multiply its stack size by 3
			// Otherwise the storage type is exosuit, and thus no change
			L(storetestLbl);
			test(byte[rdi + 0x78], 1);
			jne(&retLbl);
			//or replace it here. Or just don't ever replace it, doesn't really matter
			// as long as you're not getting rid of like stack information
			imul(eax, eax, 0x3);

			L(retLbl);
			dq(AreaToPatchJMP.GetUIntPtr() + 6); //replace x with how many instructions ahead you want to return to
			//you might have to make a couple retLbl's depending on certain instructions but it's all up to you

		}
	};

	void* mem = local_Memory.FirstWrittableAddr();
	HookCode hook(mem);
	local_Memory.CalcAllocated(hook.getCurr());

	WhereToJumpBack = (_FuncImHooking)mem;

	global_Memory.PatchBranch(AreaToPatchJMP.GetUIntPtr(), (uintptr_t)Function_Hook);
}

uintptr_t findStack(){
	/*
		stack max size constant aob: 85 C0 75 0B 69 47 78 FA 00 00 00

		Pseudocode:

		ENUM stack.type:
			0 SUBSTANCE (Carbon, Iron, etc.)
			1 EQUIPMENT (Thruster charge, etc)
			2 PRODUCT	(Warp Cell, etc)

		ENUM storage.type:
			1 EXOSUIT
			2 SPACESHIP

		OnSaveLoad OR OnCreateNewItemStack:
			if (stack.type == 0):
				stack.maxAmount = storage.type * 250
	*/
	uintptr_t funcAddr = MemoryManager::FindPattern("\x85\xC0\x75\x0B\x69\x47\x78\xFA\x00\x00\x00", "xxxxxxxxxxx", 0x200000);

	return funcAddr;
}