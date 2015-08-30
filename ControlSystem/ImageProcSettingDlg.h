#pragma once
#include "DetectCircularhole.h"
#include "afxwin.h"

// CImageProcSettingDlg dialog

class CImageProcSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageProcSettingDlg)

public:
	CImageProcSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageProcSettingDlg();

// Dialog Data
	enum { IDD = IDD_IMAG_PROC_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void SetCircleDetecter(CDetectCircularhole* detecter);
	void UpdateDetecterSetting();

private:
	CDetectCircularhole* m_CirleDetecter;
public:
	int m_MinGray;
	int m_MaxGray;
	float m_MinCirleArea;
	float m_MaxCirleArea;
	float m_MinRoundness;
	float m_MaxRoundness;

	afx_msg void OnBnClickedThresholdBtn();
	afx_msg void OnBnClickedSelectCircleBtn();
	afx_msg void OnBnClickedDetectBtn();
	afx_msg void OnBnClickedApplyButton();
};
