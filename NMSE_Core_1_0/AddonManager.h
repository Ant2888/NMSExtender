#pragma once	
#include "NMSE_Libs\MemoryManager.h"	
#include "NMSE_Libs\Hooking.h"
#include "NMSE_Libs\ModIterator.h"
#include "NMSE_Libs\Steam.h"
#include "EventManager.h"
#include "ModAPI.h"
#include "RegisterCallbacks.h"
#include <vector>



class AddonManager{
public:
	AddonManager();
	~AddonManager();
	void Init();
	void UnLoad();
	VERSION GetNMSVersion();
	void SetMainDLL(HANDLE);
private:
	HANDLE m_mainDLL;

	struct MOD{
		HMODULE mHandle;
		ModDetails modDetails;
		_OnStart startUp;
	};

	void LoadMods(void);
	bool CallStart(MOD&);

	std::string modDir;

	typedef std::vector<MOD> ModList;
	ModList m_mods;

	MOD *curMod;

};

extern AddonManager modManager;