#include "base.h"

std::unique_ptr<SignManager> Singleton<SignManager>::sInstance;

bool SignManager::StaticInitialize()
{
	sInstance.reset(new SignManager());
	return true;
}

bool SignManager::Initialize()
{
	if (false == LoadInfo())
		return false;

	return true;
}

void SignManager::Finalize()
{
}

SignManager::~SignManager()
{
}

bool SignManager::LoadInfo()
{
	// 임시 값 셋팅
	for (size_t i = 0; i < 4; i++)
	{
		SignInfoPtr newInfo = std::make_shared<SignInfo>("abcd" + ('e' + i), "1234");
		m_info_list.push_back(newInfo);
	}

	return true;
}

bool SignManager::SaveInfo()
{
	return true;
}

void SignManager::OnRecved(ClientInfoPtr inpInfo, InputMemoryStreamPtr inpStream)
{
	BaseManager::OnRecved(inpInfo, inpStream);

	// ...	
}

bool SignManager::DoFrame()
{
	return true;
}

SignManager::ResultInfo SignManager::SignUpProcess(const SignInfo inInfo)
{
	ResultInfo retData;

	for (const auto& item : m_info_list)
	{
		if (item->ID == inInfo.ID)
		{
			retData.result = E_SignResult::Fail;
			retData.failType = E_FailType::ID_exist;

			return retData;
		}
	}

	m_info_list.push_back(std::make_shared<SignInfo>(inInfo));

	retData.result = E_SignResult::Success;
	return retData;
}

SignManager::ResultInfo SignManager::DeleteAccountProcess(const SignInfo inInfo)
{
	ResultInfo retData;

	for (std::list<SignInfoPtr>::iterator it = m_info_list.begin();
		it != m_info_list.end(); ++it)
	{
		SignInfoPtr pInfo = *it;

		if (pInfo->ID == inInfo.ID)
		{
			if (pInfo->PW == inInfo.PW)
			{	// success
				retData.result = E_SignResult::Success;
				return retData;
			}
			// pw mismatch
			retData.result = E_SignResult::Fail;
			retData.failType = E_FailType::PW_mismatch;
			return retData;
		}
	}

	// id mismatch
	retData.result = E_SignResult::Fail;
	retData.failType = E_FailType::ID_mismatch;
	return retData;
}

SignManager::ResultInfo SignManager::SignInProcess(const SignInfo inInfo)
{
	ResultInfo retData;

	for (const auto& item : m_info_list)
	{
		if (item->ID == inInfo.ID)
		{
			if (item->PW == inInfo.PW)
			{	// success
				retData.result = E_SignResult::Success;
				return retData;
			}
			// pw mismatch
			retData.result = E_SignResult::Fail;
			retData.failType = E_FailType::PW_mismatch;
			return retData;
		}
	}
	// id mismatch
	retData.result = E_SignResult::Fail;
	retData.failType = E_FailType::ID_mismatch;
	return retData;
}

SignManager::ResultInfo SignManager::SignOutProcess(const SignInfo inInfo)
{
	// .. login 된 상태면 바로 로그아웃 시키도록....

	ResultInfo retData;

	return retData;
}
