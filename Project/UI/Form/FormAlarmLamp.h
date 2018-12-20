#pragma once
#include "resource.h"

//kjpark 20161022 ����->���� ���� ����
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"
 


// CFormConfig1 �� ���Դϴ�.

class CFormAlarmLamp : public CFormView
{
	DECLARE_DYNCREATE(CFormAlarmLamp)

public:
	CFormAlarmLamp();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormAlarmLamp();

public:
	enum { IDD = IDD_FORM_CONFIG_LAMP };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	//kjpark 20161022 ����->���� ���� ����
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	//afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	//kjpark 20161022 ����->���� ���� ����
	map<UINT, BOOL> iMapLamp;	

	//kjpark 20161022 ����->���� ���� ����
	void Init();
	void InitLamp();
	void InitList();
	void SetDisplay(map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator iter);
	void setLamp();
	void SaveAlarmFile();
	void ReadAlarmLabpFile();
	void SaveFile( CString strFilePath);
	void ReadFile( CString strFilePath );

	//kjpark 20161022 ����->���� ���� ����
	void OnButtonClick_INIT_Red();
	void OnButtonClick_INIT_Yellow();
	void OnButtonClick_INIT_Green();

	void OnButtonClick_IDLE_Red();
	void OnButtonClick_IDLE_Yellow();
	void OnButtonClick_IDLE_Green();

	void OnButtonClick_STOP_Red();
	void OnButtonClick_STOP_Yellow();
	void OnButtonClick_STOP_Green();

	void OnButtonClick_RUN_Red();
	void OnButtonClick_RUN_Yellow();
	void OnButtonClick_RUN_Green();

	void OnButtonClick_ALARM_Red();
	void OnButtonClick_ARARM_Yellow();
	void OnButtonClick_ARARM_Green();

	void OnButtonClick_PM_Red();
	void OnButtonClick_PM_Yellow();
	void OnButtonClick_PM_Green();

	void OnButtonClick_OPCALL_Red();
	void OnButtonClick_OPCALL_Yellow();
	void OnButtonClick_OPCALL_Green();

	//kjpark 20161022 ����->���� ���� ����
	void OnButtonClick_Title_Init();
	void OnButtonClick_Title_Idle();
	void OnButtonClick_Title_Stop();
	void OnButtonClick_Title_Run();
	void OnButtonClick_Title_Ararm();
	void OnButtonClick_Title_Pm();
	void OnButtonClick_Title_OPCall();

	//kjpark 20161022 ����->���� ���� ����
	void OnButtonSave();
	void ClickGxlblAlarm4TitleAgvRed();
	void ClickGxlblAlarm4TitleAgvYellow();
	void ClickGxlblAlarm4TitleAgvGreen();
	void ClickGxbtnAlarmLampTitleAgv();
	void ClickGxbtnAlarmLampTitleCheck();
	void ClickGxlblAlarm4TitleCheckRed();
	void ClickGxlblAlarm4TitleCheckYellow();
	void ClickGxlblAlarm4TitleCheckGreen();
};



