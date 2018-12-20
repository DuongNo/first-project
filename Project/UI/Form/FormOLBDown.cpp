// FormOLBDown.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormOLBDown.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "..\Dialog\DlgDoor.h"
#include "Etc\FileSupport.h"
#include "UI\GausGUI\GxMsgBox.h"

// CFormOLBDown

IMPLEMENT_DYNCREATE(CFormOLBDown, CFormView)

CFormOLBDown::CFormOLBDown()
	: CFormView(CFormOLBDown::IDD)
{
	m_nSpanTime = 0;
}

CFormOLBDown::~CFormOLBDown()
{
}

void CFormOLBDown::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormOLBDown, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CFormOLBDown::OnBnClickedButton1)
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFormOLBDown 진단입니다.

#ifdef _DEBUG
void CFormOLBDown::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormOLBDown::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormOLBDown 메시지 처리기입니다.


void CFormOLBDown::OnBnClickedButton1()
{
	ChangeForm(eFORM_Main);
}
// 지정 화면으로 폼을 변경한다.
void CFormOLBDown::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 호면 전환을 지시한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}
// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CFormOLBDown::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		SetTimer(OLBDOWNTIMER,1000,NULL);
		//m_nSpanTime = 0;
	}
	else
	{
		KillTimer(OLBDOWNTIMER);
	}

	return 0;
}

void CFormOLBDown::OnTimer(UINT_PTR nIDEvent)
{
	CString str;
	if(nIDEvent == OLBDOWNTIMER)
	{
		theProcBank.m_InDelayTime++;
		str.Format(_T("경과시간: %d초"),theProcBank.m_InDelayTime);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_TIME,str);
		//2018-03-12,MGYUN, IN IDLE MELSEC WRITE
		theMonitorBank.WriteInDelay(theProcBank.m_InDelayTime);

	}
	CFormView::OnTimer(nIDEvent);
}
