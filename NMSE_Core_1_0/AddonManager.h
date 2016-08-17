#pragma once	
#include "ModAPI.h"
#include <vector>


class AddonManager{
public:
	AddonManager();
	~AddonManager();
	void Init();
	void UnLoad();
	VERSION GetNMSVersion();
private:
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