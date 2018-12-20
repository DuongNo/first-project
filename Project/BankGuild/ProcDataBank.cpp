#include "StdAfx.h"
#include "ProcDataBank.h"
#include "CheetahApp.h"
#include "UI\GausGUI\GxMsgBox.h"

#include "GUIDefine.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

#include "UI\Dialog\DlgMsgTerminal.h"
#include "UI\Dialog\DlgMsgOPCall.h"
#include "UI\Dialog\DlgMsgInterlock.h"
#include "UI\Dialog\DlgMsgInterlock2.h"
#include "File\GxIniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const CONST_MACHINE_STATE	ConstMachineState[] =
{
	_T("PAUSE"),
	_T("INIT"),
	_T("CHECK"),  //2018-04-20,SKCHO,추가
	_T("IDLE"),
	_T("RUN"),
	_T("FAULT")
};

const CONST_CIM_STATE	ConstCimState[] =
{
	_T("OFFLINE"),
	_T("REMOTE"),
	_T("LOCAL")
};

const CONST_EQUIP_PART		ConstEquipPart[] =
{
	{	_T("로더부"),			_T("LOAD"),			_T("LOAD")},
	{	_T("검사부"),			_T("INSP"),			_T("INSP")},
	{	_T("언로더부"),			_T("UNLOAD"),		_T("UNLOAD")},
	{	_T("기타"),				_T("ETC."),			_T("ETC.")}
};

const CONST_EQUIP_UNIT		ConstEquipUnit[] =
{
	{	_T("EQUIP_UNIT_LOAD_CONV"			),		_T("EQUIP_UNIT_LOAD_CONV"			), _T("EQUIP_UNIT_LOAD_CONV"			)	},
	{	_T("EQUIP_UNIT_LOAD_ROBOT"			),		_T("EQUIP_UNIT_LOAD_CONV"			), _T("EQUIP_UNIT_LOAD_CONV"			)	},
	{	_T("EQUIP_UNIT_UPPER_SHUTTLE"		),		_T("EQUIP_UNIT_UPPER_SHUTTLE"		), _T("EQUIP_UNIT_UPPER_SHUTTLE"		)	},
	{	_T("EQUIP_UNIT_LOWER_SHUTTLE"		),		_T("EQUIP_UNIT_LOWER_SHUTTLE"		), _T("EQUIP_UNIT_LOWER_SHUTTLE"		)	},
	{	_T("EQUIP_UNIT_PA_TR"				),		_T("EQUIP_UNIT_PA_TR"				), _T("EQUIP_UNIT_PA_TR"				)	},
	{	_T("EQUIP_UNIT_PA_STAGE"			),		_T("EQUIP_UNIT_PA_STAGE"			), _T("EQUIP_UNIT_PA_STAGE"				)	},
	{	_T("EQUIP_UNIT_LD_TR"				),		_T("EQUIP_UNIT_LD_TR"				), _T("EQUIP_UNIT_LD_TR"				)	},
	{	_T("EQUIP_UNIT_AZONE"				),		_T("EQUIP_UNIT_AZONE"				), _T("EQUIP_UNIT_AZONE"				)	},
	{	_T("EQUIP_UNIT_BZONE"				),		_T("EQUIP_UNIT_BZONE"				), _T("EQUIP_UNIT_BZONE"				)	},
	{	_T("EQUIP_UNIT_CZONE"				),		_T("EQUIP_UNIT_CZONE"				), _T("EQUIP_UNIT_CZONE"				)	},
	{	_T("EQUIP_UNIT_DZONE"				),		_T("EQUIP_UNIT_DZONE"				), _T("EQUIP_UNIT_DZONE"				)	},
	{	_T("EQUIP_UNIT_TABLE"				),		_T("EQUIP_UNIT_TABLE"				), _T("EQUIP_UNIT_TABLE"				)	},
	{	_T("EQUIP_UNIT_UD_TR"				),		_T("EQUIP_UNIT_UD_TR"				), _T("EQUIP_UNIT_UD_TR"				)	},
	{	_T("EQUIP_UNIT_UD_STAGE"			),		_T("EQUIP_UNIT_UD_STAGE"			), _T("EQUIP_UNIT_UD_STAGE"				)	},
	{	_T("EQUIP_UNIT_UD_ROBOT"			),		_T("EQUIP_UNIT_UD_ROBOT"			), _T("EQUIP_UNIT_UD_ROBOT"				)	},
	{	_T("EQUIP_UNIT_GOOD_TRAY_TR"		),		_T("EQUIP_UNIT_GOOD_TRAY_TR"		), _T("EQUIP_UNIT_GOOD_TRAY_TR"			)	},
	{	_T("EQUIP_UNIT_NG_TRAY_TR"			),		_T("EQUIP_UNIT_NG_TRAY_TR"			), _T("EQUIP_UNIT_NG_TRAY_TR"			)	},
	{	_T("EQUIP_UNIT_GOOD_TRAY_IN"		),		_T("EQUIP_UNIT_GOOD_TRAY_IN"		), _T("EQUIP_UNIT_GOOD_TRAY_IN"			)	},
	{	_T("EQUIP_UNIT_GOOD_TRAY_OUT"		),		_T("EQUIP_UNIT_GOOD_TRAY_OUT"		), _T("EQUIP_UNIT_GOOD_TRAY_OUT"		)	},
	{	_T("EQUIP_UNIT_NG_TRAY_IN"			),		_T("EQUIP_UNIT_NG_TRAY_IN"			), _T("EQUIP_UNIT_NG_TRAY_IN"			)	},
	{	_T("EQUIP_UNIT_NG_TRAY_OUT"			),		_T("EQUIP_UNIT_NG_TRAY_OUT"			), _T("EQUIP_UNIT_NG_TRAY_OUT"			)	},
};

