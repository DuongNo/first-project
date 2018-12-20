//
// CGxNumPad : 사용자로부터 숫자를 입력받는 키보드 다이얼로그 
//
//

#pragma once

#include "GausGUI.h"


// #include "resource.h"

// CGxNumPad 대화 상자입니다.
class _GAUS_GUI CGxNumPad : public CDialog
{
	DECLARE_DYNAMIC(CGxNumPad)

public:
	CGxNumPad(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGxNumPad();

	// enum { IDD = IDD_DLG_NUMPAD1 };		

	enum { MAX_LENGTH = 12 };				// 최대 글자수

protected:

	CString		m_sTitle;				// 다이얼로그 제목
	CString		m_sEdit;					// 편집중인 글자
	BOOL			m_bIsPassword;			// 암호 입력 모드
	BOOL			m_bIsInteger;			// 정수 입력 모드
	BOOL			m_bIsPositive;			// 양의 수치만 입력 모드
	int			m_nPassLength;			// 암호 입력 최대 글자수

	void DisplayBtnMsg(UINT nID, CString sMsg);			// 현재 편집중인 글자를 표시해준다.
	void DisplayLabelMsg(UINT nID, CString sMsg);		// Static에 문자열을 표시한다.

	// 글자를 조립한다.
	void InputNumKey(CString sKey);		// 지정 키를 입력한다.
	CString GetPasswordMask(int nLen);	// 지정 길이만큼 암호 * 문자를 되돌려준다.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void OnAbort();									// 강제 종료

	DECLARE_MESSAGE_MAP()


	DECLARE_EVENTSINK_MAP()
	// 숫자 입력
	void ClickBtnxNum0();
	void ClickBtnxNum1();
	void ClickBtnxNum2();
	void ClickBtnxNum3();
	void ClickBtnxNum4();
	void ClickBtnxNum5();
	void ClickBtnxNum6();
	void ClickBtnxNum7();
	void ClickBtnxNum8();
	void ClickBtnxNum9();

	// 특수 기능
	void ClickBtnxMinus();
	void ClickBtnxDot();
	void ClickBtnxClear();
	void ClickBtnxCancel();
	void ClickBtnxEnter();
	void ClickBtnxNumpadDisplay();


public:

	// 외부 노출 내용
	void SetTitle(CString sMsg);						// 다이얼로그 표시내용
	void SetPasswordMode(BOOL bFlag, int nLen = MAX_LENGTH);				// 암호 입력 모드인가 ?
	void SetIntegerMode(BOOL bFlag);					// 정수만 입력하는 모드인가 ?
	void SetPositiveMode(BOOL bFlag);				// 양수만 입력하는 모드인가 ?
	void SetMode(BOOL bPositive, BOOL bInteger);	// 양수만 입력여부, 정수만 입력 여부 일괄 지정

	CString GetValue();									// 입력된 문자열 데이터를 가져간다.
	void GetValue(CString* sValue);					// 문자열 데이터를 가져간다.
	void GetValue(int* nValue);						// 정수형 데이터를 가져간다.
	void GetValue(double* dbValue);					// 실수형 데이터를 가져간다.
};
