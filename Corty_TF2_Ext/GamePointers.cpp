#include "GamePointers.h"

bool GamePointers::SetProcData()
{
	HANDLE hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		std::cerr << "[-] Invalid handle value. \n" << GetLastError();
		if (hSnap) CloseHandle(hSnap);
		system("PAUSE");
		return false;
	}

	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnap, &procEntry))
	{
		std::cerr << "[-] No process detected. \n" << GetLastError();
		if (hSnap) CloseHandle(hSnap);
		system("PAUSE");
		return false;
	}

	do
	{
		if (!_wcsicmp(procEntry.szExeFile, L"tf_win64.exe"))
		{
			HANDLE hProcBuffer{ OpenProcess(PROCESS_ALL_ACCESS, 0, procEntry.th32ProcessID) };
			if (hProcBuffer == NULL)
			{
				std::cerr << "[-] Process creation failed. \n" << GetLastError();
				if (hSnap) CloseHandle(hSnap);
				if (hProcBuffer) CloseHandle(hProcBuffer);
				system("PAUSE");
				return false;
			}

			std::cout << "[+] Game found. \n";
			this->dataProc.id = procEntry.th32ProcessID;
			this->dataProc.handle = hProcBuffer;
			Offsets::hProc = dataProc.handle;
			ptrList["ProcessID"] = dataProc.id;
			return true;
		}

	} while (Process32Next(hSnap, &procEntry));

	std::cerr << "[-] Please open the game first. \n";
	if (hSnap) CloseHandle(hSnap);
	system("PAUSE");
	return false;
}

DataMod GamePointers::GetModData(const wchar_t* pNameMod, DWORD pProcId)
{
	HANDLE hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pProcId) };
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		std::cerr << "[-] Invalid handle value. \n" << GetLastError();
		if (hSnap) CloseHandle(hSnap);
		return DataMod();
	}

	MODULEENTRY32 modEntry{};
	modEntry.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(hSnap, &modEntry))
	{
		std::cerr << "[-] No module detected. \n" << GetLastError();
		if (hSnap) CloseHandle(hSnap);
		return DataMod();
	}

	do
	{
		if (!_wcsicmp(modEntry.szModule, pNameMod))
		{
			std::wstring nameMod(pNameMod);
			DataMod dataMod{};
			dataMod.baseAddr = modEntry.modBaseAddr;
			dataMod.size = modEntry.modBaseSize;
			return dataMod;
		}

	} while (Module32Next(hSnap, &modEntry));

	std::wcerr << "[-] Could not get the " << pNameMod << " module. \n";
	if (hSnap) CloseHandle(hSnap);
	return DataMod();
}

void GamePointers::SetClientMod(const DataMod& pDataMod)
{
	Offsets::ClientMod = reinterpret_cast<DWORD64>(pDataMod.baseAddr);
	ptrList["Client"] = Offsets::ClientMod;
}

void GamePointers::SetEngineMod(const DataMod& pDataMod)
{
	Offsets::EngineMod = reinterpret_cast<DWORD64>(pDataMod.baseAddr);
	ptrList["Engine"] = Offsets::EngineMod;
}

bool GamePointers::Init()
{
	if (SetProcData())
	{
		DataMod dataClient{ GetModData(L"client.dll", this->dataProc.id) };
		DataMod dataEngine{ GetModData(L"engine.dll", this->dataProc.id) };

		SetClientMod(dataClient);
		SetEngineMod(dataEngine);

		// Pattern scanning
		SetAngles(&dataEngine, Pattern::vAngles);
		SetLocalPlayer(&dataClient, Pattern::LocalPlayer);
		SetEntityList(&dataClient, Pattern::EntityList);
	}

	return CheckPtrState();
}

void GamePointers::SetEntityList(DataMod* pDataMod, std::string_view pPattern)
{
	const DWORD64 patternAddr{ GetPattern(pDataMod,  pPattern) };
	if (patternAddr)
		Offsets::EntityList = ExtractOffset(pDataMod->baseAddr, patternAddr);

	ptrList["EntityList"] = Offsets::EntityList;
}

