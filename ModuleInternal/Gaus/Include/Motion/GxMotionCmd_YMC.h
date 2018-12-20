/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CGxMotionCmd_YMC : Yaskawa MP2100 Control class
//
// 2015-05-28, jhLee, Gaus2용 라이브러리 재작성
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <afxmt.h>												// CCriticalSection 사용

#include "Motion\GxMotionCmd.h"

#if defined(_ORIGIN_SOURCE) || defined(_DLL_EXPORTS)
	#include "Motion\MP2100\ymcPCAPI.h"
	#pragma comment(lib, "ymcPCAPI.lib")
#else
	#include "Motion\DLL_Include\ymcAPIStruct.h"				// 각종 Structure 들만 참조한다.
#endif


#define YMC_AXIS_COUNT			32			// 최대 축수
#define YMC_HOME_METHOD_COUNT	20			// 원점찾기 방법 수량 : 0 ~ 19까지 총 20가지 방법


// MP2100 제어를 위한 기본적인 핸들, 각 Thread별로 독립적으로 갖게된다.
typedef struct _GAUS_LIB _TGxYMCCmdHandle
{
	int			nBoardID;					// 보드 번호
	BOOL			bOpenFlag;					// 보드 초기화를 마쳤는가 ?

	int			nLastErrorAxis;				// 최적적으로 오류가 발생시 수행된 축
	DWORD			dwLastErrorCode;			// 최종적으로 오류가 발생시 수행된 명령의 결과

	HCONTROLLER		hController;				// 컨트롤러 핸들
	HAXIS				hAxis[YMC_AXIS_COUNT];		// 각 축의 핸들
	HDEVICE			hDevice[YMC_AXIS_COUNT];	// 각 축의 디바이스, 실질적으로 사용할 핸들
	BOOL				bAxisFlag[YMC_AXIS_COUNT];	// 해당 축이 정상적으로 Opne되었는지 여부,


	// TGxMotionSpeedBlock	rAxisData[YMC_AXIS_COUNT];	// 각 축이 가지는 고유의 값, 속도등 

	// 변수 초기화
	_TGxYMCCmdHandle() 
	{
		nBoardID		= 0;
		bOpenFlag		= FALSE;
		nLastErrorAxis	= 0;
		dwLastErrorCode	= 0;

		hController		= NULL;

		for(int i = 0; i < YMC_AXIS_COUNT; i++)
		{
			hAxis[i]		= NULL;
			hDevice[i]		= NULL;
			bAxisFlag[i]	= FALSE;
		}
	};

} TGxYMCCmdHandle, *TPGxYMCCmdHandle;

typedef CArray<TGxYMCCmdHandle, TGxYMCCmdHandle&> TGxYMCCmdHandleArray;
typedef CTypedPtrArray<CPtrArray, TGxYMCCmdHandle*> TGxYMCCmdHandlePtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, TGxYMCCmdHandle*> TGxMapStringToYMCCmdHandle;
typedef CMap<DWORD, DWORD, TGxYMCCmdHandle*, TGxYMCCmdHandle*> TGxMapDWORDToYMCCmdHandle;					// TGxCmdRecord 괸리 Map



// GxMotionCmd_YMC 에서 사용되는 CmdRecord, 기존의 CmdRecord를 상속 받아서 MP2100에 해당되는 내용을 추가한다. 
typedef struct _GAUS_LIB _TGxYMCCmdRec : TGxCmdRecord
{
	DWORD						dwMainFrameID;				// MainFrame의 Thread ID, 최초의 초기화시 대입된다.
	TGxMapDWORDToYMCCmdHandle	mapThreadID;				// 각 Thread ID에 따른 YMC Handle map
	
	_TGxYMCCmdRec() : _TGxCmdRecord()
	{
		dwMainFrameID = NULL;
	}

	~_TGxYMCCmdRec()			// 소멸자, 내부에 생성된 map object를 삭제한다.
	{
		DWORD key = NULL;
		TGxYMCCmdHandle* pRec = NULL;
		POSITION pos = mapThreadID.GetStartPosition();
		
		while ( pos != NULL ) //  모든 Object를 순회하며 삭제를 실시한다.
		{
			mapThreadID.GetNextAssoc(pos, key, pRec);
			if ( pRec != NULL) delete pRec;
		}

		//// [ ] 연산자를 통한 단순한 방식으로 처리가 가능하다.
		//for (int i = 0; i < mapThreadID.GetCount(); i++)
		//{
		//	delete mapThreadID[i];
		//}
		mapThreadID.RemoveAll();
	};

} TGxYMCCmdRec, *TPGxYMCCmdRec;
typedef CArray<TGxYMCCmdRec, TGxYMCCmdRec&> TGxYMCCmdRecordArray;
typedef CTypedPtrArray<CPtrArray, TGxYMCCmdRec*> TGxYMCCmdRecordPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, TGxYMCCmdRec*> TGxMapStringToYMCCmdRecord;
typedef CMap<int, int, TGxYMCCmdRec*, TGxYMCCmdRec*> TGxMapIntToYMCCmdRecord;					// TGxCmdRecord 괸리 Map



