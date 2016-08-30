#pragma once
#include "Hooking.h"
#include "VirtualWrite.h"
#include "VirtualAddress.h"
#include <Psapi.h>
#include <vector>
#include <xbyak\xbyak.h>


// Some creds to Shadwar for helping me decide what method I was going to use for the patcher

//Currently if you do not allocate enough memory using a specific instance of this class you must initialize another one!

//Defines the base addr the image should be able to hold (plus some buffer)
#define sint32_t signed long
#define JMPDF	 0x25
#define	CALLDF   0x15
#define MINPROC(x) x - (INT32_MAX + (1024*1024*128ULL))

//AM is not working atm, need to play with it

/**struct AllocatedMemory{
	AllocatedMemory()
		:addr(nullptr)
		,memAllocated(0)
		,bytesWritten(0){
		//
	};

	
};**/

/**struct RTMemory{
	//The address this memory begins
	void* firstWrittableAddr;
	//The bytes that are left in the block
	size_t remaining;
};**/

class MemoryManager{
	public:
		MemoryManager();
		~MemoryManager();

		//Finds the byte pattern address. Only use the speed offsets if you know around where it should be.
		//This will potentially fail (return null) across new versions/updates.
		//example: FindPattern("\x2E\xF3\x0F\x10\x0C\x25\xEC\x02\x6D\x08", "xxxxxxx?xx", 0x010000, 0x050000); The 0x050000 subtracts from the imagesize
		static uintptr_t FindPattern(char* pattern, char* mask, DWORD speedUpMinOffset = 0, DWORD speedUpMaxOffset = 0);

		//Frees the mem allocated
		static bool Destroy(MemoryManager*);
		bool Destroy();

		//Attemps to allocate memory inside the space created by creatememory
		//This essentially just keeps the AllocatedMem struct up to date.
		//If you didn't call it you'd just need to make sure the struct is up to date.
		//Returns address to use -- should be followed up with a CalcAlloced
		void* AllocateSpace(size_t bytes);

		//Use for xbyak
		void CalcAllocated(const void* endAddr);
		
		//Allocated memory at the lowest FREE location in the image space
		bool CreateMemory(size_t bytesToAlloc, HANDLE module = NULL);

		//Returns the space remaining for the currentlly allocated mem
		size_t RemainingSpace();

		//Grabs information about the allocated memory space -- NULL if the space is undefined
		//static void QueryAllocatedMemory(AllocatedMemory* in, RTMemory* out); -- NYI needs AM to work

		//Patches a JMP from src to rtMem nopping len bytes
		//Keep len at 6 for this build (I am not responsible for what can happen otherwise:) )
		bool PatchBranch(uintptr_t src, uintptr_t rtMem, size_t len = 6);

		//Calls rtMem from src, this keeps all regs intact while setting a return address for the code at rtMem to call ret for
		bool CallBranch(uintptr_t src, uintptr_t rtMem, size_t len = 6);

		void* FirstWrittableAddr();
	private:
		/**Struct containing all the AllocatedMemory details
		AllocatedMemory* lastWritten;
		AllocatedMemory container;**/

		bool ExecBranch(uintptr_t src, uintptr_t dst, byte command, size_t len);

		//The address AM was allocated at
		void* m_addr;
		//The bytes that AM can hold
		size_t m_memAllocated;
		//The bytes that the AM has taken up
		size_t m_bytesWritten;
		//The handle for where to start allocation
		void* m_locAlloc;
};

extern MemoryManager *global_Memory;
extern MemoryManager *local_Memory;