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
        Entity currEnt(GetEntity(i));

        if (!IsGoodEnt(&currEnt))
            continue;

        entities.push_back(currEnt);
    }

    return entities;
}

bool Cheat::IsGoodEnt(Entity* pEntity)
{
    if (!pEntity->bInit)
        return false;

    if (pEntity->baseAddr == LocalPlayer::GetBase().baseAddr)
        return false;

    if (pEntity->health <= 1)
        return false;

    //if (!pEntity->bAlive) TODO
    //    return false;

    /*if (pEntity->bDormant) TODO
        return false;*/

    if (pEntity->teamID == LocalPlayer::GetBase().teamID)
        return false;

    return true;
}

bool Cheat::Run()
{
    Aimbot aimbot{};
    //ESP esp{};
    while (!(GetAsyncKeyState(VK_DELETE) & 1))
    {
        if (Log::IsOptionChanged())
            Log::PrintMenu();

        if (Log::bAimbot /*|| bESP*/)
        {
            std::vector<Entity> entities{GetEntities() };

            if (Log::bAimbot)
                aimbot.Run(entities);

            //if(Log::bESP)
            //esp.Run(entities);
        }

        Sleep(10);
    }

    return true;
}
