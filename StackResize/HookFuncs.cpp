#include "HookFuncs.h"
#include "xbyak/xbyak.h"

//if you know the params you can just put void*'s for as many as there are
typedef void(*_FuncImHooking)(void); 

//I recommend you look into FindPattern in MemoryManager
VAddr<_FuncImHooking> AreaToPatchJMP(findStack() - VAManager::baseAddr); 


void WriteHook(SizeSettings settings){
	//you dont need the exit label you can just jump to retLbl
	//The patch also replaces the next ATLEAST 5 byte instruction so 
	//If there are two 3 byte instructions it will replace them so YOU have to write them back
	//Im still testing it though, sometimes it does sometimes it doesnt you'll have to look how 
	//many it actually takes in cheat engine. Oddly enough the jump is 6 bytes :P
	struct HookCode : Xbyak::CodeGenerator{
		HookCode(void* buffer, SizeSettings SETTINGS) : Xbyak::CodeGenerator(2048, buffer){
			Xbyak::Label retLbl, storetestLbl, isprodLbl, issubLbl, ischargeLbl;
			//So when we're here we are at the beginning of new mem you can replace the code you patched out here

			// At this point, eax contains ENUM stack.type
			// If eax is 1, then stack type is Equipment, we don't want to modify its value
			test(eax, 1);
			jne(ischargeLbl);
			// If eax is 0, then stack type is Substance, we want to change this to whatever amount is set in issubLbl
			test(eax, eax);
			je(issubLbl);

			// If eax is 2, then stack type is Product, we want to change this to whatever amount is set in isprodLbl
			L(isprodLbl);
			mov(eax, SETTINGS.DEFAULT_ITEM_SIZE);

			// Then we test what the ENUM storage.type is
			jmp(storetestLbl); //not 100% how to do only near jumps atm. havent played with it alot

			L(issubLbl);
			mov(eax, SETTINGS.DEFAULT_SUBS_SIZE);

			// We check if the storage type is spaceship, if so, we multiply its stack size by 3
			// Otherwise the storage type is exosuit, and thus no change
			L(storetestLbl);
			test(byte[rdi + 0x78], 1);
			jne(retLbl);
			//or replace it here. Or just don't ever replace it, doesn't really matter
			// as long as you're not getting rid of like stack information
			imul(eax, eax, SETTINGS.SHIP_MULT);
			jmp(retLbl);

			L(ischargeLbl);
			mov(eax, SETTINGS.CHARGE_VAL);
			//If item is equipment and change is specified, then return directly
			cmp(eax, 0xFFFFFFFF);
			jne(retLbl);

			//If item is equipment and no change is specified, then we need to skip the next block after returning
			mov(eax, 1); 
			add(dword[rsp], 0x4);
			
			L(retLbl);
			// offset the stack pointer (which points to caller address) by 5
			add(dword[rsp], 0x5);
			ret();
		}
	};

	void* mem = local_Memory->FirstWrittableAddr();
	HookCode hook(mem, settings);
	local_Memory->CalcAllocated(hook.getCurr());

	global_Memory->CallBranch(AreaToPatchJMP.GetUIntPtr(), (uintptr_t)mem);
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
	return MemoryManager::FindPattern("\x85\xC0\x75\x0B\x69\x47\x78\xFA\x00\x00\x00", "xxxxxxxxxxx", 0x200000);
}