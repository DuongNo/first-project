#include "StdAfx.h"
#include "StateLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// �޴���� �� �޴� ������
int CStateManualReceive::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:	
		
		// LD Conv�� ����� ƼĪ ����ŭ �̵� �Ѵ�.
		LDConvMove();
		m_Timer.Start();
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		nStep++;
	
		break;
	case stepEnd:				// Edge Align ��û �� End
		m_bRtn[0] = LDConvCellChk(CONV_SEN_3);
		if (IsReturnOk())
		{
			// �̵� �ϴ� ���߿� Cell Sensorüũ �� Stop.
			LDConvStop();

			// Cell Data ���� ����. Inner ID�� ����.
			CellData_LDConvIn();
			// Main ȭ�鿡 �ѷ��� In Cell ���� ������Ʈ
			theProductBank.UpdateMainInputCount();
			// Cim�� ���� Cell�� ���Դٴ°� �˸�.
			// ���� ���� ���¿����� ���� ������ ������ ���̵� ���¿�����  Run���� ���� �Ѵ� 2017/06/09 HSK.
			// ���� Run���� �ٲ�� ������ �δ� �κ��� ���� �� �ķ� �ٲ۴�. 2017/06/17 HSK.
			//if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
			//	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);

			LoaderAlignStart();
			// Load Tact ����.
			TactUpdate(LOADER_TACT);

			nStep = stepIdle;		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			LDConvStop();
			LDConvZeroPointSet();
			// ���� �ȵǵ� ���� ����.2017/05/01 HSK.
			nStep = stepIdle;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd -> Panel Not Sensing"),m_strStateName);
			// �ȶ��� �Ǵ� �˶� 2017/04/27 HSK
//			SetError(ALARM_LD_CONV_SEN3);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}
int CStateConvMove::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:	
		// LD Conv�� ����� ƼĪ ����ŭ �̵� �Ѵ�.
		LDConvMove(5.0);
		m_Timer.Start();
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		nStep++;
		break;
	case stepEnd:				// Edge Align ��û �� End

		m_bRtn[0] = LDConvCellChk(CONV_SEN_3);
		m_bRtn[1] = LDConvStopChk();
		if (IsReturnOk())
		{
			LDConvZeroPointSet();
			// Cell Data ���� ����. Inner ID�� ����.
			CellData_LDConvIn();
			// Main ȭ�鿡 �ѷ��� In Cell ���� ������Ʈ
			theProductBank.UpdateMainInputCount();
			// Cim�� ���� Cell�� ���Դٴ°� �˸�.
			// ���� ���� ���¿����� ���� ������ ������ ���̵� ���¿�����  Run���� ���� �Ѵ� 2017/06/09 HSK.
			// ���� Run���� �ٲ�� ������ �δ� �κ��� ���� �� �ķ� �ٲ۴�. 2017/06/17 HSK.
			//if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
			//	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);

			LoaderAlignStart();
			// Load Tact ����.
			TactUpdate(LOADER_TACT);

			nStep = stepIdle;		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			LDConvZeroPointSet();
			// ���� �ȵǵ� ���� ����.2017/05/01 HSK.
			nStep = stepIdle;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd -> Panel Not Sensing"),m_strStateName);
			// �ȶ��� �Ǵ� �˶� 2017/04/27 HSK
//			SetError(ALARM_LD_CONV_SEN3);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}
// �մ� ���� �� �޴� ������
int CStateCellReceive::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// Cell ���� �� �ִ� �غ� I/O On
		// Cell ���� �غ� ��ٰ� �˷���.
		SendRcvEnable(ON);
		SendRcvStart(OFF);
		SendRcvComp(OFF);
		SendRcvPause(OFF);
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepConvMove:			// �� ���� Conv Move I/O On Check �� Loader Conv Move
		m_bRtn[0] = RcvEnableChk();
		if (IsReturnOk())
		{
			// Conv ������ �ȴٰ� �˷���
			SendRcvEnable(ON);
			SendRcvStart(ON);
			SendRcvComp(OFF);
			SendRcvPause(OFF);
			m_Timer.Start();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepConvMove"),m_strStateName);
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_UpperIFWaitTime)
		{
			SendRcvEnable(OFF);
			SendRcvStart(OFF);
			SendRcvComp(OFF);
			SendRcvPause(ON);
			SetError(ALARM_UPPER_ENABLE_CHK);
		}
		break;
	case stepStartChk:
		m_bRtn[0] = RcvStartChk();
		if (IsReturnOk())
		{
			LDConvMove();

			m_Timer.Start();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStartChk"),m_strStateName);
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_UpperIFWaitTime)
		{
			SendRcvEnable(OFF);
			SendRcvStart(OFF);
			SendRcvComp(OFF);
			SendRcvPause(ON);
			SetError(ALARM_UPPER_START_CHK);
		}
		break;
	case stepSenChk: // Sensor 3 chk Stop
		if(theConfigBank.m_Option.m_bLoadPitchIn == FALSE)
			m_bRtn[0] = LDConvCellChk(CONV_SEN_3);
		else
			m_bRtn[0] = LDConvCellChk(CONV_SEN_1);

		//2018-01-11, JSJUNG, �ε� �����̾� 1������ ������ �������� ���Ͽ� ��������� ��°�� ���
		//                    ��Ÿ�� �������� �ð� �ٽ� ���� ����
		if (m_Timer.Stop(FALSE) > 0.1 && IsReturnOk())
		{
			// 2017.2.9 bgkim
			// Cell�� LoadConv Sensor3�� ���������� ������ ��� Ÿ�̸Ӹ� �����Ѵ�.
//			theProcBank.m_timerLoadConv.Start();

			LDConvStop();
			m_Timer.Start();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepSenChk"),m_strStateName);
			nStep++;
		}
		//2017-09-06,SKCHO, LD CONV SEN3 �˶� ���ֱ� ����
		else if (m_Timer.Stop(FALSE) > 9.9 && m_Timer.Stop(FALSE) <= 10.)
		{
			SendRcvEnable(OFF);
			SendRcvStart(OFF);
			SendRcvComp(ON);
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
				
			LDConvStop();		
		
			if(theConfigBank.m_Option.m_bSkipLDConvSen3 == TRUE)
			{			
				SendRcvComp(OFF);
				SendRcvPause(OFF);							
				nStep = stepIdle;	
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Sen3Chk Alarm-Skip"),m_strStateName);
			}
			else
			{
				SendRcvComp(OFF);
				SendRcvPause(ON);
    			if(m_bRtn[0] == FALSE)	
				{
				   SetError(ALARM_LD_CONV_SEN3);
				   theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Sen3Chk Alarm"),m_strStateName);
				}
			}
			
		}
		break;
	case stepCompChk:
		m_bRtn[0] = RcvCompChk();
		if (IsReturnOk())
		{
			// Cell �޾Ҵٰ� �˸�.
			SendRcvEnable(ON);
			SendRcvStart(ON);
			SendRcvComp(ON);
			SendRcvPause(OFF);
			m_Timer.Start();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCompChk"),m_strStateName);
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_UpperIFWaitTime)
		{
			SendRcvEnable(OFF);
			SendRcvStart(OFF);
			SendRcvComp(OFF);
			SendRcvPause(ON);
			SetError(ALARM_UPPER_COMP_CHK);
		}
		break;
	case stepEnd:				// Edge Align ��û �� End
		m_bRtn[0] = RcvEnableChk();
		m_bRtn[1] = RcvStartChk();
		m_bRtn[2] = RcvCompChk();
		if (m_bRtn[0] == FALSE && m_bRtn[1] == FALSE && m_bRtn[2] == FALSE) 
		{
			if(theConfigBank.m_Option.m_bLoadPitchIn == FALSE)
			{
				// Cell Data ���� ����. Inner ID�� ���� ����.
				CellData_LDConvIn();
				// Load Tact ����.
				TactUpdate(LOADER_TACT);
				// Main ȭ�鿡 �ѷ��� In Cell ���� ������Ʈ
				theProductBank.UpdateMainInputCount();
				// Cim�� ���� Cell�� ���Դٴ°� �˸�.
				LoaderAlignStart();

				// ���� ���� ���¿����� ���� ������ ������ ���̵� ���¿�����  Run���� ���� �Ѵ� 2017/06/09 HSK.
				// ���� Run���� �ٲ�� ������ �δ� �κ��� ���� �� �ķ� �ٲ۴�. 2017/06/17 HSK.
				//if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
				//	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);
			}

			// �ְ� �޾Ҵ� ��ȣ Reset.
			SendRcvEnable(OFF);
			SendRcvStart(OFF);
			SendRcvComp(OFF);
			SendRcvPause(OFF);
			nStep = stepIdle;		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);	
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}
/*
int CStateCellReceive::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// Cell ���� �� �ִ� �غ� I/O On
		if(IsCellLoadConv() && !RcvEnableChk())
		{
			LDConvMove();
			m_Timer.Start();
			nStep = stepSenChk;
			break;
		}
		
		if(RcvEnableChk())
		{
			// Cell ���� �غ� ��ٰ� �˷���.
			SendRcvEnable(ON);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepConvMove:			// �� ���� Conv Move I/O On Check �� Loader Conv Move
		m_bRtn[0] = RcvEnableChk();
		if (IsReturnOk())
		{
			// Conv ������ �ȴٰ� �˷���
			SendRcvStart(ON);
			m_Timer.Start();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepConvMove"),m_strStateName);
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_UpperIFWaitTime)
		{
			SetError(ALARM_UPPER_ENABLE_CHK);
		}
		break;
	case stepStartChk:
		m_bRtn[0] = RcvStartChk();
		if (IsReturnOk())
		{
			LDConvMove();
			m_Timer.Start();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStartChk"),m_strStateName);
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_UpperIFWaitTime)
		{
			SetError(ALARM_UPPER_START_CHK);
		}
		break;
	case stepSenChk: // Sensor 3 chk Stop
		m_bRtn[0] = LDConvCellChk(CONV_SEN_3);
		if (IsReturnOk())
		{

			LDConvStop();
			//killtimer(1);
			
//			LDConvZeroPointSet();
			m_Timer.Start();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepSenChk"),m_strStateName);

			if(IsCellLoadConv())// && !RcvEnableChk())
			{
				// 2017.2.9 bgkim
				// Cell�� ���������� �κ�Ʈ�� �ε��� ��� Ÿ�̸Ӹ� �����Ѵ�.
				theProcBank.m_timerLoadConv.Start();
				m_Timer.Start();
				nStep = stepEnd;
				break;
			}
			else
			{
				theProcBank.m_timerLoadConv.Start();
				m_Timer.Start();
				nStep++;
				break;
			}
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			LDConvStop();
//			LDConvZeroPointSet();

			if(IsCellLoadConv() && !RcvEnableChk())
			{
				m_Timer.Start();
				nStep = stepIdle;
				break;
			}

//			SetError(ALARM_LD_CONV_SEN3);
			nStep = stepIdle;
			break;
		}
		break;
	case stepCompChk:
		m_bRtn[0] = RcvCompChk();
		if (IsReturnOk())
		{
			// Cell �޾Ҵٰ� �˸�.
			SendRcvComp(ON);
			m_Timer.Start();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCompChk"),m_strStateName);
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_UpperIFWaitTime)
		{
			SetError(ALARM_UPPER_COMP_CHK);
		}
		break;
	case stepEnd:				// Edge Align ��û �� End
		m_bRtn[0] = RcvEnableChk();
		m_bRtn[1] = RcvStartChk();
		m_bRtn[2] = RcvCompChk();
		if (m_bRtn[0] == FALSE && m_bRtn[1] == FALSE && m_bRtn[2] == FALSE) 
		{
			// Cell Data ���� ����. Inner ID�� ���� ����.
			CellData_LDConvIn();
			// Main ȭ�鿡 �ѷ��� In Cell ���� ������Ʈ
			theProductBank.UpdateMainInputCount();
			// Cim�� ���� Cell�� ���Դٴ°� �˸�.
			CIM_CellExistInMachine();
			// �ְ� �޾Ҵ� ��ȣ Reset.
			SendRcvEnable(OFF);
			SendRcvStart(OFF);
			SendRcvComp(OFF);
			nStep = stepIdle;		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);	
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}
*/

// �κ� �� �ε� ������
int CStateLDRobotGet::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	long write;
	switch(nStep)
	{
	case stepStart:
		// ���� ����� ����.
//		LoaderAlignStart();
		if(theProcBank.m_nLDRobotRetryStep == 0)
		{				
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			
			//2017-12-18,SKCHO,�׻� RETRY �߰�
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_RETRY);
		}
		else if(theProcBank.m_nLDRobotRetryStep == 1)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart - Retry"),m_strStateName);	
		}
		m_Timer.Start();
		nStep++;
		break;
	case stepAlignChk:					// Edge Align Ȯ�� �� Loading Pos �غ� �Ϸ�
		// ���� ����� ���� üũ.
		if(theProcBank.m_nLDRobotRetryStep == 1)
		{
			m_bRtn[0] = TRUE;
		}
		else
		{
			m_bRtn[0] = LoaderAlignEndChk();
		}
		m_bRtn[1] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET);
		if(IsReturnOk())
		{
			if(theProcBank.m_nLDRobotRetryStep == 1)
			{
				m_bRtn[0] = TRUE;
			}
			else
			{
				// ���� ����� ��� üũ.
				m_bRtn[0] = LoaderAlignResultChk();
			}
			
			if(IsReturnOk())
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepAlignChk"),m_strStateName);	
				write = 4; // GetOK ��ȣ ��.
				theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
				m_Timer.Start();
				nStep++;
			}
			else
			{
				SetError(ALARM_LD_EDGE_ALIGN_NG);
			}
		}
		else if(m_Timer.Stop(FALSE) > 7.)  //2017-12-12,SKCHO,visoin grap���� �߻��� ī�޶� �翬�� ���� �ð� 5.5���̾ �ð�5->7�� ����
		{
			LDRobotBitAllOff(E_CELL_LOADING,FALSE);
			if (m_bRtn[0] == FALSE)
				SetError(ALARM_LD_LOADER_ALIGN_TIME_OUT);
			else if(m_bRtn[1] == FALSE)
				SetError(ALARM_LD_ROBOT_RUNNINGACK);
		}
		break;
	case stepViCompChk:	// Align Data Send
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VICOMP);
		// �κ� Align ���� ���� �ִ��� üũ.
		m_bRtn[1] = theSocketInterFace.m_pSocket_RobotAlign.IsConnected();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepViCompChk"),m_strStateName);
			
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5. && m_Timer.Stop(FALSE) < 5.5)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			LDRobotBitAllOff(E_CELL_LOADING,FALSE);
			if(m_bRtn[0] == FALSE)
				SetError(ALARM_LD_ROBOT_VICOMPACK);
			else
				SetError(ALARM_COMM_ROBOTALIGN);
		}
		break;
	case stepDataSendDelay:
		
		// ���� ������ üũ �� 200ms �ִٰ� ����Ÿ ����. �ȱ׷� ���޴� ��찡 �����....
		if(m_Timer.Stop(FALSE) > 0.5)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepDataSendDelay"),m_strStateName);
			// 200ms ������ �ľ���� ����Ÿ ����.
			LDRobotSendAlignData(LoaderAlignResultX(),LoaderAlignResultY(),LoaderAlignResultT());

			// Align Data ���� �Ϸ� Bit �����.
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VICOMP);
			// Get Purmit ��ȣ ��.
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_CVGET);

			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotGet:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_CVGET);
		if (m_bRtn[0] == FALSE)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotGet"),m_strStateName);
			//2017.10.31, JSJUNG. �۹� �ñ׳� ����
			write = 0;
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQCOMPRTN);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5. && m_Timer.Stop(FALSE) < 5.5)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQSKIP);
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			LDRobotBitAllOff(E_CELL_LOADING,FALSE);
			SetError(ALARM_LD_ROBOT_CVGETINTERLOCKOFFACK);
		}
		break;
	case stepVacOn:					// VAC ON ��ȣ ��û ���� �� 	
		
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACON); 
		if (IsReturnOk())
		{
			//long TXData[1],TYData[1],TZData[1],TTData[1],MXData[1],MYData[1],MZData[1],MTData[1];

			//theDeviceMelsec.RobotWordRecvBuf(LDROBOT_TEACH_X_VAL,sizeof(long), TXData);
			//theDeviceMelsec.RobotWordRecvBuf(LDROBOT_TEACH_Y_VAL,sizeof(long), TYData);
			//theDeviceMelsec.RobotWordRecvBuf(LDROBOT_TEACH_Z_VAL,sizeof(long), TZData);
			//theDeviceMelsec.RobotWordRecvBuf(LDROBOT_TEACH_T_VAL,sizeof(long), TTData);
			//theDeviceMelsec.RobotWordRecvBuf(LDROBOT_MOVE_X_VAL, sizeof(long), MXData);
			//theDeviceMelsec.RobotWordRecvBuf(LDROBOT_MOVE_Y_VAL, sizeof(long), MYData);
			//theDeviceMelsec.RobotWordRecvBuf(LDROBOT_MOVE_Z_VAL, sizeof(long), MZData);
			//theDeviceMelsec.RobotWordRecvBuf(LDROBOT_MOVE_T_VAL, sizeof(long), MTData);

			//theLog[LOG_TEACH_MOVE].AddBuf( _T("LD TEACH X[%f], Y[%f], Z[%f], T[%f]"),TXData[0]/100, TYData[0]/100, TZData[0]/100, TTData[0]/100);
			//
			//theLog[LOG_TEACH_MOVE].AddBuf( _T("LD MOVE X[%f], Y[%f], Z[%f], T[%f]"), MXData[0]/100, MYData[0]/100, MZData[0]/100, MTData[0]/100);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOn"),m_strStateName);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQCOMPRTN);
			// �κ� VAC ON
			RobotCellVacOn(VAC_ON,E_LD_ROBOT);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			LDRobotBitAllOff(E_CELL_LOADING,FALSE);
			SetError(ALARM_LD_ROBOT_VACONACK);
			
		}
		break;
	case stepVacOnChk:
		m_bRtn[0] =  RobotCellVacOnChk(E_LD_ROBOT,VAC_ON);
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOnChk"),m_strStateName);
			// VAC ON �Ȱ� üũ �ϰ� VAC ON BIT �츲.
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
			// VAC ON OFF�� �κ��� ���� üũ �ϹǷ� CELL DATA �̵�. �κ��� Cell�� ���ų� ���� ���� Vac�� ���� Cell Data�� �̵��ϵ��� �Ѵ�.
			CellData_LDRobotFromLDConvIn();

			// ����� ����ÿ� LD CONV�� ���� �־ IDLE�̹Ƿ� ���⼭ RUN���� �ٲ��ش�. 2017/06/17 HSK.
			if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
			{
				//2017-07-20, jsjung, ���� IDLE->RUN ����� IDLE TIME ����
				::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLECHAGNERUN_UPDATE, 0,0); 
				theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("���� IDLE->RUN ����"));

				theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);
			}

			// Ch Skip Set�ϴ� �Լ�. ���� ��� ����. 2017/04/26 HSK.
			//SetChSkip();

			//2017-12-01,SKCHO
			if(theProcBank.m_nLDRobotRetryStep == 1)
			{
				theProductBank.m_LDROBOT_RetryInfo.nSucess++;
			}
			theProcBank.m_nLDRobotRetryStep = 0;

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 2.)
		{
			LDRobotBitAllOff(E_CELL_LOADING,FALSE);
			if(theConfigBank.m_Option.m_bUseLDRobotRetry == TRUE)
			{				
				if(theProcBank.m_nLDRobotRetryStep == 0)
				{				
					theProcBank.m_nLDRobotRetryStep = 1;
					m_Timer.Start();
					//--nStep = stepBitAllOff;
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOnChk Fail - Retry"),m_strStateName);

					//2017-12-18,SKCHO, RETRY �߰�
					nStep = stepRobotGet;
					theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
					RobotCellVacOn(VAC_OFF,E_LD_ROBOT);

					//2017-12-01,SKCHO
					theProductBank.m_LDROBOT_RetryInfo.nTotal++;
				}
				else if(theProcBank.m_nLDRobotRetryStep == 1)
				{
					//2017-12-18,SKCHO
					RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
					theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQERRRTN);   //error retrun

					SetError(ALARM_LD_ROBOT_VACON);
					theProcBank.m_nLDRobotRetryStep = 0;

					theUnitFunc.RobotBlowOff(E_LD_ROBOT);
					

				}
			}
			else
			{
				//2017-12-18,SKCHO
				RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQERRRTN);   //error retrun

				SetError(ALARM_LD_ROBOT_VACON);
				theProcBank.m_nLDRobotRetryStep = 0;

				theUnitFunc.RobotBlowOff(E_LD_ROBOT);
				
			}			
		}
		break;
	case stepRobotGetComp:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_CVGETCOMP);
		if (IsReturnOk())
		{
			//2018/01/26, JSJUNG, LD ROBOT ƼĪ��/���� ��ġ��(���念��/READ ��)
			long Data[8];

			theDeviceMelsec.RobotWordRecvBuf(LDROBOT_TEACH_MOVE_XYZT_VAL,sizeof(long)*8,Data);

			theLog[LOG_LDRB_TEACH_MOVE].AddBuf( _T("LD GET TEACH X[%f], Y[%f], Z[%f], T[%f]"),(float)Data[0]/100, (float)Data[1]/100, (float)Data[2]/100, (float)Data[3]/100);
			
			theLog[LOG_LDRB_TEACH_MOVE].AddBuf( _T("LD GET MOVE X[%f], Y[%f], Z[%f], T[%f]"), (float)Data[4]/100, (float)Data[5]/100, (float)Data[6]/100, (float)Data[7]/100);
			
			//2018-03-08,MGYUN,LD ROBOT TEACHING DATA MELSEC ���.
			theMonitorBank.WriteLDRobotPOS( (double)Data[0]/100, (double)Data[1]/100, (double)Data[2]/100, (double)Data[3]/100);
			
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotGetComp"),m_strStateName);
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQCOMPRTN);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			LDRobotBitAllOff(E_CELL_LOADING,FALSE);
			SetError(ALARM_LD_ROBOT_GETCOMPACK);
		}
		break;
	case stepEnd:				// Vac Check �� End.
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_CVGET);
		m_bRtn[1] =  RobotCellVacOnChk(E_LD_ROBOT,VAC_ON);
		if(IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQCOMPRTN);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			LDRobotBitAllOff(E_CELL_LOADING,FALSE);

			//2018-05-22,NAM, Having Cell in Machine.
			theProcBank.m_CellLoadChk = FALSE;

			//2017-12-01,SKCHO
			if(theProcBank.m_nLDRobotRetryStep == 1)
			{
				theProductBank.m_LDROBOT_RetryInfo.nSucess++;
			}

			theProcBank.m_nLDRobotRetryStep = 0;
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			theProcBank.m_nLDRobotRetryStep = 0; 
			LDRobotBitAllOff(E_CELL_LOADING,FALSE);
			if(m_bRtn[0] == FALSE)		SetError(ALARM_LD_ROBOT_CVGETINTERLOCKONACK);
			else if(m_bRtn[1] == FALSE)	
			{			
				/*if(theConfigBank.m_Option.m_bUseLDRobotRetry == TRUE)
				{				
					if(theProcBank.m_nLDRobotRetryStep == 0)
					{				
						theProcBank.m_nLDRobotRetryStep = 1;
						m_Timer.Start();
						nStep = stepBitAllOff;
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd Fail - Retry"),m_strStateName);
						//2017-12-01,SKCHO
					    theProductBank.m_LDROBOT_RetryInfo.nTotal++;
					}
					else if(theProcBank.m_nLDRobotRetryStep == 1)
					{				
						SetError(ALARM_LD_ROBOT_VACON);
						theProcBank.m_nLDRobotRetryStep = 0;
					}
				}
				else
				{
					SetError(ALARM_LD_ROBOT_VACON);
					theProcBank.m_nLDRobotRetryStep = 0;
				}	*/
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd VAC FAIL"),m_strStateName);
				SetError(ALARM_LD_ROBOT_VACON);
			}
		}
		break;
	case stepBitAllOff:	// Robot�� Vac ��ȣ�� �� ��� ���� ��Ʈ�� ���ֵ��� �Ѵ�.(Vac ��ȣ�� ���Ƿ� �ǵ帱 ��� Robot�� ���� ���� �ʴ´ٰ� �Ѵ�......)
		LDRobotBitAllOff(E_ORG);
		LDRobotBitAllOff(E_CELL_LOADING,FALSE);	
		LDRobotBitAllOff(E_PUT_1,FALSE);	
		LDRobotBitAllOff(E_PUT_2,FALSE);	
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepBitAllOff"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepRobotRunningChk: // ���⼭ ���� Robot ORG ������.
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
		}		
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotRunningChk"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepRobotPauseOn:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMEND_Y_PAUSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotPauseOn"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case seqRobotExtStop:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] seqRobotExtStop"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case seqRobotMotorOn:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_STOPPING_Y_EXTMOTORON);		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] seqRobotMotorOn"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotOnChk:	
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MOTORON_Y_LOWSPEED);	
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_ALARM_Y_EXTRESET);		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotOnChk"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 1.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_LD_ROBOT_MOTOR_ENERGIZE_ONACK);		
		}
		break;
	case stepPGMSel:		
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMACK_Y_PGMSEL1);			
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPGMSel"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepPGMStrobe:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_HOMEPOS2_Y_PGMSTROBE);			
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPGMStrobe"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepExtStart:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_TEACHMODE_Y_EXTSTART);		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepExtStart"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRunningChk:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET);	
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_BATWARN_Y_ORG);		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRunningChk"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_LD_ROBOT_RUNNINGACK);		
		}
		break;
	case stepOrgOff:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP);	
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_BATWARN_Y_ORG);		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepOrgOff"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 30.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_LD_ROBOT_HOMEPOSACK);			
		}
		break;
	case stepInterlockChk: // Robot�� ORG�� ���� �� �κ� Interlock�� üũ �Ѵ�. Interlock��ȣ�� ���� �ִ� ��쿣 Org�� ����� ���� ���� ����. �׸��� Z Up.
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_CVGET);
		m_bRtn[1] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT);
		m_bRtn[2] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT);	
		if (IsReturnOk())
		{		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepInterlockChk"),m_strStateName);
			m_Timer.Start();
			nStep = stepStart;
		}
		else if (m_Timer.Stop(FALSE) > 30.)
		{
			if (m_bRtn[0] == FALSE)			SetError(ALARM_LD_ROBOT_CVGETINTERLOCKONACK);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_LD_ROBOT_UPPERSTGPUTINTERLOCKONACK);
			else if(m_bRtn[2] == FALSE)		SetError(ALARM_LD_ROBOT_LOWERSTGPUTINTERLOCKONACK);
		
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateUpperShuttleGet::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:		
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		Move_UpperShuttle_LoadPos();
		m_Timer.Start();
		nStep++;
		break;
	case stepEnd:			// Get Pos Check �� End
		m_bRtn[0] = Chk_UpperShuttle_LoadPos();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			nStep = stepIdle;		
		}
		else if(m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_UPPER_SHUTTLE_LOAD_POS);
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateLowerShuttleGet::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			// Robot Put ���°� �ƴҶ� Get Pos Move
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		Move_LowerShuttle_LoadPos();
		m_Timer.Start();
		nStep++;
		break;
	case stepEnd:			// Get Pos Check �� End
		m_bRtn[0] = Chk_LowerShuttle_LoadPos();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			nStep = stepIdle;		
		}
		else if(m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LOWER_SHUTTLE_LOAD_POS);
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateLDRobotUpperPut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	long write;
	switch(nStep)
	{
	case stepStart:			// Robot Cell Check �� Shuttle Cell Check �� �ش� ������ Move
		m_bRtn[0] = Chk_UpperShuttle_LoadPos();
		m_bRtn[1] = CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1);
		m_bRtn[2] = CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2);
		m_bRtn[3] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET);
		m_bRtn[4] = RobotCellVacOnChk(E_LD_ROBOT,VAC_ON);

		// Ch Skip üũ �ϴ� �ǵ� �Ｚ �䱸 ���� ���� ���� ������. 2017/04/26 HSK.
		/*
		if (GetChSkip() == JIG_CH_MAX)
		{
			m_bRtn[1] = CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1);
			m_bRtn[2] = CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2);
		}
		else if(GetChSkip() == JIG_CH_1)
		{
			m_bRtn[1] = CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1);
			m_bRtn[2] = CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1OFF);

			m_bRtn[1] = TRUE;
			m_bRtn[2] = CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2);
		}
		*/
		if (m_bRtn[0] && m_bRtn[3] && m_bRtn[4])
		{
			if (m_bRtn[1] == FALSE)
			{
				m_bCh1Unload = TRUE;
				//2017.11.09, JSJUNG. ���۽������� ä��1 ǲ ��ȣ 
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_UPPERSTGCH1PUT);
			}
			else if (m_bRtn[2] == FALSE)
			{
				m_bCh1Unload = FALSE;
				//2017.11.09, JSJUNG. ���۽������� ä��2 ǲ ��ȣ 
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_UPPERSTGCH2PUT);
			}

			write = 32; // PUTOK2
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 1.)  //2017-11-20,SKCHO, �ð��߰�
		{
			LDRobotBitAllOff(E_PUT_1,FALSE);
			
			//17-21-01,SKCHO, �񱳰� TRUE���� FALSE�� ���� (�˶� ���� ����)
			if (m_bRtn[0] == FALSE)  //if (m_bRtn[0])
			{
				SetError(ALARM_UPPER_SHUTTLE_LOAD_POS);
			}
			else if(m_bRtn[3] == FALSE)  //else if(m_bRtn[3])
			{
				SetError(ALARM_LD_ROBOT_RUNNINGACK);
			}
			else if(m_bRtn[4] == FALSE)//else if(m_bRtn[4])
			{
				SetError(ALARM_LD_ROBOT_VACON);
			}
				
		}
		break;
	case stepPutPermit:		//�κ� Put��ġ �̵�
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_BUSY_Y_PGMSEL6);
		if (m_bRtn[0] == FALSE)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGPUT);		

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutPermit"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			LDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_LD_ROBOT_BUSYACK);
		}
		break;
	case stepPutChk:			// Upper Stage Vac On
		//m_bRtn[1] = m_Timer.Stop(FALSE) > 0.2 ? TRUE : FALSE;
		//if(m_bRtn[1])
		//{
			m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT);		
		   // m_bRtn[2] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT) == FALSE;	
			if (m_bRtn[0] == FALSE)//|| m_bRtn[2])
			{
				//2017.10.31, JSJUNG. �۹� �ñ׳� ����
				write = 0;
				theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);

				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutChk %d"),m_strStateName,m_bRtn[0]);
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQCOMPRTN);
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQERRRTN);
				theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);

				//2017.11.09, JSJUNG. ���۽������� ä��1/2 ǲ ��ȣ ����
				theDeviceMelsec.RobotBitOff(LDROBOT_Y_UPPERSTGCH1PUT);
				theDeviceMelsec.RobotBitOff(LDROBOT_Y_UPPERSTGCH2PUT);


				m_Timer.Start();
				nStep++;
			}
			//2017-10-13,SKCHO
			/*else if(m_Timer.Stop(FALSE) >=1.5 &&  m_Timer.Stop(FALSE) < 2.0)
			{
				theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGPUT);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutPermit -Retry"),m_strStateName);
			}
			*/
			else if (m_Timer.Stop(FALSE) > 5. && m_Timer.Stop(FALSE) < 5.5)
			{
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQSKIP);
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQCOMPRTN);
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQERRRTN);
			}
			else if (m_Timer.Stop(FALSE) > 10.)
			{
				//LDRobotBitAllOff(E_PUT_1,FALSE);
				SetError(ALARM_LD_ROBOT_UPPERSTGPUTINTERLOCKOFFACK);
			}
		//}
		break;
	case stepVacOn:			// Upper Stage Vac On
		
		if (m_bCh1Unload)
			m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGVAC1ON);
		else
			m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGVAC2ON);

		if (IsReturnOk())
		{
			if (m_bCh1Unload)
			{
				UpperShuttle_VacuumOn(VAC_ON,JIG_CH_1);
				CellData_ShuttleFromLDRobot(CELL_POS_UPPER_SHUTTLE_CH1);
			}
			else
			{
				UpperShuttle_VacuumOn(VAC_ON,JIG_CH_2);
				CellData_ShuttleFromLDRobot(CELL_POS_UPPER_SHUTTLE_CH2);
				theProcBank.m_bUpperPutComp = TRUE;
			}
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQERRRTN);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOn"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 2. && m_Timer.Stop(FALSE) < 2.5)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			LDRobotBitAllOff(E_PUT_1,FALSE);
			if (m_bCh1Unload)	SetError(ALARM_LD_ROBOT_UPPERSTGVAC1ONACK);
			else				SetError(ALARM_LD_ROBOT_UPPERSTGVAC2ONACK);
		}
		break;
	case stepRobotVacOff:			// Unloading �� Wait Pos Move	
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACOFF);
		if (m_bCh1Unload)
			m_bRtn[1] = UpperShuttle_VacuumChk(JIG_CH_1,VAC_ON);
		else
			m_bRtn[1] = UpperShuttle_VacuumChk(JIG_CH_2,VAC_ON);

		if (IsReturnOk())
		{
			RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotVacOff"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)					SetError(ALARM_LD_ROBOT_VACOFFACK);
			else	
			{
				LDRobotBitAllOff(E_PUT_1,FALSE);
				if (m_bCh1Unload)
					SetError(ALARM_UPPER_CH1_CELL_VAC);
				else
					SetError(ALARM_UPPER_CH2_CELL_VAC);
			}
		}
		break;
	case stepVacChk:
		m_bRtn[0] =	RobotCellVacOnChk(E_LD_ROBOT,VAC_OFF);
		if (IsReturnOk())
		{
			//RobotBlowOff(E_LD_ROBOT);
			// �������� �̵� �� �̱��� ���� ���ϰ� �˶� �߻� �ϸ� RESTART���� VAC ON �����.
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacChk"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			LDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_LD_ROBOT_VACON);
		}
		break;
	case stepPutCompChk:					// Wait Pos Check �� End
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_UPPERSTGPUTCOMP);
		if (IsReturnOk())
		{
			//2017.11.09, JSJUNG. ��ο� ���� ���
			RobotBlowOff(E_LD_ROBOT);
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQERRRTN);
			m_Timer.Start();
			nStep++;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutCompChk"),m_strStateName);
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			LDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_LD_ROBOT_UPPERSTGPUTCOMPACK);
		}
		break;
	case stepEnd:					// Wait Pos Check �� End
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQERRRTN);
			nStep = stepIdle;		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			
			LDRobotBitAllOff(E_PUT_1,FALSE);
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			LDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_LD_ROBOT_UPPERSTGPUTINTERLOCKONACK);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateLDRobotLowerPut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	long write;
	switch(nStep)
	{
	case stepStart:			// Robot Cell Check �� Shuttle Cell Check �� �ش� ������ Move
		m_bRtn[0] = Chk_UpperShuttle_UnLoadPos();
		m_bRtn[1] = Chk_LowerShuttle_LoadPos();
		m_bRtn[2] = CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1);
		m_bRtn[3] = CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2);
		m_bRtn[4] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET);
		m_bRtn[5] = RobotCellVacOnChk(E_LD_ROBOT,VAC_ON);

		// Ch Skip üũ �ϴ� �ǵ� �Ｚ �䱸 ���� ���� ���� ������. 2017/04/26 HSK.
		/*
		if (GetChSkip() == JIG_CH_MAX)
		{
			m_bRtn[2] = CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1);
			m_bRtn[3] = CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2);
		}
		else if(GetChSkip() == JIG_CH_1)
		{
			m_bRtn[2] = CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1);
			m_bRtn[3] = CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1OFF);

			m_bRtn[2] = TRUE;
			m_bRtn[3] = CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2);
		}
		*/
		if (m_bRtn[0] && m_bRtn[1] && m_bRtn[4] && m_bRtn[5])
		{
			if (m_bRtn[2] == FALSE)
			{
				m_bCh1Unload = TRUE;
				//2017.11.09, JSJUNG. �ο콺������ ä��1 ǲ ��ȣ 
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_LOWERSTGCH1PUT);
			}
			else if(m_bRtn[3] == FALSE)
			{
				m_bCh1Unload = FALSE;
				//2017.11.09, JSJUNG. �ο콺������ ä��2 ǲ ��ȣ 
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_LOWERSTGCH2PUT);
			}

			write = 128; // PUTOK3
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 1.)  //2017-11-20,SKCHO, �ð��߰�
		{
			LDRobotBitAllOff(E_PUT_2,FALSE);
			if (m_bRtn[0] == FALSE)					SetError(ALARM_UPPER_SHUTTLE_UNLOAD_POS);
			else if(m_bRtn[1] == FALSE)				SetError(ALARM_LOWER_SHUTTLE_LOAD_POS);
			else if(m_bRtn[4] == FALSE)				SetError(ALARM_LD_ROBOT_RUNNINGACK);
			else if(m_bRtn[5] == FALSE)				SetError(ALARM_LD_ROBOT_VACON);
		}
		break;
	case stepPutPermit:		//�κ� Put��ġ �̵�
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_BUSY_Y_PGMSEL6);
		if (m_bRtn[0] == FALSE)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGPUT);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutPermit"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			LDRobotBitAllOff(E_PUT_2,FALSE);
			SetError(ALARM_LD_ROBOT_BUSYACK);
		}
		break;
	case stepPutChk:			// Upper Stage Vac On
		//m_bRtn[1] = m_Timer.Stop(FALSE) > 0.2 ? TRUE : FALSE;
		//if(m_bRtn[1])
		//{
			m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT);
		   // m_bRtn[2] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE;	
			if (m_bRtn[0] == FALSE)// || m_bRtn[2])
			//if (m_bRtn[0] == FALSE)
			{		
				//2017.10.31, JSJUNG. �۹� �ñ׳� ����
				write = 0;
				theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);

				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutChk %d"),m_strStateName,m_bRtn[0]);
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQCOMPRTN);
				theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);

				//2017.11.09, JSJUNG. �ο콺������ ä��1/2 ǲ ��ȣ ����
				theDeviceMelsec.RobotBitOff(LDROBOT_Y_LOWERSTGCH1PUT);
				theDeviceMelsec.RobotBitOff(LDROBOT_Y_LOWERSTGCH2PUT);
				
				m_Timer.Start();
				nStep++;
			}
			//2017-10-13,SKCHO
			/*else if(m_Timer.Stop(FALSE) >=1.5 &&  m_Timer.Stop(FALSE) < 2.0)
			{
				theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGPUT);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutPermit -Retry"),m_strStateName);
			}
			*/
			else if (m_Timer.Stop(FALSE) > 5. && m_Timer.Stop(FALSE) < 5.5)
			{
				theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQSKIP);
			}
			else if (m_Timer.Stop(FALSE) > 10.)
			{
				LDRobotBitAllOff(E_PUT_2,FALSE);
				SetError(ALARM_LD_ROBOT_LOWERSTGPUTINTERLOCKOFFACK);
			}
		//}
		break;
	case stepVacOn:			// Upper Stage Vac On
		
		if (m_bCh1Unload)
			m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGVAC1ON);
		else
			m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGVAC2ON);

		if (IsReturnOk())
		{
			if (m_bCh1Unload)
			{
				LowerShuttle_VacuumOn(VAC_ON,JIG_CH_1);
				CellData_ShuttleFromLDRobot(CELL_POS_LOWER_SHUTTLE_CH1);
				theProcBank.m_bLowerPutCompInterlock = TRUE;
			}
			else
			{
				LowerShuttle_VacuumOn(VAC_ON,JIG_CH_2);
				CellData_ShuttleFromLDRobot(CELL_POS_LOWER_SHUTTLE_CH2);
				theProcBank.m_bLowerPutCompInterlock = FALSE; 
				theProcBank.m_bLowerPutComp = TRUE;
			}
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOn"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 2. && m_Timer.Stop(FALSE) < 2.5)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			LDRobotBitAllOff(E_PUT_2,FALSE);
			if (m_bCh1Unload)	SetError(ALARM_LD_ROBOT_LOWERSTGVAC1ONACK);
			else				SetError(ALARM_LD_ROBOT_LOWERSTGVAC2ONACK);
		}
		break;
	case stepRobotVacOff:			// Unloading �� Wait Pos Move	
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACOFF);
		if (m_bCh1Unload)
			m_bRtn[1] = LowerShuttle_VacuumChk(JIG_CH_1,VAC_ON);
		else
			m_bRtn[1] = LowerShuttle_VacuumChk(JIG_CH_2,VAC_ON);

		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);
			RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotVacOff"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 2. && m_Timer.Stop(FALSE) < 2.5)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			LDRobotBitAllOff(E_PUT_2,FALSE);
			if (m_bRtn[0] == FALSE)					SetError(ALARM_LD_ROBOT_VACOFFACK);
			else	
			{
				if (m_bCh1Unload)
					SetError(ALARM_LOWER_CH1_CELL_VAC);
				else
					SetError(ALARM_LOWER_CH2_CELL_VAC);
			}
		}
		break;
	case stepVacChk:
		m_bRtn[0] =	RobotCellVacOnChk(E_LD_ROBOT,VAC_OFF);
		if (IsReturnOk())
		{
			//RobotBlowOff(E_LD_ROBOT);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
			// �������� �̵� �� �̱��� ���� ���ϰ� �˶� �߻� �ϸ� RESTART���� VAC ON �����.
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacChk"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			LDRobotBitAllOff(E_PUT_2,FALSE);
			SetError(ALARM_LD_ROBOT_VACON);
		}
		break;
	case stepPutCompChk:					// Wait Pos Check �� End
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_LOWERSTGPUTCOMP);
		if (IsReturnOk())
		{
			//2017.11.09, JSJUNG. ��ο� ���� ���
			RobotBlowOff(E_LD_ROBOT);
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQCOMPRTN);
			m_Timer.Start();
			nStep++;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutCompChk"),m_strStateName);
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			LDRobotBitAllOff(E_PUT_2,FALSE);
			SetError(ALARM_LD_ROBOT_LOWERSTGPUTCOMPACK);
		}
		break;
	case stepEnd:					// Wait Pos Check �� End
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQCOMPRTN);
			nStep = stepIdle;		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);

			LDRobotBitAllOff(E_PUT_2,FALSE);
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			LDRobotBitAllOff(E_PUT_2,FALSE);
			SetError(ALARM_LD_ROBOT_LOWERSTGPUTINTERLOCKONACK);
		}
		break;
	}


	m_nStep = nStep;

	return nStep;
}

