//#pragma once
//
//#include "HQDLAPIUSER.h"
//#include "DLCCamera.h"
//
//
//// CCameraParaDlg dialog
//
//class CCameraParaDlg : public CDialog
//{
//	DECLARE_DYNAMIC(CCameraParaDlg)
//
//public:
//	void SetCamera(Camera *pCamera) {m_pCamera = pCamera;};
//
//public:
//	CCameraParaDlg(CWnd* pParent = NULL);   // standard constructor
//	virtual ~CCameraParaDlg();
//
//// Dialog Data
//	enum { IDD = IDD_CAMERA_DIALOG };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//
//	DECLARE_MESSAGE_MAP()
//public:
//	BOOL m_bBW;
//	BOOL m_bClearPreview;
//	BOOL m_bRawDataShow;
//	BOOL m_bUpDown;
//	CComboBox m_comboxColor;
//	CComboBox m_comboSize;
//	CSliderCtrl m_sliderColor;
//
//private:
//	int				m_nROff, m_nGOff, m_nBOff;
//	Camera			*m_pCamera;
//
//public:
//	virtual BOOL OnInitDialog();
//	afx_msg void OnBnClickedCheckPlay();
//	virtual BOOL DestroyWindow();
//	afx_msg void OnCbnSelchangeComboSize();
//	afx_msg void OnCbnSelchangeComboColor();
//	afx_msg void OnNMCustomdrawSliderColor(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnReleasedcaptureSliderColor(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnClickedButtonCap();
//	afx_msg void OnBnClickedCheckRawdata();
//	afx_msg void OnBnClickedCheckBw();
//	afx_msg void OnBnClickedCheckUpdown();
//	afx_msg void OnBnClickedCheckClear();
//	afx_msg void OnBnClickedButtonAwb();
//	afx_msg void OnBnClickedButtonAe();
//};