CProcDataBank::CProcDataBank(void)
{
	m_MachineState = CONST_MACHINE_STATE::MACHINE_FAULT;
	m_CimState = CONST_CIM_STATE::CIM_OFFLINE;
	m_strRCMD = _T("");
	m_bProgramInitFinish = FALSE;
	ResetTemporarilyValue();

	m_nIDGenIndex = 1;
	//20170118 byskcho 리소스 스트링에서 관리하므로 삭제
	///m_strOperationVersion = PROGRAM_VERSION;
	m_bOriginSuccess = FALSE;
	GOODTrayBitX = 1;
	GOODTrayBitY = 1;
	NGTrayBitX = 1;
	NGTrayBitY = 1;
	ResetAlignData();
	ResetLossData();
	ResetPortData();

	m_MsgOpCallCnt = 0;
	m_MsgInterlockCnt = 0;
	m_strOldInterlockID = _T("");

	m_bUserStop = FALSE;

	m_bGoodTrayReq = FALSE;
	m_bNGTrayReq = FALSE;

	m_bGoodTrayInSWReq = FALSE;
	m_bGoodTrayOutSWReq = FALSE;
	m_bNGTrayInSWReq = FALSE;
	m_bNGTrayOutSWReq = FALSE;
	m_bGoodTrayInMuteSWChk = FALSE;
	m_bGoodTrayOutMuteSWChk = FALSE;
	m_bNGTrayMuteSWChk = FALSE;

	m_bLowerPutCompInterlock = FALSE;

	m_bLowerPutComp = FALSE;
	m_bUpperPutComp = FALSE;

	m_bDCRChk = TRUE;

	m_bGoodTrayAlignChk = TRUE;
	m_bNGTrayAlignChk = TRUE;

	m_bFirstLoadTactStart = TRUE;
	m_bFirstUnloadTactStart = TRUE;

	m_CellLoadChk = FALSE;
	m_CellLoadRunChk = FALSE;
	// 2017.2.3 bgkim
	m_bGoodInTrayMuteOn = FALSE;
	m_bGoodOutTrayMuteOn = FALSE;
	m_bNGTrayMuteOn = FALSE;

	memset(PRE_ALARM_ID, FALSE, sizeof(BOOL)*ALM_MAX); // 2017.2.6 bgkim

	m_bOKTrayTRLotEnd = FALSE; // 2017.2.6 bgkim
	m_bNGTrayTRLotEnd = FALSE; // 2017.2.6 bgkim

	// 2017.2.9 bgkim
	m_bDZoneTT = TRUE;

	m_bUpperMCRCompChk = TRUE;

	m_nAlarmCnt = 0;  //2017-03-06, skcho, 알람 카운트

	m_nGoodAlignCount = 0;
	m_nSubGoodAlignCount = 0;

	m_nTrayVICount = 0;
	m_bTrayVINGRetry = FALSE;

	ResetSubCarrierID();  //2017-03-22, skcho carrier id 초기화

	m_bCarrierAssign = FALSE;

	m_bTrayCloseRelease = FALSE;  //2017-03-25, 뚜껑 덮기 보고 변수

	m_bCarrierReportWaitCheck = FALSE;

	m_nGoodOutLiftCount = 0;

	m_nPre_GoodOutLiftCount = 0;

	ManualMCRReConnect = FALSE;

	m_bTRAYInChk = FALSE;
	m_bTRAYOutChk = FALSE;

	for (int i = JIG_ID_A; i < JIG_ID_MAX; i++)
	{
		for (int j = JIG_CH_1; j < JIG_CH_MAX; j++)
		{
			m_nVisionNGCnt[i][j];
			m_bVisionNG[i][j] = FALSE;
		}
	}

	for (int i = 0; i < 8; i++)
		m_bChSkip[i] = FALSE;

	m_nChSkipCount = 0;
	m_nChSecond = JIG_CH_1;
	m_nChSkipJigID = JIG_ID_A;

	m_bLightAlarmChk = FALSE;

	m_bTrayCellFullChk = FALSE;

	m_bReleaseTrayChk = FALSE;

	m_nLotCellCnt = 0;

	m_bShowCh1TrackInChk = FALSE;
	m_bShowCh2TrackInChk = FALSE;

	m_bMotorSetupChk = FALSE;

	m_bRCMDCh1Chk = FALSE;
	m_bRCMDCh2Chk = FALSE;

	m_bOriginAxisChk = FALSE;

	m_strMESRecipeName = _T("");

	for (int i = JIG_ID_A; i < JIG_ID_MAX; i++)
	{
		for (int j = JIG_CH_1; j < JIG_CH_MAX; j++)
		{
			m_JigContactNGcount[i][j] = 0;
		}
	}
	
////2017-08-01,JSJUNG
	m_JigContactNGcountMAX = 5;
	m_JigContactNGChName = 0;
	m_bJigContactNGFlag = FALSE;

	m_bTrayInStart = FALSE;
	BOOL GoodTrayInLiftingFlag = FALSE;
	
	m_bTrayOutStart = FALSE;
	GoodTrayOutAGVFlag = FALSE;

	m_nAGVDelayStep = 0;
	m_nOutAGVDelayStep = 0;

	m_nGoodInPortEmptyStep = 0;
	m_nNGInPortEmptyStep = 0;

	m_nCH1CellInfoRetryStep = 0;
	m_nCH2CellInfoRetryStep = 0;

	m_nGoodTrayRetryStep = 0;
	m_nGoodTrayRetryStep2 = 0;
	m_nNGTrayRetryStep = 0;

	//2017-10-07,SKCHO
	memset(m_bInPutIO, FALSE, sizeof(BOOL)*MAX_IN_ID);
	memset(m_bOutPutIO, FALSE, sizeof(BOOL)*MAX_OUT_ID);

	//20171108,JSJUNG
	m_bTRAYInIDLE = FALSE;
	m_bTRAYOutIDLE = FALSE;

//2017-11-01,SKCHO
	m_MsgUnitInterlockCnt = 0;
	m_strOldUnitInterlockID = _T("");

	m_cUnitModuleNum = 0x00;
	m_bUnitInterlock_LoadingStop = FALSE;

	//2017-11-15,SKCHO
	m_nGoodInPortLAGuidRetryStep = 0;

	m_bAllUnitInterlock_Stop = FALSE;

	m_bTrayVIResultRetry = FALSE;
	m_bTrayVICompRetry = FALSE;
	m_bTrayVIToRBRetry = FALSE;

	m_bPreAlignOrgStart = FALSE;

	m_InDelayTime = 0;
	nAGV_InDelayTime = 0;
	nAGV_OutDelayTime = 0;

	//2018-04-20,SKCHO
	for(int i = 0; i < 4; i++)
	{
		m_bFaultOfTray[i] = FALSE;
	}
	//2018-05-07,
	m_ChangeState1 = 0;
	//2018-05-22,MGYUN
	HybridDelayFlag = FALSE;
	//2018-06-21,NAM
	m_RestartState=FALSE;

	//2018-06-29,NAM
	m_newstateofHybrid=TRUE;
	m_oldstateofHybrid=TRUE;

	//2018wjp
	m_bInPick2Model=FALSE;
	m_bPickOUT2INTrayRequest=FALSE;
	m_bStageSeneorSingle=FALSE;
	i_ModelChange1To2=0;
	i_ModelChange2To1=0;
	m_bIsRestartDone=FALSE;
	m_bInitModelChange=FALSE;
	m_bGetCellIDComplate=FALSE;
	m_bTest=FALSE;
	m_bModel2FullTray=FALSE;
	for(int i=0;i<CELL_ID_ROW_MAX;i++)
	{
		for(int j=0;j<CELL_ID_COL_MAX;j++)
		{
			strCIMLoadCellID[i][j]="";
		}
	}
	Test_duong=FALSE;
}


CProcDataBank::~CProcDataBank(void)
{
}

