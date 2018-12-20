#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "FDCBank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CUnitCtrlBank::CUnitCtrlBank( CString StateName, MP2100_THREAD nThreadID, BOOL bManual /*= FALSE*/, LOG_INDEX logindex /*= LOG_SEQUENCE*/ )
{
	m_strStateName = StateName;
	m_nStep = stepIdle;
	m_nError = -1;
	m_nThreadID = nThreadID;
	m_bManualSequence = bManual;
	m_LogIndex = logindex;


	m_bGoodInLightCurtain = FALSE;
	m_bGoodOutLightCurtain= FALSE;
	m_bNgInLightCurtain= FALSE;

	m_bOldGoodInLightCurtain= FALSE;
	m_bOldGoodOutLightCurtain= FALSE;
	m_bOldNgInLightCurtain= FALSE;

}

void CUnitCtrlBank::ResetReturnValue()
{
	memset(m_bRtnComp, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
	memset(m_bRtn, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
}

BOOL CUnitCtrlBank::IsReturnOk()
{
	return (memcmp(m_bRtn, m_bRtnComp, MAX_RTN_SIZE) == 0) ? TRUE:FALSE;
}

void CUnitCtrlBank::SetError( ALARM_ID id, CString strDiscription/*=_T("")*/ )
{
	m_nError = id;
	theLog[LOG_SEQUENCE].AddBuf(_T("[ERROR STEP] error=%d (%s, %d)"), id, m_strStateName, m_nStep);

	if(m_bManualSequence)
	{
		m_nStep = stepIdle;
		theProcBank.AlarmMsgHappen(id);
	}
	else
	{
		//2017-11-27,SKCHO, 설비가 정지 상태 일때 통신관련 알람은 무시함.
			if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE
				|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
				|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT
				|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK)
			{
				if(id == ALARM_COMM_SIG || id == ALARM_COMM_DATA || id == ALARM_COMM_LOWER_MCR	|| id == ALARM_COMM_UPPER_MCR
						|| id == ALARM_COMM_ACTIVE1	|| id == ALARM_COMM_ACTIVE2	|| id == ALARM_COMM_PREALIGN
						|| id == ALARM_COMM_EDGEALIGN  || id == ALARM_COMM_ROBOTALIGN || id == ALARM_COMM_LDROBOT
						|| id == ALARM_COMM_UDROBOT	|| id == ALARM_COMM_DCR	|| id == ALARM_COMM_TRAYALIGN)
				{
					return;
				}

			}
			theProcBank.AlarmHappen(id);
	}
}

void CONV_TR_Z_Vacuum_Release_Off(int nCH)
{

}
///////////////////////////////////////////////////////////////////////////////////////////

BOOL CUnitCtrlBank::GetAxisCheck( TEACH_PARAM::ID  idTeach )
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, idTeach);
}

//kjpark 20161019 모터축 구동 준비 체크
BOOL CUnitCtrlBank::GetAxisReadyCheck(AXIS_ID axis, BOOL bIgnoreOrigin)
{
	//kjpark 20161024 압력센서 쓰레드 구현중
	return theDeviceMotion.IsAxisReady(m_nThreadID, axis, TRUE);
}


//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
void CUnitCtrlBank::SetMotorStop(AXIS_ID  idAxis)
{
	theDeviceMotion.AxisStop(m_nThreadID, idAxis);
}

BOOL CUnitCtrlBank::GetInPutIOCheck(int ID)
{
	return theIOBank.GetInputIOParam((INPUT_ID)ID).GetIOCheck(theDeviceIO.ReadInBit(ID));
}

BOOL CUnitCtrlBank::GetInPutIOCheck( INPUT_ID ID, ONOFF bValue )
{
	return (theDeviceIO.ReadInBit(ID) == theIOBank.GetInputIOParam(ID).GetIOCheck(bValue)) ? TRUE:FALSE;
}

BOOL CUnitCtrlBank::GetOutPutIOCheck(int ID)
{
	return theIOBank.GetOutputIOParam((OUTPUT_ID)ID).GetIOCheck(theDeviceIO.ReadOutBit(ID));
}

void CUnitCtrlBank::SetOutPutIO(int ID, BOOL bValue)
{
	theDeviceIO.WriteOutBit((OUTPUT_ID)ID, theIOBank.GetOutputIOParam((OUTPUT_ID)ID).GetIOCheck(bValue));		
}

void CUnitCtrlBank::SetOutPutIO(int ID, ONOFF bValue)
{
	theDeviceIO.WriteOutBit((OUTPUT_ID)ID, theIOBank.GetOutputIOParam((OUTPUT_ID)ID).GetIOCheck(bValue));	
}

//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
void CUnitCtrlBank::TeachModeOn(BOOL bOn) 
{
	GetMainHandler()->m_bTeachMode = bOn;
	if (bOn == TRUE)
	{
		theUnitFunc.SetOutPutIO(Y_LD_DOOR_UNLOCK_1, OFF);
		theUnitFunc.SetOutPutIO(Y_LD_DOOR_UNLOCK_2, OFF);
		theUnitFunc.SetOutPutIO(Y_LD_DOOR_UNLOCK_3, OFF);
		theUnitFunc.SetOutPutIO(Y_LD_DOOR_UNLOCK_4, OFF);
		theUnitFunc.SetOutPutIO(Y_LD_DOOR_UNLOCK_5, OFF);
		theUnitFunc.SetOutPutIO(Y_LD_DOOR_UNLOCK_6, OFF);
		theUnitFunc.SetOutPutIO(Y_UD_DOOR_UNLOCK_1, OFF);
		theUnitFunc.SetOutPutIO(Y_UD_DOOR_UNLOCK_2, OFF);
		theUnitFunc.SetOutPutIO(Y_UD_DOOR_UNLOCK_3, OFF);
		theUnitFunc.SetOutPutIO(Y_UD_DOOR_UNLOCK_4, OFF);
		theUnitFunc.SetOutPutIO(Y_UD_DOOR_UNLOCK_5, OFF);
		theUnitFunc.SetOutPutIO(Y_UD_DOOR_UNLOCK_6, OFF);
	}
}

//2017-10-07,SKCHO , IO값을 읽어서 버퍼에 저장한다.
void CUnitCtrlBank::DoorInPutIOState() 
{
	
#if(AMT_TYPE == 1)		
		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_1))
		{
			m_bInPutIO[X_LD_DOOR_SEN_1] = TRUE;			
		}
		else
		{
			m_bInPutIO[X_LD_DOOR_SEN_1] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_2))
		{
			m_bInPutIO[X_LD_DOOR_SEN_2] = TRUE;		
		}
		else
		{
			m_bInPutIO[X_LD_DOOR_SEN_2] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_3))
		{
			m_bInPutIO[X_LD_DOOR_SEN_3] = TRUE;		
		}
		else
		{
			m_bInPutIO[X_LD_DOOR_SEN_3] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_4))
		{
			m_bInPutIO[X_LD_DOOR_SEN_4] = TRUE;		 
		}
		else
		{
			m_bInPutIO[X_LD_DOOR_SEN_4] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_5))
		{
			m_bInPutIO[X_LD_DOOR_SEN_5] = TRUE;		 
		}
		else
		{
			m_bInPutIO[X_LD_DOOR_SEN_5] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_6))
		{
			m_bInPutIO[X_LD_DOOR_SEN_6] = TRUE;		
		}
		else
		{
			m_bInPutIO[X_LD_DOOR_SEN_6] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_1))
		{
			m_bInPutIO[X_ULD_DOOR_SEN_1] = TRUE;		 
		}
		else
		{
			m_bInPutIO[X_ULD_DOOR_SEN_1] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_2))
		{
			m_bInPutIO[X_ULD_DOOR_SEN_2] = TRUE;		
		}
		else
		{
			m_bInPutIO[X_ULD_DOOR_SEN_2] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_3))
		{
			m_bInPutIO[X_ULD_DOOR_SEN_3] = TRUE;		 
		}
		else
		{
			m_bInPutIO[X_ULD_DOOR_SEN_3] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_4))
		{
			m_bInPutIO[X_ULD_DOOR_SEN_4] = TRUE;		
		}
		else
		{
			m_bInPutIO[X_ULD_DOOR_SEN_4] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_5))
		{
			m_bInPutIO[X_ULD_DOOR_SEN_5] = TRUE;		
		}
		else
		{
			m_bInPutIO[X_ULD_DOOR_SEN_5] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_6))
		{
			m_bInPutIO[X_ULD_DOOR_SEN_6] = TRUE;		
		}
		else
		{
			m_bInPutIO[X_ULD_DOOR_SEN_6] = FALSE;
		}
#else
		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_1))
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_1] = TRUE;	
		}
		else
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_1] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_2))
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_2] = TRUE;		
		}
		else
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_2] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_3))
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_3] = TRUE;		 
		}
		else
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_3] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_4))
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_4] = TRUE;		 
		}
		else
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_4] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_5))
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_5] = TRUE;		 
		}
		else
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_5] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_6))
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_6] = TRUE;			
		}
		else
		{
			theProcBank.m_bInPutIO[X_LD_DOOR_SEN_6] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_1))
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_1] = TRUE;		 
		}
		else
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_1] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_2))
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_2] = TRUE;		
		}
		else
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_2] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_3))
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_3] = TRUE;		 
		}
		else
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_3] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_4))
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_4] = TRUE;		
		}
		else
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_4] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_5))
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_5] = TRUE;		 
		}
		else
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_5] = FALSE;
		}

		if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_6))
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_6] = TRUE;			 
		}
		else
		{
			theProcBank.m_bInPutIO[X_ULD_DOOR_SEN_6] = FALSE;
		}
#endif	
	
}


BOOL CUnitCtrlBank::DoorInterLockChk() 
{
	//2017-10-07,SKCHO, IO값을 읽어서 버퍼에 저장한다.아래에서 버퍼값을 참조하여 동작실행한다.
	DoorInPutIOState() ;


	//2017-10-07,SKCHO   GetInPutIOCheck()-> GetInPutIOState()함수로 변경
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
		|| theUnitFunc.GetInPutIOCheck(X_TEACH_KEY_SW))
	{
#if(AMT_TYPE == 1)		
		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_1))
		{		
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN1);
			return FALSE; 
		}	

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_2))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN2);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_3))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN3);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_4))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN4);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_5))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN5);
			return FALSE; 
		}	

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_6))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN6);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_1))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN1);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_2))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN2);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_3))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN3);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_4))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN4);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_5))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN5);
			return FALSE; 
		}	

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_6))
		{		
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN6);
			return FALSE; 
		}
		
#else
		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_1))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN3);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_2))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN2);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_3))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN1);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_4))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN6);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_5))
		{		
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN5);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_6))
		{			
			theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN4);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_1))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN6);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_2))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN1);
			return FALSE; 
		}	

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_3))
		{		
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN2);
			return FALSE; 
		}		

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_4))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN3);
			return FALSE; 
		}	

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_5))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN4);
			return FALSE; 
		}	

		if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_6))
		{			
			theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN5);
			return FALSE; 
		}		
#endif
	}
	else
	{
		//2017-07-28,JSJUNG, TEACH 상태에서 문열리면 알람 울려서 수정 (X_TEACH_KEY_SW) == FALSE->TRUE)
		// Pause 상태나 Fault 상태에서는 모터가 움직 일 때만 체크 한다.2017/04/26 HSK.
		if((theProcBank.m_bTeachMoveComplete == FALSE)
			&& (theUnitFunc.GetInPutIOCheck(X_TEACH_KEY_SW) == TRUE))  //2017-04-18,skcho, 이동중 문열리면 감지되게
		{
#if(AMT_TYPE == 1)		
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_1))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN1);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_2))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN2);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_3))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN3);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_4))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN4);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_5))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN5);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_6))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN6);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_1))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN1);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_2))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN2);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_3))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN3);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_4))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN4);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_5))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN5);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_6))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN6);
				return FALSE; 
			}
#else
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_1))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN1);
				//theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN3);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_2))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN2);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_3))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN3);
				//theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN1);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_4))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN4);
				//theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN6);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_5))
			{				
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN5);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_6))
			{
				theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN6);
				//theProcBank.AlarmHappen(ALARM_LD_DOOR_SEN4);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_1))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN1);
				//theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN6);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_2))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN2);
				//theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN1);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_3))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN3);
				//theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN2);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_4))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN4);
				//theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN3);

				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_5))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN5);
				//theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN4);
				return FALSE; 
			}
			if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_6))
			{
				theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN6);
				//theProcBank.AlarmHappen(ALARM_ULD_DOOR_SEN5);
				return FALSE; 
			}
#endif
		}
	}
	return TRUE;
}

BOOL CUnitCtrlBank::TempChk()
{
	if(theUnitFunc.GetInPutIOCheck(X_FIRE_ALARM_SEN_1))
	{
		theProcBank.AlarmHappen(ALARM_FIRE_1);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_FIRE_ALARM_SEN_2))
	{
		theProcBank.AlarmHappen(ALARM_FIRE_2);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_FIRE_ALARM_SEN_3))
	{
		theProcBank.AlarmHappen(ALARM_FIRE_3);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_FIRE_ALARM_SEN_4))
	{
		theProcBank.AlarmHappen(ALARM_FIRE_4);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_FIRE_ALARM_SEN_5))
	{
		theProcBank.AlarmHappen(ALARM_FIRE_5);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_UPS_BOX_TEMP_ALARM_SEN_1))
	{
		theProcBank.AlarmHappen(ALARM_UPS_BOX_TEMP_1);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_UPS_BOX_TEMP_ALARM_SEN_2))
	{
		theProcBank.AlarmHappen(ALARM_UPS_BOX_TEMP_2);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_GPS_BOX_TEMP_ALARM_SEN_1))
	{
		theProcBank.AlarmHappen(ALARM_GPS_BOX_TEMP_1);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_GPS_BOX_TEMP_ALARM_SEN_2))
	{
		theProcBank.AlarmHappen(ALARM_GPS_BOX_TEMP_2);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_ULD_BOX_TEMP_ALARM_SEN))
	{
		theProcBank.AlarmHappen(ALARM_ULD_BOX_TEMP);
		return FALSE; 
	}
	if(theUnitFunc.GetInPutIOCheck(X_PC_BOX_TEMP_ALARM_SEN))
	{
		theProcBank.AlarmHappen(ALARM_PC_BOX_TEMP);
		return FALSE; 
	}
	return TRUE;
}

BOOL CUnitCtrlBank::EMSChk()
{
	//EMS 설정

	if(theProcBank.m_AlarmID == ALM_NONE)
	{		
		if(GetInPutIOCheck(X_EMS_SW_FRONT) == TRUE)
		{
			theProcBank.AlarmHappen(ALARM_FRONT_EMS);
			return FALSE;	
		}
		if(GetInPutIOCheck(X_EMS_SW_BACK) == TRUE)
		{
			theProcBank.AlarmHappen(ALARM_BACK_EMS);
			return FALSE;		
		}
		if(GetInPutIOCheck(X_EMS_SW_LEFT) == TRUE)
		{
			theProcBank.AlarmHappen(ALARM_LEFT_EMS);
			return FALSE;	
		}
		if(GetInPutIOCheck(X_LD_ROBOT_EMS) == TRUE)
		{
			theProcBank.AlarmHappen(ALARM_LDROBOT_EMS);
			return FALSE;	
		}
		if(GetInPutIOCheck(X_ULD_ROBOT_EMS) == TRUE)
		{
			theProcBank.AlarmHappen(ALARM_ULDROBOT_EMS);
			return FALSE;
		}
		if(GetInPutIOCheck(X_LD_ROBOT_EMS_ALARM) == TRUE)
		{
			theProcBank.AlarmHappen(ALARM_LDROBOT_EMS);
			return FALSE;		
		}
		if(GetInPutIOCheck(X_ULD_ROBOT_EMS_ALARM) == TRUE)
		{
			theProcBank.AlarmHappen(ALARM_ULDROBOT_EMS);
			return FALSE;
		}


		//2017-12-27,KHLEE, 메인파워알람 RESET시 알람 중복누적되지 않도록 수정
		m_bMainPower = GetInPutIOCheck(X_MC_POWER);
		//if(GetInPutIOCheck(X_MC_POWER) == FALSE)
		if(m_bMainPower == FALSE)
		{
			if(m_bOldMainPower != m_bMainPower)
			{
				theProcBank.AlarmHappen(ALARM_MAIN_POWER_OFF);
				m_bOldMainPower = m_bMainPower;
			}
			return FALSE;
		}
		else
		{
			m_bOldMainPower = m_bMainPower;
		}

#if(AMT_TYPE == 2)
		if(GetInPutIOCheck(X_GPS_DROP_CHK) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_DROP);
			return FALSE;
		}
#endif
	}
	return TRUE;
}

BOOL CUnitCtrlBank::MainVacChk()
{
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
		|| theUnitFunc.GetInPutIOCheck(X_TEACH_KEY_SW))
	{
		if(theUnitFunc.GetInPutIOCheck(X_MAIN_AIR_SEN_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_MAIN_AIR_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_MAIN_AIR_SEN_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_MAIN_AIR_2);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_MAIN_VAC_SEN_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_MAIN_VAC_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_MAIN_VAC_SEN_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_MAIN_VAC_2);
			return FALSE; 
		}
	}
	return TRUE;
}
BOOL CUnitCtrlBank::FANChk()
{

	return FALSE;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
#if(AMT_TYPE == 1)
		if(theUnitFunc.GetInPutIOCheck(X_UPS_FAN_ALARM_SEN_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_UPS_FAN_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_UPS_FAN_ALARM_SEN_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_UPS_FAN_2);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_UPS_FAN_ALARM_SEN_3) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_UPS_FAN_3);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_UPS_FAN_ALARM_SEN_4) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_UPS_FAN_4);
			return FALSE; 
		}
		// 1,2호기 I/O 없음 
		//if(theUnitFunc.GetInPutIOCheck(X_UPS_FAN_ALARM_SEN_5) == FALSE)
		//{
		//	theProcBank.AlarmHappen(ALARM_UPS_FAN_5);
		//	return FALSE; 
		//}
		//if(theUnitFunc.GetInPutIOCheck(X_UPS_FAN_ALARM_SEN_6) == FALSE)
		//{
		//	theProcBank.AlarmHappen(ALARM_UPS_FAN_6);
		//	return FALSE; 
		//}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_FAN_ALARM_SEN_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_FAN_ALARM_SEN_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_2);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_FAN_ALARM_SEN_3) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_3);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_FAN_ALARM_SEN_4) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_4);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_FAN_ALARM_SEN_5) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_5);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_FAN_ALARM_SEN_6) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_6);
			return FALSE; 
		}
		// 1,2 호기 안씀
		//if(theUnitFunc.GetInPutIOCheck(X_GPS_FAN_ALARM_SEN_7) == FALSE)
		//{
		//	theProcBank.AlarmHappen(ALARM_GPS_FAN_7);
		//	return FALSE; 
		//}
		//if(theUnitFunc.GetInPutIOCheck(X_GPS_FAN_ALARM_SEN_8) == FALSE)
		//{
		//	theProcBank.AlarmHappen(ALARM_GPS_FAN_8);
		//	return FALSE; 
		//}
			if(theUnitFunc.GetInPutIOCheck(X_ULD_FAN_ALARM_SEN_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_ULD_FAN_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_ULD_FAN_ALARM_SEN_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_ULD_FAN_2);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_ULD_FAN_ALARM_SEN_3) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_ULD_FAN_3);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_ULD_FAN_ALARM_SEN_4) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_ULD_FAN_4);
			return FALSE; 
		}
