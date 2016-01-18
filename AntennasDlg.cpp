// AntennasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Antennas.h"
#include "AntennasDlg.h"
#include "afxdialogex.h"
#include "TagStructs.h"
#include <fstream>
#include <iostream>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <memory>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ADDRESS _T("192.168.1.84") //this is where the IP address goes
#define PORT 5000

// AntennasDlg dialog

int getPort(int i)
{
	i++;
	int iPort = -1;
	std::fstream file;
	using boost::property_tree::ptree;
	ptree pt;

	try
	{
		file.open(_T("settings.xml"), std::ios::in);
		read_xml(file, pt);
		char buff[25];
		_snprintf_s(buff, 25, _TRUNCATE, "Configurations.Port%d", i); 
		iPort = pt.get<int>(buff);
	}
	catch (...)
	{
		if (file.is_open())
			file.close();
	}
	file.close();
	return iPort;
}

//Device internal ID
#define DEVICE1 0
#define DEVICE2 1
#define DEVICE3 2
#define PROGRAMMER 3

AntennasDlg::AntennasDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(AntennasDlg::IDD, pParent)
	, m_edtLogger(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

AntennasDlg::~AntennasDlg()
{
}
//???????
void AntennasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_lstDevices[DEVICE1].bCheck);
	DDX_Check(pDX, IDC_CHECK2, m_lstDevices[DEVICE2].bCheck);
	DDX_Check(pDX, IDC_CHECK3, m_lstDevices[DEVICE3].bCheck);
	DDX_Control(pDX, IDC_LBL_FWINFO, m_lblFWInfo);
	DDX_Text(pDX, IDC_LOG, m_edtLogger);
}

BEGIN_MESSAGE_MAP(AntennasDlg, CDialogEx)
	ON_COMMAND_RANGE(BTN_CONNECT_1, BTN_CONNECT_1 + MAX_DEVICE - 1, &AntennasDlg::OnBnClickedConnect)
	ON_COMMAND_RANGE(BTN_DISCONNECT_1, BTN_DISCONNECT_1 + MAX_DEVICE - 1, &AntennasDlg::OnBnClickedDisconnect)
	ON_COMMAND_RANGE(BTN_GETFW1, BTN_GETFW3, &AntennasDlg::OnBnClickedAntennaGetFW)

	ON_BN_CLICKED(BTN_CONNECT_ALL, &AntennasDlg::OnBnClickedConnectAll)
	ON_BN_CLICKED(BTN_DISCONNECT_ALL, &AntennasDlg::OnBnClickedDisconnectAll)
	ON_MESSAGE(UPDATE_MSG, &AntennasDlg::OnUpdatelog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_PROGRAMMER_FIRMWARE, &AntennasDlg::OnBnClickedProgFWInfos)
	ON_BN_CLICKED(BTN_TEST, &AntennasDlg::OnBnClickedTest)
	ON_BN_CLICKED(BTN_CONNECT_CHECK, &AntennasDlg::OnBnClickedConnectCheck)
	ON_BN_CLICKED(BTN_DISCONNECT_CHECK, &AntennasDlg::OnBnClickedDisconnectCheck)
	ON_BN_CLICKED(BTN_LISTEN, &AntennasDlg::OnBnClickedListen)
	ON_BN_CLICKED(BTN_STOP_LISTEN, &AntennasDlg::OnBnClickedStopListen)
	ON_BN_CLICKED(BTN_STOP_LISTENNING_ALL, &AntennasDlg::OnBnClickedStopListenningAll)
	ON_BN_CLICKED(BTN_LISTEN_ALL, &AntennasDlg::OnBnClickedListenAll)
	ON_BN_CLICKED(BTN_CONNECT_PROGRAMMER_SINGLE, &AntennasDlg::OnBnClickedBtnConnectSingle)
	ON_BN_CLICKED(BTN_DISCONNECT_PROGRAMMER_SINGLE, &AntennasDlg::OnBnClickedDisconnectProgrammerSingle)
END_MESSAGE_MAP()


// AntennasDlg message handlers

BOOL AntennasDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString temp = _T("Antennas Checker - ");
	int iSize = temp.GetLength();
	MW_GetVersionString(&(temp.GetBufferSetLength(255))[iSize], 255-iSize);
	SetWindowText(temp);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void AntennasDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR AntennasDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void AntennasDlg::OnBnClickedConnectAll()
{
	for (int i = 0; i < MAX_DEVICE; i++)
		connect(i);
}


void AntennasDlg::OnBnClickedDisconnectAll()
{
	MW_CloseAll();
}

