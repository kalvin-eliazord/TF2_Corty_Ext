#include "Cheat.h"

DWORD64 Cheat::GetEntity(UINT pIndex)
{
	DWORD64 entityListBase{};
	if (ReadMem<DWORD64>(Offsets::ClientMod, { Offsets::EntityList }, entityListBase))
	{
		constexpr UINT entListStart{ 0x28 };
		constexpr UINT entPadding{ 0x20 };

		DWORD64 entityListEntry{};
		if (ReadMem<DWORD64>(entityListBase + entListStart,
			{ (entPadding * pIndex) },
			entityListEntry))

			return entityListEntry;
	}

	return 0;
}

std::vector<Entity> Cheat::GetEntities()
{
	std::vector<Entity> entities{};

	for (UINT i{ 0 }; i < 64; ++i)
	{
		Entity currEnt(GetEntity(i), i);

		if (!(currEnt.bInit && IsGoodEnt(&currEnt)))
			continue;

		entities.push_back(currEnt);
	}

	return entities;
}

Entity Cheat::GetLocalPlayer()
{
	Entity lpEntity{};
	DWORD64 entBaseAddr{};
	if (ReadMem<DWORD64>(Offsets::ClientMod, { Offsets::LocalPlayer::base }, entBaseAddr))
	{
		Entity lpEntity(entBaseAddr, 0);
		return lpEntity;
	}

	return Entity();
}

bool Cheat::IsGoodEnt(Entity* pEntity)
{
	if (pEntity->baseAddr == GetLocalPlayer().baseAddr)
		return false;

	if (pEntity->health <= 1)
		return false;

	//if (!pEntity->bAlive) TODO
	//    return false;

	/*if (pEntity->bDormant) TODO
		return false;*/

	if (pEntity->teamID == GetLocalPlayer().teamID)
		return false;

	return true;
}

bool Cheat::Run()
{
	Aimbot aBot{};
	ESP esp{};
	esp.InitWindow();

	while (!(GetAsyncKeyState(VK_DELETE) & 1))
	{
		if (Menu::IsOptionChanged())
			Menu::PrintMenu();

		if (Menu::bAimbot || Menu::bESP)
		{
			std::vector<Entity> entities{ GetEntities() };

			if (Menu::bAimbot)
				aBot.Run(entities);

			if (Menu::bESP)
				esp.Run(entities);
		}

		Sleep(10);
	}

	return true;
}
