#pragma once

#include "UI\GausGUI\GxImage.h"
#include "UI\GausGUI\GxStaticEx.h"


// CFormTeachLoaderTR �� ���Դϴ�.

class CFormTeachLoaderTR : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachLoaderTR)

protected:
public:
	CFormTeachLoaderTR();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormTeachLoaderTR();

	enum { IDD = IDD_FORM_TEACH_LOADER_TR };
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

	afx_msg LRESULT OnUpdateLOWERMCRData(WPARAM wParam, LPARAM lParam);				// MCR Data�� ���ŵǾ���.
	afx_msg LRESULT OnUpdateLOWERMCRLive(WPARAM wParam, LPARAM lParam);				// MCR Live ���� ����
	afx_msg LRESULT OnUpdateUPPERMCRData(WPARAM wParam, LPARAM lParam);				// MCR Data�� ���ŵǾ���.
	afx_msg LRESULT OnUpdateUPPERMCRLive(WPARAM wParam, LPARAM lParam);				// MCR Live ���� ����

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	BOOL m_bLowerShow;
	BOOL m_bUpperShow;
	BOOL m_bInit;
	BOOL m_bLowerLiveMode;
	BOOL m_bUpperLiveMode;
	AXIS_ID			m_CurAxis;
	TEACH_PARAM::ID	m_CurTeach;
	UINT			m_CurID;

	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateLDTRState();
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);

	CGxImage			m_Lower_imgLive;
	CGxImage			m_Upper_imgLive;

	void SetMCRLive();
	void SetMCRLive(MCR_UNIT Unit);
	void SetMCRData(MCR_UNIT Unit,CString strBarcode = _T(""));
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnLowermcrLive();
	void ClickGxbtnUppermcrLive();
	void ClickGxbtnLowermcrRead();
	void ClickGxbtnUppermcrRead();
	void ClickGxbtnLdtrXGet();
	void ClickGxbtnLdtrXPut();
	void ClickGxbtnLdtrXLowermcrCh1();
	void ClickGxbtnLdtrXLowermcrCh2();
	void ClickGxbtnLdtrXUppermcrCh1();
	void ClickGxbtnLdtrXUppermcrCh2();
	void ClickGxbtnLdtrZGet();
	void ClickGxbtnLdtrZPut();
	void ClickGxbtnLdtrZLower();
	void ClickGxbtnLdtrZLowermcrCh1();
	void ClickGxbtnLdtrZLowermcrCh2();
	void ClickGxbtnLdtrZUppermcrCh1();
	void ClickGxbtnLdtrZUppermcrCh2();
	void ClickGxbtnLdtrVacuumCh1On();
	void ClickGxbtnLdtrVacuumCh2On();
	void ClickGxbtnLdtrVacuumCh1Off();
	void ClickGxbtnLdtrVacuumCh2Off();
	void ClickGxbtnLdtrZUpper();
	void ClickGxbtnLdtrZUp();
	void ClickGxbtnLdtrFpcbBlowOn();
	void ClickGxbtnLdtrFpcbBlowOff();
	void ClickGxbtnLowermcrReconnect();
	void ClickGxbtnUppermcrReconnect();
};


