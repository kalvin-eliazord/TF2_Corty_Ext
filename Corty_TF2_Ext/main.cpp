#include <Windows.h>
#include <iostream>
#include "GameAccess.h"
#include "GamePointers.h"
#include "Menu.h"
#include "Cheat.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
{
	SetConsoleTitle(L"Corty TF2 External");

	GamePointers gPointers{};
	if (!gPointers.Init())
	{
		Menu::PrintPtrErr(gPointers.ptrList);

		if (Offsets::hProc)
			CloseHandle(Offsets::hProc);

		return 1;
	}

	Menu::PrintMenu();

	if (!Cheat::Run(hInstance, nShowCmd))
		system("PAUSE");

	if (Offsets::hProc)
		CloseHandle(Offsets::hProc);

	return 0;
}