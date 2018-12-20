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
	//kjpark 20161029 PG send Packet 버그 수정
	CClientSocket* pSocket = m_pSocket_PatternGenerator.GetClientAt(SOCKET_PGPC);
	//2018-07-23,NAM,Testing 
	CCellTag	tag1;
	CCellInfo* pCellInfo;
	pCellInfo = NULL;

	if ( pSocket == NULL )		// 연결 실패
	{
		//kjpark 20161021 Setup 신호기 UI 구현
		strLog.Format(_T("%s -> %s\tSEND FAIL(Not Connect) CMD : %s"), SOCKET_MAIN, SOCKET_PGPC, strCmdType);
		SetAddLog(strLog);

		return FALSE;
	}

	//if ( m_bSystemError[jig][ch] )			// 전송하고자 하는 신호기에 이상이 있다면 RESET 혹은 A-Zone 전환일 경우에만 전송한다.
	//{
	//	if( (strCmdType == SET_ZONE_A) || (strCmdType == RESET) ) //JH-PARK 2014.1.20
	//	{
	//		m_bSystemError[jig][ch] = FALSE;						// 이상 상황 미리 해제
	//	}
	//	else
	//	{
	//		m_pLogPG->Log(_T("%s -> %s\tSEND FAIL(SYSTEM_ERROR) CMD : %s"), SOCKET_MAIN, SOCKET_PGPC, strCmdType);

	//		return FALSE;
	//	}
	//}

	enum { eMAX_LEN = 2048, };			// 버퍼 최대 길이
	BYTE ucData[eMAX_LEN];

	CString strBuff		= _T("");
	CString strCount	= _T("");
	CString strTemp		= _T("");
	int nCount = 0;

	memset(ucData,	NULL,	sizeof(ucData));

	ucData[0] = STX;

	//Dest Code
	ucData[1] = CODEPGPC;

	// Message, 별도의 문자열 데이터가 필요로하는 명령이라면,
	if (	(strCmdType == MTP_VERIFY) || 
			(strCmdType == MTP_WRITE) || 
			//2018-07-24,NAM,Remove this case
			(strCmdType == CELL_LOADING) || 
			(strCmdType == PG_CIRCLE) || 
			(strCmdType == PG_MSG)  ||
			(strCmdType == PG_ROUTE) ||				//2017-02-25, jhLee, Job file download관련, MES Server의 경로를 지정한다. 채널번호는 0을 지정한다.
			(strCmdType == PG_TMD) ||					//							Download할 파일이름과 Checksum 지정
			(strCmdType == PATTERN_SELECT) )			// 2015-06-02, 특정 패턴 선택 지정 명령 추가
	{
		//2018-07-14,GHLEE, 심영욱 책임님 요청사항으로 TMD에 보내는 CELL_LOADING 메세지 구조에서 CELLID_INNERID_PRODUCTID 추가
		pCellInfo=theCellBank.SearchCellInfo(jig,ch,CELL_POS_AZONE_CH1,CELL_POS_DZONE_CH2);
		strBuff.Format(_T("Ch,%d,%s%s,%s_%s_%s"), ((jig * eMAX_ChCount ) + ch + 1), strCmdType, strExtraData,pCellInfo->m_strCellID,pCellInfo->m_strInnerID,pCellInfo->m_strProductID);
		//strBuff.Format(_T("Ch,%d,%s,%s"), ((jig * eMAX_ChCount ) + ch + 1), strCmdType, strExtraData);
	}
	//2018-07-24,NAM, Add the Cell Information if strCmdType is CELL_LOADING
	/*else if (strCmdType == CELL_LOADING)
	{
		//2018-07-14,GHLEE, 심영욱 책임님 요청사항으로 TMD에 보내는 CELL_LOADING 메세지 구조에서 CELLID_INNERID_PRODUCTID 추가
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
		strBuff.Format(_T("Ch,%d,%s"), ((jig * eMAX_ChCount ) + ch + 1), strCmdType);		// 그 외는 채널 일련번호를 넘겨준다.

	// Count
	nCount = strBuff.GetLength();
	strCount.Format(_T("%04x"),nCount);

	// Merge
	strTemp.Format(_T("%s%s"), strCount, strBuff);
	nCount = strTemp.GetLength();

	CEtc::ConvertMultibyte((wchar_t *)strTemp.GetString(), (char *)&ucData[2], eMAX_LEN-2);			// WCHAR CString을 Char 형으로 변환시켜준다.
	USHORT checksum = GetCRC16_Checksum((UCHAR *)&ucData[1], nCount+1);							// CRC16을 만들어준다.

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
	//kjpark 20161029 PG send Packet 버그 수정
	if (m_pSocket_PatternGenerator.SendClient(pSocket->GetSocketName(), ucData, nCount + 7))
	{
		strLog.Format(_T("%s -> %s\t%d\t%s"), SOCKET_MAIN, SOCKET_PGPC, nCount + 7, strTemp);
		SetAddLog(strLog);
		return TRUE;
	}
#endif

	//// 전송 실패	
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
		

		//2014-09-01, jhLee, Line Check일 경우 Alive 상태를 확인하는 Heart beat 성격의 메세지이므로
		// Log를 남기지 않도록 한다.
		// TMDCHECK명령에 CHECK가 들어가 있어서 해당 부분 주석 한다 2017/06/16 HSK.
/*		if (strMsg.Find( PG_CHECK ) >= 0)		// "LineCheck" 글귀가 들어가지 않은 경우에만 Log를 남긴다.
		{			
			return;
		}
		else*/ if (strMsg.Find( PG_LINECHECK ) >= 0)		// "LineCheck" 글귀가 들어가지 않은 경우에만 Log를 남긴다.
		{			
			return;
		}
		// TMDCHECK명령에 CHECK가 들어가 있어서 해당 부분 주석 한다 2017/06/16 HSK.
		else if (strMsg.Find( PG_LINECHECK ) == -1/* && strMsg.Find( PG_CHECK ) == -1*/)		// "LineCheck" 글귀가 들어가지 않은 경우에만 Log를 남긴다.
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
	// 신호기에서 수신된 내용을 처리한다.
	int nIndex = strMsg.Find(STX);			// STX(시작위치)를 찾는다.
	CString strLog;

	if ( nIndex > 0)	// 0위치 이후에 STX가 발견되었다면,
	{
		strLog.Format(_T("Before STX Packet[%d] : %s"), nIndex, strMsg.Mid(0, nIndex));
		theLog[LOG_SOCKET_PG].AddBuf(strLog);		
		strMsg.Delete(0, nIndex);		// 이전 데이터는 삭제하여 무시한다.
	}
	
	CCellTag	tag;
	CCellInfo* pCell;
	CCellInfo  rTempCell;

	CString strCommand = _T("");
	CString strJudge = _T("");

	int nJigNo;
	int ch;
	int nIdx;						// Cell 정보의 일련번호

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
	nIdx		= _tstoi(strTemp.GetString()) - 1;		// 일련번호로 들어온다. 1 ~ 4 -> 0 ~ 3으로 계산
	nJigNo		= (int)( nIdx / JIG_CH_MAX );				// Jig 번호 계산, 0 ~ 1
	ch			= (int)( nIdx % JIG_CH_MAX );				// 채널 번호 계산, 0 ~ 1
	
	// Command
	if( (strCommand != PG_ACK) && (strCommand != PG_DOWN) && (strCommand != PG_RUN) )
	{
		strCommand = t.GetNextToken(_T(","));
		nEtx = strCommand.Find(ETX);

		if (nEtx > 5)	strCommand = strCommand.Mid(0, nEtx-4);

		// 쓸데없는건 Parsing하지 말자..
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

	//2017-03-01, jhLee, Job file download 관련 추가
	if ( nIdx < 0 ) // == 0 && nJigNo==0 && ch==0 )
	{
		pCell = &rTempCell;
	}
	else
	{
		tag = theCellBank.SearchCellTag((JIG_ID)nJigNo, (JIG_CH)ch);

		if ( (tag.IsExist() == FALSE) && (nIdx >= 0) )		// 대상 Cell 정보를 찾지 못하면 더이상의 처리는 무의미하다 !
		{		
			strLog.Format(_T("%s <==== %s ** Cell Not Found Jig:%d Ch:%d Msg:%s"), SOCKET_MAIN, SOCKET_PGPC, nJigNo+1, ch+1, strMsg);
			theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
			// 두개 붙어서 나올 것을 대비해 다음 STX까지 지운다
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

	// 아래 명령을 제외한 처리
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

    //2017-03-01, jhLee, Job file download 관련에서 삭제
	//// 지정 명령에대한 수신 결과로 선두가 ACK로 날라오는 경우
	//if ( strCommand == PG_ACK )	// ACK로 날라오는거는 필요한 것만 Cell Search한다 [6/20/2013 OSC]
	//{
	//	PatternGeneratorACK(pCell, strJudge);
	//}
	//else 
	if( strCommand == PG_RUN )	// 지정 명령에 대한 수행 결과가 수신되는 경우
	{
		//BOOL		RecvTurnOn;
		strLog.Format(_T("%s <==== %s %s, Jig:%d Ch:%d Run Recived"),SOCKET_MAIN, SOCKET_PGPC, strCommand, pCell->m_JigId+1, pCell->m_JigCh+1);
		theLog[LOG_OP2PG_PACKET].AddBuf(strLog);
		if(strJudge == TURN_OFF)
		{
			pCell->m_Zone_D.TurnOffChk = TRUE;
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("Turn Off Receive"));
		}
		//2017-02-26, jhLee, Job file download 관련 응답을 처리한다. "RUN"이 도착하였다.
		else if ( strJudge == PG_TMD )	// _T("TMD") // MES Server의 Model file이 저장된 다운로드 할 TMD 파일과 DLL 명과 각 파일의 Checksum 전송
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_TMD Receive"));
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_TMDSend;		// TxHost에게 TMD_SEND를 전송하였다.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;		// ACKFH 정상적으로 처리를 마쳤다.
		}
		else if ( strJudge == PG_ROUTE ) // _T("ROUTE") // MES server의 model file이 저장된 경로명 전송
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_ROUTE Receive"));
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_Route;		// TxHost에게 TMD_ROUTE를 전송하였다.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;		// ACK로 정상적으로 처리를 마쳤다.
		}
		else if ( strJudge == PG_TMD_COPY_START )		// _T("TMD_COPY_START") // 지정 경로에 있는 Model file을 신호기PC C:\TMD 폴더로 복사
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_TMD_COPY_START Receive"));
		}
		else if ( strJudge == PG_TMD_CHECKSUM_START ) // _T("TMD_CHECKSUM_START") // 신호기 PC에 C:\TMD 폴더로 복사한 파일의 Checksum값을 운영PC로 전송
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_TMD_CHECKSUM_START Receive"));
		}
		else if ( strJudge == PG_CLIENT_CHECKSUM_START )	// _T("CLIENT_CHECKSUM_START")	// 신호기의 checksum 값을 확인하여 MD5에 있는 Checksum 값이 틀린지 확인
		{
			theLog[LOG_OP2PG_PACKET].AddBuf(_T("PG_CLIENT_CHECKSUM_START Receive"));
		}
		else if ( strJudge == PG_TMD_DOWNLOAD_START ) // _T("TMD_DOWNLOAD_START") // 각 신호기로 download한 파일을 전송한다.
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
	else if ( strCommand == PG_SYSTEM_ERROR ) // 신호기 System Error 발생
	{
	}
	else if( strCommand == TMD_INFO )
	{
		pCell->m_Zone_A.m_strTMD_Info = strJudge;
		//2018-05-25,MGYUN, TMD_INFO 날려 응답 받은 TMD NAME 길이가 40 초과이면 알람 발생
		if(pCell->m_Zone_A.m_strTMD_Info.GetLength() > 40)
		{
			//수신 받은 데이터 길이가 40 넘으면 알람 발생.
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

	//2018-05-17,SKCHO,고객사 요청사항
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
				// Title하고 같이 있으니 1칸 넘어감
				strTemp = t.GetNextToken(_T(","), 2);
				nEtx = strTemp.Find(ETX);
				if(nEtx > 5)
					strTemp = strTemp.Mid(0, nEtx-4);

				pCell->m_Zone_A.CellLoadingCurrent[i] = _tstof(strTemp.GetString());		
			}
			*/
						// Data 개수 (다음 쉼표 갯수)

			// AZone Cell Loading Current 항목에 이상하게 기록되는 버그 수정 2017/06/16 HSK.
			t.GetNextToken(_T(","));

			CString strTemp;
			// 전류값 총 6개 : IBAT, IVSS, IVDD, IVCI, IBAT2, IDD2
			for (int i=0 ; i<6 ; i++)
			{
				// Title하고 같이 있으니 1칸 넘어감
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
			

			// Data 개수 (다음 쉼표 갯수)
			t.GetNextToken(_T(","));

			CString strTemp;
			// 전류값 총 6개 : IBAT, IVSS, IVDD, IVCI, IBAT2, IDD2
			for (int i=0 ; i<6 ; i++)
			{
				// Title하고 같이 있으니 1칸 넘어감
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
	else if ( strCommand == TE_CHECK )		// TE_CHECK에 대한 판정 결과
	{
	}
	else if ( strCommand == CURRENT_ALRAM )			// Current 측정시 Alarm 발생
	{
	}
	else if ( strCommand == VOLTAGE_ALRAM)			// 전압 Alarm
	{
	}
	else if ( strCommand == ID_CHECK )
	{
	} 
	else if ( strCommand == OTP_REG_CHECK )
	{
	}
	//2017-02-26, jhLee, Job file download 관련 응답을 처리한다.
	else if ( strCommand == PG_TMD_COPY_START )		// _T("TMD_COPY_START") // 지정 경로에 있는 Model file을 신호기PC C:\TMD 폴더로 복사
	{
		if ( strJudge == PG_JUDGE_GOOD )					// 결과로 GOOD이 수신되었다.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_CopyStart;		// TxHost에게 TMD_COPY_START를 전송하였다.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;		// 정상적으로 처리를 마쳤다.
		}
		else														// 결과로 NG가 수신되었다.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_CopyStart;		// TxHost에게 TMD_COPY_START를 전송하였다.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_NG;			// 오류가 발생하였다.
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s"), strCommand, strJudge);
	} 
	else if ( strCommand == PG_TMD_CHECKSUM_START )	//	_T("TMD_CHECKSUM_START")		// 신호기 PC에 C:\TMD 폴더로 복사한 파일의 Checksum값을 운영PC로 전송
	{
		CString strCount = t.GetNextToken(_T(","));		// 수행결과 파일 수를 획득한다.
		CString strFile, strChecksum;
		int nCount = _tstoi( strCount.GetString() );		// 파일 수량을 변환

		for (int i = 0 ; i < nCount ; i++)					// 파일 수량만큼 반복한다.
		{
			strFile		= t.GetNextToken(_T(","));			// 파일 이름
			strChecksum	= t.GetNextToken(_T(","));			// 해당 파일의 Checksum

			nEtx = strChecksum.Find(ETX);						// Checksum 데이터에 ETX가 붙는것을 방지한다.
			if( nEtx > 5 )
			{
				strChecksum = strChecksum.Left(nEtx);
			}

			// 파일이름과 checksum 저장
			theConfigBank.m_System.m_strJobFilename2[i]	= strFile;			// 신호기로부터 회신받은 파일 이름
			theConfigBank.m_System.m_strChecksum2[i]		= strChecksum;		// 신호기로부터 회신받은 Checksum 값

			theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : List [%d] %s, %s"), i, strFile, strChecksum);
		}//of for i


		if ( strJudge == PG_JUDGE_GOOD )					// 복사한 파일의 Checksum 수행결과 정상
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_ChecksumStart;		// 회신받은 과정
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;			// 정상적으로 처리를 마쳤다.
		}//of if GOOD
		else
		{			// NG
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_ChecksumStart;		// 회신받은 과정
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_NG;			// 오류가 발생하였다.
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s, Count:%d"), strCommand, strJudge, nCount);
	} 
	else if ( strCommand == PG_CLIENT_CHECKSUM_START)	//	_T("CLIENT_CHECKSUM_START")	// 신호기의 checksum 값을 확인하여 MD5에 있는 Checksum 값이 틀린지 확인
	{
		if ( strJudge == PG_JUDGE_GOOD )					// 결과로 GOOD이 수신되었다.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_ClientChecksum;	// TxHost에게 CLIENT_CHECKSUM_START를 전송하였다.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;			// 정상적으로 처리를 마쳤다.
		}
		else														// 결과로 NG가 수신되었다.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_ClientChecksum;	// 회신받은 과정
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_NG;			// 오류가 발생하였다.
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s"), strCommand, strJudge);
	} 
	else if ( strCommand == PG_TMD_DOWNLOAD_START )	// _T("TMD_DOWNLOAD_START")		// 각 신호기로 download한 파일을 전송한다.
	{
		if ( strJudge == PG_JUDGE_GOOD )					// 결과로 GOOD이 수신되었다.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_DownloadStart;		// TxHost에게 TMD_DOWNLOAD_START를 전송하였다.
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_GOOD;			// 정상적으로 처리를 마쳤다.
		}
		else														// 결과로 NG가 수신되었다.
		{
			theConfigBank.m_System.m_nJobProcessRcv	= theConfigBank.m_System.eJOB_DownloadStart;		// 회신받은 과정
			theConfigBank.m_System.m_nJobProcessState = theConfigBank.m_System.eSTATE_JOB_NG;			// 오류가 발생하였다.
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s"), strCommand, strJudge);
	} 
	else if ( strCommand == PG_TMD_MD5 )				// _T("TMD_MD5")						// 각 신호기에서 사용되는 TMD의 Checksum 값을 전송받는다.
	{
		CString strFile		= t.GetNextToken(_T(","));			// 파일 이름
		CString strChecksum	= t.GetNextToken(_T(","));			// 해당 파일의 Checksum

		nEtx = strChecksum.Find(ETX);						// Checksum 데이터에 ETX가 붙는것을 방지한다.
		if( nEtx > 5 )
		{
			strChecksum = strChecksum.Mid(0, nEtx-4);
		}

		if ( (nIdx >= 0) && (nIdx < theConfigBank.m_System.eMAX_PGCount) )
		{
			theConfigBank.m_System.m_strCurrentTMD[nIdx]			= strFile;			// 현재 각 신호기의 적용중인 현재 TMD file 이름
			theConfigBank.m_System.m_strCurrentChecksum[nIdx]	= strChecksum;		// 각 신호기 TMD 파일의 Checksum 정보
			theConfigBank.m_System.m_strCurrentMD5[nIdx]			= strJudge;			// check 결과
		}

		theLog[LOG_SOCKET_PG].AddBuf(_T("Job-File : %s, %s"), strCommand, strJudge);
	} 
	else
	{
		theLog[LOG_SOCKET_PG].AddBuf(_T("Unknown Command [%s]"), strCommand);
	}
	//kjaprk 20161114 패킷 여러게 수신시 잘라내고 다시 파싱
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
		// TSP_START or TSP_START2의 ACK 응답
		else if( (strJudge == TSP_START) || (strJudge == TSP_START2) )
		{
		}		
		else if(strJudge == MTP_WRITE || strJudge == MTP_VERIFY)
		{
		}
		theLog[LOG_OP2PG_PACKET].AddBuf(_T("<- %s, Jig:%d Ch:%d %s Ack Recived"),SOCKET_PGPC, pCell->m_JigId+1, pCell->m_JigCh+1, strJudge);
}


