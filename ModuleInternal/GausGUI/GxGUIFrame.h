// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
//
// CGUIFrame class : 다중 Form 화면 제어(메인 메뉴구성)와 Modaless Dialog 생성 및 관리를 위한 class
//
// 2015-09-18, jhLee, 순수 MFC 만으로 구성하는 다중 폼 메뉴 플랫폼 작성
// 2015-10-05, jhLee, Gaus Lib에 편입
//
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


#pragma once

#include "GausGUI.h"

#include "GxSingleton.h" //for Singleton object

////////////////////////////////////////////////////////////////////////////////////////////////////

// GxGUI 사용을 위한 사용자 정의 Messge 정의
#define GxMSG_GUI_BASE					(WM_USER + 9000)		// 이 프로그램에서 사용할 사용자정의 메세지 Base

#define GxMSG_GUI_CHANGE_VIEW			(GxMSG_GUI_BASE + 1)	// 화면 전환 요청 BottomBar -> MainFrm, 실제 전환 수행
#define GxMSG_GUI_REQUEST_VIEW			(GxMSG_GUI_BASE + 2)	// 화면 전환 요청 AnyWhere -> MainFrm -> BottomBar 요청 전달용
#define GxMSG_GUI_CHANGE_VIEW_END		(GxMSG_GUI_BASE + 3)	// Main Menu 화면이 변경되었다. GxGUIFrame -> MainFrm

#define GxMSG_GUI_CHANGE_LANGUAGE		(GxMSG_GUI_BASE + 10)	// 표시언어를 변경한다.
#define GxMSG_GUI_REQUEST_LANGUAGE		(GxMSG_GUI_BASE + 11)	// 표시언어를 변경할 것을 요청한다.

#define GxMSG_GUI_INITIALIZE			(GxMSG_GUI_BASE + 20)	// 화면을 초기화 한다.
#define GxMSG_GUI_MENULOCK				(GxMSG_GUI_BASE + 21)	// 메뉴 사용을 활성화/비활성화 한다.

#define GxMSG_GUI_UPDATE_TIMER			(GxMSG_GUI_BASE + 30)	// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
#define GxMSG_GUI_UPDATE_DATA			(GxMSG_GUI_BASE + 31)	// View/Dialog 화면을 초기화하고 사용을 활성화 한다.
#define GxMSG_GUI_UPDATE_PROGRESS		(GxMSG_GUI_BASE + 32)	// 진행되는 Progress를 변경시킨다.
#define GxMSG_GUI_UPDATE_RESULT			(GxMSG_GUI_BASE + 33)	// 결과를 전송, Initial 결과
#define GxMSG_GUI_UPDATE_EVENT			(GxMSG_GUI_BASE + 34)	// 각종 Event를 처리한다.


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// Form의 경계를 없애기 위한 Macro, 각 FormView의 PreCreateWindow() 함수에 기술한다.

#define GxDEF_REMOVE_FRAME		cs.style = WS_OVERLAPPED | WS_CHILD;	// 폼의 테두리를 없앤다.

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// Main 화면 View 정보를 담아두는 구조체
typedef struct _TGxViewRec
{
	UINT			nID;					// View의 ID

	CView*		pView;				// View의 포인터
	BOOL			bCreate;				// 새로이 View를 생성해야 하는가 ? (존재하던 View가 아닌경우)

	// 현재 동시에 2개까지만 지원한다. 더 이상의 Dialogbar는 지저분하기때문에 2개까지만 추천한다.
	UINT			nDialogBar1;		// 하위 DialogBar를 지정한다.
	UINT			nDialogBar2;

} TGxViewRec;
typedef CArray<TGxViewRec*, TGxViewRec*> TPArrGxViewRec;


// Dialog 화면 정보를 담아두는 구조체
typedef struct _TGxDialogRec
{
	UINT		nID;					// Index ID
	UINT		nResource;			// Dialog의 Resouce ID

	CDialog*	pDialog;				// Dialog의 포인터

} TGxDialogRec;
typedef CArray<TGxDialogRec*, TGxDialogRec*> TPArrGxDialogRec;


// DialogBar 화면 정보를 담아두는 구조체
typedef struct _TGxDlgBarRec
{
	UINT		nID;					// Index ID
	UINT		nResource;			// DialogBar의 Resouce ID
	UINT		nSide;				// 어느위치에 붙일것인가 ?

	CDialogBar*	pDialog;			// Dialog의 포인터
} TGxDlgBarRec;
typedef CArray<TGxDlgBarRec*, TGxDlgBarRec*> TPArrGxDlgBarRec;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


class _GAUS_GUI CGxGUIFrame : public CGxSingleton<CGxGUIFrame>
{
public:
	CGxGUIFrame();
	~CGxGUIFrame();

public:

	enum {
		// Dialogbar의 위치지정, CreateDialogbar() 에서 사용
		ePOS_DlgBarTop			= CBRS_TOP,
		ePOS_DlgBarBottom		= CBRS_BOTTOM,
		ePOS_DlgBarLeft		= CBRS_LEFT,
		ePOS_DlgBarRight		= CBRS_RIGHT,

