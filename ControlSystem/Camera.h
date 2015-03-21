#pragma once

#include "HQDLAPIUSER.h"

#define MAXWIDTH     1280
#define MAXHEIGHT    1024

typedef struct _tagCamFeature
{
	BOOL    bBW;
	BOOL    bRawDataShow;
	BOOL    bClearView;
	BOOL    bPause;
	BOOL    bUpDown;
	BOOL    bCaptureRaw;
}CAMFEATURE, *PCAMFEATURE;


// CCamera dialog

class CCamera : public CDialog
{
	DECLARE_DYNAMIC(CCamera)

public:
	VOID SetCamFeature();
	void FinishAE( DWORD dwResult );
	void FinishAWB( DWORD dwResult );

public:
	CCamera(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCamera();

// Dialog Data
	enum { IDD = IDD_CAMERA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bBW;
	BOOL m_bClearPreview;
	BOOL m_bPause;
	BOOL m_bPlay;
	BOOL m_bRawDataShow;
	BOOL m_bUpDown;
	CComboBox m_comboxColor;
	CComboBox m_comboSize;
	CSliderCtrl m_sliderColor;
	CString m_strMessage;
	CStatic m_staticVideo;

private:
	BYTE             *m_pRawData;
	BYTE             *m_pRgbData;
	CapInfoStruct    m_CapInfo;
	VOID      Play( BOOL bPlay );

	int       m_nROff, m_nGOff, m_nBOff;
	HANDLE    m_hDevice;
	CAMFEATURE       m_CamFeature;

	USHORT    m_Out;
	BYTE      m_In;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckPlay();
	virtual BOOL DestroyWindow();
	afx_msg void OnCbnSelchangeComboSize();
	afx_msg void OnCbnSelchangeComboColor();
	afx_msg void OnNMCustomdrawSliderColor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSliderColor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCap();
	afx_msg void OnBnClickedCheckRawdata();
	afx_msg void OnBnClickedCheckBw();
	afx_msg void OnBnClickedCheckUpdown();
	afx_msg void OnBnClickedCheckClear();
	afx_msg void OnBnClickedButtonAwb();
	afx_msg void OnBnClickedButtonAe();
	afx_msg void OnBnClickedCheckPause();
};
