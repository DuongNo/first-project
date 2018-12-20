#pragma once
#include "Resource.h"
#include <afxbutton.h>


// CDlgBoxKeyBoard dialog

class CDlgBoxKeyBoard : public CDialog
{
	DECLARE_DYNAMIC(CDlgBoxKeyBoard)

public:
	CDlgBoxKeyBoard(CWnd* pParent = NULL, CString strCaption=_T("Number Pad"), CString strValue=_T(""), BOOL bNotUsePoint=TRUE, double dMin=0, double dMax=100);   // standard constructor
	virtual ~CDlgBoxKeyBoard();

	CString GetValue();

// Dialog Data
	enum { IDD = IDD_DLG_BOX_KEYBOARD };

	CEdit				m_EditNum;
	CMFCButton			m_btnNum[10];
	CMFCButton			m_btnPoint;
	CMFCButton			m_btnMinus;
	CMFCButton			m_btnClear;
	CMFCButton			m_btnCancel;
	CMFCButton			m_btnEnter;
	CMFCButton			m_btnUnderBar;

	CMFCButton			m_btnChar[26];
	
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
	afx_msg void OnBnClkMinus();
	afx_msg void OnBnClkClear();
	afx_msg void OnBnClkCancel();
	afx_msg void OnBnClkEnter();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClkUnderBar();
	
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
	afx_msg void OnBnClickedBtnStringA();
	afx_msg void OnBnClickedBtnStringB();
	afx_msg void OnBnClickedBtnStringC();
	afx_msg void OnBnClickedBtnStringD();
	afx_msg void OnBnClickedBtnStringE();
	afx_msg void OnBnClickedBtnStringF();
	afx_msg void OnBnClickedBtnStringG();
	afx_msg void OnBnClickedBtnStringH();
	afx_msg void OnBnClickedBtnStringI();
	afx_msg void OnBnClickedBtnStringJ();
	afx_msg void OnBnClickedBtnStringK();
	afx_msg void OnBnClickedBtnStringL();
	afx_msg void OnBnClickedBtnStringM();
	afx_msg void OnBnClickedBtnStringN();
	afx_msg void OnBnClickedBtnStringO();
	afx_msg void OnBnClickedBtnStringP();
	afx_msg void OnBnClickedBtnStringQ();
	afx_msg void OnBnClickedBtnStringR();
	afx_msg void OnBnClickedBtnStringS();
	afx_msg void OnBnClickedBtnStringT();
	afx_msg void OnBnClickedBtnStringU();
	afx_msg void OnBnClickedBtnStringV();
	afx_msg void OnBnClickedBtnStringW();
	afx_msg void OnBnClickedBtnStringX();
	afx_msg void OnBnClickedBtnStringY();
	afx_msg void OnBnClickedBtnStringZ();
};
