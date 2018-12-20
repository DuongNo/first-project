// DlgLossTime_New.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgLossTimeNew.h"
#include "afxdialogex.h"

#include "GxUtility.h"
#include "File\GxIniFile.h"

#include "UI\Dialog\DlgUserLogin.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxMsgBox.h"

// CDlgLossTimeNew_New 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgLossTimeNew, CDialog)

CDlgLossTimeNew::CDlgLossTimeNew(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLossTimeNew::IDD, pParent)
{
	UINT	nBtnID1[] = {	IDC_GXBTN_LOSSTIME_1ST_1,	IDC_GXBTN_LOSSTIME_1ST_2,	IDC_GXBTN_LOSSTIME_1ST_3,	IDC_GXBTN_LOSSTIME_1ST_4,
								IDC_GXBTN_LOSSTIME_1ST_5,	IDC_GXBTN_LOSSTIME_1ST_6,	IDC_GXBTN_LOSSTIME_1ST_7,	IDC_GXBTN_LOSSTIME_1ST_8 };

	UINT	nBtnID2[] = {	IDC_GXBTN_LOSSTIME_2ND_1,	IDC_GXBTN_LOSSTIME_2ND_2,	IDC_GXBTN_LOSSTIME_2ND_3,	IDC_GXBTN_LOSSTIME_2ND_4,
								IDC_GXBTN_LOSSTIME_2ND_5,	IDC_GXBTN_LOSSTIME_2ND_6,	IDC_GXBTN_LOSSTIME_2ND_7,	IDC_GXBTN_LOSSTIME_2ND_8 };

	UINT	nBtnID3[] = {	IDC_GXBTN_LOSSTIME_3RD_1,	IDC_GXBTN_LOSSTIME_3RD_2,	IDC_GXBTN_LOSSTIME_3RD_3,	IDC_GXBTN_LOSSTIME_3RD_4,
								IDC_GXBTN_LOSSTIME_3RD_5,	IDC_GXBTN_LOSSTIME_3RD_6,	IDC_GXBTN_LOSSTIME_3RD_7,	IDC_GXBTN_LOSSTIME_3RD_8 };
								
	int i;

	for (i = 0; i < eMAX_GroupCount1; i++)
	{
		m_idBtn_1st[i]	= nBtnID1[i];			// 1st class 버튼 관리용
	}

	for (i = 0; i < eMAX_GroupCount2; i++)
	{
		m_idBtn_2nd[i]	= nBtnID2[i];			// 2nd class 버튼 관리용
	}

	for (i = 0; i < eMAX_ItemCount; i++)
	{
		m_idBtn_3rd[i]	= nBtnID3[i];			// 3rd class 버튼 관리용
	}
}

CDlgLossTimeNew::~CDlgLossTimeNew()
{
}

void CDlgLossTimeNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLossTimeNew, CDialog)
	ON_WM_TIMER()
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
END_MESSAGE_MAP()


// CDlgLossTimeNew_New 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDlgLossTimeNew, CDialog)

	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_CONFIRM, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Confirm, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_CANCEL, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Cancel, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_BACK, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Back, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_NEXT, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Next, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_USER, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Login, VTS_NONE)	


	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_1ST_1, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select1stClass1, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_1ST_2, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select1stClass2, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_1ST_3, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select1stClass3, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_1ST_4, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select1stClass4, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_1ST_5, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select1stClass5, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_1ST_6, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select1stClass6, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_1ST_7, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select1stClass7, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_1ST_8, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select1stClass8, VTS_NONE)

	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_2ND_1, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select2ndClass1, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_2ND_2, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select2ndClass2, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_2ND_3, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select2ndClass3, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_2ND_4, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select2ndClass4, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_2ND_5, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select2ndClass5, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_2ND_6, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select2ndClass6, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_2ND_7, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select2ndClass7, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_2ND_8, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select2ndClass8, VTS_NONE)

	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_3RD_1, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select3rdClass1, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_3RD_2, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select3rdClass2, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_3RD_3, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select3rdClass3, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_3RD_4, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select3rdClass4, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_3RD_5, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select3rdClass5, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_3RD_6, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select3rdClass6, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_3RD_7, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select3rdClass7, VTS_NONE)
	ON_EVENT(CDlgLossTimeNew, IDC_GXBTN_LOSSTIME_3RD_8, DISPID_CLICK, CDlgLossTimeNew::ClickGxbtn_Select3rdClass8, VTS_NONE)
