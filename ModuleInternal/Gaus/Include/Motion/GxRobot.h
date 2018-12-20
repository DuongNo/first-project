//
// CGxRobot : 최종적으로 Motion Control에서 이용될 Base class, 모션 제어 명령의 복합체가 되는 Base 클래스
// 
// NMC2 혹은 MP2100도 본 클래스를 부모로 삼아서 파생 정의한다.
//
// 2014-12-21, jhLee
//

#pragma once

#include "Gaus.h"
#include "GxTask.h"
#include "File\GxIniFile.h"
#include "File\GxLog.h"
#include "Motion\GxAxis.h"


class _GAUS_LIB CGxRobot
{
public:
	CGxRobot(void);
	~CGxRobot(void);


	// 해당 Robot에대한 정보
	BOOL			m_bSimulate;					// H/W 연결없이 가상모드로 동작할 것인가 ?
	int			m_nIndex;						// Index
	CString		m_sMyName;						// Robot의 이름
	CString		m_sBasePath;					// 설정파일을 관리하는 기본 폴더

	CGxIniFile	m_iniData;						// 정보를 담고 있는 ini 파일
	CGxLog		m_log;							// 실행 내용을 파일로 기록 할 control
	BOOL			m_bLogFlag;						// 실행중 발생 이슈를 로그로 남길것인가 ?

	// 관리하는 축 정보
	int			m_nAxisCount;					// 내부적으로 사용되는 축 수량

	CUIntArray	m_arrAxisIndex;					// 구성된 축의 축 번호 저장 배열
	CGxAxisPtrArray	m_arrAxisMap;				// Axis 포인터 배열


	// 실행 예외처리 정보
	int			m_nErrorAxis;					// Error가 발생한 축 (전체 Axis의 Index)
	int			m_nErrorMyAxis;					// 현재 Robot내에서의 Error가 발생한 축 순번
	int			m_nErrorCode;					// 발생한 Error code
	CString		m_sErrorMsg;					// 발생한 Error 내용

	// 각종 시간 값
	int			m_nAlarmResetDelay;				// Alarm Clear 출력시 ON 출력 유지 시간, mSec
	int			m_nSettlingTime;				// 이동을 마친 후 안정화 시간
	int			m_nHomeTimeout;					// 원점 찾기시 시간 초과 기준
	int			m_nMoveTimeout;					// 이동 명령의 시간 초과 기준


	void Init(int nIdx, CString sBase);			// 축 Robot 정보 지정

	BOOL IsValidIndex(int nIdx, BOOL bAlarm = FALSE);	// 지정한 축 index가 유효한 것인지 check
	BOOL IsExistAxis(BOOL bAlarm = FALSE);		// 지정된 축이 존재하는가 ? 
	int GetHomeTimeoutMax(int nAxisNo = -1);	// 가장 긴 원점찾기 시간초과 설정값을 취득한다.
	int GetMoveTimeoutMax(int nAxisNo = -1);	// 가장 긴 이동수행 시간초과 설정값을 취득한다.
	int GetSettlingTimeMax(int nAxisNo = -1);	// 가장 긴 안정화 지연시간 조회
	int GetResetTimeMax(int nAxisNo = -1);		// 가장 긴 Alarm Reset 지연 시간 조회
	int GetRetryCountMax(int nAxisNo = -1);		// 가장 큰 자동 재시도 횟수 조회
	int GetRetryIntervalMax(int nAxisNo = -1);	// 가장 긴 자동 재시도 지연 시간 조회

	CGxAxis* GetAxis(int nIdx);					// 현재 Robot이 가진 지정 순번의 축을 조회한다.
	CGxAxis* GetAxis(CString sName);			// 지정 이름을 가진 축을 조회한다.
	int GetAxisIndex(CString sName);			// 지정 이름을 가진 축의 로봇내 순번을 조회한다.
	int GetAxisCount();							// 등록된 축의 수량

	// 오류 발생 내역 조회
	CString	GetErrorMsg();						// 최종적으로 지정된 오류 메세지를 조회한다.
	CString GetErrorAxisName();					// 최종적으로 오류가 발생된 축의 이름을 조회한다.

