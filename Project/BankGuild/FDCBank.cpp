#include "StdAfx.h"
#include "FDCBank.h"
#include "Etc\FileSupport.h"
#include "Etc\Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CFDCItem::CFDCItem( void )
{
	Init();
}

CFDCItem::CFDCItem( CString strSection, CString strKey, FDC_DATATYPE type, void *ptr )
{
	m_strSection = strSection;
	m_strKey = strKey;
	SetValuePtr(type, ptr);
}

CFDCItem::~CFDCItem( void )
{

}

void CFDCItem::Init()
{
	m_strSection.Empty();
	InitValue();
}

void CFDCItem::InitValue()
{
	m_ptrInt = NULL;
	m_ptrDouble = NULL;
	m_ptrString = NULL;
	m_Type = FDC_DATA_STRING;
}
CString CFDCItem::GetSection()
{
	return m_strSection;
}

CString CFDCItem::GetKey()
{
	return m_strKey;
}

void CFDCItem::SetValuePtr( FDC_DATATYPE type, void *ptr )
{
	InitValue();
	m_Type = type;
	switch(m_Type)
	{
	case FDC_DATA_INT:				
	case FDC_DATA_INT_ADD1:			m_ptrInt =			(int *)ptr;				break;
	case FDC_DATA_DOUBLE:			m_ptrDouble =		(double *)ptr;			break;
	case FDC_DATA_STRING:			m_ptrString =		(CString *)ptr;			break;
		break;
	}
}

CString CFDCItem::ValueToString()
{
	CString strRetrunText;
	switch(m_Type)
	{
	case FDC_DATA_INT:	
		if(m_ptrInt)	strRetrunText.Format(_T("%d"), *m_ptrInt);			
		break;
	case FDC_DATA_INT_ADD1:	
		if(m_ptrInt)	strRetrunText.Format(_T("%d"), *m_ptrInt + 1);			
		break;
	case FDC_DATA_DOUBLE:	
		if(m_ptrDouble)	strRetrunText.Format(_T("%.3f"), *m_ptrDouble);		
		break;
	case FDC_DATA_STRING:	
		if(m_ptrString)	strRetrunText.Format(_T("%s"), m_ptrString->GetString());		
		break;
	}

	return strRetrunText;
}

void CFDCItem::WriteFile(CString strPath)
{
	CIni ini(strPath);

	ini.WriteString(GetSection(), GetKey(), ValueToString());
}

CFDCBank::CFDCBank(void)
{
	memset(m_dTorque, 0, sizeof(double)*AXIS_ID_MAX);
	m_dMainVac_1 = m_dMainVac_2 = m_dMainAir_1 = m_dMainAir_2 = 48.4;
	for(int i = 0; i < VACCUM_MAX; i++)
		m_dInspVac[i] = 48.4;

	m_bLDBox1TempAlarm = FALSE;
	m_bLDBox2TempAlarm = FALSE;
	m_bInspBox1TempAlarm = FALSE;
	m_bInspBox2TempAlarm = FALSE;
	m_bUDBoxTempAlarm = FALSE;
	memset(m_bInspectionZLimitP, 0, sizeof(BOOL)*CIM_JIG_CH_MAX);

	m_nEFU_Alarm=1;

	
}