END_EVENTSINK_MAP()




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 화면 내용을 Update한다.
LRESULT CDlgLossTimeNew::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	if ( wParam == 1 )
	{
		CString sFile;
		//20170118 byskcho
		//m_pMCMain		= CMCMain::GetInstance();
		//20170118 byskcho
		//m_pLogGUI		= CMCMain::GetInstance()->GetLogPointer( eLOG_UI );		// 사용자 화면 조작 로그
		//m_pLogSeq		= CMCMain::GetInstance()->GetLogPointer( eLOG_SEQ );				// 자동 실행 관련 Log

		//20170118 byskcho
		//m_nLangIdx	= CGxGUIFrame::GetInstance()->GetDispLanguage();

		// 화면 표시를 위한 초기화
		//m_n2ndCount;				// 지정 1st class에서 관리되는 2nd Class 수 최대 8개
		//m_n3rdCount;				// 지정 2st class에서 관리되는 3rd Class 수 최대 24개

		m_nPageCount	= 0;		// 최대 Page 수, 2nd class가 바뀔때마다 class 3rd 수량에 따라서 갱신된다.
		m_nCurrentPage	= 0;		// 현재 보여주고 있는 page
		m_nStartIndex	= 0;		// 현재 page에서의 시작되는 3rd item의 순번, 이 순번 과 눌려진 버튼의 index로 조합하여 사용된다.

		m_nSelect_1st	= 0;		// 선택된 1번째 Class의 순번 , 0 ~ 7
		m_nSelect_2nd	= 0;		// 선택된 2번째 Class의 순번 , 0 ~ 7
		m_nSelect_3rd	= 0;		// 선택된 3번째 Class의 순번 , 0 ~ 7
		//20170118 byskcho
		//m_sWorkUser		= m_pMCMain->GetCurrentUserID();					// 작업자

		DispDefault();				// 기본적인 화면을 그려준다.


		// 현재 비가동 기록 모드 0: STOP으로 전환, 1:Auto로 전환
		// AUTO -> STOP으로 전환
		if ( theLossTimeBank.m_rLosstimeRecord.nMode == eMODE_STATE_AutoToStop )
		{
			m_sSelectReason	= _T("");			// 최종 선택된 중지 사유 초기화
		}
		else		// STOP -> AUTO로 전환
		{
			DispCurrentData();			// 현재 설정된 값들을 화면에 표시해준다.
		}



		SetTimer(eTIMER_Monitoring, 1000, NULL);
	}
	else 
		KillTimer( eTIMER_Monitoring );

	return 0;
}
// 기본적인 화면을 그려준다.
void CDlgLossTimeNew::DispDefault()
{
	int i;

	// 시각 표시
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOSSTIME_TIME1, _T(""));
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOSSTIME_TIME2, _T(""));
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOSSTIME_TIME3, _T(""));


	// 현재 Login된 작업자
	// CGxUICtrl::SetButtonString(this, IDC_GXBTN_LOSSTIME_USER, m_pMCMain->GetUserName(m_sWorkUser, eLANG_KOREAN));

	//20170118 byskcho
	//CGxUICtrl::SetButtonString(this, IDC_GXBTN_LOSSTIME_USER, m_pMCMain->GetUserName(m_sWorkUser, m_nLangIdx));


	// 1st Class 표시
	for (i = 0; i < eMAX_GroupCount1; i++ )
	{
		if ( i < (int)theLossTimeBank.m_rLosstimeList.nCount_1st ) 
		{
			CGxUICtrl::SetButtonString(this, m_idBtn_1st[i], theLossTimeBank.m_rLosstimeList.rLosstime[i].sName);
			CGxUICtrl::SetButtonVisible(this, m_idBtn_1st[i], TRUE);
			CGxUICtrl::SetButtonColor(this, m_idBtn_1st[i], GXCOLOR_OFF);
		}
		else
			CGxUICtrl::SetButtonVisible(this, m_idBtn_1st[i], FALSE);

	}//of for i

	// 2nd class는 표시하지 않는다.
	for (i = 0; i < eMAX_GroupCount2; i++ )
	{
		CGxUICtrl::SetButtonVisible(this, m_idBtn_2nd[i], FALSE);

	}

	// 3rd class는 표시하지 않는다.
	for (i = 0; i < eMAX_ItemCount; i++ )
	{
		CGxUICtrl::SetButtonVisible(this, m_idBtn_3rd[i], FALSE);

	}
}
// 현재 설정된 값을 화면에 반영해 준다.
void CDlgLossTimeNew::DispCurrentData()
{
	CString sMsg;
	int i, nIdx, nCount;
	int nLang = m_nLangIdx;				// 기본 언어

//	CString sClass1, sClass2, sClass3;

	// 시각 표시
	sMsg.Format(_T("%02u:%02u:%02u"), theLossTimeBank.m_rLosstimeRecord.tmStartTime.wHour, theLossTimeBank.m_rLosstimeRecord.tmStartTime.wMinute,
				theLossTimeBank.m_rLosstimeRecord.tmStartTime.wSecond);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOSSTIME_TIME1, sMsg);

	// ::AfxMessageBox(  m_pMCMain->m_rLosstimeRecord.sReasonLine );

	((CEdit*)GetDlgItem(IDC_EDIT_LOSSTIME_TEXT))->SetWindowText( _T("") );			// 입력한 새로운 소분류사유를 가져온다.

	// 이전에 선택한 내용이 없다면 바로 나간다.
	if ( theLossTimeBank.m_rLosstimeRecord.sReasonLine == _T("") ) return;		

	m_sSelectReason = theLossTimeBank.m_rLosstimeRecord.sReasonLine;

	// 선택된 비가동 사유 취득
	// m_sSelectReason = "PM>000>Shift점검>프로그램 점검>Check Program"
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.s1stClass, m_sSelectReason, 0, DELIMITER_DEFECT);			// 1st class
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.sMESCode,  m_sSelectReason, 1, DELIMITER_DEFECT);			// MESCode
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.s2ndClass, m_sSelectReason, 2, DELIMITER_DEFECT);			// 2nd class
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.s3rdClass, m_sSelectReason, 3, DELIMITER_DEFECT);			// 3rd class

	// Class 1의 Index를 얻는다.
	nIdx = -1;
	for (i = 0; i < eMAX_GroupCount1; i++ )
	{
		if ( i < (int)theLossTimeBank.m_rLosstimeList.nCount_1st ) 
		{
			if ( theLossTimeBank.m_rLosstimeRecord.s1stClass == theLossTimeBank.m_rLosstimeList.rLosstime[i].sName )
			{
				nIdx = i;
				break;
			}//of if
		}//of if
	}//of for i

	if ( nIdx < 0 ) return;				// Class1을 찾지 못했다.

	SelectGroup_1st( nIdx );			// 1st Class Group을 선택하여준다.

	// Class 2의 Index를 얻는다.
	nIdx = -1;
	nCount = (int)theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].n2ndCount;
	for (i = 0; i < eMAX_GroupCount2; i++ )
	{
		if ( i < nCount  ) 
		{
			// 3개 국어중 모든 기준은 첫번 째 언어이다. (한국어)
			if ( theLossTimeBank.m_rLosstimeRecord.s2ndClass == theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].r2ndClass[i].sName[0] )
			{
				nIdx = i;
				break;
			}//of if
		}//of if
	}//of for i

	if ( nIdx < 0 ) return;				// Class2을 찾지 못했다.

	SelectGroup_2nd( nIdx );			// 2nd Class Group을 선택하여준다.

	// Class 3의 Index를 얻는다.
	nIdx = -1;
	nCount = (int)theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].r2ndClass[m_nSelect_2nd].n3rdCount;		// 지정 2nd Class의 하위 항목 수 최대 24개

	for (i = 0; i < eMAX_GroupCount3; i++ )
	{
		if ( i < nCount  ) 
		{
			// 한국어가 기준
			if (theLossTimeBank.m_rLosstimeRecord.s3rdClass == theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].r2ndClass[m_nSelect_2nd].r3rdClass[i].sReason[0])
			{
				nIdx = i;
				break;
			}//of if
		}//of if i
	}//of for i
		
	if ( nIdx < 0 ) return;				// Class2을 찾지 못했다.

	int nPage = nIdx / eMAX_ItemCount;		// 해당 항목이 위치한 Page 위치

	DispGroup_3rd( nPage );						// 해당 항목이 있는 3rd Class 를 보여준다.

	
	nIdx = nIdx - m_nStartIndex;				// 해당 페이지에서의 실제 항목 index
	SelectGroup_3rd( nIdx );

	// 사용자 입력
	if ( (theLossTimeBank.m_rLosstimeRecord.s3rdClass == _T("입력")) || (theLossTimeBank.m_rLosstimeRecord.s3rdClass == ("Input") ) )
	{
		((CEdit*)GetDlgItem(IDC_EDIT_LOSSTIME_TEXT))->SetWindowText( theLossTimeBank.m_rLosstimeRecord.sUserMsg );			// 입력한 새로운 소분류사유를 가져온다.
	}
	else
		((CEdit*)GetDlgItem(IDC_EDIT_LOSSTIME_TEXT))->SetWindowText( _T("") );			// 입력한 새로운 소분류사유를 가져온다.

}


