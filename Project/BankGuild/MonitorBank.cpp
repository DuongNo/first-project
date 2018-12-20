#include "StdAfx.h"
#include "MonitorBank.h"
#include "Etc\FileSupport.h"
#include "Etc\Ini.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMonitorBank::CMonitorBank(void)
{
}


CMonitorBank::~CMonitorBank(void)
{
}


void CMonitorBank::SetInitData()
{
	CString strBuf;
	strBuf.Format(_T("%s"),theProcBank.m_strEQ_ID);
	theDeviceMelsec.UpperWordSend(MELSEC_EQID,(LPVOID)strBuf.GetString(),MELSEC_EQID_LENGTH,TRUE);

	strBuf.Format(_T("%s"),theProcBank.m_strPPID);
	theDeviceMelsec.UpperWordSend(MELSEC_PPID,(LPVOID)strBuf.GetString(),MELSEC_PPID_LENGTH,TRUE);

}

void CMonitorBank::WriteSwVersion(CString strVersion)
{
	CString strBuf;
	strBuf.Format(_T("%s"),strVersion);
	theDeviceMelsec.UpperWordSend(MELSEC_SW_VERSION,(LPVOID)strBuf.GetString(),MELSEC_SW_VERSION_LENGTH,TRUE);
}

void CMonitorBank::WriteEQPID(CString strEQPID)
{
	CString strBuf;
	strBuf.Format(_T("%s"),strEQPID);
	theDeviceMelsec.UpperWordSend(MELSEC_EQID,(LPVOID)strBuf.GetString(),MELSEC_EQID_LENGTH,TRUE);
}

void CMonitorBank::WritePPID(CString strPPID)
{
	CString strBuf;
	strBuf.Format(_T("%s"),strPPID);
	theDeviceMelsec.UpperWordSend(MELSEC_PPID,(LPVOID)strBuf.GetString(),MELSEC_PPID_LENGTH,TRUE);
}

void CMonitorBank::WriteCimStatus(int nStatus)
{
	CString strBuf;
	strBuf.Format(_T("%d"),nStatus);
	theDeviceMelsec.UpperWordSend(MELSEC_CRST,(LPVOID)strBuf.GetString(),MELSEC_CRST_LENGTH,TRUE);
}
/*
Availability State
Interlock State
Move State
Run State
Front State (상류 설비 물류 상태)
*/
/*MACHINE_PAUSE	= 0,
		MACHINE_INIT 1	,
		MACHINE_IDLE 2	,
		MACHINE_RUN	3	,
		MACHINE_FAULT 4	,
	*/
