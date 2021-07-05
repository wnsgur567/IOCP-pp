#pragma once

class SignManager : public BaseManager, public Singleton<SignManager>
{
	friend class Singleton;
public:
	static constexpr size_t MAX_IDSIZE = 30;
	static constexpr size_t MAX_PWSIZE = 40;
	enum class E_SignResult
	{
		None,
		Fail,
		Success,
	};
	enum class E_FailType
	{
		None,
		ID_exist,
		ID_mismatch,
		PW_mismatch,
	};

	struct ResultInfo
	{
		E_SignResult result;
		E_FailType failType;
		ResultInfo() : result(E_SignResult::None), failType(E_FailType::None) {}
	};

protected:
	SignManager() {}
protected:
	bool StaticInitialize() override;
public:
	bool Initialize() override;
	void Finalize() override;
	~SignManager();
private:
	bool LoadInfo();	// 정보를 읽어 list 에 셋팅
	bool SaveInfo();	// 정보 변경사항을 저장
	std::list<SignInfoPtr> m_info_list;
protected:
	void OnRecved(ClientInfoPtr, InputMemoryStreamPtr) override;
	bool DoFrame() override;
	//void Send(OutputMemoryStreamPtr) override;
public:
	ResultInfo SignUpProcess(const SignInfo inInfo);
	ResultInfo DeleteAccountProcess(const SignInfo inInfo);
	ResultInfo SignInProcess(const SignInfo inInfo);
	ResultInfo SignOutProcess(const SignInfo inInfo);
};