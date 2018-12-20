#pragma once

#include "Motion/MP2100.h"
#include "BankGuild/AxisParamBank.h"
//20160921 kw
#include "Motion/MotionNMC2.h"
#include "BankGuild/TeachBank.h"

enum MP2100_THREAD
{
	// MP2100 함수가 호출될 Thread 갯수만큼 정의
	// 각 Thread마다 전부 Open해줘야 한다
	MP2100_THREAD_MAIN_PROCESS=0,
	MP2100_THREAD_MAIN_SEQUENCE,
	MP2100_THREAD_CELLRCV_INTERFACE,
	MP2100_THREAD_AGV_INTERFACE,
	MP2100_THREAD_IO_CHECK,
	MP2100_THREAD_FDC_MONITOR,  //20161227 byskcho
	MP2100_THREAD_FDC_MONITOR_VACUUM,
	MP2100_THREAD_TRAY_MANUAL_RECEIVE,
	MP2100_THREAD_MELSEC_MONITOR,  //2018-03-03,SKCHO
	MP2100_THREAD_MAX
};

enum MP2100_BOARD
{
	// MP2100 BOARD 갯수
	MP2100_BOARD_1=0,
	MP2100_BOARD_MAX
};
enum NMC2_CTRL
{
	NMC2_BOARD_1 = 0,
	NMC2_BOARD_2 = 1,
	NMC_CTRL_MAX
};

enum	POS_RANGE_COMPAIR
{
	POS_EQUAL=0,	// 현재 위치와 같음	(Motor 정지 여부도 같이 확인한다)
	POS_LOWER,		// 현재 위치보다 작거나 같음 (Motor 정지는 확인 안한다)
	POS_UPPER,		// 현재 위치보다 높거나 같음 (Motor 정지는 확인 안한다)
};

enum	TURN_MODE	
{
	MMtoDeg,		//위치값 읽어 올때
	DegToMM,		//이동 명령 보낼때
};
class AxisMoveParam	// 이동명령에 관한 파라미터. 각 이동함수별로 필요한 거만 갖다쓴다
{
public:
	AxisMoveParam(void)
	{
		idAxis = AXIS_ID_MAX;
		dAcc = 0.;
		dDec = 0.;
		dSpeed = 0.;
		dTargetPosition = 0.;
	}

	AXIS_ID idAxis;
	double dAcc;//가속
	double dDec;//감속
	double dSpeed;//속도
	double dTargetPosition;//위치

	void operator=(const TEACH_PARAM &srcParam);
};

class AxisStatus
{
public:
	AxisStatus(void)
	{
		bServoOn = FALSE;
		bAlarmOn = FALSE;
		bWarningOn = FALSE;
		bBusy = FALSE;
		bHomeOn = FALSE;;
		bPLimitOn = FALSE;
		bNLimitOn = FALSE;
		bOriginComplete = FALSE;
	}

	BOOL bServoOn;
	BOOL bAlarmOn;
	BOOL bWarningOn;
	BOOL bBusy;
	BOOL bHomeOn;
	BOOL bPLimitOn;
	BOOL bNLimitOn;
	BOOL bOriginComplete;
};

class CDeviceMotion
{
public:
	CDeviceMotion(void);
	~CDeviceMotion(void);

	CMp2100 m_Mp2100[MP2100_THREAD_MAX][MP2100_BOARD_MAX];
	//20160921 kw
	CNmc2 m_Nmc2[NMC_CTRL_MAX];

	BOOL	Open_Mp2100(MP2100_THREAD thr);
	void Close_Mp2100();

	BOOL Open_NMC2();
	void Close_NMC2();


	BOOL ServoOn(MP2100_THREAD thr, AXIS_ID axis);
	BOOL ServoOff(MP2100_THREAD thr, AXIS_ID axis);

	BOOL ResetAlarm(MP2100_THREAD thr, AXIS_ID axis);
	//void SetPosition(MP2100_THREAD thr, double dPosition);	// 강제로 현재 위치값을 바꾼다
	void SetPosition(MP2100_THREAD thr, AXIS_ID axis, double dPosition);	// //20160923 kw 축정보 필요하여 추가

	BOOL AxisMove(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param);
	BOOL AxisMoveInc(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param);
	BOOL AxisMoveJog(MP2100_THREAD thr, AXIS_ID axis, BOOL bForward, AxisMoveParam param);
	BOOL OriginRetrun(MP2100_THREAD thr, AXIS_ID axis);

	void AxisStop(MP2100_THREAD thr, AXIS_ID axis);			// 감속 적용 후 정지
	void AxisStopEMS(MP2100_THREAD thr, AXIS_ID axis);		// 급정지

	AxisStatus GetAxisStatus(MP2100_THREAD thr, AXIS_ID axis);
	CString GetAlarmCode(MP2100_THREAD thr, AXIS_ID axis);	// 혹시 업체별로 10진수, 16진수 등 표기 차이가 있을까봐 문자열로 처리한다
	double GetTorque(MP2100_THREAD thr, AXIS_ID axis);		// int? double?
	BOOL GetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos);
	BOOL GetTargetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos);	// 이동할 목표위치

	BOOL IsAxisReady(MP2100_THREAD thr, AXIS_ID axis, BOOL bIgnoreOrigin);	// 해당 축이 지령 받을 수 있는 상태인지 확인
	BOOL IsTrouble(MP2100_THREAD thr, AXIS_ID axis);								// 축에 문제가 있는지 없는지 확인(Busy와는 상관없이 Alarm이나 Limit 등의 문제요소만 확인)

	double m_TargetPos[AXIS_ID_MAX];
	double		m_dAcctype;
	//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
	BOOL	m_bTurnTableHomeFinish;
	// 티칭값 연동
	BOOL TeachMove(MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dOffset = 0.);
	BOOL CheckAxisPosition(MP2100_THREAD thr, AXIS_ID axis, double dCheckPos, double dRange = 0.05, POS_RANGE_COMPAIR rangeCompair = POS_EQUAL );
	BOOL CheckTeachPosition(MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dRange = 0.05, POS_RANGE_COMPAIR rangeCompair = POS_EQUAL, double dOffset = 0.);
	BOOL CheckPosition(double dDefaultPos, double dNowPos, double dCheckLimit /*= 0.01*/ );

	double ConvertPosition(int nMode, double dbPos, BOOL bVision, double dbOriginPos = 0.0);
};

