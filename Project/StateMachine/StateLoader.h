#pragma once
#include "BankGuild/UnitCtrlBank.h"


class CStateConvMove : 	public CUnitCtrlBank
{
public:
	CStateConvMove(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateConvMove(void) {};

	enum
	{
		stepStart, 
		stepEnd, 
	};

	int Run();

};

class CStateManualReceive : 	public CUnitCtrlBank
{
public:
	CStateManualReceive(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateManualReceive(void) {};

	enum
	{
		stepStart, 
		stepEnd, 
	};

	int Run();

};

class CStateCellReceive : 	public CUnitCtrlBank
{
public:
	CStateCellReceive(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateCellReceive(void) {};

	enum
	{
		stepStart, // Cell 받을 수 있는 준비 I/O On
		stepConvMove, // 앞 설비 Conv Move I/O On Check 후 Loader Conv Move
		stepStartChk, // 앞설비 Start 체크
		stepSenChk, // Sensor 3 chk Stop
		stepCompChk, // Comp Chk
		stepEnd, // Edge Align 요청 후 End
	};

	int Run();
	BOOL IsWaiting()	{ return m_nStep < stepConvMove ? TRUE:FALSE; };
};

class CStateLDRobotGet : 	public CUnitCtrlBank
{
public:
	CStateLDRobotGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateLDRobotGet(void) {};

	enum
	{
		stepStart, // Edge Align 확인 후 Loading Pos 준비 완료
		stepAlignChk,
		stepViCompChk,	// Align Data Send
		stepDataSendDelay,
		stepRobotGet,	// Align Data Send
		stepVacOn, // Loading Position Check 후 Vac On
		stepVacOnChk, // Vac On Chk
		stepRobotGetComp,
		stepEnd, // 대기 위치 확인 후 End
		stepBitAllOff,
		stepRobotRunningChk,
		stepRobotPauseOn,
		seqRobotExtStop,
		seqRobotMotorOn,
		stepRobotOnChk,
		stepPGMSel,
		stepPGMStrobe,
		stepExtStart,
		stepRunningChk,
		stepOrgOff,
		stepInterlockChk,
	};

	int Run();

};

class CStateUpperShuttleGet : 	public CUnitCtrlBank
{
public:
	CStateUpperShuttleGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUpperShuttleGet(void) {};

	enum
	{
		stepStart, // Cell Check 후 Cell 없고 Robot Put 상태가 아닐때 Get Pos Move
		stepEnd, //	Get Pos Check 후 End
	};

	int Run();

};

class CStateLowerShuttleGet : 	public CUnitCtrlBank
{
public:
	CStateLowerShuttleGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateLowerShuttleGet(void) {};

	enum
	{
		stepStart, // Cell Check 후 Cell 없을 시 Get Pos Move
		stepEnd, //Get Pos Check 후 End
	};

	int Run();

};

class CStateLDRobotUpperPut : 	public CUnitCtrlBank
{
public:
	CStateLDRobotUpperPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateLDRobotUpperPut(void) {};

	enum
	{
		stepStart, // Robot Cell Check 및 Shuttle Cell Check 후 해당 포지션 Move
		stepPutPermit, ////로봇 Put위치 이동
		stepPutChk,
		stepVacOn, // Upper Stage Vac On
		stepRobotVacOff, // Robot Vac Off
		stepVacChk,	// Stage Robot Vac Chk
		stepPutCompChk,
		stepEnd, //Wait Pos Check 후 End
	};
	BOOL m_bCh1Unload;

	int Run();
};

class CStateLDRobotLowerPut : 	public CUnitCtrlBank
{
public:
	CStateLDRobotLowerPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateLDRobotLowerPut(void) {};

	enum
	{
		stepStart, // Robot Cell Check 및 Shuttle Cell Check 후 해당 포지션 Move
		stepPutPermit, ////로봇 Put위치 이동
		stepPutChk,
		stepVacOn, // Lower Stage Vac On
		stepRobotVacOff, // Robot Vac Off
		stepVacChk,	// Stage Robot Vac Chk
		stepPutCompChk,
		stepEnd, //Wait Pos Check 후 End
	};
	BOOL m_bCh1Unload;