int CStateUpperShuttlePut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			//PA TR Get ���°� �ƴҶ� Put Pos Move
		m_bRtn[0] = Chk_PATR_ZAxis_GetUpperShuttlePos();
		m_bRtn[1] = Chk_PATR_ZAxis_GetLowerShuttlePos();

		if (m_bRtn[0] == FALSE && m_bRtn[1] == FALSE)
		{
			Move_UpperShuttle_UnLoadPos();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else
		{
			if (m_bRtn[0])	SetError(ALARM_PATR_Z_UPPERSHUTTLE_GET_POS);
			else			SetError(ALARM_PATR_Z_LOWERSHUTTLE_GET_POS);
		}
		break;
	case stepEnd:			//Put Pos Check �� End
		m_bRtn[0] = Chk_UpperShuttle_UnLoadPos();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			nStep = stepIdle;		
		}
		else if(m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_UPPER_SHUTTLE_UNLOAD_POS);
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateLowerShuttlePut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			//PA TR Get ���°� �ƴҶ� Put Pos Move
		m_bRtn[0] = Chk_PATR_ZAxis_GetLowerShuttlePos();
		if (m_bRtn[0] == FALSE)
		{
			Move_LowerShuttle_UnLoadPos();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else
		{
			SetError(ALARM_PATR_Z_LOWERSHUTTLE_GET_POS);
		}
		break;
	case stepEnd:			//Put Pos Check �� End
		m_bRtn[0] = Chk_LowerShuttle_UnLoadPos();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			theProcBank.m_bLowerPutCompInterlock = FALSE; 
			nStep = stepIdle;		
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_LOWER_SHUTTLE_UNLOAD_POS);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStatePATRUpperGet::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			// PA TR Y Get ��ġ �̵�
		bExistCh1Cell = CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1);
		bExistCh2Cell = CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2);
		//2017-11-17,SKCHO, LOWERSHUTTLE�� ���� ������ ���� Z_UPPOS CHECK�߰�
		m_bRtn[0] = Chk_PATR_ZAxis_UpPos();
		if (IsReturnOk())
		{
			Move_PATR_YAxis_GetPos();
			//2017-11-17,SKCHO PA TR RETRY ��� �߰�
			//theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			if(theProcBank.m_nPATRUpperGetRetryStep == 0)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart - Retry"), m_strStateName);
			}

			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_PATR_Z_UP_POS);
		}
		break;
	case stepGet:			// Y Get ��ġ Ȯ�� �� Z Get ��ġ �̵� �� Vac On
		m_bRtn[0] = Chk_PATR_YAxis_GetPos();
		if (IsReturnOk())
		{
			Move_PATR_ZAxis_GetUpperShuttlePos();
			if (bExistCh1Cell)
				PATR_Vacuum_On(VAC_ON,JIG_CH_1);
			if (bExistCh2Cell)
				PATR_Vacuum_On(VAC_ON,JIG_CH_2);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGet"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_PATR_Y_GET_POS);
		}
		break;
	case stepGetCheck:		// Z Get ��ġ Ȯ�� �� Shuttle Vac Off
		m_bRtn[0] = Chk_PATR_ZAxis_GetUpperShuttlePos();
		if (IsReturnOk())
		{
			UpperShuttle_VacuumOn(VAC_OFF,JIG_CH_MAX);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGetCheck"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_PATR_Z_UPPERSHUTTLE_GET_POS);
		}
		break;
	case stepZUp:			// Z Up ��ġ �̵�.
		if (bExistCh1Cell)
			m_bRtn[0] = PATR_Vacuum_Chk(JIG_CH_1,VAC_ON);
		else
			m_bRtn[0] = TRUE;

		if (bExistCh2Cell)
			m_bRtn[1] = PATR_Vacuum_Chk(JIG_CH_2,VAC_ON);
		else
			m_bRtn[1] = TRUE;

		m_bRtn[2] = UpperShuttle_VacuumChk(JIG_CH_1,VAC_OFF);
		m_bRtn[3] = UpperShuttle_VacuumChk(JIG_CH_2,VAC_OFF);
		//20170205 byskcho
		//���� ���� �˶� ������ ���� ��ο�  ���� �ð� �߰�
		m_bRtn[4] = (m_Timer.Stop(FALSE) > 0.1) ? TRUE : FALSE; // 2017.2.4 bgkim ���� ������ ���� ������ �߰�

		if (IsReturnOk())
		{
			//20170205 byskcho
		    //��ο�  ���� ��ġ �̵�
			UpperShuttle_BlowOff(JIG_CH_MAX);
			Move_PATR_ZAxis_UpPos(0.5);
			theProcBank.m_bUpperPutComp = FALSE;
			CellData_PATRFromShuttle(CELL_POS_UPPER_SHUTTLE_CH1);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			
			if(m_bRtn[0] == FALSE)					SetError(ALARM_PATR_CH1_VAC);
			else if(m_bRtn[1] == FALSE)				SetError(ALARM_PATR_CH2_VAC);		
			else if(m_bRtn[2] == FALSE)				SetError(ALARM_UPPER_CH1_CELL_VAC);
			else if(m_bRtn[3] == FALSE)				SetError(ALARM_UPPER_CH2_CELL_VAC);
		}
		break;
	case stepEnd:			// Y Put ��ġ �̵�
		bExistCh1Cell = CellTagExist(CELL_POS_PA_TR_CH1);
		bExistCh2Cell = CellTagExist(CELL_POS_PA_TR_CH2);

		m_bRtn[0] = Chk_PATR_ZAxis_UpPos();
		if (bExistCh1Cell)
			m_bRtn[1] = PATR_Vacuum_Chk(JIG_CH_1,VAC_ON);
		else
			m_bRtn[1] = TRUE;

		if (bExistCh2Cell)
			m_bRtn[2] = PATR_Vacuum_Chk(JIG_CH_2,VAC_ON);
		else
			m_bRtn[2] = TRUE;

		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);


			//2017-12-01,SKCHO
			if(theProcBank.m_nPATRUpperGetRetryStep == 1)
			{
				theProductBank.m_PATR_RetryInfo.nSucess++;
			}

			theProcBank.m_nPATRUpperGetRetryStep = 0;
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_PATR_Z_UP_POS);
			else if(m_bRtn[1] == FALSE || m_bRtn[2] == FALSE)
			{
				if(theConfigBank.m_Option.m_bUsePATRRetry == TRUE)
				{
					if(theProcBank.m_nPATRUpperGetRetryStep == 0)
					{
						theProcBank.m_nPATRUpperGetRetryStep =1;
						m_Timer.Start();   
						nStep  =stepStart;

						//2017-12-01,SKCHO
					    theProductBank.m_PATR_RetryInfo.nTotal++;

					}
					else if(theProcBank.m_nPATRUpperGetRetryStep == 1)
					{
						theProcBank.m_nPATRUpperGetRetryStep = 0;
						if(m_bRtn[1] == FALSE)					SetError(ALARM_PATR_CH1_VAC);
						else if(m_bRtn[2] == FALSE)				SetError(ALARM_PATR_CH2_VAC);
					}
				}
				else
				{
					if(m_bRtn[1] == FALSE)					SetError(ALARM_PATR_CH1_VAC);
					else if(m_bRtn[2] == FALSE)				SetError(ALARM_PATR_CH2_VAC);
				}
			}
			//else if(m_bRtn[1] == FALSE)	SetError(ALARM_PATR_CH1_VAC);
			//else if(m_bRtn[2] == FALSE)	SetError(ALARM_PATR_CH2_VAC);
		}
		break;

	}

	m_nStep = nStep;

	return nStep;
}


