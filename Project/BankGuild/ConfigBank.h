#pragma once

#include "ConfigSystem.h"
#include "ConfigOption.h"
#include "UnLoadRuleInfo.h"

//////////////////////////////////////////////////////////////////////////
// Recipe�� ���ӵ��� �ʴ� �������� �����ϴ� Bank
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

