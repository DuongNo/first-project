#pragma once
#include "stdafx.h"
#include "SocketDialog.h"
#include "Serial\CQueueSerial.h"
#include "BankGuild\CellBank.h"
#include "BankGuild\CommonDefineBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BOOL CSocketDialog::SendMassageToPatternGenerator(CString strCmdType, int jig, int ch, CString strExtraData )
{		
	CString strLog;
	//kjpark 20161029 PG send Packet ���� ����
	CClientSocket* pSocket = m_pSocket_PatternGenerator.GetClientAt(SOCKET_PGPC);
	//2018-07-23,NAM,Testing 
	CCellTag	tag1;
	CCellInfo* pCellInfo;
	pCellInfo = NULL;

	if ( pSocket == NULL )		// ���� ����
	{
		//kjpark 20161021 Setup ��ȣ�� UI ����
		strLog.Format(_T("%s -> %s\tSEND FAIL(Not Connect) CMD : %s"), SOCKET_MAIN, SOCKET_PGPC, strCmdType);
		SetAddLog(strLog);

		return FALSE;
	}

	//if ( m_bSystemError[jig][ch] )			// �����ϰ��� �ϴ� ��ȣ�⿡ �̻��� �ִٸ� RESET Ȥ�� A-Zone ��ȯ�� ��쿡�� �����Ѵ�.
	//{
	//	if( (strCmdType == SET_ZONE_A) || (strCmdType == RESET) ) //JH-PARK 2014.1.20
	//	{
	//		m_bSystemError[jig][ch] = FALSE;						// �̻� ��Ȳ �̸� ����
	//	}
	//	else
	//	{
	//		m_pLogPG->Log(_T("%s -> %s\tSEND FAIL(SYSTEM_ERROR) CMD : %s"), SOCKET_MAIN, SOCKET_PGPC, strCmdType);

	//		return FALSE;
	//	}
	//}

	enum { eMAX_LEN = 2048, };			// ���� �ִ� ����
	BYTE ucData[eMAX_LEN];

	CString strBuff		= _T("");
	CString strCount	= _T("");
	CString strTemp		= _T("");
	int nCount = 0;

	memset(ucData,	NULL,	sizeof(ucData));

	ucData[0] = STX;

	//Dest Code
	ucData[1] = CODEPGPC;

	// Message, ������ ���ڿ� �����Ͱ� �ʿ���ϴ� ����̶��,
	if (	(strCmdType == MTP_VERIFY) || 
			(strCmdType == MTP_WRITE) || 
			//2018-07-24,NAM,Remove this case
			(strCmdType == CELL_LOADING) || 
			(strCmdType == PG_CIRCLE) || 
			(strCmdType == PG_MSG)  ||
			(strCmdType == PG_ROUTE) ||				//2017-02-25, jhLee, Job file download����, MES Server�� ��θ� �����Ѵ�. ä�ι�ȣ�� 0�� �����Ѵ�.
			(strCmdType == PG_TMD) ||					//							Download�� �����̸��� Checksum ����
			(strCmdType == PATTERN_SELECT) )			// 2015-06-02, Ư�� ���� ���� ���� ��� �߰�
	{
		//2018-07-14,GHLEE, �ɿ��� å�Ӵ� ��û�������� TMD�� ������ CELL_LOADING �޼��� �������� CELLID_INNERID_PRODUCTID �߰�
		pCellInfo=theCellBank.SearchCellInfo(jig,ch,CELL_POS_AZONE_CH1,CELL_POS_DZONE_CH2);
		strBuff.Format(_T("Ch,%d,%s%s,%s_%s_%s"), ((jig * eMAX_ChCount ) + ch + 1), strCmdType, strExtraData,pCellInfo->m_strCellID,pCellInfo->m_strInnerID,pCellInfo->m_strProductID);
		//strBuff.Format(_T("Ch,%d,%s,%s"), ((jig * eMAX_ChCount ) + ch + 1), strCmdType, strExtraData);
	}
	//2018-07-24,NAM, Add the Cell Information if strCmdType is CELL_LOADING
	/*else if (strCmdType == CELL_LOADING)
	{
		//2018-07-14,GHLEE, �ɿ��� å�Ӵ� ��û�������� TMD�� ������ CELL_LOADING �޼��� �������� CELLID_INNERID_PRODUCTID �߰�
		pCellInfo=theCellBank.SearchCellInfo(jig,ch,CELL_POS_AZONE_CH1,CELL_POS_DZONE_CH2);
		strBuff.Format(_T("Ch,%d,%s%s,%s_%s_%s"), ((jig * eMAX_ChCount ) + ch + 1), strCmdType, strExtraData,pCellInfo->m_strCellID,pCellInfo->m_strInnerID,pCellInfo->m_strProductID);
	}*/
	else if( (strCmdType == PG_TURN_START) || (strCmdType == PG_TURN_FINISH) ||
			(strCmdType == PG_TMD) || (strCmdType == PG_TMD_COPY_START) || (strCmdType == PG_TMD_CHECKSUM_START) || (strCmdType == PG_CLIENT_CHECKSUM_START) ||
			(strCmdType == PG_TMD_DOWNLOAD_START) )
	{
		strBuff.Format(_T("Ch,0,%s"), strCmdType);
	}
	else
		strBuff.Format(_T("Ch,%d,%s"), ((jig * eMAX_ChCount ) + ch + 1), strCmdType);		// �� �ܴ� ä�� �Ϸù�ȣ�� �Ѱ��ش�.

	// Count
	nCount = strBuff.GetLength();
	strCount.Format(_T("%04x"),nCount);

	// Merge
	strTemp.Format(_T("%s%s"), strCount, strBuff);
	nCount = strTemp.GetLength();

	CEtc::ConvertMultibyte((wchar_t *)strTemp.GetString(), (char *)&ucData[2], eMAX_LEN-2);			// WCHAR CString�� Char ������ ��ȯ�����ش�.
	USHORT checksum = GetCRC16_Checksum((UCHAR *)&ucData[1], nCount+1);							// CRC16�� ������ش�.

	CString strCheckSum;
	strCheckSum.Format(_T("%04X"), checksum);

	CEtc::ConvertMultibyte((wchar_t *)strCheckSum.GetString(), (char *)&ucData[nCount + 2], eMAX_LEN-2-nCount);

	// ETX
	ucData[nCount + 6] = ETX;
	
#if 0
	if ( m_pSocket_PatternGenerator.SendClient(m_pSocket_PatternGenerator.GetSocketName(), ucData, nCount + 7) )
	{
		
		strLog.Format(_T("%s -> %s\t%d\t%s"), SOCKET_MAIN, SOCKET_PGPC, nCount + 7, strTemp);
		SetAddLog(strLog);
		
		return TRUE;
	}
#else
	//kjpark 20161029 PG send Packet ���� ����
	if (m_pSocket_PatternGenerator.SendClient(pSocket->GetSocketName(), ucData, nCount + 7))
	{
		strLog.Format(_T("%s -> %s\t%d\t%s"), SOCKET_MAIN, SOCKET_PGPC, nCount + 7, strTemp);
		SetAddLog(strLog);
		return TRUE;
	}
#endif

	//// ���� ����	
	strLog.Format(_T("%s -> %s\tSEND FAIL : %d\t%s"), SOCKET_MAIN, SOCKET_PGPC, nCount + 7, strTemp);
	SetAddLog(strLog);
	return FALSE;
}