void GamePointers::SetLocalPlayer(DataMod* pDataMod, std::string_view pPattern)
{
	const DWORD64 patternAddr{ GetPattern(pDataMod,  pPattern) };
	if (patternAddr)
		Offsets::LocalPlayer::base = ExtractOffset(pDataMod->baseAddr, patternAddr);

	ptrList["LocalPlayer"] = Offsets::LocalPlayer::base;
}

void GamePointers::SetAngles(DataMod* pDataMod, std::string_view pPattern)
{
	const DWORD64 patternAddr{ GetPattern(pDataMod,  pPattern) };
	if (patternAddr)
		Offsets::LocalPlayer::vAngles = ExtractOffset(pDataMod->baseAddr, patternAddr);

	ptrList["vAngles"] = Offsets::LocalPlayer::vAngles;
}

DWORD64 GamePointers::GetPattern(DataMod* pDataMod, std::string_view pPattern)
{
	MEMORY_BASIC_INFORMATION mbi;
	mbi.RegionSize = 0x1000;
	BYTE* buffer{ nullptr };

	for (BYTE* currRegion{ pDataMod->baseAddr }; currRegion < pDataMod->baseAddr + pDataMod->size; currRegion += mbi.RegionSize)
	{
		// Get Valid Region
		if (!VirtualQueryEx(this->dataProc.handle, currRegion, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
			continue;

		delete[] buffer;
		buffer = new BYTE[mbi.RegionSize];
		DWORD oldProtect{};

		// Read region data
		if (!VirtualProtectEx(this->dataProc.handle, mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldProtect))
			continue;

		if (!ReadProcessMemory(this->dataProc.handle, mbi.BaseAddress, buffer, mbi.RegionSize, nullptr))
			continue;

		const DWORD64 patternIndex{ GetPatternIndex(buffer, mbi.RegionSize, pPattern) };
		if (patternIndex)
		{
			delete[] buffer;
			return reinterpret_cast<DWORD64>(mbi.BaseAddress) + patternIndex;
		}
	}

	delete[] buffer;
	return 0;
}

DWORD64 GamePointers::GetPatternIndex(BYTE* pRegionBuffer, SIZE_T pRegionSize, std::string_view pPattern)
{
	const std::vector<BYTE> parsedPatt{ GetParsedPattern(pPattern) };

	for (UINT i{ 0 }; i < pRegionSize; ++i)
	{
		bool bFound{ true };

		for (UINT j{ 0 }; j < parsedPatt.size(); ++j)
		{
			if (parsedPatt[j] != '?' && parsedPatt[j] != pRegionBuffer[i + j])
			{
				bFound = false;
				break;
			}
		}

		if (bFound) return i;
	}

	return 0;
}

DWORD GamePointers::ExtractOffset(BYTE* pModBase, DWORD64 patternAddr)
{
	DWORD patternRVA{};
	// extracting only the RVA in the instruction
	if (ReadMem<DWORD>(patternAddr, { 3 }, patternRVA))
	{
		const DWORD64 pointerAddr{ patternAddr + static_cast<DWORD64>(patternRVA) +
		7 }; // adding opcode instruction size

		// Get pointer offset
		return static_cast<DWORD>(pointerAddr - reinterpret_cast<DWORD64>(pModBase));
	}

	return DWORD();
}

std::vector<BYTE> GamePointers::GetParsedPattern(std::string_view pPattern)
{
	if (pPattern.empty())
		return std::vector<BYTE>();

	std::vector<BYTE> parsedPatt{};
	int i{ 0 };
	while (i < pPattern.size())
	{
		const std::string subPattern{ pPattern.substr(i, 2) };

		if (subPattern == "? ")
		{
			parsedPatt.push_back('?');
			i += 2;
		}
		else
		{
			const BYTE subHex{ static_cast<BYTE>(std::stoi(subPattern, nullptr, 16)) };
			parsedPatt.push_back(subHex);
			i += 3;
		}
	}

	return parsedPatt;
}

bool GamePointers::CheckPtrState()
{
	if (ptrList.empty()) return false;

	for (const auto& currPtr : ptrList)
	{
		if (!currPtr.second)
			return false;
	}
	return true;
}