void CMonitorBank::WriteEqStatus(int nEqStatus )
{
	CString strBuf;
	CString strStatus[5];
	if(nEqStatus == 2)  //MACHINE_IDLE
	{
		//E_EQST_MATCH_IDLE  : MACHINE_IDLE
		//Availability State -2
		//Interlock State -2
		//Move State - 2
		//Run State - 1
		//Front State (상류 설비 물류 상태) -2

		strStatus[0].Format(_T("%d"),2);
		strStatus[1].Format(_T("%d"),2);
		strStatus[2].Format(_T("%d"),2);
		if(theUnitFunc.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT) == FALSE)
		{
			strStatus[3].Format(_T("%d"),1);
		}
		else
		{
			strStatus[3].Format(_T("%d"),2);
		}
		strStatus[4].Format(_T("%d"),2);

	}
	else if(nEqStatus == 3)
	{
		//E_EQST_MATCH_EXECUTING = MACHINE_RUN
		//Availability State -2
		//Interlock State -2
		//Move State - 2
		//Run State - 2  //셀유무 판단후
		//Front State (상류 설비 물류 상태) -1
		strStatus[0].Format(_T("%d"),2);
		strStatus[1].Format(_T("%d"),2);
		strStatus[2].Format(_T("%d"),2);
		if(theUnitFunc.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT) == FALSE)
		{
			strStatus[3].Format(_T("%d"),1);
		}
		else
		{
			strStatus[3].Format(_T("%d"),2);
		}
		strStatus[4].Format(_T("%d"),1);

	}
	else if(nEqStatus == 4)
	{

		//E_EQST_MATCH_FAULT, 알람
		//Availability State -1
		//Interlock State -2
		//Move State - 2
		//Run State - 2  //셀유무 판단후
		//Front State (상류 설비 물류 상태) -1

		strStatus[0].Format(_T("%d"),1);
		strStatus[1].Format(_T("%d"),2);
		strStatus[2].Format(_T("%d"),2);
		if(theUnitFunc.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT) == FALSE)
		{
			strStatus[3].Format(_T("%d"),1);
		}
		else
		{
			strStatus[3].Format(_T("%d"),2);
		}
		strStatus[4].Format(_T("%d"),2);

	}
	else if(nEqStatus == 0||  nEqStatus == 1)
	{
		//E_EQST_MATCH_PAUSE,  사용자 정지
		//Availability State -2
		//Interlock State -2
		//Move State - 1
		//Run State - 2  //셀유무 판단후
		//Front State (상류 설비 물류 상태) -1
		strStatus[0].Format(_T("%d"),2);
		strStatus[1].Format(_T("%d"),2);
		strStatus[2].Format(_T("%d"),1);
		if(theUnitFunc.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT) == FALSE)
		{
			strStatus[3].Format(_T("%d"),1);
		}
		else
		{
			strStatus[3].Format(_T("%d"),2);
		}
		strStatus[4].Format(_T("%d"),1);

	}
	else if(nEqStatus == 5)
	{
		//E_EQST_MATCH_INTERLOCK,  
		//Availability State -1
		//Interlock State -1
		//Move State - 2
		//Run State - 2  //셀유무 판단후
		//Front State (상류 설비 물류 상태) -1
		strStatus[0].Format(_T("%d"),1);
		strStatus[1].Format(_T("%d"),1);
		strStatus[2].Format(_T("%d"),2);
		if(theUnitFunc.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT) == FALSE)
		{
			strStatus[3].Format(_T("%d"),1);
		}
		else
		{
			strStatus[3].Format(_T("%d"),2);
		}
		strStatus[4].Format(_T("%d"),1);
	}

	theDeviceMelsec.UpperWordSend(MELSEC_AVAILABILITY,(LPVOID)strStatus[0].GetString(),MELSEC_AVAILABILITY_LENGTH,TRUE);
	theDeviceMelsec.UpperWordSend(MELSEC_INTERLOCK,(LPVOID)strStatus[1].GetString(),MELSEC_INTERLOCK_LENGTH,TRUE);
	theDeviceMelsec.UpperWordSend(MELSEC_MOVE,(LPVOID)strStatus[2].GetString(),MELSEC_MOVE_LENGTH,TRUE);
	theDeviceMelsec.UpperWordSend(MELSEC_RUN,(LPVOID)strStatus[3].GetString(),MELSEC_RUN_LENGTH,TRUE);
	theDeviceMelsec.UpperWordSend(MELSEC_FRONT,(LPVOID)strStatus[4].GetString(),MELSEC_FRONT_LENGTH,TRUE);	

}
//2018-03-08,MGYUN,MCR 상태 WRITE 
void CMonitorBank::WriteMCRStatus(int nMCRName, BOOL nMCRStatus)
{
	//MCR #1 = Lower
	//MCR #2 = Upper
	
	CString strMCRStatus;
	// MCR STATUS
	// 0 : MCR 0FF
	// 1 : MCR ON
	strMCRStatus.Format(_T("%d"), nMCRStatus);
	//2018-03-27,MGYUN,MCR 통신 상태 체크 수정
	//Lower MCR
	if(nMCRName == eCOMM_Lower_UVMCR)
	{
		if(nMCRStatus == -1)
		{
			strMCRStatus.Format(_T("%d"), 1);
			theDeviceMelsec.UpperWordSend(MELSEC_MCR1_STATUS,(LPVOID)strMCRStatus.GetString(),MELSEC_MCR1_LENGTH,TRUE);
		}
		else
		{
			strMCRStatus.Format(_T("%d"), 0);
			theDeviceMelsec.UpperWordSend(MELSEC_MCR1_STATUS,(LPVOID)strMCRStatus.GetString(),MELSEC_MCR1_LENGTH,TRUE);
		}
	}

	//Upper MCR
	if(nMCRName == eCOMM_Upper_UVMCR)
	{
		if(nMCRStatus == -1)
		{
			strMCRStatus.Format(_T("%d"), 1);
			theDeviceMelsec.UpperWordSend(MELSEC_MCR2_STATUS,(LPVOID)strMCRStatus.GetString(),MELSEC_MCR2_LENGTH,TRUE);
		}
		else
		{
			strMCRStatus.Format(_T("%d"), 0);
			theDeviceMelsec.UpperWordSend(MELSEC_MCR2_STATUS,(LPVOID)strMCRStatus.GetString(),MELSEC_MCR2_LENGTH,TRUE);
		}
	}
} 
//2018-03-08,MGYUN,LDROBOT TEACHING WRITE
void CMonitorBank::WriteLDRobotPOS(double fX, double fY, double fZ, double fT)
{
	//LD ROBOT TEACHING 값
	//X = Data[0]
	//Y = Data[1]
	//Z = Data[2]
	//T = Data[3]

	CString strLDRobotPos;

	strLDRobotPos.Format(_T("%09f"),fX);
	theDeviceMelsec.UpperWordSend(MELSEC_LDROBOT_X,(LPVOID)strLDRobotPos.GetString(),MELSEC_LDROBOT_X_LENGTH,TRUE);	//X값
	
	strLDRobotPos.Format(_T("%09f"),fY);
	theDeviceMelsec.UpperWordSend(MELSEC_LDROBOT_Y,(LPVOID)strLDRobotPos.GetString(),MELSEC_LDROBOT_Y_LENGTH,TRUE);	//Y값
	
	strLDRobotPos.Format(_T("%09f"),fZ);
	theDeviceMelsec.UpperWordSend(MELSEC_LDROBOT_Z,(LPVOID)strLDRobotPos.GetString(),MELSEC_LDROBOT_Z_LENGTH,TRUE);	//Z값
	
	strLDRobotPos.Format(_T("%09f"),fT);
	theDeviceMelsec.UpperWordSend(MELSEC_LDROBOT_T,(LPVOID)strLDRobotPos.GetString(),MELSEC_LDROBOT_T_LENGTH,TRUE);	//T값
	

}
//2018-03-08,MGYUN,ULDROBOT TEACHING CH1 WRITE
void CMonitorBank::WriteULDRobotCH1POS(double fX, double fY, double fZ, double fT)
{
	//ULD ROBOT CH1 TEACHING 값
	//X = Data[0]
	//Y = Data[1]
	//Z = Data[2]
	//T = Data[3]

	CString strULDRobotCH1Pos;

	strULDRobotCH1Pos.Format(_T("%09f"),fX);
	theDeviceMelsec.UpperWordSend(MELSEC_ULDROBOT_CH1_X,(LPVOID)strULDRobotCH1Pos.GetString(),MELSEC_ULDROBOT_CH1_X_LENGTH,TRUE);	//X값
	
	strULDRobotCH1Pos.Format(_T("%09f"),fY);
	theDeviceMelsec.UpperWordSend(MELSEC_ULDROBOT_CH1_Y,(LPVOID)strULDRobotCH1Pos.GetString(),MELSEC_ULDROBOT_CH1_Y_LENGTH,TRUE);	//Y값
	
	strULDRobotCH1Pos.Format(_T("%09f"),fZ);
	theDeviceMelsec.UpperWordSend(MELSEC_ULDROBOT_CH1_Z,(LPVOID)strULDRobotCH1Pos.GetString(),MELSEC_ULDROBOT_CH1_Z_LENGTH,TRUE);	//Z값
	
	strULDRobotCH1Pos.Format(_T("%09f"),fT);
	theDeviceMelsec.UpperWordSend(MELSEC_ULDROBOT_CH1_T,(LPVOID)strULDRobotCH1Pos.GetString(),MELSEC_ULDROBOT_CH1_T_LENGTH,TRUE);	//T값

}
//2018-03-08,MGYUN,ULDROBOT TEACHING CH2 WRITE
void CMonitorBank::WriteULDRobotCH2POS(double fX, double fY, double fZ, double fT)
{
	//ULD ROBOT CH1 TEACHING 값
	//X = Data[0]
	//Y = Data[1]
	//Z = Data[2]
	//T = Data[3]

	CString strULDRobotCH2Pos;

	strULDRobotCH2Pos.Format(_T("%09f"),fX);
	theDeviceMelsec.UpperWordSend(MELSEC_ULDROBOT_CH2_X,(LPVOID)strULDRobotCH2Pos.GetString(),MELSEC_ULDROBOT_CH2_X_LENGTH,TRUE);	//X값
	
	strULDRobotCH2Pos.Format(_T("%09f"),fY);
	theDeviceMelsec.UpperWordSend(MELSEC_ULDROBOT_CH2_Y,(LPVOID)strULDRobotCH2Pos.GetString(),MELSEC_ULDROBOT_CH2_Y_LENGTH,TRUE);	//Y값
	
	strULDRobotCH2Pos.Format(_T("%09f"),fZ);
	theDeviceMelsec.UpperWordSend(MELSEC_ULDROBOT_CH2_Z,(LPVOID)strULDRobotCH2Pos.GetString(),MELSEC_ULDROBOT_CH2_Z_LENGTH,TRUE);	//Z값
	
	strULDRobotCH2Pos.Format(_T("%09f"),fT);
	theDeviceMelsec.UpperWordSend(MELSEC_ULDROBOT_CH2_T,(LPVOID)strULDRobotCH2Pos.GetString(),MELSEC_ULDROBOT_CH2_T_LENGTH,TRUE);	//T값

}
//2018-03-08,MGYUN,EdgeAlign write (Result, X POS, Y POS, T POS)
void CMonitorBank::WriteEdgeAlign(CString strResult, CString strX, CString strY, CString strT)
{
	//RESULT 0 : NG
	//RESULT 1 : OK

	CString strEdgeAlignResult;		//EdgeAlign Result 변수 0:NG , 1:OK
	CString strEdgeAlignPos;		//EdgeAlign RES VALUE X, Y, T 값 저장 변수.
	//OK 이면 1로
	if(strResult == "OK")
	{
		strResult.Format(_T("%d"),1);
	}
	//NG 이면 0으로
	else
	{
		strResult.Format(_T("%d"),0);
	}
	
	strEdgeAlignResult.Format(_T("%s"), strResult);
	
	theDeviceMelsec.UpperWordSend(MELSEC_EDGE_ALIGN_RESULT,(LPVOID)strEdgeAlignResult.GetString(),MELSEC_EDGE_ALIGN_RESULT_LENGTH,TRUE);	//Edge Align Result
	
	// Edge Align Value Write Start

	//Edge Align X
	strEdgeAlignPos.Format(_T("%s"),strX);
	theDeviceMelsec.UpperWordSend(MELSEC_EDGE_ALIGN_X,(LPVOID)strEdgeAlignPos.GetString(),MELSEC_EDGE_ALIGN_X_LENGTH,TRUE);	//Edge Align X
	//Edge Align Y
	strEdgeAlignPos.Format(_T("%s"),strY);
	theDeviceMelsec.UpperWordSend(MELSEC_EDGE_ALIGN_Y,(LPVOID)strEdgeAlignPos.GetString(),MELSEC_EDGE_ALIGN_Y_LENGTH,TRUE);	//Edge Align X
	//Edge Align T
	strEdgeAlignPos.Format(_T("%s"),strT);
	theDeviceMelsec.UpperWordSend(MELSEC_EDGE_ALIGN_T,(LPVOID)strEdgeAlignPos.GetString(),MELSEC_EDGE_ALIGN_T_LENGTH,TRUE);	//Edge Align X

}
//2018-03-08,MGYUN,PRE-ALIGN CH1 WRITE
void CMonitorBank::WritePREAlignCH1(int nResult, double fX, double fY, double fT)
{
	//RESULT 0 : NG
	//RESULT 1 : OK

	CString strPREAlignResult;		//EdgeAlign Result 변수 0:NG , 1:OK
	CString strPREAlignPos;
	strPREAlignResult.Format(_T("%d"), nResult);
	//PRE Align CH1 Result
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_CH1_RESULT,(LPVOID)strPREAlignResult.GetString(),MELSEC_PRE_ALIGN_CH1_LENGTH,TRUE);	
	
	// Edge Align Value Write Start

	//PRE-ALIGN CH1 X
	strPREAlignPos.Format(_T("%09f"),fX);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_CH1_X,(LPVOID)strPREAlignPos.GetString(),MELSEC_PRE_ALIGN_CH1_X_LENGTH,TRUE);	//PRE-ALIGN CH1 Align X
	//PRE-ALIGN CH1 Y
	strPREAlignPos.Format(_T("%09f"),fY);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_CH1_Y,(LPVOID)strPREAlignPos.GetString(),MELSEC_PRE_ALIGN_CH1_Y_LENGTH,TRUE);	//PRE-ALIGN CH1 Align Y
	//PRE-ALIGN CH1 T
	strPREAlignPos.Format(_T("%09f"),fT);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_CH1_T,(LPVOID)strPREAlignPos.GetString(),MELSEC_PRE_ALIGN_CH1_T_LENGTH,TRUE);	//PRE-ALIGN CH1 Align Z
}
//2018-03-08,MGYUN,PRE-ALIGN CH2 WRITE
void CMonitorBank::WritePREAlignCH2(int nResult, double fX, double fY, double fT)
{
	//RESULT 0 : NG
	//RESULT 1 : OK

	CString strPREAlignResult;		//EdgeAlign Result 변수 0:NG , 1:OK
	CString strPREAlignPos;
	strPREAlignResult.Format(_T("%d"), nResult);
	
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_CH2_RESULT,(LPVOID)strPREAlignResult.GetString(),MELSEC_PRE_ALIGN_CH2_LENGTH,TRUE);	//PRE-ALIGB CH2 Result
	
	// Edge Align Value Write Start

	//PRE-ALIGN CH2 X
	strPREAlignPos.Format(_T("%09f"),fX);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_CH2_X,(LPVOID)strPREAlignPos.GetString(),MELSEC_PRE_ALIGN_CH2_X_LENGTH,TRUE);	//PRE-ALIGN CH2 Align X
	//PRE-ALIGN CH2 Y
	strPREAlignPos.Format(_T("%09f"),fY);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_CH2_Y,(LPVOID)strPREAlignPos.GetString(),MELSEC_PRE_ALIGN_CH2_Y_LENGTH,TRUE);	//PRE-ALIGN CH2 Align Y
	//PRE-ALIGN CH2 T
	strPREAlignPos.Format(_T("%09f"),fT);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_CH2_T,(LPVOID)strPREAlignPos.GetString(),MELSEC_PRE_ALIGN_CH2_T_LENGTH,TRUE);	//PRE-ALIGN CH2 Align Z

}
//2018-03-08,MGYUN, MCR CH1,2 CELL ID 기록
void CMonitorBank::WriteMCRCellID(int nCh, CString strCellID)
{
	//FAIL or CELL ID 기록
	/*
			enum JIG_CH
				{
				JIG_CH_1 =0,
				JIG_CH_2,
				JIG_CH_MAX
		};
	*/
	CString strMCRBuf;

	//CH1 일때
	if(nCh == JIG_CH_1)
	{
		if(strCellID == "FAIL")
		{
			strMCRBuf.Format(_T("%s"),strCellID);
			theDeviceMelsec.UpperWordSend(MELSEC_MCR_CH1_CELL_ID,(LPVOID)strMCRBuf.GetString(),MELSEC_MCR_CH1_CELL_ID_LENGTH,TRUE);	
		}
		else
		{
			strMCRBuf.Format(_T("%s"),strCellID);
			theDeviceMelsec.UpperWordSend(MELSEC_MCR_CH1_CELL_ID,(LPVOID)strMCRBuf.GetString(),MELSEC_MCR_CH1_CELL_ID_LENGTH,TRUE);	
		}
	}
	//CH2 일때
	else if(nCh == JIG_CH_2)
	{
		if(strCellID == "FAIL")
		{
			strMCRBuf.Format(_T("%s"),strCellID);
			theDeviceMelsec.UpperWordSend(MELSEC_MCR_CH2_CELL_ID,(LPVOID)strMCRBuf.GetString(),MELSEC_MCR_CH2_CELL_ID_LENGTH,TRUE);	
		}
		else
		{
			strMCRBuf.Format(_T("%s"),strCellID);
			theDeviceMelsec.UpperWordSend(MELSEC_MCR_CH2_CELL_ID,(LPVOID)strMCRBuf.GetString(),MELSEC_MCR_CH2_CELL_ID_LENGTH,TRUE);
		}
	}
}

//2018-03-08,MGYUN, DCR TRAY ID 기록
void CMonitorBank::WriteDCRID(CString strDCRID)
{
	//FAIL or DCR ID 기록
	/*
	
	*/
	CString strDCRBuf;

	if(strDCRID == "FAIL")
	{
		strDCRBuf.Format(_T("%s"),strDCRID);
	}
	else
	{
		strDCRBuf.Format(_T("%s"),strDCRID);
	}
	
	theDeviceMelsec.UpperWordSend(MELSEC_DCR_ID,(LPVOID)strDCRBuf.GetString(),MELSEC_DCR_ID_LENGTH,TRUE);	
	
}

//Port Status ,(Port 관리 설비 전용)
/*
	1 : GOOD IN BUF
	2 : GOOD IN
	3 : GOOD OUT
	4 : GOOD OUT BUF
	5 : NG OUT BUF
	6 : NG IN 
*/

