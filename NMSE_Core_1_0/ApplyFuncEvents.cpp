#include "ApplyFuncEvents.h"

ApplyFuncEvents global_ApplyListener;

ApplyFuncEvents::ApplyFuncEvents(){
	//
}

typedef std::vector<_ApplyFuncEvent> Listener;
static Listener observers;


ApplyFuncEvents::~ApplyFuncEvents(){
	for (size_t i = 0; i < observers.size(); i++){
		observers.pop_back();
	}
}


void ApplyFuncEvents::RegisterForEvent(_ApplyFuncEvent func){
	observers.push_back(func);
}

void ApplyFuncEvents::NotifyListeners(){
	for (size_t i = 0; i < observers.size(); i++){
		observers[i]();
	}
}