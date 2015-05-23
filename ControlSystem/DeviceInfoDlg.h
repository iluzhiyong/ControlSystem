#pragma once
#include <IDevice.h>

// CDeviceInfoDlg dialog

class CDeviceInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CDeviceInfoDlg)

public:
	CDeviceInfoDlg(IDevice *pDevice, CWnd* pParent = NULL);
	virtual ~CDeviceInfoDlg();

	void UpdateDevice(IDevice *pDevice);

// Dialog Data
	enum { IDD = IDD_DEVICEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	BOOL ResetDevice(IDevice *pDevice, BOOL bInit = FALSE);

	afx_msg void OnBnClickedBtnWriteSN();

	DECLARE_MESSAGE_MAP()

private:
	IDevice			*m_pDevice;
};
