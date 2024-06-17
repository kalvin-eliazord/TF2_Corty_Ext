#pragma once
#include <Windows.h>
#include "GamePointers.h"

namespace Offsets
{
	inline HANDLE hProc{};
	inline DWORD64 ClientMod{};
	inline DWORD64 EngineMod{};

	inline DWORD EntityList{};

	namespace LocalPlayer
	{
		inline DWORD base{};
		inline DWORD vAngles{};
	}

	namespace Entity
	{
		inline constexpr DWORD Health{ 0xE4 };
		inline constexpr DWORD BodyPos{ 0x338 };
		inline constexpr DWORD Angles{ 0x344 };
		inline constexpr DWORD TeamID{ 0xEC };
		inline constexpr DWORD Dormant{ 0x2A };
		inline constexpr DWORD Alive{ 0x0 }; //TODO
	}
}

template <typename T>
bool ReadMem(DWORD64 pBaseAddr, auto pSize, T& pResult)
{
	if (!pBaseAddr || !pSize)
	{
		std::cerr << "[-] Cannot read memory, please check parameters. \n";
		system("PAUSE");
		return false;
	}

	SIZE_T nbBytes{ 0 };
	if (!ReadProcessMemory(Offsets::hProc, (T*)pBaseAddr, &pResult, pSize, &nbBytes) || !nbBytes)
	{
		std::cerr << "[-] Cannot read memory, please check pointer path. \n";
		system("PAUSE");
		return false;
	}

	return true;
}

template <typename T>
bool ReadMem(DWORD64 pBaseAddr, std::vector<DWORD> pOffsets, T& pResult)
{
	if (!pBaseAddr || pOffsets.empty() || !Offsets::hProc)
	{
		std::cerr << "[-] Cannot read memory, please check parameters. \n";
		system("PAUSE");
		return false;
	}

	for (int i{ 0 }; i < pOffsets.size(); ++i)
	{
		SIZE_T nbBytes{ 0 };
		pBaseAddr += pOffsets[i];
		if (!ReadProcessMemory(Offsets::hProc, (T*)pBaseAddr, &pResult, sizeof(T), &nbBytes) || !nbBytes)
		{
			std::cerr << "[-] Cannot read memory, please check pointer path. \n";
			system("PAUSE");
			return false;
		}
	}

	return true;
}