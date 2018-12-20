#include "StdAfx.h"
#include "TeachBank.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"
#include "CheetahApp.h"
#include "AxisParamBank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

TEACH_PARAM::TEACH_PARAM( void )
{
	Init();
}

TEACH_PARAM::TEACH_PARAM( CString name, AXIS_ID axis, TEACH_PARAM::ID teach, double pos, double speed, double acc, double dec )
{
	strName = name;
	idAxis = axis;
	idTeach = teach;
	dPos = pos;
	dSpeed = speed;
	dAcc = acc;
	dDec = dec;
}

TEACH_PARAM::~TEACH_PARAM( void )
{
}

void TEACH_PARAM::Init()
{
	strName = _T("");;
	idAxis = AXIS_ID_MAX;
	idTeach = TEACH_PARAM::PARAM_MAX;
	dPos = 0.;
	dSpeed = 0.;
	dAcc = ACC_300ms;
	dDec = ACC_300ms;
}

void TEACH_PARAM::ReadFile( CString strFilePath )
{
	// 파일 입출력은 티칭값과 속도만 한다.
	CIni ini(strFilePath);
	dPos = ini.GetDouble(strName, _T("POSITION"), 0.);
	dSpeed = ini.GetDouble(strName, _T("SPEED"), 0.);
}

void TEACH_PARAM::SaveFile( CString strFilePath )
{
	CAxisParam	m_Axis[AXIS_ID_MAX];
	// 파일 입출력은 티칭값과 속도속만 한다.
	CIni ini(strFilePath);
	ini.WriteDouble(strName, _T("POSITION"), dPos);
	ini.WriteDouble(strName, _T("SPEED"), dSpeed);
}

CTeachBank::CTeachBank(void)
{
}


CTeachBank::~CTeachBank(void)
{
}

void CTeachBank::GetTeachParamList(int iAxis, CArray<TEACH_PARAM,TEACH_PARAM> &arr)
{
	arr.RemoveAll();
	TEACH_PARAM param;
	for(int i = 0; i < TEACH_PARAM::PARAM_MAX; i++ )
	{
		param = GetTeachParam((TEACH_PARAM::ID)i);
		if (param.idAxis == iAxis )
			arr.Add(param);
	}
}

