//
// CGxAxis : Motion Control Base class, CGxMotionCmd를 이용하여 1개 축의 동작 명령의 기본이 되는 클래스
// 
// 공통이되는 CGxAxis와 실제 모션보드별로 따로이 존재하는 API는 사용법에서 차이가 발생 할 수 있다.
// 본 클래스는 이러한 괴리를 중간에 추상화를 시켜줌으로 상위 CGxRobot단에서 모션보드에 독립적으로 모션 명령을 수행할 수 
// 있도록 중간 단계를 연결해 준다.
//
// CGxAxis_NMC2 : Paix NMC2를 통해 구현되는 실제 Axis class
//
// 2014-12-21, jhLee
// 2014-12-24, jhLee 1차 완료
// 2015-05-28, jhLee, Gaus2용으로 변경 시작
//
#pragma once

#include "GxAxis.h"
#include "GxMotionCmd.h"
#include "GxMotionCmd_NMC1.h"		// NMC2 API Wrapper class


class _GAUS_LIB CGxAxis_NMC1 : public CGxAxis
{
public:
	CGxAxis_NMC1(void);
	~CGxAxis_NMC1(void);


	CGxMotionCmd_NMC1*	m_pMyCmd;					// 실제 동작세 사용할 Cmd : 공통에서 캐스팅 치환 시킨다.

	void	PostDeviceOpen();		// 모션 제어기 Open을 한 뒤 부가적으로 수행해야하는 동작 (MP2100에 해당)
	void	AssignMotionCmd(CGxMotionCmd* pCmd);	// 공통적인 모션 컨트롤 클래스를 실제 객체로 치환시킨다.
	BOOL	StartUp();				// 각종 설정값을 실제로 적용시키고 사용 할 준비를 한다.


	//--- Check Function
	BOOL	IsServoOn();			// Servo On 출력 여부
	BOOL	IsServoReady();			// Servo가 동작 가능 상태인가 ?
	BOOL	IsAlarm();				// Alarm 발생여부
	BOOL	IsInpos();				// 축 정지 여부
	BOOL	IsBusy();				// 명령 지령중 여부
	BOOL	IsHomeSearch();			// 원점 찾기 완료 여부
	BOOL	IsHomeSearchMoving();	// 원점 찾기 이동 중인가 ?
	BOOL	IsReady();				// 이동이 가능한지 여부
	BOOL	IsPLimit();				// Positive 센서 감지 여부
	BOOL	IsNLimit();				// Negative 센서 감지 여부
	BOOL	IsOrg();				// 원점(Near)센서 감지 여부

	// --- Position value
	BOOL	SetGearRatio(double dbRatio);	// 기어비 설정
	BOOL	GetCmdPos(double& dbPos);		// 지령 위치값 조회
	BOOL	GetEncPos(double& dbPos);		// 엔코더 위치값 조회
	BOOL	SetCmdPos(double dbNewPos);		// Command position 값을 지정한다.
	BOOL	SetEncPos(double dbNewPos);		// Encoder position 값을 지정한다.
	BOOL	GetPosition(double& dbPos);	// 현재 위치값 (servo/Step 자동 판정)
	BOOL	SetPosition(double dbPos);		// 위치값 적용
	BOOL	SetSyncToEncPos();				// 엔코더 값으로 Command position을 일치화 시킨다.

	//--- ServoAmp Control
	BOOL	ServoON();							// Servo On 지령
	BOOL	ServoOFF();							// Servo Off 지령
	BOOL	AlarmClearON();					// Servo AlarmClear ON지령
	BOOL	AlarmClearOFF();					// Servo AlarmClear OFF지령


	//--- 이동 및 정지 명령
	BOOL	SetHomeSearchSpeed(double dbHigh, double dbLow);							// 원점 이동속도 지정
	BOOL	MoveHomeSearch(); // int nHomeMode, int nEndMode, double dbOffset);			// 원점 이동 명령

	BOOL	SetMoveSpeed(double dbAcc, double dbDec, double dbDrive);			// 이동 속도 설정 (사다리꼴)
	BOOL	SetMoveSCurveSpeed(double dbAcc, double dbDec, double dbDrive);	// 이동 속도 설정 (S-커브)

	BOOL	MoveAbs(double dbPos, int nSpeedIdx=-1);			// 지정 위치로 절대좌표 이동을 지령한다.
	BOOL	MoveRel(double dbPos, int nSpeedIdx=-1);			// 지정 위치로 상대좌표 이동을 지령한다.
	BOOL	MoveJog(int nDir);				// 지정된 방향으로 JOG 이동을 지령한다.

	BOOL	MoveAbsSpeed(double dbPos, double dbSpeed);				// 지정 위치/속도로 절대좌표 이동을 지령한다.

	// V2.5.8, 2016-06-21, jhLee, 미사용
	// BOOL	MoveAbsSpeedChange(double dbPos1, double dbSpeed1, double dbPos2, double dbSpeed2);	// 지정 위치로 2지점 속도 변경 이동.

	BOOL	MoveStop();							// 이동을 정지 시킨다. (감속 정지)
	BOOL	MoveJogStop();						// Jog 이동을 정지 시킨다.
	BOOL	MoveEmgStop();						// 이동을 비상 정지 시킨다.

};