void CProcDataBank::ChangeMachineState( CONST_MACHINE_STATE::ID newState )
{	
	if(m_MachineState != newState)
	{
		//2018-04-20,SKCHO, 장비상태저장
		m_OldMachineState = m_MachineState;

		//2017-09-08,SKCHO ,OLB RUNDOWN 추가
		if((m_MachineState == CONST_MACHINE_STATE::MACHINE_RUN || m_MachineState == CONST_MACHINE_STATE::MACHINE_PAUSE) && newState == CONST_MACHINE_STATE::MACHINE_IDLE)
		{
			theProcBank.m_InDelayTime = 0;
			::PostMessage( GetMainWnd(), GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_OLBDOWN, (LPARAM)1);
		}
		else if(m_MachineState == CONST_MACHINE_STATE::MACHINE_IDLE && newState == CONST_MACHINE_STATE::MACHINE_RUN)
		{
            theProcBank.m_InDelayTime = 0;
			theMonitorBank.WriteInDelay( theProcBank.m_InDelayTime);
			::PostMessage( GetMainWnd(), GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_Main, (LPARAM)0);
		}
		//2018wjp
		if(m_MachineState == CONST_MACHINE_STATE::MACHINE_PAUSE || m_MachineState == CONST_MACHINE_STATE::MACHINE_FAULT
			|| m_MachineState == CONST_MACHINE_STATE::MACHINE_INIT)
		{
			theProcBank.m_bIsRestartDone=FALSE;
		}
		theLog[LOG_SYSTEM].AddBuf(_T("State Change %s -> %s"), ConstMachineState[m_MachineState].strName, ConstMachineState[newState].strName);
		m_MachineState = newState;

		if(m_MachineState == CONST_MACHINE_STATE::MACHINE_FAULT)
		{		
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_ALARM,1);
		}
		else if(m_MachineState == CONST_MACHINE_STATE::MACHINE_INTERLOCK)
		{		
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_PM,1);
		}
		else if(m_MachineState == CONST_MACHINE_STATE::MACHINE_IDLE)
		{		
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_IDLE,0);
		}
		//2018-04-29,GHLEE, 추가
		else if(m_MachineState == CONST_MACHINE_STATE::MACHINE_CHECK)
		{		
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_CHECK,0);
		}
		else if(m_MachineState == CONST_MACHINE_STATE::MACHINE_RUN)
		{		
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_RUN,0);
		}
		else if(m_MachineState == CONST_MACHINE_STATE::MACHINE_PAUSE)
		{		
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_STOP,0);
		}
		else if(m_MachineState == CONST_MACHINE_STATE::MACHINE_INIT)
		{		
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_INIT,0);
		}

	
		if(GetMainHandler())
		{
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top)->m_hWnd, UDMSG_STATE_CHANGE, NULL, NULL);		// 연결 이벤트
		}

		//////////////////////////////////////////////////////////////////////////
		// CIM 설비 상태 변환 [11/28/2016 OSC]
		if ((theProcBank.m_bUserStop == TRUE) && (m_MachineState == CONST_MACHINE_STATE::MACHINE_PAUSE))
			return;

		//2018-04-20,SKCHO, MACHINE_CHECK는 보고 없이 RETURN;
		if(m_MachineState == CONST_MACHINE_STATE::MACHINE_CHECK)
			return;


		E_EQST_MATCH stateCIM = E_EQST_MATCH_NONE;
		switch(m_MachineState)
		{
			case CONST_MACHINE_STATE::MACHINE_PAUSE: stateCIM = E_EQST_MATCH_PAUSE; break;
			case CONST_MACHINE_STATE::MACHINE_INIT: stateCIM = E_EQST_MATCH_PAUSE; break;
			case CONST_MACHINE_STATE::MACHINE_IDLE: stateCIM = E_EQST_MATCH_IDLE; break;
			case CONST_MACHINE_STATE::MACHINE_RUN: stateCIM = E_EQST_MATCH_EXECUTING; break;
			case CONST_MACHINE_STATE::MACHINE_FAULT: stateCIM = E_EQST_MATCH_FAULT; break;
			case CONST_MACHINE_STATE::MACHINE_INTERLOCK: stateCIM = E_EQST_MATCH_INTERLOCK; break;
		}

		//2018-06-19,NAM,
		if ((stateCIM==E_EQST_MATCH_EXECUTING) && theProcBank.GetTrayAlarmChk())
		{
			theSocketInterFace.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, E_EQST_MATCH_FAULT);
		}
		else if(stateCIM != E_EQST_MATCH_NONE)
			theSocketInterFace.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, stateCIM);

	    //2018-03-07,SKCHO, MELSEC 데이터 추가
		theMonitorBank.WriteEqStatus((int)m_MachineState);
		
		//////////////////////////////////////////////////////////////////////////
	}
}

void CProcDataBank::ChangeCimState( CONST_CIM_STATE::ID newState )
{
	//if(m_MachineState != newState)
	if(m_CimState != newState)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("Cim State Change %s -> %s"), ConstCimState[m_CimState].strName, ConstCimState[newState].strName);
		m_CimState = newState;

		//2018-03-07,SKCHO, MELSEC 데이터 추가
		theMonitorBank.WriteCimStatus(m_CimState);
	}
}

void CProcDataBank::SetOPCall()
{
	theUnitFunc.SetOutPutIO(Y_BUZZER_K1, ON);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K2, OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K3, OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K4, OFF);

	CDlgMsgOPCall dlgMsgBox;

	BOOL ret = dlgMsgBox.DoModal();

	theSocketInterFace.SendCmdToDataPC(EQUIP_SET_OPCALL_EVENT);

	if(ret == TRUE)
	{
		theUnitFunc.SetOutPutIO(Y_BUZZER_K1, OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K2, OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K3, OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K4, OFF);
		// OPCALL시에는 멈추지 않는다. 2017/05/03 HSK.
//		ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
	}
/*
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(m_strOPCallID, m_strOPCallID, m_strOPCallID);
	dlgMsgBox.SetMessage(FALSE, 
		m_strOPCallMsg, 
		m_strOPCallMsg,		
		m_strOPCallMsg );

	BOOL ret = dlgMsgBox.DoModal();

	theSocketInterFace.SendCmdToDataPC(EQUIP_SET_OPCALL_EVENT);

	if(ret == TRUE)
	{
		theUnitFunc.SetOutPutIO(Y_BUZZER_K1, OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K2, OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K3, OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K4, OFF);

		ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
	}
*/
}

void CProcDataBank::SetInterlock()
{
	
	ChangeMachineState(CONST_MACHINE_STATE::MACHINE_INTERLOCK);


	CDlgMsgInterlock dlgMsgBox;

	BOOL ret = dlgMsgBox.DoModal();

	theSocketInterFace.SendCmdToDataPC(EQUIP_SET_INTERLOCK_EVENT);

	theProcBank.m_strOldInterlockID = _T("");

	if(ret == TRUE)
		ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);

	
	

/*	ChangeMachineState(CONST_MACHINE_STATE::MACHINE_INTERLOCK);

	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(m_strInterlockID, m_strInterlockID, m_strInterlockID);
	dlgMsgBox.SetMessage(FALSE, 
		m_strInterlockMsg, 
		m_strInterlockMsg,		
		m_strInterlockMsg );

	BOOL ret = dlgMsgBox.DoModal();

	theSocketInterFace.SendCmdToDataPC(EQUIP_SET_INTERLOCK_EVENT);

	if(ret == TRUE)
		ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
*/
}
//2017-12-05,SKCHO, unit 인터락  추가 514 보고
//514보고는 해당 채널에 대해서만 보고한다.
//COMFIRM 보고 시점이 유닛인터락 후 스킵옵션 적용전이면 정상 / 스킵옵션 적용 후이면 인터락으로 보고
void CProcDataBank::SetInterlock2(){

	CDlgMsgInterlock2 dlgMsgBox;
	CString strMsg;
	BOOL ret = dlgMsgBox.DoModal();

	if(m_nSkipChannel <= 2)
	{
		if(theConfigBank.m_Option.m_bUseASkip == TRUE)
		{
			strMsg = _T("E_EQST_MATCH_INTERLOCK");
		}
		else
		{
			strMsg= _T("");
		}
	}
	else if(m_nSkipChannel <= 4)
	{
		if(theConfigBank.m_Option.m_bUseBSkip == TRUE)
		{
			strMsg = _T("E_EQST_MATCH_INTERLOCK");
		}
		else
		{
			strMsg= _T("");
		}

	}
	else if(m_nSkipChannel <= 6)
	{
		if(theConfigBank.m_Option.m_bUseCSkip == TRUE)
		{
			strMsg = _T("E_EQST_MATCH_INTERLOCK");
		}
		else
		{
			strMsg= _T("");
		}

	}
	else if(m_nSkipChannel <= 8)
	{
		if(theConfigBank.m_Option.m_bUseDSkip == TRUE)
		{
			strMsg = _T("E_EQST_MATCH_INTERLOCK");
		}
		else
		{
			strMsg= _T("");
		}

	}

	theSocketInterFace.SendCmdUnitInterlockComfirmToDataPC(EQUIP_SET_UNIT_INTERLOCK_EVENT,m_nSkipChannel,strMsg);  //기존 검사 unit 상태 변환

	m_strOldUnitInterlockID = _T("");
	//--m_cUnitModuleNum = 0x00;

}
void CProcDataBank::SetTerminalMsg()
{
	CTokenizer t(m_strTerminalMsg);
	CString strTemp = t.GetNextToken(_T("_"));
	if(strTemp == _T("LOGIN"))
	{
		strTemp = t.GetNextToken(_T("_"));
		if(strTemp == _T("PASS"))
		{
			// 로그인 성공 [12/2/2016 OSC]
		}
		else
		{
			CUserAccount user;
			GetMainHandler()->SetCurUserData(user);
			// 로그인 실패 [12/2/2016 OSC]
		}
	}
	else if (strTemp == _T("LOGOUT"))
	{
		strTemp = t.GetNextToken(_T("_"));
		if(strTemp == _T("PASS"))
		{
			// 로그아웃 성공 
			CUserAccount user;
			GetMainHandler()->SetCurUserData(user);
		}
		else
		{
			// 로그아웃 실패 
		}
	}
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("Terminal Message"), _T("Terminal Message"), _T("Terminal Message"));
	dlgMsgBox.SetMessage(FALSE, 
		m_strTerminalMsg, 
		m_strTerminalMsg,		
		m_strTerminalMsg );

	dlgMsgBox.DoModal();
}

