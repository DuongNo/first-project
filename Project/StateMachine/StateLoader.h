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
		stepStart, // Cell ���� �� �ִ� �غ� I/O On
		stepConvMove, // �� ���� Conv Move I/O On Check �� Loader Conv Move
		stepStartChk, // �ռ��� Start üũ
		stepSenChk, // Sensor 3 chk Stop
		stepCompChk, // Comp Chk
		stepEnd, // Edge Align ��û �� End
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
		stepStart, // Edge Align Ȯ�� �� Loading Pos �غ� �Ϸ�
		stepAlignChk,
		stepViCompChk,	// Align Data Send
		stepDataSendDelay,
		stepRobotGet,	// Align Data Send
		stepVacOn, // Loading Position Check �� Vac On
		stepVacOnChk, // Vac On Chk
		stepRobotGetComp,
		stepEnd, // ��� ��ġ Ȯ�� �� End
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
		stepStart, // Cell Check �� Cell ���� Robot Put ���°� �ƴҶ� Get Pos Move
		stepEnd, //	Get Pos Check �� End
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
		stepStart, // Cell Check �� Cell ���� �� Get Pos Move
		stepEnd, //Get Pos Check �� End
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
		stepStart, // Robot Cell Check �� Shuttle Cell Check �� �ش� ������ Move
		stepPutPermit, ////�κ� Put��ġ �̵�
		stepPutChk,
		stepVacOn, // Upper Stage Vac On
		stepRobotVacOff, // Robot Vac Off
		stepVacChk,	// Stage Robot Vac Chk
		stepPutCompChk,
		stepEnd, //Wait Pos Check �� End
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
		stepStart, // Robot Cell Check �� Shuttle Cell Check �� �ش� ������ Move
		stepPutPermit, ////�κ� Put��ġ �̵�
		stepPutChk,
		stepVacOn, // Lower Stage Vac On
		stepRobotVacOff, // Robot Vac Off
		stepVacChk,	// Stage Robot Vac Chk
		stepPutCompChk,
		stepEnd, //Wait Pos Check �� End
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
		stepStart, // Cell Check �� Cell �ְ� PA TR Get ���°� �ƴҶ� Put Pos Move
		stepEnd,	//Put Pos Check �� End
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
		stepStart, // Cell Check �� Cell �ְ� PA TR Get ���°� �ƴҶ� Put Pos Move
		stepEnd, //	Put Pos Check �� End
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
		stepStart, // Shuttle Cell ���� Ȯ�� �� PA TR Y Get ��ġ �̵�
		stepGet, // Y Get ��ġ Ȯ�� �� Z Get ��ġ �̵� �� Vac On
		stepGetCheck, // Z Get ��ġ Ȯ�� �� Shuttle Vac Off
		stepZUp, // Z Up ��ġ �̵�.
		stepEnd, // Y Put Pos üũ �� End
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
		stepStart, // Shuttle Cell ���� Ȯ�� �� PA TR Y Get ��ġ �̵�
		stepGet, // Y Get ��ġ Ȯ�� �� Z Get ��ġ �̵� �� Vac On
		stepGetCheck, // Z Get ��ġ Ȯ�� �� Shuttle Vac Off
		stepZUp, // Z Up ��ġ �̵�.
		stepEnd, // Y Put Pos üũ �� End
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
		stepStart, // PA Stage Cell Check �� Cell ���� �� PA Stage Get Pos �̵� �� PA Stage X,Y,T �˻� ��ġ �̵�.
		stepEnd, // PA Stage Get ��ġ �� PA Stage X,Y,T �˻���ġ Ȯ�� �� End
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
		stepStart, // PA TR Z Up ��ġ Ȯ�� �� Vac Check Ȯ�� �� Y Put ��ġ �̵�
		stepPut, // Y Put ��ġ Ȯ�� �� Z Put ��ġ �̵�
		stepVacOn, // // Z Put ��ġ Ȯ�� �� Vac On
		stepZUp, //  Vac Ȯ�� �� Z Up.
		stepGet,		//  Z Up Ȯ�� �� Y Get ��ġ �̵�.
		stepEnd, // Get��ġ Ȯ�� �� End
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
		stepStart, // PA ���� ��ġ üũ �� Prealign ��ȣ �ۺ�.
		stepPAStageAlignMoveChk,
		stepThAlignChk, // Th Prealign ���� ���� Ȯ�� �� PA TH Move.
		stepThMoveChk, // PA Th Move Ȯ�� �� XY Prealign ��ȣ �ۺ�.
		stepPAMove, // XY Prealign ���� ���� Ȯ�� �� PA Stage X,Y, Stage Move.
		stepEnd, // PA X,Y, Stage Move Ȯ�� �� End
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
		stepStart, // PA Stage Cell Check �� Align ��� üũ �� Put Pos �̵�
		stepEnd, // PA Stage Put ��ġ Ȯ�� �� End
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
		stepZDown, // LD TR X ��ġ Ȯ�� ��  Z Down.
		stepVacOn, // LD TR Vac On �� PA Stage Vac Off.
		stepZUp,	// LD TR �� PA Stage Vac Ȯ�� �� Z Up
		stepEnd, // LD TR Z Up Pos Ȯ�� �� End
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
		stepStart, // Z MCR ��ġ �̵�.
		stepZMoveChk,	// Z Lower MCR ��ġ Ȯ�� �� Cell ���� Ȯ��.
		stepCh1Move, // X Lower MCR Ch1 ��ġ�� �̵�.
		stepCh1Stable,
		stepCh1Reading, // LD TR X LEFT MCR Pos Chk �� MCR Reading.
		stepCh1ReadingChk, // MCR Reading Chk
		stepCh1CellRequest,
		stepCh1CellRequestChk,
		stepCh2Move, // X Lower MCR Ch2 ��ġ�� �̵�.
		stepCh2Stable,
		stepCh2Reading, // Right MCR Reading
		stepCh2ReadingChk, // MCR Reading Ȯ��
		stepCh2CellRequest,
		stepCh2CellRequestChk,
		stepZUpPos,	// Z Up Pos Chk �� X Put Pos �̵�
		stepXPutPos,	// Z Up Pos Chk �� X Put Pos �̵�
		stepEnd,	// Put Pos Ȯ�� �� End
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
		stepStart, // A Zone Cell ���� Ȯ�� �� TR X Move
		stepZOffsetDown, // TR X ��ġ Ȯ�� �� Z Down
		stepBlowOn,		// Offset Check �� Z Down �� Blow On
		stepContact, // TR Vac Off , Table Vac On �� Contact.
		stepZUp, // Vac ���� �� Contact Ȯ�� �� TR Z Up.
		stepZUpChk,
		stepBlowerOff, // 2017-09-02,SKCHO BLOWROFF ������ �߰�
		stepXMove,
		stepXMoveChk,
		stepEnd,	// TR Z Up Check �� MCR �������� Ȯ�� �� �Ѵ� ������ Wait ��ġ�� ���н� MCR ��ġ�� �̵� �� End
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
		stepStart, // ������ MCR ��ġ �ľ�
		stepZChk,	// Z UpperPos Chk
		stepCh1Move, // Ch1 MCR ��ġ �̵�
		stepCh1Stable,
		stepCh1Reading, // 1CH MCR ��ġ Check �� MCR Reading �õ�.
		stepCh1ReadingChk, // 1CH MCR Reading üũ �� 2CH MCR ��ġ �̵�.
		stepCh1CellRequest,
		stepCh1CellRequestChk,
		stepCh2Move,		// 2CH MCR ��ġ �̵�
		stepCh2Stable,
		stepCh2Reading, // 2CH MCR ��ġ Check �� MCR Reading �õ�
		stepCh2ReadingChk, // 2CH MCR Reading üũ
		stepCh2CellRequest,
		stepCh2CellRequestChk,
		stepXGetMove, // X GetPos Move
		stepEnd, // 2CH MCR Reading üũ �� LD TR X �ε� ��ġ�� �̵�.
	};

	MCR_JUDGE bCh1ReadingChk;
	MCR_JUDGE bCh2ReadingChk;

	int Run();

};