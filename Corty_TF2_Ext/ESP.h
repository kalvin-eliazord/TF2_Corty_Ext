#pragma once
#include <Windows.h>
#include <winnt.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib") ?
#include "Entity.h"
#include <vector>
#include <dwmapi.h>

struct DataWin
{
	HWND hwnd{};
	const wchar_t* name{ L"Team Fortress 2 - Direct3D 9 - 64 Bit" };
	int width{};
	int height{};
};

struct ESP
{
	bool InitWin(_In_ HINSTANCE hInstance, _In_ int nShowCmd);
	bool Run(std::vector<Entity> pEntities);
};