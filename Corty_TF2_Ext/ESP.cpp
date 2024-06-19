#include "ESP.h"

DataWin myHwnd{};

LRESULT CALLBACK WndProc(HWND pHwnd, UINT pMsg, WPARAM wParam, LPARAM lParam)
{
	const MARGINS margin{ 0,0, myHwnd.width, myHwnd.height };

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

bool ESP::InitWin()
{
	auto& width{ this->targetHwnd.width };
	auto& height{ this->targetHwnd.height };
	auto& targetHwnd{ this->targetHwnd.hwnd };
	const auto& wName{ L"Team Fortress 2 - Direct3D 9 - 64 Bit" };

	targetHwnd = FindWindow(NULL, wName);
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
	wndEx.hInstance = GetModuleHandle(NULL);
	wndEx.lpszClassName = L"WinClass";
	wndEx.style = CS_HREDRAW | CS_VREDRAW;
	wndEx.lpfnWndProc = WndProc;
	RegisterClassEx(&wndEx);

	myHwnd.hwnd = CreateWindowEx(
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
		NULL);

	const int extdStyle{
		static_cast<int>(GetWindowLong(myHwnd.hwnd, GWL_EXSTYLE))
		| WS_EX_LAYERED
		| WS_EX_TRANSPARENT };

	SetWindowLong(myHwnd.hwnd, GWL_EXSTYLE, extdStyle);
	SetLayeredWindowAttributes(myHwnd.hwnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);
	ShowWindow(myHwnd.hwnd, SW_SHOWDEFAULT);

	this->myWnd = &myHwnd.hwnd;

	return true;
}

void ESP::Run(const std::vector<Entity>& pEntities)
{
	auto& msg{ myHwnd.msg };
	auto& targetHwnd{ this->targetHwnd.hwnd };

	// Update window size / position
	RECT rect{};
	GetWindowRect(targetHwnd, &rect);
	const auto width{ rect.right - rect.left };
	const auto height{ rect.bottom - rect.top };
	MoveWindow(myHwnd.hwnd, rect.left, rect.top, width, height, true);
	SetWindowPos(myHwnd.hwnd, HWND_TOPMOST, rect.left, rect.top, width, height, SWP_NOMOVE | SWP_NOSIZE);

	Draw(pEntities);

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
		CloseWindow(myHwnd.hwnd);
}

bool ESP::CreateD3D9()
{
	d3d9Interface = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9Interface)
	{
		std::cerr << "[-] Cannot create the D3D9's Interface. \n";
		return false;
	}

	D3DPRESENT_PARAMETERS d3dParam{ 0 };
	ZeroMemory(&d3dParam, sizeof(d3dParam));
	d3dParam.Windowed = TRUE;
	d3dParam.hDeviceWindow = myHwnd.hwnd;
	d3dParam.SwapEffect = D3DSWAPEFFECT_COPY;

	HRESULT hRes{ d3d9Interface->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3dParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dParam,
		&d3d9Device) };

	if (FAILED(hRes))
	{
		std::cerr << "[-] Cannot create the D3D9's Device. \n";
		return false;
	}

	return true;
}

bool ESP::Draw(const std::vector<Entity>& pEntities)
{
	HRESULT hRes{ d3d9Device->BeginScene() };
	if (FAILED(hRes)) return false;

	d3d9Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	D3DVIEWPORT9 viewport{};
	d3d9Device->GetViewport(&viewport);

	if (!pEntities.empty())
	{
		float viewMatrix[4][4]{};
		const DWORD64 conVarBase{ Offsets::EngineMod + Offsets::ConVar };
		if (!ReadMem(conVarBase + Offsets::ViewMatrix, sizeof(viewMatrix), viewMatrix))
			return false;

		for (const auto& ent : pEntities)
		{
			Vector3 screenPos{};
			if (!W2S(
				ent.vBodyPos,
				screenPos,
				viewMatrix,
				static_cast<FLOAT>(this->targetHwnd.width),
				static_cast<FLOAT>(this->targetHwnd.height))
				)
				continue;

			SnapLine(screenPos, static_cast<FLOAT>(this->targetHwnd.width), static_cast<FLOAT>(this->targetHwnd.height));
			// TODO Boxes
		}
	}

	hRes = d3d9Device->EndScene();
	if (FAILED(hRes)) return false;

	d3d9Device->Present(NULL, NULL, NULL, NULL);
	return true;
}

void ESP::SnapLine(const Vector3& pScreenPos, const FLOAT pWinWidth, const FLOAT pWinHeight)
{
	// Setting line
	D3DXVECTOR2 myLines[2];
	myLines[0] = D3DXVECTOR2(pScreenPos.x, pScreenPos.y);
	myLines[1] = D3DXVECTOR2(static_cast<FLOAT>(this->targetHwnd.width / 2), static_cast<FLOAT>(this->targetHwnd.height));

	// Drawing line
	ID3DXLine* d3dLine{};
	D3DXCreateLine(d3d9Device, &d3dLine);
	d3dLine->SetWidth(2);
	d3dLine->Draw(myLines, 2, D3DCOLOR_RGBA(255, 255, 100, 255));
	d3dLine->Release();
}

bool ESP::W2S(Vector3 pWorldPos, Vector3& pScreenPos, float pMatrix[4][4], const FLOAT pWinWidth, const FLOAT pWinHeight)
{
	const float mX{ pWinWidth / 2 };
	const float mY{ pWinHeight / 2 };

	const float w{
		pMatrix[3][0] * pWorldPos.x +
		pMatrix[3][1] * pWorldPos.y +
		pMatrix[3][2] * pWorldPos.z +
		pMatrix[3][3] };

	if (w < 0.65f) return false;

	const float x{
		pMatrix[0][0] * pWorldPos.x +
		pMatrix[0][1] * pWorldPos.y +
		pMatrix[0][2] * pWorldPos.z +
		pMatrix[0][3] };

	const float y{
		pMatrix[1][0] * pWorldPos.x +
		pMatrix[1][1] * pWorldPos.y +
		pMatrix[1][2] * pWorldPos.z +
		pMatrix[1][3] };

	pScreenPos.x = (mX + mX * x / w);
	pScreenPos.y = (mY - mY * y / w);
	pScreenPos.z = 0;
	return true;
}

ESP::~ESP()
{
	if (d3d9Device) d3d9Device->Release();
	if (d3d9Interface) d3d9Interface->Release();
}