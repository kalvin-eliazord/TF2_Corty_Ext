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
	Entity GetLocalPlayer();
	DWORD64 GetEntity(UINT pIndex);
	bool IsGoodEnt(Entity* pEntity);
	std::vector<Entity> GetEntities();
};