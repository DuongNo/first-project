#pragma once

#include "AxisParamBank.h"

struct TEACH_PARAM
{
	enum ID 
	{
		//////////////////////////////////////////////////////////////////////////
		// LOADER TEACH POINT												    //
		//////////////////////////////////////////////////////////////////////////
		// CONVEYOR TEACH
		// ROBOT TEACH
		LD_ROBOT_CONV,

		// SHUTTLE TEACH
		LD_UPPERSHUTTLE_LOAD,
		LD_UPPERSHUTTLE_UNLOAD,
		LD_LOWERSHUTTLE_LOAD,
		LD_LOWERSHUTTLE_UNLOAD,
		
		// PRE-ALIGN LOADER TR
		PA_TR_Y_GET,
		PA_TR_Y_PUT,
		PA_TR_Y_WAIT,

		PA_TR_Z_UPPER_GET,
		PA_TR_Z_LOWER_GET,
		PA_TR_Z_PUT,
		PA_TR_Z_UP,

		////////////////////////////////////////////////////////////////////////
		// PREALIGN TEACH POINT													//
		////////////////////////////////////////////////////////////////////////
		PA_CH1_X,
		PA_CH1_Y,
		PA_CH1_T,
		PA_CH2_X,
		PA_CH2_Y,
		PA_CH2_T,


		PA_STAGE_LOAD_POS,
		PA_STAGE_ALIGN_POS,
		PA_STAGE_UNLOAD_POS,

		//////////////////////////////////////////////////////////////////////////
		// LOADERTR TEACH POINT												    //
		//////////////////////////////////////////////////////////////////////////
		LD_TR_X_GET,
		LD_TR_X_PUT,
		LD_TR_X_CH1_LOWER_MCR,
		LD_TR_X_CH2_LOWER_MCR,
		LD_TR_X_CH1_UPPER_MCR,
		LD_TR_X_CH2_UPPER_MCR,

		LD_TR_Z_GET,
		LD_TR_Z_PUT,
		LD_TR_Z_UP,
		LD_TR_Z_LOWER_MCR,
		LD_TR_Z_UPPER_MCR,

		////////////////////////////////////////////////////////////////////////
		// TURNTABLE TEACH POINT												//
		////////////////////////////////////////////////////////////////////////
		TT_INSP_CAM_X,
		TT_INSP_CH1_CAM_ZOOM_Z,
		TT_INSP_CH1_CAM_FOCUS_F,
		TT_INSP_CH1_CAM_TURN_T,
		TT_INSP_CH2_CAM_ZOOM_Z,
		TT_INSP_CH2_CAM_FOCUS_F,
		TT_INSP_CH2_CAM_TURN_T,

		TT_VALIGN_CAM_Y_READY,
		TT_VALIGN_CAM_X_READY,

		TT_VALIGN_CAM_CH1_Y1,
		TT_VALIGN_CAM_CH1_Y2,
		TT_VALIGN_CAM_CH1_X1,
		TT_VALIGN_CAM_CH1_X2,

		TT_VALIGN_CAM_CH2_Y1,
		TT_VALIGN_CAM_CH2_Y2,
		TT_VALIGN_CAM_CH2_X1,
		TT_VALIGN_CAM_CH2_X2,

		WORK_TABLE_T_TO_WORK,

		//////////////////////////////////////////////////////////////////////////
		// UNLOADER TEACH POINT												    //
		//////////////////////////////////////////////////////////////////////////
		ULD_TR_X_GET,
		ULD_TR_X_PUT,

		ULD_TR_Z_GET,
		ULD_TR_Z_PUT,
		ULD_TR_Z_UP,

		//////////////////////////////////////////////////////////////////////////
		// GOOD TRAY TEACH POINT												//
		//////////////////////////////////////////////////////////////////////////
		OK_TRAY_IN_LIFT_Z_TO_UP				,
		OK_TRAY_IN_LIFT_Z_TO_DOWN		,
		OK_TRAY_IN_LIFT_Z_TO_ALIGN		,     //2017-12-21,SKCHO
		OK_TRAY_IN_LIFT_Z_TO_TOP		,     //2018wjp

		OK_TRAY_OUT_LIFT_Z_TO_UP			,
		OK_TRAY_OUT_LIFT_Z_TO_DOWN		,
		OK_TRAY_OUT_LIFT_Z_TO_ALIGN,     //2017-12-21,SKCHO

		OK_TRAY_TR_Y_TO_IN						,
		OK_TRAY_TR_Y_TO_OUT					,
		OK_TRAY_TR_Z_TO_UP						,
		OK_TRAY_TR_Z_TO_IN						,
		OK_TRAY_TR_Z_TO_OUT					,

		//////////////////////////////////////////////////////////////////////////
		// NG TRAY TEACH POINT												//
		//////////////////////////////////////////////////////////////////////////

		NG_TRAY_IN_LIFT_Z_TO_UP				,
		NG_TRAY_IN_LIFT_Z_TO_DOWN		,
		NG_TRAY_IN_LIFT_Z_TO_MID		,
		NG_TRAY_OUT_LIFT_Z_TO_UP			,
		NG_TRAY_OUT_LIFT_Z_TO_DOWN		,

		NG_TRAY_TR_X_TO_IN						,
		NG_TRAY_TR_X_TO_OUT					,
		NG_TRAY_TR_Z_TO_UP						,
		NG_TRAY_TR_Z_TO_IN						,
		NG_TRAY_TR_Z_TO_OUT					,

		PARAM_MAX
	};

	CString strName;

	AXIS_ID idAxis;
	TEACH_PARAM::ID idTeach;

	double dPos;
	double dSpeed;
	double dAcc;
	double dDec;

	TEACH_PARAM(void);
	TEACH_PARAM(CString name, AXIS_ID axis, TEACH_PARAM::ID teach, double pos, double speed, double acc, double dec);
	~TEACH_PARAM(void);

	void Init();
	void ReadFile(CString strFilePath);
	void SaveFile(CString strFilePath);
};

class CTeachBank
{
public:
	CTeachBank(void);
	~CTeachBank(void);

private:
	TEACH_PARAM m_Param[TEACH_PARAM::PARAM_MAX];

public:
	void InitializeTeachParam();

	TEACH_PARAM GetTeachParam(TEACH_PARAM::ID id);
	void SetTeachParam(TEACH_PARAM param);

	void ReadTeachFile(CString strFilePath);
	void SaveTeachFile(CString strFilePath);
	
	void GetTeachParamList(int iAxis, CArray<TEACH_PARAM,TEACH_PARAM> &arr);
};

