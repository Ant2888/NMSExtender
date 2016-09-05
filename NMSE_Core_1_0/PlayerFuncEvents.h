#pragma once
#include <vector>

enum PLAYER_FUNCTIONS{
	PLAYER_SCAN
};

typedef void(*_ScannerFunc)(uint64_t, int, int, float, uint64_t, char, uint64_t);

class PlayerFuncEvents{
public:
	PlayerFuncEvents();
	~PlayerFuncEvents();

	static void RegisterForScanEvent(_ScannerFunc);

	//Do not call this... ever
	void NotifyListeners(PLAYER_FUNCTIONS func, ...);

};

extern PlayerFuncEvents global_PlayerListener;