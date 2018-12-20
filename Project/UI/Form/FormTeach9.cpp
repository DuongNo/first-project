// FormTeach9.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeach9.h"


// CFormTeach9

IMPLEMENT_DYNCREATE(CFormTeach9, CFormView)

CFormTeach9::CFormTeach9()
	: CFormView(CFormTeach9::IDD)
{

}

CFormTeach9::~CFormTeach9()
{
}

void CFormTeach9::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeach9, CFormView)
ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
END_MESSAGE_MAP()


// CFormTeach9 �����Դϴ�.

#ifdef _DEBUG
void CFormTeach9::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeach9::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeach9 �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CFormTeach9, CFormView)
	ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_TR_Y_IN, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayTrYIn, VTS_NONE)
	ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_TR_Y_OUT, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayTrYOut, VTS_NONE)
	ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_TR_Z_IN, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayTrZIn, VTS_NONE)
	ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_TR_Z_OUT, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayTrZOut, VTS_NONE)
	ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_LIF_IN_UP, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayLifInUp, VTS_NONE)
	ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_LIF_IN_DOWN, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayLifInDown, VTS_NONE)
	ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_LIF_OUT_UP, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayLifOutUp, VTS_NONE)
	ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_LIF_OUT_DOWN, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayLifOutDown, VTS_NONE)
	//ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_TR_Y_IN2, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayTrYIn2, VTS_NONE)
	//ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_TR_Y_OUT2, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayTrYOut2, VTS_NONE)
	//ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_TR_Z_IN2, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayTrZIn2, VTS_NONE)
	//ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_TR_Z_OUT2, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayTrZOut2, VTS_NONE)
	//ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_LIF_IN_UP2, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayLifInUp2, VTS_NONE)
	//ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_LIF_IN_DOWN2, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayLifInDown2, VTS_NONE)
	//ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_LIF_OUT_UP2, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayLifOutUp2, VTS_NONE)
	//ON_EVENT(CFormTeach9, IDC_GXBTN_TRAY_LIF_OUT_DOWN2, DISPID_CLICK, CFormTeach9::ClickGxbtnTrayLifOutDown2, VTS_NONE)
END_EVENTSINK_MAP()

// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
LRESULT CFormTeach9::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : ȭ�� ������ ǥ�� �ʱ�ȭ�� ���ְ� ���ſ� Ÿ�̸Ӹ� ����/���� �����ش�.
	BOOL bFlag = (BOOL)wParam;
	// ȭ�� ���� Ÿ�̸Ӹ� �����Ű��� �����ΰ� ?
	if ( bFlag )
	{
		m_pMain = (CMainFrame*)AfxGetMainWnd();		
	}
	else
	{

	}

	return 0;
}
void CFormTeach9::ClickGxbtnTrayTrYIn()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_TR_Y,0);
}


void CFormTeach9::ClickGxbtnTrayTrYOut()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_TR_Y,1);
}


void CFormTeach9::ClickGxbtnTrayTrZIn()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_TR_Z,0);
}


void CFormTeach9::ClickGxbtnTrayTrZOut()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_TR_Z,1);
}


void CFormTeach9::ClickGxbtnTrayLifInUp()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_IN_LIFTER_Z,2);
}


void CFormTeach9::ClickGxbtnTrayLifInDown()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_IN_LIFTER_Z,0);
}


void CFormTeach9::ClickGxbtnTrayLifOutUp()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_OUT_LIFTER_Z,0);
}


void CFormTeach9::ClickGxbtnTrayLifOutDown()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_OUT_LIFTER_Z,0);
}


//void CFormTeach9::ClickGxbtnTrayTrYIn2()
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_TR_Y,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayTrYOut2()
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_TR_Y,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayTrZIn2()
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_TR_Z,0);
//
//}
//
//
//void CFormTeach9::ClickGxbtnTrayTrZOut2()
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_TR_Z,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayLifInUp2()
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_IN_LIFTER_Z,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayLifInDown2()
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_IN_LIFTER_Z,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayLifOutUp2()
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_OUT_LIFTER_Z,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayLifOutDown2()
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_OUT_LIFTER_Z,0);
//}
