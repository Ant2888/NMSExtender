#include "Hooking.h"

std::string RunTimePath(void) {
	char buf[MAX_PATH];
	GetModuleFileNameA(NULL, buf, MAX_PATH);
	return std::string(buf).substr(0, std::string(buf).find_last_of("\\/"));
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
						std::cout << "Hook Sucessfull!\n";
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

/*void* GetIATAddr(void* module, const char* searchDllName, const char* searchImportName) {
	UINT8* base = (UINT8*)module;
	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER *)base;
	IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS *)(base + dosHeader->e_lfanew);
	IMAGE_IMPORT_DESCRIPTOR* importTable =
		(IMAGE_IMPORT_DESCRIPTOR *)(base + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	for (; importTable->Characteristics; ++importTable)
	{
		const char	* dllName = (const char *)(base + importTable->Name);
		if (!_stricmp(dllName, searchDllName))
		{
			IMAGE_THUNK_DATA	* thunkData = (IMAGE_THUNK_DATA *)(base + importTable->OriginalFirstThunk);
			uintptr_t			* iat = (uintptr_t *)(base + importTable->FirstThunk);

			for (; thunkData->u1.Ordinal; ++thunkData, ++iat)
			{
				if (!IMAGE_SNAP_BY_ORDINAL(thunkData->u1.Ordinal))
				{
					IMAGE_IMPORT_BY_NAME	* importInfo = (IMAGE_IMPORT_BY_NAME *)(base + thunkData->u1.AddressOfData);

					if (!_stricmp((char *)importInfo->Name, searchImportName))
					{
						return iat;
					}
				}
			}
			return NULL;
		}
	}

	return NULL;
}*/