int CStatePATRLowerGet::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			// PA TR Y Get ��ġ �̵�
		bExistCh1Cell = CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1);
		bExistCh2Cell = CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2);

		m_bRtn[0] = Chk_PATR_ZAxis_UpPos();
		if (IsReturnOk())
		{
			Move_PATR_YAxis_GetPos();
			//2017-11-17,SKCHO PA TR RETRY ��� �߰�
			//theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			if(theProcBank.m_nPATRLowerGetRetryStep == 0)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart - Retry"), m_strStateName);
			}
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_PATR_Z_UP_POS);
		}
		break;
	case stepGet:			// Y Get ��ġ Ȯ�� �� Z Get ��ġ �̵� �� Vac On
		m_bRtn[0] = Chk_PATR_YAxis_GetPos();
		m_bRtn[1] = Chk_UpperShuttle_LoadPos();
		if (IsReturnOk())
		{
			Move_PATR_ZAxis_GetLowerShuttlePos();
			if (bExistCh1Cell)
				PATR_Vacuum_On(VAC_ON,JIG_CH_1);
			if (bExistCh2Cell)
				PATR_Vacuum_On(VAC_ON,JIG_CH_2);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGet"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)	SetError(ALARM_PATR_Y_GET_POS);
			else					SetError(ALARM_UPPER_SHUTTLE_LOAD_POS);
		}
		break;
	case stepGetCheck:		// Z Get ��ġ Ȯ�� �� Shuttle Vac Off
		m_bRtn[0] = Chk_PATR_ZAxis_GetLowerShuttlePos();
		if (IsReturnOk())
		{
			LowerShuttle_VacuumOn(VAC_OFF,JIG_CH_MAX);			

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGetCheck"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_PATR_Z_LOWERSHUTTLE_GET_POS);
		break;
	case stepZUp:			// Z Up ��ġ �̵�.
		if (bExistCh1Cell)
			m_bRtn[0] = PATR_Vacuum_Chk(JIG_CH_1,VAC_ON);
		else
			m_bRtn[0] = TRUE;

		if (bExistCh2Cell)
			m_bRtn[1] = PATR_Vacuum_Chk(JIG_CH_2,VAC_ON);
		else
			m_bRtn[1] = TRUE;

		m_bRtn[2] = LowerShuttle_VacuumChk(JIG_CH_1,VAC_OFF);
		m_bRtn[3] = LowerShuttle_VacuumChk(JIG_CH_2,VAC_OFF);
		//20170205 byskcho
		m_bRtn[4] = (m_Timer.Stop(FALSE) > 0.1)? TRUE:FALSE;

		if (IsReturnOk())
		{
			//20170205 byskcho
			//��ο� �������� ����
			LowerShuttle_BlowOff(JIG_CH_MAX);
			Move_PATR_ZAxis_UpPos(0.5);
			theProcBank.m_bLowerPutComp = FALSE;
			CellData_PATRFromShuttle(CELL_POS_LOWER_SHUTTLE_CH1);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp"),m_strStateName);		

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			//2017-11-17,SKCHO, PA TR RETRY ����߰�
			if(m_bRtn[0] == FALSE)					SetError(ALARM_PATR_CH1_VAC);
			else if(m_bRtn[1] == FALSE)				SetError(ALARM_PATR_CH2_VAC);
			else if(m_bRtn[2] == FALSE)				SetError(ALARM_LOWER_CH1_CELL_VAC);
			else if(m_bRtn[3] == FALSE)				SetError(ALARM_LOWER_CH2_CELL_VAC);
		}
		break;
	case stepEnd:			// Y Put ��ġ �̵�		

		bExistCh1Cell = CellTagExist(CELL_POS_PA_TR_CH1);
		bExistCh2Cell = CellTagExist(CELL_POS_PA_TR_CH2);

		m_bRtn[0] = Chk_PATR_ZAxis_UpPos();
		if (bExistCh1Cell)
			m_bRtn[1] = PATR_Vacuum_Chk(JIG_CH_1,VAC_ON);
		else
			m_bRtn[1] = TRUE;

		if (bExistCh2Cell)
			m_bRtn[2] = PATR_Vacuum_Chk(JIG_CH_2,VAC_ON);
		else
			m_bRtn[2] = TRUE;

		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			//2017-12-01,SKCHO
			if(theProcBank.m_nPATRLowerGetRetryStep == 1)
			{
				theProductBank.m_PATR_RetryInfo.nSucess++;
			}

			theProcBank.m_nPATRLowerGetRetryStep = 0;
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_PATR_Z_UP_POS);
			else if(m_bRtn[1] == FALSE || m_bRtn[2] == FALSE)
			{
				if(theConfigBank.m_Option.m_bUsePATRRetry == TRUE)
				{
					if(theProcBank.m_nPATRLowerGetRetryStep == 0)
					{
						theProcBank.m_nPATRLowerGetRetryStep =1;
						m_Timer.Start();   
						nStep  =stepStart;
						//2017-12-01,SKCHO
					    theProductBank.m_PATR_RetryInfo.nTotal++;

					}
					else if(theProcBank.m_nPATRLowerGetRetryStep == 1)
					{
						theProcBank.m_nPATRLowerGetRetryStep = 0;
						if(m_bRtn[1] == FALSE)					SetError(ALARM_PATR_CH1_VAC);
						else if(m_bRtn[2] == FALSE)				SetError(ALARM_PATR_CH2_VAC);
					}
				}
				else
				{
					if(m_bRtn[1] == FALSE)					SetError(ALARM_PATR_CH1_VAC);
					else if(m_bRtn[2] == FALSE)				SetError(ALARM_PATR_CH2_VAC);
				}
			}
			//else if(m_bRtn[1] == FALSE)	SetError(ALARM_PATR_CH1_VAC);
			//else if(m_bRtn[2] == FALSE)	SetError(ALARM_PATR_CH2_VAC);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStatePAStageGet::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			// PA Stage Cell Check �� Cell ���� �� PA Stage Get Pos �̵� �� PA Stage X,Y,T �˻� ��ġ �̵�.
		PAStageLoadPos();
		PAXPos(JIG_CH_1);
		PAYPos(JIG_CH_1);
		PATPos(JIG_CH_1);
		PAXPos(JIG_CH_2);
		PAYPos(JIG_CH_2);
		PATPos(JIG_CH_2);
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepEnd:			// PA Stage Get ��ġ �� PA Stage X,Y,T �˻���ġ Ȯ�� �� End
		m_bRtn[0] = PAStageLoadPosChk();
		m_bRtn[1] = PAXPosChk(JIG_CH_1);
		m_bRtn[2] = PAYPosChk(JIG_CH_1);
		m_bRtn[3] = PATPosChk(JIG_CH_1);
		m_bRtn[4] = PAXPosChk(JIG_CH_2);
		m_bRtn[5] = PAYPosChk(JIG_CH_2);
		m_bRtn[6] = PATPosChk(JIG_CH_2);
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			nStep = stepIdle;		
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)						SetError(ALARM_PASTAGE_LOAD_POS);
			else if (m_bRtn[1] == FALSE)				SetError(ALARM_PA_CH1_X_LOAD_POS);
			else if (m_bRtn[2] == FALSE)				SetError(ALARM_PA_CH1_Y_LOAD_POS);
			else if (m_bRtn[3] == FALSE)				SetError(ALARM_PA_CH1_T_LOAD_POS);
			else if (m_bRtn[4] == FALSE)				SetError(ALARM_PA_CH2_X_LOAD_POS);
			else if (m_bRtn[5] == FALSE)				SetError(ALARM_PA_CH2_Y_LOAD_POS);
			else if (m_bRtn[6] == FALSE)				SetError(ALARM_PA_CH2_T_LOAD_POS);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStatePATRPut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			//  PA TR Z Up ��ġ Ȯ�� �� Y Put ��ġ �̵�
		bExistCh1Cell = CellTagExist(CELL_POS_PA_TR_CH1);
		bExistCh2Cell = CellTagExist(CELL_POS_PA_TR_CH2);

		m_bRtn[0] = Chk_PATR_ZAxis_UpPos();
		if (bExistCh1Cell)
			m_bRtn[1] = PATR_Vacuum_Chk(JIG_CH_1,VAC_ON);
		else
			m_bRtn[1] = TRUE;

		if (bExistCh2Cell)
			m_bRtn[2] = PATR_Vacuum_Chk(JIG_CH_2,VAC_ON);
		else
			m_bRtn[2] = TRUE;
		
		if (IsReturnOk())
		{
			Move_PATR_YAxis_PutPos();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)				SetError(ALARM_PATR_Z_UP_POS);
			else if (m_bRtn[1] == FALSE)		SetError(ALARM_PATR_CH1_VAC);
			else if (m_bRtn[2] == FALSE)		SetError(ALARM_PATR_CH2_VAC);
		}
		break;
	case stepPut:			//  Y Put ��ġ Ȯ�� �� Z Put ��ġ �̵�
		m_bRtn[0] = Chk_PATR_YAxis_PutPos();
		if (bExistCh1Cell)
			m_bRtn[1] = PATR_Vacuum_Chk(JIG_CH_1,VAC_ON);
		else
			m_bRtn[1] = TRUE;

		if (bExistCh2Cell)
			m_bRtn[2] = PATR_Vacuum_Chk(JIG_CH_2,VAC_ON);
		else
			m_bRtn[2] = TRUE;

		if (IsReturnOk())
		{
			Move_PATR_ZAxis_PutPos();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPut"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)				SetError(ALARM_PATR_Y_PUT_POS);
			else if (m_bRtn[1] == FALSE)		SetError(ALARM_PATR_CH1_VAC);
			else if (m_bRtn[2] == FALSE)		SetError(ALARM_PATR_CH2_VAC);
		}
		break;
	case stepVacOn:		// Z Put ��ġ Ȯ�� �� Vac On
		m_bRtn[0] = Chk_PATR_ZAxis_PutPos();
		if (IsReturnOk())
		{
			PATR_Vacuum_On(VAC_OFF,JIG_CH_MAX);
			if (bExistCh1Cell)
				PAStageVac(VAC_ON,JIG_CH_1);
			if (bExistCh2Cell)
				PAStageVac(VAC_ON,JIG_CH_2);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutCheck"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_PATR_Z_PUT_POS);
		break;
	case stepZUp:		//  Vac Ȯ�� �� Z Up.
		m_bRtn[0] = PATR_Vacuum_Chk(JIG_CH_1,VAC_OFF);
		m_bRtn[1] = PATR_Vacuum_Chk(JIG_CH_2,VAC_OFF);
		if (bExistCh1Cell)
			m_bRtn[2] = PAStageVacChk(JIG_CH_1,VAC_ON);
		else
			m_bRtn[2] = TRUE;

		if (bExistCh2Cell)
			m_bRtn[3] = PAStageVacChk(JIG_CH_2,VAC_ON);
		else
			m_bRtn[3] = TRUE;

		if (IsReturnOk())
		{
			PATR_BlowOff(JIG_CH_MAX);
			Move_PATR_ZAxis_UpPos();
			CellData_PAStageFromPATR();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGetMove"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)			SetError(ALARM_PATR_CH1_VAC);
			else if (m_bRtn[1] == FALSE)	SetError(ALARM_PATR_CH2_VAC);
			else if (m_bRtn[2] == FALSE)	SetError(ALARM_PA_STG_CH1_VAC);
			else if (m_bRtn[3] == FALSE)	SetError(ALARM_PA_STG_CH2_VAC);
		}
		break;
	case stepGet:		//  Z Up Ȯ�� �� Y Get ��ġ �̵�.
		m_bRtn[0] = Chk_PATR_ZAxis_UpPos();
		if (IsReturnOk())
		{
			Move_PATR_YAxis_WaitPos();

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGet"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_PATR_Z_UP_POS);
		break;
	case stepEnd:			// Get��ġ Ȯ�� �� End
		m_bRtn[0] = Chk_PATR_YAxis_WaitPos();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			nStep = stepIdle;
		}	
		else if (m_Timer.Stop(FALSE) > 3.)
			SetError(ALARM_PATR_Y_WAIT_POS);	
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStatePAStage::Run()
{
	
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			// PA ���� ��ġ üũ �� Prealign ��ȣ �ۺ�.
		bExistCh1Cell = CellTagExist(CELL_POS_PA_STAGE_CH1);
		bExistCh2Cell = CellTagExist(CELL_POS_PA_STAGE_CH2);
				
		PAStageAlignPos();
		PAXPos(JIG_CH_1);
		PAXPos(JIG_CH_2);
		PAYPos(JIG_CH_1);
		PAYPos(JIG_CH_2);
		PATPos(JIG_CH_1);
		PATPos(JIG_CH_2);
		Move_PATR_YAxis_WaitPos();
		
		m_Timer.Start();
		nStep++;
		break;
	case stepPAStageAlignMoveChk:
		
		//2017-09-25,SKCHO, ����۽� 2�� ������ �ش�.
	    if(theProcBank.m_nPACH1RetryStep == 1 || theProcBank.m_nPACH2RetryStep == 1)
		{
			m_bRtn[0] = m_bRtn[1] = m_bRtn[2] = m_bRtn[3] = m_bRtn[4] = m_bRtn[5]= m_bRtn[6]= m_bRtn[7]=TRUE;	
			m_bRtn[8] = m_Timer.Stop(FALSE) > 2.0 ? TRUE : FALSE;
		}
		else
		{
			m_bRtn[0] = PAStageAlignPosChk();
			m_bRtn[1] = Chk_PATR_YAxis_WaitPos();
			m_bRtn[2] = PAXPosChk(JIG_CH_1);
			m_bRtn[3] = PAYPosChk(JIG_CH_1);
			m_bRtn[4] = PATPosChk(JIG_CH_1);
			m_bRtn[5] = PAXPosChk(JIG_CH_2);
			m_bRtn[6] = PAYPosChk(JIG_CH_2);
			m_bRtn[7] = PATPosChk(JIG_CH_2);
			m_bRtn[8] = TRUE;
		}
		if (IsReturnOk())
		{			
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			// PA ���� ��ȣ ����.XYT �ѹ��� ������ �ȵǱ� ������ TH ���� ���� �Ѵ�.
			PA_SendAlignStart(TRUE);
			m_Timer.Start();
			nStep++;			
		}
		else if(m_Timer.Stop(FALSE) > 6.)
		{
			if (m_bRtn[0] == FALSE)					SetError(ALARM_PASTAGE_ALIGN_POS);
			else if (m_bRtn[1] == FALSE)			SetError(ALARM_PATR_Y_WAIT_POS);
			else if (m_bRtn[2] == FALSE)			SetError(ALARM_PA_CH1_X_LOAD_POS);
			else if (m_bRtn[3] == FALSE)			SetError(ALARM_PA_CH1_Y_LOAD_POS);
			else if (m_bRtn[4] == FALSE)			SetError(ALARM_PA_CH1_T_LOAD_POS);
			else if (m_bRtn[5] == FALSE)			SetError(ALARM_PA_CH2_X_LOAD_POS);
			else if (m_bRtn[6] == FALSE)			SetError(ALARM_PA_CH2_Y_LOAD_POS);
			else if (m_bRtn[7] == FALSE)			SetError(ALARM_PA_CH2_T_LOAD_POS);

		}
		break;
	case stepThAlignChk:	// Th Prealign ���� ���� Ȯ�� �� PA TH Move.
		//m_bRtn[0] = (theProcBank.m_nPA_MITAlignResult[JIG_CH_1] >= ALIGN_RESULT_GOOD) ? TRUE:FALSE;
		//m_bRtn[1] = (theProcBank.m_nPA_MITAlignResult[JIG_CH_2] >= ALIGN_RESULT_GOOD) ? TRUE:FALSE;
		//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶�
		if (CellTagExist(CELL_POS_PA_STAGE_CH1))
			m_bRtn[0] = (theProcBank.m_nPA_MITAlignResult[JIG_CH_1] == ALIGN_RESULT_GOOD) ? TRUE:FALSE;
		else
			m_bRtn[0] = TRUE;

		if (CellTagExist(CELL_POS_PA_STAGE_CH2))
			m_bRtn[1] = (theProcBank.m_nPA_MITAlignResult[JIG_CH_2] == ALIGN_RESULT_GOOD) ? TRUE:FALSE;
		else
			m_bRtn[1] = TRUE;
		
		if(IsReturnOk())
		{
			// Ch1 Cam
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Theta Ch1 Result = %d"),m_strStateName,theProcBank.m_nPA_MITAlignResult[JIG_CH_1]);
			if(theProcBank.m_nPA_MITAlignResult[JIG_CH_1] == ALIGN_RESULT_GOOD)
			{
				// ���� ���� ��� TH Result CellData�� ����.
				CellData_SetPreAlignStateT(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignTH[JIG_CH_1]);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepThAlignChk Prealign Th1 Success :%f"),m_strStateName,theProcBank.m_dPA_MITAlignTH[JIG_CH_1]);
			    //2017-12-01,SKCHO
				if(theProcBank.m_nPACH1RetryStep == 1)
				{
					theProductBank.m_PASTAGE_RetryInfo.nSucess++;
				}
				theProcBank.m_nPACH1RetryStep = 0;					
			}
			//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶� : ����
			//else
			//{
			//	if(theProcBank.m_nPA_MITAlignResult[JIG_CH_1] >= ALIGN_RESULT_FAIL)  //2017-07-20,SKCHO, ALIGN_RESULT_SKIP �϶� ���� ���� �߻�
			//	//if(theProcBank.m_nPA_MITAlignResult[JIG_CH_1] == ALIGN_RESULT_FAIL)
			//	{
			//		// ���� ���� ��� ��� ����ġ 0���� Result Celldata�� ����.
			//		theProcBank.m_dPA_MITAlignTH[JIG_CH_1] = theProcBank.m_dPA_MITAlignX[JIG_CH_1] = theProcBank.m_dPA_MITAlignY[JIG_CH_1] = 0.;
			//		CellData_SetPreAlignStateT(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignTH[JIG_CH_1]);
			//		CellData_SetPreAlignStateXY(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignX[JIG_CH_1], theProcBank.m_dPA_MITAlignY[JIG_CH_1],
			//			theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_RIGHT]);
			//		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepThAlignChk Prealign Th1 Fail"),m_strStateName);
			//	}
			//}

			// Ch2 Cam
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Theta Ch2 Result = %d"),m_strStateName,theProcBank.m_nPA_MITAlignResult[JIG_CH_2]);
			if(theProcBank.m_nPA_MITAlignResult[JIG_CH_2] == ALIGN_RESULT_GOOD)
			{
				// ���� ���� ��� TH Result CellData�� ����.
				CellData_SetPreAlignStateT(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignTH[JIG_CH_2]);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepThAlignChk Prealign Th2 Success :%f"),m_strStateName,theProcBank.m_dPA_MITAlignTH[JIG_CH_2]);
				//2017-12-01,SKCHO
				if(theProcBank.m_nPACH2RetryStep == 1)
				{
					theProductBank.m_PASTAGE_RetryInfo.nSucess++;
				}
				theProcBank.m_nPACH2RetryStep = 0;
			}
			//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶� : ����
			//else
			//{
			//	if(theProcBank.m_nPA_MITAlignResult[JIG_CH_2] >= ALIGN_RESULT_FAIL)  //2017-07-20,SKCHO, ALIGN_RESULT_SKIP �϶� ���� ���� �߻�
			//	//if(theProcBank.m_nPA_MITAlignResult[JIG_CH_2] == ALIGN_RESULT_FAIL)
			//	{
			//		// ���� ���� ��� ��� ����ġ 0���� Result Celldata�� ����.
			//		theProcBank.m_dPA_MITAlignTH[JIG_CH_2] = theProcBank.m_dPA_MITAlignX[JIG_CH_2] = theProcBank.m_dPA_MITAlignY[JIG_CH_2] = 0.;
			//		
			//		CellData_SetPreAlignStateT(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignTH[JIG_CH_2]);
			//		CellData_SetPreAlignStateXY(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignX[JIG_CH_2], theProcBank.m_dPA_MITAlignY[JIG_CH_2],
			//		
			//		theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_RIGHT]);
			//		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepThAlignChk Prealign Th2 Fail"),m_strStateName);
			//	}
			//}

			// Move
			if (CellTagExist(CELL_POS_PA_STAGE_CH1))
				PATPos(JIG_CH_1, CellData_GetPreAlginT(JIG_CH_1));
			if (CellTagExist(CELL_POS_PA_STAGE_CH2))
				PATPos(JIG_CH_2, CellData_GetPreAlginT(JIG_CH_2));

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.) 
		{
			//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶� : ��� �κ� �� ���� �κ��� ���нÿ��� ����.�ʱ�ȭ!@?
			if (m_bRtn[0] == FALSE)
			{
				//2017-09-16,SKCHO
				//pre align ���н� 1ȸ ��õ�
				if(theProcBank.m_nPACH1RetryStep == 0)
				{
					theProcBank.m_nPACH1RetryStep = 1;
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH1 Mark Search Retry"),m_strStateName);	
					nStep = stepPAStageAlignMoveChk;

					//2017-12-01,SKCHO
					theProductBank.m_PASTAGE_RetryInfo.nTotal++;

				}
				else if(theProcBank.m_nPACH1RetryStep == 1)
				{
					theProcBank.m_nPACH1RetryStep = 0;
					theProcBank.m_nPACH2RetryStep = 0;

					theProcBank.m_nPA_MITAlignResult[JIG_CH_1] = ALIGN_RESULT_FAIL;
					theProcBank.m_dPA_MITAlignTH[JIG_CH_1] = theProcBank.m_dPA_MITAlignX[JIG_CH_1] = theProcBank.m_dPA_MITAlignY[JIG_CH_1] =0.;
					CellData_SetPreAlignStateT(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignTH[JIG_CH_1]);
					CellData_SetPreAlignStateXY(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignX[JIG_CH_1], theProcBank.m_dPA_MITAlignY[JIG_CH_1],
						theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_RIGHT]);
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepThAlignChk Prealign Th1 Fail"),m_strStateName);
					SetError(ALARM_PA_CH1_T_ALIGN);
				}
			}
			else if (m_bRtn[1] == FALSE)
			{
				//2017-09-16,SKCHO
				//pre align ���н� 1ȸ ��õ�
				if(theProcBank.m_nPACH2RetryStep == 0)
				{
					theProcBank.m_nPACH2RetryStep = 1;
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH2 Mark Search Retry"),m_strStateName);
					nStep = stepPAStageAlignMoveChk;

					//2017-12-01,SKCHO
					theProductBank.m_PASTAGE_RetryInfo.nTotal++;
				}
				else if(theProcBank.m_nPACH2RetryStep == 1)
				{
					theProcBank.m_nPACH1RetryStep = 0;
					theProcBank.m_nPACH2RetryStep = 0;

					theProcBank.m_nPA_MITAlignResult[JIG_CH_2] = ALIGN_RESULT_FAIL;
					theProcBank.m_dPA_MITAlignTH[JIG_CH_2] = theProcBank.m_dPA_MITAlignX[JIG_CH_2] = theProcBank.m_dPA_MITAlignY[JIG_CH_2] =0.;
					CellData_SetPreAlignStateT(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignTH[JIG_CH_2]);
					CellData_SetPreAlignStateXY(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignX[JIG_CH_2], theProcBank.m_dPA_MITAlignY[JIG_CH_2],
						theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_RIGHT]);
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepThAlignChk Prealign Th2 Fail"),m_strStateName);
					SetError(ALARM_PA_CH2_T_ALIGN);
				}
			}
		
			//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶� : ����
			//// �ð��ʰ��� Align Data ���޾��� ���.
			//if(theProcBank.m_nPA_MITAlignResult[JIG_CH_1] == ALIGN_RESULT_NONE)
			//{
			//	theProcBank.m_nPA_MITAlignResult[JIG_CH_1] = ALIGN_RESULT_FAIL;
			//	theProcBank.m_dPA_MITAlignTH[JIG_CH_1] = theProcBank.m_dPA_MITAlignX[JIG_CH_1] = theProcBank.m_dPA_MITAlignY[JIG_CH_1] =0.;
			//	CellData_SetPreAlignStateT(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignTH[JIG_CH_1]);
			//	CellData_SetPreAlignStateXY(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignX[JIG_CH_1], theProcBank.m_dPA_MITAlignY[JIG_CH_1],
			//		theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_RIGHT]);
			//	theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepThAlignChk Prealign Th1 Time out"),m_strStateName);
			//}
			//if(theProcBank.m_nPA_MITAlignResult[JIG_CH_2] == ALIGN_RESULT_NONE)
			//{
			//	theProcBank.m_nPA_MITAlignResult[JIG_CH_2] = ALIGN_RESULT_FAIL;
			//	theProcBank.m_dPA_MITAlignTH[JIG_CH_2] = theProcBank.m_dPA_MITAlignX[JIG_CH_2] = theProcBank.m_dPA_MITAlignY[JIG_CH_2] =0.;
			//	CellData_SetPreAlignStateT(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignTH[JIG_CH_2]);
			//	CellData_SetPreAlignStateXY(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignX[JIG_CH_2], theProcBank.m_dPA_MITAlignY[JIG_CH_2],
			//		theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_RIGHT]);
			//	theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepThAlignChk Prealign Th2 Time out"),m_strStateName);
			//}

			//PATPos(JIG_CH_1, CellData_GetPreAlginT(JIG_CH_1));
			//PATPos(JIG_CH_2, CellData_GetPreAlginT(JIG_CH_2));
			//m_Timer.Start();
			//nStep++;
		}
		break;
	case stepThMoveChk:		// PA Th Move Ȯ�� �� XY Prealign ��ȣ �ۺ�.
		//20170730 JSJUNG, �� ���� üũ
		if (CellTagExist(CELL_POS_PA_STAGE_CH1))
			m_bRtn[0] = PATPosChk(JIG_CH_1, CellData_GetPreAlginT(JIG_CH_1));
		else
			m_bRtn[0] = TRUE;

		if (CellTagExist(CELL_POS_PA_STAGE_CH2))
			m_bRtn[1] = PATPosChk(JIG_CH_2, CellData_GetPreAlginT(JIG_CH_2));
		else
			m_bRtn[1] = TRUE;

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepThMoveChk"),m_strStateName);
			// TH ���� Ȯ�� �� XY Align ���� ��û �Ѵ�.
			PA_SendAlignStart(FALSE);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_PA_CH1_T_UNLOAD_POS);
			else						SetError(ALARM_PA_CH2_T_UNLOAD_POS);
		}
		break;
	case stepPAMove:		// XY Prealign ���� ���� Ȯ�� �� PA Stage X,Y, Stage Move.
		//m_bRtn[0] = (theProcBank.m_nPA_MITAlignResult[JIG_CH_1] >= ALIGN_RESULT_GOOD) ? TRUE:FALSE;
		//m_bRtn[1] = (theProcBank.m_nPA_MITAlignResult[JIG_CH_2] >= ALIGN_RESULT_GOOD) ? TRUE:FALSE;
		//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶�
		if (CellTagExist(CELL_POS_PA_STAGE_CH1))
			m_bRtn[0] = (theProcBank.m_nPA_MITAlignResult[JIG_CH_1] == ALIGN_RESULT_GOOD) ? TRUE:FALSE;
		else
			m_bRtn[0] = TRUE;
		if (CellTagExist(CELL_POS_PA_STAGE_CH2))
			m_bRtn[1] = (theProcBank.m_nPA_MITAlignResult[JIG_CH_2] == ALIGN_RESULT_GOOD) ? TRUE:FALSE;
		else
			m_bRtn[1] = TRUE;
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] XY A1: Result = %d"),m_strStateName,theProcBank.m_nPA_MITAlignResult[JIG_CH_1]);
			if(theProcBank.m_nPA_MITAlignResult[JIG_CH_1] == ALIGN_RESULT_GOOD)
			{
				// XY ������ Cell Data ����.
				CellData_SetPreAlignStateXY(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignX[JIG_CH_1], theProcBank.m_dPA_MITAlignY[JIG_CH_1],
					theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_RIGHT]);

				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPAMove Ch1 Prealign XY Success X:%f Y:%f"),m_strStateName, theProcBank.m_dPA_MITAlignX[JIG_CH_1], theProcBank.m_dPA_MITAlignY[JIG_CH_1]);
			}
			//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶� : ����
			//else
			//{
			//	if(theProcBank.m_nPA_MITAlignResult[JIG_CH_1] >= ALIGN_RESULT_FAIL)   //2017-07-20,SKCHO, ALIGN_RESULT_SKIP �϶� ���� ���� �߻�
			//	//if(theProcBank.m_nPA_MITAlignResult[JIG_CH_1] == ALIGN_RESULT_FAIL)
			//	{
			//		theProcBank.m_dPA_MITAlignX[JIG_CH_1] = theProcBank.m_dPA_MITAlignY[JIG_CH_1] = 0;

			//		CellData_SetPreAlignStateXY(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignX[JIG_CH_1], theProcBank.m_dPA_MITAlignY[JIG_CH_1],
			//			theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_RIGHT]);

			//		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPAMove Ch1 Prealign XY Fail"),m_strStateName);
			//	}
			//}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] XY A2: Result = %d"),m_strStateName,theProcBank.m_nPA_MITAlignResult[JIG_CH_2]);
			if(theProcBank.m_nPA_MITAlignResult[JIG_CH_2] == ALIGN_RESULT_GOOD)
			{
				// XY ������ Cell Data ����.
				CellData_SetPreAlignStateXY(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignX[JIG_CH_2], theProcBank.m_dPA_MITAlignY[JIG_CH_2],
					theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_RIGHT]);

				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPAMove Ch2 Prealign XY Success X:%f Y:%f"),m_strStateName, theProcBank.m_dPA_MITAlignX[JIG_CH_2], theProcBank.m_dPA_MITAlignY[JIG_CH_2]);
			}
			//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶� : ����
			//else
			//{
			//	if(theProcBank.m_nPA_MITAlignResult[JIG_CH_2] >= ALIGN_RESULT_FAIL)  //2017-07-20,SKCHO, ALIGN_RESULT_SKIP �϶� ���� ���� �߻�
			//	//if(theProcBank.m_nPA_MITAlignResult[JIG_CH_2] == ALIGN_RESULT_FAIL)
			//	{
			//		theProcBank.m_dPA_MITAlignX[JIG_CH_2] = theProcBank.m_dPA_MITAlignY[JIG_CH_2] = 0;

			//		CellData_SetPreAlignStateXY(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignX[JIG_CH_2], theProcBank.m_dPA_MITAlignY[JIG_CH_2],
			//			theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_RIGHT]);

			//		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPAMove Ch2 Prealign XY Fail"),m_strStateName);
			//	}
			//}

			// Move
			//20170730 JSJUNG, �� ���� üũ
			if (CellTagExist(CELL_POS_PA_STAGE_CH1))
			{
				PAXPos(JIG_CH_1, CellData_GetPreAlginX(JIG_CH_1));
				PAYPos(JIG_CH_1, CellData_GetPreAlginY(JIG_CH_1));
			}
			if (CellTagExist(CELL_POS_PA_STAGE_CH2))
			{
				PAXPos(JIG_CH_2, CellData_GetPreAlginX(JIG_CH_2));
				PAYPos(JIG_CH_2, CellData_GetPreAlginY(JIG_CH_2));
			}
			// Get��ġ�� �����°� �ٸ�����... 17/04/22
