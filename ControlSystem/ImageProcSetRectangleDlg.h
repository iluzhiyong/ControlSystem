#pragma once
#include "DetectRectangle.h"

// CImageProcSetRectangleDlg dialog

class CImageProcSetRectangleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageProcSetRectangleDlg)

public:
	CImageProcSetRectangleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageProcSetRectangleDlg();

	void SetDetecter(CDetectRectangle* detecter);
	void UpdateDetecterSetting();

// Dialog Data
	enum { IDD = IDD_IMAGE_PROC_SETTING_RECTANGLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_threMin;
	int m_threMax;
	float m_rectangularityMin;
	float m_rectangularityMax;
	float m_areaMin;
	float m_areaMax;
	afx_msg void OnBnClickedBtnRectThreTest();
	afx_msg void OnBnClickedBtnRectShapeTest();
	afx_msg void OnBnClickedBtnRectApply();
	afx_msg void OnBnClickedBtnRectTest();

private:
	CDetectRectangle*	m_detecter;
public:
	int m_minRow;
	int m_maxRow;
	int m_minColumn;
	int m_maxColumn;
	float m_maxRA;
	float m_minRA;
	float m_maxRB;
	float m_minRB;
};