#else
		if(theUnitFunc.GetInPutIOCheck(X_GPS_BOX_FAN_ALARM_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_BOX_FAN_ALARM_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_2);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_BOX_FAN_ALARM_3) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_3);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_BOX_FAN_ALARM_4) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_4);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_BOX_FAN_ALARM_5) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_5);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_GPS_BOX_FAN_ALARM_6) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_GPS_FAN_6);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_UPS_BOX_FAN_ALARM_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_UPS_FAN_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_UPS_BOX_FAN_ALARM_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_UPS_FAN_2);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_UPS_BOX_FAN_ALARM_3) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_UPS_FAN_3);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_UPS_BOX_FAN_ALARM_4) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_UPS_FAN_4);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_C_BOX_FAN_ALARM_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_CBOX_FAN_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_C_BOX_FAN_ALARM_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_CBOX_FAN_2);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_C_BOX_FAN_ALARM_3) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_CBOX_FAN_3);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_C_BOX_FAN_ALARM_4) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_CBOX_FAN_4);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_ULD_FAN_ALARM_SEN_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_ULD_FAN_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_ULD_FAN_ALARM_SEN_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_ULD_FAN_2);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_ULD_FAN_ALARM_SEN_3) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_ULD_FAN_3);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_ULD_FAN_ALARM_SEN_4) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_ULD_FAN_4);
			return FALSE; 
		}
#endif
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_1) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_1);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_2) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_2);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_3) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_3);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_4) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_4);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_5) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_5);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_6) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_6);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_7) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_7);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_8) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_8);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_9) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_9);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_10) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_10);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_11) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_11);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_12) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_12);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_13) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_13);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_14) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_14);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_15) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_15);
			return FALSE; 
		}
		if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_FAN_ALARM_SEN_16) == FALSE)
		{
			theProcBank.AlarmHappen(ALARM_PC_RACK_FAN_16);
			return FALSE; 
		}
	}
	return TRUE;
}

void CUnitCtrlBank::IonizerChk() // EJKIM - 2013/09/10
{

}

//2017-10-07,SKCHO  LC 와 TRAY LIFT ARM 센서 IO값을 버퍼에 저장한다.
void CUnitCtrlBank::LCInPutIOState()
{
	if (GetInPutIOCheck(X_GOOD_IN_TRAY_LC_CHK))
	{
		theProcBank.m_bInPutIO[X_GOOD_IN_TRAY_LC_CHK] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_GOOD_IN_TRAY_LC_CHK] = FALSE;
	}

	if (GetInPutIOCheck(X_GOOD_OUT_TRAY_LC_CHK))
	{
		theProcBank.m_bInPutIO[X_GOOD_OUT_TRAY_LC_CHK] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_GOOD_OUT_TRAY_LC_CHK] = FALSE;
	}

	if (GetInPutIOCheck(X_NG_TRAY_LC_CHK))
	{
		theProcBank.m_bInPutIO[X_NG_TRAY_LC_CHK] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_NG_TRAY_LC_CHK] = FALSE;
	}

	//2017-10-07,SKCHO
	if(theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_SEN))
	{
		theProcBank.m_bInPutIO[X_GOOD_LD_TRAY_LIFT_SEN] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_GOOD_LD_TRAY_LIFT_SEN] = FALSE;
	}

	//2017-10-07,SKCHO
	if(theUnitFunc.GetInPutIOCheck(X_GOOD_ULD_TRAY_LIFT_SEN))
	{
		theProcBank.m_bInPutIO[X_GOOD_ULD_TRAY_LIFT_SEN] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_GOOD_ULD_TRAY_LIFT_SEN] = FALSE;
	}

	//2017-10-07,SKCHO
	if(theUnitFunc.GetInPutIOCheck(X_NG_TRAY_LIFT_ARM_TRAY_SEN_1))
	{
		theProcBank.m_bInPutIO[X_NG_TRAY_LIFT_ARM_TRAY_SEN_1] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_NG_TRAY_LIFT_ARM_TRAY_SEN_1] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_NG_TRAY_LIFT_ARM_TRAY_SEN_2))  // 중간센서
	{
		theProcBank.m_bInPutIO[X_NG_TRAY_LIFT_ARM_TRAY_SEN_2] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_NG_TRAY_LIFT_ARM_TRAY_SEN_2] = FALSE;
	}

	//2017-10-07,SKCHO
	if(theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_3))  // 중간센서
	{
		theProcBank.m_bInPutIO[X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_3] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_3] = FALSE;
	}
	//2017-11-17,YMG
	if(theUnitFunc.GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1))  
	{
		theProcBank.m_bInPutIO[X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1] = FALSE;
	}

	//2017-11-17,YMG
	if(theUnitFunc.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4))  
	{
		theProcBank.m_bInPutIO[X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4] = FALSE;
	}

	//2018-03-19,GHLEE
	if(theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_FINAL_SEN_1))  
	{
		theProcBank.m_bInPutIO[X_GOOD_LD_TRAY_FINAL_SEN_1] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_GOOD_LD_TRAY_FINAL_SEN_1] = FALSE;
	}

}
//2018-05-10,MGYUN, IO 값을 버퍼 저장
void CUnitCtrlBank::HybridConvIOState()
{
	//2018-05-10,MGYUN,하이브리드 I/O 작업.
	if(theUnitFunc.GetInPutIOCheck(X_HYBRID_CONV_READY))  
	{
		theProcBank.m_bInPutIO[X_HYBRID_CONV_READY] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_HYBRID_CONV_READY] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_HYBRID_CONV_RUN))  
	{
		theProcBank.m_bInPutIO[X_HYBRID_CONV_RUN] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_HYBRID_CONV_RUN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_HYBRID_CONV_INTERFACE))  
	{
		theProcBank.m_bInPutIO[X_HYBRID_CONV_INTERFACE] = TRUE;
	}
	else
	{
		theProcBank.m_bInPutIO[X_HYBRID_CONV_INTERFACE] = FALSE;
	}
	//////////////////////OUTPUT/////////////////////////////////
	if(theUnitFunc.GetOutPutIOCheck(Y_HYBRID_ROBOT_RUN))  
	{
		theProcBank.m_bOutPutIO[Y_HYBRID_ROBOT_RUN] = TRUE;
	}
	else
	{
		theProcBank.m_bOutPutIO[Y_HYBRID_ROBOT_RUN] = FALSE;
	}

	if(theUnitFunc.GetOutPutIOCheck(Y_HYBRID_SEND_COMPLETE))  
	{
		theProcBank.m_bOutPutIO[Y_HYBRID_SEND_COMPLETE] = TRUE;
	}
	else
	{
		theProcBank.m_bOutPutIO[Y_HYBRID_SEND_COMPLETE] = FALSE;
	}
	if(theUnitFunc.GetOutPutIOCheck(Y_HYBRID_ROBOT_READY))  
	{
		theProcBank.m_bOutPutIO[Y_HYBRID_ROBOT_READY] = TRUE;
	}
	else
	{
		theProcBank.m_bOutPutIO[Y_HYBRID_ROBOT_READY] = FALSE;
	}
}
BOOL CUnitCtrlBank::LightCurtainOnCheck()
{
	//2017-10-07,SKCHO  IO값을 버퍼에 저장한다.
	LCInPutIOState();


	//2017-10-07,SKCHO  GetInPutIOCheck() -> GetInPutIOState()함수로 변경
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
		|| theUnitFunc.GetInPutIOCheck(X_TEACH_KEY_SW))
	{
		m_bGoodInLightCurtain = GetInPutIOState(X_GOOD_IN_TRAY_LC_CHK);
		m_bGoodOutLightCurtain= GetInPutIOState(X_GOOD_OUT_TRAY_LC_CHK);
		m_bNgInLightCurtain= GetInPutIOState(X_NG_TRAY_LC_CHK);

		//if (GetInPutIOState(X_GOOD_IN_TRAY_LC_CHK))
		if (m_bGoodInLightCurtain)
		{
			if (theProcBank.m_bGoodInTrayMuteOn == FALSE) // 2017.2.3 bgkim
			{
				if(m_bOldGoodInLightCurtain != m_bGoodInLightCurtain)
				{
					theProcBank.AlarmHappen(ALARM_GOOD_IN_TRAY_LC_ALARM);
					m_bOldGoodInLightCurtain= m_bGoodInLightCurtain;
				}
				return FALSE; 
			}
		}
		else
		{
			m_bOldGoodInLightCurtain= m_bGoodInLightCurtain;
		}
		//if (GetInPutIOState(X_GOOD_OUT_TRAY_LC_CHK))
		if (m_bGoodOutLightCurtain)
		{
			if (theProcBank.m_bGoodOutTrayMuteOn == FALSE) // 2017.2.3 bgkim
			{
				if(m_bOldGoodOutLightCurtain != m_bGoodOutLightCurtain)
				{
					theProcBank.AlarmHappen(ALARM_GOOD_OUT_TRAY_LC_ALARM);
					m_bOldGoodOutLightCurtain= m_bGoodOutLightCurtain;
				}
				return FALSE; 
			}
		}
		else
		{
			m_bOldGoodOutLightCurtain= m_bGoodOutLightCurtain;
		}
		//if (GetInPutIOState(X_NG_TRAY_LC_CHK))
		if (m_bNgInLightCurtain)
		{
			if (theProcBank.m_bNGTrayMuteOn == FALSE) // 2017.2.3 bgkim
			{
				if(m_bOldNgInLightCurtain != m_bNgInLightCurtain)
				{
					theProcBank.AlarmHappen(ALARM_NG_TRAY_LC_ALARM);
					m_bOldNgInLightCurtain= m_bNgInLightCurtain;
				}
				return FALSE; 
			}
		}
		else
		{
			m_bOldNgInLightCurtain= m_bNgInLightCurtain;
		}
	}

	// 2017.3.27 JHLIM STO 버튼 리셋 버튼 누를시 라이트 커튼 알람으로 인한 모터 OFF 값 ON 시키기
	if (GetInPutIOCheck(X_RESET_SW) == TRUE)
	{
		if (theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z) == TRUE)
		{
			theDeviceMotion.ResetAlarm(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z);
			theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z);
			theDeviceMotion.ServoOn(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z);
			theUnitFunc.SetOutPutIO(Y_RESET_SW_LAMP, OFF); 
		}
		if (theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z) == TRUE)
		{
			theDeviceMotion.ResetAlarm(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z);
			theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z);
			theDeviceMotion.ServoOn(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z);
			theUnitFunc.SetOutPutIO(Y_RESET_SW_LAMP, OFF); 
		}
		if (theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_IN_LIFT_Z) == TRUE)
		{
			theDeviceMotion.ResetAlarm(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_IN_LIFT_Z);
			theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_IN_LIFT_Z);
			theDeviceMotion.ServoOn(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_IN_LIFT_Z);
			theUnitFunc.SetOutPutIO(Y_RESET_SW_LAMP, OFF); 
		}
		if (theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_OUT_LIFT_Z) == TRUE)
		{
			theDeviceMotion.ResetAlarm(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_OUT_LIFT_Z);
			theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_OUT_LIFT_Z);
			theDeviceMotion.ServoOn(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_OUT_LIFT_Z);
			theUnitFunc.SetOutPutIO(Y_RESET_SW_LAMP, OFF); 
		}
		return FALSE;
	}
	return TRUE;
}

//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
void CUnitCtrlBank::LampChange(int nRed, int nYellow, int nGreen, int nBuzzer)
{	

	SetOutPutIO(Y_BUZZER_K1, OFF);
	SetOutPutIO(Y_BUZZER_K2, OFF);
	SetOutPutIO(Y_BUZZER_K3, OFF);
	SetOutPutIO(Y_BUZZER_K4, OFF);

	if(nBuzzer ==1 )
	{
		SetOutPutIO(Y_BUZZER_K1, ON);
	}
	else if(nBuzzer == 2)
	{
		SetOutPutIO(Y_BUZZER_K2, ON);
	}
	else if(nBuzzer == 3)
	{
		SetOutPutIO(Y_BUZZER_K3, ON);
	}
	else if(nBuzzer == 4)
	{
		SetOutPutIO(Y_BUZZER_K4, ON);
	}

	// RED
	if(nRed == LAMP_ON)
	{
		SetOutPutIO(Y_TOWER_LAMP_RED, ON);
		SetOutPutIO(Y_TOWER_LAMP_RED2, ON);
		GetMainHandler()->m_bChkLampR_Flick = FALSE;
	}
	else if(nRed == LAMP_OFF)
	{
		SetOutPutIO(Y_TOWER_LAMP_RED, OFF);
		SetOutPutIO(Y_TOWER_LAMP_RED2, OFF);
		GetMainHandler()->m_bChkLampR_Flick = FALSE;
	}
	else if(nRed == LAMP_FLICKER)
	{
		GetMainHandler()->m_bChkLampR_Flick = TRUE;
	}

	// YELLOW
	if(nYellow == LAMP_ON)
	{
		SetOutPutIO(Y_TOWER_LAMP_YELLOW, ON);
		SetOutPutIO(Y_TOWER_LAMP_YELLOW2, ON);
		GetMainHandler()->m_bChkLampY_Flick = FALSE;
	}
	else if(nYellow == LAMP_OFF)
	{
		SetOutPutIO(Y_TOWER_LAMP_YELLOW, OFF);
		SetOutPutIO(Y_TOWER_LAMP_YELLOW2, OFF);
		GetMainHandler()->m_bChkLampY_Flick = FALSE;
	}
	else if(nYellow == LAMP_FLICKER)
	{
		GetMainHandler()->m_bChkLampY_Flick = TRUE;
	}

	// GREEN
	if(nGreen == LAMP_ON)
	{		
		SetOutPutIO(Y_TOWER_LAMP_GREEN, ON);
		SetOutPutIO(Y_TOWER_LAMP_GREEN2, ON);
		GetMainHandler()->m_bChkLampG_Flick = FALSE;
	}
	else if(nGreen == LAMP_OFF)
	{
		SetOutPutIO(Y_TOWER_LAMP_GREEN, OFF);
		SetOutPutIO(Y_TOWER_LAMP_GREEN2, OFF);
		GetMainHandler()->m_bChkLampG_Flick = FALSE;
	}
	else if(nGreen == LAMP_FLICKER)
	{
		GetMainHandler()->m_bChkLampG_Flick = TRUE;
	}

}
void CUnitCtrlBank::LampChange(STATUS_LAMP_DESCRIPTION nState, int nBuzzer)
{
	int nRed =	theConfigBank.m_Option.iMapLampSettingList[(STATUS_LAMP_DESCRIPTION)nState].Red.lamp_state;
	int nYellow = theConfigBank.m_Option.iMapLampSettingList[(STATUS_LAMP_DESCRIPTION)nState].Yellow.lamp_state;
	int nGreen = theConfigBank.m_Option.iMapLampSettingList[(STATUS_LAMP_DESCRIPTION)nState].Green.lamp_state;
	
	SetOutPutIO(Y_BUZZER_K1, OFF);
	SetOutPutIO(Y_BUZZER_K2, OFF);
	SetOutPutIO(Y_BUZZER_K3, OFF);
	SetOutPutIO(Y_BUZZER_K4, OFF);

	if(nBuzzer ==1 )
	{
		SetOutPutIO(Y_BUZZER_K1, ON);
	}
	else if(nBuzzer == 2)
	{
		SetOutPutIO(Y_BUZZER_K2, ON);
	}
	else if(nBuzzer == 3)
	{
		SetOutPutIO(Y_BUZZER_K3, ON);
	}
	else if(nBuzzer == 4)
	{
		SetOutPutIO(Y_BUZZER_K4, ON);
	}

	// RED
	if(nRed == LAMP_ON)
	{
		SetOutPutIO(Y_TOWER_LAMP_RED, ON);
		SetOutPutIO(Y_TOWER_LAMP_RED2, ON);
		GetMainHandler()->m_bChkLampR_Flick = FALSE;
	}
	else if(nRed == LAMP_OFF)
	{
		SetOutPutIO(Y_TOWER_LAMP_RED, OFF);
		SetOutPutIO(Y_TOWER_LAMP_RED2, OFF);
		GetMainHandler()->m_bChkLampR_Flick = FALSE;
	}
	else if(nRed == LAMP_FLICKER)
	{
		GetMainHandler()->m_bChkLampR_Flick = TRUE;
	}

	// YELLOW
	if(nYellow == LAMP_ON)
	{
		SetOutPutIO(Y_TOWER_LAMP_YELLOW, ON);
		SetOutPutIO(Y_TOWER_LAMP_YELLOW2, ON);
		GetMainHandler()->m_bChkLampY_Flick = FALSE;
	}
	else if(nYellow == LAMP_OFF)
	{
		SetOutPutIO(Y_TOWER_LAMP_YELLOW, OFF);
		SetOutPutIO(Y_TOWER_LAMP_YELLOW2, OFF);
		GetMainHandler()->m_bChkLampY_Flick = FALSE;
	}
	else if(nYellow == LAMP_FLICKER)
	{
		GetMainHandler()->m_bChkLampY_Flick = TRUE;
	}

	// GREEN
	if(nGreen == LAMP_ON)
	{		
		SetOutPutIO(Y_TOWER_LAMP_GREEN, ON);
		SetOutPutIO(Y_TOWER_LAMP_GREEN2, ON);
		GetMainHandler()->m_bChkLampG_Flick = FALSE;
	}
	else if(nGreen == LAMP_OFF)
	{
		SetOutPutIO(Y_TOWER_LAMP_GREEN, OFF);
		SetOutPutIO(Y_TOWER_LAMP_GREEN2, OFF);
		GetMainHandler()->m_bChkLampG_Flick = FALSE;
	}
	else if(nGreen == LAMP_FLICKER)
	{
		GetMainHandler()->m_bChkLampG_Flick = TRUE;
	}


}
/////////////////////////////////////////////////////////////////////////////////////////
// G-AMT
/////////////////////////////////////////////////////////////////////////////////////////
// 뺑뺑이 모드 사용 시 필요
void CUnitCtrlBank::Zone_CellInfo_Clear(CELL_POS cellPos)
{
	CCellTag tagCell;
	CCellInfo* pCell;

	pCell = theCellBank.GetCellInfo(cellPos);
	tagCell = theCellBank.GetCellTag(cellPos);

	if(tagCell.IsExist()==FALSE)
		return;

	pCell->m_Zone_B.Init();
	pCell->m_Zone_C.Init();
	pCell->m_Zone_D.Init();
}


