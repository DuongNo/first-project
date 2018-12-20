
#pragma once

struct IGxIDReader;

class CGxIDReaderWrapper
{
public:
	CGxIDReaderWrapper(void);
	virtual ~CGxIDReaderWrapper(void);

	inline bool IsSucceeded() { return m_isSucceeded; }

    void SetEventHandle(HWND hOwner, int nConnect, int nResult, int nLive);
    void SetAddress(CString sAddr);                  // 접속할 IP 주소 대입
    CString GetAddress();                            // 설정된 주소 조회

    void SetLiveImageName(CString sName);            // 라이브 이미지 저장 이름
    void SaveImage(CString sName);                   // 현재 결과 이미지를 저장한다.


    void Connect();
	void DisConnect();
	void Trigger();
	BOOL IsConnect();


    void LiveOn();
    void LiveOff();
    BOOL IsLiveOn();

    int GetReadResult();                // 리딩 결과 조회
    CString GetReadString();             // 결과 데이터 조회
    void SetFailMessage(CString sMsg);   // 실패시 표기되는 메세지 지정



	void ShowVersion();
//	int Add(int a, int b);
//	void ShowForm();
//	void NewFuncCall(CString sName);

private:
	bool m_isSucceeded;
	IGxIDReader *mp_CallClass;
};

/*

        void SetAddress(string sAddr);                  // 접속할 IP 주소 대입
        string GetAddress();                            // 설정된 주소 조회

        void SetLiveImageName(string sName);            // 라이브 이미지 저장 이름
        void SaveImage(string sName);                   // 현재 결과 이미지를 저장한다.

        void SetLiveFileName(string sFileName);
        string GetLiveFileName();

  
        bool IsConnect();
        void Connect();
        void DisConnect();
        void Trigger();

        void LiveOn();
        void LiveOff();
        bool IsLiveOn();

        void ShowVersion();

		*/