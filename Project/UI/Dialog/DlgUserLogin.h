
#pragma once
#include "resource.h"

// CDlgUserLogin 대화 상자입니다.

class CDlgUserLogin : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserLogin)

public:
	CDlgUserLogin(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgUserLogin();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_USER_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()

	//변수입니다.
protected:
	CString m_strID;
	CString m_strPw;

	BOOL	m_bIDInputed;

	// Function 입니다.
protected:
	void ClickGxbtnText(UINT nID);
	void DispUserID(CString strText);
	void DispUserPw(CString strText);
	void ChangeUser(CUserAccount user);

public:
		
public:
	void ClickGxbtnEnter();
	void ClickGxbtnClear();
	void ClearLoginData();
	void ClickGxbtnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickGxbtnLoginout();
};