//2018-03-09,MGYUN, Port Status(포트 관리 설비)
void CMonitorBank::WritePortStatus(int nPortNo, int nTransfer, int nProcessing)
{
		//PORTNO
		//Availability State 
		//ACCESSMODE 
		//TRANSFERSTATE 
		//PROCESSINGSTAGE

	CString strPortBuf1,strPortBuf2,strPortBuf3;

	if(nPortNo == 1)		//GOOD IN BUF
	{
		strPortBuf1.Format(_T("%d"),nPortNo);		//PORT NO
		strPortBuf2.Format(_T("%d"),nTransfer);		//TRANSFER STATE
		strPortBuf3.Format(_T("%d"),nProcessing);	//PROCESSING STATE
		
		theDeviceMelsec.UpperWordSend(MELSEC_PORTNO_1,(LPVOID)strPortBuf1.GetString(),MELSEC_PORTNO_1_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTTRANSFERSTATE_1,(LPVOID)strPortBuf2.GetString(),MELSEC_PORTTRANSFERSTATE_1_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTPROCESSINGSTATE_1,(LPVOID)strPortBuf3.GetString(),MELSEC_PORTPROCESSINGSTATE_1_LENGTH,TRUE);	
	
	}
	else if(nPortNo == 2)	//GOOD IN (DCR READ OK)
	{
		strPortBuf1.Format(_T("%d"),nPortNo);		//PORT NO
		strPortBuf2.Format(_T("%d"),nTransfer);		//TRANSFER STATE
		strPortBuf3.Format(_T("%d"),nProcessing);	//PROCESSING STATE
		
		theDeviceMelsec.UpperWordSend(MELSEC_PORTNO_2,(LPVOID)strPortBuf1.GetString(),MELSEC_PORTNO_2_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTTRANSFERSTATE_2,(LPVOID)strPortBuf2.GetString(),MELSEC_PORTTRANSFERSTATE_2_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTPROCESSINGSTATE_2,(LPVOID)strPortBuf3.GetString(),MELSEC_PORTPROCESSINGSTATE_2_LENGTH,TRUE);	
	

	}
	else if(nPortNo == 3)	//GOOD OUT LIFT
	{
		strPortBuf1.Format(_T("%d"),nPortNo);		//PORT NO
		strPortBuf2.Format(_T("%d"),nTransfer);		//TRANSFER STATE
		strPortBuf3.Format(_T("%d"),nProcessing);	//PROCESSING STATE
		
		theDeviceMelsec.UpperWordSend(MELSEC_PORTNO_3,(LPVOID)strPortBuf1.GetString(),MELSEC_PORTNO_3_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTTRANSFERSTATE_3,(LPVOID)strPortBuf2.GetString(),MELSEC_PORTTRANSFERSTATE_3_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTPROCESSINGSTATE_3,(LPVOID)strPortBuf3.GetString(),MELSEC_PORTPROCESSINGSTATE_3_LENGTH,TRUE);	
	}

	else if(nPortNo == 4)	//GOOD OUT BUF
	{
		strPortBuf1.Format(_T("%d"),nPortNo);		//PORT NO
		strPortBuf2.Format(_T("%d"),nTransfer);		//TRANSFER STATE
		strPortBuf3.Format(_T("%d"),nProcessing);	//PROCESSING STATE

		theDeviceMelsec.UpperWordSend(MELSEC_PORTNO_4,(LPVOID)strPortBuf1.GetString(),MELSEC_PORTNO_4_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTTRANSFERSTATE_4,(LPVOID)strPortBuf2.GetString(),MELSEC_PORTTRANSFERSTATE_4_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTPROCESSINGSTATE_4,(LPVOID)strPortBuf3.GetString(),MELSEC_PORTPROCESSINGSTATE_4_LENGTH,TRUE);	
	}

	else if(nPortNo == 5)	//NG IN BUF
	{
		strPortBuf1.Format(_T("%d"),nPortNo);		//PORT NO
		strPortBuf2.Format(_T("%d"),nTransfer);		//TRANSFER STATE
		strPortBuf3.Format(_T("%d"),nProcessing);	//PROCESSING STATE

		theDeviceMelsec.UpperWordSend(MELSEC_PORTNO_5,(LPVOID)strPortBuf1.GetString(),MELSEC_PORTNO_5_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTTRANSFERSTATE_5,(LPVOID)strPortBuf2.GetString(),MELSEC_PORTTRANSFERSTATE_5_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTPROCESSINGSTATE_5,(LPVOID)strPortBuf3.GetString(),MELSEC_PORTPROCESSINGSTATE_5_LENGTH,TRUE);	
	}
	else // NG IN(ALIGN 했을 시)

	{
		strPortBuf1.Format(_T("%d"),nPortNo);		//PORT NO
		strPortBuf2.Format(_T("%d"),nTransfer);		//TRANSFER STATE
		strPortBuf3.Format(_T("%d"),nProcessing);	//PROCESSING STATE

		theDeviceMelsec.UpperWordSend(MELSEC_PORTNO_6,(LPVOID)strPortBuf1.GetString(),MELSEC_PORTNO_6_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTTRANSFERSTATE_6,(LPVOID)strPortBuf2.GetString(),MELSEC_PORTTRANSFERSTATE_6_LENGTH,TRUE);	
		theDeviceMelsec.UpperWordSend(MELSEC_PORTPROCESSINGSTATE_6,(LPVOID)strPortBuf3.GetString(),MELSEC_PORTPROCESSINGSTATE_6_LENGTH,TRUE);	
	}

}


/*
void CMonitorBank::WritePortStatus(CString strPortNo, CString strAvailable, CString strAccess, CString strTransfer, CString strProcess)
{
	CString strBuf1;	//Port No
	CString strBuf2;	//Availability
	CString strBuf3;	//ACCESSMODE 
	CString strBuf4;	//TRANSFERSTATE 
	CString strBuf5;	//PROCESSINGSTAGE	

	CString strPortStatus[5];	//포트 상태 변수
	

	strBuf1.Format(_T("%d"),strPortNo);		//포트No (1,2,3,4,5,6)
	strBuf2.Format(_T("%d"),strAvailable);	//Available 여부(1 : Down , 2 : Up )
	strBuf3.Format(_T("%d"),strAccess);		//0 : manual 1 :  autu
	strBuf4.Format(_T("%d"),strTransfer);		//0 : empty, 1 : Ready to Load, 2 : Loaded , 3 :  Ready to Unload
	strBuf5.Format(_T("%d"),strAvailable);	//0 : None , 1 : Ready to Process, 2 : Ready to start, 3 : Cancled, 4 : Processing, 5 : Paused, 6 : Processed, 7 : Aborted

	if(strBuf1 == 1)  //GOOD IN Buf
	{
		//PORTNO - 1
		//Availability State
		//ACCESSMODE 
		//TRANSFERSTATE 
		//PROCESSINGSTAGE
		strPortStatus[0].Format(_T("%d"),1);
		strPortStatus[1].Format(_T("%d"),strBuf2);
		strPortStatus[2].Format(_T("%d"),strBuf3);
		strPortStatus[3].Format(_T("%d"),strBuf4);
		strPortStatus[4].Format(_T("%d"),strBuf5);
	}
	else if(strBuf1 == 4)	//GOOD IN Buf
	{
		//PORTNO - 1
		//Availability State
		//ACCESSMODE 
		//TRANSFERSTATE 
		//PROCESSINGSTAGE
		strPortStatus[0].Format(_T("%d"),1);
		strPortStatus[1].Format(_T("%d"),strBuf2);
		strPortStatus[2].Format(_T("%d"),strBuf3);
		strPortStatus[3].Format(_T("%d"),strBuf4);
		strPortStatus[4].Format(_T("%d"),strBuf5);
	}
	else if(strBuf1 == 2) //GOOD OUT BUF
	{
		//PORTNO - 1
		//Availability State
		//ACCESSMODE 
		//TRANSFERSTATE 
		//PROCESSINGSTAGE

		strPortStatus[0].Format(_T("%d"),4);
		strPortStatus[1].Format(_T("%d"),strBuf2);
		strPortStatus[2].Format(_T("%d"),strBuf3);
		strPortStatus[3].Format(_T("%d"),strBuf4);
		strPortStatus[4].Format(_T("%d"),strBuf5);
	}
	 
		//Port 1번 : GOOD IN BUF
		theDeviceMelsec.UpperWordSend(MELSEC_PORTNO_1,(LPVOID)strPortStatus[0].GetString(),MELSEC_PORTNO_1_LENGTH,TRUE);
		theDeviceMelsec.UpperWordSend(MELSEC_PORTAVAILSTATE_1,(LPVOID)strPortStatus[1].GetString(),MELSEC_PORTAVAILSTATE_1_LENGTH,TRUE);
		theDeviceMelsec.UpperWordSend(MELSEC_PORTACCESSMODE_1,(LPVOID)strPortStatus[2].GetString(),MELSEC_PORTACCESSMODE_1_LENGTH,TRUE);
		theDeviceMelsec.UpperWordSend(MELSEC_PORTTRANSFERSTATE_1,(LPVOID)strPortStatus[3].GetString(),MELSEC_PORTTRANSFERSTATE_1_LENGTH,TRUE);
		theDeviceMelsec.UpperWordSend(MELSEC_PORTPROCESSINGSTATE_1,(LPVOID)strPortStatus[4].GetString(),MELSEC_PORTPROCESSINGSTATE_1_LENGTH,TRUE);	
		
		//PORT 4번 : GOOD OUT BUF
		theDeviceMelsec.UpperWordSend(MELSEC_PORTNO_4,(LPVOID)strPortStatus[0].GetString(),MELSEC_PORTNO_4_LENGTH,TRUE);
		theDeviceMelsec.UpperWordSend(MELSEC_PORTAVAILSTATE_4,(LPVOID)strPortStatus[1].GetString(),MELSEC_PORTAVAILSTATE_4_LENGTH,TRUE);
		theDeviceMelsec.UpperWordSend(MELSEC_PORTACCESSMODE_4,(LPVOID)strPortStatus[2].GetString(),MELSEC_PORTACCESSMODE_4_LENGTH,TRUE);
		theDeviceMelsec.UpperWordSend(MELSEC_PORTTRANSFERSTATE_4,(LPVOID)strPortStatus[3].GetString(),MELSEC_PORTTRANSFERSTATE_4_LENGTH,TRUE);
		theDeviceMelsec.UpperWordSend(MELSEC_PORTPROCESSINGSTATE_4,(LPVOID)strPortStatus[4].GetString(),MELSEC_PORTPROCESSINGSTATE_4_LENGTH,TRUE);	

}
*/

