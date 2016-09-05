#include "PlayerFuncEvents.h"

PlayerFuncEvents global_PlayerListener;

PlayerFuncEvents::PlayerFuncEvents(){
	//
}

typedef std::vector<_ScannerFunc> Listener;
static Listener scannerObs;

PlayerFuncEvents::~PlayerFuncEvents(){
	for (size_t i = 0; i < scannerObs.size(); i++){
		scannerObs.pop_back();
	}
}


void PlayerFuncEvents::RegisterForScanEvent(_ScannerFunc func){
	scannerObs.push_back(func);
}

struct ScanArgs{
	uint64_t arg1;
	int arg2;
	int arg3;
	float arg4;
	uint64_t arg5;
	char arg6;
	uint64_t arg7;
};

void PlayerFuncEvents::NotifyListeners(PLAYER_FUNCTIONS toNotify, ...){
	if (toNotify == PLAYER_SCAN){
		va_list pList;
		va_start(pList, 7);

		ScanArgs args = { va_arg(pList, uint64_t), va_arg(pList, int), va_arg(pList, int), va_arg(pList, float),
			va_arg(pList, uint64_t), va_arg(pList, char), va_arg(pList, uint64_t) };

		va_end(pList);

		for (size_t i = 0; i < scannerObs.size(); i++){
			scannerObs[i](args.arg1, args.arg2, args.arg3, args.arg4, args.arg5, args.arg6, args.arg7);
		}
	}
}