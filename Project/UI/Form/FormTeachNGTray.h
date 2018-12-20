#pragma once


// CFormTeachNGTray �� ���Դϴ�.

class CFormTeachNGTray : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachNGTray)

protected:
public:
	CFormTeachNGTray();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormTeachNGTray();

	enum { IDD = IDD_FORM_TEACH_NG_TRAY };
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


