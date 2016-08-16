#pragma once
#include <string>
#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>

bool SteamVersion(std::string runtimeDir);
bool CheckSteam();
void LaunchSteam();
