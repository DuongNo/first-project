#include "StdAfx.h"
#include "AxisParamBank.h"
#include "CheetahApp.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CAxisParamBank::CAxisParamBank(void)
{
}


CAxisParamBank::~CAxisParamBank(void)
{
}

void CAxisParamBank::InitializeAxisType()
{
#if(AMT_TYPE == 1)
	// 파일 Load/Save를 하지 않을 항목들을 여기서 정의한다
 	//////////////////////////////////////////////////////////////////////////
	////MP2142 #1
	m_Axis[AXIS_SVO_LOAD_CONV				].m_strAxisName = _T("LOAD_CONV");
	m_Axis[AXIS_SVO_LOAD_CONV				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LOAD_CONV				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LOAD_CONV				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LOAD_CONV				].m_nInnerNo = 0x00;

	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_strAxisName = _T("LD_UPPERSHUTTLE_Y");
	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_nInnerNo = 0x01;

	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_strAxisName = _T("LD_LOWERSHUTTLE_Y");
	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_nInnerNo = 0x02;

	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_strAxisName = _T("PREALIGN_TR_Y");
	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_nInnerNo = 0x03;

	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_strAxisName = _T("PREALIGN_TR_Z");
	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_nInnerNo = 0x04;

	m_Axis[AXIS_SVO_PA_STAGE_X		].m_strAxisName = _T("PA_STAGE_X");
	m_Axis[AXIS_SVO_PA_STAGE_X		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_PA_STAGE_X		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_PA_STAGE_X		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_PA_STAGE_X		].m_nInnerNo = 0x05;

	m_Axis[AXIS_SVO_LD_TR_X		].m_strAxisName = _T("LD_TR_X");
	m_Axis[AXIS_SVO_LD_TR_X		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LD_TR_X		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LD_TR_X		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LD_TR_X		].m_nInnerNo = 0x06;

	m_Axis[AXIS_SVO_LD_TR_Z		].m_strAxisName = _T("LD_TR_Z");
	m_Axis[AXIS_SVO_LD_TR_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LD_TR_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LD_TR_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LD_TR_Z		].m_nInnerNo = 0x07;

	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_strAxisName = _T("ALIGN_CAM_Y");
	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_nInnerNo = 0x08;

	m_Axis[AXIS_SVO_INS_X				].m_strAxisName = _T("INS_X");
	m_Axis[AXIS_SVO_INS_X				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_INS_X				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_INS_X				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_INS_X				].m_nInnerNo = 0x09;

	m_Axis[AXIS_SVO_INS_CH1_Z				].m_strAxisName = _T("INS_CH1_Z");
	m_Axis[AXIS_SVO_INS_CH1_Z				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_INS_CH1_Z				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_INS_CH1_Z				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_INS_CH1_Z				].m_nInnerNo = 0x0A;

	m_Axis[AXIS_SVO_INS_CH2_Z				].m_strAxisName = _T("INS_CH2_Z");
	m_Axis[AXIS_SVO_INS_CH2_Z				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_INS_CH2_Z				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_INS_CH2_Z				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_INS_CH2_Z				].m_nInnerNo = 0x0B;



	////MP2142 #2
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y			].m_strAxisName = _T("GOOD_TRAY_TR_Y");
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y			].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y			].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y			].m_nInnerNo = 0x10;

	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z			].m_strAxisName = _T("GOOD_TRAY_TR_Z");
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z			].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z			].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z			].m_nInnerNo = 0x11;

	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_strAxisName = _T("GOOD_TRAY_OUT_LIFT_Z");
	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_nInnerNo = 0x12;

	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_strAxisName = _T("GOOD_TRAY_IN_LIFT_Z");
	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_nInnerNo = 0x13;

	m_Axis[AXIS_SVO_ULD_TR_X				].m_strAxisName = _T("ULD_TR_X");
	m_Axis[AXIS_SVO_ULD_TR_X				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_ULD_TR_X				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_ULD_TR_X				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_ULD_TR_X				].m_nInnerNo = 0x14;

	m_Axis[AXIS_SVO_ULD_TR_Z				].m_strAxisName = _T("ULD_TR_Z");
	m_Axis[AXIS_SVO_ULD_TR_Z				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_ULD_TR_Z				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_ULD_TR_Z				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_ULD_TR_Z				].m_nInnerNo = 0x15;

	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_strAxisName = _T("NG_TRAY_IN_LIFT_Z");
	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_nInnerNo = 0x16;

	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_strAxisName = _T("NG_TRAY_OUT_LIFT_Z");
	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_nInnerNo = 0x17;

	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_strAxisName = _T("NG_TRAY_TR_X");
	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_nInnerNo = 0x18;

	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_strAxisName = _T("NG_TRAY_TR_Z");
	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_nInnerNo = 0x19;

	// NMC(Not Servo Axis)
	// NMC #1
	m_Axis[AXIS_STP_PA_CH1_X		].m_strAxisName = _T("PA_CH1_X");
	m_Axis[AXIS_STP_PA_CH1_X		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH1_X		].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH1_X		].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH1_X		].m_nInnerNo = 0;

	m_Axis[AXIS_STP_PA_CH2_X		].m_strAxisName = _T("PA_CH2_X");
	m_Axis[AXIS_STP_PA_CH2_X		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH2_X		].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH2_X		].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH2_X		].m_nInnerNo = 1;

	m_Axis[AXIS_STP_PA_CH1_Y	].m_strAxisName = _T("PA_CH1_Y");
	m_Axis[AXIS_STP_PA_CH1_Y	].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH1_Y	].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH1_Y	].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH1_Y	].m_nInnerNo = 2;

	m_Axis[AXIS_STP_PA_CH2_Y		].m_strAxisName = _T("PA_CH2_Y");
	m_Axis[AXIS_STP_PA_CH2_Y		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH2_Y		].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH2_Y		].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH2_Y		].m_nInnerNo = 3;

	m_Axis[AXIS_STP_PA_CH1_T	].m_strAxisName = _T("PA_CH1_T");
	m_Axis[AXIS_STP_PA_CH1_T	].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH1_T	].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[AXIS_STP_PA_CH1_T	].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH1_T	].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH1_T	].m_nInnerNo = 4;

	m_Axis[AXIS_STP_PA_CH2_T		].m_strAxisName = _T("PA_CH2_T");
	m_Axis[AXIS_STP_PA_CH2_T		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH2_T		].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[AXIS_STP_PA_CH2_T		].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH2_T		].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH2_T		].m_nInnerNo = 5;

	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_strAxisName = _T("TURN_TABLE_T");
	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_nInnerNo = 6;

	// NMC(Not Servo Axis)
	// NMC #2
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_strAxisName = _T("ALIGN_CAM_X");
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_nInnerNo = 0;

	m_Axis[AXIS_STP_RESERVED				].m_strAxisName = _T("RESERVED");
	m_Axis[AXIS_STP_RESERVED				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_RESERVED				].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_RESERVED				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_RESERVED				].m_nInnerNo = 1;

	m_Axis[AXIS_STP_INS_CH1_T				].m_strAxisName = _T("INS_CH1_T");
	m_Axis[AXIS_STP_INS_CH1_T				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_INS_CH1_T				].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[AXIS_STP_INS_CH1_T				].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_INS_CH1_T				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_INS_CH1_T				].m_nInnerNo = 2;

	m_Axis[AXIS_STP_INS_CH2_T				].m_strAxisName = _T("INS_CH2_T");
	m_Axis[AXIS_STP_INS_CH2_T				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_INS_CH2_T				].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[AXIS_STP_INS_CH2_T				].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_INS_CH2_T				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_INS_CH2_T				].m_nInnerNo = 3;

	m_Axis[AXIS_STP_INS_CH1_F				].m_strAxisName = _T("INS_CH1_F");
	m_Axis[AXIS_STP_INS_CH1_F				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_INS_CH1_F				].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_INS_CH1_F				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_INS_CH1_F				].m_nInnerNo = 4;

	m_Axis[AXIS_STP_INS_CH2_F		].m_strAxisName = _T("INS_CH2_F");
	m_Axis[AXIS_STP_INS_CH2_F		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_INS_CH2_F		].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_INS_CH2_F		].m_nModuleNo = 1;
	m_Axis[AXIS_STP_INS_CH2_F		].m_nInnerNo = 5;
