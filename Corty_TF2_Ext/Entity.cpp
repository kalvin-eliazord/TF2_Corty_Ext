#include "Entity.h"

Entity::Entity(DWORD64 pBaseAddr)
	: baseAddr{ pBaseAddr }
{ 
	// When game haven't started pointers aren't initialized
	//  so it prevents trying to init members with dangling baseAddr value
	if (baseAddr && baseAddr > 0x10000 && InitData()) 
		bInit = true;
}

Entity::Entity()
{}

bool Entity::InitData()
{
	if (!SetHealth()) return false;
	if (!SetTeamID())return false;
//	if (!SetDormant()) return false; TODO
	if (!SetBodyPos())return false;
	if (!SetAngles())return false;

	return true;
}

bool Entity::SetHealth()
{
	return ReadMem<DWORD>(this->baseAddr, { Offsets::Entity::Health }, this->health);
}

bool Entity::SetTeamID()
{
	return ReadMem<DWORD>(this->baseAddr, { Offsets::Entity::TeamID }, this->teamID);
}

bool Entity::SetDormant()
{
	return ReadMem<bool>(this->baseAddr, { Offsets::Entity::Dormant }, this->bDormant);
}

bool Entity::SetBodyPos()
{
	return ReadMem<Vector3>(this->baseAddr, { Offsets::Entity::BodyPos }, this->vBodyPos);
}

bool Entity::SetAngles()
{
	return ReadMem<Vector3>(this->baseAddr, { Offsets::Entity::Angles }, this->vAngles);
}

bool Entity::SetAlive()
{
	return ReadMem<bool>(this->baseAddr, { Offsets::Entity::Dormant }, this->bDormant);
}