void AntennasDlg::setTagDetected(TagStruct stTag)
{
	if (stTag.iDeviceKind == DEVICE_KIND_ANTENNA)
	{
		CString* msg = new CString();

		
		msg->Format(_T("%s, FW v%s\r\n"), DEVICE_KIND_STRING[stTag.iDeviceKind], stTag.csFirmwareVersion);
		
		if (PostMessage(UPDATE_MSG, LOG, (LPARAM)msg) == FALSE)
			delete msg;
	}
	else
	{
		CString* msg = new CString();
		CString csTime = _T("");
		if (stTag.ctNextMaintenance > 0)
			csTime.Format(_T(", Date : %s"), stTag.ctNextMaintenance.Format(_T("%d/%m/%Y")));
		if ( (stTag.iBatteryLevel != -1) && (((DWORD)stTag.iBatteryLevel) > 256) )
			msg->Format(_T("Tag %s detected on : %d (0x%02X%02X), Handle : 0x%08X%s\r\n"), stTag.csTagID, stTag.iSource, (stTag.iSource >> 8)&0xFF, stTag.iSource&0xFF, stTag.iBatteryLevel, csTime);
		else
			msg->Format(_T("Tag %s detected on : %d (0x%02X%02X), Handle : %d%s\r\n"), stTag.csTagID, stTag.iSource, (stTag.iSource >> 8)&0xFF, stTag.iSource&0xFF, stTag.iBatteryLevel, csTime);
		if (PostMessage(UPDATE_MSG, LOG, (LPARAM)msg) == FALSE)
			delete msg;
	}
}

/**************************************************************************************/
/* SHARED_PTR EQUIVALENT FOR OUR USES (ANY C++ VERSION)      						  */
/* Implemented for test in case the technique has to be used elsewhere                */
/**************************************************************************************/
//template<typename Type>
//struct StackDeleter
//{
//	Type* pPointer;
//	StackDeleter(Type* pToDeleteLater):pPointer(pToDeleteLater){};
//	~StackDeleter(){delete pPointer; pPointer = NULL;};
//};//<< This emulate the basic function of shared_ptr...
//afx_msg LRESULT AntennasDlg::OnUpdatelog(WPARAM wParam, LPARAM lParam)
//{
//	switch (wParam)
//	{
//	case LOG : 
//		{
//			//Since msg is on stack, it will be deleted at the end of the function no
//			//matter what happens. Since the destuctor delete lParam, it WILL be deleted.
//			StackDeleter<CString> msg((CString*)lParam);
//			CString csTemp = m_edtLogger;
//			m_edtLogger.GetLength() > 710 ? m_edtLogger = *msg.pPointer : m_edtLogger = *msg.pPointer + csTemp;
//			SetDlgItemText(IDC_LOG, m_edtLogger);
//			break;
//		}
//	case REFRESH : SetDlgItemText(IDC_LOG, m_edtLogger); break;
//	default: UpdateData(false);
//	}
//	return 0;
//}
/**************************************************************************************/

/**************************************************************************************/
/* USING SHARED_PTR (C++11 ONLY, include <memory>)                                    */
/* (WE DON'T USE ANY FANCY PROPERTIES, SO IT'S BASICLY THE SAME AS ABOVE)             */
/**************************************************************************************/
afx_msg LRESULT AntennasDlg::OnUpdatelog(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case LOG : 
		{
			//Since msg is on stack, it will be deleted at the end of the function no
			//matter what happen. Since it is the only "owner", it will then delete
			//lParam
			std::shared_ptr<CString> msg((CString*)lParam);
			CString csTemp = m_edtLogger;
			m_edtLogger.GetLength() > 710 ? m_edtLogger = *msg.get() : m_edtLogger = *msg.get() + csTemp;
			SetDlgItemText(IDC_LOG, m_edtLogger);
			break;
		}
	case REFRESH : SetDlgItemText(IDC_LOG, m_edtLogger); break;
	default: UpdateData(false);
	}

	return 0;
}
/**************************************************************************************/

void AntennasDlg::OnDestroy()
{
	__super::OnDestroy();
}

void AntennasDlg::OnBnClickedConnect(UINT nID)
{
	connect(nID - BTN_CONNECT_1);
}

