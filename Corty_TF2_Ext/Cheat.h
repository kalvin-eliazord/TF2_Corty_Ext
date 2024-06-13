#pragma once
#include <Windows.h>
#include <vector>
#include "Aimbot.h"
#include "ESP.h"
#include "Offsets.h"
#include "Entity.h"
#include "Menu.h"

namespace Cheat
{
	bool Run();
	DWORD64 GetEntity(UINT pIndex);
	std::vector<Entity> GetEntities();
	Entity GetLocalPlayer();
	bool IsGoodEnt(Entity* pEntity);
};