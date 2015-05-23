#pragma once
#include <IDevice.h>

// CExposeDlg dialog

class CExposeDlg : public CDialog
{
	DECLARE_DYNAMIC(CExposeDlg)

public:
	CExposeDlg(IDevice *pDevice, CWnd* pParent = NULL);
	virtual ~CExposeDlg();

	void UpdateDevice(IDevice *pDevice);
	void UpdateControls();
	CString GetPicPath(emDSFileType &type);
	BOOL ResetDevice(IDevice *pDevice, BOOL bInit = FALSE);
	void SetExposeTimeCtrl(USHORT usExposureTime);
	void SetAnalogGainCtrl(UCHAR uGain);
	void SetAETarget(BYTE nAETarget);
	void EnableUpdateControls(BOOL bShow = TRUE);

// Dialog Data
	enum { IDD = IDD_EXPOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedChkAE();
	afx_msg void OnBnClickedBtnPicpath();
	afx_msg void OnBnClickedBtnOutputImage(UINT nID);
	afx_msg void OnBnClickedBtnSaveType(UINT nID);

private:
	IDevice			*m_pDevice;
	CSliderCtrl m_ctrlAETarget;
	CSliderCtrl m_ctrlGain;
	CSliderCtrl m_ctrlExposeTime;
};
