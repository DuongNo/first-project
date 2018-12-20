#pragma once
#include "stdafx.h"
#include "SocketDialog.h"
#include "CheetahApp.h"
#include "File\GxIniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CSocketDialog::SetDataPCProcessing( CDataSocket* pSocket /*= NULL*/ )
{
	if(pSocket == NULL)	return;

	// 1�� �̻� �ް� �ִ°� ������ ������ �޾Ƴ����� ����������. [11/15/2016 OSC]
	if( (m_timerDataPCMsg.Stop(FALSE) > 1.) && (m_QueueDataPC.GetSize() > 0) )
	{
		m_QueueDataPC.ClearQueue();
	}
	m_QueueDataPC.Put((char*)pSocket->GetLastMsg(), pSocket->GetLastLength());
	m_timerDataPCMsg.Start();

	while(m_QueueDataPC.GetSize() >= sizeof(tag_Message))
	{
		ParseDataPCMessage();
	}
}

void CSocketDialog::ParseDataPCMessage()
{
	int nIndex = 0, nIndex1 = 0;
	CString strTemp;
	SYSTEMTIME AlarmTime;
	char msgbuf[sizeof(tag_Message)];
	tag_Message msg;
	m_QueueDataPC.Get(msgbuf, sizeof(tag_Message));
	memcpy(&msg, msgbuf, sizeof(tag_Message));
	tag_Message_Unicode msgU;
	msgU = msg;		
	int nLength = 0;

	CCellInfo *pCell = NULL;
	switch(msg.sMsgType)
	{
	case HOST_SET_CONTROL:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CONTROL] \t %s, %s"), msgU.strMsgData[9], msgU.strMsgData[10]);
		if(msgU.strMsgData[9] == CRST_REMOTE)
		{
			theProcBank.ChangeCimState(CONST_CIM_STATE::CIM_REMOTE);			
		}
		else if(msgU.strMsgData[9] == CRST_LOCAL)
		{
			theProcBank.ChangeCimState(CONST_CIM_STATE::CIM_LOCAL);
		}
		else
		{
			theProcBank.ChangeCimState(CONST_CIM_STATE::CIM_OFFLINE);
		}
		break;

	case HOST_SET_DATAPC_VERSION:
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_DATAPC_VERSION] \t %s"), msgU.strMsgData[9]);
		theProcBank.m_strDataPCVersion.Format(_T("%s"), msgU.strMsgData[9]);
		break;

	case HOST_SET_CELL_INFORMATION:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_INFORMATION] \t Jig %c, Ch %d, InnerID %s, CellID %s, ProductID %s, CellInfoResult %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, msgU.strMsgData[0], msgU.strCellID, msgU.strMsgData[2], msgU.strMsgData[3]);

		//2018-07-25,SKCHO, LDTR�� ������ ���� CELL_POS_LD_TR_CH1
		//pCell = theCellBank.SearchCellInfo((JIG_ID)msgU.sUnit, (JIG_CH)msgU.sCellPos, CELL_POS_AZONE_CH1, CELL_POS_DZONE_CH2);
		pCell = theCellBank.SearchCellInfo((JIG_ID)msgU.sUnit, (JIG_CH)msgU.sCellPos, CELL_POS_LD_TR_CH1, CELL_POS_DZONE_CH2);  
		if(pCell)
		{
			pCell->m_strProductID.Format(_T("%s"), msgU.strMsgData[2]);
			pCell->m_strCellInfoResult.Format(_T("%s"), msgU.strMsgData[3]);
		}
		break;

	case HOST_SET_OPCALL:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_OPCALL] \t OPCallID %s, Message %s%s%s%s"), 
			msgU.strMsgData[7], msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

		theProcBank.m_strOPCallID = msgU.strMsgData[7];

		if(theProcBank.m_MsgOpCallCnt > OPCALLMAX) break;

		theProcBank.m_strOPCallMsg[theProcBank.m_MsgOpCallCnt].Format(_T("%s%s%s%s\n\t"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);
		
				//2017-04-15,skcho , opcall ���� �ð� ǥ��
		GetLocalTime(&AlarmTime);
		theProcBank.alarm_info_op[theProcBank.m_MsgOpCallCnt].szERR_ID = theProcBank.m_strOPCallID;
		theProcBank.alarm_info_op[theProcBank.m_MsgOpCallCnt].TimeOccur = CTime(AlarmTime);
		theProcBank.alarm_info_op[theProcBank.m_MsgOpCallCnt].szLevel = CStringSupport::N2C(ALARM_LEVEL_ALARM);	
		
		theProcBank.m_MsgOpCallCnt++;

		if(theProcBank.m_strOldOPCallID == (_T("")))
		{
			theProcBank.m_strOldOPCallID = theProcBank.m_strOPCallID; 
			theProcBank.SetOPCall();
		}
		break;

	case HOST_SET_INTERLOCK:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_INTERLOCK] \t InterlockID %s, Message %s%s%s%s"), 
			msgU.strMsgData[7], msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

		theProcBank.m_strInterlockID = msgU.strMsgData[7];

		if(theProcBank.m_MsgInterlockCnt > INTERLOCKMAX) break;

		theProcBank.m_strInterlockMsg[theProcBank.m_MsgInterlockCnt].Format(_T("%s%s%s%s\n\t"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);
			
		//2017-07-20,SKCHO, OLB INTERLOCK SKIP
		if(theProcBank.m_bOLBInterLockSkip)
		{
			nIndex = theProcBank.m_strInterlockMsg[theProcBank.m_MsgInterlockCnt].Find(_T("MD150"));
			nIndex1 = theProcBank.m_strInterlockMsg[theProcBank.m_MsgInterlockCnt].Find(_T("MDR02"));
			if(nIndex1 > 0 || nIndex > 0 )
			{
				return;
			}
		}
		//2017-04-15,skcho , opcall ���� �ð� ǥ��
		GetLocalTime(&AlarmTime);
		theProcBank.alarm_info_inter[ theProcBank.m_MsgInterlockCnt].szERR_ID = theProcBank.m_strInterlockID;	
		theProcBank.alarm_info_inter[ theProcBank.m_MsgInterlockCnt].TimeOccur = CTime(AlarmTime);
		theProcBank.alarm_info_inter[ theProcBank.m_MsgInterlockCnt].szLevel = CStringSupport::N2C(ALARM_LEVEL_ALARM);
		
		//2018-03-15,MGYUN,���Ͷ� �Ἵ ��� �߰� ī��Ʈ ���� ���� ��� �Ѵ�.
		theMonitorBank.WriteInterLock(theProcBank.m_MsgInterlockCnt,theProcBank.m_strInterlockID);
		//2018-03-15,MGYUN,���Ͷ� �߻� �� 1�� ����
		theMonitorBank.WriteInterLockStatus(1);
		
		theProcBank.m_MsgInterlockCnt++;

		if(theProcBank.m_strOldInterlockID == (_T("")))
		{
			theProcBank.m_strOldInterlockID = theProcBank.m_strInterlockID; 
			theProcBank.SetInterlock();
		}
		break;
		//2017-11-01,SKCHO, ���Ͷ� ���� �߰�
	case HOST_SET_UNIT_INTERLOCK:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_INTERLOCK] \t InterlockID %s, Message %s%s%s%s"), 
		msgU.strMsgData[7], msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

		theProcBank.m_strUnitModuleID = msgU.strMsgData[6];
		//2017-12-05,SKCHO, AMT�� LOADING STOP ���ٰ� �Ͽ� ����
		//2017-11-01,SKCHO, MODULEID = NULL ,'-'�̸� LOADING STOP�� 101 ����
		/*if(theProcBank.m_strUnitModuleID == _T("") || theProcBank.m_strUnitModuleID == _T("-"))
		{
			theProcBank.m_bUnitInterlock_LoadingStop = TRUE;  //loading stop �մܼ����� cell�� ���� ����
		}
		else
		{
			CString strBuf;		
			int nLength = theProcBank.m_strUnitModuleID.GetLength();
			strBuf = theProcBank.m_strUnitModuleID.Mid(nLength-2,nLength);			
			nIndex= _wtoi(strBuf);
			theProcBank.ChangeToJigNum(nIndex);		
			theProcBank.m_bUnitInterlock_LoadingStop = FALSE;
			
		}
		*/
						
		nLength = theProcBank.m_strUnitModuleID.GetLength();
		strTemp = theProcBank.m_strUnitModuleID.Mid(nLength-2,nLength);			
		nIndex= _wtoi(strTemp);
		theProcBank.ChangeToJigNum(nIndex);		
			
		theProcBank.m_strUnitInterlockID = msgU.strMsgData[7];

		if(theProcBank.m_MsgUnitInterlockCnt > INTERLOCKMAX) break;

		theProcBank.m_strUnitInterlockMsg[theProcBank.m_MsgUnitInterlockCnt].Format(_T("%s%s%s%s\n\t"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);
	
		//2017-04-15,skcho , opcall ���� �ð� ǥ��
		GetLocalTime(&AlarmTime);
		theProcBank.alarm_info_inter[ theProcBank.m_MsgUnitInterlockCnt].szERR_ID = theProcBank.m_strInterlockID;	
		theProcBank.alarm_info_inter[ theProcBank.m_MsgUnitInterlockCnt].TimeOccur = CTime(AlarmTime);
		theProcBank.alarm_info_inter[ theProcBank.m_MsgUnitInterlockCnt].szLevel = CStringSupport::N2C(ALARM_LEVEL_ALARM);

		theProcBank.m_MsgUnitInterlockCnt++;

		//�ӽ÷� ���⿡�� ���� ����
		//theSocketInterFace.SendCmdToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,_T("E_EQST_MATCH_INTERLOCK"));  //���� �˻� unit ���� ��ȯ

		if(theProcBank.m_strOldUnitInterlockID == (_T("")))
		{
			theProcBank.m_strOldUnitInterlockID = theProcBank.m_strUnitInterlockID; 
			theProcBank.SetInterlock2();
		}
		break;
	case HOST_SET_TERMINAL:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_TERMINAL] \t Message %s%s%s%s"), 
			msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);

		theProcBank.m_strTerminalMsg.Format(_T("%s%s%s%s"), msgU.strMsgData[8], msgU.strMsgData[9], msgU.strMsgData[10], msgU.strMsgData[11]);
		
		//2017-07-20,SKCHO, OLB INTERLOCK SKIP
		if(theProcBank.m_bOLBInterLockSkip)
		{
			nIndex = theProcBank.m_strTerminalMsg.Find(_T("MD150"));
			nIndex1 = theProcBank.m_strTerminalMsg.Find(_T("MDR02"));
			if(nIndex1 > 0 || nIndex > 0 )
			{
				return;
			}
		}
		
		theProcBank.SetTerminalMsg();
		break;

	case HOST_SET_LINE_NO:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_LINE_NO] \t"));
		break;

	case HOST_SET_CELL_LOT_INFORMATION:	   //2017-03-09, skcho CELL LOT INFORMATION ����
		theProcBank.m_strProductID.Format(_T("%s"),msgU.strMsgData[0]);
		theProcBank.m_strPPID.Format(_T("%s"),msgU.strMsgData[1]);

		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_LOT_INFORMATION] \t  ProductID %s, PPID %s"), 
			 msgU.strMsgData[0], msgU.strMsgData[1]);
		break;
	case HOST_SET_CELL_JOB_PROCESS:
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CELL_JOB_PROCESS] \t RCMD %s, JOB ID %s, CELL ID %s, PRODUCT ID %s, ACTION TYPE %s, RETURN %s, APNID %s"), 
			msgU.strMsgData[0],msgU.strMsgData[1], msgU.strMsgData[2], msgU.strMsgData[3], msgU.strMsgData[4], msgU.strMsgData[5], msgU.strMsgData[6]);

		theProcBank.m_strRCMD.Format(_T("%s"), msgU.strMsgData[0]);
		if(theProcBank.m_bRCMDCh1Chk)
		{
			theProcBank.m_strRCMDCellIDCh1.Format(_T("%s"), msgU.strMsgData[2]);
			// Ʈ���� RMCDüũ �Ϸ� �Ŀ� RCMD CH ���� �÷��� ���� 2017/06/12 HSK
			theProcBank.m_bRCMDCh1Chk = FALSE;
		}
		if(theProcBank.m_bRCMDCh2Chk)
		{
			theProcBank.m_strRCMDCellIDCh2.Format(_T("%s"), msgU.strMsgData[2]);
			// Ʈ���� RMCDüũ �Ϸ� �Ŀ� RCMD CH ���� �÷��� ���� 2017/06/12 HSK
			theProcBank.m_bRCMDCh2Chk = FALSE;
		}
		break;
	case HOST_SET_PORT_PARAMETER_REPORT:	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_PORT_PARAMETER_REPORT]"));
		theUnitFunc.CIM_TRAYProcessReport();

		break;

	// 2017-02-27, jhLee, TMD �ڵ� Download Triger
	case HOST_SET_TMD_DOWNLOAD :					// TMD �ڵ� Download ��û, S8F1 ����

		//strcpy_s(pData->cMsgData[0],	g_pData->m_sActionFlag);	// S8F1���� ���ŵ� ACTION FLAG, AUTO or MANUAL
		//strcpy_s(pData->cMsgData[1],	g_pData->m_sTMDProductID);	// S8F1���� ���� ����� Product ID

		//strcpy_s(pData->cMsgData[2],	g_pData->m_sTMDFileType);	// S8F1���� ���� File Type
		//strcpy_s(pData->cMsgData[3],	g_pData->m_sTMDFileName);	// S8F1���� ���� File Name
		//strcpy_s(pData->cMsgData[4],	g_pData->m_sTMDFilePath);	// S8F1���� ���� ���� ���
		//strcpy_s(pData->cMsgData[5],	g_pData->m_sTMDChecksum);	// S8F1���� ���� checksum
		//strcpy_s(pData->cCellID,		g_pData->m_sTMDFilePath);	// S8F1���� ���� ���� ���, 100�� ���� �����ϹǷ� Cell ID ������ �̿��Ѵ�.

		// ���´������� S8F1 ���Ž� _T("D:\\Cheetah\\Runtime\\TMDDataList.ini") ���Ϸ� ����� ������ �о�´�.
		theConfigBank.m_System.ReadTMDJobFileInfo();							// �������� ������ Job file download���� ������ �о�´�.

		// ���� ������ ���� ó���Ѵ�.
	    ::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UDMSG_TMDDOWNLOAD_UPDATE, 0, (LPARAM)1);	//2017-03-08, skcho, �������� ����� ������ �������� PG ��ư�� ������ �˸���.
		theConfigBank.m_System.m_bJobStart = TRUE;

		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_TMD_DOWNLOAD]"));
		break;
	case HOST_SET_RECEPIE_CREATE:	  //2017-03-06, skcho, Recepie ���� cim��û ����
		
		//2017-06-16,JSJUNG. ������û- ������ �ٿ�ε� ������ NAK����
		break;
		theProcBank.m_strMESRecipeName = msgU.strMsgData[0];
		::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Recipe)->m_hWnd, UDMSG_RECIPE_UPDATE, 0,0);
		/*
		strTemp = msgU.strMsgData[0];
		theTeachBank.SaveTeachFile(strTemp);
		

		//2017-03-06,skcho, Recipe ���� �� ���� 
		theSocketInterFace.SendCmdRecipeToDataPC(EQUIP_SET_PPID_CREATE, strTemp);
		*/
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_RECEPIE_CREATE]"));
		break;

	case HOST_SET_ATTRIBUTEDATA_RECEIVE:   //2017-03-09,skcho , UNLOADER TRAY ID �������� �ʿ��� �̺�Ʈ(S14F2)
		
		theProcBank.m_strTrayQty.Format(_T("%s"),msgU.strMsgData[0]); 
		theProcBank.m_strCellQty.Format(_T("%s"),msgU.strMsgData[1]); 
		theProcBank.m_strReplyCode.Format(_T("%s"),msgU.strMsgData[2]); 

		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_ATTRIBUTEDATA_RECEIVE]"));
		break;

	case HOST_SET_CARRIER_INFORMATION:   //2017-03-09,skcho , UNLOADER TRAY ID �������� �ʿ��� �̺�Ʈ(S14F2)

		theProcBank.m_strS3F115_CarrierID.Format(_T("%s"),msgU.strMsgData[0]); 
		theProcBank.m_strS3F115_ReplyCode.Format(_T("%s"),msgU.strMsgData[1]); 
	
		theLog[LOG_CIM].AddBuf(_T("RECV\t[HOST_SET_CARRIER_INFORMATION]"));
		theLog[LOG_CIM].AddBuf(_T("RECV\t%s,%s"),theProcBank.m_strS3F115_CarrierID,theProcBank.m_strS3F115_ReplyCode);
		break;
    //2018wjp recive 256
	case EQUIP_GET_CARRIER_CELLID_REQUEST:
		if(theProcBank.LoadCellIDFromLocal())
		{
			theProcBank.m_bGetCellIDComplate=TRUE;
			::SendMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd,UDMSG_UPDATE_LOAD_CELL,0,0);
		}
		else
		{		
			theProcBank.m_bGetCellIDComplate=FALSE;
		}
		break;
	default:
		break;

	}
}

