// DlgAGVMsg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgAGVMsg.h"
#include "afxdialogex.h"

#include "..\..\..\ModuleInternal\UI\GausGUI\GxStaticEx.h"
// CDlgAGVMsg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAGVMsg, CDialogEx)

CDlgAGVMsg::CDlgAGVMsg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAGVMsg::IDD, pParent)
{

}

CDlgAGVMsg::~CDlgAGVMsg()
{
}

void CDlgAGVMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAGVMsg, CDialogEx)
	ON_MESSAGE(UDMSG_DISPDLG_AGV,OnShowMsg)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CDlgAGVMsg, CDialogEx)
	
	ON_EVENT(CDlgAGVMsg, IDC_GXBTN_MSG_AGV_OK, DISPID_CLICK, CDlgAGVMsg::ClickGxbtnMsgAgvOk, VTS_NONE)
	
	ON_EVENT(CDlgAGVMsg, IDC_GXBTN_AGV_MSG_CANCEL, DISPID_CLICK, CDlgAGVMsg::ClickGxbtnAgvMsgCancel, VTS_NONE)
END_EVENTSINK_MAP()
// CDlgAGVMsg 메시지 처리기입니다.


void CDlgAGVMsg::ClickGxbtnMsgAgvOk()
{	
	//int nLampState = theUnitFunc.GetMachinetoLampState();

	//theUnitFunc.LampChange(theConfigBank.m_Option.iMapLampSettingList[(STATUS_LAMP_DESCRIPTION)nLampState].Red.lamp_state
	//,theConfigBank.m_Option.iMapLampSettingList[(STATUS_LAMP_DESCRIPTION)nLampState].Yellow.lamp_state
	//,theConfigBank.m_Option.iMapLampSettingList[(STATUS_LAMP_DESCRIPTION)nLampState].Green.lamp_state
	//,0);

	//요청 대기 재시작
	if(theProcBank.m_nAGVDelayStep == 5)
	{
		theProcBank.m_nAGVDelayStep = 1;
	}
	if(theProcBank.m_nOutAGVDelayStep ==5)
	{
		theProcBank.m_nOutAGVDelayStep = 1;
	}
	ShowWindow(SW_HIDE);
}

LRESULT CDlgAGVMsg::OnShowMsg( WPARAM wParam, LPARAM lParam )
{
	
	
	return 0;
}

void CDlgAGVMsg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	/*CString strMsg;
	CGxStaticEx* str = (CGxStaticEx*)GetDlgItem(IDC_GXBTN_MSG);
	if(theProcBank.m_nAGVDelayStep == 3)
	{
		strMsg.Format(_T("IN PORT AGV 요청 TIME OUT"));
		str->SetCaption(strMsg);
	}
	if(theProcBank.m_nOutAGVDelayStep == 3)
	{
		strMsg.Format(_T("OUT PORT AGV 요청 TIME OUT"));
		str->SetCaption(strMsg);
	}	
	*/
	
}


void CDlgAGVMsg::ClickGxbtnAgvMsgCancel()
{
	//요청 대기 재시작
	if(theProcBank.m_nAGVDelayStep == 5)
	{
		theProcBank.m_nAGVDelayStep = 1;
	}
	if(theProcBank.m_nOutAGVDelayStep ==5)
	{
		theProcBank.m_nOutAGVDelayStep = 1;
	}
	ShowWindow(SW_HIDE);
}