void AntennasDlg::connect(int iDevice)
{
	//Override config to test ethernet
	if (iDevice == DEVICE2)
	{
		/*m_lstDevices[iDevice].bConnected =*/ m_lstDevices[iDevice].bConnected = MW_OpenEx(ADDRESS, &m_lstDevices[iDevice].iHandle, PORT) == MW_OK;
		if (m_lstDevices[iDevice].bConnected)
			MW_RegisterCallback(&AntennasDlg::callback_antenna, this, m_lstDevices[iDevice].iHandle);
	}
	else
	{
		m_lstDevices[iDevice].bConnected = MW_Open(getPort(iDevice), &m_lstDevices[iDevice].iHandle) == MW_OK;
		if (m_lstDevices[iDevice].bConnected)
		{
			if (iDevice != PROGRAMMER)
				MW_RegisterCallback(&AntennasDlg::callback_antenna, this, m_lstDevices[iDevice].iHandle);
			else
				MW_RegisterCallback(&AntennasDlg::callback_programmer, this, m_lstDevices[iDevice].iHandle);
		}
	}
}

void AntennasDlg::OnBnClickedDisconnect(UINT nID)
{
	disconnect(nID - BTN_DISCONNECT_1);
}

void AntennasDlg::disconnect(int iDevice)
{
	MW_Close(m_lstDevices[iDevice].iHandle);
	MW_UnregisterCallback(m_lstDevices[iDevice].iHandle);
	m_lstDevices[iDevice].bConnected = FALSE;
}

void AntennasDlg::OnBnClickedAntennaGetFW(UINT nID)
{
	AfxMessageBox(getFirmware(nID - BTN_GETFW1));
}

CString hextostr(const unsigned __int8* buffer)
{

	char values[17] = "0123456789ABCDEF";
	char datas[9];
	datas[7] = values[buffer[3] & 0x0F];
	datas[6] = values[buffer[3] >> 4];
	datas[5] = values[buffer[2] & 0x0F];
	datas[4] = values[buffer[2] >> 4];
	datas[3] = values[buffer[1] & 0x0F];
	datas[2] = values[buffer[1] >> 4];
	datas[1] = values[buffer[0] & 0x0F];
	datas[0] = values[buffer[0] >> 4];
	datas[8] = '\0';
	return CString(datas);
}
void AntennasDlg::callback_antenna(int iHandle, const UINT8* pMsg, int iSize, PVOID pParam)
{
	//Tag beacon or tag beacon inside a wrapped message
	TagStruct tag;
	if ( (pMsg[1] == 1) || ((pMsg[1] == 9) && (pMsg[MSG_SIZE_TAG_WRAPPER] == 1)) )
	{
		
		MW_ExtractDatas(&tag, pMsg);
		tag.iBatteryLevel = iHandle;
		((AntennasDlg*)pParam)->setTagDetected(tag);
	}
	else if (pMsg[1] == 3)
	{
		//TagStruct tag;
		MW_ExtractDatas(&tag, pMsg);
		tag.iBatteryLevel = iHandle;
		((AntennasDlg*)pParam)->setTagDetected(tag);
	}
	std::cout << tag.csTagID;
}

void AntennasDlg::callback_programmer(int iHandle, const UINT8* pMsg, int iSize, PVOID pParam)
{
	if (pMsg[1] == 3)
	{

		TagStruct tag;
		MW_ExtractDatas(&tag, pMsg);
		tag.iBatteryLevel = iHandle;
		tag.iSource = 2;
		((AntennasDlg*)pParam)->setTagDetected(tag);
	}

}

void AntennasDlg::OnBnClickedProgFWInfos()
{
	TCHAR buff[25];
	if (MW_SelectDevice(m_lstDevices[PROGRAMMER].iHandle) == MW_OK)
	{
		if (MW_GetFirmwareInfo(buff, 25) == MW_OK)
			m_lblFWInfo.SetWindowText(buff);
		else
			m_lblFWInfo.SetWindowText(_T("Connection closed"));
	}
	else
		m_lblFWInfo.SetWindowText(_T("Not Connected"));
}


void AntennasDlg::OnBnClickedTest()
{
	//Test function
	if (MW_SelectDevice(m_lstDevices[DEVICE2].iHandle) == MW_OK)
	{
		static bool temp = false;
		temp = !temp;
		CString csMsgA = _T("Failed to set address");
		CString csMsgB = _T("Failed to fetch address");
		UINT16 address = temp ? 0xAABB : 0x1244;
		UINT16 adreturn;

		int iResult = MW_SetReaderAddress(address, 1);
		if (iResult == MW_OK)
			csMsgA.Format(_T("Successfully set address to 0x%04X"), address);

		iResult = MW_GetReaderAddress(&adreturn, 1);
		if (iResult == MW_OK)
			csMsgB.Format(_T("Fetched address: 0x%04X"), adreturn);

		CString csMsg;
		csMsg.Format(_T("%s\r\n%s"), csMsgA, csMsgB);
		AfxMessageBox(csMsg);
	}
	else
		m_lblFWInfo.SetWindowText(_T("Not Connected"));
}