void CProcDataBank::ReadAlarmFile()
{
	CString filename;

	filename.Format(_T("%s\\AlarmList.ini"), PATH_SYSTEM);

	for(int i = 0; i < ALM_MAX; i++)
	{
		m_AlarmMsg[i].nAlarmLevel = ALARM_LEVEL_ALARM;
		for(int j = 0; j < LAN_MAX; j++)
		{
			m_AlarmMsg[i].strName[j] = _T("NO_ERROR");
			m_AlarmMsg[i].strCause[j] = _T("NO_CAUSE");
			m_AlarmMsg[i].strMethod[j] = _T("NO_METHOD");
		}
	}

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));
	if (e != 0) return;
	CStdioFile		file(fStream);
	CString strBuf, strItem;
	int nIndex = -1;
	CTokenizer t;
	while(file.ReadString(strBuf))
	{
		// HEADER는 무시
		if(strBuf == _T("[HEADER]"))
			continue;

		if( (strBuf.Left(1) == _T("[")) && (strBuf.Right(1) == _T("]")) )
		{
			// SECTION Check
			strItem = strBuf.Trim(_T("[]"));
			if(strItem.GetLength() > 0)
			{
				nIndex = _tstoi(strItem.GetString());
				if(nIndex >= ALM_MAX)
					nIndex = -1;
			}
		}
		else if(nIndex >= 0)
		{
			// 그 외 나머지는 nIndex가 있을 때만 처리
			t = CTokenizer(strBuf);
			strItem = t.GetNextToken(_T("="));
			if(strItem == _T("ALARM_NAME_KOR"))
			{
				m_AlarmMsg[nIndex].strName[LAN_KOR] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("ALARM_NAME_ENG"))
			{
				m_AlarmMsg[nIndex].strName[LAN_ENG] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("ALARM_NAME_VNM"))
			{
				m_AlarmMsg[nIndex].strName[LAN_VNM] = t.GetNextToken(_T("="));
			}

			if(strItem == _T("CAUSE_KOR"))
			{
				m_AlarmMsg[nIndex].strCause[LAN_KOR] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("CAUSE_ENG"))
			{
				m_AlarmMsg[nIndex].strCause[LAN_ENG] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("CAUSE_VNM"))
			{
				m_AlarmMsg[nIndex].strCause[LAN_VNM] = t.GetNextToken(_T("="));
			}

			if(strItem == _T("METHOD_KOR"))
			{
				m_AlarmMsg[nIndex].strMethod[LAN_KOR] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("METHOD_ENG"))
			{
				m_AlarmMsg[nIndex].strMethod[LAN_ENG] = t.GetNextToken(_T("="));
			}
			else if(strItem == _T("METHOD_VNM"))
			{
				m_AlarmMsg[nIndex].strMethod[LAN_VNM] = t.GetNextToken(_T("="));
			}
		}
	}

	file.Close();

	return;
}

CONST_EQUIP_PART::ID CProcDataBank::GetEqiupPart_to_AlarmID( ALARM_ID id )
{
	if(id >= ALARM_ETC)
	{
		return CONST_EQUIP_PART::EQUIP_PART_ETC;
	}
	else if( id >= ALARM_UNLOAD_PART )
	{
		return CONST_EQUIP_PART::EQUIP_PART_UNLOAD;
	}
	else if(id >= ALARM_INSP_PART)
	{
		return CONST_EQUIP_PART::EQUIP_PART_INSP;
	}
	else
	{
		return CONST_EQUIP_PART::EQUIP_PART_LOAD;
	}
}

CONST_EQUIP_UNIT::ID CProcDataBank::GetEqiupUnit_to_AlarmID( ALARM_ID id )
{
	//////////////////////////////////////////////////////////////////////////
	// Unloader
	//if(id >= ALM_UD_CELL_TR_Y)
	if(id >= ALARM_LOAD_PART)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_LD_ROBOT;
	}
	else if(id >= ALARM_UPPER_SHUTTLE_LOAD_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_UPPERSHUTTLE;
	}
	else if(id >= ALARM_LOWER_SHUTTLE_LOAD_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_LOWERSHUTTLE;
	}
	else if(id >= ALARM_PATR_Y_GET_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_PREALIGNTR;
	}
	//////////////////////////////////////////////////////////////////////////
	// Insp
	else if(id >= ALARM_PASTAGE_LOAD_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_PREALIGNSTG;
	}
	else if(id >= ALARM_PA_CH1_X_LOAD_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_PREALIGN;
	}
	else if(id >= ALARM_LD_TR_X_GET_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_LDTR;
	}
	else if(id >= ALARM_TT_POS_ALARM)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_TURNTABLE;
	}
	else if(id >= ALARM_AZONE_JIG_CH1_VAC)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_AZONE;
	}
	else if(id >= ALARM_BZONE_JIG_CH1_VAC)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_BZONE;
	}
	else if(id >= ALARM_CZONE_JIG_CH1_VAC)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_CZONE;
	}
	else if(id >= ALARM_DZONE_JIG_CH1_VAC)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_DZONE;
	}
	else if(id >= ALARM_UD_STG_CELL_EXIST)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_ULSTG;
	}
	else if(id >= ALARM_ULD_TR_X_GET_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_ULDTR;
	}
	else if(id >= ALARM_UD_ROBOT_VACON)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_ULD_ROBOT;
	}
	else if(id >= ALARM_OKTRAY_TR_Y_GET_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_GOOD_TRAY_TR;
	}
	else if(id >= ALARM_NGTRAY_TR_X_GET_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_NG_TRAY_TR;
	}
	else if(id >= ALARM_GOOD_TRAY_IN_LIFT_Z_DOWN_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_GOOD_IN_TRAY;
	}
	else if(id >= ALARM_GOOD_TRAY_OUT_LIFT_Z_DOWN_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_GOOD_OUT_TRAY;
	}
	else if(id >= ALARM_NG_TRAY_IN_LIFT_Z_DOWN_POS)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_NG_TRAY;
	}
	else if(id >= ALARM_AGV_READY)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_AGV;
	}
	else if(id >= ALARM_MONITORING_PART)
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_MONITORING;
	}
	else
	{
		return CONST_EQUIP_UNIT::EQUIP_UNIT_LOAD_COMMON;
	}
}

void CProcDataBank::AlarmMsgHappen( ALARM_ID id, CString strRemark /*= _T("")*/ )
{
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(FALSE, 
		m_AlarmMsg[id].strName[LAN_KOR], 
		m_AlarmMsg[id].strName[LAN_ENG],		
		m_AlarmMsg[id].strName[LAN_VNM] );

	dlgMsgBox.DoModal();
}

