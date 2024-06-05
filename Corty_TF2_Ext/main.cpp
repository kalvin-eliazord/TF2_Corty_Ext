#include <iostream>
#include "GameAccess.h"
#include "GamePointers.h"
#include "Log.h"
#include "Cheat.h"

int main()
{
	SetConsoleTitle(L"Corty TF2 External");

	GamePointers gPointers{};
	if (gPointers.Init())
	{
		Log::PrintMenu();
		Cheat::Run();
	}
	else
	{
		Log::PrintPtrErr(gPointers.ptrList);
	}

	if (Offsets::hProc)
		CloseHandle(Offsets::hProc);

	return 0;
}