void CDlgLossTimeNew::ClickGxbtn_Select1stClass1()
{
	SelectGroup_1st(0);
}

void CDlgLossTimeNew::ClickGxbtn_Select1stClass2()
{
	SelectGroup_1st(1);
}

void CDlgLossTimeNew::ClickGxbtn_Select1stClass3()
{
	SelectGroup_1st(2);
}

void CDlgLossTimeNew::ClickGxbtn_Select1stClass4()
{
	SelectGroup_1st(3);
}

void CDlgLossTimeNew::ClickGxbtn_Select1stClass5()
{
	SelectGroup_1st(4);
}

void CDlgLossTimeNew::ClickGxbtn_Select1stClass6()
{
	SelectGroup_1st(5);
}

void CDlgLossTimeNew::ClickGxbtn_Select1stClass7()
{
	SelectGroup_1st(6);
}

void CDlgLossTimeNew::ClickGxbtn_Select1stClass8()
{
	SelectGroup_1st(7);
}


void CDlgLossTimeNew::ClickGxbtn_Select2ndClass1()
{
	SelectGroup_2nd(0);
}

void CDlgLossTimeNew::ClickGxbtn_Select2ndClass2()
{
	SelectGroup_2nd(1);
}

void CDlgLossTimeNew::ClickGxbtn_Select2ndClass3()
{
	SelectGroup_2nd(2);
}

