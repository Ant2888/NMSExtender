#include "EventManager.h"

EventManager global_EventManager;

EventManager::EventManager(){
	//
}
EventManager::~EventManager(){
}

void EventManager::RegisterForApplyEvents(void(*functionToCall)()){
	global_ApplyListener.RegisterForEvent((_ApplyFuncEvent)(functionToCall));
}

void EventManager::RegisterForPlayerScanEvents(void(*functionToCall)(uint64_t, int, int, float, uint64_t, char, uint64_t)){
	global_PlayerListener.RegisterForScanEvent((_ScannerFunc)functionToCall);
}
