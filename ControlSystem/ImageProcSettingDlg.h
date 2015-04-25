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
	int m_MinGray;
	int m_MaxGray;
	afx_msg void OnBnClickedThresholdBtn();

public:
	void SetCircleDetecter(CDetectCircularhole* detecter);
	void ConvertSettingToDetecter();

private:
	CDetectCircularhole* m_CirleDetecter;
public:
	float m_MinCirleArea;
	float m_MaxCirleArea;
	float m_MinRoundness;
	float m_MaxRoundness;
	float m_DilationRadius;
	CString m_EdgeFilter;
	float m_EdgeAlpha;
	int m_EdgeMinThreld;
	int m_EdgeMaxThreld;
	afx_msg void OnSelchangeCombo3();
	CComboBox m_DilationRadiusComboBox;
	afx_msg void OnSelchangeCombo4();
	CComboBox m_EdgeAlphaComboBox;
	afx_msg void OnBnClickedSelectCircleBtn();
	afx_msg void OnBnClickedDialCircleBtn();
	afx_msg void OnBnClickedDetEdgesBtn();
	afx_msg void OnBnClickedDetectBtn();
	afx_msg void OnBnClickedApplyButton();
};
