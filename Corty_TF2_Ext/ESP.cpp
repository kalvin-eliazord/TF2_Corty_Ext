#include "ESP.h"

bool ESP::InitWindow()
{
	auto& hwnd{ this->dataWin.hwnd };
	auto& name{ this->dataWin.name };
	auto& width{ this->dataWin.width };
	auto& height{ this->dataWin.height };

	hwnd = FindWindow(NULL, name);
	if (!hwnd)
	{
		std::cerr << "[-] Could not get the game window. \n";
		return false;
	}

	RECT rect{};
	GetWindowRect(hwnd, &rect);
	width = static_cast<float>(rect.right - rect.left);
	height = static_cast<float>(rect.bottom - rect.top);

	//CreateWindow(, "", ); TODO

	return true;
}

bool ESP::Run(std::vector<Entity> pEntities)
{
	return true;
}
