#include "ESP.h"

DataWin dataWin{};

LRESULT CALLBACK WndProc(HWND pHwnd, UINT pMsg, WPARAM wParam, LPARAM lParam)
{
	const MARGINS margin{ 0,0, dataWin.width, dataWin.height };

	switch (pMsg) {

	case WM_PAINT:
		DwmExtendFrameIntoClientArea(pHwnd, &margin);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	}

	return DefWindowProc(pHwnd, pMsg, wParam, lParam);
}

bool ESP::InitWin(HINSTANCE hInstance, int nShowCmd)
{
	auto& targetHwnd{ dataWin.hwnd };
	auto& name{ dataWin.name };
	auto& width{ dataWin.width };
	auto& height{ dataWin.height };

	targetHwnd = FindWindow(NULL, name);
	if (!targetHwnd)
	{
		std::cerr << "[-] Could not find the game window. \n";
		return false;
	}

	RECT rect{};
	GetWindowRect(targetHwnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	WNDCLASSEX wndEx{ 0 };
	ZeroMemory(&wndEx, sizeof(WNDCLASSEX));
	wndEx.cbSize = sizeof(WNDCLASSEX);
	wndEx.hInstance = hInstance;
	wndEx.lpszClassName = L"WinClass";
	wndEx.style = CS_HREDRAW | CS_VREDRAW;
	wndEx.lpfnWndProc = WndProc;
	RegisterClassEx(&wndEx);

	auto myHwnd{ CreateWindowEx(
		0,
		wndEx.lpszClassName,
		L"",
		WS_EX_TOPMOST | WS_POPUP,
		rect.left,
		rect.top,
		width,
		height,
		NULL,
		NULL,
		wndEx.hInstance,
		NULL) };

	SetWindowLong(
		myHwnd,
		GWL_EXSTYLE,
		static_cast<int>(GetWindowLong(myHwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT));

	SetLayeredWindowAttributes(myHwnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);
	ShowWindow(myHwnd, nShowCmd);
	
	return true;
}

bool ESP::Run(std::vector<Entity> pEntities)
{
	return true;
}