#else
	// 파일 Load/Save를 하지 않을 항목들을 여기서 정의한다
 	//////////////////////////////////////////////////////////////////////////
	////MP2142 #1
	m_Axis[AXIS_SVO_LOAD_CONV				].m_strAxisName = _T("LOAD_CONV");
	m_Axis[AXIS_SVO_LOAD_CONV				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LOAD_CONV				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LOAD_CONV				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LOAD_CONV				].m_nInnerNo = 0x00;

	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_strAxisName = _T("LD_UPPERSHUTTLE_Y");
	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LD_UPPERSHUTTLE_Y				].m_nInnerNo = 0x01;

	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_strAxisName = _T("LD_LOWERSHUTTLE_Y");
	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LD_LOWERSHUTTLE_Y				].m_nInnerNo = 0x02;

	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_strAxisName = _T("PREALIGN_TR_Y");
	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_PREALIGN_TR_Y				].m_nInnerNo = 0x03;

	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_strAxisName = _T("PREALIGN_TR_Z");
	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_PREALIGN_TR_Z		].m_nInnerNo = 0x04;

	m_Axis[AXIS_SVO_PA_STAGE_X		].m_strAxisName = _T("PA_STAGE_X");
	m_Axis[AXIS_SVO_PA_STAGE_X		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_PA_STAGE_X		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_PA_STAGE_X		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_PA_STAGE_X		].m_nInnerNo = 0x05;

	m_Axis[AXIS_SVO_LD_TR_X		].m_strAxisName = _T("LD_TR_X");
	m_Axis[AXIS_SVO_LD_TR_X		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LD_TR_X		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LD_TR_X		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LD_TR_X		].m_nInnerNo = 0x06;

	m_Axis[AXIS_SVO_LD_TR_Z		].m_strAxisName = _T("LD_TR_Z");
	m_Axis[AXIS_SVO_LD_TR_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_LD_TR_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_LD_TR_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_LD_TR_Z		].m_nInnerNo = 0x07;

	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_strAxisName = _T("ALIGN_CAM_Y");
	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_ALIGN_CAM_Y		].m_nInnerNo = 0x08;

	m_Axis[AXIS_SVO_INS_X				].m_strAxisName = _T("INS_X");
	m_Axis[AXIS_SVO_INS_X				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_INS_X				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_INS_X				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_INS_X				].m_nInnerNo = 0x09;

	m_Axis[AXIS_SVO_INS_CH1_Z				].m_strAxisName = _T("INS_CH1_Z");
	m_Axis[AXIS_SVO_INS_CH1_Z				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_INS_CH1_Z				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_INS_CH1_Z				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_INS_CH1_Z				].m_nInnerNo = 0x0A;

	m_Axis[AXIS_SVO_INS_CH2_Z				].m_strAxisName = _T("INS_CH2_Z");
	m_Axis[AXIS_SVO_INS_CH2_Z				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_INS_CH2_Z				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_INS_CH2_Z				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_INS_CH2_Z				].m_nInnerNo = 0x0B;



	////MP2142 #2
	m_Axis[AXIS_SVO_ULD_TR_X			].m_strAxisName = _T("ULD_TR_X");
	m_Axis[AXIS_SVO_ULD_TR_X			].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_ULD_TR_X			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_ULD_TR_X			].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_ULD_TR_X			].m_nInnerNo = 0x10;

	m_Axis[AXIS_SVO_ULD_TR_Z			].m_strAxisName = _T("ULD_TR_Z");
	m_Axis[AXIS_SVO_ULD_TR_Z			].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_ULD_TR_Z			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_ULD_TR_Z			].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_ULD_TR_Z			].m_nInnerNo = 0x11;

	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_strAxisName = _T("GOOD_TRAY_OUT_LIFT_Z");
	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		].m_nInnerNo = 0x12;

	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_strAxisName = _T("GOOD_TRAY_IN_LIFT_Z");
	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z		].m_nInnerNo = 0x13;

	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y				].m_strAxisName = _T("GOOD_TRAY_TR_Y");
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Y				].m_nInnerNo = 0x14;

	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z				].m_strAxisName = _T("GOOD_TRAY_TR_Z");
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_GOOD_TRAY_TR_Z				].m_nInnerNo = 0x15;

	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_strAxisName = _T("NG_TRAY_IN_LIFT_Z");
	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z		].m_nInnerNo = 0x16;

	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_strAxisName = _T("NG_TRAY_OUT_LIFT_Z");
	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z		].m_nInnerNo = 0x17;

	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_strAxisName = _T("NG_TRAY_TR_X");
	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_NG_TRAY_TR_X				].m_nInnerNo = 0x18;

	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_strAxisName = _T("NG_TRAY_TR_Z");
	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_DeviceType = CAxisParam::DEVICE_TYPE_MP2100;
	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_NG_TRAY_TR_Z				].m_nInnerNo = 0x19;

	// NMC(Not Servo Axis)
	// NMC #1
	m_Axis[AXIS_STP_PA_CH1_X		].m_strAxisName = _T("PA_CH1_X");
	m_Axis[AXIS_STP_PA_CH1_X		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH1_X		].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH1_X		].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH1_X		].m_nInnerNo = 0;

	m_Axis[AXIS_STP_PA_CH2_X		].m_strAxisName = _T("PA_CH2_X");
	m_Axis[AXIS_STP_PA_CH2_X		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH2_X		].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH2_X		].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH2_X		].m_nInnerNo = 1;

	m_Axis[AXIS_STP_PA_CH1_Y	].m_strAxisName = _T("PA_CH1_Y");
	m_Axis[AXIS_STP_PA_CH1_Y	].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH1_Y	].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH1_Y	].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH1_Y	].m_nInnerNo = 2;

	m_Axis[AXIS_STP_PA_CH2_Y		].m_strAxisName = _T("PA_CH2_Y");
	m_Axis[AXIS_STP_PA_CH2_Y		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH2_Y		].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH2_Y		].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH2_Y		].m_nInnerNo = 3;

	m_Axis[AXIS_STP_PA_CH1_T	].m_strAxisName = _T("PA_CH1_T");
	m_Axis[AXIS_STP_PA_CH1_T	].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH1_T	].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[AXIS_STP_PA_CH1_T	].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH1_T	].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH1_T	].m_nInnerNo = 4;

	m_Axis[AXIS_STP_PA_CH2_T		].m_strAxisName = _T("PA_CH2_T");
	m_Axis[AXIS_STP_PA_CH2_T		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_PA_CH2_T		].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[AXIS_STP_PA_CH2_T		].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_PA_CH2_T		].m_nModuleNo = 0;
	m_Axis[AXIS_STP_PA_CH2_T		].m_nInnerNo = 5;

	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_strAxisName = _T("TURN_TABLE_T");
	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_nModuleNo = 0;
	m_Axis[AXIS_SVO_TURN_TABLE_T	].m_nInnerNo = 6;

	// NMC(Not Servo Axis)
	// NMC #2
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_strAxisName = _T("ALIGN_CAM_X");
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_ALIGN_CAM_X				].m_nInnerNo = 0;

	m_Axis[AXIS_STP_RESERVED				].m_strAxisName = _T("RESERVED");
	m_Axis[AXIS_STP_RESERVED				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_RESERVED				].m_MotorType = CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_RESERVED				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_RESERVED				].m_nInnerNo = 1;

	m_Axis[AXIS_STP_INS_CH1_T				].m_strAxisName = _T("INS_CH1_T");
	m_Axis[AXIS_STP_INS_CH1_T				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_INS_CH1_T				].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[AXIS_STP_INS_CH1_T				].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_INS_CH1_T				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_INS_CH1_T				].m_nInnerNo = 2;

	m_Axis[AXIS_STP_INS_CH2_T				].m_strAxisName = _T("INS_CH2_T");
	m_Axis[AXIS_STP_INS_CH2_T				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_INS_CH2_T				].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[AXIS_STP_INS_CH2_T				].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_INS_CH2_T				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_INS_CH2_T				].m_nInnerNo = 3;

	m_Axis[AXIS_STP_INS_CH1_F				].m_strAxisName = _T("INS_CH1_F");
	m_Axis[AXIS_STP_INS_CH1_F				].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_INS_CH1_F				].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_INS_CH1_F				].m_nModuleNo = 1;
	m_Axis[AXIS_STP_INS_CH1_F				].m_nInnerNo = 4;

	m_Axis[AXIS_STP_INS_CH2_F		].m_strAxisName = _T("INS_CH2_F");
	m_Axis[AXIS_STP_INS_CH2_F		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[AXIS_STP_INS_CH2_F		].m_MotorType = CAxisParam::MOTOR_TYPE_EZ_SERVO;//CAxisParam::MOTOR_TYPE_STEP;
	m_Axis[AXIS_STP_INS_CH2_F		].m_nModuleNo = 1;
	m_Axis[AXIS_STP_INS_CH2_F		].m_nInnerNo = 5;
#endif
}

