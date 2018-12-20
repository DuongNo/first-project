#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateJobChange : 	public CUnitCtrlBank
{
public:
	CStateJobChange(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateJobChange(void) {};

	enum
	{
		stepStart, 
		stepJob2InfoChk,
		stepJob2Attri,
		stepJob2AttriChk,
		stepEnd, 
	};

	int Run();
};

class CStateUDTRGet : 	public CUnitCtrlBank
{
public:
	CStateUDTRGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUDTRGet(void) {};

	enum
	{
		stepStart, // D Zone�� Cell �� �ְ� Zone End ���� üũ �� UD TR X�� Get��ġ�� �ƴ� ��� Get �̵�.
		stepZDown, // UD TR X �̵� üũ ��  Z Down.
		stepVacOn, // Z Down Check �� UD TR Vac On, D Zone Vac Off.
		stepZUp1,
		stepZUp1Chk,
		stepZUp, // UD TR Vac �� D Zone Vac Check �� Z up.
		stepEnd, // Z Up �̵� üũ �� End.
	};

	BOOL m_bCH1_Cell, m_bCH2_Cell;

	int Run();
};


class CStateUDTRPut : 	public CUnitCtrlBank
{
public:
	CStateUDTRPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUDTRPut(void) {};

	enum
	{
		stepStart, // UD Stage�� Cell�� ������ , Robot�� Get��ġ�� �ƴ��� üũ �� UD TR X Put ��ġ �̵�
		stepZDown, // UD TR X Put��ġ üũ �� Z Down 
		stepVacOff, // Z Down Check �� UD TR Vac Off , Stage Vac On.
		stepZup, // Vac Check�� Z Up.
		stepGetMove, // Z up Check �� UD TR X Get ��ġ�� ����.
		stepEnd, // UD TR X GET ��ġ üũ �� End.
	};

	BOOL m_bCH1_Cell, m_bCH2_Cell;

	int Run();
};


class CStateUDRobotGet : 	public CUnitCtrlBank
{
public:
	CStateUDRobotGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUDRobotGet(void) {};

	enum
	{
		stepStart, // UD Stage�� Cell �� �ְ� UD TR X�� Get ��ġ���� Ȯ�� �� Get��ġ�� �̵�.
		stepBusyChk, // UD Robot �����ӻ���üũ.
		stepGetChk,
		stepRobotVacOn, // UD Robot Vac On
		stepRobotVacOnChk, // Vac On Chk
		stepVacOff, // UD Stage Vac Off
		stepGetComp,
		stepEnd, // Robot ��ȣ Chk�� End
// 		stepJob2Start,   //jobchange case2 ����(ppid�� ���°��)
// 		stepJob2Info,    //cell lot infomation request
// 		stepJob2InfoChk, //cell lot infomation result
// 		stepJob2Attri,   //Get Attribute request
// 		stepJob2AttriChk, // Attribute result
// 		stepJob2End,
	};

	BOOL m_bCH1_Cell;

	int Run();
};

/////20180816wjp

//20180801wz///////////////////////////////////////////

class CStateUDRobotPick2 : 	public CUnitCtrlBank
{
public:
	CStateUDRobotPick2(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUDRobotPick2(void) {};

	enum
	{
		stepStart, // �κ��� Cell �� �ְ� Tray ���� Ȯ�� �� �̵�.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait ��ġ üũ �� End
	};

	int Run();
};

//duong
/*
class CTest_1 : 	public CUnitCtrlBank
{
public:
	CTest_1 (CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CTest_1 (void) {};

	enum
	{
		stepStart, // �κ��� Cell �� �ְ� Tray ���� Ȯ�� �� �̵�.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait ��ġ üũ �� End
	};

	int Run();
};*/
//===========================================


////20180816wjp

class CStateGoodTrayGetPut : public CUnitCtrlBank
{
public:
	CStateGoodTrayGetPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
		RetryNum=0;
	};
	~CStateGoodTrayGetPut(void) {};

	enum
	{
		stepStart, // �κ��� Cell �� �ְ� Tray ���� Ȯ�� �� �̵�.
		stepInLiftCylBFWD,
		stepInLiftPitchDn,	// Vacoff
		stepTratTRZDn,
		stepVacOn,
		stepTrayTRZUp,
		stepTrayTRYIn,
		stepTrayTRZIn,
		stepVacOff,
		stepTrayTRZUpOut,
		stepTrayTRYOut,
		stepInLiftUp,
		stepInLiftDiCylFWD,
		stepInLiftAlCylFWD,
	    stepDCR,
		stepEnd, //Wait ��ġ üũ �� End
		stepInitCheck=1000,
	};

	int Run();
	//2018wjp
	int RetryNum;
	int RetryDCR;
	int ResetEx();
};



/////20180816wjp

class CStateUDRobotGoodPut : 	public CUnitCtrlBank
{
public:
	CStateUDRobotGoodPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUDRobotGoodPut(void) {};

	enum
	{
		stepStart, // �κ��� Cell �� �ְ� Tray ���� Ȯ�� �� �̵�.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait ��ġ üũ �� End
	};

	int Run();
	//2018wjp
	Cell_Coord cell;
};

class CStateUDRobotNGPut : 	public CUnitCtrlBank
{
public:
	CStateUDRobotNGPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUDRobotNGPut(void) {};

	enum
	{
		stepStart, // �κ��� Cell �� �ְ� Tray ���� Ȯ�� �� �̵�.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait ��ġ üũ �� End
	};

	int Run();
};
//2018-05-14,SKCHO
class CStateUDRobotConvPut : 	public CUnitCtrlBank
{
public:
	CStateUDRobotConvPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUDRobotConvPut(void) {};

	enum
	{
		stepStart, // �κ��� Cell �� �ְ� Tray ���� Ȯ�� �� �̵�.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait ��ġ üũ �� End
	};

	int Run();
};
//---------------------------------------------------------------------------


class CStateGoodTrayReceive : 	public CUnitCtrlBank
{
public:
	CStateGoodTrayReceive(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTrayReceive(void) {};

	enum
	{
		stepStart, // AGV Ready üũ �� �ε� ��û
		stepConnectChk,
		stepTransferChk,	// �ε� ��û ���� Transfer Req Chk �� Conv ���� ���
		stepBusyChk,	// AGV Busy Chk �� Conv Sen Chk �� Comp On
		stepStopperDown,	
		stepEnd, // // AGV Comp Ȯ�� �� AGV ��ȣ All Off
	};

	int Run();
	BOOL IsWaiting()	{ return m_nStep < stepEnd ? TRUE:FALSE; };
};


class CStateGoodTrayInLift : 	public CUnitCtrlBank
{
public:
	CStateGoodTrayInLift(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTrayInLift(void) {};

	enum
	{
		stepStart, // Conv1�� Tray Ȯ�� �ϰ� Lift�� Tray���� �����ϰ� Lift Arm BWD ���� �� down Pos�� Lift Down, Conv Stopper Down. 
		stepConvMove, // Lift Arm BWD Ȯ�� �� Lift Down Pos,  Stopper Down Ȯ�� �ϰ� Conv Move 
		stepConvCheck, // Lift Tray Load Ȯ�� �ϰ� Conv Stop ����. �� �� Lift Arm FWD ����.
		stepAlignPos, // 2017-12-22, �߰�
		stepGuideFwd, // Lift Up üũ �� Lower Guide FWD �����Ͽ� TRAY ����.
		stepGuideBwd, // Lower Guide FWD üũ �� Lower Guide BWD ����.
		stepLiftUp, // Lower Guide BWD üũ �� Lift Pitch Up ����.
		stepEnd, // stepLiftUp���� End�� ������ Teaching ����Ʈ Ȯ�� �� Step End. stepAlign���� ������ Align Guide Forward�� Tray Sensor Ȯ�� �� End
	};

	int Run();
};

class CStateGoodTrayTRGet : 	public CUnitCtrlBank
{
public:
	CStateGoodTrayTRGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTrayTRGet(void) {};

	enum
	{
		stepStart, // In tray�� Cell ������ Ȯ�� �� Robot ���� Ȯ�� �ϰ� Tray TR Y Get Pos�� �̵�.
		stepZDown, // Tray TR Y Get Pos Ȯ�� �� Vac On �� Z Down.
		stepGuideBack, // Vac On �� Z Down Ȯ�� �� Align , Division Guide Back Ward.
		stepGuideBackChk,
		stepZUpChk, // Guide Backward Ȯ�� �� Z Up.
		//stepYPosChk,    //2017-12-02,SKCHO, STEP �߰�
		stepEnd,// Assign ���� ���� ���� �߰� 2017/04/27 HSK.
/*
		stepYMove, // Z Up Ȯ�� �� Y Put Pos�� �̵�.
		stepEnd, // Y Put Pos Ȯ�� �� End.
*/
	};

	int Run();
};

class CStateGoodTrayInLiftPitch : 	public CUnitCtrlBank
{
public:
	CStateGoodTrayInLiftPitch(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTrayInLiftPitch(void) {};

	enum
	{
		stepStart, // Top Tray ���� Sensor Ȯ�� �� Tray ���� Ȯ�� �ϰ� Division Guide , Align Guide BackWard.
		stepPitchUp, // Align, Division BWD Ȯ�� �ϰ� Lift Pitch Up.
		stepPitchUpChk, // Align, Division BWD Ȯ�� �ϰ� Lift Pitch Up.
		stepDivision, // Top Tray Sensor ���� �ϸ� Division Guide Forward
		stepAlignForward, // Lift Pitch Down Ȯ�� �� Align Guide Forward�ؼ� Tray ��´�.
		stepEnd, // Align Guide Forward�� Tray Sensor Ȯ�� �� End.
	};

	int Run();
};

class CStateGoodTrayTRPut : 	public CUnitCtrlBank
{
public:
	CStateGoodTrayTRPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTrayTRPut(void) {};

	enum
	{
		stepStart, // TR�� Tray ���� Ȯ�� �� Y �� Out Port �̵� Ȯ�� �� �ƴ� ��� �̵�
		stepZDown, // Y�� Out Ȯ�� �� Z Down.
		stepVacOff, // Z down Ȯ�� ��  Vac Off.
		stepZUp, // Vac Off Ȯ�� �� Z Up.
		stepOutZDown,
		stepEnd, // Z Up Ȯ�� �� End.
	};

	int Run();
};

class CStateGoodTrayDCR : 	public CUnitCtrlBank
{
public:
	CStateGoodTrayDCR(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTrayDCR(void) {};

	enum
	{
		stepStart, 
		stepLiftUp,
		stepLiftStop,
		stepCylFwd,
		stepDCRTriggerON,
		stepIDReadCheck,
		stepTrayVIReadyChk,
		stepTrayVIStartComp,
		stepTrayVIResult,
		stepTrayVIResultOK,
		stepIDReport,
		stepEnd, 
	};

	int Run();
};

class CStateGoodTrayAlign : 	public CUnitCtrlBank
{
public:
	CStateGoodTrayAlign(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTrayAlign(void) {};

	enum
	{
		stepStart, 
		stepEnd, 
	};

	int Run();
};

class CStateGoodTrayOutLift : 	public CUnitCtrlBank
{
public:
	CStateGoodTrayOutLift(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTrayOutLift(void) {};

	enum
	{
		stepStart, // ������ Tray �ܼ��� �׿��� Ȯ�� �ϰ� Lift Down Pos�� �̵� �� Stopper Up.
		stepZAlignDown,		
		stepGuideFwd,
		stepGuideBwd,
		stepZDown,
		stepConvMove, // Lift Down Pos Ȯ�� �ϰ� Conv1 , Conv2 Move
		stepLiftUp, // Conv2�� Tray �Ȱ��� Ȯ�� �ϰ� Conv Stop, Stopper Down ��  Lift Up Pos�� �̵�.
		stepLiftUpChk,
		stepStopperDown,
		stepEnd, // LiftUp Ȯ�� �ϰ� End.
	};

	int Run();
};


class CStateGoodTraySend : 	public CUnitCtrlBank
{
public:
	CStateGoodTraySend(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTraySend(void) {};

	enum
	{
		stepStart, // AGV Ready üũ �� ��ε� ��û
		stepConnectChk,
		stepTransferChk,// ��ε� ��û ���� Transfer Req Chk �� Conv ���� ���
		stepBusyChk,	// AGV Busy Chk �� Conv Sen Chk �� Comp On
		stepEnd, // // AGV Comp Ȯ�� �� AGV ��ȣ All Off
	};

	int Run();
	BOOL IsWaiting()	{ return m_nStep < stepEnd ? TRUE:FALSE; };
};


class CStateNGTrayReceive : 	public CUnitCtrlBank
{
public:
	CStateNGTrayReceive(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTrayReceive(void) {};

	enum
	{
		stepStart, // AGV Ready üũ �� �ε� ��û, Buffer Stopper Down, In Stopper UP.
		stepTransferChk,// �ε� ��û ���� Transfer Req Chk �� Conv ���� ���
		stepBusyChk,	// AGV Busy Chk �� Conv Sen Chk �� Comp On, Stopper Down
		stepEnd, // // AGV Comp Ȯ�� �� AGV ��ȣ All Off
	};

	int Run();
	BOOL IsWaiting()	{ return m_nStep < stepEnd ? TRUE:FALSE; };
};

class CStateNGTrayManualReceive : 	public CUnitCtrlBank
{
public:
	CStateNGTrayManualReceive(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTrayManualReceive(void) {};

	enum
	{
		stepStart, // AGV Ready üũ �� �ε� ��û
		stepEnd, // // AGV Comp Ȯ�� �� AGV ��ȣ All Off

	};

	int Run();
	BOOL IsWaiting()	{ return m_nStep < stepEnd ? TRUE:FALSE; };
};

class CStateNGTrayIn : 	public CUnitCtrlBank
{
public:
	CStateNGTrayIn(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTrayIn(void) {};

	enum
	{
		stepStart, // Conv1�� Tray Ȯ�� �ϰ� Lift Teaching Pos�� Tray �����ص� �Ǵ� �����̸� Conv 1 , Conv 2 ����.
		stepCylDownChk, 
		stepBTMTrayUP,
		stepBTMFwd,
		stepBTMBwd,
		stepEnd, // Lift ������ Tray ���� ���� �ϰ� Conv 1,2 Stop.
	};

	int Run();
};


class CStateNGTrayInLiftUp : 	public CUnitCtrlBank
{
public:
	CStateNGTrayInLiftUp(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTrayInLiftUp(void) {};

	enum
	{
		stepStart, // Lift�� Tray ���� ���� �ϰ� , Tray�� Conv2 ������ ���� �ִ��� Ȯ�� �ϰ� Lift, Guide BackWard. Tray ������� Step Idle.
		stepLiftMid, // Lift Mid Pos �̵�
		stepLiftMidChk, // Lift Mid Pos Check
		stepLiftDown, // Lift Guide Backward Ȯ�� �ϰ� Lift �ٿ�, Tray Up.
		stepGuideForward, // �ٿ� Pos, Tray Up Ȯ�� �ϰ� Guide Forward�ؼ� Ʈ���� �����.
		stepLiftUp, //Guide Forward Ȯ�� �� Lift Up.
		stepEnd, // Lower Guide Backward Ȯ�� �� End.
	};

	int Run();
};


class CStateNGTrayInLiftPitch : 	public CUnitCtrlBank
{
public:
	CStateNGTrayInLiftPitch(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTrayInLiftPitch(void) {};

	enum
	{
		stepStart, //Top Tray ���� �� Top Tray Guide �� Division Back Ȯ�� �ϰ� Lift Pitch Up.
		stepPitchUp,
		stepPitchUpChk,
		stepLiftStop, // Top Tray �����ϰ� Lift Stop Ȯ�� , Division Forward.
		stepGuideForward, // Pitch Down Ȯ�� �ϰ� Tray Guide Forward.
		stepEnd, // Guide Forward Ȯ�� �ϰ� End.
	};
	
	int Run();
};


class CStateNGTrayTRGet : 	public CUnitCtrlBank
{
public:
	CStateNGTrayTRGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTrayTRGet(void) {};

	enum
	{
		stepStart, //Top Tray Cell ������ Ȯ�� �ϰ� X Get Pos�� �̵�.
		stepZDown, // X Get Pos Ȯ�� �� Z Down and Vac On.
		stepGuideBackward, // Z Down �� Vac On Ȯ�� �� Division, Tray Guide Back.
		stepZUp, // Guide Back Ȯ�� �� Z Up.
		stepZUpCheck,
		stepXMove, // Z Up Ȯ�� �� X Put Pos �̵�
		stepEnd, // X Put Pos Ȯ�� �� End
	};

	int Run();
};

class CStateNGTrayTRPut : 	public CUnitCtrlBank
{
public:
	CStateNGTrayTRPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTrayTRPut(void) {};

	enum
	{
		stepStart, //Tray TR�� Tray �ִ��� X Put Pos���� Ȯ�� �� Put Pos�� �ƴ� ��� Put Pos�� �̵�. 
		stepZDown, //X Put Pos Ȯ�� �� Z Down.
		stepVacOff, // Z Down Ȯ�� �� Vac Off.
		stepZUp, // Vac Off Ȯ�� �� Z Up and OutLift Pitch Down.
		stepEnd, // Z Up Ȯ�� �� Pitch down Ȯ�� �� End.
	};

	int Run();
};

class CStateNGTrayAlign : 	public CUnitCtrlBank
{
public:
	CStateNGTrayAlign(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTrayAlign(void) {};

	enum
	{
		stepStart, 
		stepTrayAlign,
		stepTrayAlignChk,
		stepEnd, 
	};

	int Run();
};

class CStateNGTrayOutLift : 	public CUnitCtrlBank
{
public:
	CStateNGTrayOutLift(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTrayOutLift(void) {};

	enum
	{
		stepStart, // ������ Tray �ܼ��� �׿��� Ȯ�� �ϰ� Lift Down Pos�� �̵�.
		stepLiftDownChk, // Lift Down Pos Ȯ�� ��  Lift arm Bwd.
		stepLiftUp,
		stepUpChk,
		stepEnd, // Lift Arm Bwd, Conv Tray Sensor Ȯ�� �ϰ� End.
	};

	int Run();
};



class CStateNGTraySend : 	public CUnitCtrlBank
{
public:
	CStateNGTraySend(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateNGTraySend(void) {};

	enum
	{
		stepStart, // AGV Ready üũ �� ��ε� ��û
		stepTransferChk,// ��ε� ��û ���� Transfer Req Chk �� Conv ���� ���
		stepBusyChk,	// AGV Busy Chk �� Conv Sen Chk �� Comp On
		stepEnd, // // AGV Comp Ȯ�� �� AGV ��ȣ All Off
	};

	int Run();
	BOOL IsWaiting()	{ return m_nStep < stepEnd ? TRUE:FALSE; };
};

//2017-09-17,SKCHO
class CStateGoodTrayManualReceive : 	public CUnitCtrlBank
{
public:
	CStateGoodTrayManualReceive(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateGoodTrayManualReceive(void) {};

	enum
	{
		stepStart, // AGV Ready üũ �� �ε� ��û
		stepBuffSenChk,	
		stepBuffSenChk1,
		stepEnd, // // AGV Comp Ȯ�� �� AGV ��ȣ All Off
	};

	int Run();
	BOOL IsWaiting()	{ return m_nStep < stepEnd ? TRUE:FALSE; };
};