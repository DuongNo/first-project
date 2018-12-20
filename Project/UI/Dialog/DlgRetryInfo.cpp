// DlgRetryInfo.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgRetryInfo.h"
#include "afxdialogex.h"


//GxGUIFrame ���
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

// CDlgRetryInfo ��ȭ �����Դϴ�.

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
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,	OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgRetryInfo �޽��� ó�����Դϴ�.
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
	// Todo : ȭ�� ������ ǥ�� �ʱ�ȭ�� ���ְ� ���ſ� Ÿ�̸Ӹ� ����/���� �����ش�.
	BOOL bFlag = (BOOL)wParam;
	// ȭ�� ���� Ÿ�̸Ӹ� �����Ű��� �����ΰ� ?
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
