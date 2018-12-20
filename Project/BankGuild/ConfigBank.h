#pragma once

#include "ConfigSystem.h"
#include "ConfigOption.h"
#include "UnLoadRuleInfo.h"

//////////////////////////////////////////////////////////////////////////
// Recipe에 종속되지 않는 설정값들 관리하는 Bank
class CConfigBank
{
public:
	CConfigBank(void);
	~CConfigBank(void);

	CConfigSystem m_System;
	CConfigOption m_Option;
	CUnLoadRuleInfo m_UnloadRunInfo;

	void ReadSystemFile();
	void ReadOptionFile();
	void SaveOptionFile();

	void ReadNGCodeFile();
	BOOL LoadDefectNameTable();

	void ReadUnLoadRunInfo();
	void SaveUnLoadRunInfo();
};

