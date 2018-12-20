#pragma once


// DlgLossTime 대화 상자입니다.

class DlgLossTime : public CDialog
{
	DECLARE_DYNAMIC(DlgLossTime)

public:
	DlgLossTime(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgLossTime();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_LOSSTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnLosstimeFirst();
	void ClickGxbtnLosstimeSecond();
	void ClickGxbtnLosstimeThird();
	void ClickGxbtnLosstimeSelect();
};
