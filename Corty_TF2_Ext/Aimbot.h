#pragma once
#include <vector>
#include <math.h>
#include <optional>
#include "Entity.h"
#include "GamePointers.h"
#include "Cheat.h"

struct Aimbot
{
	UINT iEntLocked{ 0 };
	void ClampPitch(float& pPitch);
	void NormalizeYaw(float& pYaw);
	bool IsFOV(const Vector3& pBotAngles);
	bool SetAngles(const Vector3& pBotAngles);
	std::optional<Vector3> GetLocalPlayerAngles();
	void Run(const std::vector<Entity>& pEntities);
	bool SetAngles(Vector3 pBotAngles, int pSmooth);
	std::optional<Vector3> GetEntAngles(const Vector3& pEntPos);
	std::optional<Vector3> GetNearEntAngles(const std::vector<Entity>& pEntities);
};