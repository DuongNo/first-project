#pragma once

#include "resource.h"

// CDlgbarJog 대화 상자입니다.

typedef struct
{
	AXIS_ID nID;
	BOOL	bHorOrVer;			//	bHorOrVer가 True : X축이동, False : Z축 이동
	BOOL	bForward;			//	bForward 라면 오른쪽과 위쪽이 +
}Axis_Info;

enum DIREC
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

#define CHANGE_MOTOR_AXIS 5001

class CDlgbarJog : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgbarJog)

public:
	CDlgbarJog();   // 표준 생성자입니다.
	virtual ~CDlgbarJog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_BAR_JOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// afx_msg void OnBtnClickMenu1();
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeViewEnd(WPARAM wParam, LPARAM lParam);				// 메인 View 변환 완료 수신
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnChangeAxis(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:	
	void ClickGxbtnBarjogSjump();
	void ClickBtnApplyPos();
	void ClickBtnSave();
	void ClickBtnMove();
	void ClickBtnInputPos();

	void MouseDownJogUP(short Button, short Shift, long x, long y);
	void MouseUpJogUP(short Button, short Shift, long x, long y);
	void MouseDownJogDown(short Button, short Shift, long x, long y);
	void MouseUpJogDown(short Button, short Shift, long x, long y);
	void MouseDownJogRight(short Button, short Shift, long x, long y);
	void MouseUpJogRight(short Button, short Shift, long x, long y);
	void MouseDownJogLeft(short Button, short Shift, long x, long y);
	void MouseUpJogLeft(short Button, short Shift, long x, long y);

	
protected:
	void UpdateUI();
	void MovePositionByJog(DIREC Direc);
	

protected:
	double m_dX;
	double m_dY;

	BOOL   m_bMove;
	Axis_Info	m_AData[AXIS_ID_MAX];
	AXIS_ID		m_curAxis;
public:

	void ClickGxbtnBarjogLow();
	void ClickGxbtnBarjogMid();
	void ClickGxbtnBarjogHigh();

	int nSpeed;

	BOOL DoorOpenChk() ;
};