BOOL CSocketDialog::SendCmdToDataPC(int nMsgType, CString strData /*= _T("")*/)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;

	switch(nMsgType)
	{
		case EQUIP_SET_CONTROL:
			strType = _T("EQUIP_SET_CONTROL");
			break;

		case EQUIP_SET_DATAPC_VERSION:
			strType = _T("EQUIP_SET_DATAPC_VERSION");
			break;

		case EQUIP_SET_OPCALL_EVENT:
			strType = _T("EQUIP_SET_OPCALL_EVENT");
			break;

		case EQUIP_SET_INTERLOCK_EVENT:
			strType = _T("EQUIP_SET_INTERLOCK_EVENT");
			break;

		case EQUIP_SET_CELL_EXIST:			// ���� Cell�� �ִ�, ���� ���� [12/1/2016 OSC]
			strType = _T("EQUIP_SET_CELL_EXIST");
			msgU.strMsgData[0] = strData;		// EQST_RUN_IS_CELL or EQST_RUN_NOT_CELL
			break;

		default:
			theLog[LOG_CIM].AddBuf(_T("SendCmdToDataPC - SEND Function Error %d"), nMsgType);
			return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] %s"), strType, msgU.strMsgData[0]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

//2017-11-01,SKCHO, UNIT INTERLOCK ������ ��� �Լ�102 ����
BOOL CSocketDialog::SendCmdUnitInterlockToDataPC(int nMsgType,int nJig,CString strData, CString strInit)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;	
	int nStart = 0;

	if(nJig == 1)  //AJIG
	{
		nStart = 1;
	}
	else if(nJig == 2)  //B JIG
	{
		nStart = 3;
	}
	else if(nJig == 3)  //C JIG
	{
		nStart = 5;
	}
	else if(nJig == 4)  //D JIG
	{
        nStart = 7;
	}

	for(int i = nStart; i <=  nStart+1; i++)
	{
		msgU.sUnit = i;				//channel ��ȣ �߰�

		msgU.strMsgData[0] = strInit;  //"1"�̸� �ʱ�ȭ �޽���

		if(strData == _T("E_EQST_MATCH_INTERLOCK"))
		{
			msgU.sEqst = E_EQST_MATCH_INTERLOCK;
		}
		else if(strData == _T("E_EQST_MATCH_PAUSE"))
		{
			msgU.sEqst = E_EQST_MATCH_PAUSE;
		}		
		else
		{
			msgU.sEqst = E_EQST_MATCH_EXECUTING;
		}
		strType = _T("EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION");

		if(SendToDataPC(msgU))
		{
			theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] %s"), strType, msgU.strMsgData[0]);		
		}
		else
		{
			theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);			
		}
		Sleep(50);
	}

	return TRUE;
}

