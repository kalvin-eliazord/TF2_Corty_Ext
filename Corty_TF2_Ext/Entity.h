#pragma once
#include <Windows.h>
#include "Vector3.h"
#include "Offsets.h"

struct Entity
{
	bool bInit{ false };
	DWORD64 baseAddr{};
	DWORD health{};
	bool bAlive{};
	DWORD teamID{};
	bool bDormant{};
	Vector3 vBodyPos{};
	Vector3 vAngles{};

	Entity(DWORD64 pBaseAddr);
	Entity();

	bool InitData();
	bool SetHealth();
	bool SetTeamID();
	bool SetDormant();
	bool SetBodyPos();
	bool SetAngles();
	bool SetAlive();
};