
// Cheetah.h : Cheetah 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include "Thread/Thread.h"
#include "BankGuild/LogUnit.h"
#include "BankGuild/AxisParamBank.h"
#include "BankGuild/CellBank.h"
#include "BankGuild/LossTimeBank.h"
#include "BankGuild/IOParamBank.h"

#include "BankGuild/FDCBank.h"

//2018-03-07,SKCHO, 추가
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
// 이 클래스의 구현에 대해서는 Cheetah.cpp을 참조하십시오.
//
class CCheetahApp : public CWinApp
{
public:
	CCheetahApp();

protected:
	
	// GDI+ 用
	ULONG_PTR				gdiplusToken;
	GdiplusStartupInput		gdiplusstartupinput;
	// end of GDI+

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void InitializeLog();
	CString ReadVersion();

	SYSTEMTIME tmStart;


// 구현입니다.
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
extern CUnitCtrlFunc		theUnitFunc;		// 티칭창 등 Main Process에서 호출 위해 만듦
extern CDeviceMotion		theDeviceMotion;
extern CDeviceIO			theDeviceIO;
extern cDeviceMelsec		theDeviceMelsec;



//kjpark 20160912 socket test
extern CSocketDialog			theSocketInterFace;
extern CSerialManager			theSerialInterFace;

extern CProductBank				theProductBank;