//2017-02-26, jhLee, Job file download관련 명령 추가

// Job file을 받을 MES server의 경로를 지정한다.
BOOL CSocketDialog::PG_TMD_SendRoute(CString sData)
{
	CString sFullPath;

	sFullPath = _T("Z:") + sData;
	return SendMassageToPatternGenerator(PG_ROUTE, 0, 0, sFullPath);
}

// download할 파일 리스트를 전송한다.
BOOL CSocketDialog::PG_TMD_SendFileList(CString sData)
{
	return SendMassageToPatternGenerator(PG_TMD, 0, 0, sData);
}


// MES Server로부터 복사를 지령한다.	
BOOL CSocketDialog::PG_TMD_SendCopyStart()
{
	return SendMassageToPatternGenerator(PG_TMD_COPY_START, 0, 0);
}

// Download한 파일들의 Checksum 검사한다.
BOOL CSocketDialog::PG_TMD_SendChecksumStart()
{
	return SendMassageToPatternGenerator(PG_TMD_CHECKSUM_START, 0, 0);
}

// Download한 파일과 각 신호기의 현재 파일과의 Checksum 비교를 한다.
BOOL CSocketDialog::PG_TMD_SendClientChecksum()
{
	return SendMassageToPatternGenerator(PG_CLIENT_CHECKSUM_START, 0, 0);
}


// Download한 파일들을 각 신호기에게 전송한다.
BOOL CSocketDialog::PG_TMD_SendDownloadStart()
{
	return SendMassageToPatternGenerator(PG_TMD_DOWNLOAD_START, 0, 0);
}

// 상시 점검을 위한 TMD_INFO 명령을 전송한다.
BOOL CSocketDialog::PG_TMD_SendInfo(int nJig, int nCh)
{
	return SendMassageToPatternGenerator(TMD_INFO, nJig, nCh);
}