void CSocketDialog::SetPatternGeneratorDataProcessing(CDataSocket* pSocket)
{
		wchar_t wszMsg[4096];
		int nMsgLen = pSocket->GetLastLength();

		if(nMsgLen > 4095) nMsgLen = 4095;

		CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);
		wszMsg[nMsgLen] = NULL;
		CString strMsg;
		strMsg.Format(_T("%s"), wszMsg);

		CString strLog;
		

		//2014-09-01, jhLee, Line Check�� ��� Alive ���¸� Ȯ���ϴ� Heart beat ������ �޼����̹Ƿ�
		// Log�� ������ �ʵ��� �Ѵ�.
		// TMDCHECK��ɿ� CHECK�� �� �־ �ش� �κ� �ּ� �Ѵ� 2017/06/16 HSK.
/*		if (strMsg.Find( PG_CHECK ) >= 0)		// "LineCheck" �۱Ͱ� ���� ���� ��쿡�� Log�� �����.
		{			
			return;
		}
		else*/ if (strMsg.Find( PG_LINECHECK ) >= 0)		// "LineCheck" �۱Ͱ� ���� ���� ��쿡�� Log�� �����.
		{			
			return;
		}
		// TMDCHECK��ɿ� CHECK�� �� �־ �ش� �κ� �ּ� �Ѵ� 2017/06/16 HSK.
		else if (strMsg.Find( PG_LINECHECK ) == -1/* && strMsg.Find( PG_CHECK ) == -1*/)		// "LineCheck" �۱Ͱ� ���� ���� ��쿡�� Log�� �����.
		{			
			strLog.Format(_T("%s <--- %s\t%d\t%s"), SOCKET_MAIN, pSocket->GetSocketName(), nMsgLen, strMsg);
			SetAddLog(strLog);
		}

		int i = 0;
		while (strMsg.Find(STX, 0) != -1)
		{
			if( i > 0) 
			{
				//strLog.Format(_T("%dth Parsing %d\t%s"), i+1, strMsg.GetLength(), strMsg);
				//SetAddLog(strLog);				
			}

			strMsg = ParsePatternGeneratorMessage(strMsg);
			i++;
		}

		if ( strMsg.GetLength() > 0)
		{
			//strLog.Format(_T("Parsing After %d\t%s"), strMsg.GetLength(), strMsg);
			//SetAddLog(strLog);			

		}
}

