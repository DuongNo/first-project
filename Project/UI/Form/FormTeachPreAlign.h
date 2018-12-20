#pragma once



// CFormTeachPreAlign �� ���Դϴ�.

class CFormTeachPreAlign : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachPreAlign)

protected:
public:
	CFormTeachPreAlign();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormTeachPreAlign();

	enum { IDD = IDD_FORM_TEACH_PREALIGN };
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

	BOOL m_bInit;
	AXIS_ID			m_CurAxis;
	TEACH_PARAM::ID	m_CurTeach;
	UINT			m_CurID;

	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdatePAState();
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);

public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnLdPrealigntrYGet();
	void ClickGxbtnLdPrealigntrYPut();
	void ClickGxbtnLdPrealigntrYWait();
	void ClickGxbtnLdPrealigntrZGetshuttle1();
	void ClickGxbtnLdPrealigntrZGetshuttle2();
	void ClickGxbtnLdPrealigntrZPut();
	void ClickGxbtnLdPrealigntrZUp();
	void ClickGxbtnLdPrealignTrVacCh1On();
	void ClickGxbtnLdPrealignTrVacCh1Off();
	void ClickGxbtnLdPrealignTrVacCh2On();
	void ClickGxbtnLdPrealignTrVacCh2Off();
	void ClickGxbtnPreAlignXCh1();
	void ClickGxbtnPreAlignXCh2();
	void ClickGxbtnPreAlignYCh1();
	void ClickGxbtnPreAlignYCh2();
	void ClickGxbtnPreAlignTCh1();
	void ClickGxbtnPreAlignTCh2();
	void ClickGxbtnPreStgLoadPos();
	void ClickGxbtnPreStgUnloadPos();
	void ClickGxbtnPreStgCh1VacOn();
	void ClickGxbtnPreStgCh2VacOn();
	void ClickGxbtnPreStgCh1VacOff();
	void ClickGxbtnPreStgCh2VacOff();
	void ClickGxbtnPreAutoTeach();
	void SeqContactAutoTeaching();

	BYTE m_bRtnComp[MAX_RTN_SIZE];	
	BYTE m_bRtn[MAX_RTN_SIZE];
	void ResetReturnValue();	// Rtn�������� ���� TRUE�� Set�Ѵ�.
	BOOL IsReturnOk();			// Rtn�� RtnComp�� Compair�Ͽ� OK/NG�� �Ǵ��Ѵ�.

	enum 
	{
		seqC_idle = -1,
		seqC_AMTVacOn,
		seqC_Zup,
		seqC_Ready,
		seqC_TableEntry,
		seqC_LDTRZDown,
		seqC_TableVacOff,
		seqC_LDTRZUp,
		seqC_LDTRXGet,
		seqC_LDTRZGet,
		seqC_LDTRVacOff,
		seqC_LDTRZUp2,
		seqC_PASTGLoadPos,
		seqC_MarkSave,
		seqC_MarkSaveChk,
		seqC_LDTRZGet2,
		seqC_PASTGVacOff,
		seqC_LDTRZUp3,
		seqC_LDTRXPut,
		seqC_LDTRZPut,
		seqC_LDTRVacOff2,
		seqC_LDTRZUp4,
		seqC_CalcNextStep,
		seqC_End
	};

	JIG_ID m_Jig;
	CEdit m_edtMsg;
	int m_nSeq;
	CStopWatch m_Timer;
	int m_nTeachingTablePos;
	void ClickGxbtnPreStgAlignPos();
	void ClickGxbtnPreAlignManuualCh1();
	void ClickGxbtnPreAlignManuualCh2();
};