void CDlgLossTimeNew::ClickGxbtn_Select2ndClass4()
{
	SelectGroup_2nd(3);
}

void CDlgLossTimeNew::ClickGxbtn_Select2ndClass5()
{
	SelectGroup_2nd(4);
}

void CDlgLossTimeNew::ClickGxbtn_Select2ndClass6()
{
	SelectGroup_2nd(5);
}

void CDlgLossTimeNew::ClickGxbtn_Select2ndClass7()
{
	SelectGroup_2nd(6);
}

void CDlgLossTimeNew::ClickGxbtn_Select2ndClass8()
{
	SelectGroup_2nd(7);
}



void CDlgLossTimeNew::ClickGxbtn_Select3rdClass1()
{
	SelectGroup_3rd(0);
}

void CDlgLossTimeNew::ClickGxbtn_Select3rdClass2()
{
	SelectGroup_3rd(1);
}

void CDlgLossTimeNew::ClickGxbtn_Select3rdClass3()
{
	SelectGroup_3rd(2);
}

void CDlgLossTimeNew::ClickGxbtn_Select3rdClass4()
{
	SelectGroup_3rd(3);
}

void CDlgLossTimeNew::ClickGxbtn_Select3rdClass5()
{
	SelectGroup_3rd(4);
}

void CDlgLossTimeNew::ClickGxbtn_Select3rdClass6()
{
	SelectGroup_3rd(5);
}

void CDlgLossTimeNew::ClickGxbtn_Select3rdClass7()
{
	SelectGroup_3rd(6);
}

void CDlgLossTimeNew::ClickGxbtn_Select3rdClass8()
{
	SelectGroup_3rd(7);
}


BOOL CDlgLossTimeNew::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	// Edit 폰트변경
	CFont cFont_Edit;
	// cFont_Edit.CreateFont(15, 15, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("굴림체"));

	cFont_Edit.CreateFont(  24,									// nHeight 
                        0, 										 // nWidth 
                        0, 										 // nEscapement 
                        0, 										 // nOrientation 
                        1, 										  // nWeight 
                        0,											 // bItalic 
                        0, 										 // bUnderline 
                        0, 										 // cStrikeOut 
                        0, 										 // nCharSet 
                        OUT_DEFAULT_PRECIS, 					 // nOutPrecision 
                        0,											 // nClipPrecision 
                        DEFAULT_QUALITY,						 // nQuality
                        DEFAULT_PITCH | FF_DONTCARE,		 // nPitchAndFamily 
                        _T("나눔고딕") );					   // lpszFacename 

	GetDlgItem(IDC_GXBTN_LOSSTIME_USER)->SetFont( &cFont_Edit );
	cFont_Edit.Detach();

	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// 지정 1st class를 선택하였다.
void CDlgLossTimeNew::SelectGroup_1st(int nIdx)
{
	if ( (nIdx < 0) || (nIdx >= eMAX_GroupCount1) ) return;

	int i, j;
	// int nLang = m_nLangIdx;				// 기본 언어
	CGxButtonEx* pBtn = NULL;

	m_sSelectReason	= _T("");			// 최종 선택된 중지 사유 초기화
	m_nSelect_1st = nIdx;				// 선택된 1번째 Class의 순번 , 0 ~ 7

	for (i = 0; i < (int)theLossTimeBank.m_rLosstimeList.nCount_1st; i++ )
	{
		CGxUICtrl::SetButtonColor(this, m_idBtn_1st[i], (i == m_nSelect_1st)  ? GXCOLOR_ON : GXCOLOR_OFF);
	}

	// 하위항목 선택 초기화
	m_nSelect_2nd = 0;
	m_nSelect_3rd = 0;

	// 2nd Class를 그려준다.
	for (i = 0; i < eMAX_GroupCount2; i++ )
	{
		if ( i < (int)theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].n2ndCount ) 
		{
			pBtn = (CGxButtonEx*)GetDlgItem(m_idBtn_2nd[i]);

			for (j = 0; j < eMAX_Language; j++)
			{
				pBtn->SetMultiCaption( j+1, theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].r2ndClass[i].sName[j] );
			}
			pBtn->SetCaptionIndex(m_nLangIdx + 1);		// 현재 보여지는 언어로 설정
			
			//OLD CGxUICtrl::SetButtonString(this, m_idBtn_2nd[i], m_pMCMain->m_rLosstimeList.rLosstime[m_nSelect_1st].r2ndClass[i].sName[nLang]);
			CGxUICtrl::SetButtonVisible(this, m_idBtn_2nd[i], TRUE);
			CGxUICtrl::SetButtonColor(this, m_idBtn_2nd[i], GXCOLOR_OFF);
		}
		else
			CGxUICtrl::SetButtonVisible(this, m_idBtn_2nd[i], FALSE);

	}//of for i

	// 3rd class는 표시하지 않는다.
	for (i = 0; i < eMAX_ItemCount; i++ )
	{
		CGxUICtrl::SetButtonVisible(this, m_idBtn_3rd[i], FALSE);
	}
}

