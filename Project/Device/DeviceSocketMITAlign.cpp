#pragma once
#include "stdafx.h"
#include "SocketDialog.h"
#include "CheetahApp.h"
#include "BankGuild\RecipeBank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
void CSocketDialog::SetMITAlignDataProcessing(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	ParseMITAlignMessage((PBYTE)pSocket->GetLastMsg(), nMsgLen);

}
int CSocketDialog::ParseMITAlignMessage( PBYTE pMsg, int nLen )
{
	int nPacketLen = 0;
	SMPACK_CONTACT_ALIGN_HEAD	packHeader;
	if(nLen==0)
		return 0;

	if(nLen<0)
	{
		return SOCK_PARS_SYNTEXT_ERR;
	}

	//1. STX 유효성 검사
	if(pMsg[0]!=STX)
	{
		return SOCK_PARS_SYNTEXT_ERR;
	}
	//2. Header 유효성 검사
	if(nLen<sizeof(SMPACK_CONTACT_ALIGN_HEAD))
	{
		return nLen;
	}
	memcpy(&packHeader, pMsg, sizeof(SMPACK_CONTACT_ALIGN_HEAD));

	if(packHeader.nMsgID==SMID_ALIGN_END)
	{
		nPacketLen = sizeof(SMPACK_CONTACT_ALIGN_END);
		if(nLen<nPacketLen)
			return nLen;

		SMPACK_CONTACT_ALIGN_END msg;
		memcpy(&msg, pMsg, nPacketLen);
		if(msg.cETX!=ETX)
			return SOCK_PARS_SYNTEXT_ERR;

		for(int i = 0; i < JIG_CH_MAX; i++)
		{
			theProcBank.m_dPA_MITAlignX[i] = msg.Result[i].dX;
			theProcBank.m_dPA_MITAlignY[i] = msg.Result[i].dY;
			theProcBank.m_dPA_MITAlignTH[i] = msg.Result[i].dTH;
			theProcBank.m_dPA_MITAlignScore[i][E_MARK_LEFT] = msg.Result[i].dScore[E_MARK_LEFT];
			theProcBank.m_dPA_MITAlignScore[i][E_MARK_RIGHT] = msg.Result[i].dScore[E_MARK_RIGHT];
		}

		if((ALIGN_RESULT)msg.Result[JIG_CH_1].nResult == ALIGN_RESULT_GOOD)
		{
			theProcBank.m_dPA_MITAlignX[JIG_CH_1] *= -1.;
			theProcBank.m_dPA_MITAlignY[JIG_CH_1] *= -1.;
		}
		else
		{
			theProcBank.m_dPA_MITAlignX[JIG_CH_1] = 0.;
			theProcBank.m_dPA_MITAlignY[JIG_CH_1] = 0.;
			theProcBank.m_dPA_MITAlignTH[JIG_CH_1] = 0.;
		}
		if((ALIGN_RESULT)msg.Result[JIG_CH_2].nResult == ALIGN_RESULT_GOOD)
		{
			theProcBank.m_dPA_MITAlignY[JIG_CH_2] *= -1.;
		}
		else
		{
			theProcBank.m_dPA_MITAlignX[JIG_CH_2] = 0.;
			theProcBank.m_dPA_MITAlignY[JIG_CH_2] = 0.;
			theProcBank.m_dPA_MITAlignTH[JIG_CH_2] = 0.;
		}

		// Sequence에서 m_nPA_AlignResult를 보고 움직이므로 m_nPA_AlignResult를 맨 마지막에 넣어준다 [12/2/2015 OSC]
		for(int i = 0; i < JIG_CH_MAX; i++)
		{
			theProcBank.m_nPA_MITAlignResult[i] = (ALIGN_RESULT)msg.Result[i].nResult;
		}

		//2018-03-08,MGYUN, PRE-ALIGN CH1 END 후 MELSEC 기록.
		theMonitorBank.WritePREAlignCH1((ALIGN_RESULT)msg.Result[JIG_CH_1].nResult, theProcBank.m_dPA_MITAlignX[0], theProcBank.m_dPA_MITAlignY[0], theProcBank.m_dPA_MITAlignTH[0]);
		//2018-03-08,MGYUN, PRE-ALIGN CH2 END 후 MELSEC 기록.
		theMonitorBank.WritePREAlignCH2((ALIGN_RESULT)msg.Result[JIG_CH_2].nResult, theProcBank.m_dPA_MITAlignX[1],theProcBank.m_dPA_MITAlignY[1], theProcBank.m_dPA_MITAlignTH[1]);
		
		theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tCONTACT ALIGN END"), SOCKET_MITALIGN, SOCKET_MAIN); 
	}
	else if( (packHeader.nMsgID==SMID_HALF_ALIGN_1_END) || (packHeader.nMsgID==SMID_HALF_ALIGN_2_END) )
	{
		nPacketLen = sizeof(SMPACK_HALF_ALIGN_END);
		if(nLen<nPacketLen)
			return nLen;

		SMPACK_HALF_ALIGN_END msg;
		memcpy(&msg, pMsg, nPacketLen);
		if(msg.cETX!=ETX)
			return SOCK_PARS_SYNTEXT_ERR;

		// A존 꺼는 2에 들어온다 [5/6/2013 OSC]
		int nA = 2;
		if(msg.Result[nA].nResult != ALIGN_RESULT_SKIP)
		{
			theProcBank.m_dVI_MITAlignScore = msg.Result[nA].dScore;
			theProcBank.m_dVI_MITAlignTH = msg.Result[nA].dTH;
			theProcBank.m_nVI_MITAlignResult = (ALIGN_RESULT)msg.Result[nA].nResult;
		}
		/*
			int	   nResult;
			double dX;	
			double dY;	
			double dTH;	
			double dScore;
		*/
		if(packHeader.nMsgID==SMID_HALF_ALIGN_1_END)
		{
			theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tVISION ALIGN L END"), SOCKET_MITALIGN, SOCKET_MAIN);
		}
		else
		{
			theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tVISION ALIGN R END"), SOCKET_MITALIGN, SOCKET_MAIN);
		}
	}
	else if(packHeader.nMsgID==SMID_ALIGN_MARK_POS_SAVE_RESULT)
	{
		nPacketLen = sizeof(SMPACK_ALIGN_MARK_POS_RESULT);
		if(nLen<nPacketLen)
			return nLen;

		SMPACK_ALIGN_MARK_POS_RESULT msg;
		memcpy(&msg, pMsg, nPacketLen);
		if(msg.cETX!=ETX)
			return SOCK_PARS_SYNTEXT_ERR;

		theProcBank.m_bMarkSaveSuccess = msg.bSuccess;
		theProcBank.m_bMarkSaveRecv = TRUE;
	}
	else if(packHeader.nMsgID==SMID_VISION_MARK_POS_SAVE_RESULT)
	{
		nPacketLen = sizeof(SMPACK_VISION_MARK_POS_RESULT);
		if(nLen<nPacketLen)
			return nLen;

		SMPACK_VISION_MARK_POS_RESULT msg;
		memcpy(&msg, pMsg, nPacketLen);
		if(msg.cETX!=ETX)
			return SOCK_PARS_SYNTEXT_ERR;

		theProcBank.m_bMarkSaveSuccess = msg.bSuccess;
		theProcBank.m_bMarkSaveRecv = TRUE;
	}	
	return TRUE;
}