	int Run();
};

class CStateUpperShuttlePut : 	public CUnitCtrlBank
{
public:
	CStateUpperShuttlePut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUpperShuttlePut(void) {};

	enum
	{
		stepStart, // Cell Check 후 Cell 있고 PA TR Get 상태가 아닐때 Put Pos Move
		stepEnd,	//Put Pos Check 후 End
	};


	int Run();

};

class CStateLowerShuttlePut : 	public CUnitCtrlBank
{
public:
	CStateLowerShuttlePut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateLowerShuttlePut(void) {};

	enum
	{
		stepStart, // Cell Check 후 Cell 있고 PA TR Get 상태가 아닐때 Put Pos Move
		stepEnd, //	Put Pos Check 후 End
	};

	int Run();

};

class CStatePATRUpperGet : 	public CUnitCtrlBank
{
public:
	CStatePATRUpperGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStatePATRUpperGet(void) {};

	enum
	{
		stepStart, // Shuttle Cell 유무 확인 후 PA TR Y Get 위치 이동
		stepGet, // Y Get 위치 확인 후 Z Get 위치 이동 및 Vac On
		stepGetCheck, // Z Get 위치 확인 및 Shuttle Vac Off
		stepZUp, // Z Up 위치 이동.
		stepEnd, // Y Put Pos 체크 후 End
	};
	BOOL	bExistCh1Cell;
	BOOL	bExistCh2Cell;

	int Run();
};

class CStatePATRLowerGet : 	public CUnitCtrlBank
{
public:
	CStatePATRLowerGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStatePATRLowerGet(void) {};

	enum
	{
		stepStart, // Shuttle Cell 유무 확인 후 PA TR Y Get 위치 이동
		stepGet, // Y Get 위치 확인 후 Z Get 위치 이동 및 Vac On
		stepGetCheck, // Z Get 위치 확인 및 Shuttle Vac Off
		stepZUp, // Z Up 위치 이동.
		stepEnd, // Y Put Pos 체크 후 End
	};
	BOOL	bExistCh1Cell;
	BOOL	bExistCh2Cell;

	int Run();
};


class CStatePAStageGet : 	public CUnitCtrlBank
{
public:
	CStatePAStageGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStatePAStageGet(void) {};

	enum
	{
		stepStart, // PA Stage Cell Check 후 Cell 없을 시 PA Stage Get Pos 이동 및 PA Stage X,Y,T 검사 위치 이동.
		stepEnd, // PA Stage Get 위치 및 PA Stage X,Y,T 검사위치 확인 후 End
	};

	int Run();
};

class CStatePATRPut : 	public CUnitCtrlBank
{
public:
	CStatePATRPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStatePATRPut(void) {};

	enum
	{
		stepStart, // PA TR Z Up 위치 확인 및 Vac Check 확인 후 Y Put 위치 이동
		stepPut, // Y Put 위치 확인 후 Z Put 위치 이동
		stepVacOn, // // Z Put 위치 확인 후 Vac On
		stepZUp, //  Vac 확인 및 Z Up.
		stepGet,		//  Z Up 확인 및 Y Get 위치 이동.
		stepEnd, // Get위치 확인 후 End
	};
	BOOL	bExistCh1Cell;
	BOOL	bExistCh2Cell;

	int Run();

};

class CStatePAStage : 	public CUnitCtrlBank
{
public:
	CStatePAStage(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStatePAStage(void) {};

	enum
	{
		stepStart, // PA 모터 위치 체크 후 Prealign 신호 송부.
		stepPAStageAlignMoveChk,
		stepThAlignChk, // Th Prealign 성공 여부 확인 후 PA TH Move.
		stepThMoveChk, // PA Th Move 확인 후 XY Prealign 신호 송부.
		stepPAMove, // XY Prealign 성공 여부 확인 후 PA Stage X,Y, Stage Move.
		stepEnd, // PA X,Y, Stage Move 확인 후 End
	};

	BOOL	bExistCh1Cell;
	BOOL	bExistCh2Cell;

	int Run();

};


class CStatePAStagePut : 	public CUnitCtrlBank
{
public:
	CStatePAStagePut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStatePAStagePut(void) {};

	enum
	{
		stepStart, // PA Stage Cell Check 및 Align 결과 체크 후 Put Pos 이동
		stepEnd, // PA Stage Put 위치 확인 후 End
	};

