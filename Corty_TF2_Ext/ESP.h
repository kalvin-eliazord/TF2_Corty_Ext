#pragma once
#include <Windows.h>
#include <winnt.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <vector>
#include <dwmapi.h>
#include "Entity.h"
#include "Vector3.h"
#include "Offsets.h"

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
	HWND* myWnd {};
	IDirect3D9* d3d9Interface{ nullptr };
	IDirect3DDevice9* d3d9Device{ nullptr };

	bool InitWin();
	bool CreateD3D9();
	void Run(const std::vector<Entity>& pEntities);
	bool Draw(const std::vector<Entity>& pEntities);
	bool W2S(Vector3 pWorldPos, Vector3& pScreenPos, const FLOAT pWinWidth, const FLOAT pWinHeight);
	~ESP();
};