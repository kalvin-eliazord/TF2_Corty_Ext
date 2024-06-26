#pragma once
#include <Windows.h>
#include "Vector3.h"
#include "Offsets.h"

enum TeamID
{
	red = 2,
	blue = 3
};

struct Entity
{
	bool bInit{ false };
	DWORD64 baseAddr{};
	DWORD health{};
	bool isDead{};
	DWORD teamID{};
	bool bDormant{};
	Vector3 vBodyPos{};
	Vector3 vAngles{};
	const UINT index{};

	Entity(DWORD64 pBaseAddr, UINT pIndex);
	Entity();

	bool InitData();
	bool SetHealth();
	bool SetTeamID();
	bool SetDormant();
	bool SetBodyPos();
	bool SetAngles();
	bool SetIsDead();
};