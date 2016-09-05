#include "RegisterCallbacks.h"

void RegisterApplyEvent(void(*paramFunc)()){
	global_EventManager.RegisterForApplyEvents(paramFunc);
}

void RegisterPlayerScan(void(*paramFunc)(uint64_t, int, int, float, uint64_t, char, uint64_t)){
	global_EventManager.RegisterForPlayerScanEvents(paramFunc);
}

void RegisterModForEvents(HMODULE mod){
	if (mod){
		_RegisterForApplyEvents reg1 = (_RegisterForApplyEvents)GetProcAddress(mod, "RegisterForApplyEvent");
		if (reg1){
			reg1(RegisterApplyEvent);
		}

		_RegisterForScannerEvent reg2 = (_RegisterForScannerEvent)GetProcAddress(mod, "RegisterForPlayerScan");
		if (reg2){
			reg2(RegisterPlayerScan);
		}
	}
}