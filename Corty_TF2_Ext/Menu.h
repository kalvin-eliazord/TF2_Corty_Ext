#pragma once
#include <Windows.h>
#include <iostream>
#include <map>

namespace Menu
{
	inline bool bAimbot{ false };
	inline bool bESP{ false };
	inline bool bTargetLock{ false };
	inline int iSmooth{ 0 };
	inline int iFOV{ 30 };

	bool IsOptionChanged();
	void PrintMenu();
	void PrintPtrErr(std::map<std::string_view, intptr_t> pPtrList);
};