// Motion에 대한 실제적인 API 사용을 담은 Class, CGxMotionCmd에서 파생되었으며 CGxAxis에서 사용되어 진다.
class _GAUS_LIB CGxMotionCmd_YMC : public CGxMotionCmd
{
public:
	CGxMotionCmd_YMC(void);
	~CGxMotionCmd_YMC(void);

	enum
	{
		eMAX_AXIS_CNT = YMC_AXIS_COUNT,
	};

// friend class CMotionMgr;

protected:
	CCriticalSection m_CSDeviceOpen;							// 다중 접근 방지

	// open Controller에서 Thread별 동시 실행 방지용 Mutex
	CMutex		m_Mutex;
	CMap <DWORD, DWORD, DWORD, DWORD&> m_mapUseThreadID;					// MP2100을 사용하는 Thread 수량 관리용
	void AddUseThreadID(DWORD dwID);										// 지정 Thead의 ID를 저장한다.


public :

	// TEST用
	void TestCall()	{ AfxMessageBox(_T("CGxMotionCmd_YMC is Called !") ); };



	// GxMotionCmd_YMC에서 새롭게 재정의된 멤버 변수 및 함수들
	TGxYMCCmdRecordPtrArray		m_arrCmdRecord;							// Cmd 정보


	int GetControlCmdHandleCount(int nIdx);								// 지정 순번의 MotionCmd 보드에 해당되는 핸들 객체(Thread별 생성)의 수량을 조회한다.
	TGxYMCCmdHandle* GetCmdHandleRecord(int nIdx);						// 지정 모션보드에서 현재 호출한 Thread ID를 사용하는 핸들 객체의 포인터를 조회한다.
	TGxYMCCmdHandle* GetCmdHandleRecord(int nIdx, DWORD dwID);			// 지정 모션보드에서 해당 Thread ID를 사용하는 핸들 객체의 포인터를 조회한다.

	void	PostDeviceOpen(int nCtrlIdx);								// 장치를 Open한 뒤에 각 Task (Thread) 마다 처리해야할 내용 수행
	int		GetUseThreadCount();										// 현재 MP2100을 사용하는 THread 수량



	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
	// CGxMotionCmd 에서 정의한 가상함수의 구현
	//
	void ReleaseInstance();								// 선언 되었던 각종 데이터들을 해제시켜준다. 소멸자 역할

	// 본 조회 함수는 원형이 TGxCmdRecord* 이지만 이곳에서는 이를 상속받은 TGxYMCCmdRec*로 변형하여 사용한다.
	//
	TGxYMCCmdRec* GetCmdRecord(int nIdx);								// 지정 순번의 Cmd Record 주소 획득
	TGxYMCCmdRec* GetCmdRecordByName(CString sName);					// 지정 이름을 가지는 Cmd Record 주소 획득

	int AddControlCmd(TGxCmdRecord &rNewCmd);							// 새로운 보드를 추가한다.	GxMotionCmdMgr에서 호출
	int GetControlCmdCount();											// 지정된 Control Cmd의 수량을 되돌려준다.
//?	void SetSyncAxisData();

	// 제어기 열기와 닫기
	BOOL	OpenDevice();												// 등록된 모든 제어기 열기
	BOOL	CloseDevice();												// 등록된 모든 제어기 닫기


	BOOL	OpenDevice(int nCtrlIdx);									// 장치를 활성화하고 사용가능하게 만든다.
	BOOL	CloseDevice(int nCtrlIdx);									// 장치를 닫고 더이상 사용하지 않는다.


	// 현재 위치 값 조회
	long	PosToPulse(double dPos, double dGearRatio);
	double	PulseToPos(long lPulse, double dGearRatio);

	// Torque값을 조회한다.
	double GetTorque(int nCtrlIdx, int nAxisNo);

	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


