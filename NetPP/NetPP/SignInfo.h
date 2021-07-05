#pragma once

// ID �ߺ� ��� x
// ID ���� ������ ����
class SignInfo
{
public:
	std::string ID;
	std::string PW;
public:
	SignInfo() {}
	SignInfo(const char* inID, const char* inPW);
	SignInfo(std::string inID, std::string inPW);
	bool operator == (const SignInfo& other);
	bool operator != (const SignInfo& other);
	bool operator > (const SignInfo& other);
	bool operator < (const SignInfo& other);
};