#include "AddonManager.h"

AddonManager modManager;

AddonManager::AddonManager(){}

AddonManager::~AddonManager(){
	UnLoad();
}

void AddonManager::SetMainDLL(HANDLE h){
	m_mainDLL = h;
}

VERSION AddonManager::GetNMSVersion(){
	if (CheckFile(RunTimePath() + "\\steam_api64.dll")){
		return STEAM;
	}
	else{
		return GOG;
	}
}

void AddonManager::UnLoad(){
	for (ModList::iterator iter = m_mods.begin(); iter != m_mods.end(); ++iter){
		MOD* mod = &(*iter);
		if (mod->mHandle){
			FreeLibrary(mod->mHandle);
		}
	}
	m_mods.clear();
}

void AddonManager::Init(){
	modDir = RunTimePath() + "\\NMSE\\";
	LoadMods();
}





void AddonManager::LoadMods(void){
	m_mods.reserve(7);

	for (ModIterator mIter(modDir.c_str(), "*.dll"); !(mIter.Done()); mIter.Next()){
		bool loaded = false;
		std::string modPath = mIter.GetFullPath();
		MOD mod;
		memset(&mod, 0, sizeof(mod));
		curMod = &mod;
		mod.mHandle = (HMODULE)LoadLibrary(modPath.c_str());
		if (mod.mHandle){
			//check if the user is polling vmem
			_UseAllocMemory reg2 = (_UseAllocMemory)GetProcAddress(mod.mHandle, "GrabVirtualMem");
			if (reg2){
				reg2(&global_Memory, &local_Memory);
			}

			mod.startUp = (_OnStart)GetProcAddress(mod.mHandle, "OnStart");
			if (mod.startUp){
				if (!CallStart(mod)){
					std::string failMessage = "Mod: [" + mod.modDetails.name + "] Failed to Start... Unloading It";
					MessageBox(0, failMessage.c_str(), mIter.GetFullPath().c_str(), MB_ICONWARNING | MB_OK);
					FreeLibrary(mod.mHandle);
				}
				else{
					loaded = true;
				}
			}
			else{
				MessageBox(0, "Extern OnStart Not Found...\nLoading Failed!!!", mIter.GetFullPath().c_str(), MB_ICONWARNING | MB_OK);
				FreeLibrary(mod.mHandle);
			}

		}
		else{
			std::string err(mIter.GetFullPath());
			MessageBox(0, "The mod isn't a valid NMSE dll", err.c_str(), MB_ICONWARNING | MB_OK);
		}
		if (loaded){
			RegisterModForEvents(mod.mHandle);
			m_mods.push_back(mod);
		}
		else{
			FreeLibrary(mod.mHandle);
		}
	}
	std::string a = "" + m_mods.size();
}

bool AddonManager::CallStart(MOD& mod){
	__try{
		mod.modDetails.version = GetNMSVersion();
		mod.modDetails.dllHandle = m_mainDLL;
		if (!mod.startUp(mod.mHandle, mod.modDetails)) 
			return false;
		return true;
	}
	__except (1){
		return false;
	}
	return false;
}



