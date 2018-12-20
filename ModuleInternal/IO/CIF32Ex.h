#pragma once

#include "CIF50/CIFUSER.H"
#pragma comment(lib, "CIF32DLL.LIB")


class CCIF32Ex
{
public:
	CCIF32Ex();
	~CCIF32Ex();

public:
	BOOL		Open();
	BOOL		Close();

	short		ExchangeIO(void* pSendData, USHORT sSendSize/*Byte*/, void* pRcvData, USHORT sRcvSize/*Byte*/);
};