void CUnitCtrlBank::Set_Table_CellData()
{
	CCellTag tagCell;
	CCellInfo* pCell;

	int i,j;
	for (i = CELL_POS_AZONE_CH1; i < CELL_POS_UD_TR_CH1; i++)
	{
		tagCell = theCellBank.CreateCellTag();
		theCellBank.SetCellTag((CELL_POS)i,tagCell);
	}

	for (i = 0; i < ZONE_ID_MAX; i++)
	{
		for (j = 0; j < JIG_CH_MAX; j++)
		{
			tagCell = theCellBank.GetCellTag((ZONE_ID)i,(JIG_CH)j);
			pCell = theCellBank.GetCellInfo((ZONE_ID)i,(JIG_CH)j);

			if(tagCell.IsExist()==FALSE)
				return ;

			pCell->Init();
			pCell->m_strInnerID.Format(_T("%d%s"), j+1, CTime::GetCurrentTime().Format(_T("%H%M%S")));
			pCell->m_strCellID.Format(_T("JIG%c_CH0%d"), _T('A') + i, j+1);
			pCell->m_JigId = (JIG_ID)i;
			pCell->m_JigCh = (JIG_CH)j;
			pCell->m_Zone_A.CellLoadingClass = GOOD_CELL;
			pCell->m_Zone_A.ContactCurrentClass = GOOD_CELL;
			pCell->m_Zone_A.AZoneClass = GOOD_CELL;
			pCell->m_Zone_A.ResultCellLoadingName = _T("GOOD");
			pCell->m_Zone_A.ContactCurrentName = _T("GOOD");
			pCell->m_Zone_A.AZoneDefect = _T("GOOD");
		}
	}
}

void CUnitCtrlBank::CellData_SetDryRun(int i)
{
	static int nCnt1 = 0;
	static int nCnt2 = 0;
	CCellTag tagCell;
	CCellInfo* pCell;

	if(i != 0)
	{
		nCnt1 = 0;
		nCnt2 = 0;
		return;
	}
	tagCell = theCellBank.CreateCellTag();

	theCellBank.SetCellTag(CELL_POS_LD_CONV,tagCell);
	pCell = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
	
	switch(theProcBank.m_nIDGenIndex)
	{
		case 1:
			pCell->m_strInnerID.Format(_T("1%s"), CTime::GetCurrentTime().Format(_T("%H%M%S")));
	 		pCell->m_strCellID.Format(_T("1A3MX1D6708QAH%d"), nCnt1);
			pCell->m_JigCh = JIG_CH_1;
			theProcBank.m_nIDGenIndex = 2;
			nCnt1++;
			break;
		case 2:
			pCell->m_strInnerID.Format(_T("2%s"), CTime::GetCurrentTime().Format(_T("%H%M%S")));
			pCell->m_strCellID.Format(_T("2A3MX1D6708QAH%d"), nCnt2);
			pCell->m_JigCh = JIG_CH_2;
			theProcBank.m_nIDGenIndex = 1;
			nCnt2++;
			break;
		default:
			break;
	}
}


void CUnitCtrlBank::CellData_AllClear()
{
	CCellTag tagCell;
	for(int i = CELL_POS_LD_CONV; i < CELL_POS_MAX; i++ )
		theCellBank.RemoveCellTag((CELL_POS)i);
}

void CUnitCtrlBank::CellData_LDConvIn()
{
	//CCellTag tagCell;
	CCellInfo* pCell;
	//2017-07-17,SKCHO, 초기에 TAG 생성하면서 CELL INFO 클래스 초기화
	///tagCell = theCellBank.CreateCellTag();
	//theCellBank.SetCellTag(CELL_POS_LD_CONV,tagCell);
	theCellBank.CreateCellInfo(CELL_POS_LD_CONV);
	pCell = theCellBank.GetCellInfo(CELL_POS_LD_CONV);

//	if (theProcBank.m_CellLoadChk)
//		theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("IDLE 변경 후 CELL 투입 시작."));

	theProcBank.m_CellLoadChkTact.Start();
	theProcBank.m_CellLoadChk = FALSE;



	// 2017.2.8 bgkim
	if(theConfigBank.m_Option.m_bUseJobEnd == TRUE)
	{
		theProcBank.m_timerJobEnd.Start();
	}
}

void CUnitCtrlBank::CellData_LDRobotFromLDConvIn()
{
	CCellTag tagCell;
	tagCell = theCellBank.RemoveCellTag(CELL_POS_LD_CONV);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_LDROBOT,tagCell);

	// IDLE 변경 시간 바뀌면서 적는 곳도 바꺼준다. 2017/06/20 HSK.
	if (theProcBank.m_CellLoadChk)
		theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("IDLE 변경 후 CELL 투입 시작."));
}

void CUnitCtrlBank::CellData_ShuttleFromLDRobot(CELL_POS cellPos)
{
	CCellTag tagCell;
	CCellInfo* pCell;
	tagCell = theCellBank.RemoveCellTag(CELL_POS_LDROBOT);
	if(tagCell.IsExist())
		theCellBank.SetCellTag(cellPos,tagCell);

	pCell = theCellBank.GetCellInfo(cellPos);
	if(cellPos == CELL_POS_UPPER_SHUTTLE_CH1 || cellPos == CELL_POS_LOWER_SHUTTLE_CH1)
	{
		pCell->m_strInnerID.Format(_T("1%s"), CTime::GetCurrentTime().Format(_T("%H%M%S")));
		pCell->m_JigCh = JIG_CH_1;
	}
	else
	{
		pCell->m_strInnerID.Format(_T("2%s"), CTime::GetCurrentTime().Format(_T("%H%M%S")));
		pCell->m_JigCh = JIG_CH_2;
	}
}

void CUnitCtrlBank::CellData_PATRFromShuttle(CELL_POS cellPos)
{
	CCellTag tagCell;
	if (cellPos == CELL_POS_UPPER_SHUTTLE_CH1)
	{
		tagCell = theCellBank.RemoveCellTag(CELL_POS_UPPER_SHUTTLE_CH1);
		if(tagCell.IsExist())
		theCellBank.SetCellTag(CELL_POS_PA_TR_CH1,tagCell);

		tagCell = theCellBank.RemoveCellTag(CELL_POS_UPPER_SHUTTLE_CH2);
		if(tagCell.IsExist())
		theCellBank.SetCellTag(CELL_POS_PA_TR_CH2,tagCell);
	}
	else if(CELL_POS_LOWER_SHUTTLE_CH1)
	{
		tagCell = theCellBank.RemoveCellTag(CELL_POS_LOWER_SHUTTLE_CH1);
		if(tagCell.IsExist())
		theCellBank.SetCellTag(CELL_POS_PA_TR_CH1,tagCell);

		tagCell = theCellBank.RemoveCellTag(CELL_POS_LOWER_SHUTTLE_CH2);
		if(tagCell.IsExist())
		theCellBank.SetCellTag(CELL_POS_PA_TR_CH2,tagCell);
	}
}

void CUnitCtrlBank::CellData_PAStageFromPATR()
{
	CCellTag tagCell;
	tagCell = theCellBank.RemoveCellTag(CELL_POS_PA_TR_CH1);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_PA_STAGE_CH1,tagCell);

	tagCell = theCellBank.RemoveCellTag(CELL_POS_PA_TR_CH2);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_PA_STAGE_CH2,tagCell);
}

void CUnitCtrlBank::CellData_LDTRFromPAStage()
{
	CCellTag tagCell;
	tagCell = theCellBank.RemoveCellTag(CELL_POS_PA_STAGE_CH1);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_LD_TR_CH1,tagCell);

	tagCell = theCellBank.RemoveCellTag(CELL_POS_PA_STAGE_CH2);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_LD_TR_CH2,tagCell);
}

void CUnitCtrlBank::CellData_AMTFromLDTR()
{
	CCellTag tagCell;
	CCellInfo* pCellInfo= NULL;
	tagCell = theCellBank.RemoveCellTag(CELL_POS_LD_TR_CH1);
	if(tagCell.IsExist())
	{
		theCellBank.SetCellTag(CELL_POS_AZONE_CH1,tagCell);
	}

	tagCell = theCellBank.RemoveCellTag(CELL_POS_LD_TR_CH2);
	if(tagCell.IsExist())
	{
		theCellBank.SetCellTag(CELL_POS_AZONE_CH2,tagCell);
	}

	//20170129 byskcho
	//cim oss 파일 변경으로 인한 추가	
	//for(int nJig_Ch = 0; nJig_Ch < JIG_CH_MAX; nJig_Ch++)
	//{
	//	//pCellInfo = theCellBank.SearchCellInfo((JIG_ID)JIG_ID_A, (JIG_CH)nJig_Ch, CELL_POS_AZONE_CH1, CELL_POS_DZONE_CH2);
	//	pCellInfo = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1+nJig_Ch));
	//	if(pCellInfo != NULL)
	//	{
	//		theFDCBank.m_strJig_CellID[JIG_ID_A][nJig_Ch] = pCellInfo->m_strCellID;
	//		theFDCBank.m_nJig_StepID[JIG_ID_A][nJig_Ch] = 1;
	//		
	//	}
	//	else
	//	{
	//		theFDCBank.m_strJig_CellID[JIG_ID_A][nJig_Ch] = _T("");
	//		theFDCBank.m_nJig_StepID[JIG_ID_A][nJig_Ch] = 0;
	//	}
	//}

}

void CUnitCtrlBank::CellData_ToNextPos(BOOL bReverse)
{
	int nStart = 0;
	int nCnt = 0;
	CCellInfo* pCellInfo= NULL;	

	CCellTag tag[JIG_CH_MAX] = {theCellBank.GetCellTag(CELL_POS_AZONE_CH1),	theCellBank.GetCellTag(CELL_POS_AZONE_CH2)};
	
	//정방향
	theCellBank.SetCellTagNextPositionZone(ZONE_ID_D, ZONE_ID_A);
	theCellBank.SetCellTagNextPositionZone(ZONE_ID_C, ZONE_ID_D);
	theCellBank.SetCellTagNextPositionZone(ZONE_ID_B, ZONE_ID_C);
	theCellBank.SetCellTag(CELL_POS_BZONE_CH1, tag[0]);
	theCellBank.SetCellTag(CELL_POS_BZONE_CH2, tag[1]);

	for(int nJig_ID = JIG_ID_A; nJig_ID< JIG_ID_MAX; nJig_ID++)
	{
		nCnt = 0;
		//for(int nJig_Ch = JIG_CH_1; nJig_Ch < JIG_CH_MAX; nJig_Ch++)
		//{
		//	//20170129 byskcho
		//	//cim oss 파일 변경으로 인한 추가
		//	if(nJig_ID == JIG_ID_A )
		//	{
		//		nStart = CELL_POS_AZONE_CH1+nCnt;
		//	}
		//	else if(nJig_ID == JIG_ID_B)
		//	{
		//		nStart = CELL_POS_BZONE_CH1+nCnt;
		//	}
		//	else if(nJig_ID == JIG_ID_C)
		//	{
		//		nStart = CELL_POS_CZONE_CH1+nCnt;
		//	}
		//	else if(nJig_ID == JIG_ID_D)
		//	{
		//		nStart = CELL_POS_DZONE_CH1+nCnt;
		//	}
		//	
		//	pCellInfo = theCellBank.GetCellInfo((CELL_POS)nStart);
		//	if(pCellInfo != NULL)
		//	{
		//		if(nJig_ID == JIG_ID_A)
		//		{
		//			theFDCBank.m_strJig_CellID[nJig_ID][nJig_Ch] = _T("");
		//			theFDCBank.m_nJig_StepID[nJig_ID][nJig_Ch] = 0;
		//		}
		//		else
		//		{
		//			theFDCBank.m_strJig_CellID[nJig_ID][nJig_Ch] = pCellInfo->m_strCellID;
		//			if(pCellInfo->m_Zone_C.InspVIState == eSTATE_Insp_Run)
		//			{
		//				theFDCBank.m_nJig_StepID[nJig_ID][nJig_Ch] = 2;
		//			}
		//			else
		//			{
		//				theFDCBank.m_nJig_StepID[nJig_ID][nJig_Ch] = 1;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		theFDCBank.m_strJig_CellID[nJig_ID][nJig_Ch] = _T("");
		//		theFDCBank.m_nJig_StepID[nJig_ID][nJig_Ch] = 0;
		//	}
		//	nCnt++;
		//}		
	}
	
}

void CUnitCtrlBank::CellData_UDTRFromAMT()
{
	CCellInfo* pCellInfo= NULL;

	CCellTag tagCell;
	tagCell = theCellBank.RemoveCellTag(CELL_POS_DZONE_CH1);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_UD_TR_CH1,tagCell);

	tagCell = theCellBank.RemoveCellTag(CELL_POS_DZONE_CH2);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_UD_TR_CH2,tagCell);

		//20170129 byskcho
	//cim oss 파일 변경으로 인한 추가	
	/*for(int nJig_Ch = 0; nJig_Ch < JIG_CH_MAX; nJig_Ch++)
	{
		pCellInfo = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_UD_TR_CH1+nJig_Ch));
		if(pCellInfo != NULL)
		{
			theFDCBank.m_strULDTR_CellID[nJig_Ch] = pCellInfo->m_strCellID;
			theFDCBank.m_nULDTR_StepID[nJig_Ch] = 1;
			
		}
		else
		{
			theFDCBank.m_strULDTR_CellID[nJig_Ch] = _T("");
			theFDCBank.m_nULDTR_StepID[nJig_Ch] = 0;
		}
	}*/

	//20170129 byskcho
	//cim oss 파일 변경으로 인한 추가
	/*for(int nJig_Ch = 0; nJig_Ch < JIG_CH_MAX; nJig_Ch++)
	{
		theFDCBank.m_strJig_CellID[JIG_ID_D][nJig_Ch] = _T("");
		theFDCBank.m_nJig_StepID[JIG_ID_D][nJig_Ch] = 0;
	}*/
}

void CUnitCtrlBank::CellData_UDStageFromUDTR()
{
	CCellTag tagCell;
	CCellInfo* pCellInfo= NULL;
	tagCell = theCellBank.RemoveCellTag(CELL_POS_UD_TR_CH1);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_UD_STAGE_CH1,tagCell);

	tagCell = theCellBank.RemoveCellTag(CELL_POS_UD_TR_CH2);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_UD_STAGE_CH2,tagCell);
	//20170129 byskcho
	//cim oss 파일 변경으로 인한 추가	
	/*for(int nJig_Ch = 0; nJig_Ch < JIG_CH_MAX; nJig_Ch++)
	{
		pCellInfo = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_UD_STAGE_CH1+nJig_Ch));
		if(pCellInfo != NULL)
		{
			theFDCBank.m_strULDStage_CellID[nJig_Ch] = pCellInfo->m_strCellID;
			theFDCBank.m_nULDStage_StepID[nJig_Ch] = 1;
			
		}
		else
		{
			theFDCBank.m_strULDStage_CellID[nJig_Ch] = _T("");
			theFDCBank.m_nULDStage_StepID[nJig_Ch] = 0;
		}
	}*/

		//20170129 byskcho
	//cim oss 파일 변경으로 인한 추가
	/*for(int nJig_Ch = 0; nJig_Ch < JIG_CH_MAX; nJig_Ch++)
	{
		theFDCBank.m_strULDTR_CellID[nJig_Ch] = _T("");
		theFDCBank.m_nULDTR_StepID[nJig_Ch] = 0;
	}*/
	
}

void CUnitCtrlBank::CellData_UDRobotFromUDStage(CELL_POS cellPos)
{
	CCellTag tagCell;
	
	tagCell = theCellBank.RemoveCellTag(cellPos);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_UD_ROBOT,tagCell);

	//20170129 byskcho
	//cim oss 파일 변경으로 인한 추가
	/*if(cellPos == CELL_POS_UD_STAGE_CH1)
	{
		theFDCBank.m_strULDStage_CellID[0] = _T("");
		theFDCBank.m_nULDStage_StepID[0] = 0;
	}
	else
	{
		theFDCBank.m_strULDStage_CellID[1] = _T("");
		theFDCBank.m_nULDStage_StepID[1] = 0;
	}*/

}
//20170207 byskcho
void CUnitCtrlBank::CellData_OKTrayFromUDRobot(CELL_POS cellPos)
{
	CCellTag tagCell;
	tagCell = theCellBank.RemoveCellTag(cellPos);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_GOOD_TRAY,tagCell);
}

//2017-03-03, skcho, NG TRAY도 셀 정보 이동하게 함.OK와 동일하게 하기 위해서
void CUnitCtrlBank::CellData_NGTrayFromUDRobot(CELL_POS cellPos)
{
	CCellTag tagCell;
	
	tagCell = theCellBank.RemoveCellTag(cellPos);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_NG_TRAY,tagCell);
}
//2018-05-16,MGYUN, ULD CONV로 셀 정보 이동 하게,
void CUnitCtrlBank::CellData_ULDConvFromUDRobot(CELL_POS cellPos)
{
	CCellTag tagCell;
	
	tagCell = theCellBank.RemoveCellTag(cellPos);
	if(tagCell.IsExist())
	theCellBank.SetCellTag(CELL_POS_UD_CONV,tagCell);
}

