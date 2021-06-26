#include "base.h"

std::unique_ptr<ClientManager> ClientManager::sInstance;

bool ClientManager::StaticInit()
{
	sInstance.reset(new ClientManager());
	
	return sInstance->Init();
}

ClientManager::~ClientManager()
{

}

bool ClientManager::Init()
{
	return true;
}

void ClientManager::RegistNewClient(ClientInfoPtr inpInfo)
{
	inpInfo->Init(m_newClientID);
	m_IDtoCInfo_dic.insert({ m_newClientID,inpInfo });
	++m_newClientID;
}
