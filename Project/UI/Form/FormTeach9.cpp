// FormTeach9.cpp : 구현 파일입니다.
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
ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
END_MESSAGE_MAP()


// CFormTeach9 진단입니다.

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


// CFormTeach9 메시지 처리기입니다.
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

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CFormTeach9::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_TR_Y,0);
}


void CFormTeach9::ClickGxbtnTrayTrYOut()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_TR_Y,1);
}


void CFormTeach9::ClickGxbtnTrayTrZIn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_TR_Z,0);
}


void CFormTeach9::ClickGxbtnTrayTrZOut()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_TR_Z,1);
}


void CFormTeach9::ClickGxbtnTrayLifInUp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_IN_LIFTER_Z,2);
}


void CFormTeach9::ClickGxbtnTrayLifInDown()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_IN_LIFTER_Z,0);
}


void CFormTeach9::ClickGxbtnTrayLifOutUp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_OUT_LIFTER_Z,0);
}


void CFormTeach9::ClickGxbtnTrayLifOutDown()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pMain->SetSelectedAxis(AXIS_TRAY1_OUT_LIFTER_Z,0);
}


//void CFormTeach9::ClickGxbtnTrayTrYIn2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_TR_Y,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayTrYOut2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_TR_Y,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayTrZIn2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_TR_Z,0);
//
//}
//
//
//void CFormTeach9::ClickGxbtnTrayTrZOut2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_TR_Z,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayLifInUp2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_IN_LIFTER_Z,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayLifInDown2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_IN_LIFTER_Z,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayLifOutUp2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_OUT_LIFTER_Z,0);
//}
//
//
//void CFormTeach9::ClickGxbtnTrayLifOutDown2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	m_pMain->SetSelectedAxis(AXIS_TRAY2_OUT_LIFTER_Z,0);
//}
