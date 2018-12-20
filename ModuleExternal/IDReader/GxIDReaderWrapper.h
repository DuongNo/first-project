
#pragma once

struct IGxIDReader;

class CGxIDReaderWrapper
{
public:
	CGxIDReaderWrapper(void);
	virtual ~CGxIDReaderWrapper(void);

	inline bool IsSucceeded() { return m_isSucceeded; }

    void SetEventHandle(HWND hOwner, int nConnect, int nResult, int nLive);
    void SetAddress(CString sAddr);                  // ������ IP �ּ� ����
    CString GetAddress();                            // ������ �ּ� ��ȸ

    void SetLiveImageName(CString sName);            // ���̺� �̹��� ���� �̸�
    void SaveImage(CString sName);                   // ���� ��� �̹����� �����Ѵ�.


    void Connect();
	void DisConnect();
	void Trigger();
	BOOL IsConnect();


    void LiveOn();
    void LiveOff();
    BOOL IsLiveOn();

    int GetReadResult();                // ���� ��� ��ȸ
    CString GetReadString();             // ��� ������ ��ȸ
    void SetFailMessage(CString sMsg);   // ���н� ǥ��Ǵ� �޼��� ����



	void ShowVersion();
//	int Add(int a, int b);
//	void ShowForm();
//	void NewFuncCall(CString sName);

private:
	bool m_isSucceeded;
	IGxIDReader *mp_CallClass;
};

/*

        void SetAddress(string sAddr);                  // ������ IP �ּ� ����
        string GetAddress();                            // ������ �ּ� ��ȸ

        void SetLiveImageName(string sName);            // ���̺� �̹��� ���� �̸�
        void SaveImage(string sName);                   // ���� ��� �̹����� �����Ѵ�.

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