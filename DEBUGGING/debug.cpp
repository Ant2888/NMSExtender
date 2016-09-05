#include "NMSE_Core_1_0\ModAPI.h"
#include "NMSE_Libs\Hooking.h"
#include "NMSE_Libs\MemoryManager.h"
#include <io.h>
#include <fcntl.h>


HMODULE localH;
HANDLE hMonitor;
VERSION vers; 
HANDLE _wndThread;
bool consActive = false; 
DWORD WINAPI WndThread(LPVOID lpThreadParameter); // thread method

void ApplyFunctionUsed(){
	std::cout << "TEST\n";
}

typedef enum WRProps{
	SC_READ = (1U),
	SC_WRITE = (1U << 1),
	SC_APPND = (1U << 2),
	SC_CREATE = (1U << 3)
};

typedef int32_t SceFiosFH;

//returns 0 if not valid
typedef __int64(*sceFiosSizeSync)(void* nullVal, const char* path);

//probably returns the number of bytes written
typedef __int64(*sceFiosReadSync)(void* nullVal, const char* path, void* buffer, size_t lengthToRead);

//no idea what it rets
typedef void(*sceFiosWrite)(void* nullVal, SceFiosFH fileHandle, const void* pBuffer, size_t length);

//opens a fh with given wrparams
typedef void(*sceFiosOpen)(void* nullVal, SceFiosFH* handleOut, const char* path, unsigned int* wrparams);

typedef void(*ExportBiomeData)(__int64 biomeStruc, __int64 zeroVal, const char* templateName);
VAddr <ExportBiomeData> expBiome(0x0388450);

typedef void(*ExportCkLocs)(__int64 locStruc, __int64 zeroVal, const char* templateName);
VAddr <ExportCkLocs> expCkLocs(0x0BFC210);


typedef void(*ExportGalaxyVoxel)(__int64 locStruc, __int64 zeroVal, const char* templateName);
VAddr <ExportGalaxyVoxel> expGV(0x0347280);


typedef void(*binariesTest)(__int64 locStruc, __int64 zeroVal, const char* templateName);
VAddr <binariesTest> binTest(0x0BDBEC0);


typedef void(*expReward)(__int64 locStruc, __int64 zeroVal, const char* templateName);
VAddr <expReward> expRTest(0x02F5930);

void HookLib(){
	__int64 loadedIn = 0x1FB9CF0 + VAManager::baseAddr; //ckLocs actual

	__int64 testLoad = 0x16104D0 + VAManager::baseAddr; //GcGalaxyGlobals.global.mbin

	/*std::string path = RunTimePath();
	path = path.substr(0, path.find_last_of("\\/")) + "\\GAMEDATA\\";

	HMODULE pMod;
	GetDllModule("libSceFios2.dll", pMod);


	sceFiosSizeSync sfs = (sceFiosSizeSync)GetProcAddress(pMod, "sceFiosFileGetSizeSync");
	sceFiosReadSync sfr = (sceFiosReadSync)GetProcAddress(pMod, "sceFiosFileReadSync");
	sceFiosWrite sfw = (sceFiosWrite)GetProcAddress(pMod, "sceFiosFHWriteSync");
	sceFiosOpen sfo = (sceFiosOpen)GetProcAddress(pMod, "sceFiosFHOpenSync");

	SceFiosFH fHandle;
	unsigned int flags = SC_READ | SC_WRITE | SC_CREATE;
	WRITEMSG("CALLING SFO, PARAMS: ");
	sfo(0, &fHandle, std::string(RunTimePath()+"\\NMSE\\TESTING_SCE.txt").c_str(), &flags);
	char buff[33];
	_itoa_s(fHandle, buff, 10);
	WRITEMSG(buff);

	WRITEMSG("\nFH CREATED:: Calling SFW\n");
	sfw(0, fHandle, "TEST", 6);
	*/

	/*TEST FOR SFS
	WRITEMSG("CALLING sfs\n");
	__int64 test = sfs(0i64, std::string(path + "GCAISPACESHIPGLOBALS.GLOBAL.MBIN").c_str());
	char buff[100]; 
	_i64toa_s(test, buff, 100, 10);	
	  WRITEMSG(buff)*/

}

DWORD WINAPI ThreadProc(LPVOID threadParam){
	_wndThread = CreateThread(0, 0, WndThread, 0, 0, 0);

	while (!consActive) Sleep(1200);

	//uint32_t rdx[2];
//	int r8;
	//uint32_t r9[2];
	/*while (true){
		std::cout << "Press anything to call func: ";
		__int64 val = 0;
		std::cin >> val;
		if (!val)
			break;
		HookLib();
	}*/
	//typedef void(*_RechargeFunc)(uint64_t* gameStruct, uint64_t rdxVal, int r8Val, uint64_t r9Val);
	//VAddr<_RechargeFunc> RechargeFunc(0x043C4D0);
	/**bool flag = true;
	while (flag){
		std::cout << "Press anything to generate: ";
		int val = 0;
		std::cin >> val;
		if (!val)
			exit(0);
		uint64_t* rcx = (uint64_t*)(VAManager::baseAddr + 0x1616728);
		//rcx += 0x350;
		uint64_t* fstStep = (uint64_t*)(*rcx + 0x430);
		uint64_t secStep = *(fstStep + 0x28);
		uint64_t thrdStep = secStep + 0x30;
		uint64_t* lifeSupport = (uint64_t*)(thrdStep + 0x18);


		std::cout << "RCXADDR:" << std::hex << *rcx << std::endl;
		std::cout << "LIFESUPPORTVAL: " << lifeSupport << std::endl;
		Sleep(500);
	}**/
	CloseHandle(hMonitor);
	return 0;
}

void PlayerScanned(uint64_t a1, int a2, int a3, float a4, uint64_t a5, char a6, uint64_t a7){
	std::cout << "PLAYER SCANNED\n";
}

extern "C"
{
	bool OnStart(HMODULE& mHandle, ModDetails& info){
		localH = mHandle;
		info.name = "TESTING PATCHER";
		vers = info.version; // Which game version (GOG or STEAM) this DLL is injected into
		// Do whatever you want from here on -- send out a thread to do constant monitoring. Only do patches or just start a thread for patching
		// If this gets held up the rest of the program will fault
		global_Logger.OpenFile(std::string(RunTimePath() + "\\NMSE\\DEBUGGER.txt").c_str(), false);
		hMonitor = CreateThread(0, 0, ThreadProc, 0, 0, 0);
		return true;
	}

	void RegisterForPlayerScan(_ScannerListener RegisterFunction){
		(*RegisterFunction)(PlayerScanned);
	}
};


DWORD WINAPI WndThread(LPVOID lpThreadParameter){
	AllocConsole();
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
	consActive = true;
	CloseHandle(_wndThread);
	return 0;
}