#include "EventManager.h"

EventManager global_EventManager;

EventManager::EventManager(){
	//
}
EventManager::~EventManager(){
}

void EventManager::RegisterForApplyEvents(void(*functionToCall)()){
	global_ApplyListener.RegisterForEvent((_ApplyFuncEvent)functionToCall);
}
