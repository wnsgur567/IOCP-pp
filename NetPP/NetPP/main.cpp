#include "base.h"

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