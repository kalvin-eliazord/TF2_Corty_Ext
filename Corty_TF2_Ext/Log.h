#pragma once
#include <Windows.h>
#include <iostream>
#include <map>

namespace Log
{
	inline bool bAimbot{ false };
	bool IsOptionChanged();
	void PrintMenu();
	void PrintPtrErr(std::map<std::string_view, intptr_t> pPtrList);
};

