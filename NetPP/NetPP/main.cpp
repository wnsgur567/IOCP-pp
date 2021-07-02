#include "base.h"

int main()
{
	if (ICOPNetworkManager::StaticInit(SERVERPORT))
	{
		if(false == ClientManager::StaticInit())
			return 1;

		if (false == PacketManager::StaticInit())
			return 1;

		while (true)
		{
			if (false == ICOPNetworkManager::sInstance->DoFrame())
				break;
		}
	}
}