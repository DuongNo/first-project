#pragma once
class CRecipeTray
{
public:
	CRecipeTray(void);
	~CRecipeTray(void);

	int m_nCountX;				// Xĭ ����
	int m_nCountY;				// Yĭ ����
	double m_dPitchX;			// Xĭ ����
	double m_dPitchY;			// Yĭ ����
	double m_dPitchOffsetX;	// �ܰ����� X ù° ĭ �߾� ������ �Ÿ�
	double m_dPitchOffsetY;	// �ܰ����� Y ù° ĭ �߾� ������ �Ÿ�
	double m_dTrayThickness;	// ������ �� Tray �β�

	void ReadFile(CString strPath);
	void SaveFile(CString strPath, CString strBackupPath);
};

