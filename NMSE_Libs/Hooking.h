#pragma once



static bool DoInjectDLLThread(PROCESS_INFORMATION * info, const char * dllPath, bool sync, bool noTimeout);
bool InjectDLLThread(PROCESS_INFORMATION * info, const char * dllPath, bool sync, bool noTimeout);

//void* GetIATAddr(void* module, const char* dllName, const char* importName); -- working on it

//Gives back the run time that the main executable is in with no backslash (...\No Man's Sky\Binaries)
std::string RunTimePath(void);

//Returns whether the files at fullPath exists
bool CheckFile(std::string fullPath);