//2018wjp
void CUnitCtrlBank::CellData_TrayFromRobot(CELL_POS cellPos,int iRow,int Col,BOOL isClear)
{
	CCellTag tagCell;
	if(isClear)
		tagCell = theCellBank.RemoveCellTag(cellPos);
	else
		tagCell = theCellBank.GetCellTag(cellPos);
	if(tagCell.IsExist())
	{
		if(1==iRow)
		{
			if(1==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_11,tagCell);
			else if(2==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_12,tagCell);
			else if(3==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_13,tagCell);
			else if(4==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_14,tagCell);
			else if(5==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_15,tagCell);
			else
				return;
		}
		else if(2==iRow)
		{
			if(1==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_21,tagCell);
			else if(2==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_22,tagCell);
			else if(3==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_23,tagCell);
			else if(4==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_24,tagCell);
			else if(5==Col)
				theCellBank.SetCellTag(CELL_POS_TRAY_25,tagCell);
			else
				return;
		}
	}
}

//2018wjp
void CUnitCtrlBank::CellData_RobotFromTray(int iRow,int Col)
{
	CCellTag tagCell;
	CELL_POS cellPos;
	if(1==iRow)
	{
		if(1==Col)
			cellPos=CELL_POS_TRAY_11;
		else if(2==Col)
			cellPos=CELL_POS_TRAY_12;
		else if(3==Col)
			cellPos=CELL_POS_TRAY_13;
		else if(4==Col)
			cellPos=CELL_POS_TRAY_14;
		else if(5==Col)
			cellPos=CELL_POS_TRAY_15;
		else
			return;
	}
	else if(2==iRow)
	{
		if(1==Col)
			cellPos=CELL_POS_TRAY_21;
		else if(2==Col)
			cellPos=CELL_POS_TRAY_22;
		else if(3==Col)
			cellPos=CELL_POS_TRAY_23;
		else if(4==Col)
			cellPos=CELL_POS_TRAY_24;
		else if(5==Col)
			cellPos=CELL_POS_TRAY_25;
		else
			return;
	}
	tagCell = theCellBank.RemoveCellTag(cellPos);
	if(tagCell.IsExist())
	{
		theCellBank.SetCellTag(CELL_POS_UD_ROBOT,tagCell);
	}

}

//2018wjp
void CUnitCtrlBank::CellData_Tray_ClearAll()
{
	for(int i=CELL_POS_TRAY_11;i<=CELL_POS_TRAY_25;i++)
	{
		theCellBank.RemoveCellTag((CELL_POS)i);
	}
}

//2018wjp
void CUnitCtrlBank::CellData_Tray_CreateAll()
{
	for(int i=CELL_POS_TRAY_11;i<=CELL_POS_TRAY_25;i++)
	{
		theCellBank.RemoveCellTag((CELL_POS)i);
		theCellBank.CreateCellInfo((CELL_POS)i);
		switch(i)
		{
		case CELL_POS_TRAY_11:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[0][0];
			break;
		case CELL_POS_TRAY_12:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[0][1];
			break;
		case CELL_POS_TRAY_13:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[0][2];
			break;
		case CELL_POS_TRAY_14:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[0][3];
			break;
		case CELL_POS_TRAY_15:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[0][4];
			break;
		case CELL_POS_TRAY_21:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[1][0];
			break;
		case CELL_POS_TRAY_22:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[1][1];
			break;
		case CELL_POS_TRAY_23:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[1][2];
			break;
		case CELL_POS_TRAY_24:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[1][3];
			break;
		case CELL_POS_TRAY_25:
			theCellBank.GetCellInfo((CELL_POS)i)->m_strCellID=theProcBank.strCIMLoadCellID[1][4];
			break;
		default:
			break;
		}
	}
}

//2018-05-16,MGYUN, ULD CONV 셀정보 삭제
void CUnitCtrlBank::CellData_ULDConvRemove()
{
	CCellTag tagCell;
	theCellBank.RemoveCellTag(CELL_POS_UD_CONV);
}
void CUnitCtrlBank::CellData_UDRobotRemove()
{
	CCellTag tagCell;
	theCellBank.RemoveCellTag(CELL_POS_UD_ROBOT);
}
//20170207 byskcho
void CUnitCtrlBank::CellData_OKTrayRemove()
{
	CCellTag tagCell;
	theCellBank.RemoveCellTag(CELL_POS_GOOD_TRAY);
}

//20170303 byskcho
void CUnitCtrlBank::CellData_NGTrayRemove()
{
	CCellTag tagCell;
	theCellBank.RemoveCellTag(CELL_POS_NG_TRAY);
}
BOOL CUnitCtrlBank::CellTagExist( CELL_POS pos )
{
	return theCellBank.GetCellTag(pos).IsExist();
}

BOOL CUnitCtrlBank::CellTagExist( CELL_POS posStart, CELL_POS posEnd, CHECK_OPTION chkoption /*= CHECK_OR*/ )
{
	// 하나라도 비어있으면 FALSE [11/5/2016 OSC]
	if(chkoption == CHECK_AND)
	{
		for(int i = posStart; i <= posEnd; i++)
		{
			if(theCellBank.GetCellTag((CELL_POS)i).IsExist() == FALSE)
				return FALSE;
		}
		return TRUE;
	}
	// 하나라도 있으면 TRUE [11/5/2016 OSC]
	else
	{
		for(int i = posStart; i <= posEnd; i++)
		{
			if(theCellBank.GetCellTag((CELL_POS)i).IsExist())
				return TRUE;
		}
		return FALSE;
	}

}

BOOL CUnitCtrlBank::CellDataMachineChk(CELL_POS posStart,CELL_POS posEnd)
{
	CCellTag tag;
	BOOL bExist = FALSE;

	for (int i = posStart; i <= posEnd; i++)
	{
		tag = theCellBank.GetCellTag((CELL_POS)i);
		if(tag.IsExist())
		{
			bExist = TRUE;
			break;
		}
	}

	return bExist;
}

CCellTag CUnitCtrlBank::SearchAvtCell(CString strInnerID, ZONE_ID startZone, ZONE_ID endZone)
{
	CCellTag	tagCell;
	CCellInfo   *pCell;

	for(int zone = startZone; zone <= endZone; zone++)
	{
		for(int i = 0; i < JIG_CH_MAX; i++)
		{
			tagCell = theCellBank.GetCellTag((ZONE_ID)zone,(JIG_CH)i);
			pCell = theCellBank.GetCellInfo(tagCell);
			if(tagCell.IsExist())
			{
				if(strInnerID.Compare(pCell->m_strInnerID)==0)	
					return tagCell;
			}
		}
	}
	tagCell.Init();

	return tagCell;
}

BOOL CUnitCtrlBank::TrayData_Exist( TRAY_PORT port, TRAY_INFO_POS pos )
{
	return theProcBank.m_TrayBank.GetTrayInfo(port, pos)->IsExist();
}

void CUnitCtrlBank::TrayData_Create( TRAY_PORT port,TRAY_INFO_POS Setpos)
{
	CTrayInfo info(TRUE);
	theProcBank.m_TrayBank.SetTrayInfo(port, Setpos, info);
}

//2018wjp
void CUnitCtrlBank::TrayData_CreateEx( TRAY_PORT port,TRAY_INFO_POS Setpos)
{
	CTrayInfo info(TRUE,TRUE);
	theProcBank.m_TrayBank.SetTrayInfo(port, Setpos, info);
}

void CUnitCtrlBank::TrayData_RemoveData( TRAY_PORT port, TRAY_INFO_POS pos )
{
	theProcBank.m_TrayBank.RemoveTrayInfo(port, pos);
}

void CUnitCtrlBank::TrayData_from_to( TRAY_PORT port , TRAY_INFO_POS Removepos , TRAY_INFO_POS Setpos)
{
	CTrayInfo info = theProcBank.m_TrayBank.RemoveTrayInfo(port, Removepos);
	theProcBank.m_TrayBank.SetTrayInfo(port, Setpos, info);
}


void CUnitCtrlBank::TrayData_from_toAlign( TRAY_PORT port , TRAY_INFO_POS Removepos , TRAY_INFO_POS Setpos)
{
	CTrayInfo info = theProcBank.m_TrayBank.RemoveTrayInfo(port, Removepos);
	theProcBank.m_TrayBank.SetTrayInfo(port, Removepos, info);
	theProcBank.m_TrayBank.SetTrayInfo(port, Setpos, info);
}

BOOL CUnitCtrlBank::TrayData_CellIsEmpty(TRAY_PORT port, TRAY_INFO_POS pos)
{
	if(theProcBank.m_TrayBank.GetTrayInfo(port, pos)->IsExist() == FALSE)
		return FALSE;
	return theProcBank.m_TrayBank.GetTrayInfo(port, pos)->IsCellEmpty();
}

BOOL CUnitCtrlBank::TrayData_CellIsFull( TRAY_PORT port, TRAY_INFO_POS pos )
{
	if(theProcBank.m_TrayBank.GetTrayInfo(port, pos)->IsExist() == FALSE)
		return FALSE;
	return theProcBank.m_TrayBank.GetTrayInfo(port, pos)->IsCellFull();
}

BOOL CUnitCtrlBank::TrayData_TrayIsFull( TRAY_PORT port, TRAY_INFO_POS pos )
{
	if(theProcBank.m_TrayBank.GetTrayInfo(port, pos)->IsExist() == FALSE)
		return FALSE;
	return theProcBank.m_TrayBank.GetTrayInfo(port, pos)->GetTrayFull();
}

BOOL CUnitCtrlBank::TrayData_ExistCell(TRAY_PORT port, TRAY_INFO_POS pos)
{
	if(theProcBank.m_TrayBank.GetTrayInfo(port, pos)->IsExist() == FALSE)
		return FALSE;
	return theProcBank.m_TrayBank.GetTrayInfo(port, pos)->IsCell();
}

BOOL CUnitCtrlBank::TheachMotionChk(MP2100_THREAD thr, TEACH_PARAM::ID idTeach)
{

	if(theDeviceMotion.CheckTeachPosition(thr, idTeach)==TRUE)
		return TRUE;
		
	return FALSE;

}

BOOL CUnitCtrlBank::GetZoneEnd( ZONE_ID zone )
{
	return m_bZoneEnd[zone];
}

void CUnitCtrlBank::SetZoneEnd( ZONE_ID zone, BOOL bValue )
{
	if(bValue)
	{
		if(zone == ZONE_ID_MAX)
		{
			// ZONE_MAX일 경우 Zone End시간 기록 안된거는 여기서 다 기록한다 [6/18/2013 OSC]
			for(int i = 0; i < ZONE_ID_MAX; i++)
			{
				if(m_bZoneEnd[i] == FALSE)
				{
					m_bZoneEnd[i] = TRUE;
					ZoneTimeEnd((ZONE_ID)i);
					theLog[LOG_SEQUENCE].AddBuf(_T("[ZONE_%c] End"), _T('A')+i);
				}
			}
		}
		else
		{
			if(m_bZoneEnd[zone] == FALSE)
			{
				m_bZoneEnd[zone] = TRUE;
				ZoneTimeEnd(zone);
				theLog[LOG_SEQUENCE].AddBuf(_T("[ZONE_%c] End"), _T('A')+zone);
			}
		}
	}
	else
	{
		if(zone == ZONE_ID_MAX)
		{
			for(int i = 0; i < ZONE_ID_MAX; i++)
				m_bZoneEnd[i] = FALSE;
		}
		else
		{
			m_bZoneEnd[zone] = FALSE;
		}
	}
}

SYSTEMTIME	CUnitCtrlBank::m_systmA_StartTime;
void CUnitCtrlBank::ZoneTimeStart()
{
	CCellInfo* pCell;
	CCellTag tagCell;

	for (int zone = 0; zone < ZONE_ID_MAX; zone++)
	{
		if(zone == ZONE_ID_A)
		{
			// A존은 Cell이 놓여지기 전에 Start하므로 따로 담아놨다가 넣는다 [6/12/2013 OSC]
			GetLocalTime(&CUnitCtrlBank::m_systmA_StartTime);		
		}
		else
		{			

			for (int i = 0; i < JIG_CH_MAX; i++)
			{
				tagCell = theCellBank.GetCellTag((ZONE_ID)zone, (JIG_CH)i);
				if(tagCell.IsExist())
				{
					pCell = theCellBank.GetCellInfo(tagCell);
					switch(zone)
					{
					case ZONE_ID_B: GetLocalTime(&pCell->cellTime.systmB_StartTime); break;
					case ZONE_ID_C: GetLocalTime(&pCell->cellTime.systmC_StartTime); break;
					case ZONE_ID_D: GetLocalTime(&pCell->cellTime.systmD_StartTime); break;
					}
				}
			}
		}
	}
	theLog[LOG_SEQUENCE].AddBuf( _T("[ZONE_A] Start"));
	theLog[LOG_SEQUENCE].AddBuf( _T("[ZONE_B] Start"));
	theLog[LOG_SEQUENCE].AddBuf( _T("[ZONE_C] Start"));
	theLog[LOG_SEQUENCE].AddBuf( _T("[ZONE_D] Start"));
}

void CUnitCtrlBank::ZoneTimeEnd( ZONE_ID zone )
{
	CCellInfo* pCell;
	CCellTag tagCell;

	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		tagCell = theCellBank.GetCellTag(zone, (JIG_CH) i);
		if(tagCell.IsExist())
		{
			pCell = theCellBank.GetCellInfo(tagCell);
			switch(zone)
			{
				case ZONE_ID_A: 
					pCell->cellTime.systmA_StartTime = CUnitCtrlBank::m_systmA_StartTime;
					GetLocalTime(&pCell->cellTime.systmA_EndTime); 
					break;
				case ZONE_ID_B: 
					GetLocalTime(&pCell->cellTime.systmB_EndTime); 
					break;
				case ZONE_ID_C: 
					GetLocalTime(&pCell->cellTime.systmC_EndTime); 
					break;
				case ZONE_ID_D: 
					GetLocalTime(&pCell->cellTime.systmD_EndTime); 
					break;
			}
		}
	}
}

void CUnitCtrlBank::WaitTimeEnd()
{
	CCellInfo* pCell;
	CCellTag tagCell;

	for (int i = 0; i < JIG_CH_MAX; i++)
	{		
		tagCell = theCellBank.GetCellTag(ZONE_ID_A, (JIG_CH)i);
		if(tagCell.IsExist())
		{
			pCell = theCellBank.GetCellInfo(tagCell);
			GetLocalTime(&pCell->cellTime.systmA_WaitTime);
		}
		tagCell = theCellBank.GetCellTag(ZONE_ID_B, (JIG_CH)i);
		if(tagCell.IsExist())
		{
			pCell = theCellBank.GetCellInfo(tagCell);
			GetLocalTime(&pCell->cellTime.systmB_WaitTime);
		}
		tagCell = theCellBank.GetCellTag(ZONE_ID_C, (JIG_CH)i);
		if(tagCell.IsExist())
		{
			pCell = theCellBank.GetCellInfo(tagCell);
			GetLocalTime(&pCell->cellTime.systmC_WaitTime);
		}
		tagCell = theCellBank.GetCellTag(ZONE_ID_D, (JIG_CH)i);
		if(tagCell.IsExist())
		{
			pCell = theCellBank.GetCellInfo(tagCell);
			GetLocalTime(&pCell->cellTime.systmD_WaitTime);
		}
	}
}

BOOL CUnitCtrlBank::GetTurnTableMoveReadyAxisCheck()
{
	BYTE bRtn[8], bRtnComp[8];
	memset(bRtnComp, TRUE, sizeof(BYTE)*8);
	memset(bRtn, TRUE, sizeof(BYTE)*8);

	if(GetAxisCheck(TEACH_PARAM::LD_TR_Z_UP))
	bRtn[0] = TRUE;
	//TEACH_PARAM::UD_TR_X_to_AZONE과UD_TR_X_to_UD_SHUTTLE사이 );
	bRtn[1] = GetAxisCheck(TEACH_PARAM::TT_VALIGN_CAM_Y_READY);//이넘 추가되어서 수정되어야 함
	bRtn[2] = GetAxisCheck(TEACH_PARAM::TT_VALIGN_CAM_X_READY);
	bRtn[3] = GetAxisCheck(TEACH_PARAM::ULD_TR_Z_UP);
	
	bRtn[4] = GetAxisReadyCheck(AXIS_SVO_LD_TR_Z);
	bRtn[5] = GetAxisReadyCheck(AXIS_SVO_ALIGN_CAM_Y);
	bRtn[6] = GetAxisReadyCheck(AXIS_STP_ALIGN_CAM_X);
	bRtn[7] = GetAxisReadyCheck(AXIS_SVO_ULD_TR_Z);

	return (memcmp(bRtn, bRtnComp, 8) == 0) ? TRUE:FALSE;
}

