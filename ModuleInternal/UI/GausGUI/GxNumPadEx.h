#pragma once

#include "GausGUI.h"

// #include "resource.h"

// CNumPadEx 대화 상자입니다.

class _GAUS_GUI CGxNumPadEx : public CDialog
{
	DECLARE_DYNAMIC(CGxNumPadEx)

public:
	CGxNumPadEx(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGxNumPadEx();

	// enum { IDD = IDD_DLG_NUMPAD2 };		

	enum {
		MAX_LENGTH		= 12,					// 입력가능한 글자수
		LEN_DECIMAL		= 3,					// 기본적인 소숫점 이하 자릿수		
	};				// 최대 글자수

protected:

	CString		m_sTitle;				// 다이얼로그 제목
	CString		m_sEdit;					// 편집중인 글자
	BOOL			m_bIsInteger;			// 정수 입력 모드
	BOOL			m_bIsPositive;			// 양의 수치만 입력 모드
	BOOL			m_bCheckLimit;			// 입력 범위를 제한 할 것인가 ?
	BOOL			m_bCheckEqual;			// 동일한 값인지 점검하여 동일하지 않을때 입력값 유효
	BOOL			m_bValueError;			// 값 입력에 오류가 있었다. 화면 점멸 초기화에 이용

	UINT			m_nDecimalLen;			// 소숫점 길이
	
	double		m_dbOld;					// 이전값
	double		m_dbMin;					// 하한값
	double		m_dbMax;					// 상한값
	// double		m_dbEditValue;			// 현재 입력중인 값

	void DisplayBtnMsg(UINT nID, CString sMsg);			// 현재 편집중인 글자를 표시해준다.
	void DisplayBtnValue(UINT nID, double dbValue, int nDecimal = LEN_DECIMAL);		// Button에 값 표시
	void DisplayLabelMsg(UINT nID, CString sMsg);		// Static에 문자열을 표시한다.
	void DisplayLabelValue(UINT nID, double dbValue, int nDecimal = LEN_DECIMAL);		// Static에 값 표시
	void DisplayLabelBlink(UINT nID, BOOL bFlag);		// Static의 Blink를 활성/비활성 설정한다.
	void DisplayLabelColor(UINT nID, COLORREF cColor);	// Static의 색을 지정한다.

	void ClearBlink();					// 입력에 문제가 있어서 점멸중이던 것을 정상화 시켜준다.
	void UpdateEditValue();				// 현재 입력중인 값을 표시해준다.

	// 글자를 조립한다.
	void InputNumKey(CString sKey);		// 지정 키를 입력한다.

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
	void SetIntegerMode(BOOL bFlag);					// 정수만 입력하는 모드인가 ?
	void SetPositiveMode(BOOL bFlag);				// 양수만 입력하는 모드인가 ?
	void SetDecimal(UINT nLen);							// 실수값을 표현시 소숫점을 명시해준다.
	void SetValue(double dbMin, double dbMax, double dOld, UINT nLen = LEN_DECIMAL, BOOL bCheck = TRUE);	// 점검할 값들을 지정해준다.
	void SetValue(int dbMin, int dbMax, int dbOld, BOOL bCheck = TRUE);				// 점검할 값들을 지정해준다.
	void SetMode(BOOL bPositive, BOOL bInteger);	// 양수만 입력여부, 정수만 입력 여부 일괄 지정

	CString GetValue();									// 입력된 문자열 데이터를 가져간다.
	void GetValue(CString* sValue);					// 문자열 데이터를 가져간다.
	void GetValue(int* nValue);						// 정수형 데이터를 가져간다.
	void GetValue(double* dbValue);					// 실수형 데이터를 가져간다.

};