		eGxGUI_None				= 0,						// 없다.
		eGxGUI_DefaultView	= 0,						// 프로그램 생성시 만들어지는 기본적인 MainFormView

	};

protected:

	TPArrGxDlgBarRec		m_arrDlgbar;																// Dialogbar 관리용 가변 배열
	TPArrGxViewRec			m_arrView;																	// FormView 관리용 가변 배열
	TPArrGxDialogRec		m_arrDialog;																// Dialog 관리용 가변 배열

	// 현재 화면에 보여지고 있는 View와 Dialogbar
	UINT				m_nCurrentViewID;					// 현재 화면에 보여지고 있는 View의 ID
	UINT				m_nCurrentDialogBarID1;			// 현재 화면에 보여지고 있는 DialogBar의 ID #1, 주로 윗쪽 서브 메뉴
	UINT				m_nCurrentDialogBarID2;			// 현재 화면에 보여지고 있는 DialogBar의 ID #2, 주로 오른쪽 Jog 메뉴

public:


	// 조회 함수
	UINT	GetCurrentViewID();							// 현재 보여지고 있는 View의 ID


	// DialogBar 관련 함수 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  -- -- -- -- --
	void AddDlgBar(CWnd* pOwner, UINT nID, UINT nResource, CDialogBar* pDialog, UINT nSide);	// Dialogbar를 추가한다.
	BOOL ShowDlgBar(UINT nID, UINT nSubID);														// 지정 DialogBar를 화면에 보여준다.
	BOOL HideDlgBar(UINT nID);																			// 지정 DialogBar를 화면에서 감춘다.

	TGxDlgBarRec* GetDlgBarRec(UINT nID);															// 지정 Index의 DialogBarRec 포인터를 조회한다.
	CDialogBar* GetDialogBar(UINT nID);																// 지정 Index의 DialogBar 포인터를 조회한다.
	void RemoveDlgBar(int nID);																		// 지정 DialogBar를 삭제한다.
	void RemoveAllDlgBar();																				// 모든 DialogBar를 삭제한다. (종료시)
	void SendMessageDlgBar(BOOL bPost, UINT nMsg, WPARAM wParam, LPARAM lParam);		// 모든 DialogBar에게 지정 Message를 보낸다.
	void SendMessageDlgBar(UINT nID, BOOL bPost, UINT nMsg, WPARAM wParam, LPARAM lParam);		// 지정 DialogBar에게 지정 Message를 보낸다.
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	// Dialog 관련 함수 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  -- -- -- -- --
	void AddDialog(CWnd* pOwner, UINT nID, UINT nResource, CDialog* pDialog);			// Dialog를 추가한다.
	BOOL ShowDialog(UINT nID, UINT nSubID = 0);													// 지정 Dialog를 화면에 보여준다.
	BOOL HideDialog(UINT nID);																			// 지정 Dialog를 화면에서 감춘다.

	TGxDialogRec* GetDialogRec(UINT nID);															// 지정 Index의 DialogRec 포인터를 조회한다.
	CDialog* GetDialog(UINT nID);																		// 지정 Index의 Dialog 포인터를 조회한다.
	void RemoveDialog(int nID);																		// 지정 Dialog를 삭제한다.
	void RemoveAllDialog();																				// 모든 Dialog를 삭제한다. (종료시 자동 호출)
	void SendMessageDialog(BOOL bPost, UINT nMsg, WPARAM wParam, LPARAM lParam);		// 모든 Dialog에게 지정 Message를 보낸다.
	void SendMessageDialog(UINT nID, BOOL bPost, UINT nMsg, WPARAM wParam, LPARAM lParam);		// 지정 Dialog에게 Message를 보낸다.
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	// FormView 관련 함수 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  -- -- -- -- --
	void AddView(CWnd* pOwner, UINT nID, CView* pView, UINT nBar1, UINT nBar2, BOOL bCreate = TRUE);	// View를 추가한다.
	void AssignDefaultView(UINT nID = eGxGUI_DefaultView);									// 기본 생성 View를 획득한다.
	void ChangeView(UINT nID, BOOL bForce);														// 지정 View로 화면을 변경한다.

	TGxViewRec* GetViewRec(UINT nIndex);															// 지정 Index를 가지는 View를 조회한다. 
	CView* GetView(UINT nID);																			// 지정 View의 포인터를 조회한다.
	void ChangeView(CView* pView);																	// 지정 View로 화면을 변경한다.
	void RemoveAllView();																				// 모든 View를 삭제한다. (종료시)
	void SendMessageView(BOOL bPost, UINT nMsg, WPARAM wParam, LPARAM lParam);			// 모든 View에게 지정 Message를 보낸다.
	void SendMessageView(UINT nID, BOOL bPost, UINT nMsg, WPARAM wParam, LPARAM lParam);		// 모든 View에게 지정 Message를 보낸다.
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

};

