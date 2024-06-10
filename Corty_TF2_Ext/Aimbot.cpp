#include "Aimbot.h"

bool Aimbot::Run(const std::vector<Entity>& pEntities)
{
	if (pEntities.empty())
		return false;

	Vector3 botAngles{};
	if (!(Menu::bTargetLock && entIndex))
	{
		botAngles = GetNearEntAngles(pEntities);

		if (!IsFOV(botAngles))
			return true;
	}
	else
	{
		// Target locking 
		Entity entLocked(Cheat::GetEntity(entIndex), entIndex);
		if (entLocked.health <= 1)
		{
			entIndex = 0;
			return true;
		}

		CalcAngles(entLocked.vBodyPos, botAngles);

		if (!IsFOV(botAngles))
			return true;
	}

	if(Menu::iSmooth > 0)
		SetSmoothAngles(botAngles, Menu::iSmooth);
	else
		SetAngles(botAngles);

	return true;
}

Vector3 Aimbot::GetNearEntAngles(const std::vector<Entity>& pEntities)
{
	Vector3 vNewAngles{};
	float oldCoef{ FLT_MAX };

	for (auto& entity : pEntities)
	{
		// Body position distance
		Vector3 vDeltaPos{ Cheat::GetLocalPlayer().vBodyPos - entity.vBodyPos };
		const float posDist{ sqrtf(vDeltaPos * vDeltaPos) };

		// Angles distance
		Vector3 currAngles{};
		CalcAngles(entity.vBodyPos, currAngles);
		Vector3 vDeltaAngle{ Cheat::GetLocalPlayer().vAngles - currAngles };
		const float angleDist{ ::sqrtf(vDeltaAngle * vDeltaAngle) };

		// Nearest entity based on the position and the angle distance
		const float currCoef{ posDist * 0.8f + angleDist * 0.2f };
		if (oldCoef > currCoef)
		{
			oldCoef = currCoef;
			vNewAngles = currAngles;

			// Needed to access the entity when the target locking is activated
			entIndex = entity.index;
		}
	}

	return vNewAngles;
}

bool Aimbot::IsFOV(const Vector3& pBotAngles)
{
	Vector3 myAngles{ Cheat::GetLocalPlayer().vAngles };
	Vector3 vDelta{ myAngles - pBotAngles };
	NormalizeYaw(vDelta.y);

	float fDist{ ::sqrt(vDelta * vDelta) };

	if (fDist < Menu::iFOV) return true;

	return false;
}

void Aimbot::CalcAngles(const Vector3& pEntPos, Vector3& pBotAngles)
{
	Vector3 vDelta{ Cheat::GetLocalPlayer().vBodyPos - pEntPos };
	const float fMagnitude{ ::sqrtf(vDelta * vDelta) };

	constexpr float radToDegree{ 57.295778f };
	pBotAngles.x = asinf(vDelta.z / fMagnitude) * radToDegree;
	pBotAngles.y = (atan2f(vDelta.y, vDelta.x) * radToDegree) + 180; // Normalize Yaw
	pBotAngles.z = 0;

	ClampPitch(pBotAngles.x);
}

void Aimbot::ClampPitch(float& pPitch)
{
	pPitch = (pPitch < -89.0f) ? -89.0f : pPitch;
	pPitch = (pPitch > 89.f) ? 89.0f : pPitch;
}

void Aimbot::NormalizeYaw(float& pYaw)
{
	while (pYaw > 180.f) pYaw -= 360.f;
	while (pYaw < -180.f) pYaw += 360.f;
}

Vector3 Aimbot::GetLocalPlayerAngles()
{
	Vector3 lpAngles{};
	ReadMem<Vector3>(Cheat::GetLocalPlayer().baseAddr, { Offsets::Entity::Angles }, lpAngles);
	return lpAngles;
}

bool Aimbot::SetAngles(const Vector3& pBotAngles)
{
	if (!WriteProcessMemory(Offsets::hProc, (Vector3*)(Offsets::EngineMod + Offsets::LocalPlayer::vAngles), &pBotAngles, sizeof(Vector3), nullptr))
	{
		std::cerr << "[-] Cannot write to localPlayer angles. \r";
		return false;
	}

	return true;
}

bool Aimbot::SetSmoothAngles(Vector3 pBotAngles, int pSmooth)
{
	Vector3 lpAngles{ GetLocalPlayerAngles() };
	Vector3 vDelta{ pBotAngles - lpAngles };
	NormalizeYaw(vDelta.y);

	if (lpAngles.x != pBotAngles.x)
		lpAngles.x += vDelta.x / static_cast<float>(pSmooth);

	if (lpAngles.y != pBotAngles.y)
		lpAngles.y += vDelta.y / static_cast<float>(pSmooth);

	SetAngles(lpAngles);
	return true;
}