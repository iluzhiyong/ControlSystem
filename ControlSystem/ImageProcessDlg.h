#pragma once
#include <IDevice.h>

// CImageProcessDlg dialog

class CImageProcessDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageProcessDlg)

public:
	CImageProcessDlg(IDevice *pDevice, CWnd* pParent = NULL);
	virtual ~CImageProcessDlg();

	void UpdateDevice(IDevice *pDevice);
	void UpdateControls();
	BOOL ResetDevice(IDevice *pDevice, BOOL bInit = FALSE);
	void SetGainCtrl(UCHAR RGain, UCHAR GGain, UCHAR BGain);
	void SetGammaCtrl(FLOAT fGamma);
	void SetContrastCtrl(SHORT shContrast);
	void SetSaturationCtrl(BYTE nSaturation);
	void SetSpeedTuneCtrl(float fSpeedTune);

// Dialog Data
	enum { IDD = IDD_IMAGEPROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

private:
	IDevice			*m_pDevice;

	DECLARE_MESSAGE_MAP()

	CSliderCtrl m_ctrlBGain;
	CSliderCtrl m_ctrlContrast;
	CSliderCtrl m_ctrlGamma;
	CSliderCtrl m_ctrlGGain;
	CSliderCtrl m_ctrlRGain;
	CSliderCtrl m_ctrlSaturation;
	CSliderCtrl m_ctrlFrameTune;

public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedChkSaturation();
	afx_msg void OnCbnSelchangeCmbFrameSpeed();
	afx_msg void OnBnClickedHflip();
	afx_msg void OnBnClickedVflip();
	afx_msg void OnBnClickedMono();
	afx_msg void OnBnClickedInverse();
	afx_msg void OnBnClickedOnewb2();
};