// 지정 2nd class를 선택하였다.
void CDlgLossTimeNew::SelectGroup_2nd(int nIdx)
{
	if ( (nIdx < 0) || (nIdx >= eMAX_GroupCount2) ) return;

	int i;
	int nLang = m_nLangIdx;				// 기본 언어

	m_sSelectReason	= _T("");			// 최종 선택된 중지 사유 초기화
	m_nSelect_2nd = nIdx;				// 선택된 2nd Class의 순번 , 0 ~ 7

	for (i = 0; i < (int)theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].n2ndCount; i++ )
	{
		CGxUICtrl::SetButtonColor(this, m_idBtn_2nd[i], (i == m_nSelect_2nd)  ? GXCOLOR_ON : GXCOLOR_OFF);
	}

	// 하위항목 선택 초기화
	m_nSelect_3rd = 0;

	// 지정 그룹내 defect 정보 표시
	m_n3rdCount			= theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].r2ndClass[m_nSelect_2nd].n3rdCount;		// 지정 2nd Class의 하위 항목 수 최대 24개
	m_nPageCount		= m_n3rdCount / eMAX_ItemCount;									// 최대 Page 수, Group이 바뀔때마다 그룹내 3rd class 수량에 따라서 갱신된다.
	m_nCurrentPage		= 0;																		// 현재 보여주고 있는 page, 그룹이 변경되면 첫번째 페이지로 지정
	m_nStartIndex		= m_nCurrentPage * eMAX_ItemCount;								// 현재 page에서의 시작되는 3rd class의 순번, 이 순번 과 눌려진 버튼의 index로 조합하여 사용된다.

	if ( (m_n3rdCount % eMAX_ItemCount) == 0 ) 
	{
		// 마지막 페이지는 공란으로 남게된다면,
		if ( m_n3rdCount == (m_nPageCount * eMAX_ItemCount) )
		{
			m_nPageCount--;					// Page를 1개 더 셈하지 않도록 한다.
		}
	}

	DispGroup_3rd( m_nCurrentPage );		// 3rd Class 를 보여준다.
}

// 지정 3rd class를 선택하였다.
void CDlgLossTimeNew::SelectGroup_3rd(int nIdx)
{
	if ( (nIdx < 0) || (nIdx >= eMAX_ItemCount) ) return;

	int i;
	int nLang = m_nLangIdx;				// 기본 언어

	m_nSelect_3rd = nIdx;				// 선택된 3rd Class의 순번 , 0 ~ 23

	// 3rd class 표시
	for (i = 0; i < m_nDisp3rdCount; i++)
	{
		CGxUICtrl::SetButtonColor(this, m_idBtn_3rd[i], (i == m_nSelect_3rd)  ? GXCOLOR_ON : GXCOLOR_OFF);
	}//of for i

	// 선택된 최종 값
	m_sSelectReason = theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].r2ndClass[m_nSelect_2nd].r3rdClass[ m_nStartIndex + m_nSelect_3rd ].sLine;
}
void CDlgLossTimeNew::DispGroup_1st()
{
}

void CDlgLossTimeNew::DispGroup_2nd()
{
}

