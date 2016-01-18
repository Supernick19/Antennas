// TagStructs.h : Declaration of the CTagStructs

#pragma once

// code generated on January-15-16, 12:46 PM

class CTagStructs : public CRecordset
{
public:
	CTagStructs(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTagStructs)

// Field/Param Data

// The string types below (if present) reflect the actual data type of the
// database field - CStringA for ANSI datatypes and CStringW for Unicode
// datatypes. This is to prevent the ODBC driver from performing potentially
// unnecessary conversions.  If you wish, you may change these members to
// CString types and the ODBC driver will perform all necessary conversions.
// (Note: You must use an ODBC driver version that is version 3.5 or greater
// to support both Unicode and these conversions).

	long	m_UniqueID;
	CStringA	m_OwnerOfData;
	CStringA	m_csTagID;
	long	m_iType;
	long	m_iiBatteryLevel;
	CStringA	m_csFirmwareVersion;
	long	m_iBatchNumber;
	long	m_iBeacon;
	long	m_iTemperature;
	CTime	m_ctNextMaintenance;
	long	m_iDeviceKind;
	long	m_iMovementDuration;
	long	m_iPower;
	long	m_iSource;
	long	m_uiActiveHardware;
	long	m_iDataRate;
	BOOL	m_bLightDetected;

// Overrides
	// Wizard generated virtual function overrides
	public:
	virtual CString GetDefaultConnect();	// Default connection string

	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


