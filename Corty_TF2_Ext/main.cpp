#include <Windows.h>
#include <iostream>
#include "GameAccess.h"
#include "GamePointers.h"
#include "Menu.h"
#include "Cheat.h"

//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//	const MARGINS margin = { 0, 0, Render.RenderWidth, Render.RenderHeight };
//
//	switch (message) {
//
//	case WM_PAINT:
//		DwmExtendFrameIntoClientArea(hWnd, &margin);
//		break;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//		break;
//
//	}
//
//	return DefWindowProc(hWnd, message, wParam, lParam);
//}

int main()
{
	SetConsoleTitle(L"Corty TF2 External");

	GamePointers gPointers{};
	if (gPointers.Init())
	{
		Menu::PrintMenu();
		Cheat::Run();
	}
	else
	{
		Menu::PrintPtrErr(gPointers.ptrList);
	}

	if (Offsets::hProc)
		CloseHandle(Offsets::hProc);

	return 0;
}