CFDCBank::~CFDCBank(void)
{

}
/*
void CFDCBank::InitItem()
{
	m_listFDCItem.RemoveAll();

	CDeviceSerialGMS* pDeviceGMS = theSerialInterFace.GetDeviceGMSHandler();

	CFDCItem item;

	//////////////////////////////////////////////////////////////////////////
	// COMMON [2017/01/28]
    item = CFDCItem(_T("LOADER"),		_T("LD_UPPERSHUTTLE_Y"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_LD_UPPERSHUTTLE_Y],	&m_dTorque[AXIS_SVO_LD_UPPERSHUTTLE_Y]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("LD_LOWERSHUTTLE_Y"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_LD_LOWERSHUTTLE_Y], &m_dTorque[AXIS_SVO_LD_LOWERSHUTTLE_Y]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("LD_TR_X"),						FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_LD_TR_X],			&m_dTorque[AXIS_SVO_LD_TR_X]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("LD_TR_Z"),						FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_LD_TR_Z],			&m_dTorque[AXIS_SVO_LD_TR_Z]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("PREALIGN_TR_Y"),				FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_PREALIGN_TR_Y],		&m_dTorque[AXIS_SVO_PREALIGN_TR_Y]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("PREALIGN_TR_Z"),				FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_PREALIGN_TR_Z],     &m_dTorque[AXIS_SVO_PREALIGN_TR_Z]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("PA_STAGE_X"),					FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_PA_STAGE_X],		&m_dTorque[AXIS_SVO_PA_STAGE_X]);					m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("INS_AZONE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_A][JIG_CH_1],	&m_strJig_CellID[JIG_ID_A][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_A][JIG_CH_1],		&m_nJig_StepID[JIG_ID_A][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH1"),		_T("A_ZONE_CH1_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[A_ZONE_CH1],				&m_dInspVac[A_ZONE_CH1]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_AZONE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_A][JIG_CH_2],	&m_strJig_CellID[JIG_ID_A][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_A][JIG_CH_2],		&m_nJig_StepID[JIG_ID_A][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH2"),		_T("A_ZONE_CH2_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[A_ZONE_CH2],				&m_dInspVac[A_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_BZONE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_B][JIG_CH_1],	&m_strJig_CellID[JIG_ID_B][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_B][JIG_CH_1],		&m_nJig_StepID[JIG_ID_B][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH1"),		_T("B_ZONE_CH1_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[B_ZONE_CH1],				&m_dInspVac[B_ZONE_CH1]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_BZONE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_B][JIG_CH_2],	&m_strJig_CellID[JIG_ID_B][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_B][JIG_CH_2],		&m_nJig_StepID[JIG_ID_B][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH2"),		_T("B_ZONE_CH2_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[B_ZONE_CH2],				&m_dInspVac[B_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_CZONE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_C][JIG_CH_1],	&m_strJig_CellID[JIG_ID_C][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_C][JIG_CH_1],		&m_nJig_StepID[JIG_ID_C][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH1"),		_T("C_ZONE_CH1_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[C_ZONE_CH1],				&m_dInspVac[C_ZONE_CH1]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_CZONE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_C][JIG_CH_2],	&m_strJig_CellID[JIG_ID_C][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_C][JIG_CH_2],		&m_nJig_StepID[JIG_ID_C][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH2"),		_T("C_ZONE_CH2_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[C_ZONE_CH2],				&m_dInspVac[C_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_DZONE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_D][JIG_CH_1],	&m_strJig_CellID[JIG_ID_D][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_D][JIG_CH_1],		&m_nJig_StepID[JIG_ID_D][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH1"),		_T("D_ZONE_CH1_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[D_ZONE_CH1],				&m_dInspVac[D_ZONE_CH1]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_DZONE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_D][JIG_CH_2],	&m_strJig_CellID[JIG_ID_D][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_D][JIG_CH_2],		&m_nJig_StepID[JIG_ID_D][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH2"),		_T("D_ZONE_CH2_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[D_ZONE_CH2],				&m_dInspVac[D_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("ULD_TR_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,			&m_strULDTR_CellID[JIG_CH_1],			&m_strULDTR_CellID[JIG_CH_1]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,				&m_nULDTR_StepID[JIG_CH_1],				&m_nULDTR_StepID[JIG_CH_1]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH1"),		_T("ULD_TR_CH1_VAC"),	FDC_DATA_DOUBLE,			&m_dInspVac[ULD_TR_CH1],				&m_dInspVac[ULD_TR_CH1]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("ULD_TR_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,			&m_strULDTR_CellID[JIG_CH_2],			&m_strULDTR_CellID[JIG_CH_2]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,				&m_nULDTR_StepID[JIG_CH_2],				&m_nULDTR_StepID[JIG_CH_2]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH2"),		_T("ULD_TR_CH2_VAC"),	FDC_DATA_DOUBLE,			&m_dInspVac[ULD_TR_CH2],				&m_dInspVac[ULD_TR_CH2]);							m_listFDCItem.AddTail(item);
	

	item = CFDCItem(_T("ULD_STAGE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strULDStage_CellID[JIG_CH_1],		&m_strULDStage_CellID[JIG_CH_1]);					m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nULDStage_StepID[JIG_CH_1],			&m_nULDStage_StepID[JIG_CH_1]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH1"),		_T("ULD_STAGE_CH1_VAC"),	FDC_DATA_DOUBLE,	&m_dInspVac[ULD_STAGE_CH1],				&m_dInspVac[ULD_STAGE_CH1]);						m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("ULD_STAGE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strULDStage_CellID[JIG_CH_2],		&m_strULDStage_CellID[JIG_CH_2]);					m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nULDStage_StepID[JIG_CH_2],			&m_nULDStage_StepID[JIG_CH_2]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH2"),		_T("ULD_STAGE_CH2_VAC"),	FDC_DATA_DOUBLE,	&m_dInspVac[ULD_STAGE_CH2],				&m_dInspVac[ULD_STAGE_CH2]);						m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("GOOD TRAY"),	_T("GOOD_TRAY_IN_LIFT_Z"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z],	&m_dTorque[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("GOOD TRAY"),	_T("GOOD_TRAY_OUT_LIFT_Z"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z],	&m_dTorque[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("GOOD TRAY"),	_T("GOOD_TRAY_TR_Y"),				FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_GOOD_TRAY_TR_Y],		&m_dTorque[AXIS_SVO_GOOD_TRAY_TR_Y]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("GOOD TRAY"),	_T("GOOD_TRAY_TR_Z"),				FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_GOOD_TRAY_TR_Z],		&m_dTorque[AXIS_SVO_GOOD_TRAY_TR_Z]);				m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("NG TRAY"),		_T("NG_TRAY_IN_LIFT_Z"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_NG_TRAY_IN_LIFT_Z],		&m_dTorque[AXIS_SVO_NG_TRAY_IN_LIFT_Z]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("NG TRAY"),		_T("NG_TRAY_OUT_LIFT_Z"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_NG_TRAY_OUT_LIFT_Z],	&m_dTorque[AXIS_SVO_NG_TRAY_OUT_LIFT_Z]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("NG TRAY"),		_T("NG_TRAY_TR_X"),					FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_NG_TRAY_TR_X],			&m_dTorque[AXIS_SVO_NG_TRAY_TR_X]);					m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("NG TRAY"),		_T("NG_TRAY_TR_Z"),					FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_NG_TRAY_TR_Z],			&m_dTorque[AXIS_SVO_NG_TRAY_TR_Z]);					m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("UTIL"),			_T("PC_RACK_TEMP"),			FDC_DATA_DOUBLE,			&m_dPc_Rack_Temp,	&m_dPc_Rack_Temp);									m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("EFU_ALARM"),			FDC_DATA_INT,				&m_nEFU_Alarm,		&m_nEFU_Alarm);										m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("MAIN_AIR_1"),			FDC_DATA_DOUBLE,			&m_dMainAir_1,		&m_dMainAir_1);										m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("MAIN_AIR_2"),			FDC_DATA_DOUBLE,			&m_dMainAir_2,		&m_dMainAir_2);										m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("MAIN_VAC_1"),			FDC_DATA_DOUBLE,			&m_dMainVac_1,		&m_dMainVac_1);										m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("MAIN_VAC_2"),			FDC_DATA_DOUBLE,			&m_dMainVac_2,		&m_dMainVac_2);										m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("UTIL"),			_T("GMS_1"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_A_JIG],		&pDeviceGMS->m_LastGMSValue[GMS_A_JIG]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("GMS_2"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_B_JIG],		&pDeviceGMS->m_LastGMSValue[GMS_B_JIG]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("GMS_3"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_C_JIG],     &pDeviceGMS->m_LastGMSValue[GMS_C_JIG]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("GMS_4"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_D_JIG],		&pDeviceGMS->m_LastGMSValue[GMS_D_JIG]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("GMS_5"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_SUB_CH5],	&pDeviceGMS->m_LastGMSValue[GMS_SUB_CH5]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("PGMS_1"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastBZonePGMS,				&pDeviceGMS->m_LastBZonePGMS);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("PGMS_2"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastDZonePGMS,				&pDeviceGMS->m_LastDZonePGMS);						m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("SEM"),			_T("THREE_PHASE_POWER1"),		FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbWattNet,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbWattNet);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("INSTANTANEOUS_POWER1"),		FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbWattTotal,	&m_rDataBlock[ACCURA_CLIENT_GPS].dbWattTotal);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("R_PHASE_VOLTAGE1"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_R,	&m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_R);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("T_PHASE_VOLTAGE1"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_T,	&m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_T);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("R_PHASE_CURRENT1"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_R,	&m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_R);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("T_PHASE_CURRENT1"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_T,	&m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_T);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("THREE_PHASE_POWER2"),		FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_UPS].dbWattNet,		&m_rDataBlock[ACCURA_CLIENT_UPS].dbWattNet);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("INSTANTANEOUS_POWER2"),		FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_UPS].dbWattTotal,	&m_rDataBlock[ACCURA_CLIENT_UPS].dbWattTotal);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("R_PHASE_VOLTAGE2"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_R,	&m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_R);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("R_PHASE_CURRENT2"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_R,	&m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_R);		m_listFDCItem.AddTail(item);
}
*/
void CFDCBank::InitItem()
{
	m_listFDCItem.RemoveAll();

	CDeviceSerialGMS* pDeviceGMS = theSerialInterFace.GetDeviceGMSHandler();

	CFDCItem item;

	//////////////////////////////////////////////////////////////////////////
	// COMMON [2017/01/28]
    item = CFDCItem(_T("LOADER"),		_T("LD_UPPERSHUTTLE_Y"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_LD_UPPERSHUTTLE_Y]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("LD_LOWERSHUTTLE_Y"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_LD_LOWERSHUTTLE_Y]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("LD_TR_X"),						FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_LD_TR_X]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("LD_TR_Z"),						FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_LD_TR_Z]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("PREALIGN_TR_Y"),				FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_PREALIGN_TR_Y]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("PREALIGN_TR_Z"),				FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_PREALIGN_TR_Z]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("LOADER"),		_T("PA_STAGE_X"),					FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_PA_STAGE_X]);					m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("INS_AZONE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_A][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_A][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH1"),		_T("A_ZONE_CH1_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[A_ZONE_CH1]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH1"),		_T("A_ZONE_CH1_VAC_OUT"),	FDC_DATA_DOUBLE,	&m_dInspVac_OUT[A_ZONE_CH1]);					    m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_AZONE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_A][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_A][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH2"),		_T("A_ZONE_CH2_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[A_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_AZONE_CH2"),		_T("A_ZONE_CH2_VAC_OUT"),	FDC_DATA_DOUBLE,	&m_dInspVac_OUT[A_ZONE_CH2]);					    m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("INS_BZONE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_B][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_B][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH1"),		_T("B_ZONE_CH1_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[B_ZONE_CH1]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH1"),		_T("B_ZONE_CH1_VAC_OUT"),	FDC_DATA_DOUBLE,		&m_dInspVac_OUT[B_ZONE_CH1]);							m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("INS_BZONE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_B][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_B][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH2"),		_T("B_ZONE_CH2_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[B_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_BZONE_CH2"),		_T("B_ZONE_CH2_VAC_OUT"),	FDC_DATA_DOUBLE,		&m_dInspVac_OUT[B_ZONE_CH2]);							m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("INS_CZONE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_C][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_C][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH1"),		_T("C_ZONE_CH1_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[C_ZONE_CH1]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH1"),		_T("C_ZONE_CH1_VAC_OUT"),	FDC_DATA_DOUBLE,		&m_dInspVac_OUT[C_ZONE_CH1]);							m_listFDCItem.AddTail(item);

	
	item = CFDCItem(_T("INS_CZONE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_C][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_C][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH2"),		_T("C_ZONE_CH2_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[C_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_CZONE_CH2"),		_T("C_ZONE_CH2_VAC_OUT"),	FDC_DATA_DOUBLE,		&m_dInspVac_OUT[C_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_DZONE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_D][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_D][JIG_CH_1]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH1"),		_T("D_ZONE_CH1_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[D_ZONE_CH1]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH1"),		_T("D_ZONE_CH1_VAC_OUT"),	FDC_DATA_DOUBLE,		&m_dInspVac_OUT[D_ZONE_CH1]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("INS_DZONE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strJig_CellID[JIG_ID_D][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nJig_StepID[JIG_ID_D][JIG_CH_2]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH2"),		_T("D_ZONE_CH2_VAC"),	FDC_DATA_DOUBLE,		&m_dInspVac[D_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("INS_DZONE_CH2"),		_T("D_ZONE_CH2_VAC_OUT"),	FDC_DATA_DOUBLE,		&m_dInspVac_OUT[D_ZONE_CH2]);							m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("ULD_TR_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,			&m_strULDTR_CellID[JIG_CH_1]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,				&m_nULDTR_StepID[JIG_CH_1]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH1"),		_T("ULD_TR_CH1_VAC"),	FDC_DATA_DOUBLE,			&m_dInspVac[ULD_TR_CH1]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH1"),		_T("ULD_TR_CH1_VAC_OUT"),	FDC_DATA_DOUBLE,			&m_dInspVac_OUT[ULD_TR_CH1]);							m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("ULD_TR_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,			&m_strULDTR_CellID[JIG_CH_2]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,				&m_nULDTR_StepID[JIG_CH_2]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH2"),		_T("ULD_TR_CH2_VAC"),	FDC_DATA_DOUBLE,			&m_dInspVac[ULD_TR_CH2]);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_TR_CH2"),		_T("ULD_TR_CH2_VAC_OUT"),	FDC_DATA_DOUBLE,			&m_dInspVac_OUT[ULD_TR_CH2]);							m_listFDCItem.AddTail(item);
	

	item = CFDCItem(_T("ULD_STAGE_CH1"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strULDStage_CellID[JIG_CH_1]);					m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH1"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nULDStage_StepID[JIG_CH_1]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH1"),		_T("ULD_STAGE_CH1_VAC"),	FDC_DATA_DOUBLE,	&m_dInspVac[ULD_STAGE_CH1]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH1"),		_T("ULD_STAGE_CH1_VAC_OUT"),	FDC_DATA_DOUBLE,	&m_dInspVac_OUT[ULD_STAGE_CH1]);						m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("ULD_STAGE_CH2"),		_T("CELL_ID"),			FDC_DATA_STRING,		&m_strULDStage_CellID[JIG_CH_2]);					m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH2"),		_T("STEP_ID"),			FDC_DATA_INT,			&m_nULDStage_StepID[JIG_CH_2]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH2"),		_T("ULD_STAGE_CH2_VAC"),	FDC_DATA_DOUBLE,	&m_dInspVac[ULD_STAGE_CH2]);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("ULD_STAGE_CH2"),		_T("ULD_STAGE_CH2_VAC_OUT"),	FDC_DATA_DOUBLE,	&m_dInspVac_OUT[ULD_STAGE_CH2]);						m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("GOOD TRAY"),	_T("GOOD_TRAY_IN_LIFT_Z"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("GOOD TRAY"),	_T("GOOD_TRAY_OUT_LIFT_Z"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("GOOD TRAY"),	_T("GOOD_TRAY_TR_Y"),				FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_GOOD_TRAY_TR_Y]);				m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("GOOD TRAY"),	_T("GOOD_TRAY_TR_Z"),				FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_GOOD_TRAY_TR_Z]);				m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("NG TRAY"),		_T("NG_TRAY_IN_LIFT_Z"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_NG_TRAY_IN_LIFT_Z]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("NG TRAY"),		_T("NG_TRAY_OUT_LIFT_Z"),			FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_NG_TRAY_OUT_LIFT_Z]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("NG TRAY"),		_T("NG_TRAY_TR_X"),					FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_NG_TRAY_TR_X]);					m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("NG TRAY"),		_T("NG_TRAY_TR_Z"),					FDC_DATA_DOUBLE,	&m_dTorque[AXIS_SVO_NG_TRAY_TR_Z]);					m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("UTIL"),			_T("PC_RACK_TEMP"),			FDC_DATA_DOUBLE,			&m_dPc_Rack_Temp);									m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("EFU_ALARM"),			FDC_DATA_INT,				&m_nEFU_Alarm);										m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("MAIN_AIR1"),			FDC_DATA_DOUBLE,			&m_dMainAir_1);								m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("MAIN_AIR1_OUT"),			FDC_DATA_DOUBLE,			&m_dMainAir_1_OUT);										m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("UTIL"),			_T("MAIN_AIR2"),			FDC_DATA_DOUBLE,			&m_dMainAir_2);										m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("MAIN_AIR2_OUT"),			FDC_DATA_DOUBLE,			&m_dMainAir_2_OUT);										m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("UTIL"),			_T("MAIN_VAC1"),			FDC_DATA_DOUBLE,			&m_dMainVac_1);										m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("MAIN_VAC1_OUT"),			FDC_DATA_DOUBLE,			&m_dMainVac_1_OUT);										m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("UTIL"),			_T("MAIN_VAC2"),			FDC_DATA_DOUBLE,			&m_dMainVac_2);										m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("MAIN_VAC2_OUT"),			FDC_DATA_DOUBLE,			&m_dMainVac_2_OUT);										m_listFDCItem.AddTail(item);
	
	item = CFDCItem(_T("UTIL"),			_T("GMS_1"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_A_JIG]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("GMS_2"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_B_JIG]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("GMS_3"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_C_JIG]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("GMS_4"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_D_JIG]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("GMS_5"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastGMSValue[GMS_SUB_CH5]);			m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("PGMS_1"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastBZonePGMS);						m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("PGMS_2"),					FDC_DATA_STRING,		&pDeviceGMS->m_LastDZonePGMS);						m_listFDCItem.AddTail(item);
	/*item = CFDCItem(_T("UTIL"),			_T("LD_BOX1_TEMP_ALARM"),		FDC_DATA_INT,		&m_bLDBox1TempAlarm);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("LD_BOX2_TEMP_ALARM"),		FDC_DATA_INT,			&m_bLDBox2TempAlarm);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("INSP_BOX1_TEMP_ALARM"),		FDC_DATA_INT,			&m_bInspBox1TempAlarm);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("INSP_BOX2_TEMP_ALARM"),		FDC_DATA_INT,			&m_bInspBox2TempAlarm);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),			_T("UD_BOX_TEMP_ALARM"),		FDC_DATA_INT,			&m_bUDBoxTempAlarm);		m_listFDCItem.AddTail(item);*/

	/*item = CFDCItem(_T("FORCETOUCH"),	_T("CH1_Z_LIMIT"),				FDC_DATA_INT,			&m_bInspectionZLimitP[JIG_CH_1]);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("FORCETOUCH"),	_T("CH2_Z_LIMIT"),				FDC_DATA_INT,			&m_bInspectionZLimitP[JIG_CH_2]);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("FORCETOUCH"),	_T("CH3_Z_LIMIT"),				FDC_DATA_INT,			&m_bInspectionZLimitP[JIG_CH_3]);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("FORCETOUCH"),	_T("CH4_Z_LIMIT"),				FDC_DATA_INT,			&m_bInspectionZLimitP[JIG_CH_4]);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("FORCETOUCH"),	_T("CH5_Z_LIMIT"),				FDC_DATA_INT,			&m_bInspectionZLimitP[JIG_CH_5]);		m_listFDCItem.AddTail(item);*/

	item = CFDCItem(_T("SEM"),			_T("THREE_PHASE_POWER1"),		FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbWattNet);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("INSTANTANEOUS_POWER1"),		FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbWattTotal);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("R_PHASE_VOLTAGE1"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_R);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("T_PHASE_VOLTAGE1"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_T);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("R_PHASE_CURRENT1"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_R);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("T_PHASE_CURRENT1"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_T);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("THREE_PHASE_POWER2"),		FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_UPS].dbWattNet);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("INSTANTANEOUS_POWER2"),		FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_UPS].dbWattTotal);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("R_PHASE_VOLTAGE2"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_R);		m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),			_T("R_PHASE_CURRENT2"),			FDC_DATA_DOUBLE,		&m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_R);		m_listFDCItem.AddTail(item);
}
void CFDCBank::DeleteFDC_SV_par()
{
	//////////////////////////////////////////////////////////////////////////
	// 항목이 변경될 때 마다 지우기 귀찮으니 프로그램 재시작 할 때마다 지워버리자 
	CString strPath;
	strPath.Format(_T("%s\\FDC_SV.par"), theConfigBank.m_System.m_strFDCPath);

	if(CFileSupport::FileCheck(strPath))
		DeleteFile(strPath);
	//////////////////////////////////////////////////////////////////////////
}

void CFDCBank::SaveFDC_SV_par()
{
	if(CFileSupport::DirectoryCheck(theConfigBank.m_System.m_strFDCPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(theConfigBank.m_System.m_strFDCPath) == FALSE)
			return;
	}

	CString strPath;
	strPath.Format(_T("%s\\FDC_SV.par"), theConfigBank.m_System.m_strFDCPath);

	CFDCItem item;
	POSITION pos = m_listFDCItem.GetHeadPosition();
	while (pos)
	{
		item = m_listFDCItem.GetNext(pos);
		item.WriteFile(strPath);
	}
}
