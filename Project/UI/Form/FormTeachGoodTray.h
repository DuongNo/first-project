#pragma once


// CFormTeachGoodTray 폼 뷰입니다.

class CFormTeachGoodTray : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachGoodTray)

protected:
public:
	CFormTeachGoodTray();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeachGoodTray();

	enum { IDD = IDD_FORM_TEACH_GOOD_TRAY };
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
	void UpdateOKTrayINState();
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);

	void ClickGxbtnTrayinConvIn();
	void ClickGxbtnTrayinConvOut();
	void ClickGxbtnTrayinConvStop();
	void ClickGxbtnTrayinConvCylup();
	void ClickGxbtnTrayinConvCyldown();
	void ClickGxbtnTrayinLoadPos();
	void ClickGxbtnTrayinUpMove();
	void ClickGxbtnTrayinTopMove();//2018wjp
	void ClickGxbtnTrayinPitchupMove();
	void ClickGxbtnTrayinPitchdownMove();
	void ClickGxbtnTrayinIoTopFwd();
	void ClickGxbtnTrayinIoTopBwd();
	void ClickGxbtnTrayinIoBtmFwd();
	void ClickGxbtnTrayinIoBtmBwd();
	void ClickGxbtnTrayoutConvIn();
	void ClickGxbtnTrayoutConvOut();
	void ClickGxbtnTrayoutConvStop();
	void ClickGxbtnTrayoutConvCylup();
	void ClickGxbtnTrayoutConvCyldown();
	void ClickGxbtnTrayoutUnloadPos();
	void ClickGxbtnTrayoutUpMove();
	void ClickGxbtnTrayoutPitchupMove();
	void ClickGxbtnTrayoutPitchdownMove();
	void ClickGxbtnTraytrVacon();
	void ClickGxbtnTraytrVacoff();
	void ClickGxbtnTraytrGetpos();
	void ClickGxbtnTraytrPutpos();
	void ClickGxbtnTraytrUppos();
	void ClickGxbtnTraytrGetDownpos();
	void ClickGxbtnTraytrPutDownpos();
	void ClickGxbtnTrayinIoAlignguideFwd();
	void ClickGxbtnTrayinIoAlignguideBwd();
	void ClickGxbtnTrayoutIoAlignguideFwd();
	void ClickGxbtnTrayoutIoAlignguideBwd();
	void ClickGxbtnTrayinBufferconvIn();
	void ClickGxbtnTrayinBufferconvOut();
	void ClickGxbtnTrayinBufferconvStop();
	void ClickGxbtnTrayoutBufferconvIn();
	void ClickGxbtnTrayoutBufferconvOut();
	void ClickGxbtnTrayoutBufferconvStop();
	void ClickGxbtnTrayinConvCylup2();
	void ClickGxbtnTrayinConvCyldn2();
	void ClickGxbtnTrayoutConvCylup2();
	void ClickGxbtnTrayoutConvCyldown2();
	void ClickGxbtnOktrayAlignStart();
	void ClickGxbtnOktrayAlignTeaching();
	void ClickGxbtnTrayIDReading();
	void ClickGxbtnTrayidReconnect();
	void ClickGxbtnTrayinAlignPos();
	void ClickGxbtnTrayoutAlignPos();
};