//2018-03-09, NAM, FDC Loader 
void CMonitorBank::WriteFDCLoader()
{
	CString strFDCloader;

	//LD_UPPERSHUTTLE_Y

	strFDCloader.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_LD_UPPERSHUTTLE_Y]);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_UPPERSHUTTLE_Y,(LPVOID)strFDCloader.GetString(),MELSEC_LD_UPPERSHUTTLE_Y_LENGTH,TRUE);

	//LD_LOWERSHUTTLE_Y

	strFDCloader.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_LD_LOWERSHUTTLE_Y]);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_LOWERSHUTTLE_Y,(LPVOID)strFDCloader.GetString(),MELSEC_LD_LOWERSHUTTLE_Y_LENGTH,TRUE);

	//PREALIGN_TR_Y

	strFDCloader.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_PREALIGN_TR_Y]);
	theDeviceMelsec.UpperWordSend(MELSEC_PREALIGN_TR_Y,(LPVOID)strFDCloader.GetString(),MELSEC_PREALIGN_TR_Y_LENGTH,TRUE);

	//PREALIGN_TR_Z

	strFDCloader.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_PREALIGN_TR_Z]);
	theDeviceMelsec.UpperWordSend(MELSEC_PREALIGN_TR_Z,(LPVOID)strFDCloader.GetString(),MELSEC_PREALIGN_TR_Z_LENGTH,TRUE);

	//PA_STAGE_X

	strFDCloader.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_PA_STAGE_X]);
	theDeviceMelsec.UpperWordSend(MELSEC_PA_STAGE_X,(LPVOID)strFDCloader.GetString(),MELSEC_PA_STAGE_X_LENGTH,TRUE);

	//LD_TR_X

	strFDCloader.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_LD_TR_X]);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_TR_X,(LPVOID)strFDCloader.GetString(),MELSEC_LD_TR_X_LENGTH,TRUE);

	//LD_TR_Z

	strFDCloader.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_LD_TR_Z]);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_TR_Z,(LPVOID)strFDCloader.GetString(),MELSEC_LD_TR_Z_LENGTH,TRUE);
}

//2018-03-09, NAM, FDC for Zones 
void  CMonitorBank::WriteFDCZone()
{
	CString strCell_ID;
	CString strStep_ID;
	CString str_vac;
    

	//A ZONE  CH1
	//A Zone Cell ID CH1
	strCell_ID.Format(_T("%s"),theFDCBank.m_strJig_CellID[JIG_ID_A][JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_AZONE_CH1_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_INS_AZONE_CH1_CELL_ID_LENGTH,TRUE);

	// A Zone Step ID CH1
	strStep_ID.Format(_T("%d"),theFDCBank.m_nJig_StepID[JIG_ID_A][JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_AZONE_CH1_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_INS_AZONE_CH1_STEP_ID_LENGTH,TRUE);

	// A Zone Vac CH1
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[A_ZONE_CH1]);
	theDeviceMelsec.UpperWordSend(MELSEC_A_ZONE_CH1_VAC,(LPVOID)str_vac.GetString(),MELSEC_A_ZONE_CH1_VAC_LENGTH,TRUE);
 
	//A Zone CH2
	//A Zone Cell ID  CH2
	strCell_ID.Format(_T("%s"),theFDCBank.m_strJig_CellID[JIG_ID_A][JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_AZONE_CH2_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_INS_AZONE_CH2_CELL_ID_LENGTH,TRUE);

	// A Zone Step ID  CH2 
	strStep_ID.Format(_T("%d"),theFDCBank.m_nJig_StepID[JIG_ID_A][JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_AZONE_CH2_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_INS_AZONE_CH2_STEP_ID_LENGTH,TRUE);

	// A Zone Vac  CH2
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[A_ZONE_CH2]);
	theDeviceMelsec.UpperWordSend(MELSEC_A_ZONE_CH2_VAC,(LPVOID)str_vac.GetString(),MELSEC_A_ZONE_CH2_VAC_LENGTH,TRUE);



	//B ZONE  CH1
	//B Zone Cell ID CH1
	strCell_ID.Format(_T("%s"),theFDCBank.m_strJig_CellID[JIG_ID_B][JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_BZONE_CH1_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_INS_BZONE_CH1_CELL_ID_LENGTH,TRUE);

	// B Zone Step ID CH1
	strStep_ID.Format(_T("%d"),theFDCBank.m_nJig_StepID[JIG_ID_B][JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_BZONE_CH1_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_INS_BZONE_CH1_STEP_ID_LENGTH,TRUE);

	// B Zone Vac CH1
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[B_ZONE_CH1]);
	theDeviceMelsec.UpperWordSend(MELSEC_B_ZONE_CH1_VAC,(LPVOID)str_vac.GetString(),MELSEC_B_ZONE_CH1_VAC_LENGTH,TRUE);
 
	//B Zone CH2
	//B Zone Cell ID  CH2
	strCell_ID.Format(_T("%s"),theFDCBank.m_strJig_CellID[JIG_ID_B][JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_BZONE_CH2_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_INS_AZONE_CH2_CELL_ID_LENGTH,TRUE);

	// B Zone Step ID  CH2 
	strStep_ID.Format(_T("%d"),theFDCBank.m_nJig_StepID[JIG_ID_B][JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_BZONE_CH2_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_INS_AZONE_CH2_STEP_ID_LENGTH,TRUE);

	// B Zone Vac  CH2
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[B_ZONE_CH2]);
	theDeviceMelsec.UpperWordSend(MELSEC_B_ZONE_CH2_VAC,(LPVOID)str_vac.GetString(),MELSEC_B_ZONE_CH2_VAC_LENGTH,TRUE);

	
	//C ZONE  CH1
	//C Zone Cell ID CH1
	strCell_ID.Format(_T("%s"),theFDCBank.m_strJig_CellID[JIG_ID_C][JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_CZONE_CH1_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_INS_CZONE_CH1_CELL_ID_LENGTH,TRUE);

	// C Zone Step ID CH1
	strStep_ID.Format(_T("%d"),theFDCBank.m_nJig_StepID[JIG_ID_C][JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_CZONE_CH1_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_INS_CZONE_CH1_STEP_ID_LENGTH,TRUE);

	// C Zone Vac CH1
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[C_ZONE_CH1]);
	theDeviceMelsec.UpperWordSend(MELSEC_C_ZONE_CH1_VAC,(LPVOID)str_vac.GetString(),MELSEC_C_ZONE_CH1_VAC_LENGTH,TRUE);
 
	//C Zone CH2
	//C Zone Cell ID  CH2
	strCell_ID.Format(_T("%s"),theFDCBank.m_strJig_CellID[JIG_ID_C][JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_CZONE_CH2_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_INS_CZONE_CH2_CELL_ID_LENGTH,TRUE);

	// C Zone Step ID  CH2 
	strStep_ID.Format(_T("%d"),theFDCBank.m_nJig_StepID[JIG_ID_C][JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_CZONE_CH2_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_INS_CZONE_CH2_STEP_ID_LENGTH,TRUE);

	// C Zone Vac  CH2
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[C_ZONE_CH2]);
	theDeviceMelsec.UpperWordSend(MELSEC_C_ZONE_CH2_VAC,(LPVOID)str_vac.GetString(),MELSEC_C_ZONE_CH2_VAC_LENGTH,TRUE);

	//D ZONE  CH1
	//D Zone Cell ID CH1
	strCell_ID.Format(_T("%s"),theFDCBank.m_strJig_CellID[JIG_ID_D][JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_DZONE_CH1_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_INS_DZONE_CH1_CELL_ID_LENGTH,TRUE);

	// D Zone Step ID CH1
	strStep_ID.Format(_T("%d"),theFDCBank.m_nJig_StepID[JIG_ID_D][JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_DZONE_CH1_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_INS_DZONE_CH1_STEP_ID_LENGTH,TRUE);

	// D Zone Vac CH1
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[D_ZONE_CH1]);
	theDeviceMelsec.UpperWordSend(MELSEC_D_ZONE_CH1_VAC,(LPVOID)str_vac.GetString(),MELSEC_D_ZONE_CH1_VAC_LENGTH,TRUE);
 
	//D Zone CH2
	//D Zone Cell ID  CH2
	strCell_ID.Format(_T("%s"),theFDCBank.m_strJig_CellID[JIG_ID_D][JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_DZONE_CH2_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_INS_DZONE_CH2_CELL_ID_LENGTH,TRUE);

	// D Zone Step ID  CH2 
	strStep_ID.Format(_T("%d"),theFDCBank.m_nJig_StepID[JIG_ID_D][JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_INS_DZONE_CH2_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_INS_DZONE_CH2_STEP_ID_LENGTH,TRUE);

	// D Zone Vac  CH2
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[D_ZONE_CH2]);
	theDeviceMelsec.UpperWordSend(MELSEC_D_ZONE_CH2_VAC,(LPVOID)str_vac.GetString(),MELSEC_D_ZONE_CH2_VAC_LENGTH,TRUE);

}

//2018-03-09, NAM, FDC for Unloader Transfer

void CMonitorBank::WriteFDCUld()
{

	CString strCell_ID;
	CString strStep_ID;
	CString str_vac;

	//ULD TR  CH1
	//ULD TR  CH1 Cell ID
	strCell_ID.Format(_T("%s"),theFDCBank.m_strULDTR_CellID[JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_TR_CH1_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_ULD_TR_CH1_CELL_ID_LENGTH,TRUE);

	// ULD TR  CH1 Step ID 
	strStep_ID.Format(_T("%d"),theFDCBank.m_nULDTR_StepID[JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_TR_CH1_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_ULD_TR_CH1_STEP_ID_LENGTH,TRUE);

	// ULD TR  Vac CH1
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[ULD_TR_CH1]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_TR_CH1_VAC,(LPVOID)str_vac.GetString(),MELSEC_ULD_TR_CH1_VAC_LENGTH,TRUE);
 
	//ULD TR CH2
	//ULD TR  CH2 Cell ID
	strCell_ID.Format(_T("%s"),theFDCBank.m_strULDTR_CellID[JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_TR_CH2_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_ULD_TR_CH2_CELL_ID_LENGTH,TRUE);

	// ULD TR  CH2 Cell ID 
	strStep_ID.Format(_T("%d"),theFDCBank.m_nULDTR_StepID[JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_TR_CH2_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_ULD_TR_CH2_STEP_ID_LENGTH,TRUE);

	// ULD TR  Vac  CH2
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[ULD_TR_CH2]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_TR_CH2_VAC,(LPVOID)str_vac.GetString(),MELSEC_ULD_TR_CH2_VAC_LENGTH,TRUE);
}

//2018-03-09, NAM, FDC Unloader Stage
void CMonitorBank::WriteFDCUld_Stage()
{

	CString strCell_ID;
	CString strStep_ID;
	CString str_vac;

	//ULD Stage  CH1
	//ULD Stage  CH1 Cell ID
	strCell_ID.Format(_T("%s"),theFDCBank.m_strULDStage_CellID[JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_STAGE_CH1_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_ULD_STAGE_CH1_CELL_ID_LENGTH,TRUE);

	// ULD Stage  CH1 Step ID 
	strStep_ID.Format(_T("%d"),theFDCBank.m_nULDStage_StepID[JIG_CH_1]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_STAGE_CH1_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_ULD_STAGE_CH1_STEP_ID_LENGTH,TRUE);

	// ULD Stage  Vac CH1
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[ULD_STAGE_CH1]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_STAGE_CH1_VAC,(LPVOID)str_vac.GetString(),MELSEC_ULD_STAGE_CH1_VAC_LENGTH,TRUE);
 
	//ULD Stage CH2
	//ULD Stage CH2 Cell ID
	strCell_ID.Format(_T("%s"),theFDCBank.m_strULDStage_CellID[JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_STAGE_CH2_CELL_ID,(LPVOID)strCell_ID.GetString(),MELSEC_ULD_STAGE_CH2_CELL_ID_LENGTH,TRUE);

	// ULD Stage  CH2 Cell ID 
	strStep_ID.Format(_T("%d"),theFDCBank.m_nULDStage_StepID[JIG_CH_2]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_STAGE_CH2_STEP_ID,(LPVOID)strStep_ID.GetString(),MELSEC_ULD_STAGE_CH2_CELL_ID_LENGTH,TRUE);

	// ULD Stage  Vac  CH2
	str_vac.Format(_T("%09f"),theFDCBank.m_dInspVac_OUT[ULD_STAGE_CH2]);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_STAGE_CH2_VAC,(LPVOID)str_vac.GetString(),MELSEC_ULD_STAGE_CH2_VAC_LENGTH,TRUE);
}

//2018-03-09, NAM,  FDC for Good Tray
void CMonitorBank::WriteFDCGoodTray()
{
	CString strGoodtray;

	//GOOD_TRAY_IN_LIFT_Z
	strGoodtray.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z]);
	theDeviceMelsec.UpperWordSend(MELSEC_GOOD_TRAY_IN_LIFT_Z,(LPVOID)strGoodtray.GetString(),MELSEC_GOOD_TRAY_IN_LIFT_Z_LENGTH,TRUE);

	//GOOD_TRAY_OUT_LIFT_Z
	strGoodtray.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z]);
	theDeviceMelsec.UpperWordSend(MELSEC_GOOD_TRAY_OUT_LIFT_Z,(LPVOID)strGoodtray.GetString(),MELSEC_GOOD_TRAY_OUT_LIFT_Z_LENGTH,TRUE);

	//GOOD_TRAY_TR_Y
	strGoodtray.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_GOOD_TRAY_TR_Y]);
	theDeviceMelsec.UpperWordSend(MELSEC_GOOD_TRAY_TR_Y,(LPVOID)strGoodtray.GetString(),MELSEC_GOOD_TRAY_TR_Y_LENGTH,TRUE);

	//GOOD_TRAY_TR_Z
	strGoodtray.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_GOOD_TRAY_TR_Z]);
	theDeviceMelsec.UpperWordSend(MELSEC_GOOD_TRAY_TR_Z,(LPVOID)strGoodtray.GetString(),MELSEC_GOOD_TRAY_TR_Z_LENGTH,TRUE);

}


//2018-03-09, NAM, FDC for NG Tray 
void CMonitorBank::WriteFDCNGTray()
{
	CString strNGtray;

	//NG_TRAY_IN_LIFT_Z
	strNGtray.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_NG_TRAY_IN_LIFT_Z]);
	theDeviceMelsec.UpperWordSend(MELSEC_NG_TRAY_IN_LIFT_Z,(LPVOID)strNGtray.GetString(),MELSEC_NG_TRAY_IN_LIFT_Z_LENGTH,TRUE);

	//NG_TRAY_OUT_LIFT_Z
	strNGtray.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_NG_TRAY_OUT_LIFT_Z]);
	theDeviceMelsec.UpperWordSend(MELSEC_NG_TRAY_OUT_LIFT_Z,(LPVOID)strNGtray.GetString(),MELSEC_NG_TRAY_OUT_LIFT_Z_LENGTH,TRUE);

	//NG_TRAY_TR_X
	strNGtray.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_NG_TRAY_TR_X]);
	theDeviceMelsec.UpperWordSend(MELSEC_NG_TRAY_TR_Y,(LPVOID)strNGtray.GetString(),MELSEC_NG_TRAY_TR_Y_LENGTH,TRUE);

	//NG_TRAY_TR_Z
	strNGtray.Format(_T("%09f"),theFDCBank.m_dTorque[AXIS_SVO_NG_TRAY_TR_Z]);
	theDeviceMelsec.UpperWordSend(MELSEC_NG_TRAY_TR_Z,(LPVOID)strNGtray.GetString(),MELSEC_NG_TRAY_TR_Z_LENGTH,TRUE);
}