// Page별 3rd class 항목 표시
void CDlgLossTimeNew::DispGroup_3rd(int nPage)
{
	int i, j;
	int nLang = m_nLangIdx;				// 기본 언어
	CString sMsg;
	CGxButtonEx* pBtn = NULL;

	m_nSelect_3rd		= 0;					// 선택되지 않았다.
	m_sSelectReason	= _T("");			// 최종 선택된 중지 사유 초기화

	m_nCurrentPage = nPage;
	if ( nPage == m_nPageCount )									// 마지막 페이지인가 ?
	{
		m_nDisp3rdCount = m_n3rdCount % eMAX_ItemCount;		// 표시되는 수량으로 나머지를 구하여 표시할 

		if ( m_nDisp3rdCount == 0 ) m_nDisp3rdCount = eMAX_ItemCount;
	}
	else // 마지막 페이지가 아니라면, 모두 표시한다.
		m_nDisp3rdCount = eMAX_ItemCount;			

	m_nStartIndex	= m_nCurrentPage * eMAX_ItemCount;		// 현재 page에서의 시작되는 defect의 순번

	// 3rd class 표시
	for (i = 0; i < eMAX_ItemCount; i++)
	{
		if ( i < m_nDisp3rdCount )
		{
			pBtn = (CGxButtonEx*)GetDlgItem(m_idBtn_3rd[i]);

			for (j = 0; j < eMAX_Language; j++)
			{
				pBtn->SetMultiCaption( j+1, theLossTimeBank.m_rLosstimeList.rLosstime[m_nSelect_1st].r2ndClass[m_nSelect_2nd].r3rdClass[ m_nStartIndex + i ].sReason[j] );
			}
			pBtn->SetCaptionIndex(m_nLangIdx + 1);		// 현재 보여지는 언어로 설정


			//old sMsg = m_pMCMain->m_rLosstimeList.rLosstime[m_nSelect_1st].r2ndClass[m_nSelect_2nd].r3rdClass[ m_nStartIndex + i ].sReason[nLang];
			//old CGxUICtrl::SetButtonString(this, m_idBtn_3rd[i], sMsg);					// 최종항목 이름 표시

			CGxUICtrl::SetButtonVisible(this, m_idBtn_3rd[i], TRUE);					// 버튼을 표시한다.
			CGxUICtrl::SetButtonColor(this, m_idBtn_3rd[i], GXCOLOR_OFF);	// 선택되지 않았음을 표시
		}
		else
			{
				CGxUICtrl::SetButtonVisible(this, m_idBtn_3rd[i], FALSE);		// 버튼을 감춘다.
			}
	}//of for i

	// 하단에 Back/Next 버튼 표시여부
	CGxUICtrl::SetButtonVisible(this, IDC_GXBTN_LOSSTIME_NEXT, ( m_nCurrentPage != m_nPageCount) );		// 다음 페이지 버튼
	CGxUICtrl::SetButtonVisible(this, IDC_GXBTN_LOSSTIME_BACK, ( m_nCurrentPage != 0) );					// 이전 페이지 버튼

	// Page 정보 표시
	sMsg.Format(_T("%d / %d"), m_nCurrentPage + 1, m_nPageCount + 1);
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOSSTIME_PAGE, sMsg);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// OK 버튼 클릭
void CDlgLossTimeNew::ClickGxbtn_Confirm()
{
	if ( m_sSelectReason	== _T("") )			// 최종 선택된 중지 사유 선택않됨
	{
		return;			// 
	}

	/*if ( m_sWorkUser == _T("") )				// 입력된 작업자가 없으면
	{
		return;
	}*/
		
	CString sMsg = _T("");

	// 선택된 비가동 사유 취득
	// m_sSelectReason = "PM>000>Shift점검>프로그램 점검>Check Program"
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.s1stClass, m_sSelectReason, 0, DELIMITER_DEFECT);			// 1st class
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.sMESCode, m_sSelectReason, 1, DELIMITER_DEFECT);			// MESCode
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.s2ndClass, m_sSelectReason, 2, DELIMITER_DEFECT);			// 2nd class
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.s3rdClass, m_sSelectReason, 3, DELIMITER_DEFECT);			// 3rd class
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.sTPSystemName, m_sSelectReason, 4, DELIMITER_DEFECT);	// 3rd class 영문 = TPSystem 설명

	theLossTimeBank.m_rLosstimeRecord.sTPSystemCode = theLossTimeBank.m_rLosstimeRecord.sMESCode;				// TPSystem Code

	theLossTimeBank.m_rLosstimeRecord.sID			= m_sWorkUser;	// m_pMCMain->GetCurrentUserID();			// 현재 작업자 ID로 대입
	theLossTimeBank.m_rLosstimeRecord.sIDNo			= m_sWorkUser;	// m_pMCMain->GetUserIDNo(m_sWorkUser);
	theLossTimeBank.m_rLosstimeRecord.sReasonLine	= m_sSelectReason;						// 선택된 내용

		// 소분류를 작업자가 직접 입력하도록 지정하였다면,
	if ( (theLossTimeBank.m_rLosstimeRecord.s3rdClass == _T("입력")) || (theLossTimeBank.m_rLosstimeRecord.s3rdClass == ("Input") ) )
	{
		((CEdit*)GetDlgItem(IDC_EDIT_LOSSTIME_TEXT))->GetWindowText( sMsg );			// 입력한 새로운 소분류사유를 가져온다.

		theLossTimeBank.m_rLosstimeRecord.sUserMsg	= sMsg;						// 사용자 정의 입력
	}

	// Auto -> Stop으로의 전환인가 ?
	if ( theLossTimeBank.m_rLosstimeRecord.nMode == eMODE_STATE_AutoToStop )
	{
		// 시작 시간을 갱신한다. 
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmStartTime) );	// 현재 시각으로 초기화 치환한다.
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmResetTime) );	
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmEndTime) );

		//20170118 byskcho 삭제
		//theLossTimeBank.m_sTPSystemCode = theLossTimeBank.m_rLosstimeRecord.sTPSystemCode;	// TPSystem CODE 상위에 보고되는 변수에 저장 20141226-2 jhkim1
		//theLossTimeBank.m_sTPSystemName = theLossTimeBank.m_rLosstimeRecord.sTPSystemName;	// TPSystem NAME 상위에 보고되는 변수에 저장 20141226-2 jhkim1

		//?		theLossTimeBank.SaveTPSyetem(/*theLossTimeBank.m_sTPSystemCode, theLossTimeBank.m_sTPSystemName*/);	// TPSystem CODE, NAME 불러오기 위해 저장하는 함수 호출 20141226-2 jhkim1

		// 선택된 내용 저장
		// 현재 설정된 비가동 정보를 파일로 기록한다. (임시보관)
		theLossTimeBank.SaveLosstimeInfo();

		//2014-09-16, jhLee, Loading 작업중이라면, 다시 AUTO로 전환될 때 TACT Time용 PG-OFF시간을 갱신하기위해 Flag를 지정해준다.
		// 일상적으로 AUTO -> STOP으로 전환 
		//20170118 byskho 임시삭제
		//m_pMCMain->m_rProfileData.bTACTStartTimeUpdateFlag	= TRUE;					// TACT 측정 START시간 갱신 가능하도록 지정
		//m_pMCMain->ClearRunDownTimer();					// AutoRunDown 타이머를 초기화 해준다.


		//2014-11-10, jhLee, 김길성책임 요청, 대분류가 RD/OD를 선택한 경우 작업자 Logout을 실시한다.
		if ( (theLossTimeBank.m_rLosstimeRecord.s1stClass == _T("RD")) || (theLossTimeBank.m_rLosstimeRecord.s1stClass == _T("OD")) )
		{
			
		}

		SleepEx(25, NULL);			// 2015-03-07, jhLee, 멈추는 현상 목격, 곧바로 전환하지 않고 약간의 시간을 두고 수행하도록 Sleep을 주었다. 

		//CIM 데이터 전송
		theSocketInterFace.SendCmdTPSystemToDataPC(theLossTimeBank.m_rLosstimeRecord.sMESCode,theLossTimeBank.m_rLosstimeRecord.sTPSystemName);
		//20170118 byskcho 구조체 사용안하므로 삭제
		//theProcBank.ResetLossData();
		theProcBank.m_bUserStop = FALSE;

	}
	else		// STOP -> AUTO로의 전환인가 ?
	{
		if ( theLossTimeBank.m_rLosstimeRecord.sAlarmType != _T("A") )					// Alarm에 의한 정지가 아니 였다면,
		{
			// MD가 아닌경우 무의미
			::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmResetTime) );			// Reset 시간을 현재 시각으로 맞춘다.
		}

		// 비가동 종료 시각
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmEndTime) );				// 처리 종료하고 다시 시작 시간

		// Login과는 다른 사용자 입력이다.
		theLossTimeBank.m_rLosstimeRecord.sID	= m_sWorkUser;	// theLossTimeBank.GetCurrentUserID();		// 현재 작업자 ID로 대입, 로그인한 사용자가 아닌, 현재 폼에서 입력한 작업자를 기입힌다.
		theLossTimeBank.m_rLosstimeRecord.sIDNo	= m_sWorkUser;	// theLossTimeBank.GetUserIDNo( m_sWorkUser );


		//20170118 byskho 임시삭제rundown 기능 구현되지 않음
		// 중지 사유에 따른 처리
		// Auto RunDown에 의해 중지 되었다면,
		/*if ( (theLossTimeBank.m_rLosstimeRecord.s2ndClass == _T("AUTO RUN DOWN")) || (theLossTimeBank.m_rLosstimeRecord.s2ndClass == _T("AUTO RUNDOWN") ) )
		{
			// 비가동 Log 저장시 AUTO RUn down 기준 시간을 더하여 기록한다.
			theLossTimeBank.m_rLosstimeRecord.dbAddLossTime =  theLossTimeBank.m_rProfileData.nTIME_RunDownTime;
		}
		else
	 		// theLossTimeBank.WriteLosstimeData( 0 );														// 비가동 Log 저장
			theLossTimeBank.m_rLosstimeRecord.dbAddLossTime = 0.0;
		*/

		// 파일로 기록을 한다.
		//old theLossTimeBank.WriteLossTimeRecord( theLossTimeBank.m_rLosstimeRecord, FALSE );			 
		theLossTimeBank.WriteLossTimeRecord( theLossTimeBank.m_rLosstimeRecord, FALSE );			 

		theLossTimeBank.ClearLosstimeInfo();														// 비가동 임시기억 초기화
	
		//20170118 byskho 임시삭제rundown 기능 구현되지 않음
		//m_pMCMain->m_rProfileData.bTACTStartTimeUpdateFlag	= FALSE;			// 정상적인 방법으로 STOP으로 전환여부 Flag 초기화
		//m_pMCMain->ClearRunDownTimer();					// RunDown 타이머를 초기화 해준다.

		//pMaster->SetAuto();			// Auto 지정
		theProcBank.m_bUserStop = TRUE;
		//20170118 byskho 디펙위치 입력 기능 없음 삭제
		// Defect을 입력해야하는 Mode라면,
		//if ( theLossTimeBank.GetDefectInputMode() == TRUE )
		//{
		//	::PostMessage(((CMainFrame*)AfxGetMainWnd())->m_hWnd, UDMSG_DISPFROM_DEFECT, 1, 0);		// Defect 입력 폼으로 전환 
		//}
	} //of if
	
	ShowWindow(SW_HIDE);							// 화면을 감춘다.
}


