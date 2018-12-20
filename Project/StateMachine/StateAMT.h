#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateAMTTable : 	public CUnitCtrlBank
{
public:
	CStateAMTTable(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateAMTTable(void) {};

	enum
	{
		stepStart, // ���̺� �̵� �� ���Ͷ� üũ �� �� �� SetZone �� Turn
//		stepTableMoveChk, // ���̺� Turn üũ
//		stepWaitZoneSet, // �� Zone ��ȣ�� Set
		stepEnd, // ���̺� Turn üũ �� End.
	};

	int Run();
};


class CStateAZone : 	public CUnitCtrlBank
{
public:
	CStateAZone(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateAZone(void) {};

	//20170201 byskcho
	//cell info request �߰�
	/*enum
	{
		stepStart,
		stepCh1TrackInChk,
		stepCh2TrackIn,
		stepCh2TrackInChk,
		stepContactChk, // Contact üũ �� A Zone���� ���ϴ� �˻� ����.(Sleep Current?)
		stepEnd,  // �˻� ���� Check �� End.
	};*/

	enum
	{
		stepStart = 0,
		//stepCh1CellRequest,
		stepCh1TrackIn,
		stepCh1TrackInChk,
		//stepCh2CellRequest,
		stepCh2TrackIn,
		stepCh2TrackInChk,
		stepContactChk, // Contact üũ �� A Zone���� ���ϴ� �˻� ����.(Sleep Current?)
		stepContactRetry,
		// ���� �����̰� �ʿ����. üũ �ϴ� �ð��� �����̴�. 17/04/25 HSK.
		//stepContactDelay,
		stepEnd,  // �˻� ���� Check �� End.
	};
	BOOL m_bTrackIn;

	int Run();

};


class CStateBZone : 	public CUnitCtrlBank
{
public:
	CStateBZone(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateBZone(void) {};

	enum
	{
		stepStart, // 
		stepContactChk, // 
		stepContactRetry,
		stepAlignMove, // VT Align �ؾ��ϴ� Cell ���� Ȯ�� �� Align ���� Ch1 Left ��ũ ��ġ�� �̵�
		stepCh1LeftPosChk, // ���� �̵� üũ �� Ch1 Left ���� ����� ��û.
		stepCh1LeftPosStable, // ���� ����ȭ.
		stepCh1LeftAlignChk, // ����� ���� ���� Ȯ�� �� Ch1 Right ��ũ �̵�.
		stepCh1RightPosChk, // ���� �̵� üũ �� Ch1 Right ���� ����� ��û
		stepCh1RightPosStable, // ���� ����ȭ.
		stepCh1RightAlignChk, // ����� ���� ���� Ȯ�� �� Ch2 Left ���� ����� ��ġ�� �̵�
		stepCh2LeftPosChk, // ���� �̵� üũ �� Ch2 Left ���� ����� ��û.
		stepCh2LeftPosStable, // ���� ����ȭ.
		stepCh2LeftAlignChk, // ����� ���� ���� Ȯ�� �� Ch2 Right ��ũ �̵�.
		stepCh2RightPosChk, // ���� �̵� üũ �� Ch2 Right ���� ����� ��û
		stepCh2RightPosStable, // ���� ����ȭ.
		stepCh2RightAlignChk,  // ���� �̵� üũ �� Ch2 Right ���� ����� ��û
		stepContactEnd,
		stepEnd,  // ��� ��ġ �̵� Ȯ�� �� End.
	};

	BOOL m_bCh1Exist;
	BOOL m_bCh2Exist;

	int Run();

};

class CStateCZone : 	public CUnitCtrlBank
{
public:
	CStateCZone(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateCZone(void) {};

	enum
	{
		stepStart, // ���� ���� �� �˻� ��ġ Ȯ�� �� ���� �� �̵�.
		stepPosChk, // ���� ���� �� �˻� ��ġ Ȯ�� �� Theta Offset �� �̵�.
		stepOffsetPosChk, // B Zone���� ���� Offset �� �̵� Ȯ�� �� �׷� ��ŸƮ ����
		stepBeginPosMove, // Grab End üũ �� Turn ���� �⺻ ��ġ�� �̵�.
		stepEnd, // Turn ���� �⺻��ġ �̵� üũ
	};

	int Run();
};

class CStateDZone : 	public CUnitCtrlBank
{
public:
	CStateDZone(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateDZone(void) {};

	enum
	{
		stepStart,
		stepClassEndChk, // ���� Class End Check �� Contact Off �� ���ϴ� �˻� ���� (White Current?)
		stepFileChk, // ���� Class End Check �� Contact Off �� ���ϴ� �˻� ���� (White Current?)
		stepEnd, // Contact Off Ȯ�� �� End ( Cell data ����� Unloader TR�� ���� ���� ���.)
	};

	int Run();

};
//2017-09-26,SKCHO, T5 RESTART ���� �Լ� 8�� ����
class CStateCH1_T5ReStart : 	public CUnitCtrlBank
{
public:
	CStateCH1_T5ReStart(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateCH1_T5ReStart(void) {};

	enum
	{
		stepStart,
		stepReSetAckChk, // ���� Class End Check �� Contact Off �� ���ϴ� �˻� ���� (White Current?)
		stepChk, // ���� Class End Check �� Contact Off �� ���ϴ� �˻� ���� (White Current?)
		stepEnd, // Contact Off Ȯ�� �� End ( Cell data ����� Unloader TR�� ���� ���� ���.)
	};

	int Run();

};
