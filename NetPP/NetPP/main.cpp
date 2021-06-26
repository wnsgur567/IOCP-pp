#include "base.h"

DWORD WINAPI WorkerThread(LPVOID arg)
{
	return 0;

}

int main()
{
	if (NetworkManager::StaticInit(SERVERPORT))
	{
		if(false == ClientManager::StaticInit())
			return 1;

		while (true)
		{
			if (false == NetworkManager::sInstance->DoFrame())
				break;
		}
	}
}