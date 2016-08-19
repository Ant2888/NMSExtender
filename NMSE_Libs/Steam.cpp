#include "Steam.h"

std::string GetRegKey(HKEY root, const char * path, char * value) {
	std::string	result;
	HKEY key;
	// search through the reg for the key
	LONG erCode = RegOpenKeyExA(root, path, 0, KEY_READ, &key);
	if (!erCode) {
		DWORD dataLen = 0;
		erCode = RegQueryValueExA(key, value, NULL, NULL, NULL, &dataLen);
		if (!erCode) {
			result.resize(dataLen);
			erCode = RegQueryValueExA(key, value, NULL, NULL, (BYTE *)&result[0], &dataLen);
			if (!erCode) {
				if (result.length() > 0)
					result.resize(result.length() - 1);
			}
		}
		RegCloseKey(key);
	}
	return result;
}

bool CheckSteam() {
	//grabs a snap shot of all your processes and determines if steam is open
	bool toRet = false;
	HANDLE ss32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (ss32 != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 stmProc;
		stmProc.dwSize = sizeof(stmProc);
		if (Process32First(ss32, &stmProc)) {
			do {
				if (!_stricmp(stmProc.szExeFile, "steam.exe")) {
					toRet = true;
					break;
				}
			} while (Process32Next(ss32, &stmProc));
		}
		else {
			toRet = false;
		}
	}
	CloseHandle(ss32);
	return toRet;
}

void LaunchSteam() {
	std::string root = GetRegKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Valve\\Steam", "InstallPath");
	if (root.empty()) {
		std::cout << "Couldn't find steam path in reg\n";
	}
	else {
		std::string exe = root + "\\Steam.exe";

		STARTUPINFOA startup = { 0 };
		PROCESS_INFORMATION steamProc = { 0 };
		startup.cb = sizeof(startup);

		if (!CreateProcessA(exe.c_str(), NULL, NULL, NULL, false,
			0, NULL, root.c_str(), &startup, &steamProc)) {
			std::cout << "Failed to start steam: " << GetLastError() << std::endl;
		}
		WaitForInputIdle(steamProc.hProcess, INFINITE); // have to wait for steam to ACTUALLY start
		Sleep(5000); // static delay after it's prepped

		CloseHandle(steamProc.hProcess);
		CloseHandle(steamProc.hThread);
		std::cout << "Steam sucesfully started!\n";
	}
}

//You can give and Runtime as long as it is the full path
bool SteamVersion(std::string runtimeDir){
	if (runtimeDir.find("GOG") != std::string::npos){
		return false;
	}
	return true;
}