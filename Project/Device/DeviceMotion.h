#pragma once

#include "Motion/MP2100.h"
#include "BankGuild/AxisParamBank.h"
//20160921 kw
#include "Motion/MotionNMC2.h"
#include "BankGuild/TeachBank.h"

enum MP2100_THREAD
{
	// MP2100 �Լ��� ȣ��� Thread ������ŭ ����
	// �� Thread���� ���� Open����� �Ѵ�
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
	// MP2100 BOARD ����
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
	POS_EQUAL=0,	// ���� ��ġ�� ����	(Motor ���� ���ε� ���� Ȯ���Ѵ�)
	POS_LOWER,		// ���� ��ġ���� �۰ų� ���� (Motor ������ Ȯ�� ���Ѵ�)
	POS_UPPER,		// ���� ��ġ���� ���ų� ���� (Motor ������ Ȯ�� ���Ѵ�)
};

enum	TURN_MODE	
{
	MMtoDeg,		//��ġ�� �о� �ö�
	DegToMM,		//�̵� ��� ������
};
class AxisMoveParam	// �̵���ɿ� ���� �Ķ����. �� �̵��Լ����� �ʿ��� �Ÿ� ���پ���
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
	double dAcc;//����
	double dDec;//����
	double dSpeed;//�ӵ�
	double dTargetPosition;//��ġ

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
	//void SetPosition(MP2100_THREAD thr, double dPosition);	// ������ ���� ��ġ���� �ٲ۴�
	void SetPosition(MP2100_THREAD thr, AXIS_ID axis, double dPosition);	// //20160923 kw ������ �ʿ��Ͽ� �߰�

	BOOL AxisMove(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param);
	BOOL AxisMoveInc(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param);
	BOOL AxisMoveJog(MP2100_THREAD thr, AXIS_ID axis, BOOL bForward, AxisMoveParam param);
	BOOL OriginRetrun(MP2100_THREAD thr, AXIS_ID axis);

	void AxisStop(MP2100_THREAD thr, AXIS_ID axis);			// ���� ���� �� ����
	void AxisStopEMS(MP2100_THREAD thr, AXIS_ID axis);		// ������

	AxisStatus GetAxisStatus(MP2100_THREAD thr, AXIS_ID axis);
	CString GetAlarmCode(MP2100_THREAD thr, AXIS_ID axis);	// Ȥ�� ��ü���� 10����, 16���� �� ǥ�� ���̰� ������� ���ڿ��� ó���Ѵ�
	double GetTorque(MP2100_THREAD thr, AXIS_ID axis);		// int? double?
	BOOL GetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos);
	BOOL GetTargetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos);	// �̵��� ��ǥ��ġ

	BOOL IsAxisReady(MP2100_THREAD thr, AXIS_ID axis, BOOL bIgnoreOrigin);	// �ش� ���� ���� ���� �� �ִ� �������� Ȯ��
	BOOL IsTrouble(MP2100_THREAD thr, AXIS_ID axis);								// �࿡ ������ �ִ��� ������ Ȯ��(Busy�ʹ� ������� Alarm�̳� Limit ���� ������Ҹ� Ȯ��)

	double m_TargetPos[AXIS_ID_MAX];
	double		m_dAcctype;
	//kjpark 20161019 MainFrame timer �߰� - ���Ͷ�, Ÿ������ ��Ʈ��
	BOOL	m_bTurnTableHomeFinish;
	// ƼĪ�� ����
	BOOL TeachMove(MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dOffset = 0.);
	BOOL CheckAxisPosition(MP2100_THREAD thr, AXIS_ID axis, double dCheckPos, double dRange = 0.05, POS_RANGE_COMPAIR rangeCompair = POS_EQUAL );
	BOOL CheckTeachPosition(MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dRange = 0.05, POS_RANGE_COMPAIR rangeCompair = POS_EQUAL, double dOffset = 0.);
	BOOL CheckPosition(double dDefaultPos, double dNowPos, double dCheckLimit /*= 0.01*/ );

	double ConvertPosition(int nMode, double dbPos, BOOL bVision, double dbOriginPos = 0.0);
};

