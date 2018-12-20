#pragma once
#include "Device/DeviceAccura.h"


enum FDC_DATATYPE
{
	FDC_DATA_INT = 0,
	FDC_DATA_INT_ADD1,
	FDC_DATA_DOUBLE,
	FDC_DATA_STRING
};

class CFDCItem
{
private:
	int				*m_ptrInt;
	double			*m_ptrDouble;
	CString			*m_ptrString;

	CString m_strSection;
	CString m_strKey;
	FDC_DATATYPE m_Type;

public:
	CFDCItem(void);
	CFDCItem(CString strSection, CString strKey, FDC_DATATYPE type, void *ptr1);
	~CFDCItem(void);

	void Init();
	void InitValue();

	CString GetSection();
	CString GetKey();

	void SetValuePtr(FDC_DATATYPE type, void *ptr);
	CString ValueToString();
	void WriteFile(CString strPath);
};

class CFDCBank
{
public:
	CFDCBank(void);
	~CFDCBank(void);

	CList <CFDCItem, CFDCItem> m_listFDCItem;

	double m_dTorque[AXIS_ID_MAX];
	double m_dMainAir_1;		// 0x1000D, LD-CH1
	double m_dMainAir_2;		// 0x1000E, LD-CH2
	double m_dMainVac_1;		// 0x1000F, LD-CH3
	double m_dMainVac_2;		// 0x100012, LD-CH4

	 //2017-04-20, skcho, CIM 요청사항
	double m_dMainAir_1_OUT;		// 0x1000D, LD-CH1
	double m_dMainAir_2_OUT;		// 0x1000E, LD-CH2
	double m_dMainVac_1_OUT;		// 0x1000F, LD-CH3
	double m_dMainVac_2_OUT;		// 0x100012, LD-CH4

	//20170129 byskcho
	//cim oss 파일 변경으로 추가
	double m_dInspVac[VACCUM_MAX];

	double m_dInspVac_OUT[VACCUM_MAX];  //2017-04-20, skcho, CIM 요청사항

	double m_dAdOffset[VACCUM_MAX];

	CString m_strJig_CellID[JIG_ID_MAX][JIG_CH_MAX];   
	CString m_strULDStage_CellID[JIG_CH_MAX];

	unsigned int m_nULDStage_StepID[JIG_CH_MAX];
	unsigned int m_nJig_StepID[JIG_ID_MAX][JIG_CH_MAX];  	//

	CString m_strULDTR_CellID[JIG_CH_MAX];
	unsigned int m_nULDTR_StepID[JIG_CH_MAX];

	// 0 : Normal, 1 : Alarm
	BOOL m_bLDBox1TempAlarm;	// X_LOADER_BOX_A_TEMP_ALARM
	BOOL m_bLDBox2TempAlarm;	// X_LOADER_BOX_B_TEMP_ALARM
	BOOL m_bInspBox1TempAlarm;	// X_INSP_BOX_A_TEMP_ALARM
	BOOL m_bInspBox2TempAlarm;	// X_INSP_BOX_B_TEMP_ALARM
	BOOL m_bUDBoxTempAlarm;		// X_UNLOAD_BOX_A_TEMP_ALARM

	// 0 : Normal, 1 : Limit
	BOOL m_bInspectionZLimitP[CIM_JIG_CH_MAX];

	TAccuraData		m_rDataBlock[ACCURA_CLIENT_MAX];
	
	void InitItem();

	// 프로그램 시작 후 첫 기록 전에 이전꺼 지운다
	// 혹시나 항목 변경되면 그때마다 지우기 귀찮으므로..
	void DeleteFDC_SV_par();	

	void SaveFDC_SV_par();


	//20161228 byskcho
	double m_dPc_Rack_Temp ;//PC_RACK_TEMP
	double m_dDriverBox_Temp;
	double m_dLoad_Temp;
	double m_dUps_Temp;
	double m_dUnload_Temp;
	double m_dGps_Temp;
    UINT   m_nEFU_Alarm ;  //EFU_ALARM

	BOOL m_bFDCCheckFlag;
};