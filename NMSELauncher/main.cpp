#include "NMSE_Libs\Hooking.h"
#include "NMSE_Libs\Steam.h"

// Have to give large credit to the F4SE/SKSE/OBSE etc.. crew. This first pre-alpha is VERY LARGELY based off their SE's.

//275850 -- game id
int main(int argc, char* argv) {
	// CURRENTLY ONLY WORKS WITH STEAM (NOT GOG) -- should be an easy change to make it work
	const char* dllName = "NMSE Core.dll";
	std::string prgmName = "NMS.exe";
	std::string curPath = RunTimePath();
	if (!CheckSteam()) {
		std::cout << "Steam not open... Launching\n";
		LaunchSteam();
	}

	std::cout << "Staring NMS\n";

	std::string exe = curPath + "\\" + prgmName;

	const char* nmsId = "275850";
	SetEnvironmentVariable("SteamGameId", nmsId);
	SetEnvironmentVariable("SteamAppID", nmsId);

	STARTUPINFO startup = { 0 };
	PROCESS_INFORMATION nmsProc = { 0 };

	if (!CreateProcess(exe.c_str(), NULL, NULL, NULL, false,
		CREATE_SUSPENDED, NULL, NULL, &startup, &nmsProc)) {
		std::cout << "Failed to start NMS: " << GetLastError() << std::endl;
		std::cout << "If 740 please try running as Admin\n";
	}

	//inject the dll via the steam dll
	std::string NMSEsteam = curPath + "\\NMSE_steam.dll";
	bool isInjected = InjectDLLThread(&nmsProc, NMSEsteam.c_str(), true, false);
	if (isInjected) {
		std::cout << "Steam loader injected...\n";
	}
	else {
		std::cout << "Shutting down\n";
	}

	if (!ResumeThread(nmsProc.hThread)) {
		std::cout << "Thread resume failed.\n";
	}

	std::cout << "\nProgram Injected Everything Succesfully!\n";
	CloseHandle(nmsProc.hProcess);
	CloseHandle(nmsProc.hThread);
	std::cin.ignore();
	return 0;
}