//2018-03-09, NAM, FDC for UTIL 
void  CMonitorBank::WriteFDCUtil()
{
	CDeviceSerialGMS* pDeviceGMS = theSerialInterFace.GetDeviceGMSHandler();
	CString  strFDCUtil;

	// PC_RACK_TEMP
	strFDCUtil.Format(_T("%09f"),theFDCBank.m_dPc_Rack_Temp);
	theDeviceMelsec.UpperWordSend(MELSEC_PC_RACK_TEMP,(LPVOID)strFDCUtil.GetString(),MELSEC_PC_RACK_TEMP_LENGTH,TRUE);

	// EFU_ALARM
	strFDCUtil.Format(_T("%d"),theFDCBank.m_nEFU_Alarm);
	theDeviceMelsec.UpperWordSend(MELSEC_EFU_ALARM,(LPVOID)strFDCUtil.GetString(),MELSEC_EFU_ALARM_LENGTH,TRUE);

	// MAIN_AIR1
	strFDCUtil.Format(_T("%09f"),theFDCBank.m_dMainAir_1_OUT);
	theDeviceMelsec.UpperWordSend(MELSEC_MAIN_AIR1,(LPVOID)strFDCUtil.GetString(),MELSEC_MAIN_AIR1_LENGTH,TRUE);

	// MAIN_AIR2
	strFDCUtil.Format(_T("%09f"),theFDCBank.m_dMainAir_2_OUT);
	theDeviceMelsec.UpperWordSend(MELSEC_MAIN_AIR2,(LPVOID)strFDCUtil.GetString(),MELSEC_MAIN_AIR2_LENGTH,TRUE);

	// MAIN_VAC1
	strFDCUtil.Format(_T("%09f"),theFDCBank.m_dMainVac_1_OUT);
	theDeviceMelsec.UpperWordSend(MELSEC_MAIN_VAC1,(LPVOID)strFDCUtil.GetString(),MELSEC_MAIN_VAC1_LENGTH,TRUE);

	// MAIN_VAC2
	strFDCUtil.Format(_T("%09f"),theFDCBank.m_dMainVac_2_OUT);
	theDeviceMelsec.UpperWordSend(MELSEC_MAIN_VAC2,(LPVOID)strFDCUtil.GetString(),MELSEC_MAIN_VAC2_LENGTH,TRUE);

	// GMS_1
	strFDCUtil.Format(_T("%s"),pDeviceGMS->m_LastGMSValue[GMS_A_JIG]);
	theDeviceMelsec.UpperWordSend(MELSEC_GMS_1,(LPVOID)strFDCUtil.GetString(),MELSEC_GMS_1_LENGTH,TRUE);

	// GMS_2
	strFDCUtil.Format(_T("%s"),pDeviceGMS->m_LastGMSValue[GMS_B_JIG]);
	theDeviceMelsec.UpperWordSend(MELSEC_GMS_2,(LPVOID)strFDCUtil.GetString(),MELSEC_GMS_2_LENGTH,TRUE);

	// GMS_3
	strFDCUtil.Format(_T("%s"),pDeviceGMS->m_LastGMSValue[GMS_C_JIG]);
	theDeviceMelsec.UpperWordSend(MELSEC_GMS_3,(LPVOID)strFDCUtil.GetString(),MELSEC_GMS_3_LENGTH,TRUE);

	// GMS_4
	strFDCUtil.Format(_T("%s"),pDeviceGMS->m_LastGMSValue[GMS_D_JIG]);
	theDeviceMelsec.UpperWordSend(MELSEC_GMS_4,(LPVOID)strFDCUtil.GetString(),MELSEC_GMS_4_LENGTH,TRUE);

	// GMS_5
	strFDCUtil.Format(_T("%s"),pDeviceGMS->m_LastGMSValue[GMS_SUB_CH5]);
	theDeviceMelsec.UpperWordSend(MELSEC_GMS_5,(LPVOID)strFDCUtil.GetString(),MELSEC_GMS_5_LENGTH,TRUE);

	// BZone PGMS
	strFDCUtil.Format(_T("%s"),pDeviceGMS->m_LastBZonePGMS);
	theDeviceMelsec.UpperWordSend(MELSEC_PGMS_1,(LPVOID)strFDCUtil.GetString(),MELSEC_PGMS_1_LENGTH,TRUE);

	// DZone PGMS
	strFDCUtil.Format(_T("%s"),pDeviceGMS->m_LastDZonePGMS);
	theDeviceMelsec.UpperWordSend(MELSEC_PGMS_2,(LPVOID)strFDCUtil.GetString(),MELSEC_PGMS_2_LENGTH,TRUE);

}