//			Move_PATR_YAxis_GetPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.) 
		{
			//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶� : ��� �κ� �� ���� �κ��� ���нÿ��� ����.�ʱ�ȭ!@?
			if (m_bRtn[0] == FALSE)
			{
				theProcBank.m_nPA_MITAlignResult[JIG_CH_1] = ALIGN_RESULT_FAIL;
				theProcBank.m_dPA_MITAlignX[JIG_CH_1] = theProcBank.m_dPA_MITAlignY[JIG_CH_1] = 0;

				CellData_SetPreAlignStateXY(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignX[JIG_CH_1], theProcBank.m_dPA_MITAlignY[JIG_CH_1],
					theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_RIGHT]);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPAMove Ch1 Prealign XY Fail"),m_strStateName);
				SetError(ALARM_PA_CH1_XY_ALIGN);
			}
			else if (m_bRtn[1] == FALSE)
			{
				theProcBank.m_nPA_MITAlignResult[JIG_CH_2] = ALIGN_RESULT_FAIL;
				theProcBank.m_dPA_MITAlignX[JIG_CH_2] = theProcBank.m_dPA_MITAlignY[JIG_CH_2] = 0;

				CellData_SetPreAlignStateXY(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignX[JIG_CH_2], theProcBank.m_dPA_MITAlignY[JIG_CH_2],
					theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_RIGHT]);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPAMove Ch2 Prealign XY Fail"),m_strStateName);
				SetError(ALARM_PA_CH2_XY_ALIGN);
			}
			//20170730 JSJUNG, �� ���� üũ �ϰ� GOOD�϶��� ������ PA ALIGN ���н� �˶� : ����
			// Align Data �޴� �ð� �ʰ�
			//if(theProcBank.m_nPA_MITAlignResult[JIG_CH_1] == ALIGN_RESULT_NONE)
			//{
			//	theProcBank.m_nPA_MITAlignResult[JIG_CH_1] = ALIGN_RESULT_FAIL;
			//	theProcBank.m_dPA_MITAlignX[JIG_CH_1] = theProcBank.m_dPA_MITAlignY[JIG_CH_1] = 0;

			//	CellData_SetPreAlignStateXY(JIG_CH_1, theProcBank.m_nPA_MITAlignResult[JIG_CH_1], theProcBank.m_dPA_MITAlignX[JIG_CH_1], theProcBank.m_dPA_MITAlignY[JIG_CH_1],
			//		theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_1][E_MARK_RIGHT]);
			//	theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPAMove Ch1 Prealign XY Time Out"),m_strStateName);
			//}
			//if(theProcBank.m_nPA_MITAlignResult[JIG_CH_2] == ALIGN_RESULT_NONE)
			//{
			//	theProcBank.m_nPA_MITAlignResult[JIG_CH_2] = ALIGN_RESULT_FAIL;
			//	theProcBank.m_dPA_MITAlignX[JIG_CH_2] = theProcBank.m_dPA_MITAlignY[JIG_CH_2] = 0;

			//	CellData_SetPreAlignStateXY(JIG_CH_2, theProcBank.m_nPA_MITAlignResult[JIG_CH_2], theProcBank.m_dPA_MITAlignX[JIG_CH_2], theProcBank.m_dPA_MITAlignY[JIG_CH_2],
			//		theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_LEFT], theProcBank.m_dPA_MITAlignScore[JIG_CH_2][E_MARK_RIGHT]);
			//	theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPAMove Ch2 Prealign XY Time Out"),m_strStateName);
			//}

			//PAXPos(JIG_CH_1, CellData_GetPreAlginX(JIG_CH_1));
			//PAYPos(JIG_CH_1, CellData_GetPreAlginY(JIG_CH_1));
			//PAXPos(JIG_CH_2, CellData_GetPreAlginX(JIG_CH_2));
			//PAYPos(JIG_CH_2, CellData_GetPreAlginY(JIG_CH_2));
			//m_Timer.Start();
			//nStep++;
		}
		break;
	case stepEnd:			// PA X,Y, Stage Move Ȯ�� �� End
		if (CellTagExist(CELL_POS_PA_STAGE_CH1))
		{
			m_bRtn[0] = PAXPosChk(JIG_CH_1, CellData_GetPreAlginX(JIG_CH_1));
			m_bRtn[1] = PAYPosChk(JIG_CH_1, CellData_GetPreAlginY(JIG_CH_1));
		}
		else
		{
			m_bRtn[0] = TRUE;
			m_bRtn[1] = TRUE;
		}

		if (CellTagExist(CELL_POS_PA_STAGE_CH2))
		{
			m_bRtn[2] = PAXPosChk(JIG_CH_2, CellData_GetPreAlginX(JIG_CH_2));
			m_bRtn[3] = PAYPosChk(JIG_CH_2, CellData_GetPreAlginY(JIG_CH_2));
		}
		else
		{
			m_bRtn[2] = TRUE;
			m_bRtn[3] = TRUE;
		}
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);

			theProcBank.m_nPACH1RetryStep = 0;
			theProcBank.m_nPACH2RetryStep = 0;

			//2017-12-12,SKCHO , ��������� ������  GET POSITION���� �̵�
			if(theUnitFunc.CellTagExist(CELL_POS_PA_TR_CH1) == FALSE && theUnitFunc.CellTagExist(CELL_POS_PA_TR_CH2) == FALSE)
			{			
				Move_PATR_YAxis_GetPos();
			}

			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)			SetError(ALARM_PA_CH1_X_UNLOAD_POS);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_PA_CH1_Y_UNLOAD_POS);
			else if(m_bRtn[2] == FALSE)		SetError(ALARM_PA_CH2_X_UNLOAD_POS);
			else if(m_bRtn[3] == FALSE)		SetError(ALARM_PA_CH2_Y_UNLOAD_POS);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStatePAStagePut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			// PA Stage Cell Check �� Cell ���� �� Put Pos �̵�
		PAStageUnloadPos();
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepEnd:			// PA Stage Put ��ġ Ȯ�� �� End
		m_bRtn[0] = PAStageUnloadPosChk();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			nStep = stepIdle;		
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_PASTAGE_UNLOAD_POS);
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateLDTRGet::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			// LD TR X PA Cell Load��ġ�� �̵�.
		bExistCh1Cell = CellTagExist(CELL_POS_PA_STAGE_CH1);
		bExistCh2Cell = CellTagExist(CELL_POS_PA_STAGE_CH2);

		m_bRtn[0] = LDTRZUpPosChk();
		if (IsReturnOk())
		{		
			LDTRXGetPos();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}	
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_LD_TR_Z_UP_POS);
		}
		break;
	case stepZDown:			// LD TR X ��ġ Ȯ�� ��  Z Down �� 
		m_bRtn[0] = LDTRXGetPosChk();
		if (IsReturnOk())
		{
			//2018-01-18,JSJUNG, LDTR X GET �������� ��ġ �α� �߰�.
			double dValue = 0;
			theDeviceMotion.GetPosition(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_LD_TR_X, dValue);
			theLog[LOG_TEACH_MOVE].AddBuf( _T("LDTR X GET : %f"),dValue);

			LDTRZGetPos();

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}	
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_LD_TR_X_GET_POS);
		}
		break;
	case stepVacOn:		// LD TR Vac On �� PA Stage Vac Off.
		m_bRtn[0] = LDTRZGetPosChk();
		if (IsReturnOk())
		{
			//2018-01-18,JSJUNG, LDTR Z GET �������� ��ġ �α� �߰�.
			double dValue = 0;
			theDeviceMotion.GetPosition(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_LD_TR_Z, dValue);		///	������ġ ǥ��
			theLog[LOG_TEACH_MOVE].AddBuf( _T("LDTR Z GET : %f"),dValue);

			PAStageVac(VAC_OFF,JIG_CH_MAX);

			if (bExistCh1Cell)	LDTRVacOn(VAC_ON,JIG_CH_1);
			if (bExistCh2Cell)	LDTRVacOn(VAC_ON,JIG_CH_2);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOn"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_LD_TR_Z_GET_POS);
		}
		break;
	case stepZUp:		// LD TR �� PA Stage Vac Ȯ�� �� Z Up
		m_bRtn[0] = PAStageVacChk(JIG_CH_1,VAC_OFF);
		m_bRtn[1] = PAStageVacChk(JIG_CH_2,VAC_OFF);
		if (bExistCh1Cell)	m_bRtn[2] = LDTRCellVacChk(JIG_CH_1,VAC_ON);
		else				m_bRtn[2] = TRUE;
		if (bExistCh2Cell)	m_bRtn[3] = LDTRCellVacChk(JIG_CH_2,VAC_ON);
		else				m_bRtn[3] = TRUE;

		m_bRtn[4] = (m_Timer.Stop(FALSE) > 0.2) ? TRUE : FALSE;

		if (IsReturnOk())
		{
			//20170205 byskcho
			// ��ο� ������ ���� ����
			//PAStage_BlowOff(JIG_CH_MAX);
			LDTRZUpPos(0.5);
			CellData_LDTRFromPAStage();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)			SetError(ALARM_PA_STG_CH1_VAC);
			else if (m_bRtn[1] == FALSE)	SetError(ALARM_PA_STG_CH2_VAC);
			else if (m_bRtn[2] == FALSE)	SetError(ALARM_LD_TR_CH1_VAC);
			else if (m_bRtn[3] == FALSE)	SetError(ALARM_LD_TR_CH2_VAC);
		}
		break;
	case stepEnd:		// LD TR Z Up Pos Ȯ�� �� End
		bExistCh1Cell = CellTagExist(CELL_POS_LD_TR_CH1);
		bExistCh2Cell = CellTagExist(CELL_POS_LD_TR_CH2);

		m_bRtn[0] = LDTRZUpPosChk();
		if (bExistCh1Cell)	m_bRtn[1] = LDTRCellVacChk(JIG_CH_1,VAC_ON);
		else				m_bRtn[1] = TRUE;
		if (bExistCh2Cell)	m_bRtn[2] = LDTRCellVacChk(JIG_CH_2,VAC_ON);
		else				m_bRtn[2] = TRUE;

		if (IsReturnOk())
		{
			//20170205 byskcho
			//�� ���������� �̵��� ������ ��ο� �������� �߰�
			PAStage_BlowOff(JIG_CH_MAX);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			nStep = stepIdle;	
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)	SetError(ALARM_LD_TR_Z_UP_POS);
			else if (m_bRtn[1] == FALSE)	SetError(ALARM_LD_TR_CH1_VAC);
			else if (m_bRtn[2] == FALSE)	SetError(ALARM_LD_TR_CH2_VAC);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


