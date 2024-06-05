#include "GameAccess.h"

bool GameAccess::SetHProc()
{
	HANDLE hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		std::cerr << "[-] Failed to take a process snapshot. \n";
		if(hSnap) CloseHandle(hSnap);
		system("PAUSE");
		return false;
	}

	PROCESSENTRY32 procEntry{};
	procEntry.dwSize = sizeof(procEntry);
	if (!Process32First(hSnap, &procEntry))
	{
		std::cerr << "[-] Failed to get the first process entry. \n";
		if (hSnap) CloseHandle(hSnap);
		system("PAUSE");
		return false;
	}

	do
	{
		if (!_wcsicmp(procEntry.szExeFile, L"tf_win64.exe"))
		{
			hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procEntry.th32ProcessID);
			std::cout << " [+] Game process retrieved. \n";
			return true;
		}
	}
	while (Process32Next(hSnap, &procEntry));

	// Can't find the process
	std::cerr << "[-] Please open the game first. \n";
	if (hSnap) CloseHandle(hSnap);
	system("PAUSE");
	return false;
}

intptr_t GameAccess::GetClientMod()
{
	return intptr_t();
}
