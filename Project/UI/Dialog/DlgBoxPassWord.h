#pragma once
#include "Resource.h"
#include <afxbutton.h>


// CDlgBoxPassWord dialog

class CDlgBoxPassWord : public CDialog
{
	DECLARE_DYNAMIC(CDlgBoxPassWord)

public:
	CDlgBoxPassWord(CWnd* pParent = NULL, CString strCaption=_T("PassWord"), CString strValue=_T(""), BOOL bNotUsePoint=TRUE, double dMin=0, double dMax=100);   // standard constructor
	virtual ~CDlgBoxPassWord();

	CString GetValue();

// Dialog Data
	enum { IDD = IDD_DLG_BOX_PASSWORD };

	CEdit				m_EditNum;
	CMFCButton			m_btnNum[10];
	CMFCButton			m_btnPoint;	
	CMFCButton			m_btnClear;
	CMFCButton			m_btnCancel;
	CMFCButton			m_btnEnter;



	
	CString 			m_strCaption;
	CString 			m_strValue;
	BOOL				m_bNotUsePoint;
	double				m_dMin;
	double				m_dMax;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
CFont	m_fontBold;

	DECLARE_MESSAGE_MAP()

public:
	
	CString m_strReturnText;

public:
	void BnClkNum(int nNum);
	void BnClkChar(CString strChar);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClkPoint();
	
	afx_msg void OnBnClkClear();
	afx_msg void OnBnClkCancel();
	afx_msg void OnBnClkEnter();
	virtual BOOL OnInitDialog();

	
	afx_msg void OnBnClickedBtnNum00();
	afx_msg void OnBnClickedBtnNum01();
	afx_msg void OnBnClickedBtnNum02();
	afx_msg void OnBnClickedBtnNum03();
	afx_msg void OnBnClickedBtnNum04();
	afx_msg void OnBnClickedBtnNum05();
	afx_msg void OnBnClickedBtnNum06();
	afx_msg void OnBnClickedBtnNum07();
	afx_msg void OnBnClickedBtnNum08();
	afx_msg void OnBnClickedBtnNum09();

};