void CAxisParamBank::ReadAxisParam()
{
	CString strPath;
	strPath.Format(_T("%s\\Axis.ini"), theConfigBank.m_System.m_strConfigPath);
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		m_Axis[i].ReadFile(strPath);
	}
}

void CAxisParamBank::SaveAxisParam()
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath, strECIDPath;
	strPath.Format(_T("%s\\Axis.ini"), theConfigBank.m_System.m_strConfigPath);
	strECIDPath.Format(_T("%s\\CheetahECID.ini"), theConfigBank.m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\%d_%d\\Axis_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.Format(_T("%H%M%S")));
	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}
	strFolderPath = CFileSupport::GetPathName(strECIDPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}
	strFolderPath = CFileSupport::GetPathName(strBackupPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}
	if(CFileSupport::FileCheck(strECIDPath))
	{
		// 기존 파일 백업
		::CopyFile(strECIDPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strECIDPath);
	}

	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		m_Axis[i].SaveFile(strPath);
		m_Axis[i].SaveECIDFile(strECIDPath);
	}
}
BOOL CAxisParamBank::MotorIsUsed(AXIS_ID id)
{
	if(id==AXIS_ID_MAX)
		return FALSE;


	CAxisParam	node;
	node = m_Axis[id];

	return node.m_bUse;
}
BOOL CAxisParamBank::GetParamNode(AXIS_ID id, CAxisParam& node)
{
	if(id==AXIS_ID_MAX)
		return FALSE;

	node = m_Axis[id];

	return TRUE;
}