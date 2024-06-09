#pragma once
#include <Windows.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib") ?
#include "Entity.h"
#include <vector>

struct DataWin
{
	HWND hwnd{};
	const wchar_t* name{ L"Team Fortress 2 - Direct3D 9 - 64 Bit" };
	float width{};
	float height{};
};

struct ESP
{
	DataWin dataWin{};
	bool InitWindow();
	bool Run(std::vector<Entity> pEntities);
};