#include "NMSE_Libs\Hooking.h"
#include "NMSE_Libs\Steam.h"
#include "NMSE_Libs\VersionControl.h"

// Have to give large credit to the F4SE/SKSE/OBSE etc.. crew; Most specifically Ian Patterson and Stephen Abel


#define DEBUG 0
//275850 -- game id
int main(int argc, char* argv) {
	const char* dllName = "NMSE Core.dll";
	std::string prgmName = "NMS.exe";
	std::string curPath = RunTimePath();

	if (CheckFile(curPath + "\\opengl32.dll")){ 
		if (rename(std::string(curPath + "\\opengl32.dll").c_str(), std::string(curPath + "\\reshade64.dll").c_str())){
			MessageBox(0, "FAILED TO RENAME \"opengl32.dll\" PLEASE RENAME IT TO \"reshade64.dll\"", "NMSE: OPENGL32", MB_OK | MB_ICONWARNING);
			std::cout << "Failed to rename ogl32\n";
			return 0;
		}
		std::cout << "opengl32 Renamed to reshade64\n";
	}

	if (CheckFile(curPath + "\\OPENGL32.dll")){
		if (rename(std::string(curPath + "\\OPENGL32.dll").c_str(), std::string(curPath + "\\reshade64.dll").c_str())){
			MessageBox(0, "FAILED TO RENAME \"OPENGL32.dll\" PLEASE RENAME IT TO \"reshade64.dll\"", "NMSE: OPENGL32", MB_OK | MB_ICONWARNING);
			std::cout << "Failed to rename ogl32\n";
			return 0;
		}
		std::cout << "OPENGL32 Renamed to reshade64\n";
	}

	Sleep(1000);

	std::cout << "Using runtime: " << curPath << "\n";
	bool steam;
	if (CheckFile(curPath+"\\steam_api64.dll")){
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

	std::cout << "Booting NMS...\n";

	std::string exe = curPath + "\\" + prgmName;

	//Check if the user has name the file something else
	if (CheckFile(curPath + "\\" + "_" + prgmName)){
		exe = curPath + "\\" + "_" + prgmName;
	}

	//ValidateVersion(exe); -- waiting for VC implementation on HG's side :\
	

	if (steam){
		auto nmsId = "275850";
		SetEnvironmentVariable("SteamGameId", nmsId);
		SetEnvironmentVariable("SteamAppID", nmsId);
	}

	STARTUPINFO startup = { 0 };
	PROCESS_INFORMATION nmsProc = { 0 };

	if (!CreateProcess(exe.c_str(), NULL, NULL, NULL, false,
		CREATE_SUSPENDED, NULL, NULL, &startup, &nmsProc)) {
		std::cerr << "Failed to start NMS: " << GetLastError() << std::endl;
		std::cerr << "If 740 please try running as Admin\n";
	}

#if DEBUG
	std::cout << "HOOK BASE: " << std::hex << GetModuleHandle(NULL) << std::endl;
	MessageBox(0, "APP SUSPENDED, HOOK AND PRESS OK", "CUSTOM NMSE", MB_OK | MB_ICONWARNING);
#endif

	//inject the dll
	auto NMSEsteam = curPath + "\\NMSE_steam.dll";
	auto isInjected = InjectDLLThread(&nmsProc, NMSEsteam.c_str(), true, false);
	if (isInjected) {
		std::cout << "Loader injected...\n";
	}
	else {
		std::cerr << "Loader Injection Failed\n";
	}

	std::cout << "Resuming thread...\n";
	Sleep(2000); //Static sleep to make sure hooks happen and to avoid weird bugs possibly?
	if (!ResumeThread(nmsProc.hThread)) {
		std::cout << "Thread resume failed (ERR): " << GetLastError() << std::endl;
	}
#if DEBUG
	SuspendThread(nmsProc.hThread);
	std::cout << "aa BASE: " << std::hex << GetModuleHandle(NULL) << std::endl;
	MessageBox(0, "APP SUSPENDED, HOOK AND PRESS OK", "CUSTOM NMSE", MB_OK | MB_ICONWARNING);
	ResumeThread(nmsProc.hThread);
#endif

	std::cout << "\nProgram Injected Everything Succesfully!\n";
	std::cout << "Please wait for this to close before worrying :)\n\n";
	if (CheckFile(curPath + "\\ReShade64.dll") || CheckFile(curPath + "\\reshade64.dll")){
		std::cout << "You're using a shader. You might have to wait longer than usual!\n";
	}
	CloseHandle(nmsProc.hProcess);
	CloseHandle(nmsProc.hThread);
	Sleep(25000);
	return 0;
}