void CTeachBank::InitializeTeachParam()
{
	// 여기서 티칭값 명칭과, ID, 가속도, 감속도를 지정해준다.
	// 배열순서 : 티칭위치 이름,	ID, 티칭값,	속도,		가속도,	감속도

	// Loader
	// Robot
	m_Param[TEACH_PARAM::LD_ROBOT_CONV						] = TEACH_PARAM(_T("LD_ROBOT_CONV"						), AXIS_SVO_LOAD_CONV					, TEACH_PARAM::LD_ROBOT_CONV					, 	 0., 0., ACC_250ms, ACC_250ms);

	// Shuttle
	m_Param[TEACH_PARAM::LD_UPPERSHUTTLE_LOAD				] = TEACH_PARAM(_T("LD_UPPERSHUTTLE_LOAD"				), AXIS_SVO_LD_UPPERSHUTTLE_Y			, TEACH_PARAM::LD_UPPERSHUTTLE_LOAD				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_UPPERSHUTTLE_UNLOAD				] = TEACH_PARAM(_T("LD_UPPERSHUTTLE_UNLOAD"				), AXIS_SVO_LD_UPPERSHUTTLE_Y			, TEACH_PARAM::LD_UPPERSHUTTLE_UNLOAD			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_LOWERSHUTTLE_LOAD				] = TEACH_PARAM(_T("LD_LOWERSHUTTLE_LOAD"				), AXIS_SVO_LD_LOWERSHUTTLE_Y			, TEACH_PARAM::LD_LOWERSHUTTLE_LOAD				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_LOWERSHUTTLE_UNLOAD				] = TEACH_PARAM(_T("LD_LOWERSHUTTLE_UNLOAD"				), AXIS_SVO_LD_LOWERSHUTTLE_Y			, TEACH_PARAM::LD_LOWERSHUTTLE_UNLOAD			, 	 0., 0., ACC_250ms, ACC_250ms);

	// Prealign TR
	m_Param[TEACH_PARAM::PA_TR_Y_GET						] = TEACH_PARAM(_T("PA_TR_Y_GET"						), AXIS_SVO_PREALIGN_TR_Y				, TEACH_PARAM::PA_TR_Y_GET						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_TR_Y_PUT						] = TEACH_PARAM(_T("PA_TR_Y_PUT"						), AXIS_SVO_PREALIGN_TR_Y				, TEACH_PARAM::PA_TR_Y_PUT						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_TR_Y_WAIT						] = TEACH_PARAM(_T("PA_TR_Y_WAIT"						), AXIS_SVO_PREALIGN_TR_Y				, TEACH_PARAM::PA_TR_Y_WAIT						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_TR_Z_UPPER_GET					] = TEACH_PARAM(_T("PA_TR_Z_UPPER_GET"					), AXIS_SVO_PREALIGN_TR_Z				, TEACH_PARAM::PA_TR_Z_UPPER_GET				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_TR_Z_LOWER_GET					] = TEACH_PARAM(_T("PA_TR_Z_LOWER_GET"					), AXIS_SVO_PREALIGN_TR_Z				, TEACH_PARAM::PA_TR_Z_LOWER_GET				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_TR_Z_PUT						] = TEACH_PARAM(_T("PA_TR_Z_PUT"						), AXIS_SVO_PREALIGN_TR_Z				, TEACH_PARAM::PA_TR_Z_PUT						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_TR_Z_UP							] = TEACH_PARAM(_T("PA_TR_Z_UP"							), AXIS_SVO_PREALIGN_TR_Z				, TEACH_PARAM::PA_TR_Z_UP						, 	 0., 0., ACC_250ms, ACC_250ms);
	
	////////Pre_Align
	m_Param[TEACH_PARAM::PA_CH1_X							] = TEACH_PARAM(_T("PA_CH1_X"							), AXIS_STP_PA_CH1_X					, TEACH_PARAM::PA_CH1_X							, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_CH1_Y							] = TEACH_PARAM(_T("PA_CH1_Y"							), AXIS_STP_PA_CH1_Y					, TEACH_PARAM::PA_CH1_Y							, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_CH1_T							] = TEACH_PARAM(_T("PA_CH1_T"							), AXIS_STP_PA_CH1_T					, TEACH_PARAM::PA_CH1_T							, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_CH2_X							] = TEACH_PARAM(_T("PA_CH2_X"							), AXIS_STP_PA_CH2_X					, TEACH_PARAM::PA_CH2_X							, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_CH2_Y							] = TEACH_PARAM(_T("PA_CH2_Y"							), AXIS_STP_PA_CH2_Y					, TEACH_PARAM::PA_CH2_Y							, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_CH2_T							] = TEACH_PARAM(_T("PA_CH2_T"							), AXIS_STP_PA_CH2_T					, TEACH_PARAM::PA_CH2_T							, 	 0., 0., ACC_250ms, ACC_250ms);

	m_Param[TEACH_PARAM::PA_STAGE_LOAD_POS					] = TEACH_PARAM(_T("PA_STAGE_LOAD_POS"			), AXIS_SVO_PA_STAGE_X					, TEACH_PARAM::PA_STAGE_LOAD_POS				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_STAGE_ALIGN_POS					] = TEACH_PARAM(_T("PA_STAGE_ALIGN_POS"			), AXIS_SVO_PA_STAGE_X					, TEACH_PARAM::PA_STAGE_ALIGN_POS				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::PA_STAGE_UNLOAD_POS				] = TEACH_PARAM(_T("PA_STAGE_UNLOAD_POS"			), AXIS_SVO_PA_STAGE_X					, TEACH_PARAM::PA_STAGE_UNLOAD_POS				, 	 0., 0., ACC_250ms, ACC_250ms);

	// Loader TR
	m_Param[TEACH_PARAM::LD_TR_X_GET						] = TEACH_PARAM(_T("LD_TR_X_GET"						), AXIS_SVO_LD_TR_X						, TEACH_PARAM::LD_TR_X_GET						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_X_PUT						] = TEACH_PARAM(_T("LD_TR_X_PUT"						), AXIS_SVO_LD_TR_X						, TEACH_PARAM::LD_TR_X_PUT						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_X_CH1_LOWER_MCR				] = TEACH_PARAM(_T("LD_TR_X_CH1_LOWER_MCR"				), AXIS_SVO_LD_TR_X						, TEACH_PARAM::LD_TR_X_CH1_LOWER_MCR			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_X_CH2_LOWER_MCR				] = TEACH_PARAM(_T("LD_TR_X_CH2_LOWER_MCR"				), AXIS_SVO_LD_TR_X						, TEACH_PARAM::LD_TR_X_CH2_LOWER_MCR			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_X_CH1_UPPER_MCR				] = TEACH_PARAM(_T("LD_TR_X_CH1_UPPER_MCR"				), AXIS_SVO_LD_TR_X						, TEACH_PARAM::LD_TR_X_CH1_UPPER_MCR			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_X_CH2_UPPER_MCR				] = TEACH_PARAM(_T("LD_TR_X_CH2_UPPER_MCR"				), AXIS_SVO_LD_TR_X						, TEACH_PARAM::LD_TR_X_CH2_UPPER_MCR			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_Z_GET						] = TEACH_PARAM(_T("LD_TR_Z_GET"						), AXIS_SVO_LD_TR_Z						, TEACH_PARAM::LD_TR_Z_GET						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_Z_PUT						] = TEACH_PARAM(_T("LD_TR_Z_PUT"						), AXIS_SVO_LD_TR_Z						, TEACH_PARAM::LD_TR_Z_PUT						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_Z_UP							] = TEACH_PARAM(_T("LD_TR_Z_UP"							), AXIS_SVO_LD_TR_Z						, TEACH_PARAM::LD_TR_Z_UP						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_Z_LOWER_MCR					] = TEACH_PARAM(_T("LD_TR_Z_LOWER_MCR"					), AXIS_SVO_LD_TR_Z						, TEACH_PARAM::LD_TR_Z_LOWER_MCR				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::LD_TR_Z_UPPER_MCR					] = TEACH_PARAM(_T("LD_TR_Z_UPPER_MCR"					), AXIS_SVO_LD_TR_Z						, TEACH_PARAM::LD_TR_Z_UPPER_MCR				, 	 0., 0., ACC_250ms, ACC_250ms);

	///////Turn Table
	m_Param[TEACH_PARAM::TT_VALIGN_CAM_Y_READY				] = TEACH_PARAM(_T("TT_VALIGN_CAM_Y_READY"				), AXIS_SVO_ALIGN_CAM_Y					, TEACH_PARAM::TT_VALIGN_CAM_Y_READY			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_VALIGN_CAM_X_READY				] = TEACH_PARAM(_T("TT_VALIGN_CAM_X_READY"				), AXIS_STP_ALIGN_CAM_X					, TEACH_PARAM::TT_VALIGN_CAM_X_READY			, 	 0., 0., ACC_250ms, ACC_250ms);

	m_Param[TEACH_PARAM::TT_VALIGN_CAM_CH1_Y1				] = TEACH_PARAM(_T("TT_VALIGN_CAM_CH1_Y1"				), AXIS_SVO_ALIGN_CAM_Y					, TEACH_PARAM::TT_VALIGN_CAM_CH1_Y1				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_VALIGN_CAM_CH1_Y2				] = TEACH_PARAM(_T("TT_VALIGN_CAM_CH1_Y2"				), AXIS_SVO_ALIGN_CAM_Y					, TEACH_PARAM::TT_VALIGN_CAM_CH1_Y2				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_VALIGN_CAM_CH1_X1				] = TEACH_PARAM(_T("TT_VALIGN_CAM_CH1_X1"				), AXIS_STP_ALIGN_CAM_X					, TEACH_PARAM::TT_VALIGN_CAM_CH1_X1				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_VALIGN_CAM_CH1_X2				] = TEACH_PARAM(_T("TT_VALIGN_CAM_CH1_X2"				), AXIS_STP_ALIGN_CAM_X					, TEACH_PARAM::TT_VALIGN_CAM_CH1_X2				, 	 0., 0., ACC_250ms, ACC_250ms);

	m_Param[TEACH_PARAM::TT_VALIGN_CAM_CH2_Y1				] = TEACH_PARAM(_T("TT_VALIGN_CAM_CH2_Y1"				), AXIS_SVO_ALIGN_CAM_Y					, TEACH_PARAM::TT_VALIGN_CAM_CH2_Y1				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_VALIGN_CAM_CH2_Y2				] = TEACH_PARAM(_T("TT_VALIGN_CAM_CH2_Y2"				), AXIS_SVO_ALIGN_CAM_Y					, TEACH_PARAM::TT_VALIGN_CAM_CH2_Y2				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_VALIGN_CAM_CH2_X1				] = TEACH_PARAM(_T("TT_VALIGN_CAM_CH2_X1"				), AXIS_STP_ALIGN_CAM_X					, TEACH_PARAM::TT_VALIGN_CAM_CH2_X1				, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_VALIGN_CAM_CH2_X2				] = TEACH_PARAM(_T("TT_VALIGN_CAM_CH2_X2"				), AXIS_STP_ALIGN_CAM_X					, TEACH_PARAM::TT_VALIGN_CAM_CH2_X2				, 	 0., 0., ACC_250ms, ACC_250ms);

	m_Param[TEACH_PARAM::TT_INSP_CAM_X						] = TEACH_PARAM(_T("TT_INSP_CAM_X"						), AXIS_SVO_INS_X						, TEACH_PARAM::TT_INSP_CAM_X					, 	 0., 0., ACC_250ms, ACC_250ms);

	m_Param[TEACH_PARAM::TT_INSP_CH1_CAM_ZOOM_Z				] = TEACH_PARAM(_T("TT_INSP_CH1_CAM_ZOOM_Z"				), AXIS_SVO_INS_CH1_Z					, TEACH_PARAM::TT_INSP_CH1_CAM_ZOOM_Z			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_INSP_CH1_CAM_FOCUS_F			] = TEACH_PARAM(_T("TT_INSP_CH1_CAM_FOCUS_F"			), AXIS_STP_INS_CH1_F					, TEACH_PARAM::TT_INSP_CH1_CAM_FOCUS_F			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_INSP_CH1_CAM_TURN_T				] = TEACH_PARAM(_T("TT_INSP_CH1_CAM_TURN_T"				), AXIS_STP_INS_CH1_T					, TEACH_PARAM::TT_INSP_CH1_CAM_TURN_T			, 	 0., 0., ACC_250ms, ACC_250ms);

	m_Param[TEACH_PARAM::TT_INSP_CH2_CAM_ZOOM_Z				] = TEACH_PARAM(_T("TT_INSP_CH2_CAM_ZOOM_Z"				), AXIS_SVO_INS_CH2_Z					, TEACH_PARAM::TT_INSP_CH2_CAM_ZOOM_Z			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_INSP_CH2_CAM_FOCUS_F			] = TEACH_PARAM(_T("TT_INSP_CH2_CAM_FOCUS_F"			), AXIS_STP_INS_CH2_F					, TEACH_PARAM::TT_INSP_CH2_CAM_FOCUS_F			, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::TT_INSP_CH2_CAM_TURN_T				] = TEACH_PARAM(_T("TT_INSP_CH2_CAM_TURN_T"				), AXIS_STP_INS_CH2_T					, TEACH_PARAM::TT_INSP_CH2_CAM_TURN_T			, 	 0., 0., ACC_250ms, ACC_250ms);

	m_Param[TEACH_PARAM::WORK_TABLE_T_TO_WORK					] = TEACH_PARAM(_T("WORK_TABLE_T_TO_WORK"					), AXIS_SVO_TURN_TABLE_T				, TEACH_PARAM::WORK_TABLE_T_TO_WORK				, 	 0., 0., ACC_250ms, ACC_250ms);

	// UnLoader TR
	m_Param[TEACH_PARAM::ULD_TR_X_GET						] = TEACH_PARAM(_T("ULD_TR_X_GET"						), AXIS_SVO_ULD_TR_X					, TEACH_PARAM::ULD_TR_X_GET						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::ULD_TR_X_PUT						] = TEACH_PARAM(_T("ULD_TR_X_PUT"						), AXIS_SVO_ULD_TR_X					, TEACH_PARAM::ULD_TR_X_PUT						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::ULD_TR_Z_GET						] = TEACH_PARAM(_T("ULD_TR_Z_GET"						), AXIS_SVO_ULD_TR_Z					, TEACH_PARAM::ULD_TR_Z_GET						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::ULD_TR_Z_PUT						] = TEACH_PARAM(_T("ULD_TR_Z_PUT"						), AXIS_SVO_ULD_TR_Z					, TEACH_PARAM::ULD_TR_Z_PUT						, 	 0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::ULD_TR_Z_UP						] = TEACH_PARAM(_T("ULD_TR_Z_UP"						), AXIS_SVO_ULD_TR_Z					, TEACH_PARAM::ULD_TR_Z_UP						, 	 0., 0., ACC_250ms, ACC_250ms);
// GOOD TRAY, NG TRAY 

	m_Param[TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_UP			] = TEACH_PARAM(_T("OK_TRAY_IN_LIFT_Z_TO_UP"			), AXIS_SVO_GOOD_TRAY_IN_LIFT_Z			, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_UP			,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_DOWN			] = TEACH_PARAM(_T("OK_TRAY_IN_LIFT_Z_TO_DOWN"			), AXIS_SVO_GOOD_TRAY_IN_LIFT_Z			, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_DOWN		,  0., 0., ACC_250ms, ACC_250ms);
	
	//2017-12-21,SKCHO, 추가
	m_Param[TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_ALIGN			] = TEACH_PARAM(_T("OK_TRAY_IN_LIFT_Z_TO_ALIGN"			), AXIS_SVO_GOOD_TRAY_IN_LIFT_Z			, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_ALIGN		,  0., 0., ACC_250ms, ACC_250ms);
	
	//2018wjp
	m_Param[TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_TOP			] = TEACH_PARAM(_T("OK_TRAY_IN_LIFT_Z_TO_TOP"			), AXIS_SVO_GOOD_TRAY_IN_LIFT_Z			, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_TOP		,  0., 0., ACC_250ms, ACC_250ms);


	m_Param[TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_UP			] = TEACH_PARAM(_T("OK_TRAY_OUT_LIFT_Z_TO_UP"			), AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_UP			,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_DOWN			] = TEACH_PARAM(_T("OK_TRAY_OUT_LIFT_Z_TO_DOWN"			), AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_DOWN		,  0., 0., ACC_250ms, ACC_250ms);
	//2017-12-21,SKCHO, 추가
	m_Param[TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_ALIGN			] = TEACH_PARAM(_T("OK_TRAY_OUT_LIFT_Z_TO_ALIGN"			), AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z		, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_ALIGN		,  0., 0., ACC_250ms, ACC_250ms);


	m_Param[TEACH_PARAM::OK_TRAY_TR_Y_TO_IN					] = TEACH_PARAM(_T("OK_TRAY_TR_Y_TO_IN"					), AXIS_SVO_GOOD_TRAY_TR_Y				, TEACH_PARAM::OK_TRAY_TR_Y_TO_IN				,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::OK_TRAY_TR_Y_TO_OUT				] = TEACH_PARAM(_T("OK_TRAY_TR_Y_TO_OUT"				), AXIS_SVO_GOOD_TRAY_TR_Y				, TEACH_PARAM::OK_TRAY_TR_Y_TO_OUT				,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::OK_TRAY_TR_Z_TO_UP					] = TEACH_PARAM(_T("OK_TRAY_TR_Z_TO_UP"					), AXIS_SVO_GOOD_TRAY_TR_Z				, TEACH_PARAM::OK_TRAY_TR_Z_TO_UP				,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::OK_TRAY_TR_Z_TO_IN					] = TEACH_PARAM(_T("OK_TRAY_TR_Z_TO_IN"					), AXIS_SVO_GOOD_TRAY_TR_Z				, TEACH_PARAM::OK_TRAY_TR_Z_TO_IN				,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::OK_TRAY_TR_Z_TO_OUT				] = TEACH_PARAM(_T("OK_TRAY_TR_Z_TO_OUT"				), AXIS_SVO_GOOD_TRAY_TR_Z				, TEACH_PARAM::OK_TRAY_TR_Z_TO_OUT				,  0., 0., ACC_250ms, ACC_250ms);

	m_Param[TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_UP			] = TEACH_PARAM(_T("NG_TRAY_IN_LIFT_Z_TO_UP"			), AXIS_SVO_NG_TRAY_IN_LIFT_Z			, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_UP			,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_DOWN			] = TEACH_PARAM(_T("NG_TRAY_IN_LIFT_Z_TO_DOWN"			), AXIS_SVO_NG_TRAY_IN_LIFT_Z			, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_DOWN		,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_MID			] = TEACH_PARAM(_T("NG_TRAY_IN_LIFT_Z_TO_MID"			), AXIS_SVO_NG_TRAY_IN_LIFT_Z			, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_MID			,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_UP			] = TEACH_PARAM(_T("NG_TRAY_OUT_LIFT_Z_TO_UP"			), AXIS_SVO_NG_TRAY_OUT_LIFT_Z			, TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_UP			,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_DOWN			] = TEACH_PARAM(_T("NG_TRAY_OUT_LIFT_Z_TO_DOWN"			), AXIS_SVO_NG_TRAY_OUT_LIFT_Z			, TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_DOWN		,  0., 0., ACC_250ms, ACC_250ms);

	m_Param[TEACH_PARAM::NG_TRAY_TR_X_TO_IN					] = TEACH_PARAM(_T("NG_TRAY_TR_X_TO_IN"					), AXIS_SVO_NG_TRAY_TR_X				, TEACH_PARAM::NG_TRAY_TR_X_TO_IN				,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::NG_TRAY_TR_X_TO_OUT				] = TEACH_PARAM(_T("NG_TRAY_TR_X_TO_OUT"				), AXIS_SVO_NG_TRAY_TR_X				, TEACH_PARAM::NG_TRAY_TR_X_TO_OUT				,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::NG_TRAY_TR_Z_TO_UP					] = TEACH_PARAM(_T("NG_TRAY_TR_Z_TO_UP"					), AXIS_SVO_NG_TRAY_TR_Z				, TEACH_PARAM::NG_TRAY_TR_Z_TO_UP				,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::NG_TRAY_TR_Z_TO_IN					] = TEACH_PARAM(_T("NG_TRAY_TR_Z_TO_IN"					), AXIS_SVO_NG_TRAY_TR_Z				, TEACH_PARAM::NG_TRAY_TR_Z_TO_IN				,  0., 0., ACC_250ms, ACC_250ms);
	m_Param[TEACH_PARAM::NG_TRAY_TR_Z_TO_OUT				] = TEACH_PARAM(_T("NG_TRAY_TR_Z_TO_OUT"				), AXIS_SVO_NG_TRAY_TR_Z				, TEACH_PARAM::NG_TRAY_TR_Z_TO_OUT				,  0., 0., ACC_250ms, ACC_250ms);
}

