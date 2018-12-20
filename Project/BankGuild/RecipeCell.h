#pragma once
class CRecipeCell
{
public:
	CRecipeCell(void);
	~CRecipeCell(void);

	double m_dWidth;
	double m_dHeight;
	BOOL   m_bFPCBuse;
	double m_dMarkDistance;

	void ReadFile(CString strPath);
	void SaveFile(CString strPath, CString strBackupPath);
};

