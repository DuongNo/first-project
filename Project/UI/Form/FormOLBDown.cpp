// FormOLBDown.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "resource.h"
#include "FormOLBDown.h"

#include "GUIDefine.h"

//GxGUIFrame ���
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
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFormOLBDown �����Դϴ�.

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


// CFormOLBDown �޽��� ó�����Դϴ�.


void CFormOLBDown::OnBnClickedButton1()
{
	ChangeForm(eFORM_Main);
}
// ���� ȭ������ ���� �����Ѵ�.
void CFormOLBDown::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame���� Message�� ȣ�� ��ȯ�� �����Ѵ�.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}
// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
LRESULT CFormOLBDown::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : ȭ�� ������ ǥ�� �ʱ�ȭ�� ���ְ� ���ſ� Ÿ�̸Ӹ� ����/���� �����ش�.
	BOOL bFlag = (BOOL)wParam;
	// ȭ�� ���� Ÿ�̸Ӹ� �����Ű��� �����ΰ� ?
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
		str.Format(_T("����ð�: %d��"),theProcBank.m_InDelayTime);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_TIME,str);
		//2018-03-12,MGYUN, IN IDLE MELSEC WRITE
		theMonitorBank.WriteInDelay(theProcBank.m_InDelayTime);

	}
	CFormView::OnTimer(nIDEvent);
}
