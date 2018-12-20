#pragma once

#include "Gaus.h"
#include "GxSingleton.h"
#include "GxService.h"

class _GAUS_LIB CGxServiceHub : public CGxSingleton<CGxServiceHub>	
{
private:
//	static CGxServiceHub*	theServiceHub;			// SingleTone 객체

	CGxServicePtrArray		m_arrServices;			// Service 보관 Array
	CGxMapStringToService	m_mapServices;			// 이름에 의한 검색을 위한 Map


public:
	CGxServiceHub(void);
	~CGxServiceHub(void);

	//static CGxServiceHub* GetServiceHub();			// Service Hub 조회
	//static void ReleaseServiceHub(void);				// SingleTone, Station Hub 객체 파괴

	int	GetCount(void);
	void AddService(CGxService* pService);				// Service을 추가한다.
	void RemoveAllService(void);						// Add로 통해 등록된 모든 서비스를 삭제해준다.

	CGxService* GetService(CString sName);				// 이름으로 Service을 검색하여 포인터를 획득한다.
	CGxService* GetService(int nIdx);					// Index로 Service을 조회한다.
	void SetBasePath(CString sPath);					// 모든 Service들 기본 경로를 변경시켜준다.


	// 등록된 Service들을 제어한다.
	BOOL Startup(void);									// 등록된 모든 Service들을 기동 시킨다.
	void Shutdown(void);								// 등록된 모든 Service들을 종료처리한다.

	BOOL InitProfile(void);
	BOOL LoadProfile(void);
	BOOL SaveProfile(void);

};

