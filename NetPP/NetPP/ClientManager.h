#pragma once


class ClientManager
{
public:
	static std::unique_ptr<ClientManager> sInstance;
private:
	unsigned __int32 m_newClientID;
	std::unordered_map<unsigned __int32, ClientInfoPtr> m_IDtoCInfo_dic;
private:
	ClientManager() :m_newClientID(0) {}
	bool Init();
public:
	ClientManager(const ClientManager&) = delete;
	ClientManager& operator=(const ClientManager&) = delete;
	static bool StaticInit();
	~ClientManager();	
public:
	// id 셋팅하고 map 에 등록하기
	void RegistNewClient(ClientInfoPtr inpInfo);
};