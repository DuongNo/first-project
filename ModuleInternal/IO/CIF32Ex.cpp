#include "stdafx.h"
#include "CIF32Ex.h"
#define IO_BOARD_NO			0
#define SEND_OFFSET				0
#define READ_OFFSET				0
#define DEV_TIME_OUT			100L
#define DEV_TIME_OUT			100L

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
CCIF32Ex::CCIF32Ex()
{

}

CCIF32Ex::~CCIF32Ex()
{
}

BOOL CCIF32Ex::Open()
{
	if(::DevOpenDriver(IO_BOARD_NO) != DRV_NO_ERROR)
	{
		AfxMessageBox(_T("DeviceNet Controller를 DevOpenDriver 하지 못했습니다."), MB_ICONEXCLAMATION);
		return FALSE;        
	}
    else if(::DevInitBoard(IO_BOARD_NO, NULL) != DRV_NO_ERROR)
	{
		AfxMessageBox(_T("DeviceNet Controller를 DevInitBoard 하지 못했습니다."), MB_ICONEXCLAMATION);
		DevCloseDriver(IO_BOARD_NO);
		return FALSE;
	}
    else if(::DevSetHostState(IO_BOARD_NO, HOST_READY, 0L) != DRV_NO_ERROR) 
	{
		AfxMessageBox(_T("DeviceNet Controller를 DevSetHostState 하지 못했습니다."), MB_ICONEXCLAMATION);
		::DevExitBoard(IO_BOARD_NO);
		::DevCloseDriver(IO_BOARD_NO);
        return FALSE;
	}
	return TRUE;
}

BOOL CCIF32Ex::Close()
{
	BOOL bRet=TRUE;
	if(::DevSetHostState(IO_BOARD_NO, HOST_NOT_READY, 0L) != DRV_NO_ERROR)
		bRet = FALSE;
	
	if(::DevExitBoard(IO_BOARD_NO) != DRV_NO_ERROR)
		bRet = FALSE;
	
	if(::DevCloseDriver(IO_BOARD_NO) != DRV_NO_ERROR) 
		bRet = FALSE;

	return bRet;
}

short CCIF32Ex::ExchangeIO(void* pSendData, USHORT sSendSize, void* pRcvData, USHORT sRcvSize)
{
	return ::DevExchangeIO(IO_BOARD_NO, SEND_OFFSET, sSendSize, pSendData, READ_OFFSET,  sRcvSize, pRcvData,0L /*DEV_TIME_OUT*/);
}
