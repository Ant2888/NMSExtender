#pragma once
#include "ApplyFuncEvents.h"
#include "PlayerFuncEvents.h"
#include <vector>


class EventManager{
public:
	EventManager();
	~EventManager();

	static void RegisterForApplyEvents(void (*functionToCall)());
	static void RegisterForPlayerScanEvents(void(*functionToCall)(uint64_t, int, int, float, uint64_t, char, uint64_t));
};

extern EventManager global_EventManager;