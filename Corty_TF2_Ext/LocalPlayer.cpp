#include "LocalPlayer.h"

Entity LocalPlayer::GetBase()
{
    Entity lpEntity{};
    DWORD64 entBaseAddr{};
    if (ReadMem<DWORD64>(Offsets::ClientMod, { Offsets::LocalPlayer::base }, entBaseAddr))
    {
        Entity lpEntity(entBaseAddr);
        return lpEntity;
    }

    return Entity();
}

bool LocalPlayer::SetMyAngles(const Vector3& pBotAngles)
{
    if (!WriteProcessMemory(Offsets::hProc, (Vector3*)(Offsets::EngineMod + Offsets::LocalPlayer::vAngles), &pBotAngles, sizeof(Vector3), nullptr))
    {
        std::cerr << "[-] Cannot write to localPlayer angles. \n";
        system("PAUSE");
        return false;
    }

    //std::cout << "[+] Aimbot working. \r \t";
    return true;
}