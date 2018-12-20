// DlgBarHTeach1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgBarHTeach2.h"


//GxGUIFrame ���
#include "GUIDefine.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "MainFrm.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CDlgBarHTeach1 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgBarHTeach2, CDialogBar)

CDlgBarHTeach2::CDlgBarHTeach2()
{

}

CDlgBarHTeach2::~CDlgBarHTeach2()
{
}

void CDlgBarHTeach2::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgBarHTeach2, CDialogBar)
	// ����� ���� �޼��� ó��
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// ���� ȭ���� ���� �Ϸ�Ǿ���.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// ǥ�� �� �ٲ����.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()


// Active-X Event ó��
BEGIN_EVENTSINK_MAP(CDlgBarHTeach2, CDialogBar)
	ON_EVENT(CDlgBarHTeach2, IDC_BTNX_HTEACH2_WORKTABLE, DISPID_CLICK, CDlgBarHTeach2::OnClickBtnxWorkTable, VTS_NONE)
	ON_EVENT(CDlgBarHTeach2, IDC_BTNX_HTEACH2_UDTR_UDSTAGE, DISPID_CLICK, CDlgBarHTeach2::OnClickBtnx_UD_TrStage, VTS_NONE)
	ON_EVENT(CDlgBarHTeach2, IDC_BTNX_HTEACH2_UDCONVTR, DISPID_CLICK, CDlgBarHTeach2::OnClickBtnx_UDConv, VTS_NONE)
	ON_EVENT(CDlgBarHTeach2, IDC_BTNX_HTEACH2_INSPECTION_UNIT, DISPID_CLICK, CDlgBarHTeach2::OnClickBtn_InspectionUnit, VTS_NONE)
END_EVENTSINK_MAP()

// CDlgBarHTeach1 �޽��� ó�����Դϴ�.



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
LRESULT CDlgBarHTeach2::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	DispCurrentSelect( (UINT)wParam );		// ���õ� �޴� ����

	return 0;
}

// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
LRESULT CDlgBarHTeach2::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CDlgBarHTeach2::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ǥ�ø� �����ϵ��� �Ѵ�.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_HTEACH2_WORKTABLE);
	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���� ȭ������ ���� �����Ѵ�.
void CDlgBarHTeach2::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame���� Message�� ȣ�� ��ȯ�� �����Ѵ�.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);

	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	pMain->SetCurPageTea2(nIdx);
}

// ���� ���õ� ��ư�� ���� �޸����ش�.
void CDlgBarHTeach2::DispCurrentSelect(UINT nID)
{
	// ���� ����� ȭ���� �ش� ��ư�� �ش�ȴٸ� ��ư�� ���� �޸� ���ش�.
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH2_WORKTABLE, ((nID == eFORM_Teach2_WkTable) ?	GXCOLOR_ON : GXCOLOR_OFF));	
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH2_UDTR_UDSTAGE, ((nID == eFORM_Teach2_UD_TrStg) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH2_UDCONVTR, ((nID == eFORM_Teach2_UD_Conv) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH2_INSPECTION_UNIT, ((nID == eFORM_Teach2_Insp) ?	GXCOLOR_ON : GXCOLOR_OFF));
}




// ���� �޴� 1 ����
void CDlgBarHTeach2::OnClickBtnxWorkTable()
{
	ChangeForm( eFORM_Teach2_WkTable );
}


void CDlgBarHTeach2::OnClickBtnx_UD_TrStage()
{
	ChangeForm( eFORM_Teach2_UD_TrStg );	
}


void CDlgBarHTeach2::OnClickBtnx_UDConv()
{
	ChangeForm( eFORM_Teach2_UD_Conv );
}


void CDlgBarHTeach2::OnClickBtn_InspectionUnit()
{
	ChangeForm( eFORM_Teach2_Insp );
}
