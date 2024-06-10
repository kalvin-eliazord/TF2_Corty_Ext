#pragma once
#include <vector>
#include <math.h>
#include "Entity.h"
#include "GamePointers.h"
#include "Cheat.h"

struct Aimbot
{
	bool Run(const std::vector<Entity>& pEntities);
	Vector3 GetNearEntAngles(const std::vector<Entity>& pEntities);
	bool IsFOV(const Vector3& pBotAngles);
	void CalcAngles(const Vector3& pEntPos, Vector3& pBotAngles);
	void ClampPitch(float& pPitch);
	void NormalizeYaw(float& pYaw);
	Vector3 GetLocalPlayerAngles();
	bool SetAngles(const Vector3& pBotAngles);
	bool SetSmoothAngles(Vector3 pBotAngles, int pSmooth);
	UINT entIndex{0};
};