int CStateLowerMCR::Run()
{
	CCellInfo* pCell;
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:
		// �ش� ���������� Error �߻��� �ι� �д� ��� �߻� ���� �ʵ��� �о����� Ȯ�� �Ѵ�.
		bCh1ReadingChk = CellData_UVReadingChk(LOWER_MCR,JIG_CH_1);
		bCh2ReadingChk = CellData_UVReadingChk(LOWER_MCR,JIG_CH_2);

		LDTR_Z_LOWERMCRPos();
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZMoveChk"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepZMoveChk:			 //Z Lower MCR ��ġ Ȯ�� �� Cell ���� Ȯ��.
		m_bRtn[0] = LDTR_Z_LOWERMCRPosChk();
		if (IsReturnOk())
		{
			//2017-09-17,SKCHO, PUT��ġ�� �̵�
			if (!theConfigBank.m_Option.m_bUseLowerMCRRead)
			{
				nStep = stepXPutPos;
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] LowerSkip MoveXPutPos"),m_strStateName);
			}
			else
			{			
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZMoveChk"),m_strStateName);
				if (bCh1ReadingChk == MCR_RESULT_NONE)
					nStep = stepCh1Move;
				else if (bCh2ReadingChk == MCR_RESULT_NONE)
					nStep = stepCh2Move;
				else
					nStep = stepZUpPos;
			}
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_Z_LOWERMCR_POS);
		break;
	case stepCh1Move:	// X Lower MCR Ch1 ��ġ�� �̵�.
		LDTR_X_LOWERMCRPos(JIG_CH_1,0.5);
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1Move"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepCh1Stable:		
		m_bRtn[0] = LDTR_X_LOWERMCRPosChk(JIG_CH_1);
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1Stable"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_X_CH1_LOWERMCR_POS);
		break;
	case stepCh1Reading:		// LD TR X Ch1 MCR Pos Chk �� MCR Reading.
		if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dMCRDelayTime)
		{
			Trigger(LOWER_MCR,JIG_CH_1);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1Reading"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh1ReadingChk:		// MCR Reading Chk
		//m_bRtn[0] = (MCR_GetLastID(JIG_CH_1).GetLength() > 1) ? TRUE:FALSE;
		//2018-04-18,NAM, Getlength for "Fail" is 4
		m_bRtn[0] = (MCR_GetLastID(JIG_CH_1).GetLength() > 4) ? TRUE:FALSE;
		if (IsReturnOk())
		{
			// MCR���� ��� ����.
			CellData_SetUVResult(LOWER_MCR,JIG_CH_1,MCR_RESULT_GOOD);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1ReadingChk %s"),m_strStateName,MCR_GetLastID(JIG_CH_1));
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Lower MCR Ch1 MCR Reading Success"),m_strStateName);
			//2018-07-25,SKCHO,
			m_Timer.Start();
			nStep++;
			/*if (bCh2ReadingChk == MCR_RESULT_NONE)
				nStep = stepCh2Move;
			else
				nStep = stepZUpPos;
				*/
		}
		else if (m_Timer.Stop(FALSE) > 1.5)
		{
			// MCR���� ��� ����.
			CellData_SetUVResult(LOWER_MCR,JIG_CH_1,MCR_RESULT_FAIL);

			//2017-04-14,skcho , ���α׷� �ٿ� ���� �־� �ӽ� ����
			SaveMCRNGImage(LOWER_MCR,JIG_CH_1);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Lower MCR Ch1 MCR Reading Fail"),m_strStateName);
			if (bCh2ReadingChk == MCR_RESULT_NONE)
				nStep = stepCh2Move;
			else
				nStep = stepZUpPos;
		}
		break;
		case stepCh1CellRequest:

			 //2018-07-25,SKCHO, CELL LOADING �� PRODUCT_ID ���� �ֱ� ���ؼ� CELL INFORMATION REQUEST�� MCR READING �������� �̵�
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_LD_TR_CH1));
		    if(pCell)
			{		
				if( pCell->m_strCellID != _T("FAIL") )
				{
					//2018-07-25,SKCHO, CellInforRequest �Լ� ����
					//theUnitFunc.CIM_CH1_CellInfoRequest();
					theUnitFunc.CIM_CellInfoRequest(CELL_POS_LD_TR_CH1);
					if(theProcBank.m_nCH1CellInfoRetryStep == 0)
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellInfoRequest"),m_strStateName);
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellInfoRequest - Retry"),m_strStateName);
					}
					pCell->bCellInfoRequest = TRUE;
					pCell->m_strCellInfoResult = _T("-1");//REQUEST �ʱⰪ			
				}
				else
				{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH1 MCR_FAIL"),m_strStateName);
					theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH1,TRUE);  //2017-04-08,SKCHO, MCR READING ���н� �˻� ��ŵ				
				}
			}
		
			m_Timer.Start();
			nStep++;
		break;
	//2018-07-25,SKCHO
	case stepCh1CellRequestChk:
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_LD_TR_CH1));
			if(pCell)
			{
				if(pCell->bCellInfoRequest)
				{			
					int nRequest = theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_LD_TR_CH1);				

					if(nRequest == 1)  //�������� ������
					{
							m_Timer.Start();

							if (bCh2ReadingChk == MCR_RESULT_NONE)
								nStep = stepCh2Move;
							else
								nStep = stepZUpPos;
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellRequestChk -OK"),m_strStateName);
					}
					else if( nRequest== 0)
					{
		
						//Ÿ�� �ƿ��̰� �����н�  false �̸� �˶�
						if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime && !theConfigBank.m_Option.m_bUseCh1ReqByPass)
						{
							//2017-09-24,SKCHO,Ÿ�Ӿƿ��� �ѹ��� ��û�ϰ� �ι�° Ÿ�Ӿƿ������� ������ �ִ��� Ȯ���Ѵ�.
							if(theProcBank.m_nCH1CellInfoRetryStep == 0)
							{
								theProcBank.m_nCH1CellInfoRetryStep = 1;
								nStep = stepCh1CellRequest;
								m_Timer.Start();
							}
							else if(theProcBank.m_nCH1CellInfoRetryStep == 1)
							{
								theProcBank.m_nCH1CellInfoRetryStep = 0;
								//2017-09-24,SKCHO,Ÿ�Ӿƿ��϶� ��� ������ ������ �˶�
								if(!theUnitFunc.ReadCellInformationResult(CELL_POS_LD_TR_CH1))
								{								
									SetError(ALARM_CIM_CELL_INFO_CH1);
								}
							}					
						}
						//�����н� true �̸� ����
						else if(theConfigBank.m_Option.m_bUseCh1ReqByPass)
						{			
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellInfoRequest BY-PASS"),m_strStateName);
							m_Timer.Start();

							if (bCh2ReadingChk == MCR_RESULT_NONE)
								nStep = stepCh2Move;
							else
								nStep = stepZUpPos;
						}
					
					}
					else
					{

						//2018-07-25,SKCHO, InspectSkip�� AZONE���� �Ǵ��ؼ� ����
						//theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH1,TRUE);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL1 TO NG"),m_strStateName);
	
						m_Timer.Start();
						if (bCh2ReadingChk == MCR_RESULT_NONE)
							nStep = stepCh2Move;
						else
							nStep = stepZUpPos;
					}
				}
				else
				{
					//2018-07-25,SKCHO, InspectSkip�� AZONE���� �Ǵ��ؼ� ����
					//	theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH1,TRUE);
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL1 TO NG"),m_strStateName);

					m_Timer.Start();
					if (bCh2ReadingChk == MCR_RESULT_NONE)
						nStep = stepCh2Move;
					else
						nStep = stepZUpPos;
				}
			}
			else
			{
				if (bCh2ReadingChk == MCR_RESULT_NONE)
					nStep = stepCh2Move;
				else
					nStep = stepZUpPos;

					m_Timer.Start();		
			}
		break;
	case stepCh2Move: 			// X Lower MCR Ch2 ��ġ�� �̵�.
		m_bRtn[0] = (BYTE)theProcBank.GetMCRFail();  //2017-09-17,SKCHO, LOWER MCR CH1 FAIL ī��Ʈ üũ
		if(m_bRtn[0] == 0)
		{
			LDTR_X_LOWERMCRPos(JIG_CH_2,0.5);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2Move"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else
		{
			//20171017 JSJUNG, MCR 5ȸ�̻� ���н� ���ѷ��� ���� �˶� ó��
			if(m_bRtn[0] == 1) SetError(ALARM_LOWER_MCR_CH1_CONTINUE_FAIL); //lower ch1 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 2) SetError(ALARM_LOWER_MCR_CH2_CONTINUE_FAIL); //lower ch2 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 3) SetError(ALARM_UPPER_MCR_CH1_CONTINUE_FAIL); //upper ch1 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 4) SetError(ALARM_UPPER_MCR_CH2_CONTINUE_FAIL); //upper ch2 �˶��� �߻� ��Ŵ		
		}
		break;
	case stepCh2Stable:
		m_bRtn[0] = LDTR_X_LOWERMCRPosChk(JIG_CH_2);
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2Stable"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_X_CH2_LOWERMCR_POS);
		break;
	case stepCh2Reading:		// LD TR X Ch1 MCR Pos Chk �� MCR Reading.
		if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dMCRDelayTime)
		{
			Trigger(LOWER_MCR,JIG_CH_2);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2Reading"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh2ReadingChk:		// MCR Reading Chk
		//m_bRtn[0] = (MCR_GetLastID(JIG_CH_2).GetLength() > 1) ? TRUE:FALSE;
		//2018-04-18,NAM, Getlength for "Fail" is 4
		m_bRtn[0] = (MCR_GetLastID(JIG_CH_2).GetLength() > 4) ? TRUE:FALSE;
		if (IsReturnOk())
		{
			// MCR���� ��� ����.
			CellData_SetUVResult(LOWER_MCR,JIG_CH_2,MCR_RESULT_GOOD);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2ReadingChk %s"),m_strStateName,MCR_GetLastID(JIG_CH_2));
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Lower MCR Ch2 MCR Reading Success"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 1.5)
		{
			// MCR���� ��� ����.
			CellData_SetUVResult(LOWER_MCR,JIG_CH_2,MCR_RESULT_FAIL);

			//2017-04-14,skcho , ���α׷� �ٿ� ���� �־� �ӽ� ����
			SaveMCRNGImage(LOWER_MCR,JIG_CH_2);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Lower MCR Ch2 MCR Reading Fail"),m_strStateName);
			m_Timer.Start();
			//2018-07-25,SKCHO,
			//nStep++;
			nStep = stepZUpPos;
		}
		break;
	//2018-07-25,SKCHO,
	case stepCh2CellRequest:

			 //2018-07-25,SKCHO, CELL LOADING �� PRODUCT_ID ���� �ֱ� ���ؼ� CELL INFORMATION REQUEST�� MCR READING �������� �̵�
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_LD_TR_CH2));
		    if(pCell)
			{		
				if( pCell->m_strCellID != _T("FAIL") )
				{
					//2018-07-25,SKCHO, CellInforRequest �Լ� ����
					//theUnitFunc.CIM_CH2_CellInfoRequest();
					theUnitFunc.CIM_CellInfoRequest(CELL_POS_LD_TR_CH2);
					if(theProcBank.m_nCH2CellInfoRetryStep == 0)
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellInfoRequest"),m_strStateName);
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellInfoRequest - Retry"),m_strStateName);
					}
					pCell->bCellInfoRequest = TRUE;
					pCell->m_strCellInfoResult = _T("-1");//REQUEST �ʱⰪ			
				}
				else
				{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH2 MCR_FAIL"),m_strStateName);				
					theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH2,TRUE);  //2017-04-08,SKCHO, MCR READING ���н� �˻� ��ŵ
				
				}
			}
		
			m_Timer.Start();
			nStep++;
		break;
	case stepCh2CellRequestChk:
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_LD_TR_CH2));
			if(pCell)
			{
				if(pCell->bCellInfoRequest)
				{			
					int nRequest = theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_LD_TR_CH2);		

					if(nRequest == 1)  //�������� ������
					{
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellRequestChk -OK"),m_strStateName);	
						m_Timer.Start();
							nStep++;
					}
					else if( nRequest== 0)
					{
		
						//Ÿ�� �ƿ��̰� �����н�  false �̸� �˶�
						if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime && !theConfigBank.m_Option.m_bUseCh2ReqByPass)
						{
							//2017-09-24,SKCHO,Ÿ�Ӿƿ��� �ѹ��� ��û�ϰ� �ι�° Ÿ�Ӿƿ������� ������ �ִ��� Ȯ���Ѵ�.
							if(theProcBank.m_nCH2CellInfoRetryStep == 0)
							{
								theProcBank.m_nCH2CellInfoRetryStep = 1;
								nStep = stepCh2CellRequest;
								m_Timer.Start();
							}
							else if(theProcBank.m_nCH2CellInfoRetryStep == 1)
							{
								theProcBank.m_nCH2CellInfoRetryStep = 0;
								//2017-09-24,SKCHO,Ÿ�Ӿƿ��϶� ��� ������ ������ �˶�
								if(!theUnitFunc.ReadCellInformationResult(CELL_POS_LD_TR_CH2))
								{								
									SetError(ALARM_CIM_CELL_INFO_CH2);
								}
							}					
						}
						//�����н� true �̸� ����
						else if(theConfigBank.m_Option.m_bUseCh2ReqByPass)
						{			
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellInfoRequest BY-PASS"),m_strStateName);
							m_Timer.Start();
							nStep++;	
						}
					
					}
					else
					{
						//2018-07-25,SKCHO, InspectSkip�� AZONE���� �Ǵ��ؼ� ����
						//theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH2,TRUE);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL2TO NG"),m_strStateName);						
						m_Timer.Start();
						nStep++;
					}
				}
				else
				{
					//2018-07-25,SKCHO, InspectSkip�� AZONE���� �Ǵ��ؼ� ����
					//theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH2,TRUE);
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL2 TO NG"),m_strStateName);
					m_Timer.Start();
					nStep++;
				}
			}
			else
			{
				m_Timer.Start();	
				nStep++;
			}
		break;
	case stepZUpPos:
		m_bRtn[0] = (BYTE)theProcBank.GetMCRFail();  //2017-09-17,SKCHO, LOWER MCR CH1 FAIL ī��Ʈ üũ
		if(m_bRtn[0] == 0)
		{
			LDTRZUpPos();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUpPos"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else
		{
			//20171017 JSJUNG, MCR 5ȸ�̻� ���н� ���ѷ��� ���� �˶� ó��
			if(m_bRtn[0] == 1) SetError(ALARM_LOWER_MCR_CH1_CONTINUE_FAIL); //lower ch1 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 2) SetError(ALARM_LOWER_MCR_CH2_CONTINUE_FAIL); //lower ch2 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 3) SetError(ALARM_UPPER_MCR_CH1_CONTINUE_FAIL); //upper ch1 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 4) SetError(ALARM_UPPER_MCR_CH2_CONTINUE_FAIL); //upper ch2 �˶��� �߻� ��Ŵ	
		}
		break;
	case stepXPutPos:
		m_bRtn[0] = LDTRZUpPosChk();
		if (IsReturnOk())
		{
			LDTRXPutPos();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepXPutPos"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_Z_UP_POS);
		break;
	case stepEnd:				// Put Pos Ȯ�� �� End
		m_bRtn[0] = LDTRXPutPosChk();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);

			//2017-09-17,SKCHO, FALSE�� ����� UPPER���� �а� ������
			if (!theConfigBank.m_Option.m_bUseLowerMCRRead)
			{
				theProcBank.m_bUpperMCRCompChk = FALSE;
				CellData_SetUVResult(LOWER_MCR,JIG_CH_1,MCR_RESULT_SKIP);
				CellData_SetUVResult(LOWER_MCR,JIG_CH_2,MCR_RESULT_SKIP);
			}

			nStep = stepIdle;	
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_X_PUT_POS);
		break;
	}

	m_nStep = nStep;

	return nStep;
}


