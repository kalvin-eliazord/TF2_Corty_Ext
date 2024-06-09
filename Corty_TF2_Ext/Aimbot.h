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
	bool IsFOV(const Entity& pEntity);
	bool CalcAngles(const Vector3& pEntPos, Vector3& pBotAngles);
	void ClampPitch(float& pPitch);
	void NormalizeYaw(float& pYaw);
	Entity entLocked{};
};
