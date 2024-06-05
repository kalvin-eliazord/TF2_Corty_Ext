#pragma once
#include <vector>
#include <math.h>
#include "Entity.h"
#include "LocalPlayer.h"
#include "GamePointers.h"
#include "Cheat.h"

struct Aimbot
{
	bool Run(const std::vector<Entity>& pEntities);
	Entity GetNearEntity(const std::vector<Entity>& pEntities);
	bool CalcAngles(const Entity& pEntity, Vector3& pNewAngles);
	void ClampPitch(float& pPitch);
	void NormalizeYaw(float& pYaw);
};
