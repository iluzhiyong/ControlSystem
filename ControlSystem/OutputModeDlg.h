#pragma once
#include <IDevice.h>

// COutputModeDlg dialog

class COutputModeDlg : public CDialog
{
	DECLARE_DYNAMIC(COutputModeDlg)

public:
	COutputModeDlg(IDevice *pDevice, CWnd* pParent = NULL);   // standard constructor
	virtual ~COutputModeDlg();

	void UpdateDevice(IDevice *pDevice);
	void UpdateControls();
	void SetCaptureModeCtrl(emDeviceCaptureMode mode);
	void SetDataTypeCtrl(emDeviceDataType dataType);
	BOOL ResetDevice(IDevice *pDevice, BOOL bInit = FALSE);
	

// Dialog Data
	enum { IDD = IDD_OUTPUTMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnSoftTrigger();
	afx_msg void OnCbnSelchangeCmbOutputmode();
	afx_msg void OnCbnSelchangeCmbDatatype();
	afx_msg void OnEnChangeEdtSoftTriggerframenum();
	afx_msg void OnEnKillfocusEdtSoftTriggerframenum();

private:
	IDevice			*m_pDevice;
	CSpinButtonCtrl m_spinTriggerFrameNum;
};