void CProcDataBank::AlarmHappen( ALARM_ID id, CString strRemark /*= _T("") */ )
{
	CString strBuf;
	// 기존 Alarm 상태가 아닐 때만 상태 변경 후 상위보고
	if (GetMainHandler()==NULL)
		return;
	

	CMainFrame* pMain = GetMainHandler();
	pMain->setModePM(FALSE);	

	if(PRE_ALARM_ID[id] == FALSE)
//	if(m_AlarmID == ALM_NONE)
	{
			//2017-07-17, SKCHO, AGV가 받지 못할 알람이면 포트 상태 변경 보고
		if(id == ALARM_AGV_READY 
			|| id == ALARM_AGV_TRANSFER_REQ 		
			|| id == ALARM_AGV_CONV_MOVE
			|| id == ALARM_AGV_BUSY 
			|| id == ALARM_AGV_COMP)
		{
			theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORTSTATECHANGE_REQUEST , _T("1"), _T("DOWN"));
			theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORTSTATECHANGE_REQUEST , _T("2"), _T("DOWN"));

			m_nStatePortType = 3;  //in/out 포트 모두
		}
		else if(id == ALARM_GOOD_IN_TRAY_LC_ALARM 
				|| id == ALARM_OKTRAY_IN_LIFT_STOPPER_UP_CHK 
				|| id == ALARM_OKTRAY_IN_BUFF_CONV_TRAY_CHK )
		{
			theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORTSTATECHANGE_REQUEST , _T("1"), _T("DOWN"));
			m_nStatePortType = 1;  //in/out 포트 모두
		}
		else if( id == ALARM_OKTRAY_OUT_BUF_CONV_TRAY_CHK
				 || id == ALARM_OKTRAY_OUT_LIFT_STOPPER_DOWN_CHK
				 || id == ALARM_GOOD_OUT_TRAY_LC_ALARM)
		{
			theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORTSTATECHANGE_REQUEST , _T("2"), _T("DOWN"));
			m_nStatePortType = 2;  //in/out 포트 모두
		}
		else
		{
			m_nStatePortType = 0;  //in/out 포트 모두
		}

		//2017-07-15,SKCHO, 알람 발생 후 티칭모드에서 문 열면 도어 알람 발생 방지하기 위해서
		theProcBank.m_bTeachMoveComplete = TRUE;

		if(theProcBank.m_bLightAlarmChk == FALSE)
		{
			theUnitFunc.ConvRun(GOOD_IN_TRAY_PORT,OFF);
			theUnitFunc.ConvRun(GOOD_OUT_TRAY_PORT,OFF);
			theUnitFunc.ConvRun(NG_TRAY_PORT,OFF);
			theUnitFunc.BufConvRun(GOOD_IN_TRAY_PORT,OFF);
			theUnitFunc.BufConvRun(GOOD_OUT_TRAY_PORT,OFF);
			theUnitFunc.BufConvRun(NG_TRAY_PORT,OFF); 

			theUnitFunc.InTrayLiftZ_PitchUpStop();
			theUnitFunc.InNGTrayLiftZ_PitchUpStop();

			// 알람 발생 시에 포워드 된 상태로 트레이가 껴있는 경우가 있다 2017/04/29 HSK.
			theUnitFunc.BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			theUnitFunc.BottomAlignCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
		}

		m_AlarmID = id;
		m_AlarmPart = GetEqiupPart_to_AlarmID(m_AlarmID);		// 로더/검사/언로더부 중 어느곳에서 났는지...
		m_AlarmUnit = GetEqiupUnit_to_AlarmID(m_AlarmID);		// 어느 유닛에서 났는지...

		theLog[LOG_ERROR].AddBuf(_T("Alarm Happen\t%d\t%s\t%s\t%d"), m_AlarmID, m_AlarmMsg[id].strName[1],strRemark, m_AlarmMsg[id].nAlarmLevel);
		GetLocalTime(&m_AlarmTime);

		alarm_info[m_nAlarmCnt].iERR_ID = m_AlarmID;

		strBuf.Format(_T("%d"),(int)m_AlarmID);  //2017-03-23,skcho, 알람
		alarm_info[m_nAlarmCnt].szERR_ID = strBuf;

//		alarm_info.szERR_NAME = m_AlarmMsg[id].strName[GetMainHandler()->m_nLangIdx];
		alarm_info[m_nAlarmCnt].szERR_NAME = m_AlarmMsg[id].strName[1];
		alarm_info[m_nAlarmCnt].TimeOccur = CTime(m_AlarmTime);
		alarm_info[m_nAlarmCnt].szLevel = CStringSupport::N2C(m_AlarmMsg[id].nAlarmLevel);
		
		if(GetMachineState() == CONST_MACHINE_STATE::MACHINE_CHECK)
		{
			alarm_info[m_nAlarmCnt].szLevel = CStringSupport::N2C(ALARM_LEVEL_CHECK);
			ChangeMachineState(CONST_MACHINE_STATE::MACHINE_CHECK);
			//2018-05-07,NAM, Last state and current state is same (MACHINE_CHECK) 
			m_ChangeState1=1;
 
		}
		else
		{
			theSocketInterFace.SendCmdAlarmToDataPC(m_AlarmID, TRUE);		

			//2018-04-20,SKCHO, TRAY가 없어서 발생한 알람에서는 장비 상태 변경을 다르게 한다.(홍영기 과장 요청)
			//트레이가 없어 장비가 멈추면 알람보고 하고 장비 상태는 RUN 유지하고 상위에 상태 변경만 보고
			if( id == ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY ||  id ==ALARM_OKTRAY_OUT_BUFF_PORT_FULL  
				||  id ==ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY  ||  id ==ALARM_NGTRAY_OUT_BUFF_PORT_FULL)
			{
				int nIndex = 0;
				if(id ==  ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY) nIndex = 0;
				else if(id ==  ALARM_OKTRAY_OUT_BUFF_PORT_FULL) nIndex = 1;
				else if(id ==  ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY) nIndex = 2;
				else if(id ==  ALARM_NGTRAY_OUT_BUFF_PORT_FULL) nIndex = 3;
				//FAULT로 상태 보고	/ 리셋은
				theSocketInterFace.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, E_EQST_MATCH_FAULT);
				m_bFaultOfTray[nIndex] = TRUE;
				m_RestartState=TRUE;
			}
			else
			{
																					    
				if(theProcBank.m_bLightAlarmChk == FALSE)			   
				{																			  
					ChangeMachineState(CONST_MACHINE_STATE::MACHINE_FAULT);
					theUnitFunc.SetOutPutIO(Y_START_SW_LAMP,OFF);
					theUnitFunc.SetOutPutIO(Y_STOP_SW_LAMP,ON);
				}
				else
				{
					theUnitFunc.SetOutPutIO(Y_BUZZER_K1,ON);
				}

				::PostMessage( GetMainWnd(), GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_Alarm, (LPARAM)0);
			}
		}

		PRE_ALARM_ID[m_AlarmID] = TRUE;	
	
		//m_AlarmID_List[m_nAlarmCnt++] = m_AlarmID;	//2017-03-06,skcho, 발생한 알람을 리스트에 저장한다.
	    m_AlarmID_List[m_nAlarmCnt] = m_AlarmID;
		if(m_nAlarmCnt > 99)
			m_nAlarmCnt = 0;

		//2018-03-14,MGYUN, 알람 MELSEC 기록 추가.
		theMonitorBank.WriteAlarm(m_nAlarmCnt, alarm_info[m_nAlarmCnt].iERR_ID);
		//2018-03-14,MGYUN, 알람 CAUSE MELSEC 기록 추가.
		theMonitorBank.WriteAlarmCause(m_nAlarmCnt,m_AlarmMsg[id].strCause[LAN_KOR]); 
		//2018-03-14,MGYUN, 알람 발생 시 FDC_ALARM_STATUS 1로 보고
		theMonitorBank.WriteFDCAlarmStatus(1);
		
		m_nAlarmCnt++;		//알람 전체 카운트
	
	}
	else
	{
	}

}

