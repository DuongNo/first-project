
#pragma once
#include "resource.h"

// CDlgUserLogin ��ȭ �����Դϴ�.

class CDlgUserLogin : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserLogin)

public:
	CDlgUserLogin(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgUserLogin();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_USER_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()

	//�����Դϴ�.
protected:
	CString m_strID;
	CString m_strPw;

	BOOL	m_bIDInputed;

	// Function �Դϴ�.
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
