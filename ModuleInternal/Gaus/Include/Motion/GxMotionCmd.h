//
// CGxMotionCmd : Motion Control Base class, 모션 제어 명령의 기본이 되는 클래스
// 
// NMC2 혹은 MP2100도 본 클래스를 부모로 삼아서 파생 정의한다.
//
// 2014-12-21, jhLee
// 2015-06-04, jhLee, 각 제어기에 종속되는 부분을 분리하고 공통 부분을 추려내어 상속이 가능하도록 고려
//

#pragma once

#include "Gaus.h"
#include "Motion\GxMotion.h"						// Motion 관련 상수 및 구조체 선언


// 변환 비율 상수
#define _SEC_TO_MSEC	1000				// 초 -> 밀리초로 변환시 비율
#define _MSEC_TO_SEC	0.001				// 밀리 초 -> 초로 변환시 비율

#define _MM_TO_MICRO	1000				// 밀리 미터를 마이크로 미터로 변환시 비율
#define _MICRO_TO_MM	0.001				// 마이크로 미터를 밀리 미터로 변환시 비율

// 2015-06-29, jhLee, 경험치에 의해서 1 ~ 50의 수치는 동작 완료후에도 계속해서 펄스가 지령되어 Inpos가 늦게뜬다
//					  100정도의 수치를 주면 정지와 동시에 동작이 완료된다.
#define _START_VELOCITY	100					// Stepping Motor의 초기 속도를 지정한다. 0 초과의 값이 필요하다.



// 컨트롤별/축별 지정 데이터
typedef struct _GAUS_LIB _TGxMotionSpeedBlock				// 축별 속도 지정 데이터
{
	double		dbGearRatio;					// 기어비

	double		dbRunSpeed;						// 구동 속도
	double		dbJogSpeed;						// 조그 동작 속도
	double		dbAcc;							// 가속도
	double		dbDec;							// 감속도
	double		dbHomeHigh;						// 원점 복귀시 고속
	double		dbHomeLow;						// 원점 복귀시 저속

	_TGxMotionSpeedBlock()						// 생성시 초기화
	{	
		// 비율 및 속도의 초기값은 0 이 아닌 1.0으로 지정한다.
		dbGearRatio		= 1.0;					// 기어비

		dbRunSpeed		= 1.0;					// 구동 속도
		dbJogSpeed		= 1.0;					// 조그 동작 속도
		dbAcc			= 1.0;					// 가속도
		dbDec			= 1.0;					// 감속도
		dbHomeHigh		= 1.0;					// 원점 복귀시 고속
		dbHomeLow		= 1.0;					// 원점 복귀시 저속
	};

	void operator=(const _TGxMotionSpeedBlock &rSrc)			// 대입 연산자
    {
		dbGearRatio		= rSrc.dbGearRatio;		// 기어비

		dbRunSpeed		= rSrc.dbRunSpeed;		// 구동 속도
		dbJogSpeed		= rSrc.dbJogSpeed;		// 조그 동작 속도
		dbAcc				= rSrc.dbAcc;			// 가속도
		dbDec				= rSrc.dbDec;			// 감속도
		dbHomeHigh		= rSrc.dbHomeHigh;		// 원점 복귀시 고속
		dbHomeLow		= rSrc.dbHomeLow;		// 원점 복귀시 저속
	};

} TGxMotionSpeedBlock;



// GxMotionCmd의 데이터를 관리하는 구조체
// 각 Motion controller board 1장 1장 마다 1개의 Record를 가진다.
// 각 Motion Board의 특성이 다르다면 본 struct를 상속 받아서 추가로 기술하여 사용한다. (MP2100 에)
//
typedef struct _GAUS_LIB _TGxCmdRecord
{
	BOOL			bSimulate;					// H/W 연결없이 가상모드로 동작할 것인가 ?

	int			nCmdType;					// 어떠한 제어기 인가 ? 0:NMC2 제어기, 1: ? 추가예정
	int			nCmdIndex;					// 지정 Controller Type에서의 Index
	int			nCtrlID;					// 컨트롤러의 고우 ID 혹은 No (NMC는 Num, MP2100은 CPU번호)

	CString		sMyName;						// MotionCmd의 이름
	CString		sAddress;						// 각종 주소
	BOOL			bOpenFlag;					// 컨트롤러가 Open 되어있나 ?

	
	CString		sLastError;					// 최근에 발생한 오류내용
	int			nLastError;					// 최근에 발생한 오류코드

	TGxMotionSpeedBlock	rAxisData[eGxMOTION_MaxAxis];	// 각 축이 가지는 고유의 값, 속도등 설정값 저장

	_TGxCmdRecord()
	{
		bSimulate		= FALSE;			// H/W 연결없이 가상모드로 동작할 것인가 ?

		nCmdType		= 0;				// 어떠한 제어기 인가 ? 0:NMC2 제어기, 1: ? 추가예정
		nCmdIndex		= 0;				// 지정 Controller Type에서의 Index
		nCtrlID			= 0;				// 컨트롤러의 고우 ID 혹은 No (NMC는 Num, MP2100은 CPU번호)

		sMyName			= _T("");			// MotionCmd의 이름
		sAddress		= _T("");			// 각종 주소
		bOpenFlag		= FALSE;			// 컨트롤러가 Open 되어있나 ?

		sLastError		= _T("");			// 최근에 발생한 오류내용
		nLastError		= 0;				// 최근에 발생한 오류코드
	};

	//CString		m_sBasePath;					// 설정파일을 관리하는 기본 폴더
	//CGxIniFile	m_iniData;						// 정보를 담고 있는 ini 파일
} TGxCmdRecord, *TPGxCmdRecord;

