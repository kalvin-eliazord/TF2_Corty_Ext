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

	//if (pEntity->health <= 0)
	//	return false;

	if (pEntity->isDead)
	    return false;

	/*if (pEntity->bDormant) TODO
		return false;*/

	if (pEntity->teamID == GetLocalPlayer().teamID)
		return false;

	return true;
}

bool Cheat::Run()
{
	Aimbot aBot{};
	ESP    esp{};
	if (!(esp.InitWin() && esp.CreateD3D9()))
		return false;

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

			if(Menu::bCleared) Menu::bCleared = false;
		}

		if (!(Menu::bESP || Menu::bCleared))
		{
			esp.d3d9Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
			esp.d3d9Device->Present(NULL, NULL, NULL, NULL);
			Menu::bCleared = true;
		}

		Sleep(10);
	}

	return true;
}