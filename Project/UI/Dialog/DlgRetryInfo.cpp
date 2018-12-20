// DlgRetryInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgRetryInfo.h"
#include "afxdialogex.h"


//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

// CDlgRetryInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgRetryInfo, CDialog)

CDlgRetryInfo::CDlgRetryInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRetryInfo::IDD, pParent)
{

}

CDlgRetryInfo::~CDlgRetryInfo()
{
}

void CDlgRetryInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRetryInfo, CDialog)
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,	OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgRetryInfo 메시지 처리기입니다.
void CDlgRetryInfo::DisplayRetryInfo()
{

	CString strTotal;
	CString strSucess;

	strTotal.Format(_T("%d"),theProductBank.m_LDROBOT_RetryInfo.nTotal);
	strSucess.Format(_T("%d"),theProductBank.m_LDROBOT_RetryInfo.nSucess);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_LD_TOTAL, strTotal);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_LD_SUCESS, strSucess);


	strTotal.Format(_T("%d"),theProductBank.m_PATR_RetryInfo.nTotal);
	strSucess.Format(_T("%d"),theProductBank.m_PATR_RetryInfo.nSucess);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_PATR_TOTAL, strTotal);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_PATR_SUCESS, strSucess);

	strTotal.Format(_T("%d"),theProductBank.m_PASTAGE_RetryInfo.nTotal);
	strSucess.Format(_T("%d"),theProductBank.m_PASTAGE_RetryInfo.nSucess);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_PAALIGN_TOTAL, strTotal);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_PAALIGN_SUCESS, strSucess);

	strTotal.Format(_T("%d"),theProductBank.m_ULDTR_RetryInfo.nTotal);
	strSucess.Format(_T("%d"),theProductBank.m_ULDTR_RetryInfo.nSucess);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_ULDTR_TOTAL, strTotal);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_ULDTR_SUCESS, strSucess);

	strTotal.Format(_T("%d"),theProductBank.m_OKTR_RetryInfo.nTotal);
	strSucess.Format(_T("%d"),theProductBank.m_OKTR_RetryInfo.nSucess);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_OKTR_TOTAL, strTotal);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_OKTR_SUCESS, strSucess);

	strTotal.Format(_T("%d"),theProductBank.m_NGTR_RetryInfo.nTotal);
	strSucess.Format(_T("%d"),theProductBank.m_NGTR_RetryInfo.nSucess);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_NGTR_TOTAL, strTotal);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_COUNT_NGTR_SUCESS, strSucess);

}
BEGIN_EVENTSINK_MAP(CDlgRetryInfo, CDialog)
	ON_EVENT(CDlgRetryInfo, IDC_GXLBL_CLEAR, DISPID_CLICK, CDlgRetryInfo::ClickGxlblClear, VTS_NONE)
	ON_EVENT(CDlgRetryInfo, IDC_GXLBL_OK, DISPID_CLICK, CDlgRetryInfo::ClickGxlblOk, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgRetryInfo::ClickGxlblClear()
{
	
	//2017-12-01,SKCHO
	theProductBank.InitRetryCount();
}

LRESULT CDlgRetryInfo::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		SetTimer(2001, 500, NULL);
	
	}
	else
	{
		KillTimer(2001);
	}

	return 0;
}


void CDlgRetryInfo::ClickGxlblOk()
{
	ShowWindow(SW_HIDE);
}


void CDlgRetryInfo::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 2001)
	{
		DisplayRetryInfo();
	}

	CDialog::OnTimer(nIDEvent);
}
