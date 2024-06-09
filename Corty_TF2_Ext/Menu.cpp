#include "Menu.h"

bool Menu::IsOptionChanged()
{
	if (GetAsyncKeyState(VK_LSHIFT) & 1)
	{
		bAimbot = !bAimbot;
		return true;
	}
	else if (GetAsyncKeyState(VK_F2) & 1)
	{
		bTargetLock = !bTargetLock;
		return true;
	}
	else if (GetAsyncKeyState(VK_F3) & 1 &&
		iSmooth > 0)
	{
		--iSmooth;
		return true;
	}
	else if (GetAsyncKeyState(VK_F4))
	{
		++iSmooth;
		return true;
	}
	else if (GetAsyncKeyState(VK_F5) & 1 &&
		iFOV > 10)
	{
		iFOV -= 10;
		return true;
	}
	else if (GetAsyncKeyState(VK_F6) & 1 &&
		iFOV < 100)
	{
		iFOV += 10;
		return true;
	}
	else if (GetAsyncKeyState(VK_F7) & 1)
	{
		bESP = !bESP;
		return true;
	}

	return false;
}

void Menu::PrintMenu()
{
	system("cls");

	std::cout << R"(
 _____            _           _____         _                 
/  __ \          | |         |_   _|       (_)                
| /  \/ ___  _ __| |_ _   _    | |_ __ __ _ _ _ __   ___ _ __ 
| |    / _ \| '__| __| | | |   | | '__/ _` | | '_ \ / _ \ '__|
| \__/\ (_) | |  | |_| |_| |   | | | | (_| | | | | |  __/ |   
 \____/\___/|_|   \__|\__, |   \_/_|  \__,_|_|_| |_|\___|_|   
                       __/ |                                  
                      |___/                 
		)" << '\n';
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] AIMBOT [LSHIFT] " << std::boolalpha << "->[[" << bAimbot << "]] \n";
	std::cout << "[+] T_LOCK [F2]     " << std::boolalpha << "->[[" << bTargetLock << "]] \n";
	//std::cout << "[+] SMOOTH [F3- F4+]" << "->[[" << iSmooth << "]] \n"; TODO
	std::cout << "[+] FOV    [F5- F6+]" << "->[[" << iFOV << "]] \n";
	//std::cout << "[+] ESP    [F7]     " << std::boolalpha << "->[[" << bESP << "]] \n"; TODO

	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] DELETE to exit \n";
}

void Menu::PrintPtrErr(const std::map<std::string_view, intptr_t> pPtrList)
{
	std::cout << "[-] Memory initialization error : \n";

	if (pPtrList.empty())
	{
		std::cerr << "[-] Pointer map is empty. \n";
		system("PAUSE");
		return;
	}

	for (const auto& currPtr : pPtrList)
	{
		if (!currPtr.second)
			std::cout << "[-] ";
		else
			std::cout << "[+] ";

		std::cout << currPtr.first << " = 0x" << std::hex << currPtr.second << '\n';
	}
	system("PAUSE");
}