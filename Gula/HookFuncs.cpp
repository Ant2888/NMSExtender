#include "HookFuncs.h"

uintptr_t findStack(){
	/*
		stack max size constant aob: 69 47 78 FA 00 00 00 89 44 24

		NMS(exp).exe+422262 - 69 47 78 FA000000     - imul eax,[rdi+78], 000000FA // 250

		Pseudocode:

		ENUM storage.type:
			1 EXOSUIT
			2 SPACESHIP

		OnSaveLoad OR OnCreateNewItemStack
			if !isProduct(stack) =>
				stack.maxAmount = storage.type * 250
	*/
	uintptr_t funcAddr = MemoryManager::FindPattern("\x69\x47\x78\xFA\x00\x00\x00\x89\x44\x24", "xxxxxxxxxx", 0x200000);

	return funcAddr;
}