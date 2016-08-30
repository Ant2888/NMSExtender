#include "MemoryManager.h"

MemoryManager *global_Memory = new MemoryManager();
MemoryManager *local_Memory = new MemoryManager();

MemoryManager::MemoryManager()
	: m_addr(nullptr)
	, m_locAlloc(nullptr)
	, m_memAllocated(0)
	, m_bytesWritten(0){
	//
};

MemoryManager::~MemoryManager(){
	Destroy();
};

bool MemoryManager::Destroy(MemoryManager* mem){
	if (mem){
		return mem->Destroy();
	}
	return true;
};

bool MemoryManager::Destroy(){
	if (m_addr){
		if (VirtualFree(m_addr, m_memAllocated, MEM_RELEASE)){
			m_addr = nullptr;
			return true;
		}
		return false;
	}
	return true;
};

uintptr_t MemoryManager::FindPattern(char* pattern, char* mask, DWORD speedUpMinOffset, DWORD speedUpMaxOffset){
	MODULEINFO modinfo = { 0 };
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &modinfo, sizeof(MODULEINFO));

	DWORD maskBuffer = (DWORD)strlen(mask);
	DWORD imgSize = ((DWORD)modinfo.SizeOfImage); 

	if (speedUpMaxOffset < imgSize)
		imgSize -= speedUpMaxOffset;
	if (speedUpMinOffset >= imgSize)
		speedUpMinOffset = 0;

	for (DWORD i = speedUpMinOffset; i < imgSize - maskBuffer; i++){
		bool located = true;
		for (DWORD j = 0; j < maskBuffer; j++){
			located &= (mask[j] == '?' || (pattern[j] == *(char*)(VAManager::baseAddr + i + j)));
		}
		if (located){
			return VAManager::baseAddr + i;
		}	
	}
	return NULL;
};


bool MemoryManager::CreateMemory(size_t bytesToAlloc, HANDLE module){
	uintptr_t baseMod;
	module ? baseMod = (uintptr_t)module : baseMod = (uintptr_t)GetModuleHandle(NULL);
	uintptr_t addrCount = baseMod;
	uintptr_t baseImg = MINPROC(baseMod);
	addrCount--;

	while (!m_addr){
		//Query the region
		MEMORY_BASIC_INFORMATION processMem;
		if (!VirtualQuery((LPCVOID)addrCount, &processMem, sizeof(processMem))){
			MessageBox(0, "FATAL ERROR: " + GetLastError(), "VIRTUAL QUERY", MB_OK | MB_ICONWARNING);
			return false;
		}
		//Check the region
		if (processMem.State == MEM_FREE){
			//Does the region have enough space
			if (processMem.RegionSize >= bytesToAlloc){
				//Try to assign the mem to vmem
				addrCount = ((uintptr_t)processMem.BaseAddress) + processMem.RegionSize - bytesToAlloc;

				void* addrHandle(nullptr);
				addrHandle = VirtualAlloc((void*)addrCount, bytesToAlloc, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				if (addrHandle){
					m_addr = addrHandle;
					m_memAllocated = bytesToAlloc;
					m_bytesWritten = 0;
					return true;
				}
			}
		}

		if (addrCount < baseImg){
			MessageBox(0, "No Free Space.\n FATAL ERROR", "ADC<IMG", MB_OK|MB_ICONWARNING);
			return false;
		}

		addrCount = ((uintptr_t)processMem.BaseAddress) - 1;
	}
	return false;
};

/*
void MemoryManager::QueryAllocatedMemory(AllocatedMemory* in, RTMemory* out){
	if (in){
		if (in->addr){
			RTMemory toRet = { 0 };
			toRet.firstWrittableAddr = ((uint8_t*)(in->addr)) + in->bytesWritten;
			toRet.remaining = in->memAllocated - in->bytesWritten;
			*out = toRet;
		}
	}
};
*/

size_t MemoryManager::RemainingSpace(){
	if (m_addr){
		return  m_memAllocated - m_bytesWritten;
	}
	return 0;
};


bool MemoryManager::PatchBranch(uintptr_t src, uintptr_t dst, size_t len){
	return ExecBranch(src, dst, JMPDF, len);
}

bool MemoryManager::CallBranch(uintptr_t src, uintptr_t dst, size_t len){
	return ExecBranch(src, dst, CALLDF, len);
}

bool MemoryManager::ExecBranch(uintptr_t src, uintptr_t dst, uint8_t command, size_t len){
	// need atleast 5 bytes to write the jump
	if (len < 5 || !m_addr){
		return false;
	}

	uintptr_t* rtMem = (uintptr_t*)(AllocateSpace(sizeof(void*)));

	if (!rtMem) return false;

	//grab the relative jmp
	uintptr_t rtMemAddr = (uintptr_t)rtMem;
	uintptr_t nextByte = src + 6; //adds the instr count for rip 
	ptrdiff_t ripDiff = rtMemAddr - nextByte;

	//null the bytes (your job to replace at the moment) -- use xbyak
	if (len > 6)
		VirtualSet(src, 0x90, len);

	uint8_t instr[6];
	instr[0] = 0xff;
	instr[1] = command;
	*((sint32_t*)&instr[2]) = (sint32_t)ripDiff;
	VirtualWrite(src, instr, sizeof(instr));

	*rtMem = dst;

	return true;
}

void* MemoryManager::AllocateSpace(size_t bytes){
	if (m_addr){
		if (bytes <= RemainingSpace()){
			m_locAlloc = ((uint8_t*)(m_addr)) + m_bytesWritten;
			m_bytesWritten += bytes;
			return m_locAlloc;
		}
	}
	return 0;
};

void MemoryManager::CalcAllocated(const void* endAddr){
	if (m_locAlloc && endAddr){
		m_bytesWritten += ((uintptr_t)(endAddr))-((uintptr_t)(m_locAlloc));
		m_locAlloc = nullptr;
	}
};

void* MemoryManager::FirstWrittableAddr(){
	if (m_addr){
		if (!m_locAlloc){
			m_locAlloc = (uint8_t*)(m_addr) + m_bytesWritten;
			return m_locAlloc;
		}
	}
	return 0;
};