//2017-11-01,SKCHO, UNIT INTERLOCK ������ ��� �Լ� 514 ����
BOOL CSocketDialog::SendCmdUnitInterlockComfirmToDataPC(int nMsgType,int nChannel,CString strData)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;;
	CString strType;	

	msgU.sUnit = nChannel;				//channel ��ȣ �߰�	

	if(strData == _T("E_EQST_MATCH_INTERLOCK"))
	{
		msgU.sEqst = E_EQST_MATCH_INTERLOCK;
	}
	else if(strData == _T("E_EQST_MATCH_PAUSE"))
	{
		msgU.sEqst = E_EQST_MATCH_PAUSE;
	}		
	else
	{
		msgU.sEqst = E_EQST_MATCH_EXECUTING;
	}
	strType = _T("EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION");

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] %s"), strType, msgU.strMsgData[0]);		
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);			
	}


	return TRUE;
}
BOOL CSocketDialog::SendCmdTrayToDataPC(int nMsgType, CString strData1 , CString strData2)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType,strCEID;

	switch(nMsgType)
	{
	case EQUIP_SET_PORT_PARAMETER_MANAGEMENT:
		strType = _T("EQUIP_SET_PORT_PARAMETER_MANAGEMENT");
		msgU.strMsgData[0] = strData1;		// CEID
		msgU.strMsgData[1] = strData2;		// PortNo

		if (strData1 == _T("350") && strData2 == _T("1"))
		{
			//if (theProcBank.m_bGoodTrayInSWReq)  //2017-04-14,skcho, cimqual - ������ auto 1�� ����
			//	msgU.strMsgData[3] = _T("1");   
			//else
				msgU.strMsgData[3] = _T("1");
		}
		else if (strData1 == _T("353") && strData2 == _T("1"))
		{
				msgU.strMsgData[3] = _T("1");
				msgU.strMsgData[4] = _T("1");
		}
		else if (strData1 == _T("353") && strData2 == _T("2"))
		{
			    //2017-05-25,skcho, ��������  empty
			    msgU.strMsgData[4] = _T("0");  //353 ���� empty: 0
				msgU.strMsgData[3] = _T("1");
		}
		else if (strData1 == _T("350") && strData2 == _T("3"))
		{
			//if (theProcBank.m_bNGTrayInSWReq)
			//	msgU.strMsgData[3] = _T("0");
			//else
				msgU.strMsgData[3] = _T("1");
		}
		else if (strData1 == _T("353") && strData2 == _T("3"))
		{
			//if (theProcBank.m_bNGTrayOutSWReq)
			//	msgU.strMsgData[3] = _T("0");
			//else
				msgU.strMsgData[3] = _T("1");
		}		
		else
			msgU.strMsgData[3] = _T("1");

		if (strData1 == _T("350"))
		{
			msgU.strMsgData[2] = _T("2");
			msgU.strMsgData[4] = _T("1");
			msgU.strMsgData[5] = _T("0");
			
			//�߰� 20170512 JSJUNG
			msgU.strMsgData[6] = _T("");			
			msgU.strMsgData[7] = _T("EMPTYTRAY");

			//PORT1
			//READYTOLOAD, NONE
			//2018-03-09,MGYUN, PORT STATUS MELSEC ��� �߰�
			theMonitorBank.WritePortStatus(1,1,0);
		}
		else if (strData1 == _T("351"))
		{
			msgU.strMsgData[2] = _T("2");
			msgU.strMsgData[4] = _T("2");
			msgU.strMsgData[5] = _T("1");
			//2017-07-17,SKCHO,PORT 4�߰� OK BUFFER PORT�� Ʈ���̰� ������
			if(strData2 == _T("4"))
			{
				msgU.strMsgData[6] = _T("PRELOAD");
				//PORT1
				//LOADED, REAYTOPROCESS
				//2018-03-09,MGYUN, PORT STATUS MELSEC ��� �߰�
				theMonitorBank.WritePortStatus(1,2,1);
			}
			else
			{
				//�߰� 20170512 JSJUNG
				msgU.strMsgData[6] = theProcBank.m_strTrayOpenRelease;  //ó�� �Ѳ�TRAY ID
				//PORT2
				//LOADED, REAYTOPROCESS
				//2018-03-09,MGYUN, PORT STATUS MELSEC ��� �߰�
				theMonitorBank.WritePortStatus(2,2,1);
			}
			msgU.strMsgData[7] = _T("EMPTYTRAY");
		}
		else if (strData1 == _T("352"))
		{
			msgU.strMsgData[2] = _T("2");			
			msgU.strMsgData[4] = _T("3");	//3:ready to Unload		
			msgU.strMsgData[5] = _T("6");
			//2017-12-11,SKCHO �߰� �� ����
			if(theUnitFunc.ReadPortEvent353())
			{
				theProcBank.m_strClose352TrayID = theProcBank.m_strClose353TrayID;

			}
			else
			{
				theProcBank.m_strClose352TrayID = theProcBank.m_strCloseTrayID;	//2017-07-07,SKCHO

			}
			//theProcBank.m_strClose352TrayID = theProcBank.m_strCloseTrayID;	//2017-07-07,SKCHO
			//-----
			msgU.strMsgData[6] = theProcBank.m_strClose352TrayID;  ////�߰� 20170512 JSJUNG ���� �Ѳ�TRAYID 
			msgU.strMsgData[7] = _T("BATCHTRAY");

			theProcBank.m_strClose353TrayID = theProcBank.m_strClose352TrayID;//2017-07-07,SKCHO

			//2017-12-04,SKCHO, ���� ����
			theUnitFunc.SavePortEvent353(theProcBank.m_strClose353TrayID);

			//2017-11-27,SKCHO, �ʱ�ȭ �Ͽ� �ߺ� ���� �ȵǰ�
			//2017-12-04,SKCHO, CIM OFFLINE ��Ȳ�� ���� ���� EVENT ����(�ߺ� ���� ����)
			//theProcBank.m_strCloseTrayID = _T("");

			//PORT4
			//READYTOUNLOAD, PROCESSED
			//2018-03-09,MGYUN, PORT STATUS MELSEC ��� �߰�
			theMonitorBank.WritePortStatus(4,3,6);

			//PORT3
			//EMPTY, NONE
			//2018-03-09,MGYUN, PORT STATUS MELSEC ��� �߰�
			theMonitorBank.WritePortStatus(3,0,0);

		}
		else if (strData1 == _T("353"))
		{
			msgU.strMsgData[2] = _T("2");
			//2017-05-25,skcho, ����
			//msgU.strMsgData[4] = _T("1");
			msgU.strMsgData[5] = _T("0");		
			msgU.strMsgData[6] = theProcBank.m_strClose353TrayID;  //2017-07-07,SKCHO ���� �Ѳ�TRAYID 
			msgU.strMsgData[7] = _T("BATCHTRAY");

			//2017-11-27,SKCHO, �ʱ�ȭ �Ͽ� �ߺ� ���� �ȵǰ�
			theProcBank.m_strClose353TrayID = _T("");
		
			//2017-12-11,SKCHO ����
			//2017-12-04,SKCHO, CIM OFFLINE ��Ȳ�� ���� ���� EVENT ����(�ߺ� ���� ����), 353���� �� ID �ʱ�ȭ
			//--theProcBank.m_strCloseTrayID = _T("");
			theProcBank.m_strClose352TrayID = _T("");

			//2017-12-04,SKCHO, ���� ����
			theUnitFunc.DeletePortEvent353();

			//PORT4
			//EMPTY, NONE
			//2018-03-09,MGYUN, PORT STATUS MELSEC ��� �߰�
			theMonitorBank.WritePortStatus(4,0,0);
		}

		if (strData2 == _T("1") || strData2 == _T("4"))  //2017-07-17,SKCHO,PORT 4�߰� OK BUFFER PORT�� Ʈ���̰� ������
		{
			for (int i = 0; i < 8; i++)
			{
				theProcBank.Port_info.GoodInPortData[i] = msgU.strMsgData[i];
			}
		}
		else if (strData2 == _T("2"))
		{
			for (int i = 0; i < 8; i++)
			{
				theProcBank.Port_info.GoodOutPortData[i] = msgU.strMsgData[i];
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				theProcBank.Port_info.NGPortData[i] = msgU.strMsgData[i];
			}
		}

		theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("CIM_TRAYProcess CEID %s,PORT NO %s,PORT AVAILABLE %s,PORT ACCESS %s,PORT TRANSFER %s,PORT PROCESS %s,TRAY ID %s,TRAY TYPE %s")
			, msgU.strMsgData[0],msgU.strMsgData[1],msgU.strMsgData[2],msgU.strMsgData[3],msgU.strMsgData[4],msgU.strMsgData[5],msgU.strMsgData[6],msgU.strMsgData[7]);

		break;
	case EQUIP_SET_PORT_PARAMETER_REPORT:
		strType = _T("EQUIP_SET_PORT_PARAMETER_REPORT");
		if (strData2 == _T("1"))
		{
			for (int i = 0; i < 6; i++)
			{
				msgU.strMsgData[i] = theProcBank.Port_info.GoodInPortData[i];
			}

			msgU.strMsgData[1] = _T("1");
		}
		else if (strData2 == _T("2"))
		{
			if(theProcBank.Port_info.GoodOutPortData[0] == _T(""))
			{
				for (int i = 0; i < 6; i++)
				{
					msgU.strMsgData[i] = theProcBank.Port_info.GoodInPortData[i];
				}

				msgU.strMsgData[1] = _T("2");
			}
			else
			{
				for (int i = 0; i < 6; i++)
				{
					msgU.strMsgData[i] = theProcBank.Port_info.GoodOutPortData[i];
				}

				msgU.strMsgData[1] = _T("2");
			}
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				msgU.strMsgData[i] = theProcBank.Port_info.NGPortData[i];
			}

			msgU.strMsgData[1] = _T("3");
		}
		
		break;

	case EQUIP_SET_PORTSTATECHANGE_REQUEST:  //2017-07-11, SKCHO, PORT STATE CHANGE �߰�
		strType = _T("EQUIP_SET_PORTSTATECHANGE_REQUEST");

		msgU.strMsgData[0] = _T("254");		// CEID
		msgU.strMsgData[1] = strData1;		// PortNo

		
		if(strData2 == _T("DOWN"))
		{
			msgU.strMsgData[2] = _T("1");  //PORTAVAILSTATE 1:down , 2:up
			msgU.strMsgData[3] = _T("1");  //PORTACCESSMODE 1: auto, 0: manual		
		}	
		else
		{
			msgU.strMsgData[2] = _T("2");
			msgU.strMsgData[3] = _T("1");
		}	

		if(strData1 == _T("1"))
		{
			msgU.strMsgData[4] = theProcBank.Port_info.GoodInPortData[4];
			msgU.strMsgData[5] = theProcBank.Port_info.GoodInPortData[5];
		}
		else
		{
			msgU.strMsgData[4] = theProcBank.Port_info.GoodOutPortData[4];
			msgU.strMsgData[5] = theProcBank.Port_info.GoodOutPortData[5];
		}
	
		if (strData1 == _T("1"))
		{
			for (int i = 0; i < 8; i++)
			{
				theProcBank.Port_info.GoodInPortData[i] = msgU.strMsgData[i];
			}
		}
		else if (strData1 == _T("2"))
		{
			for (int i = 0; i < 8; i++)
			{
				theProcBank.Port_info.GoodOutPortData[i] = msgU.strMsgData[i];
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				theProcBank.Port_info.NGPortData[i] = msgU.strMsgData[i];
			}
		}

		theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("CIM_TRAYProcess CEID %s,PORT NO %s,PORT AVAILABLE %s,PORT ACCESS %s,PORT TRANSFER %s,PORT PROCESS %s,TRAY ID %s,TRAY TYPE %s")
			, msgU.strMsgData[0],msgU.strMsgData[1],msgU.strMsgData[2],msgU.strMsgData[3],msgU.strMsgData[4],msgU.strMsgData[5],msgU.strMsgData[6],msgU.strMsgData[7]);


		break;
	default:
		theLog[LOG_CIM].AddBuf(_T("SendCmdToDataPC - SEND Function Error %d"), nMsgType);
		return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] %s"), strType, msgU.strMsgData[0]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CSocketDialog::SendCmdCellToDataPC(int nMsgType, CCellInfo *pCell)
{
	// Dry Run�߿��� ���� ���� ���Ѵ�... 2017/06/12 HSK.
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;
	CString strLog;
	CString strBuf;
	CString strTmdTemp;			//2018-05-25,MGYUN, CSTRING �ڸ��� ���� �ӽ� ���� �߰�
	//int nTmdLen;				//2018-05-25,MGYUN, DATA LENGTH

	switch(nMsgType)
	{
	case EQUIP_SET_CELL_DATA_CLEAR_REQUEST:		// ������ Cell�� �ʱ�ȭ�Ѵ�. ���� �̻�� [11/28/2016 OSC]
		strType = _T("EQUIP_SET_CELL_DATA_CLEAR_REQUEST");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->m_JigId;
		msgU.sCellPos = pCell->m_JigCh;

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->m_strInnerID, pCell->m_strCellID);
		break;

	case EQUIP_SET_CELL_INFORMATION_REQUEST:
		strType = _T("EQUIP_SET_CELL_INFORMATION_REQUEST");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->m_JigId;
		msgU.sCellPos = pCell->m_JigCh;
		msgU.strMsgData[0] = pCell->m_strInnerID;
		msgU.strCellID = pCell->m_strCellID;

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, msgU.strMsgData[0], msgU.strCellID);
		break;

	case EQUIP_SET_CELL_LOT_INFORMATION_REQUEST:	// 2017-03-09, skcho , UNLOADER TRAY ID SQUENCE���� ���
		strType = _T("EQUIP_SET_CELL_LOT_INFORMATION_REQUEST");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->m_JigId;
		msgU.sCellPos = pCell->m_JigCh;
		msgU.strCellID = pCell->m_strCellID;

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->m_strInnerID, msgU.strCellID);
		break;

	case EQUIP_SET_INSPECTION_RESULT_REPORT: 
		strType = _T("EQUIP_SET_INSPECTION_RESULT_REPORT");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->m_JigId;
		msgU.sCellPos = pCell->m_JigCh;
		//Process Flag
		if (pCell->m_bIsInspectionSkip)
			msgU.strMsgData[0] = _T("N");
		else
			msgU.strMsgData[0] = _T("Y");

		// m_cJudge "G", "R", "L" -> BIN1, BIN2, REJECT

		if (pCell->m_bIsInspectionSkip == FALSE)
		{
			if(pCell->m_LastClass == GOOD_CELL)
				msgU.strMsgData[1] = _T("G");		
			else
			{
				//msgU.strMsgData[1] = _T("L");
				//���� �ڵ带 L���� R�� �ٲ��޶�µ� ���߿� �� �ٲ� ������ �־� �ɼ� ó��. 2017/06/15 HSK.
				msgU.strMsgData[1] = theConfigBank.m_Option.m_strRejectCode;
			}
		}
		else
		{
			//2017-04-07,skcho, CONTACT NG �ϰ�� L�� �����Ѵ�.���� ��û
			if(pCell->m_LastClass == REJECT_CELL)
			{
				//msgU.strMsgData[1] = _T("L");
				msgU.strMsgData[1] = theConfigBank.m_Option.m_strRejectCode;
			}
			else
			{				
				//msgU.strMsgData[1] = _T("R");
				msgU.strMsgData[1] = _T("O");//2017-02-01,CIM QUAL ��û - �˻� ��ŵ�� ��� 'O'�� ó���� ����
			}
		}

		if(theConfigBank.m_Option.m_bUseJudgeOK)
		{
			//2017-08-01,JSJUNG, ����OK ������� �������� "G"
			msgU.strMsgData[1] = _T("G");
		}
		
		msgU.strMsgData[2] = pCell->MesCode;//2017-02-28, skcho, �ҷ��ڵ� �� ���� �߰�
		msgU.strMsgData[3] = pCell->m_LastResult;//2017-02-28, skcho, �ҷ��ڵ� �� ���� �߰�

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s, Judge : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->m_strInnerID, pCell->m_strCellID,msgU.strMsgData[1]);
		break;

	case EQUIP_SET_CELL_PROCESS_START_LOAD:	// TACK IN ���� [11/30/2016 OSC]
		strType = _T("EQUIP_SET_CELL_PROCESS_START_LOAD");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->m_JigId;
		msgU.sCellPos = pCell->m_JigCh;

		msgU.strMsgData[0] = pCell->m_strInnerID;	
		msgU.strCellID = pCell->m_strCellID;

		//2018-05-25,MGYUN, TACK IN ���� �� TMD NAME ���̰� 40 ���� ��� �˶� �߻� �� ������ - ���� �ۿ��� �븮 ��û
		//40 ��ŭ �߶� ������ �� Ȯ���غ��� ����
		strTmdTemp = pCell->m_Zone_A.m_strTMD_Info.SpanExcluding(_T("\n\r"));
		if(strTmdTemp.GetLength() > 40)
		{
			theUnitFunc.SetError(ALARM_TMD_NAME_LENGTH_OVER);
			return FALSE;
			//dlgMsgBox.SetLangName(2, _T("VTM"));		// ����� �̸��� ����
			//dlgMsgBox.SetTitle(_T("�˸�"), _T("Confirm"), _T("Confirm"));
			//dlgMsgBox.SetMessage(FALSE, 
			//	_T("TMD ���� �̸��� 40�� �Ѵ��� Ȯ���� �ּ���."),	//��
			//	_T("Please! CHECK THE TMD FINE NAME LENGTH."),		//��
			//	_T("Please! CHECK THE TMD FINE NAME LENGTH.") );		//��

			//dlgMsgBox.DoModal();
			//// �÷��� ������ �� �÷��׷� ���̾�α� �����.
		}

		//nTmdLen = strTmdTemp.GetLength();
		//pCell->m_Zone_A.m_strTMD_Info = strTmdTemp.Mid(0,39);		//39�ڸ� ���� �ڸ���.

		//2017-10-09,SKCHO
		msgU.strMsgData[1] = pCell->m_Zone_A.m_strTMD_Info;
		msgU.strMsgData[2] = pCell->m_Zone_A.m_strProgram1;
		msgU.strMsgData[3] = pCell->m_Zone_A.m_strProgram2;
		msgU.strMsgData[4] = pCell->m_Zone_A.m_strProgram3;
		msgU.strMsgData[5] = pCell->m_Zone_A.m_strProgram4;		

		//msgU.strMsgData[6] = pCell->m_strCellID == TEXT_UV_FAIL ? _T("1"):_T("0");		// �о����� 0, �ƴϸ� 1 //2017-04-05,skcho, �ڱ״н����� FAIL �� �����͸� ��
		msgU.strMsgData[6] = pCell->m_strCellID == FAIL ? _T("1"):_T("0");	
		
		if (theConfigBank.m_Option.m_bUseRCMD == FALSE)
			msgU.strMsgData[7] = _T("5");
		else
			msgU.strMsgData[7] = pCell->strReadID;

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s, ID Read : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, msgU.strMsgData[0], msgU.strCellID, msgU.strMsgData[6]);
		
		//2017-10-09,SKCHO
		msgU.strMsgData[8] = pCell->m_Zone_A.m_strProgram5;

		break;

	case EQUIP_SET_CELL_TSP_ENDINS:		// TSP �������� ���� [11/30/2016 OSC]
		strType = _T("EQUIP_SET_CELL_TSP_ENDINS");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->m_JigId;
		msgU.sCellPos = pCell->m_JigCh;

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->m_strInnerID, pCell->m_strCellID);
		break;

	case EQUIP_SET_CELL_ENDINS:		// ��� �˻� �������� ���� [11/30/2016 OSC]
		strType = _T("EQUIP_SET_CELL_ENDINS");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->m_JigId;
		msgU.sCellPos = pCell->m_JigCh;
		
		// m_cJudge "G", "R", "L" -> BIN1, BIN2, REJECT
		if(pCell->m_LastClass == GOOD_CELL)
		{
			msgU.strMsgData[3] = _T("G");
		}
		//2017.07.22. JSJUNG. 	GDCLDNG�� "O" �ڵ� ����. ���� �˻�� ��û
		else if(pCell->MesCode == _T("GDCLDNG"))
		{
			msgU.strMsgData[3] = _T("O");
		}
		else
		{
			msgU.strMsgData[3] = theConfigBank.m_Option.m_strRejectCode;
			//msgU.strMsgData[3] = _T("L");	
		}

		msgU.strMsgData[4] = pCell->MesCode;		

		msgU.strMsgData[9] = pCell->PrintDefect;			// m_cDescription	

		strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s, Judge %s, Code : %s, PrintName : %s"), 
			_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->m_strInnerID, pCell->m_strCellID, 
			msgU.strMsgData[3], msgU.strMsgData[4],  msgU.strMsgData[9]	);
		break;

	case EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD:	// TRACK OUT ���� [11/30/2016 OSC]
		strType = _T("EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD");
		if(pCell == NULL)	return FALSE;

		msgU.sUnit = pCell->m_JigId;
		msgU.sCellPos = pCell->m_JigCh;

		msgU.strMsgData[0] = pCell->MesCode;//2017-02-28, skcho, �ҷ��ڵ� �� ���� �߰�
		msgU.strMsgData[1] = pCell->m_LastResult;//2017-02-28, skcho, �ҷ��ڵ� �� ���� �߰�
		msgU.strMsgData[2] = pCell->sAbstract;  //2017-03-02, skcho, �������� ���� 406-judge -"O"

		//2017-09-24,SKCHO, �ִ� 5������
		strBuf.Format(_T("%d"),pCell->m_Zone_C.VIRejectCount);	
		msgU.strMsgData[3] = strBuf;	
		for(int i = 0; i < pCell->m_Zone_C.VIRejectCount; i++)
		{
			strBuf.Format(_T("%s-%s"),pCell->m_Zone_C.stRejct[i].strName,pCell->m_Zone_C.stRejct[i].strXY);
			msgU.strMsgData[4+i] = strBuf;
		}

		//���� �����϶� �α� �ٸ��� ��
		if(pCell ->sAbstract ==_T("1"))
		{
			strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s - TRACKOUT_MANUAL"), 
				_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->m_strInnerID, pCell->m_strCellID);
		}
		else
		{
			strLog.Format(_T("Jig : %c, Ch : %d, InnerID : %s, CellID : %s"), 
				_T('A') + msgU.sUnit, msgU.sCellPos+1, pCell->m_strInnerID, pCell->m_strCellID);
		}
		break;

	case EQUIP_SET_LOSS_TRACK_REPORT:				// �̻�� [12/1/2016 OSC]
		strType = _T("EQUIP_SET_LOSS_TRACK_REPORT");
		if(pCell == NULL)	return FALSE;

		msgU.strMsgData[0] = pCell->m_strInnerID;
		msgU.strCellID = pCell->m_strCellID;
		msgU.strMsgData[2] = pCell->m_strInnerID;	// ���� ���, G (Good, Bin1), L(loss, Reject), R (Retest, Bin2)
		msgU.strMsgData[3] = pCell->MesCode;
		msgU.strMsgData[4] = pCell->MesCode;		// m_cDescription
		break;
	case EQUIP_SET_ATTRIBUTEDATA_REQUEST:            //2017-03-09, skcho, S14F1 ATTRIBUTE
        strType = _T("EQUIP_SET_ATTRIBUTEDATA_REQUEST");
		msgU.strMsgData[0] = _T("");
		msgU.strMsgData[1] = NEXTSTEPID;
		msgU.strMsgData[2] = theConfigBank.m_UnloadRunInfo.m_sCurrentProduct;
		msgU.strMsgData[3] = _T("");
		break;
  
	default:
		theLog[LOG_CIM].AddBuf(_T("SendCellCmdToDataPC - SEND Function Error %d"), nMsgType);

		return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] %s"), strType, strLog);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CSocketDialog::SendCmdAlarmToDataPC(ALARM_ID id, BOOL bSet)
{
	// ������ ���� �����ϴ� Alarm�� �������� [11/30/2016 OSC]
	if(theProcBank.m_AlarmMsg[id].nAlarmLevel == ALARM_LEVEL_WARNING)
		return TRUE;

	tag_Message_Unicode msgU;
	msgU.sMsgType = EQUIP_SET_ALARM;
	CString strType = _T("EQUIP_SET_ALARM");
	msgU.strMsgData[0] = bSet ? ALST_SET:ALST_CLEAR;					// Set or Reset
	// ��˶� ���� �߰�. 2017/04/27 HSK
	if(theProcBank.m_bLightAlarmChk)
	{
		if(bSet == FALSE)
			theProcBank.m_bLightAlarmChk = FALSE;

		msgU.strMsgData[1] = ALCD_LIGHT;	
	}// ��˶�(Warning) or �߾˶�(Alarm)
	else
		msgU.strMsgData[1] = ALCD_SERIOUS;									// ��˶�(Warning) or �߾˶�(Alarm)

	msgU.strMsgData[2].Format(_T("%d"), id);							// Alarm ID
	msgU.strMsgData[3] = theProcBank.m_AlarmMsg[id].strCause[LAN_ENG];	// Alarm Eng Name

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] Set : %s, Level : %s, ID : %s, Name : %s"), 
			strType, msgU.strMsgData[0], msgU.strMsgData[1], msgU.strMsgData[2], msgU.strMsgData[3]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CSocketDialog::SendCmdStateToDataPC(int nMsgType, E_EQST_MATCH state)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;

	switch(nMsgType)
	{
		case EQUIP_SET_EQUIPMENT_STATUS_CHANGE:
			strType = _T("EQUIP_SET_EQUIPMENT_STATUS_CHANGE");
			msgU.sEqst = state;
			break;

		case EQUIP_SET_UNIT_STATUS_CHANGE_LOAD:
			strType = _T("EQUIP_SET_UNIT_STATUS_CHANGE_LOAD");
			msgU.sEqst = state;
			break;

		case EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION:
			strType = _T("EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION");
			msgU.sEqst = state;
			break;

		case EQUIP_SET_UNIT_STATUS_CHANGE_UNLOAD:
			strType = _T("EQUIP_SET_UNIT_STATUS_CHANGE_UNLOAD");
			msgU.sEqst = state;
			break;

		default:
			theLog[LOG_CIM].AddBuf(_T("SendStateToDataPC - SEND Function Error %d"), nMsgType);
			return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] State %d"), strType, msgU.sEqst);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CSocketDialog::SendCmdRecipeToDataPC( int nMsgType, CString strRecipeName, CString strOldRecipeName /*= _T("")*/ )
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;

	switch(nMsgType)
	{

	case EQUIP_SET_PPID_CREATE:
		strType = _T("EQUIP_SET_PPID_CREATE");
		msgU.strMsgData[7] = strRecipeName;	
		break;

	case EQUIP_SET_PPID_MODIFY:
		strType = _T("EQUIP_SET_PPID_MODIFY");
		msgU.strMsgData[7] = strRecipeName;	
		break;

	case EQUIP_SET_PPID_DELETE:
		strType = _T("EQUIP_SET_PPID_DELETE");
		msgU.strMsgData[7] = strRecipeName;	
		break;

	case EQUIP_SET_PPID_CHANGE:
		strType = _T("EQUIP_SET_PPID_CHANGE");
		msgU.strMsgData[7] = strOldRecipeName;	
		msgU.strMsgData[8] = strRecipeName;	
		break;

	case EQUIP_SET_PPID_OPERATION:
		strType = _T("EQUIP_SET_PPID_OPERATION");
		msgU.strMsgData[7] = strRecipeName;	
		break;

	default:
		theLog[LOG_CIM].AddBuf(_T("SendCmdRecipeToDataPC - SEND Function Error %d"), nMsgType);
		return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] Recipe Name : %s"), strType, msgU.strMsgData[7]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CSocketDialog::SendCmdLoginToDataPC( int nMsgType, CString strID /*= _T("")*/, CString strPW /*= _T("")*/ )
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType;

	switch(nMsgType)
	{
	case EQUIP_SET_INS1_USER_LOGIN:		// GaonNuri�� ���� ������ ��� �� GaonNuri�� ID ����ȭ ���� ����ϴ� CMD [11/30/2016 OSC]
		strType = _T("EQUIP_SET_INS1_USER_LOGIN");
		msgU.strMsgData[5] = strID;	
		break;

	case EQUIP_SET_INS1_USER_LOGOUT:		// GaonNuri�� ���� ������ ��� �� GaonNuri�� ID ����ȭ ���� ����ϴ� CMD [11/30/2016 OSC]
		strType = _T("EQUIP_SET_INS1_USER_LOGOUT");
		break;

	case EQUIP_SET_INS2_USER_LOGIN:		// GaonNuri�� ���� ������ ��� �� GaonNuri�� ID ����ȭ ���� ����ϴ� CMD [11/30/2016 OSC]
		strType = _T("EQUIP_SET_INS2_USER_LOGIN");
		msgU.strMsgData[5] = strID;	
		break;

	case EQUIP_SET_INS2_USER_LOGOUT:		// GaonNuri�� ���� ������ ��� �� GaonNuri�� ID ����ȭ ���� ����ϴ� CMD [11/30/2016 OSC]
		strType = _T("EQUIP_SET_INS2_USER_LOGOUT");
		break;

	case EQUIP_SET_USER_LOGIN_REQUEST:		// MES�� User Login �㰡 ��û
		strType = _T("EQUIP_SET_USER_LOGIN_REQUEST");
		msgU.strMsgData[5] = strID;	
		msgU.strMsgData[6] = strPW;	
		break;

	case EQUIP_SET_USER_LOGOUT_REQUEST:		// MES�� User Logout �㰡 ��û
		strType = _T("EQUIP_SET_USER_LOGOUT_REQUEST");
		msgU.strMsgData[5] = strID;	
		msgU.strMsgData[6] = strPW;	
		break;

	default:
		theLog[LOG_CIM].AddBuf(_T("SendCmdLoginToDataPC - SEND Function Error %d"), nMsgType);
		return FALSE;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] ID : %s"), strType, msgU.strMsgData[5]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CSocketDialog::SendCmdTPSystemToDataPC( CString strCode, CString strName )
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = EQUIP_SET_LOSS_CODE;
	CString strType = _T("EQUIP_SET_LOSS_CODE");
	msgU.strMsgData[0] = strCode;
	msgU.strMsgData[1] = strName;

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] Code : %s, Name : %s"), strType, msgU.strMsgData[0], msgU.strMsgData[1]);
		return TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		return FALSE;
	}
}

