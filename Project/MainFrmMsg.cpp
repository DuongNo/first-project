#include "stdafx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LRESULT CMainFrame::OnSerialMsg(WPARAM wParam, LPARAM lParam)
{
	SERIALEvent* pMsg = (SERIALEvent*)lParam;
	if(!pMsg)
		return -1;

	SERIAL_MODE mode = (SERIAL_MODE)pMsg->message;

	switch(mode)
	{
	case SERIAL_GMS:
		break;
//kjpark 20161010 라벨프린터 PG PC 에서 사용		
#ifdef OPERATOR_PC
	case SERIAL_LABELPRINT:
		break;
#endif
	case SERIAL_LABELPRINT:
		{
			BYTE buf[SERIAL_MAX_BUFFER];
			memset(buf, 0x00, SERIAL_MAX_BUFFER);
			memcpy(&buf, (char*)pMsg->pParam, pMsg->nParam1);
			
#ifdef _DEBUG
			CString strTemp, strLogHeader;
			wchar_t wszMsg[SERIAL_MAX_BUFFER];
			MultiByteToWideChar(CP_ACP, 0, (char*)buf, pMsg->nParam1, wszMsg, SERIAL_MAX_BUFFER);
					
			strTemp.Format(_T("%s"), wszMsg);
			strTemp = strTemp.Mid(0, pMsg->nParam1);
			CString sdf;
			sdf.Format(_T("%s"), strTemp);
			
			SYSTEMTIME	time;
			GetLocalTime(&time);
			strLogHeader.Format(_T("%02u:%02u:%02u:%03u\t:_OnSerialMsg\n"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			TRACE(strLogHeader);					

#endif
		}		
		break;
	default:		
		break;
	}

	if(pMsg)
	{
		delete pMsg;
		pMsg = NULL;
	}
	return 0;
}