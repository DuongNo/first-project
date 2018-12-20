//
// CGxRobotCtrl : GxStation 혹은 GxSystem 등의 Task 자손에서 모션 제어 명령과 에러처리를 가능하게 만드는 Class
// 
// 이동 명령 지령 후 완료를 기다리거나 에러발생시 처리해주는 기능을 담당
//
// 2015-06-01, jhLee, 최초 작성
//

#pragma once

#include "Gaus.h"
#include "GxCallback.h"							// Safety Check를 위한 함수용	
#include "GxObject.h"
#include "GxTask.h"
#include "GxEvent.h"
#include "File\GxLog.h"
#include "Motion\GxMotion.h"
#include "Motion\GxRobot.h"
#include "Motion\GxAxis.h"


class _GAUS_LIB CGxRobotCtrl
{
public:
	CGxRobotCtrl();
	~CGxRobotCtrl();

	//----- 내부 
	CString			m_sName;							// 이름 지정
	CGxTask*			m_pTask;							// 주체가 되는 Thread 모체
	CGxRobot*		m_pMyRobot;						// 제어에 사용될 Robot
	CGxLog			m_log;


	// 각종 상태 동기화를 위한 변수
	CGxEventPtrArray	m_pArrStopEvent;			// 축을 정지 시키기 위한 요청 신호 event


	// Callback 함수 
	CGxCallbackStr*	m_pInterlockPtr;					// Interlock을 점검하는 Callback함수, CString 형태로 리턴 받는다.

	void SetName(CString sName);							// 이름을 지정한다.
	CString GetName();										// 이름을 조회한다.
	void SetLogEnabled(BOOL bFlag);						// Log를 남길것인지 여부를 지정한다.
	BOOL GetLogEnabled();									// Log를 남기도록 지정하였는지 조회한다.

	void AssignTask(CGxTask* pTask);						// 소유하고 있는 Task 모체 포인터
	void AssignRobot(CGxRobot* pRobot);					// 사용하게될 Robot을 지정해준다.
	void SetInterlockFunction(CGxCallbackStr* p);	// Interlock 점검 함수 포인터 대입

	void ClearStopEvent();									// 정지를 위해 지정하였던 Event 신호를 초기화 해준다.
	void AddStopEvent(CGxEvent* pSignal);				// 정지를 하기위한 Event 신호를 추가해준다.

	// 라이브러리 테스트용 원점찾기 함수
	int	DEBUG_HomeSearch() ;


	//-----
	BOOL	GetCurrentPosition(TGxMPos &rMPos);			// 현재 각 축의 위치값을 대입 받는다.


	//--- Check Function
	BOOL	IsServoOn(BOOL bAlarm = FALSE);				// Servo On 출력 여부
	BOOL	IsServoOnAxis(int nAxis);					// 지정 축 Servo On 출력 여부
	BOOL	IsServoReady(BOOL bAlarm = FALSE);			// Servo가 동작 가능 상태인가 ?
	BOOL	IsServoReadyAxis(int nAxis);				// 지정축 Servo가 동작 가능 상태인가 ?
	BOOL	IsAlarm(BOOL bAlarm = FALSE);				// Alarm 발생여부
	BOOL	IsAlarmAxis(int nAxis);						// 지정축 Alarm 발생여부
	BOOL	IsInpos(BOOL bAlarm = FALSE);				// 축 정지 여부
	BOOL	IsInposAxis(int nAxis);						// 지정 축의 축 정지 여부
	BOOL	IsBusy(BOOL bAlarm = FALSE);				// 명령 지령중 여부
	BOOL	IsBusyAxis(int nAxis);						// 지정 축의 명령 지령중 여부
	BOOL	IsHomeSearch(BOOL bAlarm = FALSE);			// 원점 찾기 완료 여부
	BOOL	IsHomeSearchAxis(int nAxis);				// 지정축 원점 찾기 완료 여부
	BOOL	IsReady(BOOL bAlarm = FALSE);				// 이동이 가능한지 여부
	BOOL	IsReadyAxis(int nAxis);						// 지정축 이동이 가능한지 여부
	BOOL	IsPLimit(BOOL bAlarm = FALSE);				// Positive 센서 감지 여부
	BOOL	IsPLimitAxis(int nAxis);					// 지정축 Positive 센서 감지 여부
	BOOL	IsNLimit(BOOL bAlarm = FALSE);				// Negative 센서 감지 여부
	BOOL	IsNLimitAxis(int nAxis);					// 지정축 Negative 센서 감지 여부
	BOOL	IsOrgAxis(int nAxis);						// 지정축 원점(Near)센서 감지 여부