	int Run();
};

class CStateLDTRGet : 	public CUnitCtrlBank
{
public:
	CStateLDTRGet(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateLDTRGet(void) {};

	enum
	{
		stepStart, 		
		stepZDown, // LD TR X 위치 확인 후  Z Down.
		stepVacOn, // LD TR Vac On 및 PA Stage Vac Off.
		stepZUp,	// LD TR 및 PA Stage Vac 확인 후 Z Up
		stepEnd, // LD TR Z Up Pos 확인 후 End
	};
	BOOL	bExistCh1Cell;
	BOOL	bExistCh2Cell;

	int Run();

};

class CStateLowerMCR : 	public CUnitCtrlBank
{
public:
	CStateLowerMCR(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateLowerMCR(void) {};

	enum
	{
		stepStart, // Z MCR 위치 이동.
		stepZMoveChk,	// Z Lower MCR 위치 확인 및 Cell 유무 확인.
		stepCh1Move, // X Lower MCR Ch1 위치로 이동.
		stepCh1Stable,
		stepCh1Reading, // LD TR X LEFT MCR Pos Chk 후 MCR Reading.
		stepCh1ReadingChk, // MCR Reading Chk
		stepCh1CellRequest,
		stepCh1CellRequestChk,
		stepCh2Move, // X Lower MCR Ch2 위치로 이동.
		stepCh2Stable,
		stepCh2Reading, // Right MCR Reading
		stepCh2ReadingChk, // MCR Reading 확인
		stepCh2CellRequest,
		stepCh2CellRequestChk,
		stepZUpPos,	// Z Up Pos Chk 후 X Put Pos 이동
		stepXPutPos,	// Z Up Pos Chk 후 X Put Pos 이동
		stepEnd,	// Put Pos 확인 후 End
	};
	MCR_JUDGE	bCh1ReadingChk;
	MCR_JUDGE	bCh2ReadingChk;

	int Run();

};


class CStateLDTRPut : 	public CUnitCtrlBank
{
public:
	CStateLDTRPut(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateLDTRPut(void) {};

	enum
	{
		stepStart, // A Zone Cell 유무 확인 후 TR X Move
		stepZOffsetDown, // TR X 위치 확인 후 Z Down
		stepBlowOn,		// Offset Check 후 Z Down 및 Blow On
		stepContact, // TR Vac Off , Table Vac On 및 Contact.
		stepZUp, // Vac 상태 및 Contact 확인 후 TR Z Up.
		stepZUpChk,
		stepBlowerOff, // 2017-09-02,SKCHO BLOWROFF 시퀀스 추가
		stepXMove,
		stepXMoveChk,
		stepEnd,	// TR Z Up Check 후 MCR 성공여부 확인 후 둘다 성공시 Wait 위치로 실패시 MCR 위치로 이동 후 End
	};
	BOOL	bExistCh1Cell;
	BOOL	bExistCh2Cell;

	int Run();

};

class CStateUpperMCR : 	public CUnitCtrlBank
{
public:
	CStateUpperMCR(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateUpperMCR(void) {};

	enum
	{
		stepStart, // 실패한 MCR 위치 파악
		stepZChk,	// Z UpperPos Chk
		stepCh1Move, // Ch1 MCR 위치 이동
		stepCh1Stable,
		stepCh1Reading, // 1CH MCR 위치 Check 후 MCR Reading 시도.
		stepCh1ReadingChk, // 1CH MCR Reading 체크 후 2CH MCR 위치 이동.
		stepCh1CellRequest,
		stepCh1CellRequestChk,
		stepCh2Move,		// 2CH MCR 위치 이동
		stepCh2Stable,
		stepCh2Reading, // 2CH MCR 위치 Check 후 MCR Reading 시도
		stepCh2ReadingChk, // 2CH MCR Reading 체크
		stepCh2CellRequest,
		stepCh2CellRequestChk,
		stepXGetMove, // X GetPos Move
		stepEnd, // 2CH MCR Reading 체크 후 LD TR X 로딩 위치로 이동.
	};

	MCR_JUDGE bCh1ReadingChk;
	MCR_JUDGE bCh2ReadingChk;

	int Run();

};