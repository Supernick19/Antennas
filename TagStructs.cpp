// TagStructs.h : Implementation of the CTagStructs class



// CTagStructs implementation

// code generated on January-15-16, 12:46 PM

#include "stdafx.h"
#include "TagStructs.h"
IMPLEMENT_DYNAMIC(CTagStructs, CRecordset)

CTagStructs::CTagStructs(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_UniqueID = 0;
	m_OwnerOfData = "";
	m_csTagID = "";
	m_iType = 0;
	m_iiBatteryLevel = 0;
	m_csFirmwareVersion = "";
	m_iBatchNumber = 0;
	m_iBeacon = 0;
	m_iTemperature = 0;
	m_ctNextMaintenance;
	m_iDeviceKind = 0;
	m_iMovementDuration = 0;
	m_iPower = 0;
	m_iSource = 0;
	m_uiActiveHardware = 0;
	m_iDataRate = 0;
	m_bLightDetected = FALSE;
	m_nFields = 17;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// The connection string below may contain plain text passwords and/or
// other sensitive information. Please remove the #error after reviewing
// the connection string for any security related issues. You may want to
// store the password in some other form or use a different user authentication.
CString CTagStructs::GetDefaultConnect()
{
	return _T("DSN=sitesecureDSN;Description=Main Site-Secure DSN;UID=SiteSecure;PWD=SiteSecure2005;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=DEVLAPTOP2;DATABASE=SiteSecure");
}

CString CTagStructs::GetDefaultSQL()
{
	return _T("[dbo].[TagStructs]");
}

void CTagStructs::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Long(pFX, _T("[UniqueID]"), m_UniqueID);
	RFX_Text(pFX, _T("[OwnerOfData]"), m_OwnerOfData);
	RFX_Text(pFX, _T("[csTagID]"), m_csTagID);
	RFX_Long(pFX, _T("[iType]"), m_iType);
	RFX_Long(pFX, _T("[iiBatteryLevel]"), m_iiBatteryLevel);
	RFX_Text(pFX, _T("[csFirmwareVersion]"), m_csFirmwareVersion);
	RFX_Long(pFX, _T("[iBatchNumber]"), m_iBatchNumber);
	RFX_Long(pFX, _T("[iBeacon]"), m_iBeacon);
	RFX_Long(pFX, _T("[iTemperature]"), m_iTemperature);
	RFX_Date(pFX, _T("[ctNextMaintenance]"), m_ctNextMaintenance);
	RFX_Long(pFX, _T("[iDeviceKind]"), m_iDeviceKind);
	RFX_Long(pFX, _T("[iMovementDuration]"), m_iMovementDuration);
	RFX_Long(pFX, _T("[iPower]"), m_iPower);
	RFX_Long(pFX, _T("[iSource]"), m_iSource);
	RFX_Long(pFX, _T("[uiActiveHardware]"), m_uiActiveHardware);
	RFX_Long(pFX, _T("[iDataRate]"), m_iDataRate);
	RFX_Bool(pFX, _T("[bLightDetected]"), m_bLightDetected);

}
/////////////////////////////////////////////////////////////////////////////
// CTagStructs diagnostics

#ifdef _DEBUG
void CTagStructs::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTagStructs::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