	//--- ServoAmp Control
	BOOL	ServoON();								// Servo On 지령
	BOOL	ServoONAxis(int nAxis);					// 지정축 Servo On 지령
	BOOL	ServoOFF();								// Servo Off 지령
	BOOL	ServoOFFAxis(int nAxis);				// 지정축 Servo Off 지령
	BOOL	AlarmClear();							// Servo AlarmClear 지령
	BOOL	AlarmClearAxis(int nAxis);				// 지정축 Servo AlarmClear 지령


	//--- 이동 및 정지 명령 (기본)
	int	MoveWaitDone(TGxMParam &rMParam);			// 지정 위치로 절대좌표 이동을 지령하고 완료때까지 대기한다.
	int	MoveWaitDoneAxis(TGxMParam &rMParam, int nAxisNo);	// 한 축을 지정 위치로 절대좌표 이동을 지령하고 완료때까지 대기한다.

	// SFT전용 특수 기능 (2016-01-02), 특정은 속도를 지정하여 이동 시킨다.
	int	MoveSpeedWaitDoneAxis(TGxMParam &rMParam, int nAxisNo);			// 한 축을 지정 속돌 지정위치로 절대좌표 이동을 지령하고 완료때까지 대기한다.

	//d 삭제함 int	MoveSpeedChangeWaitDoneAxis(TGxMParam &rMParam, int nAxisNo);	// 한 축을 지정 위치로 속도 변환 절대좌표 이동을 지령하고 완료때까지 대기한다.
	
	
	int	HomeSearchWaitDone(TGxMParam &rMParam); 	// 원점 이동 명령 후 완료때까지 대기한다.
	int	HomeSearchWaitDoneAxis(TGxMParam &rMParam, int nAxisNo); 	// 지정 한 축 원점찾기 후 완료때까지 대기한다.


	//int	MoveHomeSearch(const TGxMParam &rMParam); 	// 원점 이동 명령
	//int	IsDoneMoveHomeSearch();					// 원점 찾기 이동을 완료하였는가 조회
	//int	MoveHomeSearchWaitDone();				// 원점 찾기후 완료될때까지 대기한다.

	//int	MoveHomeSearchAxis(int nAxis); 			// 지정 축 원점 이동 명령
	//int	IsDoneMoveHomeSearchAxis(int nAxis);	// 원점 찾기 이동을 완료하였는가 조회
	//int	MoveHomeSearchWaitDoneAxis(int nAxis);	// 원점 찾기후 완료될때까지 대기한다.


	//int	IsDoneMove();							// 지정 위치로 이동을 완료하였는가 ?
	//int	IsDoneMoveAxis(int nAxis);				// 지정축이 이동을 완료하였는가 ?

	//int	MoveAbs(const TGxMParam &rMParam);			// 지정 위치로 절대좌표 이동을 지령한다.
	//int	MoveAbsAxis(int nAxis, double dbPos);	// 지정축을 지정 위치로 절대좌표 이동을 지령한다.
	//int	MoveWaitDone(TGxPosition* pPos);		// 지정 위치로 절대좌표 이동을 지령후 이동 완료를 기다린다.
	//int	MoveWaitDoneAxis(int nAxis);			// 지정축이 절대위치로 이동 지령 후 완료하기를 기다린다.

	//int	MoveRel(TGxPosition* pPos);				// 지정 위치로 상대좌표 이동을 지령한다.
	//int	MoveRelAxis(int nAxis, double dbPos);	// 지정 축을 지정 위치로 상대좌표 이동을 지령한다.

	//int	MoveJog(int nDir);						// 모든축을 지정된 방향으로 JOG 이동을 지령한다.
	//int	MoveJogAxis(int nAxis, int nDir);		// 지정 축을 지정된 방향으로 JOG 이동을 지령한다.

	//int	MoveStop();								// 모든축의 이동을 정지 시킨다. (감속 정지)
	//int	MoveStopAxis(int nAxis);				// 지정축 이동을 정지 시킨다. (감속 정지)
	//int	MoveEmgStop();							// 모든축을 비상 정지 시킨다.
	//int	MoveEmgStopAxis(int nAxis);				// 지정축을 비상 정지 시킨다.

};