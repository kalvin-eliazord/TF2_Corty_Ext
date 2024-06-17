#include "Aimbot.h"

void Aimbot::Run(const std::vector<Entity>& pEntities)
{
	if (pEntities.empty()) return;

	std::optional<Vector3> botAngles{};
	if (!(Menu::bTargetLock && iEntLocked))
	{
		botAngles = GetNearEntAngles(pEntities);
		if (!botAngles) return;

		if (!IsFOV(*botAngles)) return;
	}
	else
	{
		// Target locking 
		Entity entLocked(Cheat::GetEntity(iEntLocked), iEntLocked);
		if (entLocked.health <= 1)
		{
			iEntLocked = 0;
			return;
		}

		botAngles = GetEntAngles(entLocked.vBodyPos);

		if (!IsFOV(*botAngles)) return;
	}

	if (Menu::iSmooth > 0)
		SetAngles(*botAngles, Menu::iSmooth);
	else
		SetAngles(*botAngles);
}

std::optional<Vector3> Aimbot::GetNearEntAngles(const std::vector<Entity>& pEntities)
{
	std::optional<Vector3> vNewAngles{};
	float oldCoef{ FLT_MAX };

	for (auto& entity : pEntities)
	{
		// Body position distance
		Vector3 vDeltaPos{ Cheat::GetLocalPlayer().vBodyPos - entity.vBodyPos };
		const float posDist{ sqrtf(vDeltaPos * vDeltaPos) };

		// Angles distance
		std::optional<Vector3> currAngles{ GetEntAngles(entity.vBodyPos) };
		if (!currAngles) continue;

		Vector3 vDeltaAngle{ Cheat::GetLocalPlayer().vAngles - *currAngles };
		const float angleDist{ ::sqrtf(vDeltaAngle * vDeltaAngle) };

		// Nearest entity based on the position and the angle distance
		const float currCoef{ posDist * 0.8f + angleDist * 0.2f };
		if (oldCoef > currCoef)
		{
			oldCoef = currCoef;
			vNewAngles = currAngles;

			// Needed to access the entity when the target locking is activated
			iEntLocked = entity.index;
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

std::optional<Vector3> Aimbot::GetEntAngles(const Vector3& pEntPos)
{
	std::optional<Vector3> botAngles{};

	Vector3 vDelta{ Cheat::GetLocalPlayer().vBodyPos - pEntPos };
	const float fMagnitude{ ::sqrtf(vDelta * vDelta) };

	constexpr float radToDegree{ 57.295778f };
	botAngles->x = asinf(vDelta.z / fMagnitude) * radToDegree;
	botAngles->y = (atan2f(vDelta.y, vDelta.x) * radToDegree) + 180; // Normalize Yaw
	botAngles->z = 0;

	ClampPitch(botAngles->x);

	return botAngles;
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

std::optional<Vector3> Aimbot::GetLocalPlayerAngles()
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

bool Aimbot::SetAngles(Vector3 pBotAngles, int pSmooth)
{
	std::optional<Vector3> lpAngles{ GetLocalPlayerAngles() };
	if (!lpAngles)
	{
		std::cerr << "[-] Cannot get localPlayer angles. \r";
		return false;
	}

	Vector3 vDelta{ pBotAngles - *lpAngles };
	NormalizeYaw(vDelta.y);

	if (lpAngles->x != pBotAngles.x)
		lpAngles->x += vDelta.x / static_cast<float>(pSmooth);

	if (lpAngles->y != pBotAngles.y)
		lpAngles->y += vDelta.y / static_cast<float>(pSmooth);

	SetAngles(*lpAngles);
	return true;
}