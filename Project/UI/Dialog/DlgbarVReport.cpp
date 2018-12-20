// DlgbarVReport.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgbarVReport.h"

#include "GUIDefine.h"

//GxGUIFrame ���
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "MainFrm.h"

// CDlgbarVReport ��ȭ �����Դϴ�.
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDlgbarVReport, CDialogBar)

CDlgbarVReport::CDlgbarVReport()
{

}

CDlgbarVReport::~CDlgbarVReport()
{
}

void CDlgbarVReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarVReport, CDialogBar)
	// ON_UPDATE_COMMAND_UI(IDC_BTN_VREPORT_SUBMENU1, OnUpdateCmdUI)
	// ON_BN_CLICKED(IDC_BTN_VREPORT_SUBMENU1, &CDlgbarVReport::OnBnClickedBtnVreportSubmenu1)

	// ����� ���� �޼��� ó��
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// ���� ȭ���� ���� �Ϸ�Ǿ���.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// ǥ�� �� �ٲ����.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()

// ActivX Event ó��
BEGIN_EVENTSINK_MAP(CDlgbarVReport, CDialogBar)
	ON_EVENT(CDlgbarVReport, IDC_BTNX_VREPORT_SUBMENU1, DISPID_CLICK, CDlgbarVReport::OnClickBtnxSubMenu1, VTS_NONE)
	ON_EVENT(CDlgbarVReport, IDC_BTNX_VREPORT_SUBMENU2, DISPID_CLICK, CDlgbarVReport::OnClickBtnxSubMenu2, VTS_NONE)
	ON_EVENT(CDlgbarVReport, IDC_BTNX_VREPORT_SUBMENU3, DISPID_CLICK, CDlgbarVReport::OnClickBtnxSubMenu3, VTS_NONE)
	ON_EVENT(CDlgbarVReport, IDC_BTNX_VREPORT_SUBMENU4, DISPID_CLICK, CDlgbarVReport::OnClickBtnxSubMenu4, VTS_NONE)
END_EVENTSINK_MAP()



// CDlgbarVReport �޽��� ó�����Դϴ�.
// CDlgbarHReport �޽��� ó�����Դϴ�.
void CDlgbarVReport::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame���� ShowDialog�� �����ϸ� �Ʒ� ������� �޼����� ȣ���Ѵ�.
//
// ShowDlgBar() ȣ���
// 2. GxMSG_GUI_CHANGE_SUBMENU,	nSubID
// 3. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() ȣ���
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// ȭ�� ������ �Ϸ�Ǿ���.
LRESULT CDlgbarVReport::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	DispCurrentSelect( (UINT)wParam );		// ���õ� �޴� ����

	return 0;
}

// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
LRESULT CDlgbarVReport::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CDlgbarVReport::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ǥ�ø� �����ϵ��� �Ѵ�.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_VREPORT_SUBMENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���� ȭ������ ���� �����Ѵ�.
void CDlgbarVReport::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame���� Message�� ȣ�� ��ȯ�� �����Ѵ�.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}


// ���� ���õ� ��ư�� ���� �޸����ش�.
void CDlgbarVReport::DispCurrentSelect(UINT nID)
{
	// ���� ����� ȭ���� �ش� ��ư�� �ش�ȴٸ� ��ư�� ���� �޸� ���ش�.
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_VREPORT_SUBMENU1, ((nID == eFORM_Report1) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_VREPORT_SUBMENU2, ((nID == eFORM_Report2) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_VREPORT_SUBMENU3, ((nID == eFORM_Report3) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_VREPORT_SUBMENU4, ((nID == eFORM_Report4) ?	GXCOLOR_ON : GXCOLOR_OFF));
}


// Sub menu 1 ����
void CDlgbarVReport::OnClickBtnxSubMenu1()
{
	ChangeForm( eFORM_Report1 );
}

// Sub menu 2 ����
void CDlgbarVReport::OnClickBtnxSubMenu2()
{
	ChangeForm( eFORM_Report2 );
}

// Sub menu 3 ����
void CDlgbarVReport::OnClickBtnxSubMenu3()
{
	ChangeForm( eFORM_Report3 );
}

// Sub menu 4 ����
void CDlgbarVReport::OnClickBtnxSubMenu4()
{
	ChangeForm( eFORM_Report4 );
}

// Sub menu 5 ����
void CDlgbarVReport::OnClickBtnxSubMenu5()
{
	ChangeForm( eFORM_Report5 );
}