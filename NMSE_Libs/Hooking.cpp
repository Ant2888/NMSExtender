#include "Hooking.h"
#include <sys/stat.h>

std::string RunTimePath(void) {
	char buf[MAX_PATH];
	GetModuleFileNameA(NULL, buf, MAX_PATH);
	return std::string(buf).substr(0, std::string(buf).find_last_of("\\/"));
}

bool CheckFile(std::string fullPath){
	struct stat buffer;
	return (stat(std::string(fullPath).c_str(), &buffer) == 0);
}

static bool DoInjectDLLThread(PROCESS_INFORMATION * info, const char * dllPath, bool sync, bool noTimeout) {
	bool result = false;
	HANDLE	process = OpenProcess(
		PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, info->dwProcessId);
	if (process) {
		uintptr_t hookBase = (uintptr_t)VirtualAllocEx(process, NULL, 8192, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (hookBase) {
			// safe because kernel32 is loaded at the same address in all processes
			// (can change across restarts)
			uintptr_t LoadLibAdr = (uintptr_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
			WriteProcessMemory(process, (LPVOID)hookBase, dllPath, strlen(dllPath) + 1, NULL);
			HANDLE thread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAdr, (LPVOID)hookBase, NULL, NULL);
			if (thread) {
				if (sync) {
					switch (WaitForSingleObject(thread, noTimeout ? INFINITE : 1000 * 60))	// timeout = one minute
					{
					case WAIT_OBJECT_0:
						std::cout << "Hook Sucessfull...\n";
						result = true;
						break;
					case WAIT_ABANDONED:
						std::cout << "Hook failed... Abandoned\n";
						break;
					case WAIT_TIMEOUT:
						std::cout << "Hook failed... Timed out\n";
						break;
					}
				}
				else {
					result = true;
				}

				CloseHandle(thread);
			}
			VirtualFreeEx(process, (LPVOID)hookBase, 0, MEM_RELEASE);
		}
		CloseHandle(process);
	}
	return result;
}

bool InjectDLLThread(PROCESS_INFORMATION * info, const char * dllPath, bool sync, bool noTimeout) {
	bool result = false;
	__try {
		result = DoInjectDLLThread(info, dllPath, sync, noTimeout);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		std::cout << "Exeception caught. Hook failed... Check your virus protection softwares and or Firewall!!";
		result = false;
	}
	return result;
}

void** GetImportFunctionAddress(const char* DLL, const char* function, HMODULE mod){
	if (!mod) mod = GetModuleHandle(NULL);
	//setup to read dos file structures
	PIMAGE_DOS_HEADER pHead = (PIMAGE_DOS_HEADER)mod;
	PIMAGE_NT_HEADERS pNT = (PIMAGE_NT_HEADERS)((LPBYTE)pHead + pHead->e_lfanew);
	PIMAGE_IMPORT_DESCRIPTOR pDesc = (PIMAGE_IMPORT_DESCRIPTOR)((LPBYTE)pHead 
		+ pNT->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	//don't really need to check the sig 
	while (pDesc->Characteristics){
		if (!_stricmp(DLL, (char*)(mod + pDesc->FirstThunk))){

			PIMAGE_THUNK_DATA tData = (PIMAGE_THUNK_DATA)(pHead + pDesc->OriginalFirstThunk);
			uintptr_t* table = (uintptr_t*)(pHead + pDesc->FirstThunk);

			while (tData->u1.Ordinal){

				if (!IMAGE_SNAP_BY_ORDINAL(tData->u1.Ordinal)){
					PIMAGE_IMPORT_BY_NAME info = (PIMAGE_IMPORT_BY_NAME)(pHead + tData->u1.AddressOfData);
					if (!_stricmp(function, (char*)(info->Name))){
						return (void**)table;
					}
				}
				++tData;
				++table;
			}
			return NULL; //function wasn't found in this DLL
		}
		++pDesc;
	}
	return NULL;
}