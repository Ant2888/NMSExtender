#pragma once
#include <vector>


typedef void(*_ApplyFuncEvent)();
class ApplyFuncEvents{
public:
	ApplyFuncEvents();
	~ApplyFuncEvents();

	static void RegisterForEvent(_ApplyFuncEvent);
	//Do not call this... ever
	void NotifyListeners();

};

extern ApplyFuncEvents global_ApplyListener;