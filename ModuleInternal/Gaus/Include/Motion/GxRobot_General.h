//
// CGxRobot_General : 다축을 사용하는 일반적인 Robot 파생 Class
// 
// NMC2 혹은 MP2100도 본 클래스에서 공통적으로 일반화된 Axis를 이용하여 제어한다.
//
// 2014-12-21, jhLee
// 2015-05-31, jhLee, 기존 특정 Motion board별로 특화된 Robot을 준비하여 사용하던것을 일반적인 특성을 갖는 Class로 통합하여 추상화 구현
//

#pragma once

#include "Gaus.h"
#include "GxTask.h"							// GxStation / GxSystem / GxService 등 별도의 Thread를 가지는 객체

#include "Motion\GxMotion.h"
#include "Motion\GxRobot.h"
#include "Motion\GxAxis.h"

//
//
//#include "GxMotionCmd.h"
//#include "GxRobot_NMC2.h"
//#include "GxMotionCmd_NMC2.h"


class _GAUS_LIB CGxRobot_General : public CGxRobot
{
public:
	CGxRobot_General();
	~CGxRobot_General();


	void	PostDeviceOpen();					// 모션 제어기 Open을 한 뒤 부가적으로 수행해야하는 동작 (MP2100에 해당)
	
	//--- Check Function
	BOOL	IsServoOn(int nErrMode=0);						// Servo On 출력 여부
	BOOL	IsServoOnAxis(int nAxis, int nErrMode=0);		// 지정 축 Servo On 출력 여부
	BOOL	IsServoReady(int nErrMode=0);					// Servo가 동작 가능 상태인가 ?
	BOOL	IsServoReadyAxis(int nAxis, int nErrMode=0);	// 지정축 Servo가 동작 가능 상태인가 ?
	BOOL	IsAlarm(int nErrMode=0);						// Alarm 발생여부
	BOOL	IsAlarmAxis(int nAxis, int nErrMode=0);			// 지정축 Alarm 발생여부

	BOOL	IsInpos(int nErrMode=0);						// 축 정지 여부
	BOOL	IsInposAxis(int nAxis, int nErrMode=0);			// 지정 축의 축 정지 여부

	BOOL	IsBusy(int nErrMode=0);							// 명령 지령중 여부
	BOOL	IsBusyAxis(int nAxis,int nErrMode=0);			// 지정 축의 명령 지령중 여부
	BOOL	IsHomeSearch(int nErrMode=0);					// 원점 찾기 완료 여부
	BOOL	IsHomeSearchAxis(int nAxis, int nErrMode=0);	// 지정축 원점 찾기 완료 여부
	BOOL	IsReady(int nErrMode=0);						// 이동이 가능한지 여부
	BOOL	IsReadyAxis(int nAxis,int nErrMode=0);			// 지정축 이동이 가능한지 여부
	BOOL	IsPLimit(int nErrMode=0);						// Positive 센서 감지 여부
	BOOL	IsPLimitAxis(int nAxis, int nErrMode=0);		// 지정축 Positive 센서 감지 여부
	BOOL	IsNLimit(int nErrMode=0);						// Negative 센서 감지 여부
	BOOL	IsNLimitAxis(int nAxis, int nErrMode=0);		// 지정축 Negative 센서 감지 여부
	BOOL	IsOrgAxis(int nAxis);							// 지정축 원점(Near)센서 감지 여부

	int	GetIsMoveReady(BOOL bAlarm=FALSE);				// 이동이 가능한 상태인가 ? 
	int	GetIsMoveReadyAxis(int nAxis, BOOL bAlarm=FALSE);		// 이동이 가능한 상태인가 ? 
	int	GetIsValidRange(const TGxMPos &rMPos, BOOL bAlarm=FALSE);			// 지정 좌표가 합당한 위치 값인가 ?


	//--- ServoAmp Control
	BOOL	ServoON(BOOL bAlarm=FALSE);						// Servo On 지령
	BOOL	ServoONAxis(int nAxis, BOOL bAlarm=FALSE);		// 지정축 Servo On 지령
	BOOL	ServoOFF(BOOL bAlarm=FALSE);					// Servo Off 지령
	BOOL	ServoOFFAxis(int nAxis, BOOL bAlarm=FALSE);		// 지정축 Servo Off 지령
	BOOL	AlarmClear(BOOL bAlarm=FALSE);					// Servo AlarmClear 지령
	BOOL	AlarmClearAxis(int nAxis, BOOL bAlarm=FALSE);	// 지정축 Servo AlarmClear 지령


	//--- 이동 및 정지 명령 (기본)
	int	MoveHomeSearch(CGxTask* pTask = NULL);  // 원점 이동 명령
	int	MoveHomeSearchAxis(int nAxis, CGxTask* pTask = NULL);  // 원점 이동 명령
	int	IsDoneMoveHomeSearch();					// 원점 찾기 이동을 완료하였는가 조회
	int	IsDoneMoveHomeSearchAxis(int nAxis);	// 원점 찾기 이동을 완료하였는가 조회

	int	MoveAbs(const TGxMPos &rMPos, int nSpeedIdx=-1);			// 지정 위치로 절대좌표 이동을 지령한다.
	int	MoveAbsAxis(int nAxis, double dbPos, int nSpeedIdx=-1);	// 지정축을 지정 위치로 절대좌표 이동을 지령한다.
	int	MoveRel(const TGxMPos &rMPos, int nSpeedIdx=-1);			// 지정 위치로 상대좌표 이동을 지령한다.
	int	MoveRelAxis(int nAxis, double dbPos, int nSpeedIdx=-1);	// 지정 축을 지정 위치로 상대좌표 이동을 지령한다.
	int	IsDoneMove(int* pRetry = NULL);			// 지정 위치로 이동을 완료하였는가 ?
	int	IsDoneMoveAxis(int nAxis);				// 지정축이 이동을 완료하였는가 ?

	int	MoveJog(int nDir);						// 모든축을 지정된 방향으로 JOG 이동을 지령한다.
	int	MoveJogAxis(int nAxis, int nDir);		// 지정 축을 지정된 방향으로 JOG 이동을 지령한다.

	// 특수 목적 함수
	int	MoveAbsSpeedAxis(int nAxis, double dbPos, double dbSpeed);	// 지정축을 지정 위치로 지정 속도로 절대좌표 이동을 지령한다.

	// V2.5.8, 2016-06-21, jhLee, 미사용
	//	int	MoveAbsSpeedChange(const TGxMPos &rMPos, int nAxis);	// 지정 축의  속도 변환 이동을 지령한다.

	int	MoveStop();								// 모든축의 이동을 정지 시킨다. (감속 정지)
	int	MoveStopAxis(int nAxis);				// 지정축 이동을 정지 시킨다. (감속 정지)
	int	MoveEmgStop();							// 모든축을 비상 정지 시킨다.
	int	MoveEmgStopAxis(int nAxis);				// 지정축을 비상 정지 시킨다.


};