BOOL CSocketDialog::SendMsgToContactAlign( JIG_ID jig, JIG_CH ch, CString strInnerID1, CString strInnerID2, CString strCellID1, CString strCellID2 )
{
	SMPACK_CONTACT_ALIGN_START	msg;
	char cBuff[64];

	CEtc::ConvertMultibyte((wchar_t*)theRecipeBank.m_strRecipeName.GetString(), cBuff, 64);

	msg.Jig = jig;
	msg.Ch = ch;
	memset(msg.cRcpName, NULL, 20);
	memcpy(msg.cRcpName, cBuff, 19);
	msg.lTryAlign = 0;
	msg.lTryAlign |= (strInnerID1.GetLength() > 1) ? FG_CONTACT_L:0;
	msg.lTryAlign |= (strInnerID2.GetLength() > 1) ? FG_CONTACT_R:0;
	msg.dMarkDistance = theRecipeBank.m_Cell.m_dMarkDistance;

	CEtc::ConvertMultibyte(	(wchar_t*)strInnerID1.GetString(), msg.szInnerID[0], 9);
	CEtc::ConvertMultibyte(	(wchar_t*)strCellID1.GetString(), msg.szCellID[0], 19);
	msg.szInnerID[0][9] = NULL;
	msg.szCellID[0][19] = NULL;

	CEtc::ConvertMultibyte(	(wchar_t*)strInnerID2.GetString(), msg.szInnerID[1], 9);
	CEtc::ConvertMultibyte(	(wchar_t*)strCellID2.GetString(), msg.szCellID[1], 19);
	msg.szInnerID[1][9] = NULL;
	msg.szCellID[1][19] = NULL;

	int nLen = sizeof(SMPACK_CONTACT_ALIGN_START);
	if(m_pSocket_MITAlign.Send(&msg, nLen) == nLen)
	{
		if(ch == 0)
		{
			theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tCONTACT ALIGN START Jig %c, Ch %d,InnerID %s"), 
				SOCKET_MAIN, SOCKET_MITALIGN, _T('A') + msg.Jig, msg.Ch+1,strInnerID1);
		}
		else
		{
			theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tCONTACT ALIGN START Jig %c, Ch %d,InnerID %s"), 
				SOCKET_MAIN, SOCKET_MITALIGN, _T('A') + msg.Jig, msg.Ch+1,strInnerID2);
		}
		return TRUE;
	}
	else
	{
		if(ch == 0)
		{
			theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tSEND FAIL : CONTACT ALIGN START Jig %c, Ch %d,InnerID %s"), 
				SOCKET_MAIN, SOCKET_MITALIGN, _T('A') + msg.Jig, msg.Ch+1,strInnerID1);
		}
		else
		{
			theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tSEND FAIL : CONTACT ALIGN START Jig %c, Ch %d,InnerID %s"), 
				SOCKET_MAIN, SOCKET_MITALIGN, _T('A') + msg.Jig, msg.Ch+1,strInnerID2);
		}
		
		return FALSE;
	}
}