typedef CArray<TGxCmdRecord, TGxCmdRecord&> TGxCmdRecordArray;
typedef CTypedPtrArray<CPtrArray, TGxCmdRecord*> TGxCmdRecordPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, TGxCmdRecord*> TGxMapStringToCmdRecord;
typedef CMap<int, int, TGxCmdRecord*, TGxCmdRecord*> TGxMapIntToCmdRecord;					// TGxCmdRecord 괸리 Map


// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


class _GAUS_LIB CGxMotionCmd
{
public:
	CGxMotionCmd(void);
	~CGxMotionCmd(void);

	// 아래 속성은 모션 제어기별로 유니크하다, 
	int			m_nCmdType;						// Motion Cmd의 Type
	CString		m_sCmdTypeName;					// Motion Cmd의 이름 

	
	// 각종 위치/속도 값 계산 함수
	long	UnitToPulse(double dPos, double dRatio);				// 좌표 표시 단위 (mm) -> Pulse 지령 단위로 
	double	PulseToUnit(long lPulse, double dRatio);				// Pulse 단위 -> 좌표 표시 단위 (mm)

	void	SetBasicSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbVel, double dbJog);	//기본적인 속도값 설정



	// 아래 이후는 각 Motion controller의 API에 맞게 재정의 하여 사용한다. == == == == == == == == == == == == == == == == == == == == == == == ==

	// 지원하지 않는 기능인 경우 빈 함수만 기술한다.
	virtual void ReleaseInstance() = 0;								// 선언 되었던 각종 데이터들을 해제시켜준다. 소멸자 역할

	virtual TGxCmdRecord* GetCmdRecord(int nIdx) = 0;				// 지정 순번의 Cmd Record 주소 획득
	virtual TGxCmdRecord* GetCmdRecordByName(CString sName) = 0;	// 지정 이름을 가지는 Cmd Record 주소 획득
	
	virtual int AddControlCmd(TGxCmdRecord &rNewCmd) = 0;			// 새로운 보드를 추가한다.	GxMotionCmdMgr에서 호출
	virtual int GetControlCmdCount() = 0;							// 지정된 Control Cmd의 수량을 되돌려준다.


	//--- System function
	virtual BOOL OpenDevice() = 0;									// 등록된 모든 제어기 열기
	virtual BOOL CloseDevice() = 0;									// 등록된 모든 제어기 닫기

	virtual BOOL OpenDevice(int nCtrlIdx) = 0;						// 장치를 활성화하고 사용가능하게 만든다.
	virtual BOOL CloseDevice(int nCtrlIdx) = 0;						// 장치를 닫고 더이상 사용하지 않는다.


	//--- Check Function
	virtual BOOL IsCheckBoard(int nCtrlIdx) = 0;					// 지정 모션 보드가 정상적인가 ?
	virtual BOOL IsOpen(int nCtrlIdx)	= 0;
	virtual BOOL IsServoOn(int nCtrlIdx, int nAxisNo) = 0;			// Servo On 출력 여부
	virtual BOOL IsServoReady(int nCtrlIdx, int nAxisNo) = 0;		// Servo가 동작 가능 상태인가 ?
	virtual BOOL IsAlarm(int nCtrlIdx, int nAxisNo) = 0;				// Alarm 발생여부
	virtual BOOL IsInpos(int nCtrlIdx, int nAxisNo) = 0;				// 축 정지 여부
	virtual BOOL IsBusy(int nCtrlIdx, int nAxisNo) = 0;				// 명령 지령중 여부
	virtual BOOL IsHomeSearch(int nCtrlIdx, int nAxisNo) = 0;		// 원점 찾기 완료 여부
	virtual BOOL IsHomeSearchMoving(int nCtrlIdx, int nAxisNo) = 0;	// 원점 찾기 수행중인가 ?
	virtual BOOL IsReady(int nCtrlIdx, int nAxisNo) = 0;				// 이동이 가능한지 여부
	virtual BOOL IsPLimit(int nCtrlIdx, int nAxisNo) = 0;			// Positive 센서 감지 여부
	virtual BOOL IsNLimit(int nCtrlIdx, int nAxisNo) = 0;			// Negative 센서 감지 여부
	virtual BOOL IsOrg(int nCtrlIdx, int nAxisNo) = 0;				// 원점(Near)센서 감지 여부

	// 현재 위치 값 조회
	virtual BOOL SetGearRatio(int nCtrlIdx, int nAxisNo, double dbRatio) = 0;		// 기어비 설정
	virtual BOOL GetCmdPos(int nCtrlIdx, int nAxisNo, double& dbPos) = 0;			// 지령 위치값 조회
	virtual BOOL GetEncPos(int nCtrlIdx, int nAxisNo, double& dbPos) = 0;			// 엔코더 위치값 조회
	virtual BOOL SetCmdPos(int nCtrlIdx, int nAxisNo, double dbNewPos) = 0;		// Command position 값을 지정한다.
	virtual BOOL SetEncPos(int nCtrlIdx, int nAxisNo, double dbNewPos) = 0;		// Encoder position 값을 지정한다.
	virtual BOOL SetPosition(int nCtrlIdx, int nAxisNo, double dbPos) = 0;			// 위치값 적용
	virtual BOOL SetSyncToEncPos(int nCtrlIdx, int nAxisNo) = 0;				// 엔코더 값으로 Command position을 일치화 시킨다.

	//--- ServoAmp Control
	virtual BOOL ServoON(int nCtrlIdx, int nAxisNo) = 0;									// Servo On 지령
	virtual BOOL ServoOFF(int nCtrlIdx, int nAxisNo) = 0;								// Servo Off 지령
	virtual BOOL CurrentON(int nCtrlIdx, int nAxisNo) = 0;								// Step motor Amp의 Power On 지령
	virtual BOOL CurrentOFF(int nCtrlIdx, int nAxisNo) = 0;								// Step motor Amp의 Power Off 지령
	virtual BOOL AlarmClearON(int nCtrlIdx, int nAxisNo) = 0;							// Servo AlarmClear 신호 ON
	virtual BOOL AlarmClearOFF(int nCtrlIdx, int nAxisNo) = 0;							// Servo AlarmClear 신호 OFF


	//--- 이동 및 정지 명령
	virtual BOOL SetHomeSearchSpeed(int nCtrlIdx, int nAxisNo, double dbHigh, double dbLow) = 0;							// 원점 이동속도 지정
	virtual BOOL MoveHomeSearch(int nCtrlIdx, int nAxisNo, int nHomeMode, int nEndMode, double dbOffset) = 0;			// 원점 이동 명령

	virtual BOOL SetMoveSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbDrive) = 0;	// 이동 속도 설정 (사다리꼴)
	virtual BOOL SetMoveSCurveSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbDrive) = 0;	// 이동 속도 설정 (S-커브)

	virtual BOOL MoveAbs(int nCtrlIdx, int nAxisNo, double dbPos) = 0;			// 지정 위치로 절대좌표 이동을 지령한다.
	virtual BOOL MoveRel(int nCtrlIdx, int nAxisNo, double dbPos) = 0;			// 지정 위치로 상대좌표 이동을 지령한다.
	virtual BOOL MoveJog(int nCtrlIdx, int nAxisNo, int nDir) = 0;				// 지정된 방향으로 JOG 이동을 지령한다.

	virtual BOOL MoveStop(int nCtrlIdx, int nAxisNo) = 0;							// 이동을 정지 시킨다. (감속 정지)
	virtual BOOL MoveJogStop(int nCtrlIdx, int nAxisNo) = 0;						// Jog 이동을 정지 시킨다.
	virtual BOOL MoveEmgStop(int nCtrlIdx, int nAxisNo) = 0;						// 이동을 비상 정지 시킨다.

	//검토 virtual BOOL MoveStopAll(int nCtrlIdx) = 0;										// 모든 축을 감속 정지 시킨다.
	//검토 virtual BOOL MoveEmgStopAll(int nCtrlIdx) = 0;									// 모든 축을 비상정지 시킨다.

	// 특수 기능 -----
	//? virtual BOOL InitAbsEncoder(int nCtrlIdx, int nAxisNo);						// 절대치 엔코더를 초기화 한다. 

};


typedef CArray<CGxMotionCmd, CGxMotionCmd&> CGxMotionCmdArray;
typedef CTypedPtrArray<CPtrArray, CGxMotionCmd*> CGxMotionCmdPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxMotionCmd*> CGxMapStringToMotionCmd;
