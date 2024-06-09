#include "Aimbot.h"
bool Aimbot::Run(const std::vector<Entity>& pEntities)
{
	if (pEntities.empty())
		return false;

	Entity nearEnt{};
	if (pEntities.size() > 1)
		nearEnt = GetNearEntity(pEntities);
	else
		nearEnt = pEntities[0];

	if (!nearEnt.bInit)
	{
		std::cerr << "[-] Could not get the nearest entity. \t \r";
		return false;
	}

	if (!Menu::bTargetLock)
		entLocked = nearEnt;

	if (!IsFOV(nearEnt)) // TODO: FUSE IT W CALCANGLE FUNC because it already calcangle 
		return false;

	Vector3 botAngles{};
	if (Menu::bTargetLock)
	{
		if (entLocked.bInit && entLocked.health > 1)
		{
			if (!CalcAngles(entLocked.vBodyPos, botAngles))
				return false;

			//if(Menu::Smoothing > 0)
			// LocalPlayer::SetSmoothAngles(botAngles); TODO
			// else
			LocalPlayer::SetMyAngles(botAngles);
		}
		else
		{
			entLocked = NULL;
		}
	}
	else
	{
		if (!CalcAngles(nearEnt.vBodyPos, botAngles))
			return false;

		//if(Menu::Smoothing > 0)
		// LocalPlayer::SetSmoothAngles(botAngles); TODO
		// else
		LocalPlayer::SetMyAngles(botAngles);
	}

	return true;
}

Entity Aimbot::GetNearEntity(const std::vector<Entity>& pEntities)
{
	Entity nearEntity{};
	float oldCoef{ FLT_MAX };

	for (auto& entity : pEntities)
	{
		Vector3 vDeltaPos{ LocalPlayer::GetBase().vBodyPos - entity.vBodyPos };
		const float posDist{ sqrtf(vDeltaPos * vDeltaPos) };

		Vector3 currAngles{};
		CalcAngles(entity.vBodyPos, currAngles);
		Vector3 vDeltaAngle{ LocalPlayer::GetBase().vAngles - currAngles };
		const float angleDist{ ::sqrtf(vDeltaAngle * vDeltaAngle) };

		const float currCoef{ posDist * 0.8f + angleDist * 0.2f };

		if (oldCoef > currCoef)
		{
			oldCoef = currCoef;
			nearEntity = entity;
		}
	}

	return nearEntity;
}

bool Aimbot::IsFOV(const Entity& pEntity)
{
	Vector3 myAngles{ LocalPlayer::GetBase().vAngles };

	Vector3 botAngles{};
	CalcAngles(pEntity.vBodyPos, botAngles);

	Vector3 vDelta{ myAngles - botAngles };
	const float fDist{ ::sqrt(vDelta * vDelta) };

#if _DEBUG
	std::cout << "[+] distFov : " << fDist << '\r';
#endif

	if (fDist < Menu::iFOV) return true;

	return false;
}

bool Aimbot::CalcAngles(const Vector3& pEntPos, Vector3& pBotAngles)
{
	Vector3 vDelta{ LocalPlayer::GetBase().vBodyPos - pEntPos };
	const float fMagnitude{ ::sqrtf(vDelta * vDelta) };

	constexpr float radToDegree{ 57.295778f };
	pBotAngles.x = asinf(vDelta.z / fMagnitude) * radToDegree;
	pBotAngles.y = atan2f(vDelta.y, vDelta.x) * radToDegree;
	pBotAngles.z = 0;

	ClampPitch(pBotAngles.x);
	NormalizeYaw(pBotAngles.y);
	return true;
}

void Aimbot::ClampPitch(float& pPitch)
{
	pPitch = (pPitch < -89.0f) ? -89.0f : pPitch;
	pPitch = (pPitch > 89.f) ? 89.0f : pPitch;
}

void Aimbot::NormalizeYaw(float& pYaw)
{
	pYaw += 180;
}