//2018-03-09, NAM, FDC for Sem
void CMonitorBank::WriteFDCSem()
{
	CString strFDCSem;

	// THREE_PHASE_POWER1
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbWattNet);
	theDeviceMelsec.UpperWordSend(MELSEC_THREE_PHASE_POWER1,(LPVOID)strFDCSem.GetString(),MELSEC_THREE_PHASE_POWER1_LENGTH,TRUE);

	// INSTANTANEOUS_POWER1
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbWattTotal);
	theDeviceMelsec.UpperWordSend(MELSEC_INSTANTANEOUS_POWER1,(LPVOID)strFDCSem.GetString(),MELSEC_INSTANTANEOUS_POWER1_LENGTH,TRUE);

	// R_PHASE_VOLTAGE1
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_R);
	theDeviceMelsec.UpperWordSend(MELSEC_R_PHASE_VOLTAGE1,(LPVOID)strFDCSem.GetString(),MELSEC_R_PHASE_VOLTAGE1_LENGTH,TRUE);

	// T_PHASE_VOLTAGE1
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_T);
	theDeviceMelsec.UpperWordSend(MELSEC_T_PHASE_VOLTAGE1,(LPVOID)strFDCSem.GetString(),MELSEC_T_PHASE_VOLTAGE1_LENGTH,TRUE);

	// R_PHASE_CURRENT1
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_R);
	theDeviceMelsec.UpperWordSend(MELSEC_R_PHASE_CURRENT1,(LPVOID)strFDCSem.GetString(),MELSEC_R_PHASE_CURRENT1_LENGTH,TRUE);

	// T_PHASE_CURRENT1
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_T);
	theDeviceMelsec.UpperWordSend(MELSEC_T_PHASE_CURRENT1,(LPVOID)strFDCSem.GetString(),MELSEC_T_PHASE_CURRENT1_LENGTH,TRUE);

	// THREE_PHASE_POWER2
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbWattNet);
	theDeviceMelsec.UpperWordSend(MELSEC_THREE_PHASE_POWER2,(LPVOID)strFDCSem.GetString(),MELSEC_THREE_PHASE_POWER2_LENGTH,TRUE);

	// INSTANTANEOUS_POWER2
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbWattTotal);
	theDeviceMelsec.UpperWordSend(MELSEC_INSTANTANEOUS_POWER2,(LPVOID)strFDCSem.GetString(),MELSEC_INSTANTANEOUS_POWER2_LENGTH,TRUE);

	// R_PHASE_VOLTAGE2
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_R);
	theDeviceMelsec.UpperWordSend(MELSEC_R_PHASE_VOLTAGE2,(LPVOID)strFDCSem.GetString(),MELSEC_R_PHASE_VOLTAGE2_LENGTH,TRUE);

	// R_PHASE_CURRENT2
	strFDCSem.Format(_T("%09f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_R);
	theDeviceMelsec.UpperWordSend(MELSEC_R_PHASE_CURRENT2,(LPVOID)strFDCSem.GetString(),MELSEC_R_PHASE_CURRENT2_LENGTH,TRUE);

}

//2018-03-09, NAM, Step ID for Status of Cell in Loader position 
void CMonitorBank::WriteStepID()
{
	CString strStepID;
	BYTE    Cellexist;

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_LD_CONV);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_CONV_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_LD_CONV_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_LDROBOT);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_ROBOT_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_LD_ROBOT_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_UPPER_SHUTTLE_CH1_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_LD_UPPER_SHUTTLE_CH1_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_UPPER_SHUTTLE_CH2_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_LD_UPPER_SHUTTLE_CH2_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_LOWER_SHUTTLE_CH1_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_LD_LOWER_SHUTTLE_CH1_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_LOWER_SHUTTLE_CH2_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_LD_LOWER_SHUTTLE_CH2_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_PA_TR_CH1);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_TR_CH1_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_PRE_ALIGN_TR_CH1_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_PA_TR_CH2);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_TR_CH2_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_PRE_ALIGN_TR_CH2_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_PA_STAGE_CH1);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_STAGE_CH1_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_PRE_ALIGN_STAGE_CH1_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_PA_STAGE_CH2);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_PRE_ALIGN_STAGE_CH2_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_PRE_ALIGN_STAGE_CH2_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_LD_TR_CH1);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_TR_CH1_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_LD_TR_CH1_STEP_ID_LENGTH,TRUE);

	Cellexist = theUnitFunc.CellTagExist(CELL_POS_LD_TR_CH2);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_LD_TR_CH2_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_LD_TR_CH2_STEP_ID_LENGTH,TRUE);
	
	Cellexist = theUnitFunc.CellTagExist(CELL_POS_UD_ROBOT);	
	strStepID.Format(_T("%d"),Cellexist);
	theDeviceMelsec.UpperWordSend(MELSEC_ULD_ROBOT_STEP_ID,(LPVOID)strStepID.GetString(),MELSEC_ULD_ROBOT_STEP_ID_LENGTH,TRUE);

}
/*
enum MAIN_TACK
{
	LOADER_TACT =0,
	UNLOADER_TACT,
	AZONE_TACT,
	BZONE_TACT,
	CZONE_TACT,
	DZONE_TACT,
};
*/
//2018-03-12,MGYUN. TACT TIME MELSEC 기록 함수
void CMonitorBank::WriteTactTime(MAIN_TACK main_tact, CString strTactTime)
{
	CString strBuf;
	
	if(main_tact == LOADER_TACT)
	{
		strBuf.Format(_T("%s"),strTactTime);
		theDeviceMelsec.UpperWordSend(MELSEC_TACT_TIME_LD,(LPVOID)strBuf.GetString(),MELSEC_TACT_TIME_LD_LENGTH,TRUE);
	}
	else if(main_tact == UNLOADER_TACT)
	{
		strBuf.Format(_T("%s"),strTactTime);
		theDeviceMelsec.UpperWordSend(MELSEC_TACT_TIME_ULD,(LPVOID)strBuf.GetString(),MELSEC_TACT_TIME_ULD_LENGTH,TRUE);
	}
	else if(main_tact == AZONE_TACT)
	{
		strBuf.Format(_T("%s"),strTactTime);
		theDeviceMelsec.UpperWordSend(MELSEC_TACT_TIME_AZONE,(LPVOID)strBuf.GetString(),MELSEC_TACT_TIME_AZONE_LENGTH,TRUE);
	}
	else if(main_tact == BZONE_TACT)
	{
		strBuf.Format(_T("%s"),strTactTime);
		theDeviceMelsec.UpperWordSend(MELSEC_TACT_TIME_BZONE,(LPVOID)strBuf.GetString(),MELSEC_TACT_TIME_BZONE_LENGTH,TRUE);
	}
	else if(main_tact == CZONE_TACT)
	{
		strBuf.Format(_T("%s"),strTactTime);
		theDeviceMelsec.UpperWordSend(MELSEC_TACT_TIME_CZONE,(LPVOID)strBuf.GetString(),MELSEC_TACT_TIME_CZONE_LENGTH,TRUE);
	}
	else if(main_tact == DZONE_TACT)
	{
		strBuf.Format(_T("%s"),strTactTime);
		theDeviceMelsec.UpperWordSend(MELSEC_TACT_TIME_DZONE,(LPVOID)strBuf.GetString(),MELSEC_TACT_TIME_DZONE_LENGTH,TRUE);
	}
	
}
/*//2018-03-12,MGYUN, CONTACT (실시간,일(이전일),주,월 기록)
void CMonitorBank::WriteContactInfo(double dContactRate)
{
	CString strBuf;
	strBuf.Format(_T("%d"), dContactRate);
	
	//실시간
	theDeviceMelsec.UpperWordSend(MELSEC_CONTACT_RATE,(LPVOID)strBuf.GetString(),MELSEC_CONTACT_RATE_LENGTH,TRUE);
}*/

//2018-03-12,MGYUN, 생산량 (실시간,일(이전일),주,월 기록)
/*void CMonitorBank::WriteProductInfo(int nProductCount)
{
	CString strBuf;
	strBuf.Format(_T("%d"), nProductCount);
	
	//실시간
	theDeviceMelsec.UpperWordSend(MELSEC_PRODUCTION_QUANTITY,(LPVOID)strBuf.GetString(),MELSEC_PRODUCTION_QUANTITY_LENGTH,TRUE);
}*/
//2018-03-12,MGYUN, 상류 설비로 부터 지연 시간.(초 단위) / 사용자가 STOP 눌러 PAUSE 전환 시 초기화 해야 되는지?
void CMonitorBank::WriteInDelay(int nDelay)
{
	CString strBuf;
	strBuf.Format(_T("%d"), nDelay);

	
	theDeviceMelsec.UpperWordSend(MELSEC_IN_DELAY,(LPVOID)strBuf.GetString(),MELSEC_IN_DELAY_LENGTH,TRUE);

}
//2018-03-12,MGYUN, AGV로 부터 IN 지연 시간.(초 단위)
void CMonitorBank::WriteAGVInDelay(int nDelay)
{
	/*
		조건 : GOOD IN BUF, GOOD IN LIFT 둘다 비었을 때 GOOD IN EMPTY STEP = 3 일때 호출.
	*/
	CString strBuf;
	strBuf.Format(_T("%d"), nDelay);

	
	theDeviceMelsec.UpperWordSend(MELSEC_AGV_IN_DELAY,(LPVOID)strBuf.GetString(),MELSEC_AGV_IN_DELAY_LENGTH,TRUE);

}
//2018-03-12,MGYUN, AGV로 부터 OUT 지연 시간.(초 단위)
void CMonitorBank::WriteAGVOutDelay(int nDelay)
{
	/*
		조건 : GOOD OUT BUF, GOOD OUT LIFT 둘다 FULL 이였을 때
	*/
	CString strBuf;
	strBuf.Format(_T("%d"), nDelay);

	
	theDeviceMelsec.UpperWordSend(MELSEC_AGV_OUT_DELAY,(LPVOID)strBuf.GetString(),MELSEC_AGV_OUT_DELAY_LENGTH,TRUE);

}

//2018-03-13,MGYUN, CELL 관련 항목 기록 함수. (셀 하나에 대한 기록인지 아니면 두개다 기록인지? 물어볼 것)
void CMonitorBank::WriteCellItemCH1(CELL_POS Pos)
{
	
	CString strCellInfoBuf;
	
	CCellInfo *pCellInfo = NULL;	
	pCellInfo = theCellBank.GetCellInfo(Pos);
	
	if(pCellInfo == NULL)
		return;

	// INNER ID
	strCellInfoBuf.Format(_T("%s"),pCellInfo->m_strInnerID);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_INNER_ID,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_INNER_ID_LENGTH,TRUE);

	// MODEL
	strCellInfoBuf.Format(_T("%s"),theRecipeBank.m_strRecipeName);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_MODEL,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_MODEL_LENGTH,TRUE);

	// JIG NAME
	strCellInfoBuf.Format(_T("%d"),pCellInfo->m_JigId);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_JIG_NAME,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_JIG_NAME_LENGTH,TRUE);

	// CH NAME
	strCellInfoBuf.Format(_T("%d"),pCellInfo->m_JigCh);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_CH_NAME,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_CH_NAME_LENGTH,TRUE);
	
	// B ZONE ALIGN RESULT
	//if(pCellInfo->m_Zone_B.m_nVI_AlignResult == )
	if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_GOOD)
	{
		strCellInfoBuf.Format(_T("%d"), 1 );
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_ALIGN_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_ALIGN_RESULT_LENGTH,TRUE);
	}
	else if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_FAIL)
	{
		strCellInfoBuf.Format(_T("%d"), 0 );
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_ALIGN_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_ALIGN_RESULT_LENGTH,TRUE);
	}
	else
	{
		strCellInfoBuf.Format(_T("%d"), 0 );
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_ALIGN_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_ALIGN_RESULT_LENGTH,TRUE);
	}

	// CONTACT RESULT
	if(pCellInfo->m_strContactResult == "GOOD")
	{
		strCellInfoBuf.Format(_T("%d"), 1);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_CONTACT_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_CONTACT_RESULT_LENGTH,TRUE);
	}
	else if(pCellInfo->m_strContactResult == "NG")
	{
		strCellInfoBuf.Format(_T("%d"), 0);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_CONTACT_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_CONTACT_RESULT_LENGTH,TRUE);
	}
	else
	{
		strCellInfoBuf.Format(_T("%d"), 0);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_CONTACT_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_CONTACT_RESULT_LENGTH,TRUE);
	}
	
	// VERSION
	strCellInfoBuf.Format(_T("%s"),theApp.ReadVersion());
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_VERSION,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_VERSION_LENGTH,TRUE);
	
	// TMD NAME
	strCellInfoBuf.Format(_T("%s"),pCellInfo->m_Zone_A.m_strTMD_Info);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_TMD_NAME,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_TMD_NAME_LENGTH,TRUE);

	// INSP TACT TIME
	strCellInfoBuf.Format(_T("%s"),theProcBank.m_strCZoneTack);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_INSP_TACT_TIME,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_INSP_TACT_TIME_LENGTH,TRUE);
	
	// INSP RESULT
	if(pCellInfo->m_Zone_C.VIClass == GOOD_CELL)
	{
		strCellInfoBuf.Format(_T("%d"), 1);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_INSP_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_INSP_RESULT_LENGTH,TRUE);
	}
	else if(pCellInfo->m_Zone_C.VIClass == REJECT_CELL)
	{
		strCellInfoBuf.Format(_T("%d"), 0);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_INSP_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_INSP_RESULT_LENGTH,TRUE);
	}
	else
	{
		strCellInfoBuf.Format(_T("%d"), 0);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_INSP_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_INSP_RESULT_LENGTH,TRUE);
	}
	
	// INSP CODE
	//검사 결과 코드 넣어야 함.
	strCellInfoBuf.Format(_T("%s"),pCellInfo->MesCode);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH1_INSP_CODE,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH1_INSP_CODE_LENGTH,TRUE);
}

