#pragma once
#include <Windows.h>
#include <vector>
#include "Aimbot.h"
//#include "ESP.h"
#include "Offsets.h"
#include "Entity.h"
#include "Log.h"

//struct ESP;

namespace Cheat
{
	bool Run();
	DWORD64 GetEntity(UINT pIndex);
	std::vector<Entity> GetEntities();
	bool IsGoodEnt(Entity* pEntity);
	//ESP* esp;
};