TEACH_PARAM CTeachBank::GetTeachParam( TEACH_PARAM::ID id )
{
	TEACH_PARAM param = m_Param[id];

	//////////////////////////////////////////////////////////////////////////
	// Recipe에 따라 자동으로 가변하는 수치 처리부분
	//////////////////////////////////////////////////////////////////////////

	return param;
}

void CTeachBank::SetTeachParam( TEACH_PARAM param )
{
	//////////////////////////////////////////////////////////////////////////
	// Recipe로 가변했던 값 원복
	int nTeachID = param.idTeach;
	switch (nTeachID)
	{
		case TEACH_PARAM::LD_UPPERSHUTTLE_LOAD:
			break;
		default:															
			break;
	}
	//////////////////////////////////////////////////////////////////////////
	m_Param[nTeachID] = param;
}

void CTeachBank::ReadTeachFile(CString strFilePath)
{
	CString strPath;
//	strPath.Format(_T("%s\\Teach.ini"), theConfigBank.m_System.m_strConfigPath);
//	Teach값 Recipe로 변경
	
	//20170201 byskcho
	//strPath.Format(_T("%s\\%s\\Teach.ini"), theConfigBank.m_System.m_strRecipePath,theRecipeBank.m_strRecipeName);
	strPath.Format(_T("%s\\%s\\Teach.ini"), theConfigBank.m_System.m_strRecipePath,strFilePath);
	for(int i = 0; i < TEACH_PARAM::PARAM_MAX; i++)
	{
		m_Param[i].ReadFile(strPath);
	}
}
//20170201 byskcho
//레시피 생성시 티칭값도 생성
//void CTeachBank::SaveTeachFile()
void CTeachBank::SaveTeachFile(CString strFilePath)
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;
//	strPath.Format(_T("%s\\Teach.ini"), theConfigBank.m_System.m_strConfigPath);
//	Teach값 Recipe로 변경
	
	//20170201 byskcho
	//strPath.Format(_T("%s\\%s\\Teach.ini"), theConfigBank.m_System.m_strRecipePath,theRecipeBank.m_strRecipeName);
	strPath.Format(_T("%s\\%s\\Teach.ini"), theConfigBank.m_System.m_strRecipePath,strFilePath);	
	strBackupPath.Format(_T("%s\\%d_%d\\Teach_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.Format(_T("%H%M%S")));

	CString strFolderPath = CFileSupport::GetPathName(strPath);
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

	for(int i = 0; i < TEACH_PARAM::PARAM_MAX; i++)
	{
		m_Param[i].SaveFile(strPath);
	}
}
