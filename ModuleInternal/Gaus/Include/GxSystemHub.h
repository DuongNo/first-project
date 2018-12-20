#pragma once

#include "Gaus.h"
#include "GxSingleton.h"
#include "GxSystem.h"

class _GAUS_LIB CGxSystemHub : public CGxSingleton<CGxSystemHub>	
{
private:
//	static CGxSystemHub*	theSystemHub;			// SingleTone 객체

	CGxSystemPtrArray		m_arrSystems;			// System 보관 Array
	CGxMapStringToSystem	m_mapSystems;			// 이름에 의한 검색을 위한 Map

public:
	CGxSystemHub();
	~CGxSystemHub();

	//static CGxSystemHub* GetSystemHub();			// System Hub 조회
	//static void ReleaseSystemHub(void);				// SingleTone, System Hub 객체 파괴

	int	GetCount();
	void AddSystem(CGxSystem* pSystem);				// System을 추가한다.
	void RemoveAllSystem();						// Add로 통해 등록된 모든 시스템을 삭제해준다.


	CGxSystem* GetSystem(CString sName);			// 이름으로 System을 검색하여 포인터를 획득한다.
	CGxSystem* GetSystem(int nIdx);					// Index로 System을 조회한다.
	void SetBasePath(CString sPath);				// 모든 System들 기본 경로를 변경시켜준다.


	// 등록된 System들을 제어한다.
	BOOL Startup();								// 등록된 모든 System들을 기동 시킨다.
	void Shutdown();							// 등록된 모든 System들을 종료처리한다.


	BOOL InitProfile();
	BOOL LoadProfile();
	BOOL SaveProfile();
};

