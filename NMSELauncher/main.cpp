#include "NMSE_Libs\Hooking.h"
#include "NMSE_Libs\Steam.h"
#include "NMSE_Libs\VersionControl.h"

// Have to give large credit to the F4SE/SKSE/OBSE etc.. crew. This first pre-alpha is VERY LARGELY based off their SE's.
#define DEBUG 0
//275850 -- game id
int main(int argc, char* argv) {
	// CURRENTLY ONLY WORKS WITH STEAM (NOT GOG) -- should be an easy change to make it work
	const char* dllName = "NMSE Core.dll";
	std::string prgmName = "NMS.exe";
	std::string curPath = RunTimePath();

	std::cout << "Using runtime: " << curPath << "\n";
	bool steam;
	if (SteamVersion(curPath)){
		steam = true;
		std::cout << "Using Steam Version\n";
		if (!CheckSteam()) {
			std::cout << "Steam not open... Launching\n";
			LaunchSteam();
		}
	}
	else{
		steam = false;
		std::cout << "Using GOG Version\n";
	}
	std::cout << "Staring NMS\n";

	std::string exe = curPath + "\\" + prgmName;

	//ValidateVersion(exe); -- waiting for VC implementation on HG's side :\
	

	if (steam){
		const char* nmsId = "275850";
		SetEnvironmentVariable("SteamGameId", nmsId);
		SetEnvironmentVariable("SteamAppID", nmsId);
	}

	STARTUPINFO startup = { 0 };
	PROCESS_INFORMATION nmsProc = { 0 };


	if (!CreateProcess(exe.c_str(), NULL, NULL, NULL, false,
		CREATE_SUSPENDED, NULL, NULL, &startup, &nmsProc)) {
		std::cout << "Failed to start NMS: " << GetLastError() << std::endl;
		std::cout << "If 740 please try running as Admin\n";
	}
#if DEBUG
	std::cout << "HOOK BASE: " << std::hex << GetModuleHandle(NULL) << std::endl;
	MessageBox(0, "APP SUSPENDED, HOOK AND PRESS OK", "CUSTOM NMSE", MB_OK | MB_ICONWARNING);
#endif
	//inject the dll via the steam dll
	std::string NMSEsteam = curPath + "\\NMSE_steam.dll";
	bool isInjected = InjectDLLThread(&nmsProc, NMSEsteam.c_str(), true, false);
	if (isInjected) {
		std::cout << "Loader injected...\n";
	}
	else {
		std::cout << "Loader Injection Failed\n";
	}

	if (!ResumeThread(nmsProc.hThread)) {
		std::cout << "Thread resume failed.\n";
	}

	std::cout << "\nProgram Injected Everything Succesfully!\n";
	CloseHandle(nmsProc.hProcess);
	CloseHandle(nmsProc.hThread);
	Sleep(10000);
	return 0;
}