//축들이 원점을 잡을때 인터락
BOOL CUnitCtrlBank::CheckOriginInterlock(AXIS_ID idAxis)
{
	m_strLastKorMsg = _T("");
	m_strLastEngMsg = _T("");
	m_strLastVnmMsg = _T("");
	BOOL bRet = TRUE;

	// 뮤팅 동작 할 때 모터 이동 버튼은 전부 막아 버렸다 2017/06/08 HSK.
	if(theProcBank.m_bNGTrayMuteOn == TRUE || theProcBank.m_bGoodInTrayMuteOn == TRUE || theProcBank.m_bGoodOutTrayMuteOn)
	{
		m_strLastKorMsg = _T("Mute Ramp가 켜져 있어, 모터를 구동 할 수 없습니다.");
		m_strLastEngMsg = _T("Mute Ramp On, Can not Motor Move.");
		m_strLastVnmMsg = _T("Mute Ramp On, Can not Motor Move.");
		return FALSE;
	}

	switch(idAxis)
	{
	case AXIS_SVO_LD_UPPERSHUTTLE_Y:
		break;
	case AXIS_SVO_LD_LOWERSHUTTLE_Y:
		break;
	case AXIS_SVO_PREALIGN_TR_Y:
		if (Chk_PATR_ZAxis_UpPos() == FALSE)
		{
			m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
			m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
			bRet = FALSE;
		}
		break;
	case AXIS_SVO_PREALIGN_TR_Z:
		break;
	case AXIS_SVO_PA_STAGE_X:
		if (Chk_PATR_ZAxis_UpPos() == FALSE)
		{
			m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
			m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
			bRet = FALSE;
		}
		break;
	case AXIS_SVO_LD_TR_X:
		if (LDTRZUpPosChk() == FALSE)
		{
			m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
			m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
			bRet = FALSE;
		}
		break;
	case AXIS_SVO_LD_TR_Z:
		break;
	case AXIS_SVO_TURN_TABLE_T:
		if (LDTRZUpPosChk() == FALSE)
		{
			m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
			m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
			bRet = FALSE;
		}
		else if (B_Zone_Cam_X_Ready_Chk() == FALSE)
		{
			m_strLastKorMsg = _T("VISION ALIGN X를 대기 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Ready Position Vision Align X.");
			m_strLastVnmMsg = _T("Please Move Ready Position Vision Align X.");
			bRet = FALSE;
		}
		else if (B_Zone_Cam_Y_Ready_Chk() == FALSE)
		{
			m_strLastKorMsg = _T("VISION ALIGN Y를 대기 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Ready Position Vision Align Y.");
			m_strLastVnmMsg = _T("Please Move Ready Position Vision Align Y.");
			bRet = FALSE;
		}
		else if (MoveUnLoadTR_ZAxis_UpChk() == FALSE)
		{
			m_strLastKorMsg = _T("ULD TR Z를 상승 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Up Position ULD TR Z.");
			m_strLastVnmMsg = _T("Please Move Up Position ULD TR Z.");
			bRet = FALSE;
		}
		break;
	case AXIS_SVO_ALIGN_CAM_Y:
		break;
	case AXIS_SVO_INS_CH1_Z:
		break;
	case AXIS_SVO_INS_CH2_Z:
		break;
	case AXIS_SVO_INS_X:
		break;
	case AXIS_SVO_ULD_TR_X:
		if (MoveUnLoadTR_ZAxis_UpChk() == FALSE)
		{
			m_strLastKorMsg = _T("ULD TR Z를 상승 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Up Position ULD TR Z.");
			m_strLastVnmMsg = _T("Please Move Up Position ULD TR Z.");
			bRet = FALSE;
		}
		break;
	case AXIS_SVO_ULD_TR_Z:
		break;
	case AXIS_SVO_GOOD_TRAY_IN_LIFT_Z:
		break;
	case AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z:
		break;
	case AXIS_SVO_GOOD_TRAY_TR_Y:
		if (TrayTrZ_UpChk() == FALSE)
		{
			m_strLastKorMsg = _T("Good Tray TR Z를 상승 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Up Position Good Tray TR Z.");
			m_strLastVnmMsg = _T("Please Move Up Position Good Tray TR Z.");
			bRet = FALSE;
		}
		break;
	case AXIS_SVO_GOOD_TRAY_TR_Z:
		break;
	case AXIS_SVO_NG_TRAY_IN_LIFT_Z:
		if(NGLiftArmFwdBwdChk(NG_TRAY_PORT,CYL_FWD) == FALSE)
		{
			m_strLastKorMsg = _T("NG TRAY IN LIFT Z ARM 전진 시켜 주세요");
			m_strLastEngMsg = _T("NG TRAY IN LIFT Z ARM Forward Please.");
			m_strLastVnmMsg = _T("NG TRAY IN LIFT Z ARM Forward Please.");
			bRet = FALSE;
		}
		break;
	case AXIS_SVO_NG_TRAY_OUT_LIFT_Z:
		break;
	case AXIS_SVO_NG_TRAY_TR_X:
		if (NGTrayTrZ_UpChk() == FALSE)
		{
			m_strLastKorMsg = _T("NG Tray TR Z를 상승 위치로 이동 시켜 주세요");
			m_strLastEngMsg = _T("Please Move Up Position NG Tray TR Z.");
			m_strLastVnmMsg = _T("Please Move Up Position NG Tray TR Z.");
			bRet = FALSE;
		}
		break;
	case AXIS_SVO_NG_TRAY_TR_Z:
		break;
	case AXIS_STP_PA_CH1_X:
		break;
	case AXIS_STP_PA_CH1_Y:
		break;
	case AXIS_STP_PA_CH1_T:
		break;
	case AXIS_STP_PA_CH2_X:
		break;
	case AXIS_STP_PA_CH2_Y:
		break;
	case AXIS_STP_PA_CH2_T:
		break;
	case AXIS_STP_ALIGN_CAM_X:
		break;
	case AXIS_STP_INS_CH1_T:
		break;
	case AXIS_STP_INS_CH1_F:
		break;
	case AXIS_STP_INS_CH2_T:
		break;
	case AXIS_STP_INS_CH2_F:
		break;
	}
	return bRet;
}


BOOL CUnitCtrlBank::CheckTeachMoveInterlock(TEACH_PARAM::ID idTeach, double dOffset /*= 0.*/,BOOL RunState )
{
	// CDeviceMotion::TeachMove에서 축에 이동명령을 내리기 전에 확인하는 Interlock [10/26/2016 OSC]
	// BOOL로 return하며, 관련 Msg를 따로 멤버변수에 남기므로 티칭 UI에서는 아래 변수 Text를 MsgBox로 띄우면 됨
	m_strLastKorMsg = _T("");
	m_strLastEngMsg = _T("");
	m_strLastVnmMsg = _T("");
	BOOL bRet = TRUE;
	switch(idTeach)
	{
		case TEACH_PARAM::LD_UPPERSHUTTLE_LOAD:
			if(RunState)
			{
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
			}
			else
			{
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
				//2017_05_23, JSJUNG. 어퍼셔틀에 프리얼라인TR Z축이 UP위치 상태일때 이동가능
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::LD_UPPERSHUTTLE_UNLOAD:
			if(RunState)
			{
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
			}
			else
			{
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
				//2017_05_23, JSJUNG. 어퍼셔틀에 프리얼라인TR Z축이 UP위치 상태일때 이동가능
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
			}

			break;
		case TEACH_PARAM::LD_LOWERSHUTTLE_LOAD:
			if(RunState)
			{
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
				//2017-06-22 JSJUNG. LOWERSHUTTLE이동시 로봇 어퍼셔틀 위치의 움직임 무시
				//if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT) == FALSE)
				//{
				//	m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
				//	m_strLastEngMsg = _T("Please Move LD Robot.");
				//	m_strLastVnmMsg = _T("Please Move LD Robot");
				//	bRet = FALSE;
				//}
			}
			else
			{
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
				//2017_05_23, JSJUNG. 로우셔틀에 프리얼라인TR Z축이 UP위치 상태일때 이동가능
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
			}

			break;
		case TEACH_PARAM::LD_LOWERSHUTTLE_UNLOAD:
			if(RunState)
			{
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
				//2017-06-22 JSJUNG. LOWERSHUTTLE이동시 로봇 어퍼셔틀 위치의 움직임 무시
				//if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT) == FALSE)
				//{
				//	m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
				//	m_strLastEngMsg = _T("Please Move LD Robot.");
				//	m_strLastVnmMsg = _T("Please Move LD Robot");
				//	bRet = FALSE;
				//}
			}
			else
			{
				//2017_05_23, JSJUNG. 로우셔틀에 프리얼라인TR Z축이 UP위치 상태일때 이동가능
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
				if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
				{
					m_strLastKorMsg = _T("LOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move LD Robot.");
					m_strLastVnmMsg = _T("Please Move LD Robot");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::PA_TR_Y_GET:
			if(RunState)
			{
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::PA_TR_Y_PUT:
			if(RunState)
			{
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::PA_TR_Y_WAIT:
			if(RunState)
			{
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::PA_TR_Z_UPPER_GET:
			if(RunState)
			{
				if (Chk_PATR_YAxis_GetPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Y을 집을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Get Position PA TR Y.");
					m_strLastVnmMsg = _T("Please Move Get Position PA TR Y.");
					bRet = FALSE;
				}
			}
			else
			{
				if (Chk_PATR_YAxis_GetPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Y을 집을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Get Position PA TR Y.");
					m_strLastVnmMsg = _T("Please Move Get Position PA TR Y.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::PA_TR_Z_LOWER_GET:
			if(RunState)
			{
				if (Chk_PATR_YAxis_GetPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Y을 집을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Get Position PA TR Y.");
					m_strLastVnmMsg = _T("Please Move Get Position PA TR Y.");
					bRet = FALSE;
				}
				else if (Chk_UpperShuttle_LoadPos() == FALSE)
				{
					m_strLastKorMsg = _T("UpperShuttle을 로딩 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Loading Position UpperShuttle.");
					m_strLastVnmMsg = _T("Please Move Loading Position UpperShuttle.");
					bRet = FALSE;
				}
			}
			else
			{
				if (Chk_PATR_YAxis_GetPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Y을 집을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Get Position PA TR Y.");
					m_strLastVnmMsg = _T("Please Move Get Position PA TR Y.");
					bRet = FALSE;
				}
				else if (Chk_UpperShuttle_LoadPos() == FALSE)
				{
					m_strLastKorMsg = _T("UpperShuttle을 로딩 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Loading Position UpperShuttle.");
					m_strLastVnmMsg = _T("Please Move Loading Position UpperShuttle.");
					bRet = FALSE;
				}
			}

			break;
		case TEACH_PARAM::PA_TR_Z_PUT:
			if(RunState)
			{
				if (Chk_PATR_YAxis_PutPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Y을 놓을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Put Position PA TR Y.");
					m_strLastVnmMsg = _T("Please Move Put Position PA TR Y.");
					bRet = FALSE;
				}
			}
			else
			{
				if (Chk_PATR_YAxis_PutPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Y을 놓을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Put Position PA TR Y.");
					m_strLastVnmMsg = _T("Please Move Put Position PA TR Y.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::PA_TR_Z_UP:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::PA_CH1_X:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::PA_CH1_Y:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::PA_CH1_T:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::PA_CH2_X:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::PA_CH2_Y:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::PA_CH2_T:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::PA_STAGE_LOAD_POS:
			if(RunState == TRUE)
			{
				if (Chk_PATR_YAxis_PutPos() && Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
				if (LDTRXGetPosChk() && LDTRZUpPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
				if (LDTRZUpPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::PA_STAGE_ALIGN_POS:
			if(RunState == TRUE)
			{
				if (Chk_PATR_YAxis_PutPos() && Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
				if (LDTRXGetPosChk() && LDTRZUpPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				if (Chk_PATR_ZAxis_UpPos() == FALSE)
				{
					m_strLastKorMsg = _T("PA TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position PA TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position PA TR Z.");
					bRet = FALSE;
				}
				if (LDTRZUpPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}

	
			break;
		case TEACH_PARAM::PA_STAGE_UNLOAD_POS:
			if(RunState == TRUE)
			{
				if (LDTRXGetPosChk() && LDTRZUpPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				if (LDTRZUpPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::LD_TR_X_GET:
			if(RunState == TRUE)
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTRZUpPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTRZUpPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::LD_TR_X_PUT:
			if(RunState == TRUE)
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTRZUpPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTRZUpPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::LD_TR_X_CH1_LOWER_MCR:
			if(RunState == TRUE)
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTR_Z_LOWERMCRPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 LOWER MCR 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTR_Z_LOWERMCRPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 LOWER MCR 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
	
			break;
		case TEACH_PARAM::LD_TR_X_CH2_LOWER_MCR:
			if(RunState == TRUE)
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTR_Z_LOWERMCRPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 LOWER MCR 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTR_Z_LOWERMCRPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 LOWER MCR 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::LD_TR_X_CH1_UPPER_MCR:
			if(RunState == TRUE)
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTR_Z_UPPERMCRPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 UPPER MCR 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTR_Z_UPPERMCRPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 UPPER MCR 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}

			break;
		case TEACH_PARAM::LD_TR_X_CH2_UPPER_MCR:
			if(RunState == TRUE)
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTR_Z_UPPERMCRPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 UPPER MCR 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				//2017-05-27,SKCHO, 업 위치가 아니면 이동 못하게 수정
				if ((LDTR_Z_UPPERMCRPosChk() == FALSE))
				//if ((LDTRZGetPosChk() == TRUE) || (LDTRZPutPosChk() == TRUE))
				{
					m_strLastKorMsg = _T("LD TR Z을 UPPER MCR 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::LD_TR_Z_GET:
			if(RunState == TRUE)
			{
				if (LDTRXGetPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR X를 Get 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Get Position LD TR X.");
					m_strLastVnmMsg = _T("Please Move Get Position LD TR X.");
					bRet = FALSE;
				}
			}
			else
			{
				if (LDTRXGetPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR X를 Get 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Get Position LD TR X.");
					m_strLastVnmMsg = _T("Please Move Get Position LD TR X.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::LD_TR_Z_PUT:
			if(RunState == TRUE)
			{
				if (LDTRXPutPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR X를 Put 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Put Position LD TR X.");
					m_strLastVnmMsg = _T("Please Move Put Position LD TR X.");
					bRet = FALSE;
				}
			}
			else
			{
				if (LDTRXPutPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("LD TR X를 Put 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Put Position LD TR X.");
					m_strLastVnmMsg = _T("Please Move Put Position LD TR X.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::LD_TR_Z_UP:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::LD_TR_Z_LOWER_MCR:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::LD_TR_Z_UPPER_MCR:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_INSP_CAM_X:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_INSP_CH1_CAM_ZOOM_Z:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_INSP_CH1_CAM_FOCUS_F:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_INSP_CH1_CAM_TURN_T:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_INSP_CH2_CAM_ZOOM_Z:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_INSP_CH2_CAM_FOCUS_F:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_INSP_CH2_CAM_TURN_T:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_Y_READY:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_X_READY:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_CH1_X1:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_CH1_X2:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_CH1_Y1:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_CH1_Y2:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_CH2_X1:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_CH2_X2:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_CH2_Y1:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::TT_VALIGN_CAM_CH2_Y2:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::WORK_TABLE_T_TO_WORK:
			if(RunState == TRUE)
			{
				if (LDTRXPutPosChk())
				{
					if (LDTRZPutPosChk() == TRUE || LDTRZGetPosChk() == TRUE)
					{
						m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
						m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
						m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
						bRet = FALSE;
					}
				}
				if (B_Zone_Cam_X_Ready_Chk() == FALSE)
				{
					m_strLastKorMsg = _T("VISION ALIGN X를 대기 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Ready Position Vision Align X.");
					m_strLastVnmMsg = _T("Please Move Ready Position Vision Align X.");
					bRet = FALSE;
				}
				if (B_Zone_Cam_Y_Ready_Chk() == FALSE)
				{
					m_strLastKorMsg = _T("VISION ALIGN Y를 대기 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Ready Position Vision Align Y.");
					m_strLastVnmMsg = _T("Please Move Ready Position Vision Align Y.");
					bRet = FALSE;
				}
				if (MoveUnLoadTR_ZAxis_UpChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR Z를 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position ULD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position ULD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				if (LDTRZGetPosChk() == TRUE || LDTRZPutPosChk() == TRUE)
				{
					m_strLastKorMsg = _T("LD TR Z을 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position LD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position LD TR Z.");
					bRet = FALSE;
				}
				if (B_Zone_Cam_X_Ready_Chk() == FALSE)
				{
					m_strLastKorMsg = _T("VISION ALIGN X를 대기 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Ready Position Vision Align X.");
					m_strLastVnmMsg = _T("Please Move Ready Position Vision Align X.");
					bRet = FALSE;
				}
				if (B_Zone_Cam_Y_Ready_Chk() == FALSE)
				{
					m_strLastKorMsg = _T("VISION ALIGN Y를 대기 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Ready Position Vision Align Y.");
					m_strLastVnmMsg = _T("Please Move Ready Position Vision Align Y.");
					bRet = FALSE;
				}
				if (MoveUnLoadTR_ZAxis_UpChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR Z를 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position ULD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position ULD TR Z.");
					bRet = FALSE;
				}
			}

			break;
		case TEACH_PARAM::ULD_TR_X_GET:
			if(RunState == TRUE)
			{
				if (MoveUnLoadTR_ZAxis_UpChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR Z를 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position ULD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position ULD TR Z.");
					bRet = FALSE;
				}
			}
			else
			{
				if (MoveUnLoadTR_ZAxis_UpChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR Z를 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position ULD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position ULD TR Z.");
					bRet = FALSE;
				}
			}
	
			break;
		case TEACH_PARAM::ULD_TR_X_PUT:
			if(RunState == TRUE)
			{
				if (MoveUnLoadTR_ZAxis_UpChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR Z를 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position ULD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position ULD TR Z.");
					bRet = FALSE;
				}
				if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_GETSTAGE) == FALSE)
				{
					m_strLastKorMsg = _T("UNLOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move UD Robot.");
					m_strLastVnmMsg = _T("Please Move UD Robot");
					bRet = FALSE;
				}
			}
			else
			{
				if (MoveUnLoadTR_ZAxis_UpChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR Z를 상승 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Up Position ULD TR Z.");
					m_strLastVnmMsg = _T("Please Move Up Position ULD TR Z.");
					bRet = FALSE;
				}
				if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_GETSTAGE) == FALSE)
				{
					m_strLastKorMsg = _T("UNLOADER ROBOT을 회피 시켜 주세요");
					m_strLastEngMsg = _T("Please Move UD Robot.");
					m_strLastVnmMsg = _T("Please Move UD Robot");
					bRet = FALSE;
				}
			}

			break;
		case TEACH_PARAM::ULD_TR_Z_GET:
			if(RunState == TRUE)
			{
				if (MoveUnloadTR_X_GetPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR X를 집을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Get Position ULD TR X.");
					m_strLastVnmMsg = _T("Please Move Get Position ULD TR X.");
					bRet = FALSE;
				}
			}
			else
			{
				if (MoveUnloadTR_X_GetPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR X를 집을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Get Position ULD TR X.");
					m_strLastVnmMsg = _T("Please Move Get Position ULD TR X.");
					bRet = FALSE;
				}
			}

			break;
		case TEACH_PARAM::ULD_TR_Z_PUT:
			if(RunState == TRUE)
			{
				if (MoveUnloadTR_X_PutPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR X를 놓을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Put Position ULD TR X.");
					m_strLastVnmMsg = _T("Please Move Put Position ULD TR X.");
					bRet = FALSE;
				}
			}
			else
			{
				if (MoveUnloadTR_X_PutPosChk() == FALSE)
				{
					m_strLastKorMsg = _T("ULD TR X를 놓을 위치로 이동 시켜 주세요");
					m_strLastEngMsg = _T("Please Move Put Position ULD TR X.");
					m_strLastVnmMsg = _T("Please Move Put Position ULD TR X.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::ULD_TR_Z_UP:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_UP:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_DOWN:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_UP:
			if(RunState == TRUE)
			{
				//2017.07.22. JSJUNG. GOOD ULD TRAY 리프트 상승시 충돌 예방
				if(theUnitFunc.GetInPutIOCheck(X_GOOD_ULD_TRAY_LIFT_SEN))
				{
					m_strLastKorMsg = _T("GOOD TR과 충돌위험 있습니다. 조그로 이동하여 주십시오.");
					m_strLastEngMsg = _T("Please Move UP JOG.");
					m_strLastVnmMsg = _T("Please Move UP JOG.");
					bRet = FALSE;
				}

			}
			else
			{
				//2017.07.22. JSJUNG. GOOD ULD TRAY 리프트 상승시 충돌 예방
				if(theUnitFunc.GetInPutIOCheck(X_GOOD_ULD_TRAY_LIFT_SEN))
				{
					m_strLastKorMsg = _T("GOOD TR과 충돌위험 있습니다. 조그로 이동하여 주십시오.");
					m_strLastEngMsg = _T("Please Move UP JOG.");
					m_strLastVnmMsg = _T("Please Move UP JOG.");
					bRet = FALSE;
				}
			}
			break;
		case TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_DOWN:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::OK_TRAY_TR_Y_TO_IN:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			if (TrayTrZ_UpChk() == FALSE)
			{
				m_strLastKorMsg = _T("Good Tray TR Z를 상승 위치로 이동 시켜 주세요");
				m_strLastEngMsg = _T("Please Move Up Position Good Tray TR Z.");
				m_strLastVnmMsg = _T("Please Move Up Position Good Tray TR Z.");
				bRet = FALSE;
			}
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_OKTRAYPUT) == FALSE)
			{
				m_strLastKorMsg = _T("UNLOADER ROBOT을 회피 시켜 주세요");
				m_strLastEngMsg = _T("Please Move UD Robot.");
				m_strLastVnmMsg = _T("Please Move UD Robot");
				bRet = FALSE;
			}
			break;
		case TEACH_PARAM::OK_TRAY_TR_Y_TO_OUT:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			if (TrayTrZ_UpChk() == FALSE)
			{
				m_strLastKorMsg = _T("Good Tray TR Z를 상승 위치로 이동 시켜 주세요");
				m_strLastEngMsg = _T("Please Move Up Position Good Tray TR Z.");
				m_strLastVnmMsg = _T("Please Move Up Position Good Tray TR Z.");
				bRet = FALSE;
			}
			break;
		case TEACH_PARAM::OK_TRAY_TR_Z_TO_UP:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::OK_TRAY_TR_Z_TO_IN:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::OK_TRAY_TR_Z_TO_OUT:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_UP:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			if(NGLiftArmFwdBwdChk(NG_TRAY_PORT,CYL_FWD) == FALSE)
			{
				m_strLastKorMsg = _T("NG TRAY IN LIFT Z ARM 전진 시켜 주세요");
				m_strLastEngMsg = _T("NG TRAY IN LIFT Z ARM Forward Please.");
				m_strLastVnmMsg = _T("NG TRAY IN LIFT Z ARM Forward Please.");
				bRet = FALSE;
			}
		case TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_DOWN:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_MID:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			if(NGLiftArmFwdBwdChk(NG_TRAY_PORT,CYL_FWD) == FALSE)
			{
				m_strLastKorMsg = _T("NG TRAY IN LIFT Z ARM 전진 시켜 주세요");
				m_strLastEngMsg = _T("NG TRAY IN LIFT Z ARM Forward Please.");
				m_strLastVnmMsg = _T("NG TRAY IN LIFT Z ARM Forward Please.");
				bRet = FALSE;
			}
			break;
		case TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_UP:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_DOWN:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::NG_TRAY_TR_X_TO_IN:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			if (NGTrayTrZ_UpChk() == FALSE)
			{
				m_strLastKorMsg = _T("NG Tray TR Z를 상승 위치로 이동 시켜 주세요");
				m_strLastEngMsg = _T("Please Move Up Position NG Tray TR Z.");
				m_strLastVnmMsg = _T("Please Move Up Position NG Tray TR Z.");
				bRet = FALSE;
			}
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_NGTRAYPUT) == FALSE)
			{
				m_strLastKorMsg = _T("UNLOADER ROBOT을 회피 시켜 주세요");
				m_strLastEngMsg = _T("Please Move UD Robot.");
				m_strLastVnmMsg = _T("Please Move UD Robot");
				bRet = FALSE;
			}
			break;
		case TEACH_PARAM::NG_TRAY_TR_X_TO_OUT:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			if (NGTrayTrZ_UpChk() == FALSE)
			{
				m_strLastKorMsg = _T("NG Tray TR Z를 상승 위치로 이동 시켜 주세요");
				m_strLastEngMsg = _T("Please Move Up Position NG Tray TR Z.");
				m_strLastVnmMsg = _T("Please Move Up Position NG Tray TR Z.");
				bRet = FALSE;
			}
			break;
		case TEACH_PARAM::NG_TRAY_TR_Z_TO_UP:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::NG_TRAY_TR_Z_TO_IN:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
		case TEACH_PARAM::NG_TRAY_TR_Z_TO_OUT:
			if(RunState == TRUE)
			{
			}
			else
			{
			}
			break;
   }
	return bRet;
}

BOOL CUnitCtrlBank::AGVConnectionChk(TRAY_PORT Port)
{
	BOOL bRtn = FALSE;
	if (Port == GOOD_IN_TRAY_PORT)
	{
		bRtn = GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_PIO_CONNECTION); //connection
		theProcBank.m_bInPutIO[X_GOOD_LD_CONV_TRAY_PIO_CONNECTION] = bRtn;
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		bRtn = GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_PIO_CONNECTION);
		theProcBank.m_bInPutIO[X_GOOD_ULD_CONV_TRAY_PIO_CONNECTION] = bRtn;
	}
	else
	{
	    bRtn = GetInPutIOCheck(X_NG_CONV_TRAY_PIO_CONNECTION);
		theProcBank.m_bInPutIO[X_NG_CONV_TRAY_PIO_CONNECTION] = bRtn;
	}

	return bRtn;
}


BOOL CUnitCtrlBank::AGVRequestChk(TRAY_PORT Port)
{
	BOOL bRtn = FALSE;
	if (Port == GOOD_IN_TRAY_PORT)
	{
		 bRtn =	GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_PIO_REQ);  //request
		 theProcBank.m_bInPutIO[X_GOOD_LD_CONV_TRAY_PIO_REQ] = bRtn;
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		 bRtn =	GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_PIO_REQ);
		 theProcBank.m_bInPutIO[X_GOOD_ULD_CONV_TRAY_PIO_REQ] = bRtn;
	}
	else
	{
		 bRtn =	GetInPutIOCheck(X_NG_CONV_TRAY_PIO_REQ);
		 theProcBank.m_bInPutIO[X_NG_CONV_TRAY_PIO_REQ] = bRtn;
	}
	
	return bRtn;
}


BOOL CUnitCtrlBank::AGVBusyChk(TRAY_PORT Port)
{
	BOOL bRtn = FALSE;

	if (Port == GOOD_IN_TRAY_PORT)
	{
		bRtn =	GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_PIO_BUSY);
		theProcBank.m_bInPutIO[X_GOOD_LD_CONV_TRAY_PIO_BUSY] = bRtn;
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		bRtn =	GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_PIO_BUSY);
		theProcBank.m_bInPutIO[X_GOOD_ULD_CONV_TRAY_PIO_BUSY] = bRtn;
	}
	else
	{
		bRtn =	GetInPutIOCheck(X_NG_CONV_TRAY_PIO_BUSY);
		theProcBank.m_bInPutIO[X_NG_CONV_TRAY_PIO_BUSY] = bRtn;
	}
	return bRtn;
}


BOOL CUnitCtrlBank::AGVCompChk(TRAY_PORT Port)
{
	BOOL bRtn = FALSE;
	if (Port == GOOD_IN_TRAY_PORT)
	{
		bRtn =	GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_PIO_COMP);
		theProcBank.m_bInPutIO[X_GOOD_LD_CONV_TRAY_PIO_COMP] = bRtn;
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		bRtn =	GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_PIO_COMP);
		theProcBank.m_bInPutIO[X_GOOD_ULD_CONV_TRAY_PIO_COMP] = bRtn;
	}
	else
	{
		bRtn =	GetInPutIOCheck(X_NG_CONV_TRAY_PIO_COMP);
		theProcBank.m_bInPutIO[X_NG_CONV_TRAY_PIO_COMP] = bRtn;
	}
	return bRtn;
}

void CUnitCtrlBank::SendAGVReq(TRAY_PORT Port, ONOFF bOn)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_LD_CONV_TRAY_PIO_REQ,bOn);
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_PIO_REQ,bOn);
	}
	else
	{
		SetOutPutIO(Y_NG_CONV_TRAY_PIO_REQ,bOn);
	}
}

void CUnitCtrlBank::SendAGVConnection(TRAY_PORT Port, ONOFF bOn)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_LD_CONV_TRAY_PIO_CONNECTION,bOn);
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_PIO_CONNECTION,bOn);
	}
	else
	{
		SetOutPutIO(Y_NG_CONV_TRAY_PIO_CONNECTION,bOn);
	}
}

void CUnitCtrlBank::SendAGVStart(TRAY_PORT Port, ONOFF bOn)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_LD_CONV_TRAY_PIO_START,bOn);	
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_PIO_START,bOn);	
	}
	else
	{
		SetOutPutIO(Y_NG_CONV_TRAY_PIO_START,bOn);	
	}
}

void CUnitCtrlBank::SendAGVComp(TRAY_PORT Port, ONOFF bOn)
{
	//2017-08-30,SKCHO , PIO COMP 사용하지 않아 삭제
	/*if (Port == GOOD_IN_TRAY_PORT)
		SetOutPutIO(Y_GOOD_LD_CONV_TRAY_PIO_COMP,bOn);
	else if (Port == GOOD_OUT_TRAY_PORT)
		SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_PIO_COMP,bOn);
	else
	*/
	SetOutPutIO(Y_NG_CONV_TRAY_PIO_COMP,bOn);
	
}

void CUnitCtrlBank::SendAGVPortType(TRAY_PORT Port)
{
	
	if (Port == GOOD_IN_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_LD_CONV_TRAY_PIO_LOADING,ON);	

		SetOutPutIO(Y_GOOD_LD_CONV_TRAY_PIO_UNLOADING,OFF);		

	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_PIO_LOADING,OFF);	

		SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_PIO_UNLOADING,ON);		
	}
	
}
void CUnitCtrlBank::SendAGVAbort(TRAY_PORT Port,ONOFF bOn)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_LD_CONV_TRAY_PIO_ABORT,bOn);		
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_PIO_ABORT,bOn);		
	}
}
void CUnitCtrlBank::SendPGMessage(CString strCommand, ZONE_ID zone, CString strExtraData)
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		theSocketInterFace.SendMassageToPatternGenerator(strCommand, AMT_GetAutoJigID(zone),(JIG_CH)i, strExtraData);
	}
}