CString	CSocketDialog::ParsePatternGeneratorMessage(CString strMsg)
{
	// ��ȣ�⿡�� ���ŵ� ������ ó���Ѵ�.
	int nIndex = strMsg.Find(STX);			// STX(������ġ)�� ã�´�.
	CString strLog;

	if ( nIndex > 0)	// 0��ġ ���Ŀ� STX�� �߰ߵǾ��ٸ�,
	{
		strLog.Format(_T("Before STX Packet[%d] : %s"), nIndex, strMsg.Mid(0, nIndex));
		theLog[LOG_SOCKET_PG].AddBuf(strLog);		
		strMsg.Delete(0, nIndex);		// ���� �����ʹ� �����Ͽ� �����Ѵ�.
	}
	
	CCellTag	tag;
	CCellInfo* pCell;
	CCellInfo  rTempCell;

	CString strCommand = _T("");
	CString strJudge = _T("");

	int nJigNo;
	int ch;
	int nIdx;						// Cell ������ �Ϸù�ȣ

	CString strTemp;
	CTokenizer t(strMsg);
	int nEtx;

	// Ack
	strTemp = t.GetNextToken(_T(","));

	if(strTemp.Find(PG_ACK) != -1)
	{
		strCommand = PG_ACK;
		t.GetNextToken(_T(","));
	}
	else if(strTemp.Find(PG_DOWN) != -1)
	{
		strCommand = PG_DOWN;
		t.GetNextToken(_T(","));
	}
	else if(strTemp.Find(PG_RUN) != -1)
	{
		strCommand = PG_RUN;
		t.GetNextToken(_T(","));
	}

	// Channel
	strTemp	= t.GetNextToken(_T(","));
	nIdx		= _tstoi(strTemp.GetString()) - 1;		// �Ϸù�ȣ�� ���´�. 1 ~ 4 -> 0 ~ 3���� ���
	nJigNo		= (int)( nIdx / JIG_CH_MAX );				// Jig ��ȣ ���, 0 ~ 1
	ch			= (int)( nIdx % JIG_CH_MAX );				// ä�� ��ȣ ���, 0 ~ 1
	
	// Command
	if( (strCommand != PG_ACK) && (strCommand != PG_DOWN) && (strCommand != PG_RUN) )
	{
		strCommand = t.GetNextToken(_T(","));
		nEtx = strCommand.Find(ETX);

		if (nEtx > 5)	strCommand = strCommand.Mid(0, nEtx-4);

		// �������°� Parsing���� ����..
		if( (strCommand == PG_LINECHECK) )
		{
			return PacketClear(SOCKET_PGPC, strMsg);
		}
	}

	nEtx = strMsg.Find(ETX);
	if(nEtx > 0)
		strLog.Format(_T("RECV %s"), strMsg.Left(strMsg.Find(ETX)+1));
	else
		strLog.Format(_T("RECV %s"), strMsg);
	SetAddLog(strLog);

	//2017-03-01, jhLee, Job file download ���� �߰�
	if ( nIdx < 0 ) // == 0 && nJigNo==0 && ch==0 )
	{
		pCell = &rTempCell;
	}
	else
	{
		tag = theCellBank.SearchCellTag((JIG_ID)nJigNo, (JIG_CH)ch);

		if ( (tag.IsExist() == FALSE) && (nIdx >= 0) )		// ��� Cell ������ ã�� ���ϸ� ���̻��� ó���� ���ǹ��ϴ� !
		{		
			strLog.Format(_T("%s <==== %s ** Cell Not Found Jig:%d Ch:%d Msg:%s"), SOCKET_MAIN, SOCKET_PGPC, nJigNo+1, ch+1, strMsg);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
			// �ΰ� �پ ���� ���� ����� ���� STX���� �����
			return PacketClear(SOCKET_PGPC, strMsg);
		}

		pCell = theCellBank.GetCellInfo(tag);
	
	}

	//pCell = theCellBank.SearchCellInfo((JIG_ID)nJigNo, (JIG_CH)ch);

	// Judge
	strJudge = t.GetNextToken(_T(","));

	nEtx = strJudge.Find(ETX);
	if (nEtx > 5)
	{
		strJudge = strJudge.Mid(0, nEtx-4);
	}

	// �Ʒ� ����� ������ ó��
	if(	(	TMD_INFO != strCommand) 
		&& (SET_ZONE_A != strCommand) 
		&& (SET_ZONE_B != strCommand)
		&& (SET_ZONE_C != strCommand) 
		&& (SET_ZONE_D != strCommand) 
		&& (PG_ACK != strCommand) 
		&& (PG_RUN != strCommand) )
	{
		strLog.Format(_T("<= Jig:%d Ch:%d InnerID:%s CellID:%s Command:%s Judge:%s"), 
						nJigNo+1, ch+1, pCell->m_strInnerID, pCell->m_strCellID, strCommand, strJudge);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
	}//of if 

    //2017-03-01, jhLee, Job file download ���ÿ��� ����
	//// ���� ��ɿ����� ���� ����� ���ΰ� ACK�� ������� ���
	//if ( strCommand == PG_ACK )	// ACK�� ������°Ŵ� �ʿ��� �͸� Cell Search�Ѵ� [6/20/2013 OSC]
	//{
	//	PatternGeneratorACK(pCell, strJudge);
	//}
	//else 
	if( strCommand == PG_RUN )	// ���� ��ɿ� ���� ���� ����� ���ŵǴ� ���
	{
		//BOOL		RecvTurnOn;
		strLog.Format(_T("%s <==== %s %s, Jig:%d Ch:%d Run Recived"),SOCKET_MAIN, SOCKET_PGPC, strCommand, pCell->m_JigId+1, pCell->m_JigCh+1);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		if(strJudge == TURN_OFF)
		{
			pCell->m_Zone_D.TurnOffChk = TRUE;
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("Turn Off Receive"));
		}
		//2017-02-26, jhLee, Job file download ���� ������ ó���Ѵ�. "RUN"�� �����Ͽ���.
		else if ( strJudge == PG_TMD )	// _T("TMD") // MES Server�� Model file�� ����� �ٿ�ε� �� TMD ���ϰ� DLL ��� �� ������ Checksum ����
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_TMD Receive"));
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_TMDSend;		// TxHost���� TMD_SEND�� �����Ͽ���.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;		// ACKFH ���������� ó���� ���ƴ�.
		}
		else if ( strJudge == PG_ROUTE ) // _T("ROUTE") // MES server�� model file�� ����� ��θ� ����
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_ROUTE Receive"));
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_Route;		// TxHost���� TMD_ROUTE�� �����Ͽ���.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;		// ACK�� ���������� ó���� ���ƴ�.
		}
		else if ( strJudge == PG_TMD_COPY_START )		// _T("TMD_COPY_START") // ���� ��ο� �ִ� Model file�� ��ȣ��PC C:\TMD ������ ����
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_TMD_COPY_START Receive"));
		}
		else if ( strJudge == PG_TMD_CHECKSUM_START ) // _T("TMD_CHECKSUM_START") // ��ȣ�� PC�� C:\TMD ������ ������ ������ Checksum���� �PC�� ����
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_TMD_CHECKSUM_START Receive"));
		}
		else if ( strJudge == PG_CLIENT_CHECKSUM_START )	// _T("CLIENT_CHECKSUM_START")	// ��ȣ���� checksum ���� Ȯ���Ͽ� MD5�� �ִ� Checksum ���� Ʋ���� Ȯ��
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_CLIENT_CHECKSUM_START Receive"));
		}
		else if ( strJudge == PG_TMD_DOWNLOAD_START ) // _T("TMD_DOWNLOAD_START") // �� ��ȣ��� download�� ������ �����Ѵ�.
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_TMD_DOWNLOAD_START Receive"));
		}
	}
	else if( strCommand == PG_DOWN )
	{
		//P0013DOWN,Ch,11,TMD_INFO8502
		if( strJudge == TMD_INFO )
			pCell->m_Zone_A.m_bReceive_TMD_Info = TMD_INFO_DOWN;
	}
	else if ( strCommand == PG_SYSTEM_ERROR ) // ��ȣ�� System Error �߻�
	{
	}
	else if( strCommand == TMD_INFO )
	{
		pCell->m_Zone_A.m_strTMD_Info = strJudge;
		//2018-05-25,MGYUN, TMD_INFO ���� ���� ���� TMD NAME ���̰� 40 �ʰ��̸� �˶� �߻�
		if(pCell->m_Zone_A.m_strTMD_Info.GetLength() > 40)
		{
			//���� ���� ������ ���̰� 40 ������ �˶� �߻�.
			theUnitFunc.SetError(ALARM_TMD_NAME_LENGTH_OVER);
		}
		pCell->m_Zone_A.m_bReceive_TMD_Info = TMD_INFO_GOOD;//TEST
		pCell->m_strPG_UI_Version = t.GetNextToken(_T(","));   //CLIENT VERSION
	   
		//2017-10-09,SKCHO
		pCell->m_Zone_A.m_strProgram1 = t.GetNextToken(_T(","));
		pCell->m_Zone_A.m_strProgram2 = t.GetNextToken(_T(","));
		pCell->m_Zone_A.m_strProgram3 = t.GetNextToken(_T(","));
		pCell->m_Zone_A.m_strProgram4 = t.GetNextToken(_T(","));
		pCell->m_Zone_A.m_strProgram5 = t.GetNextToken(_T(","));

		strTemp = t.GetNextToken(_T(","), 6);
		nEtx = strTemp.Find(ETX);
		if(nEtx > 5)
			strTemp = strTemp.Mid(0, nEtx-4);
		pCell->m_strPG_MAC_Address = strTemp;
		strLog.Format(_T("%s <==== %s Jig %d, Ch %d, TMD : %s, Client : %s, MAC Address : %s"), SOCKET_MAIN, SOCKET_PGPC, nJigNo+1, ch+1, pCell->m_Zone_A.m_strTMD_Info, pCell->m_strPG_UI_Version, pCell->m_strPG_MAC_Address);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
	}

	//2018-05-17,SKCHO,���� ��û����
	else if ( strCommand == REG_PIN )
	{
		if(PG_JUDGE_GOOD == strJudge)
		{
			pCell->m_Zone_A.RegPinName = strJudge;
			pCell->m_Zone_A.RegPinClass = GOOD_CELL;
		}
		else
		{
			pCell->m_Zone_A.RegPinClass = REJECT_CELL;
			pCell->m_Zone_A.RegPinName = TEXT_CONTACT_NG;
		}
		strLog.Format( _T("%s <==== %s %s, Jig:%d, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, nJigNo+1, ch+1, strJudge);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
	}
	else if ( strCommand == DIC_CHECK )
	{
		if(PG_JUDGE_GOOD == strJudge)
		{
			pCell->m_Zone_A.DicCheckName = strJudge;
			pCell->m_Zone_A.DicCheckClass = GOOD_CELL;
		}
		else
		{
			pCell->m_Zone_A.DicCheckClass = REJECT_CELL;
			pCell->m_Zone_A.DicCheckName = TEXT_CONTACT_NG;
		}
		strLog.Format( _T("%s <==== %s %s, Jig:%d, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, nJigNo+1, ch+1, strJudge);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
	}
	else if (CELL_LOADING == strCommand)
	{
		// Judge
		if(PG_JUDGE_GOOD == strJudge)
		{
			pCell->m_Zone_A.ResultCellLoadingName = strJudge;
			pCell->m_Zone_A.CellLoadingClass = GOOD_CELL;
			/*
			CString strTemp = t.GetNextToken(_T(","), 1);
			pCell->m_Zone_A.CellLoadingCurrent[0] = _tstof(strTemp.GetString());					
			for (int i=1 ; i<6 ; i++)
			{
				// Title�ϰ� ���� ������ 1ĭ �Ѿ
				strTemp = t.GetNextToken(_T(","), 2);
				nEtx = strTemp.Find(ETX);
				if(nEtx > 5)
					strTemp = strTemp.Mid(0, nEtx-4);

				pCell->m_Zone_A.CellLoadingCurrent[i] = _tstof(strTemp.GetString());		
			}
			*/
						// Data ���� (���� ��ǥ ����)

			// AZone Cell Loading Current �׸� �̻��ϰ� ��ϵǴ� ���� ���� 2017/06/16 HSK.
			t.GetNextToken(_T(","));

			CString strTemp;
			// ������ �� 6�� : IBAT, IVSS, IVDD, IVCI, IBAT2, IDD2
			for (int i=0 ; i<6 ; i++)
			{
				// Title�ϰ� ���� ������ 1ĭ �Ѿ
				strTemp = t.GetNextToken(_T(","), 2);
				nEtx = strTemp.Find(ETX);
				if(nEtx > 5)
					strTemp = strTemp.Mid(0, nEtx-4);

				pCell->m_Zone_A.CellLoadingCurrent[i] = _tstof(strTemp.GetString());		
			}

			pCell->m_Zone_A.m_strCellLoading_IBAT.Format(_T("%.2f"),pCell->m_Zone_A.CellLoadingCurrent[0]);
			pCell->m_Zone_A.m_strCellLoading_IVSS.Format(_T("%.2f"),pCell->m_Zone_A.CellLoadingCurrent[1]);
			pCell->m_Zone_A.m_strCellLoading_IVDD.Format(_T("%.2f"),pCell->m_Zone_A.CellLoadingCurrent[2]);
			pCell->m_Zone_A.m_strCellLoading_IVCI.Format(_T("%.2f"),pCell->m_Zone_A.CellLoadingCurrent[3]);
			pCell->m_Zone_A.m_strCellLoading_IBAT2.Format(_T("%.2f"),pCell->m_Zone_A.CellLoadingCurrent[4]);
			pCell->m_Zone_A.m_strCellLoading_IDD2.Format(_T("%.2f"),pCell->m_Zone_A.CellLoadingCurrent[5]);
		}
		else
		{
			pCell->m_Zone_A.CellLoadingClass = REJECT_CELL;
			pCell->m_Zone_A.ResultCellLoadingName = TEXT_CONTACT_NG;
		}

		strLog.Format( _T("%s <==== %s %s, Jig:%d, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, nJigNo+1, ch+1, strJudge);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
	}	
	else if(CONTACT_CURRENT == strCommand || _T("CONTACT_CHECK") == strCommand)
	{
		if(pCell->m_Zone_A.ContactCurrentClass == NONE_CELL
			  || pCell->m_Zone_A.ContactCurrentClass == REJECT_CELL)
		{
			pCell->m_Zone_A.ContactCurrentName = strJudge;
			if(strJudge == PG_JUDGE_GOOD)
			{
				pCell->m_Zone_A.ContactCurrentClass = GOOD_CELL;
			}
			else
			{
				pCell->m_Zone_A.ContactCurrentClass = REJECT_CELL;
			}
			

			// Data ���� (���� ��ǥ ����)
			t.GetNextToken(_T(","));

			CString strTemp;
			// ������ �� 6�� : IBAT, IVSS, IVDD, IVCI, IBAT2, IDD2
			for (int i=0 ; i<6 ; i++)
			{
				// Title�ϰ� ���� ������ 1ĭ �Ѿ
				strTemp = t.GetNextToken(_T(","), 2);
				nEtx = strTemp.Find(ETX);
				if(nEtx > 5)
					strTemp = strTemp.Mid(0, nEtx-4);

				pCell->m_Zone_A.ContactCurrent[i] = _tstof(strTemp.GetString());		
			}
			pCell->m_Zone_A.m_strContactCurrent_IBAT.Format(_T("%.2f"),pCell->m_Zone_A.ContactCurrent[0]);
			pCell->m_Zone_A.m_strContactCurrent_IVSS.Format(_T("%.2f"),pCell->m_Zone_A.ContactCurrent[1]);
			pCell->m_Zone_A.m_strContactCurrent_IVDD.Format(_T("%.2f"),pCell->m_Zone_A.ContactCurrent[2]);
			pCell->m_Zone_A.m_strContactCurrent_IVCI.Format(_T("%.2f"),pCell->m_Zone_A.ContactCurrent[3]);
			pCell->m_Zone_A.m_strContactCurrent_IBAT2.Format(_T("%.2f"),pCell->m_Zone_A.ContactCurrent[4]);
			pCell->m_Zone_A.m_strContactCurrent_IDD2.Format(_T("%.2f"),pCell->m_Zone_A.ContactCurrent[5]);
			
		}
		strLog.Format( _T("%s <==== %s %s, Jig:%d, Ch:%d, Judge:%s"), SOCKET_MAIN, SOCKET_PGPC, strCommand, nJigNo+1, ch+1, strJudge);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
	}
	else if( (TSP_START == strCommand) || (strCommand == TSP_START2) )
	{
	}
	else if(strJudge == ZONE_FIN)
	{
	}
	else if ( strCommand == TE_CHECK )		// TE_CHECK�� ���� ���� ���
	{
	}
	else if ( strCommand == CURRENT_ALRAM )			// Current ������ Alarm �߻�
	{
	}
	else if ( strCommand == VOLTAGE_ALRAM)			// ���� Alarm
	{
	}
	else if ( strCommand == ID_CHECK )
	{
	} 
	else if ( strCommand == OTP_REG_CHECK )
	{
	}
	//2017-02-26, jhLee, Job file download ���� ������ ó���Ѵ�.
	else if ( strCommand == PG_TMD_COPY_START )		// _T("TMD_COPY_START") // ���� ��ο� �ִ� Model file�� ��ȣ��PC C:\TMD ������ ����
	{
		if ( strJudge == PG_JUDGE_GOOD )					// ����� GOOD�� ���ŵǾ���.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_CopyStart;		// TxHost���� TMD_COPY_START�� �����Ͽ���.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;		// ���������� ó���� ���ƴ�.
		}
		else														// ����� NG�� ���ŵǾ���.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_CopyStart;		// TxHost���� TMD_COPY_START�� �����Ͽ���.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_NG;			// ������ �߻��Ͽ���.
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s"), strCommand, strJudge);
	} 
	else if ( strCommand == PG_TMD_CHECKSUM_START )	//	_T("TMD_CHECKSUM_START")		// ��ȣ�� PC�� C:\TMD ������ ������ ������ Checksum���� �PC�� ����
	{
		CString strCount = t.GetNextToken(_T(","));		// ������ ���� ���� ȹ���Ѵ�.
		CString strFile, strChecksum;
		int nCount = _tstoi( strCount.GetString() );		// ���� ������ ��ȯ

		for (int i = 0 ; i < nCount ; i++)					// ���� ������ŭ �ݺ��Ѵ�.
		{
			strFile		= t.GetNextToken(_T(","));			// ���� �̸�
			strChecksum	= t.GetNextToken(_T(","));			// �ش� ������ Checksum

			nEtx = strChecksum.Find(ETX);						// Checksum �����Ϳ� ETX�� �ٴ°��� �����Ѵ�.
			if( nEtx > 5 )
			{
				strChecksum = strChecksum.Left(nEtx);
			}

			// �����̸��� checksum ����
			theConfigBank.m_System.m_strJobFilename2[i]	= strFile;			// ��ȣ��κ��� ȸ�Ź��� ���� �̸�
			theConfigBank.m_System.m_strChecksum2[i]		= strChecksum;		// ��ȣ��κ��� ȸ�Ź��� Checksum ��

			theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : List [%d] %s, %s"), i, strFile, strChecksum);
		}//of for i


		if ( strJudge == PG_JUDGE_GOOD )					// ������ ������ Checksum ������ ����
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_ChecksumStart;		// ȸ�Ź��� ����
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;			// ���������� ó���� ���ƴ�.
		}//of if GOOD
		else
		{			// NG
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_ChecksumStart;		// ȸ�Ź��� ����
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_NG;			// ������ �߻��Ͽ���.
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s, Count:%d"), strCommand, strJudge, nCount);
	} 
	else if ( strCommand == PG_CLIENT_CHECKSUM_START)	//	_T("CLIENT_CHECKSUM_START")	// ��ȣ���� checksum ���� Ȯ���Ͽ� MD5�� �ִ� Checksum ���� Ʋ���� Ȯ��
	{
		if ( strJudge == PG_JUDGE_GOOD )					// ����� GOOD�� ���ŵǾ���.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_ClientChecksum;	// TxHost���� CLIENT_CHECKSUM_START�� �����Ͽ���.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;			// ���������� ó���� ���ƴ�.
		}
		else														// ����� NG�� ���ŵǾ���.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_ClientChecksum;	// ȸ�Ź��� ����
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_NG;			// ������ �߻��Ͽ���.
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s"), strCommand, strJudge);
	} 
	else if ( strCommand == PG_TMD_DOWNLOAD_START )	// _T("TMD_DOWNLOAD_START")		// �� ��ȣ��� download�� ������ �����Ѵ�.
	{
		if ( strJudge == PG_JUDGE_GOOD )					// ����� GOOD�� ���ŵǾ���.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_DownloadStart;		// TxHost���� TMD_DOWNLOAD_START�� �����Ͽ���.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;			// ���������� ó���� ���ƴ�.
		}
		else														// ����� NG�� ���ŵǾ���.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_DownloadStart;		// ȸ�Ź��� ����
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_NG;			// ������ �߻��Ͽ���.
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s"), strCommand, strJudge);
	} 
	else if ( strCommand == PG_TMD_MD5 )				// _T("TMD_MD5")						// �� ��ȣ�⿡�� ���Ǵ� TMD�� Checksum ���� ���۹޴´�.
	{
		CString strFile		= t.GetNextToken(_T(","));			// ���� �̸�
		CString strChecksum	= t.GetNextToken(_T(","));			// �ش� ������ Checksum

		nEtx = strChecksum.Find(ETX);						// Checksum �����Ϳ� ETX�� �ٴ°��� �����Ѵ�.
		if( nEtx > 5 )
		{
			strChecksum = strChecksum.Mid(0, nEtx-4);
		}

		if ( (nIdx >= 0) && (nIdx < theConfigBank.m_System.eMAX_PGCount) )
		{
			theConfigBank.m_System.m_strCurrentTMD[nIdx]			= strFile;			// ���� �� ��ȣ���� �������� ���� TMD file �̸�
			theConfigBank.m_System.m_strCurrentChecksum[nIdx]	= strChecksum;		// �� ��ȣ�� TMD ������ Checksum ����
			theConfigBank.m_System.m_strCurrentMD5[nIdx]			= strJudge;			// check ���
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s"), strCommand, strJudge);
	} 
	else
	{
		theLog[LOG_SOCKET_PG].AddBuf(_T("Unknown Command [%s]"), strCommand);
	}
	//kjaprk 20161114 ��Ŷ ������ ���Ž� �߶󳻰� �ٽ� �Ľ�
	return PacketClear(SOCKET_PGPC, strMsg);
}

void CSocketDialog::PatternGeneratorACK(CCellInfo* pCell, CString strJudge)
{
		if ( strJudge == SET_ZONE_A )
		{
		} 
		else if ( strJudge == SET_ZONE_B )
		{
		} 
		else if ( strJudge == SET_ZONE_C )
		{
		} 
		else if ( strJudge == SET_ZONE_D )
		{
		}
		else if( (strJudge == TE_CHECK) )	
		{
		}
		// TSP_START or TSP_START2�� ACK ����
		else if( (strJudge == TSP_START) || (strJudge == TSP_START2) )
		{
		}		
		else if(strJudge == MTP_WRITE || strJudge == MTP_VERIFY)
		{
		}
		theLog[LOG_OP2PG_PACKET].AddBuf(_T("<- %s, Jig:%d Ch:%d %s Ack Recived"),SOCKET_PGPC, pCell->m_JigId+1, pCell->m_JigCh+1, strJudge);
}


//2017-02-26, jhLee, Job file download���� ��� �߰�

// Job file�� ���� MES server�� ��θ� �����Ѵ�.
BOOL CSocketDialog::PG_TMD_SendRoute(CString sData)
{
	CString sFullPath;

	sFullPath = _T("Z:") + sData;
	return SendMassageToPatternGenerator(PG_ROUTE, 0, 0, sFullPath);
}

// download�� ���� ����Ʈ�� �����Ѵ�.
BOOL CSocketDialog::PG_TMD_SendFileList(CString sData)
{
	return SendMassageToPatternGenerator(PG_TMD, 0, 0, sData);
}


// MES Server�κ��� ���縦 �����Ѵ�.	
BOOL CSocketDialog::PG_TMD_SendCopyStart()
{
	return SendMassageToPatternGenerator(PG_TMD_COPY_START, 0, 0);
}

// Download�� ���ϵ��� Checksum �˻��Ѵ�.
BOOL CSocketDialog::PG_TMD_SendChecksumStart()
{
	return SendMassageToPatternGenerator(PG_TMD_CHECKSUM_START, 0, 0);
}

// Download�� ���ϰ� �� ��ȣ���� ���� ���ϰ��� Checksum �񱳸� �Ѵ�.
BOOL CSocketDialog::PG_TMD_SendClientChecksum()
{
	return SendMassageToPatternGenerator(PG_CLIENT_CHECKSUM_START, 0, 0);
}


// Download�� ���ϵ��� �� ��ȣ�⿡�� �����Ѵ�.
BOOL CSocketDialog::PG_TMD_SendDownloadStart()
{
	return SendMassageToPatternGenerator(PG_TMD_DOWNLOAD_START, 0, 0);
}

// ��� ������ ���� TMD_INFO ����� �����Ѵ�.
BOOL CSocketDialog::PG_TMD_SendInfo(int nJig, int nCh)
{
	return SendMassageToPatternGenerator(TMD_INFO, nJig, nCh);
}
