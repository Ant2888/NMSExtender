#include "AddonManager.h"
#include "NMSE_Libs\Hooking.h"
#include "NMSE_Libs\ModIterator.h"
#include "NMSE_Libs\Steam.h"

AddonManager modManager;

AddonManager::AddonManager(){}

AddonManager::~AddonManager(){
	UnLoad();
}

VERSION AddonManager::GetNMSVersion(){
	if (SteamVersion(RunTimePath())){
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
			mod.startUp = (_OnStart)GetProcAddress(mod.mHandle, "OnStart");
			if (mod.startUp){
				if (!CallStart(mod)){
					std::string err();
					MessageBox(0, "Mod Failed To Load!", mIter.GetFullPath().c_str(), MB_ICONWARNING | MB_OK);
				}
				else{
					loaded = true;
				}
			}

		}
		else{
			std::string err(mIter.GetFullPath());
			MessageBox(0, "The mod isn't a valid NMSE dll", err.c_str(), MB_ICONWARNING | MB_OK);
		}
		if (loaded){
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
		if (!mod.startUp(mod.mHandle, mod.modDetails)) return false;
		return true;
	}
	__except (1){
		return false;
	}
	return false;
}