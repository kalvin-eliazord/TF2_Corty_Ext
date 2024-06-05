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

	//if(!IsFOV(nearEntity)) return false; TODO

	Vector3 newAngles{};
	if (!CalcAngles(nearEnt, newAngles))
		return false;

	//if(Log::Smoothing > 0)
	// LocalPlayer::SetSmoothAngles(newAngles); TODO
	// else
	LocalPlayer::SetMyAngles(newAngles);

	return true;
}

Entity Aimbot::GetNearEntity(const std::vector<Entity>& pEntities)
{
	Entity nearEntity{};
	float oldDist{ FLT_MAX};

	for (auto& entity : pEntities)
	{
		Vector3 vDelta{ LocalPlayer::GetBase().vBodyPos - entity.vBodyPos };
		const float currDist{ sqrtf(vDelta * vDelta) };
		if (currDist < oldDist)
		{
			oldDist = currDist;
			nearEntity = entity;
		}
	}

	return nearEntity;
}

bool Aimbot::CalcAngles(const Entity& pEntity, Vector3& pNewAngles)
{
	Vector3 vDelta{ LocalPlayer::GetBase().vBodyPos - pEntity.vBodyPos };
	const float fMagnitude{ ::sqrtf(vDelta * vDelta) };

	constexpr float radToDegree{ 57.295778f };
	pNewAngles.x = asinf(vDelta.z / fMagnitude) * radToDegree;
	pNewAngles.y = atan2f(vDelta.y, vDelta.x) * radToDegree;
	pNewAngles.z = 0;

	ClampPitch(pNewAngles.x);
	NormalizeYaw(pNewAngles.y);
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