void CUnitCtrlBank::SendPGMessage(CString strCommand, ZONE_ID zone, JIG_CH nCh, CString strExtraData)
{
	theSocketInterFace.SendMassageToPatternGenerator(strCommand, AMT_GetAutoJigID(zone),nCh, strExtraData);
}

BOOL CUnitCtrlBank::DoorInterLockChkStart()
{
	if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_1))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Loader door 1가 열렸습니다. "),	//한
			_T("Loader door 1 opened."),			//영
			_T("Loader cửa được mở 1.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_2))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Loader door 2 가 열렸습니다. "),	//한
			_T("Loader door 2 opened."),			//영
			_T("Loader cửa được mở 2.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_3))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Loader door 3 가 열렸습니다. "),	//한
			_T("Loader door 3 opened."),			//영
			_T("Loader cửa được mở 3.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	
	if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_4))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Loader door 4 가 열렸습니다. "),	//한
			_T("Loader door 4 opened."),			//영
			_T("Loader cửa được mở 4.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_5))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Loader door 5 가 열렸습니다. "),	//한
			_T("Loader door 5 opened."),			//영
			_T("Loader cửa được mở 5.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_6))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Loader door 6 가 열렸습니다. "),	//한
			_T("Loader door 6 opened."),			//영
			_T("Loader cửa được mở 6.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_1))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UnLoader door 1 가 열렸습니다. "),	//한
			_T("UnLoader door 1 opened."),			//영
			_T("UNLoader cửa được mở 1.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_2))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UnLoader door 2 가 열렸습니다. "),	//한
			_T("UnLoader door 2 opened."),			//영
			_T("UNLoader cửa được mở 2.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_3))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UnLoader door 3 가 열렸습니다. "),	//한
			_T("UnLoader door 3 opened."),			//영
			_T("UNLoader cửa được mở 3.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_4))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UnLoader door 4 가 열렸습니다. "),	//한
			_T("UnLoader door 4 opened."),			//영
			_T("UNLoader cửa được mở 4.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	if(theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_5))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UnLoader door 5 가 열렸습니다. "),	//한
			_T("UnLoader door 5 opened."),			//영
			_T("UNLoader cửa được mở 5.") );			//베

		dlgMsgBox.DoModal();

		return FALSE;
	}
	//if(theUnitFunc.GetInPutIOCheck(X_ULD_SLIDE_DOOR_SEN))
	//{
	//	CGxMsgBox	dlgMsgBox;

	//	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	//	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	//	dlgMsgBox.SetMessage(FALSE, 
	//		_T("UnLoader Slide door가 열렸습니다. "),	//한
	//		_T("UnLoader Slide door opened."),			//영
	//		_T("UNLoader Slide cửa được mở.") );			//베

	//	dlgMsgBox.DoModal();

	//	return FALSE;
	//}

	return TRUE;
}

BOOL CUnitCtrlBank::LightCurtainChkStart() 
{
	if(theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_RUN
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
		if(theUnitFunc.GetInPutIOCheck(X_GOOD_OUT_TRAY_LC_CHK))
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("Good Out Tray light curtain을 감지 했습니다."),	//한
				_T("Good Out Tray light curtain detected."),		//영
				_T("Good Out Tray ánh sáng bức màn phát hiện.") );		//베

			dlgMsgBox.DoModal();
			return FALSE;
		}
		if(theUnitFunc.GetInPutIOCheck(X_GOOD_IN_TRAY_LC_CHK))
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("Good IN Tray light curtain을 감지 했습니다."),	//한
				_T("Good IN Tray light curtain detected."),		//영
				_T("Good IN Tray ánh sáng bức màn phát hiện.") );		//베

			dlgMsgBox.DoModal();
			return FALSE;
		}
		if(theUnitFunc.GetInPutIOCheck(X_NG_TRAY_LC_CHK))
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("NG Tray light curtain을 감지 했습니다."),	//한
				_T("NG Tray light curtain detected."),		//영
				_T("NG Tray ánh sáng bức màn phát hiện.") );		//베

			dlgMsgBox.DoModal();
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CUnitCtrlBank::DoorKeyModeChkStart()
{
	if(theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_RUN
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
		if(theUnitFunc.GetInPutIOCheck(X_TEACH_KEY_SW) == FALSE)
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("auto Teach key가 Teach mode 입니다."),	//한
				_T("auto teach key state is Teach mode."),		//영
				_T("Các trạng thái auto Teach key hoạt động là Teach mode") );		//베

			//int nRet = dlgMsgBox.DoModal();
			//if(nRet != IDOK)
			dlgMsgBox.DoModal();
			return FALSE;
		}
	}
	return TRUE;
}

void CUnitCtrlBank::FPCBUseSol(BOOL bOn)
{
#if(AMT_TYPE == 1)
	SetOutPutIO(Y_LD_ROBOT_FPCB_USE_SOL, bOn);
	SetOutPutIO(Y_PA_TR_CH1_FPCB_USE_SOL, bOn);
	SetOutPutIO(Y_PA_TR_CH2_FPCB_USE_SOL, bOn);
	SetOutPutIO(Y_LD_TR_CH1_FPCB_BLOW_SOL, bOn);
	SetOutPutIO(Y_LD_TR_CH2_FPCB_BLOW_SOL, bOn);
	SetOutPutIO(Y_ULD_TR_CH1_FPCB_USE_SOL, bOn);
	SetOutPutIO(Y_ULD_TR_CH2_FPCB_USE_SOL, bOn);
	SetOutPutIO(Y_ULD_ROBOT_FPCB_USE_SOL, bOn);
#endif
}


void CUnitCtrlBank::SetProductData(CELL_POS pos)
{
	CCellInfo* pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(pos));

	if (pCell == NULL)
	{
		return;
	}

	BOOL bAlignResult = FALSE;
	BOOL bContactResult = FALSE;
	BOOL bMCRResult = FALSE;
	BOOL bCellStatus = FALSE;
	if ( pCell->Pre_AlignResultXY == RESULT_GOOD )		bAlignResult = TRUE;
	else												bAlignResult = FALSE;

	//2018-08-02,SKCHO, DIC_CHECK가 추가되어 조건을 추가함, 모두 만족해야 CONTACT GOOD
	//if ( pCell->m_Zone_A.AZoneClass == GOOD_CELL && pCell->m_Zone_A.ContactCurrentClass == GOOD_CELL)
	if (pCell->m_Zone_A.CellLoadingClass  == GOOD_CELL 
		|| pCell->m_Zone_A.ContactCurrentClass  == GOOD_CELL
		|| pCell->m_Zone_A.RegPinClass  == GOOD_CELL
		|| pCell->m_Zone_A.DicCheckClass  == GOOD_CELL)
	{
		bContactResult = TRUE;
	}
	else
	{
		bContactResult = FALSE;
	}

	//if ( pCell->m_strCellID == TEXT_UV_FAIL )		bMCRResult = FALSE;
	if ( pCell->m_strCellID == FAIL )		bMCRResult = FALSE; //2017-04-05,skcho, 코그닉스에서 FAIL 로 데이터를 줌
	else											bMCRResult = TRUE;

	if ( pCell->m_LastClass == GOOD_CELL )			bCellStatus = TRUE;
	else											bCellStatus = FALSE;

	theProductBank.SetProductData(bCellStatus, bMCRResult, bAlignResult, bContactResult);

	theProductBank.UpdateMainOutputCount(pCell);

	theProductBank.UpdateJigOutputCount(pCell);

}

