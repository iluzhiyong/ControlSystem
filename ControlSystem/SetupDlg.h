#pragma once
#include <IDevice.h>

class CImageProcessDlg;
class CExposeDlg;
class CROIDlg;
class COutputModeDlg;
class CDeviceInfoDlg;

class CSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(IDevice *pDevice, CWnd* pParent = NULL);
	virtual ~CSetupDlg();

	void UpdateDevice(IDevice *pDevice);
	CString GetPicPath(emDSFileType &type) const;
	void UpdateControls();
	void ResetDevice(IDevice *pDevice);

// Dialog Data
	enum { IDD = IDD_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
public:
	afx_msg void OnTcnSelchangeTabSetup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDefault();
	afx_msg void OnBnClickedSaveParam();
	afx_msg void OnBnClickedParamTeam(UINT nID);
	DECLARE_MESSAGE_MAP()

private:
	CTabCtrl			m_tabCtrl;
	IDevice				*m_pDevice;
	CImageProcessDlg	*m_pImgProcessDlg;
	CExposeDlg			*m_pExposeDlg;
	CROIDlg				*m_pROIDlg;
	COutputModeDlg		*m_pOutputModeDlg;
	CDeviceInfoDlg		*m_pDeviceInfoDlg;
};