	// 내부 로직에 의한 원점찾기 관련 함수
	BOOL IsHomeSearchByData(BOOL bAlarm = FALSE);					// 원점 찾기 수행 여부 확인
	BOOL IsHomeSearchByDataAxis(int nAxisNo, BOOL bAlarm = FALSE);	// 지정 축의 원점 찾기 수행 여부 확인
	BOOL SetHomeSearchForData(BOOL bFlag);							// 원점 찾기 수행 여부 지정

	// 위치관련 조회/비교 함수
	BOOL IsInposition(const TGxMPos &rMPos, double dbRatio = 1.0);			// 현재 위치가 지정된 위치값과 일치하는가 ?
	BOOL IsInpositionAxis(int nAxis, double dbDestPos, double dbRatio = 1.0);	// 지정 단일축 현재 위치가 지정된 위치값과 일치하는가 ?
	BOOL GetCurrentPosition(TGxMPos &rMPos);			// 현재 각 축의 위치값을 대입 받는다.

	// 전축 정지 요청
	void MoveStopRequest(BOOL bEmg);			// 정지 요청을 지정하고 실제 이동을 정지시킨다.
	BOOL IsMoveStopRequest();					// 정지 요청이 있었는가 ? 1개 축이라도 있었다면 TRUE
	void ClearStopRequest();					// 정지 요청 플랙을 초기화 한다.

	//----- Debug Test용 함수 -----
	void DEBUG_SetHomeSearch();					// 원점 찾기 완료를 처리해 준다.
	void DEBUG_SetHomeSearchAxis(int nAxisNo);	// 지정 축을 가상으로 원점 찾기를 마쳤다고 지정한다.
	void DEBUG_SetMovePosition(const TGxMPos &rMPos);			// 지정 위치로 이동하였다고 지정
	void DEBUG_SetMovePositionAxis(int nAxisNo, double dbPos);	// 지정 축을 지정 위치로 이동하였다고 지정한다.


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 가상함수 선언 : 상속 받는 자손은 반드시 기술해야한다.


	virtual void	PostDeviceOpen() = 0;		// 모션 제어기 Open을 한 뒤 부가적으로 수행해야하는 동작 (MP2100에 해당)


	//--- Check Function
	virtual BOOL	IsServoOn(int nErrMode=0) = 0;						// Servo On 출력 여부
	virtual BOOL	IsServoOnAxis(int nAxis, int nErrMode=0) = 0;		// 지정 축 Servo On 출력 여부
	virtual BOOL	IsServoReady(int nErrMode=0) = 0;					// Servo가 동작 가능 상태인가 ?
	virtual BOOL	IsServoReadyAxis(int nAxis, int nErrMode=0) = 0;	// 지정축 Servo가 동작 가능 상태인가 ?
	virtual BOOL	IsAlarm(int nErrMode=0) = 0;						// Alarm 발생여부
	virtual BOOL	IsAlarmAxis(int nAxis, int nErrMode=0) = 0;			// 지정축 Alarm 발생여부
	virtual BOOL	IsInpos(int nErrMode=0) = 0;						// 축 정지 여부
	virtual BOOL	IsInposAxis(int nAxis, int nErrMode=0) = 0;			// 지정 축의 축 정지 여부
	virtual BOOL	IsBusy(int nErrMode=0) = 0;							// 명령 지령중 여부
	virtual BOOL	IsBusyAxis(int nAxis,int nErrMode=0) = 0;			// 지정 축의 명령 지령중 여부
	virtual BOOL	IsHomeSearch(int nErrMode=0) = 0;					// 원점 찾기 완료 여부
	virtual BOOL	IsHomeSearchAxis(int nAxis, int nErrMode=0) = 0;	// 지정축 원점 찾기 완료 여부
	virtual BOOL	IsReady(int nErrMode=0) = 0;						// 이동이 가능한지 여부
	virtual BOOL	IsReadyAxis(int nAxis,int nErrMode=0) = 0;			// 지정축 이동이 가능한지 여부
	virtual BOOL	IsPLimit(int nErrMode=0) = 0;						// Positive 센서 감지 여부
	virtual BOOL	IsPLimitAxis(int nAxis, int nErrMode=0) = 0;		// 지정축 Positive 센서 감지 여부
	virtual BOOL	IsNLimit(int nErrMode=0) = 0;						// Negative 센서 감지 여부
	virtual BOOL	IsNLimitAxis(int nAxis, int nErrMode=0) = 0;		// 지정축 Negative 센서 감지 여부
	virtual BOOL	IsOrgAxis(int nAxis) = 0;							// 지정축 원점(Near)센서 감지 여부