BOOL CSocketDialog::SendMsgToVisionAlign(JIG_CH ch, BOOL bFirst, CString strInnerID, CString strCellID )
{
	SMPACK_HALF_ALIGN_START	msg;
	if(bFirst)
	{
		msg.nMsgID = SMID_HALF_ALIGN_1_START;
	}
	else
	{
		msg.nMsgID = SMID_HALF_ALIGN_2_START;
	}

	msg.Ch = (JIG_CH)ch;
	msg.lTryAlign = 0;
	msg.lTryAlign |= (strInnerID.GetLength() > 1) ? FG_VISION_A:0;

	msg.dMarkDistance = theRecipeBank.m_Cell.m_dMarkDistance;

	CEtc::ConvertMultibyte(	(wchar_t*)strInnerID.GetString(), msg.szInnerID[0], 9);
	CEtc::ConvertMultibyte(	(wchar_t*)strCellID.GetString(), msg.szCellID[0], 19);
	msg.szInnerID[0][9] = NULL;
	msg.szCellID[0][19] = NULL;

	CEtc::ConvertMultibyte(	(wchar_t*)strInnerID.GetString(), msg.szInnerID[1], 9);
	CEtc::ConvertMultibyte(	(wchar_t*)strCellID.GetString(), msg.szCellID[1], 19);
	msg.szInnerID[1][9] = NULL;
	msg.szCellID[1][19] = NULL;

	int nLen = sizeof(SMPACK_HALF_ALIGN_START);
	if(m_pSocket_MITAlign.Send(&msg, nLen) == nLen)
	{
		theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\t VISION ALIGN %d %s START"), 
			SOCKET_MAIN, SOCKET_MITALIGN, ch+1, bFirst ? _T("L"):_T("R"));
		return TRUE;
	}
	else
	{
		theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tSEND FAIL : VISION ALIGN %d, %s START"), 
			SOCKET_MAIN, SOCKET_MITALIGN, ch+1, bFirst ? _T("L"):_T("R") );
		return FALSE;
	}
}

BOOL CSocketDialog::SendMsgToContactSave( JIG_ID jig, JIG_CH ch )
{
	SMPACK_ALIGN_MARK_POS_SAVE	msg;

	msg.Jig = jig;
	msg.Ch = ch;
	msg.lTrySave = FG_CONTACT_L | FG_CONTACT_R;

	theProcBank.m_bMarkSaveRecv = FALSE;


	int nLen = sizeof(SMPACK_ALIGN_MARK_POS_SAVE);
	if(m_pSocket_MITAlign.Send(&msg, nLen) == nLen)
	{
		theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tCONTACT MARK POS SAVE Jig %c, Ch %d"), 
			SOCKET_MAIN, SOCKET_MITALIGN, _T('A') + msg.Jig, msg.Ch+1);
		return TRUE;
	}
	else
	{
		theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tSEND FAIL : CONTACT MARK POS SAVE %c, Ch %d"), 
			SOCKET_MAIN, SOCKET_MITALIGN, _T('A') + msg.Jig, msg.Ch+1);
		return FALSE;
	}
}

BOOL CSocketDialog::SendMsgToVisionSave( JIG_CH ch, BOOL bFirst )
{
	SMPACK_VISION_MARK_POS_SAVE	msg;

	msg.Ch = ch;
	msg.lTrySave = FG_VISION_A;
	msg.bFirst = bFirst;

	theProcBank.m_bMarkSaveRecv = FALSE;
	int nLen = sizeof(SMPACK_VISION_MARK_POS_SAVE);
	if(m_pSocket_MITAlign.Send(&msg, nLen) == nLen)
	{
		theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tVISION MARK POS SAVE Ch %d"), 
			SOCKET_MAIN, SOCKET_MITALIGN, msg.Ch+1);
			return TRUE;
	}
	else
	{
		theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tSEND FAIL : VISION MARK POS SAVE Ch %d"), 
			SOCKET_MAIN, SOCKET_MITALIGN, msg.Ch+1);
		return FALSE;
	}
}


BOOL CSocketDialog::SendMsgToGrabMode()
{
	SMPACK_GRAB_MODE	msg;

	theProcBank.m_bMarkSaveRecv = FALSE;
	int nLen = sizeof(SMPACK_GRAB_MODE);
	if(m_pSocket_MITAlign.Send(&msg, nLen) == nLen)
	{
		theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tGRAB MODE"), 
			SOCKET_MAIN, SOCKET_MITALIGN);
		return TRUE;
	}
	else
	{
		theLog[LOG_MITALIGN].AddBuf(_T("%s -> %s\tSEND FAIL : GRAB MODE"), 
			SOCKET_MAIN, SOCKET_MITALIGN);
		return FALSE;
	}
}
