#include "StdAfx.h"
#include "AxisParam.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAxisParam::CAxisParam(void)
{
	Init();
}


CAxisParam::~CAxisParam(void)
{
}

void CAxisParam::Init()
{
 //	m_strAxisName.Empty();
	m_bUse = TRUE;
	m_DeviceType = DEVICE_TYPE_NMC;
	m_nModuleNo = 0;
	m_nInnerNo = 0;
	m_OriginType = ORIGIN_TYPE_HOME;
	m_MotorType = MOTOR_TYPE_SERVO;
	m_dJogSpeedHigh = 0.;
	m_dJogSpeedMiddle = 0.;
	m_dJogSpeedLow = 0.;
	m_dHomeSpeed = 0.;
	m_dDefaultMoveSpeed = 0.;
	m_dPositiveLimit = 0.;
	m_dNegativeLimit = 0.;
	m_dGearRatio = 0.;
}

void CAxisParam::ReadFile( CString strFilePath )
{
	// 축 이름이 지정 안되 있으면 return
	if(m_strAxisName.GetLength() < 1)
		return;


	if(CFileSupport::FileCheck(strFilePath) == FALSE)
	{
		// 파일이 없을 경우 예외처리. 일단 return처리 하지는 않는다
	}

	CIni ini(strFilePath);
	// 기존 값은 초기화 한 후 불러온다
	Init();

	m_bUse				= ini.GetBoolean(m_strAxisName, _T("m_bUse"), m_bUse);
	// 아래 세개는 날라가면 위험하니 일단 File 입출력은 주석
	m_DeviceType		= (DEVICE_TYPE)ini.GetLong(m_strAxisName, _T("m_DeviceType"), m_DeviceType);
	m_nModuleNo			= ini.GetLong(m_strAxisName, _T("m_nModuleNo"), m_nModuleNo);
	m_nInnerNo			= ini.GetLong(m_strAxisName, _T("m_nInnerNo"), m_nInnerNo);
	m_OriginType		= (ORIGIN_TYPE)ini.GetLong(m_strAxisName, _T("m_OriginType"), (long)m_OriginType);
	m_MotorType			= (MOTOR_TYPE)ini.GetLong(m_strAxisName, _T("m_MotorType"), (long)m_MotorType);
	m_dJogSpeedHigh		= ini.GetDouble(m_strAxisName, _T("m_dJogSpeedHigh"), m_dJogSpeedHigh);
	m_dJogSpeedMiddle	= ini.GetDouble(m_strAxisName, _T("m_dJogSpeedMiddle"), m_dJogSpeedMiddle);
	m_dJogSpeedLow		= ini.GetDouble(m_strAxisName, _T("m_dJogSpeedLow"), m_dJogSpeedLow);
	m_dHomeSpeed			= ini.GetDouble(m_strAxisName, _T("m_dHomeSpeed"), m_dHomeSpeed);
	m_dDefaultMoveSpeed = ini.GetDouble(m_strAxisName, _T("m_dDefaultMoveSpeed"), m_dDefaultMoveSpeed);
	m_dPositiveLimit			= ini.GetDouble(m_strAxisName, _T("m_dPositiveLimit"), m_dPositiveLimit);
	m_dNegativeLimit		= ini.GetDouble(m_strAxisName, _T("m_dNegativeLimit"), m_dNegativeLimit);
	m_dGearRatio			= ini.GetDouble(m_strAxisName, _T("m_dGearRatio"), m_dGearRatio);
}

void CAxisParam::SaveFile( CString strFilePath )
{
	CIni ini(strFilePath);

	ini.WriteBoolean(m_strAxisName, _T("m_bUse"), m_bUse);
	// 아래 세개는 날라가면 위험하니 일단 File 입출력은 주석
	ini.WriteLong(m_strAxisName, _T("m_DeviceType"),				(long)m_DeviceType		);
	ini.WriteLong(m_strAxisName, _T("m_nModuleNo"),					(long)m_nModuleNo	);
	ini.WriteLong(m_strAxisName, _T("m_nInnerNo"),					(long)m_nInnerNo		);
	ini.WriteLong(m_strAxisName, _T("m_OriginType"),				(long)m_OriginType		);
	ini.WriteLong(m_strAxisName, _T("m_MotorType"),					(long)m_MotorType		);
	ini.WriteDouble(m_strAxisName, _T("m_dJogSpeedHigh"),			m_dJogSpeedHigh		);
	ini.WriteDouble(m_strAxisName, _T("m_dJogSpeedMiddle"),		m_dJogSpeedMiddle	);
	ini.WriteDouble(m_strAxisName, _T("m_dJogSpeedLow"),			m_dJogSpeedLow		);
	ini.WriteDouble(m_strAxisName, _T("m_dHomeSpeed"),			m_dHomeSpeed			);
	ini.WriteDouble(m_strAxisName, _T("m_dDefaultMoveSpeed "),	m_dDefaultMoveSpeed );
	ini.WriteDouble(m_strAxisName, _T("m_dPositiveLimit"),			m_dPositiveLimit			);
	ini.WriteDouble(m_strAxisName, _T("m_dNegativeLimit"),			m_dNegativeLimit		);
	ini.WriteDouble(m_strAxisName, _T("m_dGearRatio"),				m_dGearRatio			);
}

void CAxisParam::SaveECIDFile( CString strFilePath )
{
	CIni ini(strFilePath);
	CString StrBuf;
	StrBuf = m_strAxisName;
	m_strAxisName.Format(_T("%s_CMDTYPE"),m_strAxisName);
	ini.WriteLong(m_strAxisName, _T("CMDTYPE"),					(long)m_DeviceType	);
	m_strAxisName = StrBuf;
	m_strAxisName.Format(_T("%s_CMDINDEX"),m_strAxisName);
	ini.WriteLong(m_strAxisName, _T("CMDINDEX"),				(long)m_nModuleNo	);
	m_strAxisName = StrBuf;
	m_strAxisName.Format(_T("%s_AXISNO"),m_strAxisName);
	ini.WriteLong(m_strAxisName, _T("AXISNO"),					(long)m_nInnerNo	);
	m_strAxisName = StrBuf;
	m_strAxisName.Format(_T("%s_RUNSPEED"),m_strAxisName);
	ini.WriteDouble(m_strAxisName, _T("RUNSPEED"),				m_dDefaultMoveSpeed );
	m_strAxisName = StrBuf;

}