void CProcDataBank::AlarmClear()
{
	
	// 기존 Alarm 상태일 때만 상위보고
	if(m_AlarmID != ALM_NONE)
	{

		//2017-08-30,SKCHO,알람 발생 시 ABORT 초기화
		theUnitFunc.SendAGVAbort(GOOD_OUT_TRAY_PORT,OFF);
		theUnitFunc.SendAGVAbort(GOOD_IN_TRAY_PORT,OFF);

		//2017-03-06,skcho, 발생한 알람 수 만큼 알람 해제 보고를 한다.cim 검수 요청 사항
		for(int i = 0; i < m_nAlarmCnt; i++)
		{
			//2018-04-20,SKCHO, 이전 상태가  CHECK가 아닐때만 보고함
			if(m_OldMachineState != CONST_MACHINE_STATE::MACHINE_CHECK)
			{
				theSocketInterFace.SendCmdAlarmToDataPC(m_AlarmID_List[i], FALSE);
			}
			
			theMonitorBank.WriteAlarmClear(i);
			theMonitorBank.WriteAlarmCauseClear(i);			
			
			//2017-04-15,skcho, 다중알람발생시 로그에 정확한 시간 기록하기 위해서 배열로 만듦
			if ( theProcBank.alarm_info[i].szERR_ID != _T(""))
			{
				theProcBank.alarm_info[i].TimeClear = CTime::GetCurrentTime();		
				theProcBank.alarm_info[i].SetTakeTime();
				theProcBank.alarm_info[i].WriteErrorBuf();
			}
		}
		//2018-03-14,MGYUN, 알람 클리어시 상태 0으로 기록
		theMonitorBank.WriteFDCAlarmStatus(0);

		m_nAlarmCnt = 0;
		//Alarm Reset을 해도 설비 상태 변경은 되지 말아야 한다고 함.
 		//ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);

		theUnitFunc.SetOutPutIO(Y_BUZZER_K1, OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K2, OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K3, OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K4, OFF);

		theLog[LOG_ERROR].AddBuf(_T("Alarm Clear\t%d"), m_AlarmID);

		//20170201 byskcho
		//cell id 정보 틀릴경우 바이패스 시키는 메세지 박스
		if(m_AlarmID == ALARM_CIM_CELL_INFO_CH1 || m_AlarmID == ALARM_CIM_CELL_INFO_CH2)
		{
			CGxMsgBox	dlgMsgBox;
			
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("CELL ID가 일치하지 않습니다."),	//한
				_T("CELL ID가 일치하지 않습니다."),		//영
				_T("CELL ID가 일치하지 않습니다."));		//베

			int nRet = dlgMsgBox.DoModal();
			if(nRet == IDOK)
			{
				//2017-03-07,skcho, cell info가 틀리면 계속해서 요청하게 변경 / cim 요구사항
				//if(m_AlarmID == ALARM_CIM_CELL_INFO_CH1)
				//	theConfigBank.m_Option.m_bUseCh1ReqByPass = TRUE;
				//else
				//	theConfigBank.m_Option.m_bUseCh2ReqByPass = TRUE;
			}	

		}
			//20170201 byskcho
		//cell id 정보 틀릴경우 바이패스 시키는 메세지 박스
		if(m_AlarmID == ALARM_CIM_GET_RCMD_CH1 || m_AlarmID == ALARM_CIM_GET_RCMD_CH2)
		{
			CGxMsgBox	dlgMsgBox;
			
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("RCMD 가 일치하지 않습니다."),	//한
				_T("RCMD 가 일치하지 않습니다."),		//영
				_T("RCMD 가 일치하지 않습니다."));		//베

			int nRet = dlgMsgBox.DoModal();
			if(nRet == IDOK)
			{
				if(m_AlarmID == ALARM_CIM_GET_RCMD_CH1)
					theConfigBank.m_Option.m_bUseCh1RcmdByPass = TRUE;
				else
					theConfigBank.m_Option.m_bUseCh2RcmdByPass = TRUE;
			}	

		}
		m_AlarmID = ALM_NONE;

		memset(PRE_ALARM_ID, FALSE, sizeof(BOOL)*ALM_MAX);
	}
}

void CProcDataBank::ResetTemporarilyValue()
{
	//m_TrayBank.InitTrayInfo();

	for(int i = 0 ; i < ZONE_ID_MAX ; i++)
	{
		//kjpark 20161105 Sequence 동작 수정
		m_bTableTurnStatus[i] = TRUE;
	}
	m_nInspTablePos = CONST_TABLE_POS::POS_1;
	m_bInsptableTurn = FALSE;

	m_nIDGenIndex = 1;
	
	GOODTrayBitX = 1;
	GOODTrayBitY = 1;
	NGTrayBitX = 1;
	NGTrayBitY = 1;
	ResetAlignData();
	ResetLossData();
	ResetPortData();

	m_MsgOpCallCnt = 0;
	m_MsgInterlockCnt = 0;
	m_strOldInterlockID = _T("");

	m_bUserStop = FALSE;

	m_bGoodTrayReq = FALSE;
	m_bNGTrayReq = FALSE;

	m_bGoodTrayInSWReq = FALSE;
	m_bGoodTrayOutSWReq = FALSE;
	m_bNGTrayInSWReq = FALSE;
	m_bNGTrayOutSWReq = FALSE;
	m_bGoodTrayInMuteSWChk = FALSE;
	m_bGoodTrayOutMuteSWChk = FALSE;
	m_bNGTrayMuteSWChk = FALSE;

	m_bLowerPutCompInterlock = FALSE;

	m_bLowerPutComp = FALSE;
	m_bUpperPutComp = FALSE;

	m_bDCRChk = TRUE;

	m_bGoodTrayAlignChk = TRUE;
	m_bNGTrayAlignChk = TRUE;

	m_bFirstLoadTactStart = TRUE;
	m_bFirstUnloadTactStart = TRUE;

	m_CellLoadChk = FALSE;
	m_CellLoadRunChk = FALSE;

	m_bGoodInTrayMuteOn = FALSE;
	m_bGoodOutTrayMuteOn = FALSE;
	m_bNGTrayMuteOn = FALSE;

	memset(PRE_ALARM_ID, FALSE, sizeof(BOOL)*ALM_MAX); // 2017.2.6 bgkim

	m_bOKTrayTRLotEnd = FALSE; // 2017.2.6 bgkim
	m_bNGTrayTRLotEnd = FALSE; // 2017.2.6 bgkim

	// 2017.2.9 bgkim
	m_bDZoneTT = TRUE;

	m_bUpperMCRCompChk = TRUE;

	m_nAlarmCnt = 0;  //2017-03-06, skcho, 알람 카운트

	m_nGoodAlignCount = 0;  //2017-04-18,skcho,CELL 정보 지울때 초기화

	m_bTeachMoveComplete = TRUE;  //2017-04-18,skcho, 티칭모드 이동명령 중 플래그
	m_nPresentAxis = (AXIS_ID)0;  //티칭모드에서 이동중인 모터  ID 저장 변숲

	m_nGoodOutLiftCount = 0;//2017-04-20,skcho, CELL 정보 지울때 초기화
	m_nPre_GoodOutLiftCount = 0;  //2017-04-23,skcho, goodoutliftcount 이전값 저장

	m_nChSkipCount = 0;
	m_nChSecond = JIG_CH_1;
	m_nChSkipJigID = JIG_ID_A;

	//2017-09-17,SKCHO, 변수 초기화
	m_nCH1LowerMCRFailCnt = 0;
	m_nCH2LowerMCRFailCnt = 0;

	m_nCH1UpperMCRFailCnt = 0;
	m_nCH2UpperMCRFailCnt = 0;

	//2017-09-19,SKCHO, 변수 초기화
	m_nCH1CellInfoRetryStep = 0;
	m_nCH2CellInfoRetryStep = 0;
	//2017-09-21,SKCHO
	m_nULDTRRetryStep = 0;

	m_nGoodTrayRetryStep = 0;
	m_nGoodTrayRetryStep2 = 0;
	m_nNGTrayRetryStep = 0;
	m_nLDRobotRetryStep = 0;

	m_nGoodInPortLAGuidRetryStep = 0;
	m_nPATRUpperGetRetryStep = 0;
	m_nPATRLowerGetRetryStep = 0;

}