int CStateLDTRPut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			// A Zone Cell ���� Ȯ�� �� TR X �̵�.
		bExistCh1Cell = CellTagExist(CELL_POS_LD_TR_CH1);
		bExistCh2Cell = CellTagExist(CELL_POS_LD_TR_CH2);

		LDTRXPutPos();
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepXPutPos"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepZOffsetDown:			// TR X ��ġ Ȯ�� �� Z Offset Down
		m_bRtn[0] = LDTRXPutPosChk();
		if (IsReturnOk())
		{
			//2018-01-18,JSJUNG, LDTR X PUT �������� ��ġ �α� �߰�.
			double dValue = 0;
			theDeviceMotion.GetPosition(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_LD_TR_X, dValue);
			theLog[LOG_TEACH_MOVE].AddBuf( _T("LDTR X PUT : %f"),dValue);

			LDTRZPutPos(-5.);

			//2017-10-09,SKCHO,TMD INFO ��û��
			// TMD���� ó�� ����.
			//A_Zone_TMDInfo(JIG_CH_1);
			//A_Zone_TMDInfo(JIG_CH_2);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZOffsetDown"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_X_PUT_POS);
		break;
	case stepBlowOn:		 // Offset Check �� Z Down �� Blow On
		m_bRtn[0] = LDTRZPutPosChk(-5.);
		if (IsReturnOk())
		{
			LDTRZPutPos();
			LDTRFPCBBlowOn(BLOW_ON,bExistCh1Cell,bExistCh2Cell);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepBlowOn"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_Z_PUT_POS); 
		break;
	case stepContact:
		m_bRtn[0] = LDTRZPutPosChk();
		//m_bRtn[1] =m_Timer.Stop(FALSE) > 0.5 ? TRUE:FALSE;  //2017-04-11,skcho, blow �ð� �ø�
		if (IsReturnOk())
		{
			//2018-01-18,JSJUNG, LDTR Z PUT �������� ��ġ �α� �߰�.
			double dValue = 0;
			theDeviceMotion.GetPosition(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_LD_TR_Z, dValue);
			theLog[LOG_TEACH_MOVE].AddBuf( _T("LDTR Z PUT : %f"),dValue);

			LDTRVacOn(VAC_OFF,JIG_CH_MAX);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContact"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_Z_PUT_POS);
		break;
	case stepZUp:		 // Vac ���� �� Contact Ȯ�� �� TR Z Up.
		if(bExistCh1Cell)
			m_bRtn[0] = LDTRCellVacChk(JIG_CH_1,VAC_OFF);
		else
			m_bRtn[0] = TRUE;

		if(bExistCh2Cell)
			m_bRtn[1] = LDTRCellVacChk(JIG_CH_2,VAC_OFF);
		else
			m_bRtn[1] = TRUE;

		if (IsReturnOk()) 
		{
			LDTR_BlowOff(JIG_CH_MAX);
			//���� ��ٴ� ��� ���� �Ŀ� ���� �ɷ� ����.
//			LDTRFPCBBlowOn(BLOW_OFF,bExistCh1Cell,bExistCh2Cell);
			LDTRZUpPos();
			CellData_AMTFromLDTR();
			// ���� ID ����.	
			CellData_SetJIGID(AMT_GetAutoJigID(ZONE_ID_A));

			//2018-07-25,SKCHO, Cell Information Request  Success is contact		
			if( bExistCh1Cell && theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH1) == 1 )
			{
					ContactStart(JIG_CH_1);	
			}
			
			if( bExistCh2Cell && theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH2) == 1 )
			{
					ContactStart(JIG_CH_2);	
			}
			/*
			// TR�� ���� ���� �� Contact ��ȣ ������.
			if (bExistCh1Cell)
				ContactStart(JIG_CH_1);	
			if (bExistCh2Cell)
				ContactStart(JIG_CH_2);
				*/
			AZoneContactStartTime();

			if (theConfigBank.m_Option.m_bUseUpperMCRRead)
			{
				if(CellData_UVReadingChk(UPPER_MCR,JIG_CH_1) == MCR_RESULT_GOOD
				&& CellData_UVReadingChk(UPPER_MCR,JIG_CH_2) == MCR_RESULT_GOOD)
					theProcBank.m_bUpperMCRCompChk = TRUE;
				else
					theProcBank.m_bUpperMCRCompChk = FALSE;
			}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			LDTR_BlowOff(JIG_CH_MAX);
			LDTRZUpPos();
			CellData_AMTFromLDTR();
			CellData_SetJIGID(AMT_GetAutoJigID(ZONE_ID_A));
			
			//2018-07-25,SKCHO, Cell Information Request  Success is contact		
			if( bExistCh1Cell && theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH1) == 1 )
			{
					ContactStart(JIG_CH_1);	
			}
			
			if( bExistCh2Cell && theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH2) == 1 )
			{
					ContactStart(JIG_CH_2);	
			}
			/*
			// TR�� ���� ���� �� Contact ��ȣ ������.
			if (bExistCh1Cell)
				ContactStart(JIG_CH_1);	
			if (bExistCh2Cell)
				ContactStart(JIG_CH_2);
				*/
			AZoneContactStartTime();

			if (theConfigBank.m_Option.m_bUseUpperMCRRead)
			{
				if(CellData_UVReadingChk(UPPER_MCR,JIG_CH_1) == MCR_RESULT_GOOD
				&& CellData_UVReadingChk(UPPER_MCR,JIG_CH_2) == MCR_RESULT_GOOD)
					theProcBank.m_bUpperMCRCompChk = TRUE;
				else
					theProcBank.m_bUpperMCRCompChk = FALSE;
			}
	
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp Vac Fail"),m_strStateName);
			m_Timer.Start();
			nStep++;
			
			if (m_bRtn[0] == FALSE)			SetError(ALARM_LD_TR_CH1_VAC);
			else if (m_bRtn[1] == FALSE)	SetError(ALARM_LD_TR_CH2_VAC);
		}
		break;
	case stepZUpChk:
		m_bRtn[0] = LDTRZUpPosChk();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUpChk"),m_strStateName);
			//2018-07-25,SKCHO, Cell Information Request  Success is TIM_INFO		
			if( bExistCh1Cell && theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH1) == 1 )
			{
					A_Zone_TMDInfo(JIG_CH_1);	
			}
			
			if( bExistCh2Cell && theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH2) == 1 )
			{
					A_Zone_TMDInfo(JIG_CH_2);	
			}
			/*
			//2017-10-09,SKCHO,TMD INFO ��û��
			//�������� ���׿� ������ TMD INFO�� ��û��
			if (bExistCh1Cell)
			A_Zone_TMDInfo(JIG_CH_1);

			if (bExistCh2Cell)
			A_Zone_TMDInfo(JIG_CH_2);
			*/
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_LD_TR_Z_UP_POS);
		}
		break;
	case stepBlowerOff:
		m_bRtn[0] = m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dLDTRBlowerOffTime ? TRUE : FALSE;  
		if(IsReturnOk())
		{
			LDTRFPCBBlowOn(BLOW_OFF,bExistCh1Cell,bExistCh2Cell);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Blower OFF"),m_strStateName);

			//2017-12-06,SKCHO BLOWER OFF���� ���� ����(�赿ȣ ���� ��û)
			if (CellTagExist(CELL_POS_AZONE_CH1))
			{
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
			}
			if (CellTagExist(CELL_POS_AZONE_CH2))
			{
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
			}
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepXMove:
		//2017-12-06,SKCHO TACK TIME �������� ����
		//if(m_Timer.Stop(FALSE) > 1.)
		//{
			if(CellData_UVReadingChk(UPPER_MCR,JIG_CH_1) == MCR_RESULT_GOOD && CellData_UVReadingChk(UPPER_MCR,JIG_CH_2) == MCR_RESULT_GOOD)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepXMove -> stepXMoveChk"),m_strStateName);
				LDTRXGetPos();	
				m_Timer.Start();
				nStep++;
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepXMove -> stepEnd"),m_strStateName);
				m_Timer.Start();
				nStep = stepEnd;
			}
		//}
		break;
	case stepXMoveChk:
		m_bRtn[0] = LDTRXGetPosChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s]stepXMoveChk"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_LD_TR_X_GET_POS);
		}
		break;
	case stepEnd:		// TR Z Up Check �� MCR �������� Ȯ�� �� �Ѵ� ������ Get ��ġ�� ���н� MCR ��ġ�� �̵� �� End
		LDTRFPCBBlowOn(BLOW_OFF,TRUE,TRUE);
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
		nStep = stepIdle;		
		break;
	}

	m_nStep = nStep;

	return nStep;
}


