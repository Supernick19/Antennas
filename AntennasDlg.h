// AntennasDlg.h : header file
//

#pragma once
#include "SPIRF01_DLL.h"
#include <stdlib.h>
#include <exception>
#include "afxwin.h"
#include <map>

#define UPDATE_MSG WM_USER+WM_APP+1
#define LOG 0
#define REFRESH 1
#define MAX_DEVICE 4 //This must match the number of device in settings.xml

// AntennasDlg dialog
class AntennasDlg : public CDialogEx
{
private:
	//Variables
	struct DeviceStruct
	{
		BOOL bCheck;
		BOOL bConnected;
		int  iHandle;
		explicit DeviceStruct():bCheck(FALSE), bConnected(FALSE), iHandle(0){};
	};
	std::map<int, DeviceStruct> m_lstDevices;

	CString m_edtLogger;
	CStatic m_lblFWInfo;

	//Functions
	void connect(int iDevice);
	void disconnect(int iDevice);
	CString getFirmware(int iDevice);
	static void _stdcall callback_antenna(int iHandle, const UINT8* pMsg, int iSize, PVOID pParam);
	static void _stdcall callback_programmer(int iHandle, const UINT8* pMsg, int iSize, PVOID pParam);
	void __stdcall client_function(int iHandle, const unsigned __int8 *pMsg, int iSize, void *pParam);

protected:
	//Variables
	HICON m_hIcon;

	//Functions
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUpdatelog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	//Constructors
	AntennasDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~AntennasDlg();

	//Functions
	afx_msg void OnBnClickedConnect(UINT nID);
	afx_msg void OnBnClickedConnectAll();
	afx_msg void OnBnClickedConnectCheck();
	afx_msg void OnBnClickedDisconnect(UINT nID);
	afx_msg void OnBnClickedDisconnectAll();
	afx_msg void OnBnClickedDisconnectCheck();
	afx_msg void OnBnClickedAntennaGetFW(UINT nID);
	afx_msg void OnBnClickedProgFWInfos();
	afx_msg void OnBnClickedListen();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedStopListen();
	afx_msg void OnBnClickedStopListenningAll();
	afx_msg void OnBnClickedTest();           ////////////////////////TEST//////////////////////
	afx_msg void OnDestroy();
	void setTagDetected(TagStruct stTag);

	enum { IDD = IDD_TEMP_DIALOG };
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedListenAll();
	afx_msg void OnBnClickedBtnConnectSingle();
	afx_msg void OnBnClickedDisconnectProgrammerSingle();
	BOOL newRecord(TagStruct TagData);
};

class RFIDConnectionException : public std::exception
{
private:
	CString m_csErrorMsg;
public :
	RFIDConnectionException();
	RFIDConnectionException(CString csErrorMsg):exception((char*)&(csErrorMsg.GetString())[0])
		, m_csErrorMsg(csErrorMsg){}

	virtual ~RFIDConnectionException() throw(){};
	const CString getErrorMsg() const throw()
	{
		return CString(m_csErrorMsg);
	}
	void reportError()
	{
		AfxMessageBox(getErrorMsg(), MB_OK|MB_ICONERROR);
	}
};
