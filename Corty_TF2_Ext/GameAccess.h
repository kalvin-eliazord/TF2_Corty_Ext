#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

namespace GameAccess
{
	HANDLE hProc{};
	bool SetHProc();
	intptr_t GetClientMod();
	intptr_t GetEngineMod();
};