int CStateUpperMCR::Run()
{
	CCellInfo* pCell;
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:			// Z Upper MCR ��ġ�� �̵�.
		LDTR_Z_UPPERMCRPos();
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepZChk:			// Z Upper Pos Chk
		m_bRtn[0] = LDTR_Z_UPPERMCRPosChk();
		bCh1ReadingChk = CellData_UVReadingChk(UPPER_MCR,JIG_CH_1);
		bCh2ReadingChk = CellData_UVReadingChk(UPPER_MCR,JIG_CH_2);
		if (IsReturnOk())
		{
			if (bCh1ReadingChk != MCR_RESULT_GOOD)
				nStep = stepCh1Move;
			else if(bCh2ReadingChk != MCR_RESULT_GOOD)
				nStep = stepCh2Move;

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZChk"),m_strStateName);
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_Z_UPPERMCR_POS);
		break;
	case stepCh1Move:		//Ch1 MCR ��ġ �̵�.
		LDTR_X_UPPERMCRPos(JIG_CH_1);
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1Move"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepCh1Stable:		
		m_bRtn[0] = LDTR_X_UPPERMCRPosChk(JIG_CH_1);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1Stable"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_X_CH1_UPPERMCR_POS);
		break;
	case stepCh1Reading:			// 1CH MCR ��ġ Check �� MCR Reading �õ�.
		if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dMCRDelayTime)
		{
			Trigger(UPPER_MCR,JIG_CH_1);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1Reading"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh1ReadingChk:		  // 1CH MCR Reading üũ 
		//m_bRtn[0] = (MCR_GetLastID(JIG_CH_1).GetLength() > 1) ? TRUE:FALSE;
		//2018-04-18,NAM, Getlength for "Fail" is 4
		m_bRtn[0] = (MCR_GetLastID(JIG_CH_1).GetLength() > 4) ? TRUE:FALSE;
		if (IsReturnOk())
		{
			// MCR ���� ���� ����.
			CellData_SetUVResult(UPPER_MCR,JIG_CH_1,MCR_RESULT_GOOD);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1ReadingChk %s"),m_strStateName,MCR_GetLastID(JIG_CH_1));
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Upper MCR Ch1 MCR Reading Success"),m_strStateName);
			
			//2018-07-25,SKCHO,
			m_Timer.Start();
			nStep++;
			/*if (bCh2ReadingChk != MCR_RESULT_GOOD)
			{
				nStep = stepCh2Move;
			}
			else
			{
				theProcBank.m_bUpperMCRCompChk = TRUE;
				nStep = stepXGetMove;
			}*/
		}
		else if (m_Timer.Stop(FALSE) > 2.)
		{
			// MCR ���� ���� ����.
			CellData_SetUVResult(UPPER_MCR,JIG_CH_1,MCR_RESULT_FAIL);
			//2017-04-14,skcho , ���α׷� �ٿ� ���� �־� �ӽ� ����
			SaveMCRNGImage(UPPER_MCR,JIG_CH_1);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Upper MCR Ch1 MCR Reading Fail"),m_strStateName);
			m_Timer.Start();
			if (bCh2ReadingChk != MCR_RESULT_GOOD)
			{
				nStep = stepCh2Move;
			}
			else
			{
				theProcBank.m_bUpperMCRCompChk = TRUE;
				nStep = stepXGetMove;
			}
		}
		break;
	//2018-07-25,SKCHO,
	case stepCh1CellRequest:

			 //2018-07-25,SKCHO, CELL LOADING �� PRODUCT_ID ���� �ֱ� ���ؼ� CELL INFORMATION REQUEST�� MCR READING �������� �̵�
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
		    if(pCell)
			{		
				if( pCell->m_strCellID != _T("FAIL") )
				{
				    //2018-07-25,SKCHO, CellInforRequest�Լ� ����
					//theUnitFunc.CIM_CH1_CellInfoRequest();
					theUnitFunc.CIM_CellInfoRequest(CELL_POS_AZONE_CH1);
					if(theProcBank.m_nCH1CellInfoRetryStep == 0)
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellInfoRequest"),m_strStateName);
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellInfoRequest - Retry"),m_strStateName);
					}
					pCell->bCellInfoRequest = TRUE;
					pCell->m_strCellInfoResult = _T("-1");//REQUEST �ʱⰪ		

					m_Timer.Start();
					nStep++;
				}
				else
				{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH1 MCR_FAIL"),m_strStateName);
					theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH1,TRUE);  //2017-04-08,SKCHO, MCR READING ���н� �˻� ��ŵ				
				}
			}		
		
		break;
	case stepCh1CellRequestChk:
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
			if(pCell)
			{
				if(pCell->bCellInfoRequest)
				{			
					int nRequest = theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH1);				

					if(nRequest == 1)  //�������� ������
					{
							m_Timer.Start();
							if (bCh2ReadingChk != MCR_RESULT_GOOD){
								nStep = stepCh2Move;
							}
							else{
								theProcBank.m_bUpperMCRCompChk = TRUE;
								nStep = stepXGetMove;
							}

							//2018-07-25,SKCHO, CellInfoRequest �����̸� ����	 			
							ContactStart(JIG_CH_1);	
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] SEND CH1 Contact"),m_strStateName);
			
					}
					else if( nRequest== 0)
					{		
						//Ÿ�� �ƿ��̰� �����н�  false �̸� �˶�
						if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime && !theConfigBank.m_Option.m_bUseCh1ReqByPass)
						{
							//2017-09-24,SKCHO,Ÿ�Ӿƿ��� �ѹ��� ��û�ϰ� �ι�° Ÿ�Ӿƿ������� ������ �ִ��� Ȯ���Ѵ�.
							if(theProcBank.m_nCH1CellInfoRetryStep == 0)
							{
								theProcBank.m_nCH1CellInfoRetryStep = 1;
								nStep = stepCh1CellRequest;
								m_Timer.Start();
							}
							else if(theProcBank.m_nCH1CellInfoRetryStep == 1)
							{
								theProcBank.m_nCH1CellInfoRetryStep = 0;
								//2017-09-24,SKCHO,Ÿ�Ӿƿ��϶� ��� ������ ������ �˶�
								if(!theUnitFunc.ReadCellInformationResult(CELL_POS_AZONE_CH1))
								{								
									SetError(ALARM_CIM_CELL_INFO_CH1);
								}
							}					
						}
						//�����н� true �̸� ����
						else if(theConfigBank.m_Option.m_bUseCh1ReqByPass)
						{			
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellInfoRequest BY-PASS"),m_strStateName);
							m_Timer.Start();
							if (bCh2ReadingChk != MCR_RESULT_GOOD){
								nStep = stepCh2Move;
							}
							else{
								theProcBank.m_bUpperMCRCompChk = TRUE;
								nStep = stepXGetMove;
							}					
						}					
					}
					else
					{
						//2018-07-25,SKCHO, InspectSkip�� AZONE���� �Ǵ��ؼ� ����
						//theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH1,TRUE);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL1 TO NG"),m_strStateName);	
						m_Timer.Start();
						if (bCh2ReadingChk != MCR_RESULT_GOOD){
							nStep = stepCh2Move;
						}
						else{
							theProcBank.m_bUpperMCRCompChk = TRUE;
							nStep = stepXGetMove;
						}
					}
				}
				else
				{
					//2018-07-25,SKCHO, InspectSkip�� AZONE���� �Ǵ��ؼ� ����
					//	theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH1,TRUE);
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL1 TO NG"),m_strStateName);
					m_Timer.Start();
					if (bCh2ReadingChk != MCR_RESULT_GOOD){
						nStep = stepCh2Move;
					}
					else{
						theProcBank.m_bUpperMCRCompChk = TRUE;
						nStep = stepXGetMove;
					}
			
				}
			}
			else
			{				
					m_Timer.Start();		
					if (bCh2ReadingChk != MCR_RESULT_GOOD){
						nStep = stepCh2Move;
					}
					else{
						theProcBank.m_bUpperMCRCompChk = TRUE;
						nStep = stepXGetMove;
					}
			}
		break;
	case stepCh2Move:		 // 2CH MCR ��ġ �̵�
		m_bRtn[0] = (BYTE)theProcBank.GetMCRFail();  //2017-09-17,SKCHO, LOWER MCR CH1 FAIL ī��Ʈ üũ
		if(m_bRtn[0] == 0)
		{
			LDTR_X_UPPERMCRPos(JIG_CH_2);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2Move"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else
		{
			//20171017 JSJUNG, MCR 5ȸ�̻� ���н� ���ѷ��� ���� �˶� ó��
			if(m_bRtn[0] == 1) SetError(ALARM_LOWER_MCR_CH1_CONTINUE_FAIL); //lower ch1 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 2) SetError(ALARM_LOWER_MCR_CH2_CONTINUE_FAIL); //lower ch2 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 3) SetError(ALARM_UPPER_MCR_CH1_CONTINUE_FAIL); //upper ch1 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 4) SetError(ALARM_UPPER_MCR_CH2_CONTINUE_FAIL); //upper ch2 �˶��� �߻� ��Ŵ
		}
		break;
	case stepCh2Stable:		 // 2CH MCR ��ġ Check �� MCR Reading �õ�.
		m_bRtn[0] = LDTR_X_UPPERMCRPosChk(JIG_CH_2);
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2Stable"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
			SetError(ALARM_LD_TR_X_CH2_UPPERMCR_POS);
		break;
	case stepCh2Reading:		 // 2CH MCR ��ġ Check �� MCR Reading �õ�.
		if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dMCRDelayTime)
		{
			Trigger(UPPER_MCR,JIG_CH_2);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2Reading"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh2ReadingChk:		// 2CH MCR Reading üũ 
		//m_bRtn[0] = (MCR_GetLastID(JIG_CH_2).GetLength() > 1) ? TRUE:FALSE;
		//2018-04-18,NAM, Getlength for "Fail" is 4
		m_bRtn[0] = (MCR_GetLastID(JIG_CH_2).GetLength() > 4) ? TRUE:FALSE;
		if (IsReturnOk())
		{
			// MCR ���� ���� ����.
			CellData_SetUVResult(UPPER_MCR,JIG_CH_2,MCR_RESULT_GOOD);
			theProcBank.m_bUpperMCRCompChk = TRUE;

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2ReadingChk %s"),m_strStateName,MCR_GetLastID(JIG_CH_2));
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Upper MCR Ch2 MCR Reading Success"),m_strStateName);
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 2.)
		{
			// MCR ���� ���� ����.
			CellData_SetUVResult(UPPER_MCR,JIG_CH_2,MCR_RESULT_FAIL);
			theProcBank.m_bUpperMCRCompChk = TRUE;
			//2017-04-14,skcho , ���α׷� �ٿ� ���� �־� �ӽ� ����
			SaveMCRNGImage(UPPER_MCR,JIG_CH_2);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Upper MCR Ch2 MCR Reading Fail"),m_strStateName);
			m_Timer.Start();
			nStep = stepXGetMove;
			//nStep++;
		}
		break;
case stepCh2CellRequest:

			 //2018-07-25,SKCHO, CELL LOADING �� PRODUCT_ID ���� �ֱ� ���ؼ� CELL INFORMATION REQUEST�� MCR READING �������� �̵�
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
		    if(pCell)
			{		
				if( pCell->m_strCellID != _T("FAIL") )
				{
					//2018-07-25,SKCHO, CellInforRequest�Լ� ����
					//theUnitFunc.CIM_CH2_CellInfoRequest();
					theUnitFunc.CIM_CellInfoRequest(CELL_POS_AZONE_CH2);
					if(theProcBank.m_nCH2CellInfoRetryStep == 0)
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellInfoRequest"),m_strStateName);
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellInfoRequest - Retry"),m_strStateName);
					}
					pCell->bCellInfoRequest = TRUE;
					pCell->m_strCellInfoResult = _T("-1");//REQUEST �ʱⰪ	
					m_Timer.Start();
					nStep++;
				}
				else
				{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH2 MCR_FAIL"),m_strStateName);				
					theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH2,TRUE);  //2017-04-08,SKCHO, MCR READING ���н� �˻� ��ŵ
					m_Timer.Start();
					nStep = stepXGetMove ;
				
				}
			}	
		
		break;
	case stepCh2CellRequestChk:
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
			if(pCell)
			{
				if(pCell->bCellInfoRequest)
				{			
					int nRequest = theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH2);		

					if(nRequest == 1)  //�������� ������
					{
						   //2018-07-25,SKCHO, CellInfoRequest �����̸� ����	 			
							ContactStart(JIG_CH_2);	
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] SEND CH2 Contact"),m_strStateName);

							m_Timer.Start();
							nStep++;
					}
					else if( nRequest== 0)
					{
		
						//Ÿ�� �ƿ��̰� �����н�  false �̸� �˶�
						if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime && !theConfigBank.m_Option.m_bUseCh2ReqByPass)
						{
							//2017-09-24,SKCHO,Ÿ�Ӿƿ��� �ѹ��� ��û�ϰ� �ι�° Ÿ�Ӿƿ������� ������ �ִ��� Ȯ���Ѵ�.
							if(theProcBank.m_nCH2CellInfoRetryStep == 0)
							{
								theProcBank.m_nCH2CellInfoRetryStep = 1;
								nStep = stepCh2CellRequest;
								m_Timer.Start();
							}
							else if(theProcBank.m_nCH2CellInfoRetryStep == 1)
							{
								theProcBank.m_nCH2CellInfoRetryStep = 0;
								//2017-09-24,SKCHO,Ÿ�Ӿƿ��϶� ��� ������ ������ �˶�
								if(!theUnitFunc.ReadCellInformationResult(CELL_POS_AZONE_CH2))
								{								
									SetError(ALARM_CIM_CELL_INFO_CH2);
								}
							}					
						}
						//�����н� true �̸� ����
						else if(theConfigBank.m_Option.m_bUseCh2ReqByPass)
						{			
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellInfoRequest BY-PASS"),m_strStateName);
							m_Timer.Start();
							nStep++;	
						}
					
					}
					else
					{
						//2018-07-25,SKCHO, InspectSkip�� AZONE���� �Ǵ��ؼ� ����
						//theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH2,TRUE);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL2TO NG"),m_strStateName);						
						m_Timer.Start();
						nStep++;
					}
				}
				else
				{
					//2018-07-25,SKCHO, InspectSkip�� AZONE���� �Ǵ��ؼ� ����
					//theUnitFunc.Set_InspectSkip(CELL_POS_LD_TR_CH2,TRUE);
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL2 TO NG"),m_strStateName);
					m_Timer.Start();
					nStep++;
				}
			}
			else
			{
				m_Timer.Start();	
				nStep++;
			}
		break;
	case stepXGetMove:	// X GetPos Move
		m_bRtn[0] = (BYTE)theProcBank.GetMCRFail();  //2017-09-17,SKCHO, LOWER MCR CH1 FAIL ī��Ʈ üũ
		if(m_bRtn[0] == 0)
		{
			LDTRXGetPos();
			LDTRZUpPos();
			//2018-07-25,SKCHO, Cell Information Request  Success is TIM_INFO		
			if( theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH1) == 1 )
			{
					A_Zone_TMDInfo(JIG_CH_1);
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] SEND CH1 TMD_INFO"),m_strStateName);
			}			
			if( theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH2) == 1 )
			{
					A_Zone_TMDInfo(JIG_CH_2);
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] SEND CH2 TMD_INFO"),m_strStateName);
			}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepXGetMove"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else
		{
			//20171017 JSJUNG, MCR 5ȸ�̻� ���н� ���ѷ��� ���� �˶� ó��
			if(m_bRtn[0] == 1) SetError(ALARM_LOWER_MCR_CH1_CONTINUE_FAIL); //lower ch1 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 2) SetError(ALARM_LOWER_MCR_CH2_CONTINUE_FAIL); //lower ch2 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 3) SetError(ALARM_UPPER_MCR_CH1_CONTINUE_FAIL); //upper ch1 �˶��� �߻� ��Ŵ
			else if(m_bRtn[0] == 4) SetError(ALARM_UPPER_MCR_CH2_CONTINUE_FAIL); //upper ch2 �˶��� �߻� ��Ŵ
		}
		break;
	case stepEnd:		// X GetPos Ȯ��.
		m_bRtn[0] = LDTRXGetPosChk();
		m_bRtn[1] = LDTRZUpPosChk();
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			nStep = stepIdle;		
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_LD_TR_X_GET_POS);
			else						SetError(ALARM_LD_TR_Z_UP_POS);
		}

		break;
	}

	m_nStep = nStep;

	return nStep;
}