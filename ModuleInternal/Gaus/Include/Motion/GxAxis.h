//
// CGxAxis : Motion Control Base class, CGxMotionCmd를 이용하여 1개 축의 동작 명령의 기본이 되는 클래스
// 
// NMC2 혹은 MP2100도 본 클래스를 부모로 삼아서 파생 정의한다.
//
// 2014-12-21, jhLee
//
#pragma once

#include "Gaus.h"

#include "GxEvent.h"
#include "File\GxIni.h"
#include "Motion\GxMotion.h"							// 모션제어관련 정의
#include "Motion\GxMotionCmd.h"

class _GAUS_LIB CGxAxis
{
public:
	CGxAxis(void);
	~CGxAxis(void);


public:
	BOOL				m_bSimulate;					// H/W 연결없이 가상모드로 동작할 것인가 ?

	int				m_nCmdType;						// 모션보드 종류
	int				m_nCmdIndex;					// 지정 모션 보드내 구분자, MotionCmd Index
	int				m_nAxisNo;						// 내부적으로 사용되는 모션 컨트롤내 축 번호(Index)
	int				m_nAmpNo;						// 실제 Servo Pack 번호
	CString			m_sAxisName;					// 축 이름

	CGxMotionCmd*	m_pMotionCmd;					// Motion 제어 명령을 수행할 Controll pointer (공통 Base Class)


	// 해당 축에대한 정보
	CString			m_sBasePath;					// 설정파일을 관리하는 기본 폴더
	CGxIni			m_iniData;						// 정보를 담고 있는 ini 파일

	TGxAxisData		m_rAxisData;					// 축정보 보관
	TGxAxisData*	m_pAxisData;					// 축정보 포인터
	TGxAxisData*	GetAxisInfo();					// 축정보 포인터


	//// 각종 상태 동기화를 위한 변수
	//CGxEvent			m_evtInpos;						// 정지 상태 
	//CGxEvent			m_evtAlarm;						// Servo Alarm  상태
	//CGxEvent*		m_pEvtStopSignal;				// 정지 하도록하는 요청 신호


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	void Init(int nAxisNo, CString sBase);			// 축 정보 지정
	void Load();									// 파일에서 정보 읽어오기
	void Save();									// 파일로 정보 저장

	// Pulse / Position간 변환, 가급적 모션제어기 자체의 기어비 설정을 이용하도록 한다.
	long PosToPulse(double dPos, double dGearRatio);
	double PulseToPos(long lPulse, double dGearRatio);

	void SetJogSpeedLevel(int nIdx);			// 지정한 Level의 Jog 속도를 지정한다.
	int GetJogSpeedLevel();						// 지정된 Jog 속도 Level 조회

	double GetJogSpeed();						// 현재 지정된 Jog 속도를 조회한다.
	double GetJogSpeed(int nIdx);				// 지정된 Jog 속도를 조회한다.
	BOOL SetJogSpeed(int nIdx, double dbSpeed);	// Jog 속도를 지정한다.

	// 이동 속도 지정과 조회
	void SetRunSpeed(double dbSpeed);					// 기본 이동 속도 지정
	BOOL SetRunSpeed(int nIdx, double dbSpeed);		// 지정 순번의 속도값을 지정한다.
	double GetRunSpeed();									// 기본 이동 속도 값 조회
	double GetRunSpeed(int nIdx);							// 지정 순번의 속도값을 조회한다.


	// 상태 조회 및 설정 공통 함수
	CString GetName();							// 축의 이름을 되돌려준다.
	void	ApplyBasicValue();					// 기본적인 값들 (기어비(스케일), 기본 속도) 적용한다.
	void	SetBasicSpeed(double dbAcc, double dbDec, double dbVel);	//기본적인 속도값 설정

	void	MoveStopRequest(BOOL bEmg);		// 정지 요청을 지정하고 실제 이동을 정지시킨다.
	void	SetInterruptFlag(BOOL bFlag);		// 요청에 의한 정지인지 Flag 변수만 지정한다.

	BOOL	IsStopRequest();						// 정지 요청 명령이 있었는가 ?
	void	ClearStopRequest();					// 정지 요청을 초기화 시켜준다.

	BOOL	GetInPosition(double& dbErr);		// 지정한 위치인가 ? 현재위치와 설정 위치 비교
	BOOL	CheckInPosition(double dbPos, double dbRatio=1.0);		// 현재 위치가 지정한 위치인가 ?
	BOOL	IsValidRange(double dbDest);		// 지정 위치가 S/W Limit 범위 이내인가 ?

	double	GetPositionError();				// 목표위치대비 현재 위치의 차이 
	virtual double	GetTorque();				// 현재 Torque값을 조회한다.
	void	SetTorque(double dbValue);			// 현재  Torque값을 지정한다 (각종 데이터 수집 Class에서 지정)
	double	GetMaxTorque();					// 최대로 기록된 토크값 조회
	void	SetClearMaxTorque();					// 최대 토크값을 초기화 한다.


	//? 사용여부 ? FormSetup2 에서만 사용
	double	GetTactTime();						// 최근에 이동한 TACT Time을 조회한다.
	UINT	GetSettlingTime();					// 이동 완료 후 안정화 시간을 되돌려준다.

