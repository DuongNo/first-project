#pragma once
class CRecipeTray
{
public:
	CRecipeTray(void);
	~CRecipeTray(void);

	int m_nCountX;				// X칸 개수
	int m_nCountY;				// Y칸 개수
	double m_dPitchX;			// X칸 간격
	double m_dPitchY;			// Y칸 간격
	double m_dPitchOffsetX;	// 외각에서 X 첫째 칸 중앙 까지의 거리
	double m_dPitchOffsetY;	// 외각에서 Y 첫째 칸 중앙 까지의 거리
	double m_dTrayThickness;	// 겹쳤을 시 Tray 두께

	void ReadFile(CString strPath);
	void SaveFile(CString strPath, CString strBackupPath);
};

