#pragma once
#include "DetectOblong.h"

// CImageProcSetOblongDlg dialog

class CImageProcSetOblongDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageProcSetOblongDlg)

public:
	CImageProcSetOblongDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageProcSetOblongDlg();

	void SetDetecter(CDetectOblong* detecter);
	void UpdateDetecterSetting();

// Dialog Data
	enum { IDD = IDD_IMAGE_PROC_SETTING_OBLONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_threMin;
	int m_threMax;
	float m_roundnessMin;
	float m_roundnessMax;
	afx_msg void OnBnClickedBtnOblongThreTest();
	afx_msg void OnBnClickedBtnOblongShapeTest();
	afx_msg void OnBnClickedBtnOblongApply();
	afx_msg void OnBnClickedBtnOblongTest();
	float m_areaMin;
	float m_areaMax;

private:
	CDetectOblong*	m_detecter;
};