	// 특수한 경우 위치 계산을 별도의 공식을 이용하여 산출한다.
	double ConvertPosition(int nMode, double dbPos, double dbOriginPos=0.0);		// 특수 위치 변환용 함수 호출

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 반드시 구현해야 하는 추상화 함수들
	virtual void	PostDeviceOpen() = 0;		// 모션 제어기 Open을 한 뒤 부가적으로 수행해야하는 동작 (MP2100에 해당)
	virtual void	AssignMotionCmd(CGxMotionCmd* pCmd) = 0;	// 공통적인 모션 컨트롤 클래스를 실제 객체로 치환시킨다.
	virtual BOOL	StartUp() = 0;				// 각종 설정값을 실제로 적용시키고 사용 할 준비를 한다.

	//--- Check Function
	virtual BOOL	IsServoOn()	= 0;			// Servo On 출력 여부
	virtual BOOL	IsServoReady() = 0;			// Servo가 동작 가능 상태인가 ?
	virtual BOOL	IsAlarm() = 0;				// Alarm 발생여부
//	virtual BOOL	IsWarning() = 0;			// Warning 발생여부
	virtual BOOL	IsInpos() = 0;				// 축 정지 여부
	virtual BOOL	IsBusy() = 0;				// 명령 지령중 여부
	virtual BOOL	IsHomeSearch() = 0;			// 원점 찾기 완료 여부
	virtual BOOL	IsHomeSearchMoving() = 0;	// 원점 찾기 이동 중인가 ?
	virtual BOOL	IsReady() = 0;				// 이동이 가능한지 여부
	virtual BOOL	IsPLimit() = 0;				// Positive 센서 감지 여부
	virtual BOOL	IsNLimit() = 0;				// Negative 센서 감지 여부
	virtual BOOL	IsOrg() = 0;				// 원점(Near)센서 감지 여부

	// 현재 위치 값 조회
	virtual BOOL	SetGearRatio(double dbRatio) = 0;	// 기어비 설정
	virtual BOOL	GetCmdPos(double& dbPos) = 0;		// 지령 위치값 조회
	virtual BOOL	GetEncPos(double& dbPos) = 0;		// 엔코더 위치값 조회
	virtual BOOL	SetCmdPos(double dbNewPos) = 0;		// Command position 값을 지정한다.
	virtual BOOL	SetEncPos(double dbNewPos) = 0;		// Encoder position 값을 지정한다.
	virtual BOOL	GetPosition(double& dbPos) = 0;		// 현재 위치값 (servo/Step 자동 판정)
	virtual BOOL	SetPosition(double dbPos) = 0;		// 위치값 적용(Cmd/Enc
	virtual BOOL	SetSyncToEncPos() = 0;				// 엔코더 값으로 Command position을 일치화 시킨다.

	//--- ServoAmp Control
	virtual BOOL	ServoON() = 0;								// Servo On 지령
	virtual BOOL	ServoOFF() = 0;							// Servo Off 지령
	virtual BOOL	AlarmClearON() = 0;						// Servo AlarmClear ON 지령
	virtual BOOL	AlarmClearOFF() = 0;						// Servo AlarmClear OFF 지령


	//--- 이동 및 정지 명령
	virtual BOOL	SetHomeSearchSpeed(double dbHigh, double dbLow) = 0;							// 원점 이동속도 지정
	virtual BOOL	MoveHomeSearch() = 0; // int nHomeMode, int nEndMode, double dbOffset);			// 원점 이동 명령

	virtual BOOL	SetMoveSpeed(double dbAcc, double dbDec, double dbDrive) = 0;			// 이동 속도 설정 (사다리꼴)
	virtual BOOL	SetMoveSCurveSpeed(double dbAcc, double dbDec, double dbDrive) = 0;	// 이동 속도 설정 (S-커브)

	virtual BOOL	MoveAbs(double dbPos, int nSpeedIdx=-1) = 0;				// 지정 위치로 절대좌표 이동을 지령한다.
	virtual BOOL	MoveRel(double dbPos, int nSpeedIdx=-1) = 0;				// 지정 위치로 상대좌표 이동을 지령한다.
	virtual BOOL	MoveJog(int nDir) = 0;					// 지정된 방향으로 JOG 이동을 지령한다.

	// 모션 제어기별로 구현 가능한 선택적 함수, 현재(2015-12-31)는 NMC2 제어기만 구현 가능
	virtual BOOL	MoveAbsSpeed(double dbPos, double dbSpeed) = 0;				// 지정 위치/속도로 절대좌표 이동을 지령한다.

	// V2.5.8, 2016-06-21, jhLee, 미사용
	//	virtual	BOOL	MoveAbsSpeedChange(double dbPos1, double dbSpeed1, double dbPos2, double dbSpeed2) = 0;	// 지정 위치로 2지점 속도 변경 이동.


	virtual BOOL	MoveStop() = 0;							// 이동을 정지 시킨다. (감속 정지)
	virtual BOOL	MoveJogStop() = 0;						// Jog 이동을 정지 시킨다.
	virtual BOOL	MoveEmgStop() = 0;						// 이동을 비상 정지 시킨다.

};

typedef CArray<CGxAxis, CGxAxis&> CGxAxisArray;
typedef CTypedPtrArray<CPtrArray, CGxAxis*> CGxAxisPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxAxis*> CGxMapStringToAxis;