BOOL CSocketDialog::SendToDataPC(tag_Message_Unicode msgU)
{
	msgU.sMsgClass = CLASS_CONTROL;
	tag_Message msg;
	msgU.Convert_tag_Message(&msg);
	int nSendLen = m_pSocket_DataPC.Send(&msg, sizeof(tag_Message));
	if(nSendLen == sizeof(tag_Message))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CSocketDialog::SendCmdCarrierAssignToDataPC( int nMsgType, CCarrierInfo *pCarrier)
{
	tag_Message_Unicode msgU;
	msgU.sMsgType = nMsgType;
	CString strType,strBuff;

	BOOL bResult = FALSE;

	switch(nMsgType)
	{
		case EQUIP_SET_CARRIER_RELEASE_REQUEST:		//Tray ID Release Request
			strType = _T("EQUIP_SET_CARRIER_RELEASE_REQUEST");	
			break;
        case EQUIP_SET_CARRIER_CLOSE_REQUEST:		//Tray ID Release Request
			strType = _T("EQUIP_SET_CARRIER_CLOSE_REQUEST");	
			break;
		case EQUIP_SET_CARRIER_ASSIGN_REQUEST:		//Tray ID Assign Request
			strType = _T("EQUIP_SET_CARRIER_ASSIGN_REQUEST");		
			break;
		case EQUIP_GET_CARRIER_CELLID_REQUEST:      //Cell ID Get Request
			strType = _T("EQUIP_GET_CARRIER_CELLID_REQUEST");	//2018wjpCIM	
			break;
		case EQUIP_SET_CARRIER_CELL_LOAD_START:
			msgU.strMsgData[0] = theConfigBank.m_UnloadRunInfo.m_sTrayQtyInLot;	
			msgU.strMsgData[1] = theConfigBank.m_UnloadRunInfo.m_sCellQtyInTray;	

			//2017-05-17,skcho, 301/302 ����� CELL ID ���� �߰� CIM QUAL ��û
			msgU.strMsgData[2] = theConfigBank.m_UnloadRunInfo.m_sStartCellID;	
			strType = _T("EQUIP_SET_CARRIER_CELL_LOAD_START");		
			break;

		case EQUIP_SET_CARRIER_CELL_LOAD_END:
			strBuff.Format(_T("%d"),theProcBank.m_nLotCellCnt);
			msgU.strMsgData[0] = theConfigBank.m_UnloadRunInfo.m_sTrayQtyInLot;	
			msgU.strMsgData[1] = theConfigBank.m_UnloadRunInfo.m_sCellQtyInTray;	
			msgU.strMsgData[2] = strBuff;

			//2017-05-17,skcho, 301/302 ����� CELL ID ���� �߰� CIM QUAL ��û
			msgU.strMsgData[3] = theConfigBank.m_UnloadRunInfo.m_sEndCellID;	
			strType = _T("EQUIP_SET_CARRIER_CELL_LOAD_END");		
			break;

		default:
			theLog[LOG_CIM].AddBuf(_T("SendCmdCarrierToDataPC - SEND Function Error %d"), nMsgType);
			bResult= FALSE;
			break;
	}

	if(SendToDataPC(msgU))
	{
		theLog[LOG_CIM].AddBuf(_T("SEND\t[%s] ID : %s"), strType, msgU.strMsgData[5]);
		bResult=  TRUE;
	}
	else
	{
		theLog[LOG_CIM].AddBuf(_T("SEND FAIL\t[%s]"), strType);
		bResult = FALSE;
	}

	return bResult;
}


//
// 2017-02-27, jhLee, ���� �������� Job file download ������ ������ �����ϵ��� �Ѵ�.
//
BOOL CSocketDialog::SendToTMDDownloadState(CString sAction, CString sResult)
{
	tag_Message_Unicode msgU;

	msgU.sMsgClass = CLASS_CONTROL;
	msgU.sMsgType = EQUIP_SET_TMD_DOWNLOAD;

	tag_Message msg;

	// ���� : ���´������� �޴� ������
	//case EQUIP_SET_TMD_DOWNLOAD:		// 2017-01-08, jhLee, TMD �ڵ� Download ������ ������ �����Ѵ�.
	//	g_pData->m_sActionType				= pData->cMsgData[0];	// S6F11�� ���� �� Action type
	//	g_pData->m_sActionResult			= pData->cMsgData[1];	// S6F11�� ���� �� Action �����, OK / NG
	//	g_pData->m_sTMDLocalChecksum		= pData->cMsgData[2];	// S6F11�� ���� �� Download file�� checksum
	//	g_pData->m_sTMDCurrentChecksum	= pData->cMsgData[3];	// S6F11�� ���� �� ���� ���̐� checksum


	msgU.strMsgData[0] = sAction;
	msgU.strMsgData[1] = sResult;
	msgU.strMsgData[2] = _T("");
	msgU.strMsgData[3] = _T("");

	msgU.Convert_tag_Message(&msg);

	int nSendLen = m_pSocket_DataPC.Send(&msg, sizeof(tag_Message));
	if(nSendLen == sizeof(tag_Message))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL CSocketDialog::SendToAttributRequest(CString sEqpID, CString sObjType, CString sObjID)
{
	tag_Message_Unicode msgU;

	msgU.sMsgClass = CLASS_CONTROL;
	msgU.sMsgType = EQUIP_SET_ATTRIBUTEDATA_REQUEST;

	tag_Message msg;
		

	msgU.strMsgData[0] = sEqpID;
	msgU.strMsgData[1] = sObjType;
	msgU.strMsgData[2] = sObjID;
	msgU.strMsgData[3] = _T("");

	msgU.Convert_tag_Message(&msg);

	int nSendLen = m_pSocket_DataPC.Send(&msg, sizeof(tag_Message));
	if(nSendLen == sizeof(tag_Message))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