void CUnitCtrlBank::TactUpdate(int Tact)
{
	double dTact = 0;
	int nEqTack = 0;	// 메인 화면에 보여줄 TACT
	CString strDataPath;
	double dMin = 0;
	double dMax = 0;
	double dAvr = 0;
	double dUnloadtact[20];
	int dCount = 0;
	CString strBuf;
	CCellInfo* pCellInfo;
	CCellTag tagCell;
	CIni ini;

	strDataPath.Format(_T("%s\\ViewTactTime.ini"), theConfigBank.m_System.m_strConfigPath);

	ini.SetIniFilename(strDataPath);

	if (Tact == LOADER_TACT)
	{
		dTact = theProcBank.m_CellLoadTact.Stop(TRUE);
		if(theProcBank.m_bFirstLoadTactStart)
		{
			theProcBank.m_bFirstLoadTactStart = FALSE;
			dTact = 0.;
	
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);
			if(tagCell.IsExist()==FALSE)
				return;

			pCellInfo->LoadTact = dTact;
			nEqTack = (int)(dTact * 1000);
		}
		else
		{
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);
			if(tagCell.IsExist()==FALSE)
				return;
			pCellInfo->LoadTact = dTact;

			nEqTack = (int)(dTact * 1000);

			if (nEqTack > 10000.)
				return;

			dMin = ini.GetDouble(_T("LOAD TACT"),_T("MIN"));
			dMax = ini.GetDouble(_T("LOAD TACT"),_T("MAX"));
			dAvr = ini.GetDouble(_T("LOAD TACT"),_T("AVR"));

			ini.WriteDouble(_T("LOAD TACT"), _T("RECENTLY"), dTact);

			if ((dTact < dMin) || (dMin == 0.0))
				ini.WriteDouble(_T("LOAD TACT"), _T("MIN"), dTact);

			if ((dTact >= dMax) || (dMax == 0.0))
				ini.WriteDouble(_T("LOAD TACT"), _T("MAX"), dTact);

			if (dAvr == 0.0)
				ini.WriteDouble(_T("LOAD TACT"), _T("AVR"), dTact);
			else
				ini.WriteDouble(_T("LOAD TACT"), _T("AVR"), (dAvr + dTact)/2);
		}

		::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_TACT_CHANGE, nEqTack, Tact);
	}
	else if (Tact == UNLOADER_TACT)
	{
		dTact = theProcBank.m_CellUnloadTact.Stop(TRUE);
		if(theProcBank.m_bFirstUnloadTactStart)
		{
			theProcBank.m_bFirstUnloadTactStart = FALSE;
			dTact = 0.;

			pCellInfo = theCellBank.GetCellInfo(CELL_POS_UD_ROBOT);
			tagCell = theCellBank.GetCellTag(CELL_POS_UD_ROBOT);
			if(tagCell.IsExist()==FALSE)
				return;
			pCellInfo->UnLoadTact = dTact;

			nEqTack = (int)(dTact * 1000); 
		}
		else
		{
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_UD_ROBOT);
			tagCell = theCellBank.GetCellTag(CELL_POS_UD_ROBOT);
			if(tagCell.IsExist()==FALSE)
				return;
			pCellInfo->UnLoadTact = dTact;

			nEqTack = (int)(dTact * 1000);

			if (nEqTack > 10000.)
				return;

			dMin = ini.GetDouble(_T("UNLOAD TACT"),_T("MIN"));
			dMax = ini.GetDouble(_T("UNLOAD TACT"),_T("MAX"));
			dAvr = ini.GetDouble(_T("UNLOAD TACT"),_T("AVR"));

			ini.WriteDouble(_T("UNLOAD TACT"), _T("RECENTLY"), dTact);

			if ((dTact < dMin) || (dMin == 0.0))
				ini.WriteDouble(_T("UNLOAD TACT"), _T("MIN"), dTact);

			if ((dTact >= dMax) || (dMax == 0.0))
				ini.WriteDouble(_T("UNLOAD TACT"), _T("MAX"), dTact);

			if (dAvr == 0.0)
				ini.WriteDouble(_T("UNLOAD TACT"), _T("AVR"), dTact);
			else
				ini.WriteDouble(_T("UNLOAD TACT"), _T("AVR"), (dAvr + dTact)/2);

			// 최근 20Cell Unload Tact.
			dCount = ini.GetLong(_T("UNLOAD TACT"),_T("UNLOADCOUNT"));
			dCount++;
			ini.WriteLong(_T("UNLOAD TACT"), _T("UNLOADCOUNT"), dCount);
			strBuf.Format(_T("RECENTLY%d"),dCount);
			ini.WriteDouble(_T("UNLOAD TACT"), strBuf, dTact);
			strBuf.Format(_T("RECENTLYID%d"),dCount);
			ini.WriteString(_T("UNLOAD TACT"), strBuf, pCellInfo->m_strCellID);
			if (dCount == 20)
			{
				dCount = 0;
				ini.WriteLong(_T("UNLOAD TACT"), _T("UNLOADCOUNT"), dCount);
			}

			for (int i = 0; i < 20; i++)
				dUnloadtact[i] = 0.0;

			dAvr = 0.0;

			for (int i = 0; i < 20; i++)
			{
				strBuf.Format(_T("RECENTLY%d"),i);
				dUnloadtact[i] = ini.GetDouble(_T("UNLOAD TACT"), strBuf);

				dAvr = dAvr + dUnloadtact[i];
			}

			dAvr = dAvr/20;
			ini.WriteDouble(_T("UNLOAD TACT"), _T("20AVR"), dAvr);
		}
	}
	else if (Tact == AZONE_TACT)
	{
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH1);
		tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH1);
		if(tagCell.IsExist()==FALSE)
			return;
		if (pCellInfo != NULL)
		{
			pCellInfo->AZoneTact = CountTactTime(pCellInfo->cellTime.systmA_StartTime,pCellInfo->cellTime.systmA_EndTime);

			nEqTack = (int)(pCellInfo->AZoneTact * 1000); 

			if (nEqTack > 20000.)
				return;

			dMin = ini.GetDouble(_T("AZONE TACT"),_T("MIN"));
			dMax = ini.GetDouble(_T("AZONE TACT"),_T("MAX"));
			dAvr = ini.GetDouble(_T("AZONE TACT"),_T("AVR"));

			ini.WriteDouble(_T("AZONE TACT"), _T("RECENTLY"), pCellInfo->AZoneTact);

			if ((pCellInfo->AZoneTact < dMin) || (dMin == 0.0))
				ini.WriteDouble(_T("AZONE TACT"), _T("MIN"), pCellInfo->AZoneTact);

			if ((pCellInfo->AZoneTact >= dMax) || (dMax == 0.0))
				ini.WriteDouble(_T("AZONE TACT"), _T("MAX"), pCellInfo->AZoneTact);

			if (dAvr == 0.0)
				ini.WriteDouble(_T("AZONE TACT"), _T("AVR"), pCellInfo->AZoneTact);
			else
				ini.WriteDouble(_T("AZONE TACT"), _T("AVR"), (dAvr + pCellInfo->AZoneTact)/2);
		}
	}
	else if (Tact == BZONE_TACT)
	{
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH1);
		tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH1);
		if(tagCell.IsExist()==FALSE)
			return;
		if (pCellInfo != NULL)
		{
			pCellInfo->BZoneTact = CountTactTime(pCellInfo->cellTime.systmB_StartTime,pCellInfo->cellTime.systmB_EndTime);
			nEqTack = (int)(pCellInfo->BZoneTact * 1000); 

			if (nEqTack > 20000.)
				return;

			dMin = ini.GetDouble(_T("BZONE TACT"),_T("MIN"));
			dMax = ini.GetDouble(_T("BZONE TACT"),_T("MAX"));
			dAvr = ini.GetDouble(_T("BZONE TACT"),_T("AVR"));

			ini.WriteDouble(_T("BZONE TACT"), _T("RECENTLY"), pCellInfo->BZoneTact);

			if ((pCellInfo->BZoneTact < dMin) || (dMin == 0.0))
				ini.WriteDouble(_T("BZONE TACT"), _T("MIN"), pCellInfo->BZoneTact);

			if ((pCellInfo->BZoneTact >= dMax) || (dMax == 0.0))
				ini.WriteDouble(_T("BZONE TACT"), _T("MAX"), pCellInfo->BZoneTact);

			if (dAvr == 0.0)
				ini.WriteDouble(_T("BZONE TACT"), _T("AVR"), pCellInfo->BZoneTact);
			else
				ini.WriteDouble(_T("BZONE TACT"), _T("AVR"), (dAvr + pCellInfo->BZoneTact)/2);
		}
	}
	else if (Tact == CZONE_TACT)
	{
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH1);
		if(tagCell.IsExist()==FALSE)
			return;
		if (pCellInfo != NULL)
		{
			pCellInfo->CZoneTact = CountTactTime(pCellInfo->cellTime.systmC_StartTime,pCellInfo->cellTime.systmC_EndTime);
			nEqTack = (int)(pCellInfo->CZoneTact * 1000); 

			if (nEqTack > 20000.)
				return;

			dMin = ini.GetDouble(_T("CZONE TACT"),_T("MIN"));
			dMax = ini.GetDouble(_T("CZONE TACT"),_T("MAX"));
			dAvr = ini.GetDouble(_T("CZONE TACT"),_T("AVR"));

			ini.WriteDouble(_T("CZONE TACT"), _T("RECENTLY"), pCellInfo->CZoneTact);

			if ((pCellInfo->CZoneTact < dMin) || (dMin == 0.0))
				ini.WriteDouble(_T("CZONE TACT"), _T("MIN"), pCellInfo->CZoneTact);

			if ((pCellInfo->CZoneTact >= dMax) || (dMax == 0.0))
				ini.WriteDouble(_T("CZONE TACT"), _T("MAX"), pCellInfo->CZoneTact);

			if (dAvr == 0.0)
				ini.WriteDouble(_T("CZONE TACT"), _T("AVR"), pCellInfo->CZoneTact);
			else
				ini.WriteDouble(_T("CZONE TACT"), _T("AVR"), (dAvr + pCellInfo->CZoneTact)/2);

		}
	}
	else if (Tact == DZONE_TACT)
	{
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_DZONE_CH1);
		tagCell = theCellBank.GetCellTag(CELL_POS_DZONE_CH1);
		if(tagCell.IsExist()==FALSE)
			return;
		if (pCellInfo != NULL)
		{
			pCellInfo->DZoneTact = CountTactTime(pCellInfo->cellTime.systmD_StartTime,pCellInfo->cellTime.systmD_EndTime);
			nEqTack = (int)(pCellInfo->DZoneTact * 1000); 

			if (nEqTack > 20000.)
				return;

			dMin = ini.GetDouble(_T("DZONE TACT"),_T("MIN"));
			dMax = ini.GetDouble(_T("DZONE TACT"),_T("MAX"));
			dAvr = ini.GetDouble(_T("DZONE TACT"),_T("AVR"));

			ini.WriteDouble(_T("DZONE TACT"), _T("RECENTLY"), pCellInfo->DZoneTact);

			if ((pCellInfo->DZoneTact < dMin) || (dMin == 0.0))
				ini.WriteDouble(_T("DZONE TACT"), _T("MIN"), pCellInfo->DZoneTact);

			if ((pCellInfo->DZoneTact >= dMax) || (dMax == 0.0))
				ini.WriteDouble(_T("DZONE TACT"), _T("MAX"), pCellInfo->DZoneTact);

			if (dAvr == 0.0)
				ini.WriteDouble(_T("DZONE TACT"), _T("AVR"), pCellInfo->DZoneTact);
			else
				ini.WriteDouble(_T("DZONE TACT"), _T("AVR"), (dAvr + pCellInfo->DZoneTact)/2);
		}
	}
	if(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd != NULL)
	{
		::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_TACT_CHANGE, nEqTack, Tact);
	}
}


float CUnitCtrlBank::CountTactTime(SYSTEMTIME StartTime, SYSTEMTIME EndTime)
{
	FILETIME fTm1,fTm2;
	ULONGLONG *ullVal1,*ullVal2;
	ULONGLONG ullDif;
	float fTactTime;

	SystemTimeToFileTime(&StartTime,&fTm1);
	SystemTimeToFileTime(&EndTime,&fTm2);
	ullVal1 = (ULONGLONG *)&fTm1;
	ullVal2 = (ULONGLONG *)&fTm2;
	ullDif = *ullVal2 - * ullVal1;

	fTactTime = float(ullDif/10000)/1000;

	// 1시간 limit
	if(fTactTime > 3600 || fTactTime < 0)	fTactTime = 0;

	return fTactTime;
}

void CUnitCtrlBank::LoadADBoardOffset()
{
	CIni ini;

	CString strDataPath;

	strDataPath.Format(_T("%s\\ADBoardOffSet.ini"), theConfigBank.m_System.m_strConfigPath);

	ini.SetIniFilename(strDataPath);

	theFDCBank.m_dAdOffset[A_ZONE_CH1] = ini.GetDouble(_T("INSP"),_T("A_ZONE_CH1"));
	theFDCBank.m_dAdOffset[A_ZONE_CH2] = ini.GetDouble(_T("INSP"),_T("A_ZONE_CH2"));
	theFDCBank.m_dAdOffset[B_ZONE_CH1] = ini.GetDouble(_T("INSP"),_T("B_ZONE_CH1"));
	theFDCBank.m_dAdOffset[B_ZONE_CH2] = ini.GetDouble(_T("INSP"),_T("B_ZONE_CH2"));
	theFDCBank.m_dAdOffset[C_ZONE_CH1] = ini.GetDouble(_T("INSP"),_T("C_ZONE_CH1"));
	theFDCBank.m_dAdOffset[C_ZONE_CH2] = ini.GetDouble(_T("INSP"),_T("C_ZONE_CH2"));
	theFDCBank.m_dAdOffset[D_ZONE_CH1] = ini.GetDouble(_T("INSP"),_T("D_ZONE_CH1"));
	theFDCBank.m_dAdOffset[D_ZONE_CH2] = ini.GetDouble(_T("INSP"),_T("D_ZONE_CH2"));

	theFDCBank.m_dAdOffset[MAIN_AIR_1] = ini.GetDouble(_T("UD"),_T("MAIN_AIR_1"));
	theFDCBank.m_dAdOffset[MAIN_AIR_2] = ini.GetDouble(_T("UD"),_T("MAIN_AIR_2"));
	theFDCBank.m_dAdOffset[MAIN_VAC_1] = ini.GetDouble(_T("UD"),_T("MAIN_VAC_1"));
	theFDCBank.m_dAdOffset[MAIN_VAC_2] = ini.GetDouble(_T("UD"),_T("MAIN_VAC_2"));
	theFDCBank.m_dAdOffset[ULD_TR_CH1] = ini.GetDouble(_T("UD"),_T("ULD_TR_CH1"));
	theFDCBank.m_dAdOffset[ULD_TR_CH2] = ini.GetDouble(_T("UD"),_T("ULD_TR_CH2"));
	theFDCBank.m_dAdOffset[ULD_STAGE_CH1] = ini.GetDouble(_T("UD"),_T("ULD_STAGE_CH1"));
	theFDCBank.m_dAdOffset[ULD_STAGE_CH2] = ini.GetDouble(_T("UD"),_T("ULD_STAGE_CH2"));
}

//2017-03-09,skcho  CELL 정보에서 PPID를 읽어옴
CString CUnitCtrlBank::GetCellPPID(CELL_POS Pos)
{
	CString strRe =_T("");
	CCellInfo* pCellInfo = NULL;
	pCellInfo = theCellBank.GetCellInfo(Pos);
	if(pCellInfo)
	{
		strRe = pCellInfo->m_strPPID;
	}

	return strRe;
}

// Motor All Stop 함수 추가...17/04/22
void CUnitCtrlBank::MotorAllStop()
{
	for(int i = 0 ; i < AXIS_STP_INS_CH2_F; i++)
	{
		theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, AXIS_ID(i));
	}
}


void CUnitCtrlBank::TrayOutBuzzer(BOOL bLotEnd,ONOFF bOn)
{	
	if(bLotEnd)
		SetOutPutIO(Y_BUZZER_K4, bOn);
	else
		SetOutPutIO(Y_BUZZER_K3, bOn);
}

void CUnitCtrlBank::TrayInBuzzer(int nType,ONOFF bOn)
{

	if(nType == 1)  //ALARM
	{
		SetOutPutIO(Y_BUZZER_K1, bOn);
	}
	else if(nType == 2)  //NG/GOOD IN
	{
		SetOutPutIO(Y_BUZZER_K2, bOn);
	}
	else if(nType == 3)
	{
		SetOutPutIO(Y_BUZZER_K3, bOn);
	}
	else if(nType == 4)
	{
		SetOutPutIO(Y_BUZZER_K4, bOn);
	}

}

//2017-08-22 KAKAKA
void CUnitCtrlBank::TrayInDelayBuzzer(int nType,ONOFF bOn)
{
	/*if (theProcBank.m_bAGVDelay == TRUE)
	{
		if(nType == 1)  //ALARM
		{
			SetOutPutIO(Y_BUZZER_K1, bOn);
		}
		else if(nType == 2)  //NG/GOOD IN
		{
			SetOutPutIO(Y_BUZZER_K2, bOn);
		}
		else if(nType == 3)
		{
			SetOutPutIO(Y_BUZZER_K3, bOn);
			theProcBank.m_bAGVDelay = FALSE;
		}
		else if(nType == 4)
		{
			SetOutPutIO(Y_BUZZER_K4, bOn);
			theProcBank.m_bAGVDelay = FALSE;
		}
	}
	*/
}