//2018-03-13,MGYUN, CELL 관련 항목 기록 함수. (셀 하나에 대한 기록인지 아니면 두개다 기록인지? 물어볼 것)
void CMonitorBank::WriteCellItemCH2(CELL_POS Pos)
{
	CString strCellInfoBuf;
	
	CCellInfo *pCellInfo = NULL;	
	pCellInfo = theCellBank.GetCellInfo(Pos);

	if(pCellInfo == NULL)
		return;

	// INNER ID
	strCellInfoBuf.Format(_T("%s"),pCellInfo->m_strInnerID);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_INNER_ID,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_INNER_ID_LENGTH,TRUE);

	// MODEL
	strCellInfoBuf.Format(_T("%s"),theRecipeBank.m_strRecipeName);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_MODEL,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_MODEL_LENGTH,TRUE);

	// JIG NAME
	strCellInfoBuf.Format(_T("%d"),pCellInfo->m_JigId);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_JIG_NAME,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_JIG_NAME_LENGTH,TRUE);

	// CH NAME
	strCellInfoBuf.Format(_T("%d"),pCellInfo->m_JigCh);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_CH_NAME,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_CH_NAME_LENGTH,TRUE);
	
	//2018-03-15,MGYUN, B ZONE ALIGN RESULT
	//if(pCellInfo->m_Zone_B.m_nVI_AlignResult == )
	if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_GOOD)
	{
		strCellInfoBuf.Format(_T("%d"), 1 );
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_ALIGN_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_ALIGN_RESULT_LENGTH,TRUE);
	}
	else if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_FAIL)
	{
		strCellInfoBuf.Format(_T("%d"), 0 );
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_ALIGN_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_ALIGN_RESULT_LENGTH,TRUE);
	}
	else
	{
		strCellInfoBuf.Format(_T("%d"), 0 );
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_ALIGN_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_ALIGN_RESULT_LENGTH,TRUE);
	}

	// CONTACT RESULT
	if(pCellInfo->m_strContactResult == "GOOD")
	{
		strCellInfoBuf.Format(_T("%d"), 1);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_CONTACT_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_CONTACT_RESULT_LENGTH,TRUE);
	}
	else if(pCellInfo->m_strContactResult == "NG")
	{
		strCellInfoBuf.Format(_T("%d"), 0);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_CONTACT_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_CONTACT_RESULT_LENGTH,TRUE);
	}
	else
	{
		strCellInfoBuf.Format(_T("%d"), 0);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_CONTACT_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_CONTACT_RESULT_LENGTH,TRUE);
	}
	
	// VERSION
	strCellInfoBuf.Format(_T("%s"),theApp.ReadVersion());
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_VERSION,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_VERSION_LENGTH,TRUE);
	
	// TMD NAME
	strCellInfoBuf.Format(_T("%s"),pCellInfo->m_Zone_A.m_strTMD_Info);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_TMD_NAME,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_TMD_NAME_LENGTH,TRUE);

	// INSP TACT TIME
	strCellInfoBuf.Format(_T("%s"),theProcBank.m_strCZoneTack);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_INSP_TACT_TIME,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_INSP_TACT_TIME_LENGTH,TRUE);
	
	// INSP RESULT
	if(pCellInfo->m_Zone_C.VIClass == GOOD_CELL)
	{
		strCellInfoBuf.Format(_T("%d"), 1);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_INSP_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_INSP_RESULT_LENGTH,TRUE);
	}
	else if(pCellInfo->m_Zone_C.VIClass == REJECT_CELL)
	{
		strCellInfoBuf.Format(_T("%d"), 0);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_INSP_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_INSP_RESULT_LENGTH,TRUE);
	}
	else
	{
		strCellInfoBuf.Format(_T("%d"), 0);
		theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_INSP_RESULT,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_INSP_RESULT_LENGTH,TRUE);
	}
	// INSP CODE
	//검사 결과 코드 넣어야 함.
	strCellInfoBuf.Format(_T("%d"),pCellInfo->MesCode);
	theDeviceMelsec.UpperWordSend(MELSEC_CELL_CH2_INSP_CODE,(LPVOID)strCellInfoBuf.GetString(),MELSEC_CELL_CH2_INSP_CODE_LENGTH,TRUE);
}


//2018-03-14,MGYUN, 알람, 알람원인 MELSEC 기록 함수
void CMonitorBank::WriteAlarm(int nAlarmCnt, int nAlarmCode)
{
	//알람 코드
	CString strAlarmBuf;
	short sMelsec_Length = MELSEC_ALARM_CODE;
	
	strAlarmBuf.Format(_T("%04d"), nAlarmCode);
	sMelsec_Length = sMelsec_Length + (nAlarmCnt * 2); //들어온 갯수만큼 2크기씩 늘림
	
	//50보다 크면 0으로 초기화 해준다.
	if(nAlarmCnt > 50)
		nAlarmCnt = 0;
	
	theDeviceMelsec.UpperWordSend(sMelsec_Length,(LPVOID)strAlarmBuf.GetString(),2,TRUE);
	
}
//2018-03-14,MGYUN, 클리어 시 0으로 초기화 함수.
void CMonitorBank::WriteAlarmClear(int nAlarmCnt)
{
	//알람 코드
	CString strAlarmBuf;
	int nClearAlarmCode = 0;		// AlarmDefine.h 에 있는 0은 알람이 아니므로 0으로 초기화.
	short sMelsec_Length = MELSEC_ALARM_CODE;
	
	strAlarmBuf.Format(_T("%04d"), nClearAlarmCode);
	sMelsec_Length = sMelsec_Length + (nAlarmCnt * 2); //들어온 갯수만큼 2크기씩 늘림
	
	//50보다 크면 0으로 초기화 해준다.
	if(nAlarmCnt > 50)
		nAlarmCnt = 0;
	
	theDeviceMelsec.UpperWordSend(sMelsec_Length,(LPVOID)strAlarmBuf.GetString(),2,TRUE);
	
}
//2018-03-14,MGYUN, 고객 요청 사항 FDC STATUS 0 : 알람 미 발생, 1 :  알람 발생
void CMonitorBank::WriteFDCAlarmStatus(int nAlarmStatus)
{
	CString strBuf;
	strBuf.Format(_T("%d"), nAlarmStatus );
	
	theDeviceMelsec.UpperWordSend(MELSEC_FDC_ALARM_STATUS,(LPVOID)strBuf.GetString(),MELSEC_FDC_ALARM_STATUS_LENGTH,TRUE);
	theDeviceMelsec.UpperWordSend(MELSEC_FDC_ALARM_CASUE,(LPVOID)strBuf.GetString(),MELSEC_FDC_ALARM_CASUE_LENGTH,TRUE);
}

//2018-03-14,MGYUN, 해당 알람의 원인(Text)
void CMonitorBank::WriteAlarmCause(int nAlarmCnt, CString strAlarmCause)
{
	//알람 코드
	CString strAlarmCauseBuf;
	short sMelsec_Cause_Length = MELSEC_ALARM_CAUSE_CODE;
	
	strAlarmCauseBuf.Format(_T("%s"), strAlarmCause);
	sMelsec_Cause_Length = sMelsec_Cause_Length + (nAlarmCnt * 2); //들어온 갯수만큼 2크기씩 늘림

	//50보다 크면 0으로 초기화 해준다.
	if(nAlarmCnt > 50)
		nAlarmCnt = 0;

	theDeviceMelsec.UpperWordSend(sMelsec_Cause_Length,(LPVOID)strAlarmCauseBuf.GetString(),20,TRUE);
}

