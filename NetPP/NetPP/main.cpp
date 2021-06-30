#include "base.h"

int main()
{
	if (NetworkManager::StaticInit(SERVERPORT))
	{
		if(false == ClientManager::StaticInit())
			return 1;

		if (false == PacketManager::StaticInit())
			return 1;
		PacketManager::sInstance->OnCompleteRecv = CompleteRecvProcess;
		PacketManager::sInstance->OnCompleteSend = CompleteSendProcess;


		while (true)
		{
			if (false == NetworkManager::sInstance->DoFrame())
				break;
		}
	}
}