void CProcDataBank::ResetAlignData()
{
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		m_nPA_MITAlignResult[i] = ALIGN_RESULT_NONE;
		m_dPA_MITAlignX[i] = 0.0;	
		m_dPA_MITAlignY[i] = 0.0;	
		m_dPA_MITAlignTH[i] = 0.0;	
		m_dPA_MITAlignScore[i][E_MARK_LEFT] = 0.0;	
		m_dPA_MITAlignScore[i][E_MARK_RIGHT] = 0.0;	
	}

	m_nVI_MITAlignResult = ALIGN_RESULT_NONE;
	m_dVI_MITAlignScore = 0.0;
	m_dVI_MITAlignTH = 0.0;	

	// MIT Edge Align
	m_strLA_AlignOK = _T("NG");// ALIGN_RES OK or NG
	m_strLA_TeachingOK = _T("NG");// ALIGN_TEACHING_RES OK or NG
	m_strLAResX = _T("0.0");	// ALIGN_RES X축
	m_strLAResY = _T("0.0");;	// ALIGN_RES Y축
	m_strLAResT = _T("0.0");;	// ALIGN_RES Theta축
}


void CProcDataBank::ResetLossData()
{
	Loss_info.bFirstClass = FALSE;
	Loss_info.bSecondClass = FALSE;
	Loss_info.bThirdClass = FALSE;
	Loss_info.strFirstClass = _T("");
	Loss_info.strSecondClass = _T("");
	Loss_info.strThirdClass = _T("");
	Loss_info.strFirstGroup = _T("");
	Loss_info.strSecondGroup = _T("");
	Loss_info.strThirdGroup = _T("");
	Loss_info.strLossCode = _T("");
}


void CProcDataBank::ResetPortData()
{
	for (int i = 0; i < 6 ; i++)
	{
		if(i == 4)
		{
			Port_info.GoodInPortData[i] = _T("1");    //처음시작시 포트 이벤트가 없을 경우 in port는 up:2 / auto:1/ readytoload:1 / none:0
			Port_info.GoodOutPortData[i] = _T("0");   //처음시작시 포트 이벤트가 없을 경우 out port는 up:2 / auto:1/ empty:0 / none:0
		}
		else if(i == 5)
		{
			Port_info.GoodInPortData[i] = _T("0");
			Port_info.GoodOutPortData[i] = _T("0");
		}
		else
		{
			Port_info.GoodInPortData[i] = _T("");
			Port_info.GoodOutPortData[i] = _T("");
		}
		Port_info.NGPortData[i] = _T("");
	}
}


void CProcDataBank::ResetSubCarrierID()
{
	for(int i = 0; i < MAX_STRUCT; i++)
	{
		m_structSubCarrierID[i].m_strSubCarrier_ID=_T("");   
	    m_structSubCarrierID[i].m_strCellQty= _T("");
	}
}

void CProcDataBank::InitTime()
{
	GetLocalTime(&m_CurDate);

	CTime cTime(m_CurDate);

	if ( m_CurDate.wHour < 7 )
		cTime += CTimeSpan(-1,0,0,0);
	
	
	m_CurDate.wYear = (WORD)cTime.GetYear();
	m_CurDate.wMonth = (WORD)cTime.GetMonth();
	m_CurDate.wDay = (WORD)cTime.GetDay();

	m_szCurrent_time.Format(_T("%04u-%02u-%02u"),m_CurDate.wYear, m_CurDate.wMonth, m_CurDate.wDay);
	
	theProductBank.SetCurrentTime(m_szCurrent_time);
}

//2018-03-14, NAM, Initilise Time 
void CProcDataBank::InitTime1()
{
	GetLocalTime(&m_CurDate1);

	CTime cTime(m_CurDate1);

	if ( m_CurDate1.wHour < 7 )
		cTime += CTimeSpan(-1,0,0,0);
	
	
	m_CurDate1.wYear = (WORD)cTime.GetYear();
	m_CurDate1.wMonth = (WORD)cTime.GetMonth();
	m_CurDate1.wDay = (WORD)cTime.GetDay();

	m_szCurrent_time1.Format(_T("%04u-%02u-%02u"),m_CurDate1.wYear, m_CurDate1.wMonth, m_CurDate1.wDay);
	
	//theProductBank.SetCurrentTime(m_szCurrent_time1);
}

//2018-03-14, NAM, Initilise Time 
void CProcDataBank::InitTime2()
{
	GetLocalTime(&m_CurDate2);

	CTime cTime(m_CurDate2);

	if ( m_CurDate2.wHour < 7 )
		cTime += CTimeSpan(-1,0,0,0);
	
	
	m_CurDate2.wYear = (WORD)cTime.GetYear();
	m_CurDate2.wMonth = (WORD)cTime.GetMonth();
	m_CurDate2.wDay = (WORD)cTime.GetDay();

	m_szCurrent_time2.Format(_T("%04u-%02u-%02u"),m_CurDate2.wYear, m_CurDate2.wMonth, m_CurDate2.wDay);

	//theProductBank.SetCurrentTime(m_szCurrent_time2);
}
void CProcDataBank::InitTime3()
{
	GetLocalTime(&m_CurDate3);

	CTime cTime(m_CurDate3);
	cTime += CTimeSpan(0,-1,0,0);
	if ( m_CurDate3.wHour < 7 )
		cTime += CTimeSpan(-1,0,0,0);
	
	
	m_CurDate3.wYear = (WORD)cTime.GetYear();
	m_CurDate3.wMonth = (WORD)cTime.GetMonth();
	m_CurDate3.wDay = (WORD)cTime.GetDay();

	//2018-03-18, NAM, To get which hour for getting data. 
	m_CurDate3.wHour=(WORD)cTime.GetHour();
	m_szCurrent_time3.Format(_T("%04u-%02u-%02u"),m_CurDate3.wYear, m_CurDate3.wMonth, m_CurDate3.wDay);
	//theProductBank.SetCurrentTime(m_szCurrent_time3);
}


//2017-07-10,SKCHO, 
void CProcDataBank::MinusSubGoodAlignCount()
{
	if(m_nSubGoodAlignCount > 0)
	{
		m_nSubGoodAlignCount--;
	}
}
void CProcDataBank::MinusGoodAlignCount()
{
	if(m_nGoodAlignCount > 0)
	{
		m_nGoodAlignCount--;
	}
}//2017-09-17,SKCHO MCR 연속 실패 카운트 
//20171017 JSJUNG, MCR 5회이상 실패시 무한루프 문제 알람 처리
int CProcDataBank::GetMCRFail()
{
	int nRe = 0;
	if(m_nCH1LowerMCRFailCnt >= MCR_FAIL_CNT)
	{
		nRe = 1;
		m_nCH1LowerMCRFailCnt = 0;
	}
	else if(m_nCH2LowerMCRFailCnt >= MCR_FAIL_CNT)
	{
		nRe = 2;
		m_nCH2LowerMCRFailCnt = 0;
	}
	else if(m_nCH1UpperMCRFailCnt >= MCR_FAIL_CNT)
	{
		nRe = 3;
		m_nCH1UpperMCRFailCnt = 0;
	}
	else if(m_nCH2UpperMCRFailCnt >= MCR_FAIL_CNT)
	{
		nRe = 4;
		m_nCH2UpperMCRFailCnt = 0;
	}	
	return nRe;
}

