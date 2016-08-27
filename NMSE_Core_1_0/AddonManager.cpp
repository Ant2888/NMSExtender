#include "AddonManager.h"
#include "NMSE_Libs\Hooking.h"
#include "NMSE_Libs\ModIterator.h"
#include "NMSE_Libs\Steam.h"
#include "NMSE_Core_1_0\EventManager.h"
#include "NMSE_Core_1_0\ApplyFuncEvents.h"
#include "NMSE_Libs\MemoryManager.h"

AddonManager modManager;

AddonManager::AddonManager(){}

AddonManager::~AddonManager(){
	UnLoad();
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


void RegisterEvent(void(*paramFunc)()){
	global_EventManager.RegisterForApplyEvents(paramFunc);
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
					MessageBox(0, "Mod Failed to Start... Unloading It", mIter.GetFullPath().c_str(), MB_ICONWARNING | MB_OK);
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
			_RegisterForApplyEvents reg = (_RegisterForApplyEvents)GetProcAddress(mod.mHandle, "RegisterForApplyEvent");
			if (reg){
				reg(RegisterEvent);
			}
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
		if (!mod.startUp(mod.mHandle, mod.modDetails, local_Memory, global_Memory)) return false;
		return true;
	}
	__except (1){
		return false;
	}
	return false;
}