//2018-03-14,MGYUN, 해당 알람의 원인(Text)
void CMonitorBank::WriteAlarmCauseClear(int nAlarmCnt)
{
	//알람 코드
	CString strAlarmCauseBuf;
	CString strClear = _T("");
	//int nClearAlarmCode = 0;		// AlarmDefine.h 에 있는 0은 알람이 아니므로 0으로 초기화.

	short sMelsec_Length = MELSEC_ALARM_CAUSE_CODE;
	
	strAlarmCauseBuf.Format(_T("%d"), strClear);
	sMelsec_Length = sMelsec_Length + (nAlarmCnt * 2); //들어온 갯수만큼 2크기씩 늘림
	
	//50보다 크면 0으로 초기화 해준다.
	if(nAlarmCnt > 50)
		nAlarmCnt = 0;
	
	theDeviceMelsec.UpperWordSend(sMelsec_Length,(LPVOID)strAlarmCauseBuf.GetString(),20,TRUE);
}
/*
	0 : A ZONE
	1 : B ZONE
	2 : C ZONE
	3 : D ZONE
*/
//2018-03-15,MGYUN, 룰렛 정보(턴테이블 위치) 기록 함수
void CMonitorBank::WriteTurnTablePos()
{
	CString strBuf;
	if(theProcBank.m_nInspTablePos == CONST_TABLE_POS::POS_1)
	{
		strBuf.Format(_T("%d"),0);
		theDeviceMelsec.UpperWordSend(MELSEC_TURNTABLE_POS,(LPVOID)strBuf.GetString(),MELSEC_TURNTABLE_POS_LENGTH,TRUE);
	}
	else if(theProcBank.m_nInspTablePos == CONST_TABLE_POS::POS_2)
	{
		strBuf.Format(_T("%d"),1);
		theDeviceMelsec.UpperWordSend(MELSEC_TURNTABLE_POS,(LPVOID)strBuf.GetString(),MELSEC_TURNTABLE_POS_LENGTH,TRUE);
	}
	else if(theProcBank.m_nInspTablePos == CONST_TABLE_POS::POS_3)
	{
		strBuf.Format(_T("%d"),2);
		theDeviceMelsec.UpperWordSend(MELSEC_TURNTABLE_POS,(LPVOID)strBuf.GetString(),MELSEC_TURNTABLE_POS_LENGTH,TRUE);
	}
	else
	{
		strBuf.Format(_T("%d"),3);
		theDeviceMelsec.UpperWordSend(MELSEC_TURNTABLE_POS,(LPVOID)strBuf.GetString(),MELSEC_TURNTABLE_POS_LENGTH,TRUE);
	}
}
//2018-03-15,MGYUN, 지그별 스킵 상태 기록
void CMonitorBank::WriteJigSkipStatus()
{
	/*
		0 : NO NKIP
		1 : SKIP 
	*/
	CString strASkip;
	CString strBSkip;
	CString strCSkip;
	CString strDSkip;
	// 전체 스킵 아니면
	if (theConfigBank.m_Option.m_bUseASkip == FALSE
			&& theConfigBank.m_Option.m_bUseBSkip == FALSE
			&& theConfigBank.m_Option.m_bUseCSkip == FALSE
			&& theConfigBank.m_Option.m_bUseDSkip == FALSE)
		{
			strASkip.Format(_T("%d"),0);
			strBSkip.Format(_T("%d"),0);
			strCSkip.Format(_T("%d"),0);
			strDSkip.Format(_T("%d"),0);

			theDeviceMelsec.UpperWordSend(MELSEC_AJIG_SKIP_STATUS,(LPVOID)strASkip.GetString(),MELSEC_AJIG_SKIP_STATUS_LENGTH,TRUE);
			theDeviceMelsec.UpperWordSend(MELSEC_BJIG_SKIP_STATUS,(LPVOID)strBSkip.GetString(),MELSEC_BJIG_SKIP_STATUS_LENGTH,TRUE);
			theDeviceMelsec.UpperWordSend(MELSEC_CJIG_SKIP_STATUS,(LPVOID)strCSkip.GetString(),MELSEC_CJIG_SKIP_STATUS_LENGTH,TRUE);
			theDeviceMelsec.UpperWordSend(MELSEC_DJIG_SKIP_STATUS,(LPVOID)strDSkip.GetString(),MELSEC_DJIG_SKIP_STATUS_LENGTH,TRUE);
		}
		
		JIG_ID JigID;
		JigID = theUnitFunc.AMT_GetAutoJigID(ZONE_ID_A);	
		//A JIG SKIP ?
		if (theConfigBank.m_Option.m_bUseASkip)
		{
			//if (JigID == JIG_ID_A)
			//{
				strASkip.Format(_T("%d"),1);
				theDeviceMelsec.UpperWordSend(MELSEC_AJIG_SKIP_STATUS,(LPVOID)strASkip.GetString(),MELSEC_AJIG_SKIP_STATUS_LENGTH,TRUE);
			//}
			
		}
		//A JIG SKIP 아니면
		else
		{
			strASkip.Format(_T("%d"),0);
			theDeviceMelsec.UpperWordSend(MELSEC_AJIG_SKIP_STATUS,(LPVOID)strASkip.GetString(),MELSEC_AJIG_SKIP_STATUS_LENGTH,TRUE);
		}
		//B JIG SKIP ?
		if (theConfigBank.m_Option.m_bUseBSkip)
		{
			//if (JigID == JIG_ID_B)
			//{
				strBSkip.Format(_T("%d"),1);
				theDeviceMelsec.UpperWordSend(MELSEC_BJIG_SKIP_STATUS,(LPVOID)strBSkip.GetString(),MELSEC_BJIG_SKIP_STATUS_LENGTH,TRUE);
			//}
		}
		//B JIG SKIP 아니면
		else
		{
			strBSkip.Format(_T("%d"),0);
			theDeviceMelsec.UpperWordSend(MELSEC_BJIG_SKIP_STATUS,(LPVOID)strBSkip.GetString(),MELSEC_BJIG_SKIP_STATUS_LENGTH,TRUE);
		}
		//C JIG SKIP?
		if (theConfigBank.m_Option.m_bUseCSkip)
		{
			//if (JigID == JIG_ID_C)
			//{
				strCSkip.Format(_T("%d"),1);
				theDeviceMelsec.UpperWordSend(MELSEC_CJIG_SKIP_STATUS,(LPVOID)strCSkip.GetString(),MELSEC_CJIG_SKIP_STATUS_LENGTH,TRUE);
			//}
			
		}
		else
		{
			strCSkip.Format(_T("%d"),0);
			theDeviceMelsec.UpperWordSend(MELSEC_CJIG_SKIP_STATUS,(LPVOID)strCSkip.GetString(),MELSEC_CJIG_SKIP_STATUS_LENGTH,TRUE);
		}
		//D JIG SKIP?
		if (theConfigBank.m_Option.m_bUseDSkip)
		{
			//if (JigID == JIG_ID_D)
			//{
				strDSkip.Format(_T("%d"),1);
				theDeviceMelsec.UpperWordSend(MELSEC_DJIG_SKIP_STATUS,(LPVOID)strDSkip.GetString(),MELSEC_DJIG_SKIP_STATUS_LENGTH,TRUE);
			//}
		}
		//D JIG SKIP 아니면
		else
		{
			strDSkip.Format(_T("%d"),0);
			theDeviceMelsec.UpperWordSend(MELSEC_DJIG_SKIP_STATUS,(LPVOID)strDSkip.GetString(),MELSEC_DJIG_SKIP_STATUS_LENGTH,TRUE);
		}

}

//2018-03-14,MGYUN, 인터락 이름 멜섹 기록 함수
void CMonitorBank::WriteInterLock(int nInterLockCnt, CString strInterlockName)
{
	/*
	
	CString strAlarmBuf;
	short sMelsec_Length = MELSEC_ALARM_CODE;
	
	strAlarmBuf.Format(_T("%04d"), nAlarmCode);
	sMelsec_Length = sMelsec_Length + (nAlarmCnt * 2); //들어온 갯수만큼 2크기씩 늘림
	
	//50보다 크면 0으로 초기화 해준다.
	if(nAlarmCnt > 50)
		nAlarmCnt = 0;
	
	theDeviceMelsec.UpperWordSend(sMelsec_Length,(LPVOID)strAlarmBuf.GetString(),2,TRUE);
	*/
	//인터락 이름.(CString 으로)
	CString strInterLockBuf;
	short sMelsec_Length = MELSEC_INTERLOCK_NAME;
	
	strInterLockBuf.Format(_T("%s"), strInterlockName);
	sMelsec_Length = sMelsec_Length + (nInterLockCnt * 2); //들어온 갯수만큼 2크기씩 늘림
	if(nInterLockCnt > 50)
		nInterLockCnt = 0;
	
	theDeviceMelsec.UpperWordSend(sMelsec_Length,(LPVOID)strInterLockBuf.GetString(),MELSEC_INTERLOCK_NAME_LENGTH,TRUE);
	
}

//2018-03-14,MGYUN, 인터락 상태 기록 함수
void CMonitorBank::WriteInterLockStatus(int nInterLockStatus)
{
	//인터락 상태
	//0 : 미 발생 , 1 : 발생
	CString strInterLockStatusBuf;
	strInterLockStatusBuf.Format(_T("%04d"), nInterLockStatus);

	theDeviceMelsec.UpperWordSend(MELSEC_INTERLOCK_STATUS,(LPVOID)strInterLockStatusBuf.GetString(),MELSEC_INTERLOCK_STATUS_LENGTH,TRUE);
	
}

//2018-03-15,NAM, Write Contact for 1 hour ago
void CMonitorBank::WritePreContact(CString strContact)
{
	CString strBuf;
	strBuf.Format(_T("%s"), strContact);
	theDeviceMelsec.UpperWordSend(MELSEC_CONTACT_RATE_DAY,(LPVOID)strBuf.GetString(),MELSEC_CONTACT_RATE_DAY_LENGTH,TRUE);
}

//2018-03-15,NAM, Write Contact (Up to Date)
void CMonitorBank::WriteDayContact(CString strContact)
{
	CString strDayContact;
	strDayContact.Format(_T("%s"),strContact);
	theDeviceMelsec.UpperWordSend(MELSEC_CONTACT_RATE,(LPVOID)strDayContact.GetString(),MELSEC_CONTACT_RATE_LENGTH,TRUE);
}

//2018-03-15,NAM, Write Contact for Week  ( Starting from Monday until now) 
void CMonitorBank::WriteWeekContact(CString strContact)
{
	CString strWeekContact;
	strWeekContact.Format(_T("%s"),strContact);
	theDeviceMelsec.UpperWordSend(MELSEC_CONTACT_RATE_WEEK,(LPVOID)strWeekContact.GetString(),MELSEC_CONTACT_RATE_WEEK_LENGTH,TRUE);
}

//2018-03-15,NAM, Write Contact for Month   ( Starting from 1st day of month until now) 
void CMonitorBank::WriteMonthContact(CString strContact)
{
	CString strMonthContact;
	strMonthContact.Format(_T("%s"),strContact);
	theDeviceMelsec.UpperWordSend(MELSEC_CONTACT_RATE_MONTH,(LPVOID)strMonthContact.GetString(),MELSEC_CONTACT_RATE_MONTH_LENGTH,TRUE);
}

//2018-03-16,NAM, Write Production Quantity of 1 hour ago of today to Device Monitoring  
void CMonitorBank::WritePreProduct(int nProductQuant)
{
	CString strPreProduct;
	strPreProduct.Format(_T("%d"),nProductQuant);
	theDeviceMelsec.UpperWordSend(MELSEC_PRODUCTION_QUANTITY_DAY,(LPVOID)strPreProduct.GetString(),MELSEC_PRODUCTION_QUANTITY_DAY_LENGTH,TRUE);
}

//2018-03-16,NAM, Write Up to date Production Quantity to Device Monitoring  
void CMonitorBank::WriteDayProduct(int nProductQuant)
{
	CString strDayProduct;
	strDayProduct.Format(_T("%d"),nProductQuant);
	theDeviceMelsec.UpperWordSend(MELSEC_PRODUCTION_QUANTITY,(LPVOID)strDayProduct.GetString(),MELSEC_PRODUCTION_QUANTITY_LENGTH,TRUE);
}

//2018-03-16,NAM, Write Production Quantity of 1 week to Device Monitoring (Starting from Monday until present)
void CMonitorBank::WriteWeekProduct(int nProductQuant)
{
	CString strWeekProduct;
	strWeekProduct.Format(_T("%d"),nProductQuant);
	theDeviceMelsec.UpperWordSend(MELSEC_PRODUCTION_QUANTITY_WEEK,(LPVOID)strWeekProduct.GetString(),MELSEC_PRODUCTION_QUANTITY_WEEK_LENGTH,TRUE);
}

//2018-03-16,NAM, Write Production Quantity of 1 month to Device Monitoring (Starting from 1st day of month until present)
void CMonitorBank::WriteMonthProduct(int nProductQuant)
{
	CString strMonthProduct;
	strMonthProduct.Format(_T("%d"),nProductQuant);
	theDeviceMelsec.UpperWordSend(MELSEC_PRODUCTION_QUANTITY_MONTH,(LPVOID)strMonthProduct.GetString(),MELSEC_PRODUCTION_QUANTITY_MONTH_LENGTH,TRUE);
}