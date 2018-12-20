// DlgMsgInterlock.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgMsgInterlock2.h"

// #include "MainFrm.h"

#include "GUIDefine.h"

//GxGUIFrame ���
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxStaticEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
// CDlgMsgInterlock2 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMsgInterlock2, CDialog)

CDlgMsgInterlock2::CDlgMsgInterlock2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsgInterlock2::IDD, pParent)
{

}

CDlgMsgInterlock2::~CDlgMsgInterlock2()
{
}

void CDlgMsgInterlock2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMsgInterlock2, CDialog)

	// ����� ���� �޼��� ó��
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// ǥ�� �� �ٲ����.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgMsgInterlock2 �޽��� ó�����Դϴ�.


// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame���� ShowDialog�� �����ϸ� �Ʒ� ������� �޼����� ȣ���Ѵ�.
//
// ShowDialog() ȣ���
// 1. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() ȣ���
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
LRESULT CDlgMsgInterlock2::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : ȭ�� ������ ǥ�� �ʱ�ȭ�� ���ְ� ���ſ� Ÿ�̸Ӹ� ����/���� �����ش�.
	BOOL bFlag = (BOOL)wParam;
	// ȭ�� ���� Ÿ�̸Ӹ� �����Ű��� �����ΰ� ?
	if ( bFlag )
	{

	}
	else
	{

	}

	return 0;
}

// ǥ�� �� ����Ǿ���.
LRESULT CDlgMsgInterlock2::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ǥ�ø� �����ϵ��� �Ѵ�.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
BEGIN_EVENTSINK_MAP(CDlgMsgInterlock2, CDialog)
	ON_EVENT(CDlgMsgInterlock2, IDC_GXBTN_OK, DISPID_CLICK, CDlgMsgInterlock2::ClickGxbtnOk, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgMsgInterlock2::ClickGxbtnOk()
{
	int i = 0;
	SYSTEMTIME AlarmTime;
	CString strTemp;
	KillTimer(1);
	
	//2017-03-22,skcho, interlock �߻��ϸ� ���� ����Ʈ�� ���� �ϱ� ���ؼ�	,cim �߰� ����
	for(i = 0; i < theProcBank.m_MsgUnitInterlockCnt; i++)
	{
		if(i >= INTERLOCKMAX) return;

		strTemp.Format(_T("%s"),theProcBank.m_strUnitInterlockMsg[i]);
		strTemp.TrimRight();	 

		GetLocalTime(&AlarmTime);
		//theProcBank.alarm_info.iERR_ID = ALARM_CIM_INTERLOCK;

		//2017-04-15,skcho, ���߾˶��߻��� �α׿� ��Ȯ�� �ð� ����ϱ� ���ؼ� �迭�� ����
		theProcBank.alarm_info_inter[i].szERR_ID = theProcBank.m_strUnitInterlockID;
		theProcBank.alarm_info_inter[i].szERR_NAME = strTemp;
		//theProcBank.alarm_info[i].TimeOccur = CTime(AlarmTime);
		//theProcBank.alarm_info[i].szLevel = CStringSupport::N2C(ALARM_LEVEL_ALARM);	
			
		//2017-05-18,skcho, ���� ���Ͷ� ID ���� ���Ͷ� ������ �α� ���� ����
		//if (theProcBank.alarm_info_inter[i].szERR_ID != _T(""))
		//{
			theProcBank.alarm_info_inter[i].TimeClear = CTime::GetCurrentTime();		
			theProcBank.alarm_info_inter[i].SetTakeTime();
			theProcBank.alarm_info_inter[i].WriteErrorBuf();
		//}		
	}
	//

	//20170131 byskcho
	for( i = 0; i < 10; i++)
	{
		if(theProcBank.m_strUnitInterlockMsg[i] == _T(""))
		{
			theLog[LOG_ERROR].AddBuf(_T("INTERLOCK\t%d\t%s\t%s\t%d"), ALARM_CIM_INTERLOCK, theProcBank.m_strUnitInterlockMsg[i],"", ALARM_LEVEL_ALARM);
		}

	}

	for ( i = 0; i < 10; i++)
		theProcBank.m_strUnitInterlockMsg[i] = _T("");

	theProcBank.m_strUnitInterlockID = _T("");
	theProcBank.m_MsgUnitInterlockCnt = 0;
	OnOK();
}


void CDlgMsgInterlock2::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	CGxStaticEx* InterlockID = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_INTERLOCK_ID);
	InterlockID->SetCaption(theProcBank.m_strUnitInterlockID);

	CGxStaticEx* InterlockMsg = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_INTERLOCK_MSG);
	InterlockMsg->SetCaption(theProcBank.m_strUnitInterlockMsg[0]);
	SetTimer(1,100,NULL);
}


void CDlgMsgInterlock2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnTimer(nIDEvent);
	KillTimer(1);
	if(nIDEvent == 1)
	{
		CGxStaticEx* InterlockID = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_INTERLOCK_ID);
	    InterlockID->SetCaption(theProcBank.m_strUnitInterlockID);

		CGxStaticEx* InterlockMsg = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_INTERLOCK_MSG);
		InterlockMsg->SetCaption(theProcBank.m_strUnitInterlockMsg[0]+theProcBank.m_strUnitInterlockMsg[1]+theProcBank.m_strUnitInterlockMsg[2]
		+theProcBank.m_strUnitInterlockMsg[3]+theProcBank.m_strUnitInterlockMsg[4]+theProcBank.m_strUnitInterlockMsg[5]
		+theProcBank.m_strUnitInterlockMsg[6]+theProcBank.m_strUnitInterlockMsg[7]+theProcBank.m_strUnitInterlockMsg[8]
		+theProcBank.m_strUnitInterlockMsg[9]);

		SetTimer(1,100,NULL);
	}
}