//2017-11-01,SKCHO, UNIT INTERLOCK 체크 함수
void CProcDataBank::CheckUnitInterlock(ZONE_ID zone)
{
	JIG_ID nJig = JIG_ID_A ;
	nJig = (JIG_ID)(theUnitFunc.AMT_GetAutoJigID((ZONE_ID)zone));
	// 102보고  JIG SKIP
	if(nJig == JIG_ID_A)
	{
		if( m_cUnitModuleNum & 0x01)  //A JIG
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,1,_T("E_EQST_MATCH_INTERLOCK"),_T("0"));  //A JIG
			theConfigBank.m_Option.m_bUseASkip = TRUE;
			theConfigBank.m_Option.m_bOldUseASkip = theConfigBank.m_Option.m_bUseASkip;
			theConfigBank.m_Option.m_strASkipDescription = _T("UNIT INTERLOCK");
			m_cUnitModuleNum  &= 0XFE;
		}
	}
	else if(nJig == JIG_ID_B)
	{
		if(m_cUnitModuleNum & 0x02)  //B JIG
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,2,_T("E_EQST_MATCH_INTERLOCK"),_T("0"));  //A JIG
			
			theConfigBank.m_Option.m_bUseBSkip = TRUE;
			theConfigBank.m_Option.m_bOldUseBSkip = theConfigBank.m_Option.m_bUseBSkip;
			theConfigBank.m_Option.m_strBSkipDescription = _T("UNIT INTERLOCK");
			m_cUnitModuleNum  &= 0XFD;
		}
	}
	else if(nJig == JIG_ID_C)
	{
		if(m_cUnitModuleNum & 0x04)  //C JIG
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,3,_T("E_EQST_MATCH_INTERLOCK"),_T("0"));  //A JIG
			
			theConfigBank.m_Option.m_bUseCSkip = TRUE;
			theConfigBank.m_Option.m_bOldUseCSkip = theConfigBank.m_Option.m_bUseCSkip;
			theConfigBank.m_Option.m_strCSkipDescription = _T("UNIT INTERLOCK");
			m_cUnitModuleNum  &= 0XFB;
		}
	}
	else if(nJig == JIG_ID_D)
	{
		if(m_cUnitModuleNum & 0x08)  //D JIG
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,4,_T("E_EQST_MATCH_INTERLOCK"),_T("0"));  //A JIG
			
			theConfigBank.m_Option.m_bUseDSkip = TRUE;
			theConfigBank.m_Option.m_bOldUseDSkip = theConfigBank.m_Option.m_bUseDSkip;

			theConfigBank.m_Option.m_strDSkipDescription = _T("UNIT INTERLOCK");
			m_cUnitModuleNum  &= 0XF7;
		}
	}

	//2017-12-05,SKCHO 전체 스킵일 경우 TURN TABLE은 돌리 않는다.
	if(theConfigBank.m_Option.m_bUseASkip && theConfigBank.m_Option.m_bUseBSkip 
		&&theConfigBank.m_Option.m_bUseCSkip && theConfigBank.m_Option.m_bUseDSkip)
	{
		theProcBank.m_bAllUnitInterlock_Stop = TRUE;
	}
	else
	{
		theProcBank.m_bAllUnitInterlock_Stop = FALSE;
	}

}
//상위에서 받은 채널 아이디를 지그 넘버로 변환
void CProcDataBank::ChangeToJigNum(int nChannel)
{
	if(nChannel <= 2)
	{
		m_cUnitModuleNum |= 0x01;
	}
	else if(nChannel <= 4)
	{
		m_cUnitModuleNum |= 0x02;
	}
	else if(nChannel <= 6)
	{
		m_cUnitModuleNum |= 0x04;
	}
	else if(nChannel <= 8)
	{
		m_cUnitModuleNum |= 0x08;
	}
	else
	{
		m_cUnitModuleNum = 0x01;
	}

	m_nSkipChannel = nChannel;   //514보고는 내려온 채널에 대해서만 한다.

	//2017-11-01,SKCHO 테스트 삭제 예정
	/*for(int i = 0; i < 4; i++)
	{
		if(m_cUnitModuleNum >> i & 0x01)
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,i+1,_T("E_EQST_MATCH_INTERLOCK"));  //A JIG
		}
	}
	*/
}
//2017-12-05,SKCHO , CIM 연결 시 데이터 초기화
void CProcDataBank::InitUnitInterlock()
{

	if(theConfigBank.m_Option.m_bUseASkip == TRUE )
	{
		theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,1,_T("E_EQST_MATCH_INTERLOCK"),_T("1"));  //A JIG		
	}
	else 
	{
		theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,1,_T(""),_T("1"));  //A JIG	
	}
	Sleep(20);
	if(theConfigBank.m_Option.m_bUseBSkip == TRUE )
	{
		theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,2,_T("E_EQST_MATCH_INTERLOCK"),_T("1"));  //BJIG	
	}
	else 
	{
		theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,2,_T(""),_T("1"));  //B JIG	
	}
	Sleep(20);
	if(theConfigBank.m_Option.m_bUseCSkip == TRUE )
	{
		theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,3,_T("E_EQST_MATCH_INTERLOCK"),_T("1"));  //C JIG	
	}
	else 
	{
		theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,3,_T(""),_T("1"));  //C JIG	
	}
	Sleep(20);
	if(theConfigBank.m_Option.m_bUseDSkip == TRUE )
	{
		theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,4,_T("E_EQST_MATCH_INTERLOCK"),_T("1"));  //D JIG	
	}
	else 
	{
		theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,4,_T(""),_T("1"));  //D JIG	
	}
	
}

//2018-04-20,SKCHO
//ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY  -> indxe = 0
//ALARM_OKTRAY_OUT_BUFF_PORT_FULL   -> indxe = 1
//ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY -> indxe = 2
//ALARM_NGTRAY_OUT_BUFF_PORT_FULL -> indxe = 3
void CProcDataBank::ResetTrayAlarm(int nIndex)
{
	
	m_bFaultOfTray[nIndex] = FALSE;

	if(GetTrayAlarmChk() == FALSE)
	{
		if(GetOtherAlarmChk() == FALSE)  //다른 알람이 없으면 클리어 및 상태 변경 보고
		{
			theProcBank.AlarmClear();
			theSocketInterFace.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, E_EQST_MATCH_EXECUTING);
			//2018-06-21,NAM
			m_RestartState=FALSE;
		}
	}
}
//RESULT FALSE일때 런 실행되게
BOOL CProcDataBank::GetTrayAlarmChk()
{
	BOOL bResult = FALSE;
		//PORT 4개 중에 런을 방해하는 알람이 한개도 없어야 런을 진행하게함
	for(int i = 0; i < 4; i++)
	{
		if(m_bFaultOfTray[i] == TRUE)
		{
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

//PORT/FULL 알람 외 다른 알람 발생 체크
BOOL CProcDataBank::GetOtherAlarmChk()
{
	BOOL bResult = FALSE;

	for(int i = 0; i < ALM_MAX; i++)   //다른 알람이 있으면 상태 보고 안함
	{
		if( i != ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY 
			&& i != ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY 
			&& i != ALARM_OKTRAY_OUT_BUFF_PORT_FULL)
		{
			if(PRE_ALARM_ID[(ALARM_ID)i]== TRUE)
			{
				bResult = TRUE;
				break;
			}
		}
	}

	return bResult;
}

//2018wjp
BOOL CProcDataBank::LoadCellIDFromLocal()
{
	CGxIniFile iniFile; 
	iniFile.SetIniFilename(theConfigBank.m_System.m_strCarrierLoadCellIDPath);
	int iCount=0;
	CStringA str;
	str=iniFile.GetString(_T("CellID"), _T("Count"),_T("0"));
	iCount=atoi(str);
	CString strCellID=_T("");
	CString sKey=_T("");
	if(iCount<=0||iCount!=(theRecipeBank.m_Tray.m_nCountX*theRecipeBank.m_Tray.m_nCountY))
		return FALSE;
	if(iCount>0)
	{
		for(int i=0;i<CELL_ID_ROW_MAX;i++)
		{
			for(int j=0;j<CELL_ID_COL_MAX;j++)
			{
				theProcBank.strCIMLoadCellID[i][j]="";
			}
		}
	}
	for(int i=0;i<iCount;i++)
	{
		sKey.Format(_T("%d"),i+1);
		strCellID= iniFile.GetString(_T("CellID"), _T("Cell")+sKey,_T(""));
		theProcBank.strCIMLoadCellID[(int)(i/theRecipeBank.m_Tray.m_nCountY)][i%theRecipeBank.m_Tray.m_nCountY]=strCellID;
	}
	return TRUE;
}