#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>


static bool DoInjectDLLThread(PROCESS_INFORMATION * info, const char * dllPath, bool sync, bool noTimeout);
bool InjectDLLThread(PROCESS_INFORMATION * info, const char * dllPath, bool sync, bool noTimeout);
void* GetIATAddr(void* module, const char* dllName, const char* importName);
std::string RunTimePath(void);