void AntennasDlg::OnBnClickedConnectCheck()
{
	UpdateData();
	for (int i = 0; i < MAX_DEVICE; i++)
	{
		if (m_lstDevices[i].bCheck)
		{
			if (i != DEVICE2)
				m_lstDevices[i].bConnected = MW_Open(getPort(i), &m_lstDevices[i].iHandle) == MW_OK;
			else
				m_lstDevices[i].bConnected = MW_OpenEx(ADDRESS, &m_lstDevices[i].iHandle, PORT) == MW_OK;
		}
	}
}


void AntennasDlg::OnBnClickedDisconnectCheck()
{
	UpdateData();
	for (int i = 0; i < MAX_DEVICE; i++)
	{
		if (m_lstDevices[i].bCheck)
			m_lstDevices[i].bConnected = MW_Close(m_lstDevices[i].iHandle) == MW_OK;
	}
}


void AntennasDlg::OnBnClickedListen()
{
	UpdateData();
	for (int i = 0; i < MAX_DEVICE; i++)
	{
		if (m_lstDevices[i].bCheck)
		{
			if (i != PROGRAMMER)
				MW_RegisterCallback(&AntennasDlg::callback_antenna, this, m_lstDevices[i].iHandle);
			else
				MW_RegisterCallback(&AntennasDlg::callback_programmer, this, m_lstDevices[i].iHandle);
		}
	}
}


void AntennasDlg::OnBnClickedStopListen()
{
	UpdateData();
	for (int i = 0; i < MAX_DEVICE; i++)
	{
		if (m_lstDevices[i].bCheck)
			MW_UnregisterCallback(m_lstDevices[i].iHandle);
	}
}

//Test broadcast
void AntennasDlg::OnBnClickedStopListenningAll()
{
	MW_UnregisterCallback(BROADCAST);
}

//Test broadcast
void AntennasDlg::OnBnClickedListenAll()
{
	MW_RegisterCallback(&AntennasDlg::callback_antenna, this, BROADCAST);
	//Overwrite function reference set in previous line for programmer
	MW_RegisterCallback(&AntennasDlg::callback_programmer, this, m_lstDevices[PROGRAMMER].iHandle);
}


void AntennasDlg::OnBnClickedBtnConnectSingle()
{
	if ( MW_OpenSingle(getPort(PROGRAMMER)) != MW_OK )
		AfxMessageBox(_T("Port already open"));
}

CString AntennasDlg::getFirmware(int iDevice)
{
	CString csResult;
	TCHAR buff[25];
	if (MW_SelectDevice(m_lstDevices[iDevice].iHandle) == MW_OK)
	{
		if (MW_GetFirmwareInfo(buff, 25) == MW_OK)
			csResult = buff;
		else
			csResult = _T("Failed");
	}
	else
		csResult = _T("Not Connected");
	return csResult;
}

void AntennasDlg::OnBnClickedDisconnectProgrammerSingle()
{
	MW_CloseSingle();
}
BOOL newRecord(TagStruct TagData){
	CTagStructs cts;
	std::cout << "function has been called";
	if (!cts.Open())
		return FALSE;
	if (!cts.CanAppend())
		return FALSE;                      // no field values were set
	cts.AddNew();
	cts.m_bLightDetected = TagData.bLightDetected;
	cts.m_csFirmwareVersion = TagData.csFirmwareVersion;
	cts.m_csTagID = TagData.csTagID;
	cts.m_ctNextMaintenance = TagData.ctNextMaintenance;
	cts.m_iBatchNumber = TagData.iBatchNumber;
	cts.m_iBeacon = TagData.iBeacon;
	cts.m_iDataRate = TagData.iDataRate;
	cts.m_iDeviceKind = TagData.iDeviceKind;
	cts.m_iiBatteryLevel = TagData.iBatteryLevel;
	cts.m_iMovementDuration = TagData.iMovementDuration;
	cts.m_iPower = TagData.iPower;
	cts.m_iSource = TagData.iSource;
	cts.m_iTemperature = TagData.iTemperature;
	cts.m_iType = TagData.iType;
	if (!cts.Update())
	{
		//AfxMessageBox("Record not added; no field values were set.");
		return FALSE;
	}
	return true;
}
void __stdcall client_function(int iHandle, const unsigned __int8 *pMsg, int iSize, void *pParam){
	// pParam is the object passed in parameter when MW_RegisterCallback was called    
	// Optionnal: extract frame datas to a TagStruct structure    
	TagStruct tagData;
	MW_ExtractDatas(&tagData, pMsg);
	std::cout << "function called";
	newRecord(tagData);
}