BOOL CUnitCtrlBank::NetWorkCheck()
{	
	CGxMsgBox	dlgMsgBox;
	if(theSocketInterFace.m_pSocket_VisionInspect[VI_A1].IsConnected() == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Vision PC 1이 연결 되어 있지 않습니다."), 
			_T("Vision PC 1 Not Connceted"),		
			_T("Vision PC 1 Not Connceted") );

		dlgMsgBox.DoModal();
		return FALSE;
	}
	else if(theSocketInterFace.m_pSocket_VisionInspect[VI_A2].IsConnected() == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Vision PC 2이 연결 되어 있지 않습니다."), 
			_T("Vision PC 2 Not Connceted"),		
			_T("Vision PC 2 Not Connceted") );

		dlgMsgBox.DoModal();
		return FALSE;
	}
	else if(theSocketInterFace.m_pSocket_MITAlign.IsConnected() == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("PreAlign이 연결 되어 있지 않습니다."), 
			_T("PreAlign Not Connceted"),		
			_T("PreAlign Not Connceted") );

		dlgMsgBox.DoModal();
		return FALSE;
	}
	else if(theSocketInterFace.m_pSocket_DCR.IsConnected() == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("DCR이 연결 되어 있지 않습니다."), 
			_T("DCR Not Connceted"),		
			_T("DCR Not Connceted") );

		dlgMsgBox.DoModal();
		return FALSE;
	}
	else if(theSocketInterFace.m_pSocket_Lower_UVMCR.IsConnected() == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("하단 MCR 이 연결 되어 있지 않습니다."), 
			_T("Lower MCR Not Connceted"),		
			_T("Lower MCR Not Connceted") );

		dlgMsgBox.DoModal();
		return FALSE;
	}
	else if(theSocketInterFace.m_pSocket_Upper_UVMCR.IsConnected() == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("상단 MCR 이 연결 되어 있지 않습니다."), 
			_T("Upper MCR Not Connceted"),		
			_T("Upper MCR Not Connceted") );

		dlgMsgBox.DoModal();
		return FALSE;
	}

	//2017-12-29,SKCHO, 얼라인 프로그램 PC가 꺼진 상태에서 런하면 디버그 에러 발생하여 추가
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(FALSE, 
			_T("Edge Aling 이 연결 되어 있지 않습니다."), 
			_T("Edge Aling  Not Connceted"),		
			_T("Edge Aling  Not Connceted") );

	//else if(theSocketInterFace.m_pSocket_MITEdgeAlign.IsConnected() == FALSE)	
	if(theSocketInterFace.m_pSocket_MITEdgeAlign.GetClientAt(SOCKET_MITEDGEALIGN) != NULL)
	{
		if(theSocketInterFace.m_pSocket_MITEdgeAlign.GetClientAt(SOCKET_MITEDGEALIGN)->IsConnected() == FALSE)
		{		
			dlgMsgBox.DoModal();
			return FALSE;
		}
	}
	else
	{
		dlgMsgBox.DoModal();
		return FALSE;
	}

	//2017-12-29,SKCHO, PG 프로그램 PC가 꺼진 상태에서 런하면 디버그 에러 발생하여 추가
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(FALSE, 
			_T("PG 가 연결 되어 있지 않습니다."), 
			_T("PG Not Connceted"),		
			_T("PG Not Connceted") );

	//else if(theSocketInterFace.m_pSocket_PatternGenerator.IsConnected() == FALSE)
	if(theSocketInterFace.m_pSocket_PatternGenerator.GetClientAt(SOCKET_PGPC) != NULL)
	{
		if(theSocketInterFace.m_pSocket_PatternGenerator.GetClientAt(SOCKET_PGPC)->IsConnected() == FALSE)
		{
			dlgMsgBox.DoModal();
			return FALSE;
		}
	}
	else
	{		
		dlgMsgBox.DoModal();
		return FALSE;
	}
	
	if(theSocketInterFace.m_pSocket_DataPC.IsConnected() == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("DATA 가 연결 되어 있지 않습니다."), 
			_T("DATA Not Connceted"),		
			_T("DATA Not Connceted") );

		dlgMsgBox.DoModal();
		return FALSE;
	}
	return TRUE;
}
//2017-06-03,SKCHO - SetUp 모터화면에서 원점 이동할 경우 뮤팅 기능 안되게 수정, 모터 원점 체크 함수
BOOL  CUnitCtrlBank::OriginStatusCheck()
{
	BOOL bReturn = TRUE;
	AXIS_ID axis;
	AxisStatus status;

	for(int i = 1; i < AXIS_ID_MAX; i++)
	{
		axis = (AXIS_ID)(i);
		if(axis < AXIS_ID_MAX && axis != AXIS_STP_RESERVED)
		{
			status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, axis);
			if(!status.bOriginComplete)
			{
				bReturn = FALSE;
			}
		}
	}

	return bReturn;
}
//2017-09-01,SKCHO, 요청 대기 시간 지나면 메시지 출력
void  CUnitCtrlBank::DisplayAGVMsg()
{
	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_SHOW_DLG, (WPARAM)eDLG_AGVMsg, (LPARAM)1);	
}


//2017-09-01,SKCHO, 설비 상태에와 램프 상태 매칭
int CUnitCtrlBank::GetMachinetoLampState()
{
	int nState = theProcBank.GetMachineState();
	int nLampState = LAMP_PM;
	if(nState == CONST_MACHINE_STATE::MACHINE_PAUSE)
	{
		nLampState = LAMP_STOP;
	}
	else if(nState == CONST_MACHINE_STATE::MACHINE_INIT)
	{
		nLampState = LAMP_INIT;
	}
	else if(nState == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
        nLampState = LAMP_IDLE;
	}
	else if(nState == CONST_MACHINE_STATE::MACHINE_RUN)
	{
		nLampState = LAMP_RUN;
	}
	else if(nState == CONST_MACHINE_STATE::MACHINE_FAULT || nState == CONST_MACHINE_STATE::MACHINE_INTERLOCK)
	{
		nLampState = LAMP_ALARM;
	}
	return nLampState;
}

//2017-09-01,SKCHO, 요청 대기 시간 지나면 메시지 출력
void  CUnitCtrlBank::DisplayPortEmptyMsg()
{
	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_SHOW_DLG, (WPARAM)eDLG_PortEmptyMsg, (LPARAM)1);	
}

//2017-11-17, YMG AGV 트레이 투입완료, 배출 완료시 메시지 숨김.
void  CUnitCtrlBank::HideAGVMsg()
{
	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_SHOW_DLG, (WPARAM)eDLG_AGVMsg, (LPARAM)0);	
}


//2017-10-07,SKCHO
void CUnitCtrlBank::VacuumIOState()
{
	
	JIG_ID nJig =JIG_ID_A;
	INPUT_ID nINPUTID_CH1 =X_A_JIG_CH1_VAC_SEN , nINPUTID_CH2=X_A_JIG_CH2_VAC_SEN;
	UINT nCONTROLID_CH1=IDC_GXSTC_A_ZONE_VAC1, nCONTROLID_CH2=IDC_GXSTC_A_ZONE_VAC2;

	for(int i = 0; i < ZONE_ID_MAX; i++)
	{
	   nJig = (JIG_ID)(theUnitFunc.AMT_GetAutoJigID((ZONE_ID)i));
	   if(nJig == JIG_ID_A)
	   {
          nINPUTID_CH1 = X_A_JIG_CH1_VAC_SEN;
		  nINPUTID_CH2 = X_A_JIG_CH2_VAC_SEN;		 
	   }
	   else if(nJig == JIG_ID_B)
	   {
		  nINPUTID_CH1 = X_B_JIG_CH1_VAC_SEN;
		  nINPUTID_CH2 = X_B_JIG_CH2_VAC_SEN;		
	   }
	   else if(nJig == JIG_ID_C)
	   {
		  nINPUTID_CH1 = X_C_JIG_CH1_VAC_SEN;
		  nINPUTID_CH2 = X_C_JIG_CH2_VAC_SEN;		
	   }
	   else if(nJig == JIG_ID_D)
	   {
		  nINPUTID_CH1 = X_D_JIG_CH1_VAC_SEN;
		  nINPUTID_CH2 = X_D_JIG_CH2_VAC_SEN;		
	   }

		if(theUnitFunc.GetInPutIOCheck(nINPUTID_CH1))
		{			
			theProcBank.m_bInPutIO[nINPUTID_CH1] = TRUE;
		}
		else
		{		
			theProcBank.m_bInPutIO[nINPUTID_CH1] = FALSE;
		}
		if(theUnitFunc.GetInPutIOCheck(nINPUTID_CH2))
		{			
			theProcBank.m_bInPutIO[nINPUTID_CH2] = TRUE;
		}
		else
		{
			theProcBank.m_bInPutIO[nINPUTID_CH2] = FALSE;
		}
	 
	}

	if(theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_VAC_SEN))
	{	
		theProcBank.m_bInPutIO[X_LD_ROBOT_VAC_SEN] = TRUE;
	}
	else
	{	 
	   theProcBank.m_bInPutIO[X_LD_ROBOT_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_PA_TR_CH1_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_PA_TR_CH1_VAC_SEN] = TRUE;
	}
	else
	{	
	  theProcBank.m_bInPutIO[X_PA_TR_CH1_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_PA_TR_CH2_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_PA_TR_CH2_VAC_SEN] = TRUE;
	}
	else
	{	  
	   theProcBank.m_bInPutIO[X_PA_TR_CH2_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_PA_STG_CH1_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_PA_STG_CH1_VAC_SEN] = TRUE;
	}
	else
	{	 
	   theProcBank.m_bInPutIO[X_PA_STG_CH1_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_PA_STG_CH2_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_PA_STG_CH2_VAC_SEN] = TRUE;
	}
	else
	{	  
	   theProcBank.m_bInPutIO[X_PA_STG_CH2_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH1_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_LD_UPPER_SHUTTLE_CH1_VAC_SEN] = TRUE;
	}
	else
	{	
	   theProcBank.m_bInPutIO[X_LD_UPPER_SHUTTLE_CH1_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH2_VAC_SEN))
	{	
		theProcBank.m_bInPutIO[X_LD_UPPER_SHUTTLE_CH2_VAC_SEN] = TRUE;
	}
	else
	{	
	   theProcBank.m_bInPutIO[X_LD_UPPER_SHUTTLE_CH2_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH1_VAC_SEN))
	{
	   theProcBank.m_bInPutIO[X_LD_LOWER_SHUTTLE_CH1_VAC_SEN] = TRUE;
	}
	else
	{
	   theProcBank.m_bInPutIO[X_LD_LOWER_SHUTTLE_CH1_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH2_VAC_SEN))
	{
	   theProcBank.m_bInPutIO[X_LD_LOWER_SHUTTLE_CH2_VAC_SEN] = TRUE;
	}
	else
	{
	   theProcBank.m_bInPutIO[X_LD_LOWER_SHUTTLE_CH2_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_LD_TR_CH1_VAC_SEN))
	{	
		theProcBank.m_bInPutIO[X_LD_TR_CH1_VAC_SEN] = TRUE;
	}
	else
	{	   
	   theProcBank.m_bInPutIO[X_LD_TR_CH1_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_LD_TR_CH2_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_LD_TR_CH2_VAC_SEN] = TRUE;
	}
	else
	{	  
		theProcBank.m_bInPutIO[X_LD_TR_CH2_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_ULD_TR_CH1_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_ULD_TR_CH1_VAC_SEN] = TRUE;
	}
	else
	{	 
	   theProcBank.m_bInPutIO[X_ULD_TR_CH1_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_ULD_TR_CH2_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_ULD_TR_CH2_VAC_SEN] = TRUE;
	}
	else
	{
	   theProcBank.m_bInPutIO[X_ULD_TR_CH2_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_ULD_STAGE_CH1_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_ULD_STAGE_CH1_VAC_SEN] = TRUE;
	}
	else
	{	  
	   theProcBank.m_bInPutIO[X_ULD_STAGE_CH1_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_ULD_STAGE_CH2_VAC_SEN))
	{		
		theProcBank.m_bInPutIO[X_ULD_STAGE_CH2_VAC_SEN] = TRUE;
	}
	else
	{	 
	   theProcBank.m_bInPutIO[X_ULD_STAGE_CH2_VAC_SEN] = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_VAC_SEN))
	{	
		theProcBank.m_bInPutIO[X_ULD_ROBOT_VAC_SEN] = TRUE;
	}
	else
	{	 
	   theProcBank.m_bInPutIO[X_ULD_ROBOT_VAC_SEN] = FALSE;
	}

}
//2017-10-07,SKCHO
void CUnitCtrlBank::AGVSendIOState()
{

	for(int i = Y_GOOD_ULD_CONV_TRAY_PIO_MODE; i<= Y_NG_CONV_TRAY_PIO_IN_8; i++)
	{
		if(theUnitFunc.GetOutPutIOCheck((OUTPUT_ID)i))
		{	
			theProcBank.m_bOutPutIO[i] = TRUE;
		}
		else
		{	 
			theProcBank.m_bOutPutIO[i] = FALSE;
		}
	}

}
//2017-10-09,SKCHO
void CUnitCtrlBank::AGVReceiveIOState()
{
	BOOL bRtn = FALSE;	

	bRtn =	GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_PIO_REQ);  //request
	theProcBank.m_bInPutIO[X_GOOD_LD_CONV_TRAY_PIO_REQ] = bRtn;
	
	bRtn =	GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_PIO_REQ);
	theProcBank.m_bInPutIO[X_GOOD_ULD_CONV_TRAY_PIO_REQ] = bRtn;	

	bRtn =	GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_PIO_BUSY);
	theProcBank.m_bInPutIO[X_GOOD_LD_CONV_TRAY_PIO_BUSY] = bRtn;

	bRtn =	GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_PIO_BUSY);
	theProcBank.m_bInPutIO[X_GOOD_ULD_CONV_TRAY_PIO_BUSY] = bRtn;

	bRtn =	GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_PIO_COMP);
	theProcBank.m_bInPutIO[X_GOOD_LD_CONV_TRAY_PIO_COMP] = bRtn;
	
	bRtn =	GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_PIO_COMP);
	theProcBank.m_bInPutIO[X_GOOD_ULD_CONV_TRAY_PIO_COMP] = bRtn;

}

void CUnitCtrlBank::AutoTeachKeyIOState()
{
	BOOL bRtn = FALSE;
	bRtn =	GetInPutIOCheck(X_TEACH_KEY_SW);
	theProcBank.m_bInPutIO[X_TEACH_KEY_SW] = bRtn;
}
BOOL CUnitCtrlBank::GetInPutIOState(INPUT_ID nID)
{
	return theProcBank.m_bInPutIO[nID];
}
BOOL CUnitCtrlBank::GetOutPutIOState(OUTPUT_ID nID)
{
	return theProcBank.m_bOutPutIO[nID];
}

//2018-04-01,GHLEE, FDC 오보고 관련 테스트
void CUnitCtrlBank::SetFDCCellInfo(CELL_POS nPos)
{
	CCellInfo* pCell;

	JIG_ID nJig = JIG_ID_A;
	JIG_CH nCh = JIG_CH_1;

	pCell = theCellBank.GetCellInfo(nPos);
	if( nPos < CELL_POS_UD_TR_CH1 )
	{
		if( nPos == CELL_POS_AZONE_CH1)
		{
			nJig = JIG_ID_A;
			nCh = JIG_CH_1;
		}
		else if( nPos == CELL_POS_AZONE_CH2 )
		{
			nJig = JIG_ID_A;
			nCh = JIG_CH_2;
		}
		else if( nPos == CELL_POS_BZONE_CH1 )
		{
			nJig = JIG_ID_B;
			nCh = JIG_CH_1;
		}
		else if( nPos == CELL_POS_BZONE_CH2 )
		{
			nJig = JIG_ID_B;
			nCh = JIG_CH_2;
		}
		else if( nPos == CELL_POS_CZONE_CH1 )
		{
			nJig = JIG_ID_C;
			nCh = JIG_CH_1;
		}
		else if( nPos == CELL_POS_CZONE_CH2 )
		{
			nJig = JIG_ID_C;
			nCh = JIG_CH_2;
		}
		else if( nPos == CELL_POS_DZONE_CH1 )
		{
			nJig = JIG_ID_D;
			nCh = JIG_CH_1;
		}
		else if( nPos == CELL_POS_DZONE_CH2 )
		{
			nJig = JIG_ID_D;
			nCh = JIG_CH_2;
		}

		if( pCell != NULL )
		{
			theFDCBank.m_nJig_StepID[nJig][nCh] = 2;
			theFDCBank.m_strJig_CellID[nJig][nCh] = pCell->m_strCellID;
		}
		else
		{
			theFDCBank.m_nJig_StepID[nJig][nCh] = 0;
			theFDCBank.m_strJig_CellID[nJig][nCh] = _T(" ");
		}
	}
	else
	{
		if( nPos <= CELL_POS_UD_TR_CH2)
		{
			nCh = (JIG_CH)(nPos-CELL_POS_UD_TR_CH1);
			
			if( pCell != NULL )
			{
				theFDCBank.m_nULDTR_StepID[nCh] = 2;
				theFDCBank.m_strULDTR_CellID[nCh] = pCell->m_strCellID;
			}
			else
			{
				theFDCBank.m_nULDTR_StepID[nCh] = 0;
				theFDCBank.m_strULDTR_CellID[nCh] = _T(" ");
			}
		}
		else if( nPos <= CELL_POS_UD_STAGE_CH2 )
		{
			nCh = (JIG_CH)(nPos-CELL_POS_UD_STAGE_CH1);
			
			if( pCell != NULL )
			{
				theFDCBank.m_nULDStage_StepID[nCh] = 2;
				theFDCBank.m_strULDStage_CellID[nCh] = pCell->m_strCellID;
			}
			else
			{
				theFDCBank.m_nULDStage_StepID[nCh] = 0;
				theFDCBank.m_strULDStage_CellID[nCh] = _T(" ");
			}
		}
	}
}

//2018-05-16,MGYUN, AGV 효율 관련 딜레이 추가
void CUnitCtrlBank::WaitDelayTime(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();
	while(GetTickCount() - dwStart < dwMillisecond)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}