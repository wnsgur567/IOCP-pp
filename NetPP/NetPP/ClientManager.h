#pragma once


class ClientManager
{
public:
	static std::unique_ptr<ClientManager> sInstance;
	using id_t = unsigned __int32;
	using pid_t = PacketManager::id_t;
private:
	id_t m_newClientID;
	std::unordered_map<id_t, ClientInfoPtr> m_IDtoCInfo_dic;
private:
	ClientManager() :m_newClientID(0) {}
	bool Init();
public:
	ClientManager(const ClientManager&) = delete;
	ClientManager& operator=(const ClientManager&) = delete;
	static bool StaticInit();
	~ClientManager();	
public:
	// id �����ϰ� map �� ����ϱ�
	void RegistNewClient(ClientInfoPtr inpInfo);
	ClientInfoPtr GetClientInfo(const id_t inId);
};