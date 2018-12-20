#pragma once

#include "Serial\CommThread.h"
#include <vector>
#include "DeviceLoadCell.h"
#include "DeviceSerialGMS.h"
//kjpark 20161010 라벨프린터 PG PC 에서 사용
#ifdef OPERATOR_PC
#include "DeviceSerialPrinter.h"
#endif
#include "DeviceSerialServoTorque.h"
#include "DeviceSerialTemperature.h"
#include "DeviceSerialVACUUM.h"


using namespace std;

//20161227 byskcoh
#define PORT_MAX 11
//end

struct SERIAL_MODE_SET
{
	SERIAL_MODE SerialMode;
	int			nPort;
	CString		m_sPortNickName;
};


static SERIAL_MODE_SET SerialModeSet[] = {
	{SERIAL_GMS						, 1, _T("SERIAL_GMS")},
//kjpark 20161010 라벨프린터 PG PC 에서 사용
#ifdef OPERATOR_PC
	{SERIAL_LABELPRINT				, 2, _T("SERIAL_LABELPRINT")},
#endif
	{SERIAL_TEMPERATURE				, 11, _T("SERIAL_TEMPERATURE")},
	{SERIAL_VACUUM_LD				, 15, _T("SERIAL_VACUUM_LD")},
	{SERIAL_VACUUM_INSP				, 18, _T("SERIAL_VACUUM_INSP")},
	{SERIAL_VACUUM_UD				, 14, _T("SERIAL_VACUUM_UD")}	
};

class CSerialManager
{
public:
	CSerialManager(void);
	~CSerialManager(void);

	std::vector<CCommThread> m_vctNPortReal;
	std::vector<std::vector<unsigned char>> m_vctStrData; 

	DWORD	SerialSendData(SERIAL_MODE nPort, BYTE *pBuff, DWORD nToWrite); 
	int		SerialGetDataBuffer(SERIAL_MODE nPort, BYTE *cstr, int nMaxSize);


	//20161227 byskcoh
	void SetSerialInfo(SERIAL_MODE nPort,UINT nPortNum);
	//end

	BOOL SerialOpen(SERIAL_MODE nPort = SERIAL_GMS);
	BOOL SerialClose(SERIAL_MODE nPort);
	BOOL SerialCodeSend(int nCode, int nPort);
	BOOL SerialAckCheck(SERIAL_MODE nPort);
		
	//20160930 kjpark Serial IF
	CDeviceSerialGMS* GetDeviceGMSHandler() {return &m_pDeviceGMS;};
//kjpark 20161010 라벨프린터 PG PC 에서 사용	
#ifdef OPERATOR_PC
	CDeviceSerialPrinter* GetLabelPrinterHandler() {return m_pDeviceLabelPrinter;};
#endif
	CDeviceSerialTemperature* GetTemperatureHandler() {return &m_pDeviceTemperature;};
	CDeviceSerialVACUUM* GetVACUUMLDHandler() {return &m_DeviceVACUUMLD;};
	CDeviceSerialVACUUM* GetVACUUMINSPHandler() {return &m_DeviceVACUUMINSP;};
	CDeviceSerialVACUUM* GetVACUUMUDHandler() {return &m_DeviceVACUUMUD;};
	
	void SendCommand(void);

	//20160930 kjpark Serial IF
	void PacketReceive(SERIAL_MODE serialMode);

private:	
	CDeviceSerialGMS  m_pDeviceGMS;
	CDeviceSerialVACUUM m_DeviceVACUUMLD;
	CDeviceSerialVACUUM m_DeviceVACUUMINSP;
	CDeviceSerialVACUUM m_DeviceVACUUMUD;

	CDeviceSerialTemperature m_pDeviceTemperature;

	// VACUUM [12/11/2016 OSC]
public:
	void SendSetVacRange(int nType);

private:
	BOOL m_bFirst;
public:
	void SendVacReadUD(int nSlot);
	void SendVacReadINSP(int nSlot);
	void SendVacReadLD(int nSlot);
	void SendTempRead(int nAddr);
	BOOL ParsingUDVACUUMValue();
	BOOL ParsingINSPVACUUMValue();
	BOOL ParsingLDVACUUMValue();
	BOOL ParsingTemperatureValue();
	void CopyAirValue_to_FDCBank(SERIAL_MODE nMode);
	void CopyTempValue_to_FDCBank(int nAddr);
	int GetDeviceAddr();

};
