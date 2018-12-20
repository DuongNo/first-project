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
		stepStart, // D Zone에 Cell 이 있고 Zone End 된지 체크 후 UD TR X가 Get위치가 아닐 경우 Get 이동.
		stepZDown, // UD TR X 이동 체크 후  Z Down.
		stepVacOn, // Z Down Check 후 UD TR Vac On, D Zone Vac Off.
		stepZUp1,
		stepZUp1Chk,
		stepZUp, // UD TR Vac 및 D Zone Vac Check 후 Z up.
		stepEnd, // Z Up 이동 체크 후 End.
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
		stepStart, // UD Stage에 Cell이 없는지 , Robot이 Get위치가 아닌지 체크 후 UD TR X Put 위치 이동
		stepZDown, // UD TR X Put위치 체크 후 Z Down 
		stepVacOff, // Z Down Check 후 UD TR Vac Off , Stage Vac On.
		stepZup, // Vac Check후 Z Up.
		stepGetMove, // Z up Check 후 UD TR X Get 위치로 도피.
		stepEnd, // UD TR X GET 위치 체크 후 End.
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
		stepStart, // UD Stage에 Cell 이 있고 UD TR X가 Get 위치인지 확인 후 Get위치로 이동.
		stepBusyChk, // UD Robot 움직임상태체크.
		stepGetChk,
		stepRobotVacOn, // UD Robot Vac On
		stepRobotVacOnChk, // Vac On Chk
		stepVacOff, // UD Stage Vac Off
		stepGetComp,
		stepEnd, // Robot 신호 Chk후 End
// 		stepJob2Start,   //jobchange case2 시작(ppid가 없는경우)
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
		stepStart, // 로봇에 Cell 이 있고 Tray 상태 확인 후 이동.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait 위치 체크 후 End
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
		stepStart, // 로봇에 Cell 이 있고 Tray 상태 확인 후 이동.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait 위치 체크 후 End
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
		stepStart, // 로봇에 Cell 이 있고 Tray 상태 확인 후 이동.
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
		stepEnd, //Wait 위치 체크 후 End
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
		stepStart, // 로봇에 Cell 이 있고 Tray 상태 확인 후 이동.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait 위치 체크 후 End
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
		stepStart, // 로봇에 Cell 이 있고 Tray 상태 확인 후 이동.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait 위치 체크 후 End
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
		stepStart, // 로봇에 Cell 이 있고 Tray 상태 확인 후 이동.
		stepPutChk,
		stepVacOff,	// Vacoff
		stepVacOffChk,
		stepPutComp,
		stepEnd, //Wait 위치 체크 후 End
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
		stepStart, // AGV Ready 체크 후 로딩 요청
		stepConnectChk,
		stepTransferChk,	// 로딩 요청 받은 Transfer Req Chk 후 Conv 구동 명령
		stepBusyChk,	// AGV Busy Chk 후 Conv Sen Chk 후 Comp On
		stepStopperDown,	
		stepEnd, // // AGV Comp 확인 후 AGV 신호 All Off
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
		stepStart, // Conv1에 Tray 확인 하고 Lift에 Tray없음 감지하고 Lift Arm BWD 진행 후 down Pos로 Lift Down, Conv Stopper Down. 
		stepConvMove, // Lift Arm BWD 확인 후 Lift Down Pos,  Stopper Down 확인 하고 Conv Move 
		stepConvCheck, // Lift Tray Load 확인 하고 Conv Stop 진행. 그 후 Lift Arm FWD 진행.
		stepAlignPos, // 2017-12-22, 추가
		stepGuideFwd, // Lift Up 체크 후 Lower Guide FWD 진행하여 TRAY 정렬.
		stepGuideBwd, // Lower Guide FWD 체크 후 Lower Guide BWD 진행.
		stepLiftUp, // Lower Guide BWD 체크 후 Lift Pitch Up 진행.
		stepEnd, // stepLiftUp에서 End로 왔으면 Teaching 포인트 확인 후 Step End. stepAlign에서 왔으면 Align Guide Forward및 Tray Sensor 확인 후 End
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
		stepStart, // In tray에 Cell 가득참 확인 및 Robot 없음 확인 하고 Tray TR Y Get Pos로 이동.
		stepZDown, // Tray TR Y Get Pos 확인 후 Vac On 및 Z Down.
		stepGuideBack, // Vac On 및 Z Down 확인 후 Align , Division Guide Back Ward.
		stepGuideBackChk,
		stepZUpChk, // Guide Backward 확인 후 Z Up.
		//stepYPosChk,    //2017-12-02,SKCHO, STEP 추가
		stepEnd,// Assign 보고 판정 여부 추가 2017/04/27 HSK.