	//--- Check Function
	BOOL	IsCheckBoard(int nCtrlIdx);									// 지정 모션 보드가 정상적인가 ?
	BOOL	IsOpen(int nCtrlIdx);
	BOOL	IsServoOn(int nCtrlIdx, int nAxisNo);						// Servo On 출력 여부
	BOOL	IsCurrentOn(int nCtrlIdx, int nAxisNo);						// Step motor Amp On 출력 여부
	BOOL	IsServoReady(int nCtrlIdx, int nAxisNo);					// Servo가 동작 가능 상태인가 ?
	BOOL	IsAlarm(int nCtrlIdx, int nAxisNo);							// Alarm 발생여부
	BOOL	IsInpos(int nCtrlIdx, int nAxisNo);							// 축 정지 여부
	BOOL	IsBusy(int nCtrlIdx, int nAxisNo);							// 명령 지령중 여부
	BOOL	IsHomeSearch(int nCtrlIdx, int nAxisNo);					// 원점 찾기 완료 여부
	BOOL	IsHomeSearchMoving(int nCtrlIdx, int nAxisNo);				// 원점 찾기 수행중인가 ?
	BOOL	IsReady(int nCtrlIdx, int nAxisNo);							// 이동이 가능한지 여부
	BOOL	IsPLimit(int nCtrlIdx, int nAxisNo);						// Positive 센서 감지 여부
	BOOL	IsNLimit(int nCtrlIdx, int nAxisNo);						// Negative 센서 감지 여부
	BOOL	IsOrg(int nCtrlIdx, int nAxisNo);							// 원점(Near)센서 감지 여부


	BOOL	SetGearRatio(int nCtrlIdx, int nAxisNo, double dbRatio);	// 기어비 설정
	BOOL	GetCmdPos(int nCtrlIdx, int nAxisNo, double& dbPos);		// 지령 위치값 조회
	BOOL	GetEncPos(int nCtrlIdx, int nAxisNo, double& dbPos);		// 엔코더 위치값 조회
	BOOL	SetCmdPos(int nCtrlIdx, int nAxisNo, double dbNewPos);		// Command position 값을 지정한다.
	BOOL	SetEncPos(int nCtrlIdx, int nAxisNo, double dbNewPos);		// Encoder position 값을 지정한다.
	BOOL	SetPosition(int nCtrlIdx, int nAxisNo, double dbPos);		// 위치값 적용
	BOOL	SetSyncToEncPos(int nCtrlIdx, int nAxisNo);					// 엔코더 값으로 Command position을 일치화 시킨다.

	//--- ServoAmp Control
	BOOL	ServoON(int nCtrlIdx, int nAxisNo);							// Servo On 지령
	BOOL	ServoOFF(int nCtrlIdx, int nAxisNo);						// Servo Off 지령
	BOOL	CurrentON(int nCtrlIdx, int nAxisNo);						// Step motor Amp의 Power On 지령
	BOOL	CurrentOFF(int nCtrlIdx, int nAxisNo);						// Step motor Amp의 Power Off 지령
	BOOL	AlarmClearON(int nCtrlIdx, int nAxisNo);					// Servo AlarmClear 신호 ON
	BOOL	AlarmClearOFF(int nCtrlIdx, int nAxisNo);					// Servo AlarmClear 신호 OFF


	//--- 이동 및 정지 명령
	BOOL	SetHomeSearchSpeed(int nCtrlIdx, int nAxisNo, double dbHigh, double dbLow);							// 원점 이동속도 지정
	BOOL	MoveHomeSearch(int nCtrlIdx, int nAxisNo, int nHomeMode, int nEndMode, double dbOffset);			// 원점 이동 명령

	BOOL	SetJogMoveSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbDrive);		// Jog이동 속도 설정 (사다리꼴)
	BOOL	SetMoveSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbDrive);		// 이동 속도 설정 (사다리꼴)
	BOOL	SetMoveSCurveSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbDrive);	// 이동 속도 설정 (S-커브)

	BOOL	MoveAbs(int nCtrlIdx, int nAxisNo, double dbPos);			// 지정 위치로 절대좌표 이동을 지령한다.
	BOOL	MoveRel(int nCtrlIdx, int nAxisNo, double dbPos);			// 지정 위치로 상대좌표 이동을 지령한다.
	BOOL	MoveJog(int nCtrlIdx, int nAxisNo, int nDir);				// 지정된 방향으로 JOG 이동을 지령한다.

	BOOL	MoveStop(int nCtrlIdx, int nAxisNo);						// 이동을 정지 시킨다. (감속 정지)
	BOOL	MoveJogStop(int nCtrlIdx, int nAxisNo);						// Jog 이동을 정지 시킨다.
	BOOL	MoveEmgStop(int nCtrlIdx, int nAxisNo);						// 이동을 비상 정지 시킨다.

	//검토 BOOL	MoveStopAll(int nCtrlIdx);									// 모든 축을 감속 정지 시킨다.
	//검토 BOOL	MoveEmgStopAll(int nCtrlIdx);								// 모든 축을 비상정지 시킨다.

};

