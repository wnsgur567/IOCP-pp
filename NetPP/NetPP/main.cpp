#include "base.h"

DWORD WINAPI WorkerThread(LPVOID arg)
{
	return 0;
}

int main()
{
	if (NetworkManager::StaticInit(SERVERPORT))
	{
		while (true)
		{
			if (false == NetworkManager::sInstance->DoFrame())
				break;
		}
	}
}