/*
		stepYMove, // Z Up 확인 후 Y Put Pos로 이동.
		stepEnd, // Y Put Pos 확인 후 End.
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
		stepStart, // Top Tray 감지 Sensor 확인 및 Tray 없음 확인 하고 Division Guide , Align Guide BackWard.
		stepPitchUp, // Align, Division BWD 확인 하고 Lift Pitch Up.
		stepPitchUpChk, // Align, Division BWD 확인 하고 Lift Pitch Up.
		stepDivision, // Top Tray Sensor 감지 하면 Division Guide Forward
		stepAlignForward, // Lift Pitch Down 확인 후 Align Guide Forward해서 Tray 잡는다.
		stepEnd, // Align Guide Forward및 Tray Sensor 확인 후 End.
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
		stepStart, // TR에 Tray 있음 확인 후 Y 축 Out Port 이동 확인 후 아닐 경우 이동
		stepZDown, // Y축 Out 확인 후 Z Down.
		stepVacOff, // Z down 확인 후  Vac Off.
		stepZUp, // Vac Off 확인 후 Z Up.
		stepOutZDown,
		stepEnd, // Z Up 확인 후 End.
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
		stepStart, // 지정된 Tray 단수가 쌓였음 확인 하고 Lift Down Pos로 이동 후 Stopper Up.
		stepZAlignDown,		
		stepGuideFwd,
		stepGuideBwd,
		stepZDown,
		stepConvMove, // Lift Down Pos 확인 하고 Conv1 , Conv2 Move
		stepLiftUp, // Conv2에 Tray 옴겨짐 확인 하고 Conv Stop, Stopper Down 후  Lift Up Pos로 이동.
		stepLiftUpChk,
		stepStopperDown,
		stepEnd, // LiftUp 확인 하고 End.
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
		stepStart, // AGV Ready 체크 후 언로딩 요청
		stepConnectChk,
		stepTransferChk,// 언로딩 요청 받은 Transfer Req Chk 후 Conv 구동 명령
		stepBusyChk,	// AGV Busy Chk 후 Conv Sen Chk 후 Comp On
		stepEnd, // // AGV Comp 확인 후 AGV 신호 All Off
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
		stepStart, // AGV Ready 체크 후 로딩 요청, Buffer Stopper Down, In Stopper UP.
		stepTransferChk,// 로딩 요청 받은 Transfer Req Chk 후 Conv 구동 명령
		stepBusyChk,	// AGV Busy Chk 후 Conv Sen Chk 후 Comp On, Stopper Down
		stepEnd, // // AGV Comp 확인 후 AGV 신호 All Off
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
		stepStart, // AGV Ready 체크 후 로딩 요청
		stepEnd, // // AGV Comp 확인 후 AGV 신호 All Off

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
		stepStart, // Conv1에 Tray 확인 하고 Lift Teaching Pos가 Tray 투입해도 되는 구간이면 Conv 1 , Conv 2 굴림.
		stepCylDownChk, 
		stepBTMTrayUP,
		stepBTMFwd,
		stepBTMBwd,
		stepEnd, // Lift 구간에 Tray 들어옴 감지 하고 Conv 1,2 Stop.
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
		stepStart, // Lift에 Tray 없음 감지 하고 , Tray가 Conv2 구간에 들어와 있는지 확인 하고 Lift, Guide BackWard. Tray 있을경우 Step Idle.
		stepLiftMid, // Lift Mid Pos 이동
		stepLiftMidChk, // Lift Mid Pos Check
		stepLiftDown, // Lift Guide Backward 확인 하고 Lift 다운, Tray Up.
		stepGuideForward, // 다운 Pos, Tray Up 확인 하고 Guide Forward해서 트레이 잡아줌.
		stepLiftUp, //Guide Forward 확인 후 Lift Up.
		stepEnd, // Lower Guide Backward 확인 후 End.
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
		stepStart, //Top Tray 없음 및 Top Tray Guide 및 Division Back 확인 하고 Lift Pitch Up.
		stepPitchUp,
		stepPitchUpChk,
		stepLiftStop, // Top Tray 감지하고 Lift Stop 확인 , Division Forward.
		stepGuideForward, // Pitch Down 확인 하고 Tray Guide Forward.
		stepEnd, // Guide Forward 확인 하고 End.
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
		stepStart, //Top Tray Cell 가득참 확인 하고 X Get Pos로 이동.
		stepZDown, // X Get Pos 확인 후 Z Down and Vac On.
		stepGuideBackward, // Z Down 및 Vac On 확인 후 Division, Tray Guide Back.
		stepZUp, // Guide Back 확인 후 Z Up.
		stepZUpCheck,
		stepXMove, // Z Up 확인 후 X Put Pos 이동
		stepEnd, // X Put Pos 확인 후 End
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
		stepStart, //Tray TR에 Tray 있는지 X Put Pos인지 확인 및 Put Pos가 아닐 경우 Put Pos로 이동. 
		stepZDown, //X Put Pos 확인 및 Z Down.
		stepVacOff, // Z Down 확인 후 Vac Off.
		stepZUp, // Vac Off 확인 후 Z Up and OutLift Pitch Down.
		stepEnd, // Z Up 확인 및 Pitch down 확인 후 End.
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
		stepStart, // 지정된 Tray 단수가 쌓였음 확인 하고 Lift Down Pos로 이동.
		stepLiftDownChk, // Lift Down Pos 확인 후  Lift arm Bwd.
		stepLiftUp,
		stepUpChk,
		stepEnd, // Lift Arm Bwd, Conv Tray Sensor 확인 하고 End.
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
		stepStart, // AGV Ready 체크 후 언로딩 요청
		stepTransferChk,// 언로딩 요청 받은 Transfer Req Chk 후 Conv 구동 명령
		stepBusyChk,	// AGV Busy Chk 후 Conv Sen Chk 후 Comp On
		stepEnd, // // AGV Comp 확인 후 AGV 신호 All Off
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
		stepStart, // AGV Ready 체크 후 로딩 요청
		stepBuffSenChk,	
		stepBuffSenChk1,
		stepEnd, // // AGV Comp 확인 후 AGV 신호 All Off
	};

	int Run();
	BOOL IsWaiting()	{ return m_nStep < stepEnd ? TRUE:FALSE; };
};