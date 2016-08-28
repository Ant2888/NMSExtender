#include "RegisterCallbacks.h"

void RegisterApplyEvent(void(*paramFunc)()){
	global_EventManager.RegisterForApplyEvents(paramFunc);
}

void RegisterModForEvents(HMODULE mod){
	if (mod){
		_RegisterForApplyEvents reg1 = (_RegisterForApplyEvents)GetProcAddress(mod, "RegisterForApplyEvent");
		if (reg1){
			reg1(RegisterApplyEvent);
		}

		
	}
}