#pragma once


// CFormTeachNGTray 폼 뷰입니다.

class CFormTeachNGTray : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachNGTray)

protected:
public:
	CFormTeachNGTray();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeachNGTray();

	enum { IDD = IDD_FORM_TEACH_NG_TRAY };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	DECLARE_EVENTSINK_MAP()
	BOOL m_bShow;
	BOOL m_bInit;
	AXIS_ID			m_CurAxis;
	TEACH_PARAM::ID	m_CurTeach;
	UINT			m_CurID;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateNGTrayINState();
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);

	void ClickGxbtnNgtrayConvIn();
	void ClickGxbtnNgtrayConvOut();
	void ClickGxbtnNgtrayConvStop();
	void ClickGxbtnNgtrayConvCylup();
	void ClickGxbtnNgtrayBufferconvIn();
	void ClickGxbtnNgtrayBufferconvOut();
	void ClickGxbtnNgtrayBufferconvStop();
	void ClickGxbtnNgtrayConvCyldown();
	void ClickGxbtnNgtrayinLoadPos();
	void ClickGxbtnNgtrayinUpMove();
	void ClickGxbtnNgtrayinPitchupMove();
	void ClickGxbtnNgtrayinPitchdownMove();
	void ClickGxbtnNgtrayinLiftBwd();
	void ClickGxbtnNgtrayinLiftFwd();
	void ClickGxbtnNgtrayoutUnloadPos();
	void ClickGxbtnNgtrayoutUpMove();
	void ClickGxbtnNgtrayoutPitchupMove();
	void ClickGxbtnNgtrayoutPitchdownMove();
	void ClickGxbtnNgtrayoutLiftBwd();
	void ClickGxbtnNgtrayoutLiftFwd();
	void ClickGxbtnNgtrayinIoTopFwd();
	void ClickGxbtnNgtrayinIoTopBwd();
	void ClickGxbtnNgtrayinIoBtmFwd();
	void ClickGxbtnNgtrayinIoBtmBwd();
	void ClickGxbtnNgtrayinIoAlignguideFwd();
	void ClickGxbtnNgtrayinIoAlignguideBwd();
	void ClickGxbtnNgtraytrVacon();
	void ClickGxbtnNgtraytrVacoff();
	void ClickGxbtnNgtraytrGetpos();
	void ClickGxbtnNgtraytrPutpos();
	void ClickGxbtnNgtraytrUppos();
	void ClickGxbtnNgtraytrGetDownpos();
	void ClickGxbtnNgtraytrPutDownpos();
	void ClickGxbtnNgtrayinTrayUp();
	void ClickGxbtnNgtrayinTrayDown();
	void ClickGxbtnNgtrayoutTrayUp();
	void ClickGxbtnNgtrayoutTrayDown();
	void ClickGxbtnNgbuftrayConvCylup();
	void ClickGxbtnNgbuftrayConvCyldown();
	void ClickGxbtnNgtrayAlignStart();
	void ClickGxbtnNgtrayAlignTeaching();
	void ClickGxbtnNgtrayinMidPos();
};


