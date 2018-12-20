// FormConfig2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormConfig2.h"

#include "UI\GausGUI\GxUICtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormConfig2

IMPLEMENT_DYNCREATE(CFormConfig2, CFormView)

CFormConfig2::CFormConfig2()
	: CFormView(CFormConfig2::IDD)
{

}

CFormConfig2::~CFormConfig2()
{
}

void CFormConfig2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormConfig2, CFormView)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFormConfig2 진단입니다.

#ifdef _DEBUG
void CFormConfig2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormConfig2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormConfig2 메시지 처리기입니다.


void CFormConfig2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	SetTimer(0,3000,NULL);
}

void CFormConfig2::SetUpdateData()
{
	    CString szMsg;
		CDeviceSerialGMS* pDeviceGMS = theSerialInterFace.GetDeviceGMSHandler();
		
		CGxUICtrl::SetStaticString(this, IDC_GMS1, pDeviceGMS->m_LastGMSValue[GMS_A_JIG]);	
		CGxUICtrl::SetStaticString(this, IDC_GMS2, pDeviceGMS->m_LastGMSValue[GMS_B_JIG]);	
		CGxUICtrl::SetStaticString(this, IDC_GMS3,  pDeviceGMS->m_LastGMSValue[GMS_C_JIG]);		
		CGxUICtrl::SetStaticString(this, IDC_GMS4, pDeviceGMS->m_LastGMSValue[GMS_D_JIG]);			
		CGxUICtrl::SetStaticString(this, IDC_GMS5, pDeviceGMS->m_LastGMSValue[GMS_SUB_CH5]);
	
		szMsg.Format(_T("%.3f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbWattTotal);
		CGxUICtrl::SetStaticString(this, IDC_UPS_POWER,szMsg.GetString());
	
		szMsg.Format(_T("%.1f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_R);
		CGxUICtrl::SetStaticString(this, IDC_UPS_VOLTAGE, szMsg);

		szMsg.Format(_T("%.3f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_R);
		CGxUICtrl::SetStaticString(this, IDC_UPS_CURRENT,szMsg);
	
		szMsg.Format(_T("%.1f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_R);
		CGxUICtrl::SetStaticString(this, IDC_GPS_VOLTAGE, szMsg);
	
		szMsg.Format(_T("%.3f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_R);
		CGxUICtrl::SetStaticString(this, IDC_GPS_CURRENT,szMsg);

		szMsg.Format(_T("%.3f"),theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbWattTotal);
		CGxUICtrl::SetStaticString(this, IDC_GPS_POWER,szMsg);

		// OUT 변수 추가 후에 적용 안되어 변경. 17/04/25 HSK
		szMsg.Format(_T("%.1f"),theFDCBank.m_dMainAir_1_OUT);
		CGxUICtrl::SetStaticString(this, IDC_MAIN_AIR_1, szMsg);
	
		szMsg.Format(_T("%.1f"),theFDCBank.m_dMainAir_2_OUT);
		CGxUICtrl::SetStaticString(this, IDC_MAIN_AIR_2,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dMainVac_1_OUT);
		CGxUICtrl::SetStaticString(this, IDC_MAIN_VACUUM_1,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dMainVac_2_OUT);
		CGxUICtrl::SetStaticString(this, IDC_MAIN_VACUUM_2,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[A_ZONE_CH1]);
		CGxUICtrl::SetStaticString(this, IDC_A_VACUUM_1,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[A_ZONE_CH2]);
		CGxUICtrl::SetStaticString(this, IDC_A_VACUUM_2,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[B_ZONE_CH1]);
		CGxUICtrl::SetStaticString(this, IDC_B_VACUUM_1,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[B_ZONE_CH2]);
		CGxUICtrl::SetStaticString(this, IDC_B_VACUUM_2,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[C_ZONE_CH1]);
		CGxUICtrl::SetStaticString(this, IDC_C_VACUUM_1,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[C_ZONE_CH2]);
		CGxUICtrl::SetStaticString(this, IDC_C_VACUUM_2,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[D_ZONE_CH1]);
		CGxUICtrl::SetStaticString(this, IDC_D_VACUUM_1,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[D_ZONE_CH2]);
		CGxUICtrl::SetStaticString(this, IDC_D_VACUUM_2,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[ULD_TR_CH1]);
		CGxUICtrl::SetStaticString(this, IDC_UD_TR_VAC_1,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[ULD_TR_CH2]);
		CGxUICtrl::SetStaticString(this, IDC_UD_TR_VAC_2,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[ULD_STAGE_CH1]);
		CGxUICtrl::SetStaticString(this, IDC_UD_STAGE_VAC_1,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dInspVac_OUT[ULD_STAGE_CH2]);
		CGxUICtrl::SetStaticString(this, IDC_UD_STAGE_VAC_2,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dPc_Rack_Temp);
		CGxUICtrl::SetStaticString(this, IDC_TEMP_PC_RACK,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dUps_Temp);
		CGxUICtrl::SetStaticString(this, IDC_UPS_TEMP,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dGps_Temp);
		CGxUICtrl::SetStaticString(this, IDC_GPS_TEMP,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dLoad_Temp);
		CGxUICtrl::SetStaticString(this, IDC_LOAD_TEMP,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dUnload_Temp);
		CGxUICtrl::SetStaticString(this, IDC_UNLOAD_TEMP,szMsg);

		szMsg.Format(_T("%.1f"),theFDCBank.m_dDriverBox_Temp);
		CGxUICtrl::SetStaticString(this, IDC_DRIVERBOX_TEMP,szMsg);

}

void CFormConfig2::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 0)
	{
		SetUpdateData();
	}

	CFormView::OnTimer(nIDEvent);
}
BEGIN_EVENTSINK_MAP(CFormConfig2, CFormView)
	ON_EVENT(CFormConfig2, IDC_OFFSET_BUTTON, DISPID_CLICK, CFormConfig2::ClickOffsetButton, VTS_NONE)
END_EVENTSINK_MAP()


void CFormConfig2::ClickOffsetButton()
{
	theUnitFunc.LoadADBoardOffset();
}
