#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <Psapi.h>
#include <string>
#include <vector>
#include <map>
#include "Offsets.h"

struct DataProc
{
	HANDLE handle{};
	DWORD id{};
};

struct DataMod
{
	DWORD size{};
	BYTE* baseAddr{};
};

struct GamePointers
{
	DataProc dataProc{};
	std::map<std::string_view, intptr_t>  ptrList{};

	bool Init();
	bool SetProcData();
	bool CheckPtrState();
	void SetClientMod(const DataMod& pDataMod);
	void SetEngineMod(const DataMod& pDataMod);
	DWORD ExtractOffset(BYTE* pModBase, DWORD64 patternAddr);
	DataMod GetModData(const wchar_t* pNameMod, DWORD pProcId);
	std::vector<BYTE> GetParsedPattern(std::string_view pPattern);
	void SetAngles(DataMod* pDataMod, std::string_view pPattern);
	void SetEntityList(DataMod* pDataMod, std::string_view pPattern);
	void SetConVar(DataMod* pDataMod, std::string_view pPattern);
	void SetLocalPlayer(DataMod* pDataMod, std::string_view pPattern);
	DWORD64 GetPattern(DataMod* pDataMod, std::string_view pPattern);
	DWORD64 GetPatternIndex(BYTE* pRegionBuffer, SIZE_T pRegionSize, std::string_view pPattern);
};

namespace Pattern
{
	const std::string_view EntityList{ "48 8B ? ? ? ? ? 48 83 C0 ? 48 C1 E1 ? 48 03 ? 74 ? C1 EA ? 39 50 ? 75 ? 48 8B ? C3 33 C0 C3 CC CC CC CC 48 89 ? ? ? 57 48 83 EC ? 48 63" };
	const std::string_view LocalPlayer{ "48 89 ? ? ? ? ? 48 8D ? ? ? ? ? 48 8B ? FF 50 ? 48 8B ? E8" };
	//const std::string_view EntityList{ "48 89 ? ? ? ? ? 0F 57 ? 48 8D ? ? ? ? ? 44 89" };
	const std::string_view vAngles{ "48 8D ? ? ? ? ? 48 8B ? ? ? ? ? 0F 28 ? F3 0F ? ? F3 0F" }; // engine.dll
	const std::string_view ConVar{ "48 8D ? ? ? ? ? E9 ? ? ? ? CC CC CC CC 48 8D ? ? ? ? ? E9 ? ? ? ? CC CC CC CC 48 83 EC ? 48 8D ? ? ? ? ? E8 ? ? ? ? 48 8D ? ? ? ? ? 48 83 C4 ? E9 ? ? ? ? 40 ? 48 83 EC ? 48 8D" };
};