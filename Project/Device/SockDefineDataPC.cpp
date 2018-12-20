#include "stdafx.h"
#include "SockDefineDataPC.h"
#include "Etc\Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void tag_Message_Unicode::operator =(const tag_Message &src)
{
	sMsgClass = src.sMsgClass;
	sMsgType = src.sMsgType;
	sEqst = src.sEqst;
	sUnit = src.sUnit;
	sCellPos = src.sCellPos;
	sTactPoint = src.sTactPoint;
	const int length = MAX_MES_COUNT*MAX_MES_SIZE;
	wchar_t buf[length];

	for(int i = 0; i < MAX_MES_COUNT; i++)
	{
		memset(buf, NULL, length*sizeof(wchar_t));
		CEtc::ConvertUnicode((char*)src.cMsgData[i], (wchar_t*)buf, length-1);
		strMsgData[i].Format(_T("%s"), buf);
	}

	memset(buf, NULL, length*sizeof(wchar_t));
	CEtc::ConvertUnicode((char*)src.cCellID, (wchar_t*)buf, length-1);
	strCellID.Format(_T("%s"), buf);
}

void tag_Message_Unicode::Clear()
{
	sMsgClass = CLASS_NONE;
	sMsgType = TYPE_SET_NONE;
	sEqst = E_EQST_MATCH_NONE;
	sUnit = 0;
	sCellPos = 0;
	sTactPoint = 0;
	for(int i = 0; i < MAX_MES_COUNT; i++)
	{
		strMsgData[i].Empty();
	}
	strCellID.Empty();
}

void tag_Message_Unicode::Convert_tag_Message(tag_Message *pMsg)
{
	pMsg->sMsgClass = sMsgClass;
	pMsg->sMsgType = sMsgType;
	pMsg->sEqst = sEqst;
	pMsg->sUnit = sUnit;
	pMsg->sCellPos = sCellPos;
	pMsg->sTactPoint = sTactPoint;

	for(int i = 0; i < MAX_MES_COUNT; i++)
	{
		memset(pMsg->cMsgData[i], NULL, MAX_MES_SIZE*sizeof(char));
		CEtc::ConvertMultibyte((wchar_t*)strMsgData[i].GetString(), (char*)pMsg->cMsgData[i], MAX_MES_SIZE-1);
	}

	memset(pMsg->cCellID, NULL, (MAX_CELL_ID+1)*sizeof(char));
	CEtc::ConvertMultibyte((wchar_t*)strCellID.GetString(), (char*)pMsg->cCellID, MAX_CELL_ID);
}