	virtual int		GetIsMoveReady(BOOL bAlarm = FALSE) = 0;			// 이동이 가능한 상태인가 ? 
	virtual int		GetIsValidRange(const TGxMPos &rMPos, BOOL bAlarm=FALSE) = 0;	// 지정 좌표가 합당한 위치 값인가 ?

	//--- ServoAmp Control
	virtual BOOL	ServoON(BOOL bAlarm=FALSE) = 0;						// Servo On 지령
	virtual BOOL	ServoONAxis(int nAxis, BOOL bAlarm=FALSE) = 0;		// 지정축 Servo On 지령
	virtual BOOL	ServoOFF(BOOL bAlarm=FALSE) = 0;					// Servo Off 지령
	virtual BOOL	ServoOFFAxis(int nAxis, BOOL bAlarm=FALSE) = 0;		// 지정축 Servo Off 지령
	virtual BOOL	AlarmClear(BOOL bAlarm=FALSE) = 0;					// Servo AlarmClear 지령
	virtual BOOL	AlarmClearAxis(int nAxis, BOOL bAlarm=FALSE) = 0;	// 지정축 Servo AlarmClear 지령


	//--- 이동 및 정지 명령 (기본)
	virtual int	MoveHomeSearch(CGxTask* pTask = NULL) = 0;	// 원점 이동 명령
	virtual int	MoveHomeSearchAxis(int nAxis, CGxTask* pTask = NULL) = 0;	// 원점 이동 명령
	virtual int	IsDoneMoveHomeSearch() = 0;					// 원점 찾기 이동을 완료하였는가 조회
	virtual int	IsDoneMoveHomeSearchAxis(int nAxis) = 0;	// 원점 찾기 이동을 완료하였는가 조회

	virtual int	MoveAbs(const TGxMPos &rMPos, int nSpeedIdx=-1) = 0;			// 지정 위치로 절대좌표 이동을 지령한다.
	virtual int	MoveAbsAxis(int nAxis, double dbPos, int nSpeedIdx=-1) = 0;	// 지정축을 지정 위치로 절대좌표 이동을 지령한다.
	virtual int	MoveAbsSpeedAxis(int nAxis, double dbPos, double dbSpeed) = 0;	// 지정축을 지정 위치로 지정 속도로 절대좌표 이동을 지령한다.
	// V2.5.8, 2016-06-21, jhLee, 미사용, virtual int	MoveAbsSpeedChange(const TGxMPos &rMPos, int nAxis) = 0;	// 지정 축의  속도 변환 이동을 지령한다.
	virtual int	MoveRel(const TGxMPos &rMPos, int nSpeedIdx=-1) = 0;			// 지정 위치로 상대좌표 이동을 지령한다.
	virtual int	MoveRelAxis(int nAxis, double dbPos, int nSpeedIdx=-1) = 0;	// 지정 축을 지정 위치로 상대좌표 이동을 지령한다.
	virtual int	IsDoneMove(int* pRetry = NULL) = 0;			// 지정 위치로 이동을 완료하였는가 ?
	virtual int	IsDoneMoveAxis(int nAxis) = 0;				// 지정축이 이동을 완료하였는가 ?

	virtual int	MoveJog(int nDir) = 0;						// 모든축을 지정된 방향으로 JOG 이동을 지령한다.
	virtual int	MoveJogAxis(int nAxis, int nDir) = 0;		// 지정 축을 지정된 방향으로 JOG 이동을 지령한다.
	
	virtual int	MoveStop() = 0;								// 모든축의 이동을 정지 시킨다. (감속 정지)
	virtual int	MoveStopAxis(int nAxis) = 0;				// 지정축 이동을 정지 시킨다. (감속 정지)
	virtual int	MoveEmgStop() = 0;							// 모든축을 비상 정지 시킨다.
	virtual int	MoveEmgStopAxis(int nAxis) = 0;				// 지정축을 비상 정지 시킨다.

};

typedef CArray<CGxRobot, CGxRobot&> CGxRobotArray;
typedef CTypedPtrArray<CPtrArray, CGxRobot*> CGxRobotPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxRobot*> CGxMapStringToRobot;
