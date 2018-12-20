
// Cheetah.h : Cheetah ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "Thread/Thread.h"
#include "BankGuild/LogUnit.h"
#include "BankGuild/AxisParamBank.h"
#include "BankGuild/CellBank.h"
#include "BankGuild/LossTimeBank.h"
#include "BankGuild/IOParamBank.h"

#include "BankGuild/FDCBank.h"

//2018-03-07,SKCHO, �߰�
#include "BankGuild/MonitorBank.h"

#include "BankGuild/ConfigBank.h"
#include "BankGuild/ProcDataBank.h"
#include "BankGuild/RecipeBank.h"
#include "BankGuild/TeachBank.h"
#include "BankGuild/UnitCtrlBank.h"
#include "BankGuild/ProductBank.h"
#include "Device/DeviceMotion.h"
#include "Device/DeviceIO.h"
#include "Device/DeviceMelsec.h"
#include "Device/SocketDialog.h"
#include "Device/SerialManager.h"
#include "UI/Form/FormRecipe.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

// CCheetahApp:
// �� Ŭ������ ������ ���ؼ��� Cheetah.cpp�� �����Ͻʽÿ�.
//
class CCheetahApp : public CWinApp
{
public:
	CCheetahApp();

protected:
	
	// GDI+ ��
	ULONG_PTR				gdiplusToken;
	GdiplusStartupInput		gdiplusstartupinput;
	// end of GDI+

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void InitializeLog();
	CString ReadVersion();

	SYSTEMTIME tmStart;


// �����Դϴ�.
	DECLARE_MESSAGE_MAP()
};

extern CCheetahApp			theApp;
extern CThreadInitializer	theThreadInitializer;
extern CLogUnit				theLog[LOG_MAX];
extern CAxisParamBank	theAxisBank;
extern CCellBank			theCellBank;
extern CIOParamBank		theIOBank;

//20170119 byskcho
extern CLossTimeBank        theLossTimeBank;
//20161227 byskcho
extern CFDCBank         theFDCBank;

//2018-03-07,SKCHO
extern CMonitorBank    theMonitorBank;

extern CConfigBank			theConfigBank;
extern CProcDataBank		theProcBank;
extern CRecipeBank			theRecipeBank;
extern CTeachBank			theTeachBank;
extern CUnitCtrlFunc		theUnitFunc;		// ƼĪâ �� Main Process���� ȣ�� ���� ����
extern CDeviceMotion		theDeviceMotion;
extern CDeviceIO			theDeviceIO;
extern cDeviceMelsec		theDeviceMelsec;



//kjpark 20160912 socket test
extern CSocketDialog			theSocketInterFace;
extern CSerialManager			theSerialInterFace;

extern CProductBank				theProductBank;
