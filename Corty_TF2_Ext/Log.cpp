#include "Log.h"

bool Log::IsOptionChanged()
{
	if (GetAsyncKeyState(VK_LSHIFT) & 1)
	{
		bAimbot = !bAimbot;
		return true;
	}
	//else if (GetAsyncKeyState(VK_F2) & 1)
	//{
	//	bTargetLock = !bTargetLock;
	//	return true;
	//}
	//else if (GetAsyncKeyState(VK_F3) & 1 &&
	//	smoothValue > 0)
	//{
	//	--smoothValue;
	//	return true;
	//}
	//else if (GetAsyncKeyState(VK_F4))
	//{
	//	++smoothValue;
	//	return true;
	//}
	//else if (GetAsyncKeyState(VK_F5) & 1 &&
	//	fovValue > 10)
	//{
	//	fovValue -= 10;
	//	return true;
	//}
	//else if (GetAsyncKeyState(VK_F6) & 1 &&
	//	fovValue < 100)
	//{
	//	fovValue += 10;
	//	return true;
	//}
	//else if (GetAsyncKeyState(VK_F8) & 1)
	//{
	//	bESP = !bESP;
	//	return true;
	//}

	return false;
}

void Log::PrintMenu()
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
	std::cout << "[+] AIMBOT - [LSHIFT] " << std::boolalpha << "->[[" << bAimbot << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] DELETE to exit \n";
}

void Log::PrintPtrErr(const std::map<std::string_view, intptr_t> pPtrList)
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