#pragma once
#include <IDevice.h>

// CROIDlg dialog

class CROIDlg : public CDialog
{
	DECLARE_DYNAMIC(CROIDlg)

public:
	CROIDlg(IDevice *pDevice, CWnd* pParent = NULL);
	virtual ~CROIDlg();

	void UpdateDevice(IDevice *pDevice);
	void UpdateControls();
	BOOL ResetDevice(IDevice *pDevice, BOOL bInit = FALSE);
	void EnablePresetResolutionCtrls(BOOL bEnable);
	void UpdateCurrentInfo(int offsetX, int offsetY, int iWidth, int iHeight);


// Dialog Data
	enum { IDD = IDD_ROI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_ctrlHeight;
	CSliderCtrl m_ctrlOffseX;
	CSliderCtrl m_ctrlOffseY;
	CSliderCtrl m_ctrlWidth;

private:
	IDevice					*m_pDevice;
	LPDeviceResolutionItem	m_lpDevRes;
	int						m_nResSize;
	int						m_nCulIndex;

	DeviceCapability		m_devCapability;
public:
	afx_msg void OnBnClickedBtnMaximize();
	afx_msg void OnBnClickedBtnSetroi();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedRadResolution(UINT nID);
	afx_msg void OnCbnSelchangeResolutionlist();
	afx_msg void OnEnKillfocusEdit(UINT nID);
	virtual BOOL OnInitDialog();
};
