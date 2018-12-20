// DlgbarHConfig.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgbarHConfig.h"

//GxGUIFrame ���
#include "GUIDefine.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "MainFrm.h"
// CDlgbarHConfig ��ȭ �����Դϴ�.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
IMPLEMENT_DYNAMIC(CDlgbarHConfig, CDialogBar)

CDlgbarHConfig::CDlgbarHConfig()
{

}

CDlgbarHConfig::~CDlgbarHConfig()
{
}

void CDlgbarHConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarHConfig, CDialogBar)
	// ����� ���� �޼��� ó��
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// ���� ȭ���� ���� �Ϸ�Ǿ���.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// ǥ�� �� �ٲ����.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgbarHConfig, CDialogBar)
	ON_EVENT(CDlgbarHConfig, IDC_BTNX_HCONFIG_SUBMENU1, DISPID_CLICK, CDlgbarHConfig::OnClickBtnxSubMenu1, VTS_NONE)
	ON_EVENT(CDlgbarHConfig, IDC_BTNX_HCONFIG_SUBMENU2, DISPID_CLICK, CDlgbarHConfig::OnClickBtnxSubMenu2, VTS_NONE)
	ON_EVENT(CDlgbarHConfig, IDC_BTNX_HCONFIG_OPTIONS, DISPID_CLICK, CDlgbarHConfig::ClickBtnxHconfigOptions, VTS_NONE)
END_EVENTSINK_MAP()
	

// CDlgbarHConfig �޽��� ó�����Դϴ�.
void CDlgbarHConfig::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

BOOL CDlgbarHConfig::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;	// ���� �׵θ��� ���ش�.

	return CDialogBar::PreCreateWindow(cs);
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

// ���� �޴� ���� ȭ�� ��ȯ �Ϸ�
LRESULT CDlgbarHConfig::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	DispCurrentSelect( (UINT)wParam );		// ���õ� �޴� ����

	return 0;
}

// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
LRESULT CDlgbarHConfig::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CDlgbarHConfig::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ǥ�ø� �����ϵ��� �Ѵ�.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_HCONFIG_SUBMENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ���� ���õ� ��ư�� ���� �޸����ش�.
void CDlgbarHConfig::DispCurrentSelect(UINT nID)
{
	// ���� ����� ȭ���� �ش� ��ư�� �ش�ȴٸ� ��ư�� ���� �޸� ���ش�.
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HCONFIG_SUBMENU1, ((nID == eFORM_Config1) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HCONFIG_OPTIONS, ((nID == eFORM_Config2) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HCONFIG_SUBMENU2, ((nID == eFORM_Config3) ?	GXCOLOR_ON : GXCOLOR_OFF));
}




// ���� ȭ������ ���� �����Ѵ�.
void CDlgbarHConfig::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame���� Message�� ȣ�� ��ȯ�� �����Ѵ�.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);

	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	pMain->SetCurPageCon(nIdx);
}


void CDlgbarHConfig::OnClickBtnxSubMenu1()
{
	ChangeForm( eFORM_Config1 );

}


void CDlgbarHConfig::ClickBtnxHconfigOptions()
{
	ChangeForm( eFORM_Config2 );
}

void CDlgbarHConfig::OnClickBtnxSubMenu2()
{
	ChangeForm( eFORM_Config3 );
}
