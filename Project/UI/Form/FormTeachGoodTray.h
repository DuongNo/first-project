#pragma once


// CFormTeachGoodTray �� ���Դϴ�.

class CFormTeachGoodTray : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachGoodTray)

protected:
public:
	CFormTeachGoodTray();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormTeachGoodTray();

	enum { IDD = IDD_FORM_TEACH_GOOD_TRAY };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.

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



