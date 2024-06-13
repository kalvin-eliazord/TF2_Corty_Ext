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
	int width{};
	int height{};
	MSG msg{};
};

struct ESP
{
	DataWin targetHwnd{};
	IDirect3D9* d3d9Interface{ nullptr };
	IDirect3DDevice9* d3d9Device{ nullptr };

	bool InitWin();
	bool Run(std::vector<Entity> pEntities);
	bool CreateD3D9();
	bool Draw();
	~ESP();
};