#pragma once

#include "GausGUI.h"


// #include "resource.h"

// CGxMsgBox 대화 상자입니다.

class _GAUS_GUI CGxMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CGxMsgBox)

public:
	CGxMsgBox(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGxMsgBox();

	// 대화 상자 데이터입니다.
	// enum { IDD = IDD_DLG_MSGBOX };

	enum { eMAX_LANG = 3 };


protected:


	int m_nResult;							// 사용자의 선택
	BOOL m_bConfirm;						// 사용자의 선택을 묻는 것인가 ? TRUE:Yes/No로 선택, FALSE:OK로 선택
	int m_nLangIdx;						// 언어 index

	CString m_sLangName[eMAX_LANG];	// 언어 이름
	CString m_sTitle[eMAX_LANG];		// 언어별 제목 문자열
	CString m_sMsg[eMAX_LANG];			// 언어별 메세지 문자열

	void DispLangSelect();				// 현재 선택된 언어의 버튼을 반전시켜준다.


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

	virtual void OnCancel();
	virtual void OnOK();
	void OnAbort();									// 강제 종료


	// GxOCX Click Event
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnBnClickedBtn_OK();
	afx_msg void OnBnClickedBtn_YES();
	afx_msg void OnBnClickedBtn_NO();

	afx_msg void OnBnClickedBtn_Lang1();
	afx_msg void OnBnClickedBtn_Lang2();
	afx_msg void OnBnClickedBtn_Lang3();

public:


	// 모든 메세지를 한 번에 설정한다.
	void SetLangName(int nIdx, CString sName);			// 지정 순번의 언어 이름을 변경한다.
	void SetTitle(CString sTitle1, CString sTitle2=_T(""), CString sTitle3=_T(""));			// 제목
	void SetMessage(BOOL bConfirm, CString sMsg1, CString sMsg2=_T(""), CString sMsg3=_T(""), int nLang=0);


};