// 취소 버튼 클릭
void CDlgLossTimeNew::ClickGxbtn_Cancel()
{
	ShowWindow(SW_HIDE);							// 화면을 감춘다.
}

// 이전 페이지로 이동
void CDlgLossTimeNew::ClickGxbtn_Back()
{
	if ( m_nCurrentPage > 0 ) m_nCurrentPage--;		// 페이지 감소 이동

	DispGroup_3rd(m_nCurrentPage);						
}

// 다음 페이지로 이동
void CDlgLossTimeNew::ClickGxbtn_Next()
{
	if ( m_nCurrentPage < m_nPageCount ) m_nCurrentPage++;

	DispGroup_3rd(m_nCurrentPage);						
}


// 비가동 사유를 입력시에는 현재 로그인 된 작업자 외에 별도의 작업자를 지정할 수 있다.
// 새로운 작업자 입력
void CDlgLossTimeNew::ClickGxbtn_Login()
{
	// 파일 확인
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	CString strFileName;
	strFileName.Format(_T("%s\\%s") ,theConfigBank.m_System.m_strConfigPath, NAME_FILE_USER_LIST);			// 읽어 올 파일

	// 파일이 존재하는지 점검
	if ( PathFileExists( strFileName ) == FALSE )		//  파일 존재여부
	{
		return;
	}
	
	CString strUserID = _T("");
	CString strNewPw = _T("");
	int nLevel = 0;

	CDlgUserLogin dlgLogIn;
	if( dlgLogIn.DoModal() == IDOK)
	{
		//m_sWorkUser = dlgLogIn.GetUserID();
		CGxUICtrl::SetButtonString(this, IDC_GXBTN_LOSSTIME_USER, pMain->GetCurUserData().sID);
	}

	//CGxUICtrl::SetButtonString(this, IDC_GXBTN_LOSSTIME_USER,m_pMCMain->GetUserName(m_sWorkUser, eLANG_KOREAN));



}
// 현재 시각을 반영한다.
void CDlgLossTimeNew::DispCurrentTime()
{
	CString sTime;
	SYSTEMTIME tmNow;

	::GetLocalTime( &tmNow );
	sTime.Format(_T("%02u:%02u:%02u"), tmNow.wHour, tmNow.wMinute, tmNow.wSecond);


	// Auto -> Stop으로의 전환인가 ?
	if ( theLossTimeBank.m_rLosstimeRecord.nMode == eMODE_STATE_AutoToStop )
	{


		// 시작 시간을 갱신한다.
		// ::GetLocalTime( &(m_pMCMain->m_rLosstimeRecord.tmStartTime) );	// 현재 시각으로 치환한다.
		// ::GetLocalTime( &(m_pMCMain->m_rLosstimeRecord.tmResetTime) );	
		// ::GetLocalTime( &(m_pMCMain->m_rLosstimeRecord.tmEndTime) );

		((CEdit*)GetDlgItem(IDC_EDIT_LOSSTIME_TEXT))->SetWindowText( _T("") );			// 입력한 새로운 소분류사유를 가져온다.


		CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOSSTIME_TIME1, sTime);
	}
	else	// Stop -> Auto로의 전환인가 ?
	{
		SYSTEMTIME tmDiff;

		// sTime.Format(_T("%02u:%02u:%02u"), m_pMCMain->m_rLosstimeRecord.tmStartTime.wHour, m_pMCMain->m_rLosstimeRecord.tmStartTime.wMinute,
		// 	m_pMCMain->m_rLosstimeRecord.tmStartTime.wSecond);
		//
		// CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOSSTIME_TIME1, sTime);

		// 종료 시간을 갱신한다.
		// ::GetLocalTime( &(m_pMCMain->m_rLosstimeRecord.tmEndTime) );
		//
		// sTime.Format(_T("%02u:%02u:%02u"), m_pMCMain->m_rLosstimeRecord.tmEndTime.wHour, m_pMCMain->m_rLosstimeRecord.tmEndTime.wMinute,
		//		m_pMCMain->m_rLosstimeRecord.tmEndTime.wSecond);


		CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOSSTIME_TIME2, sTime);						// 종료시각 증가

		//
		// 경과시간
		CGxUtility::DiffTimes(theLossTimeBank.m_rLosstimeRecord.tmStartTime, tmNow, tmDiff);

		sTime.Format(_T("%02u:%02u:%02u"), tmDiff.wHour, tmDiff.wMinute, tmDiff.wSecond);
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOSSTIME_TIME3, sTime);
	}
}


void CDlgLossTimeNew::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ( nIDEvent == eTIMER_Monitoring )
	{
		DispCurrentTime();							// 현재 시각을 반영해준다.
	}
	CDialog::OnTimer(nIDEvent);
}
