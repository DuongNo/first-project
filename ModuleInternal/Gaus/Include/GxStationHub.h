#pragma once

#include "Gaus.h"
#include "GxSingleton.h"
#include "GxStation.h"

class _GAUS_LIB CGxStationHub : public CGxSingleton<CGxStationHub>	
{
private:
//	static CGxStationHub*	theStationHub;			// SingleTone 객체

	CGxStationPtrArray		m_arrStations;			// Station 보관 Array
	CGxMapStringToStation	m_mapStations;			// 이름에 의한 검색을 위한 Map


public:
	CGxStationHub(void);
	~CGxStationHub(void);

//	static CGxStationHub* GetStationHub();			// Station Hub 조회
//	static void ReleaseStationHub(void);			// SingleTone, Station Hub 객체 파괴

	int	GetCount(void);
	void	AddStation(CGxStation* pStation);			// Station을 추가한다.
	void	RemoveAllStation(void);						// 모든 Station 삭제 (종료시)

	CGxStation* GetStation(CString sName);		// 이름으로 Station을 검색하여 포인터를 획득한다.
	CGxStation* GetStation(int nIdx);				// Index로 Station을 조회한다.
	void SetBasePath(CString sPath);					// 모든 Station들에게 기본 경로를 변경시켜준다.


	// 등록된 Station들을 제어한다.
	BOOL Startup(void);									// 등록된 모든 Station들을 기동 시킨다.
	void Shutdown(void);									// 등록된 모든 Station들을 종료처리한다.

	BOOL InitProfile(void);
	BOOL LoadProfile(void);
	BOOL SaveProfile(void);


	// 각종 Event를 지정해준다.
	void SetBroadcastAuto(void);						// AUTO 상태가 되도록 모든 Slave에게 통보한다.
	void SetBroadcastStop(void);						// STOP 상태가 되도록 모든 Slave에게 통보한다.
	void SetBroadcastManual(void);					// MANUAL 상태가 되도록 모든 Slave에게 통보한다.
	void SetBroadcastAbort(void);						// ABORT 상태가 되도록 모든 Slave에게 통보한다.
	void SetBroadcastReset(void);						// ABORT/IDLE 상태에서 사용이 가능하도록 Slave에게 통보한다.


//	void SetApplyReset(void);							// 등록된 모든 Station들의 명령수신 Apply Event를 초기화 해준다.
};

