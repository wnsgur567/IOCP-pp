#include "base.h"

int main()
{
	if (false == IOCPSession::StaticInit())
		return -1;

	if (false == IOCPSession::sInstance->MainLoop())
		return -1;

	IOCPSession::sInstance->Finalize();

	return 0;
}