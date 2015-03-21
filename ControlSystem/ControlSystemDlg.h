
// ControlSystemDlg.h : header file
//

#pragma once

// CControlSystemDlg dialog
class CControlSystemDlg : public CDialogEx
{
public:
	// Local iconic variables
  //HObject  ho_Rim, ho_Edges, ho_Holes, ho_Hole;

  // Local control variables
  //HTuple  hv_Width, hv_Height, hv_WindowID, hv_Row;
  //HTuple  hv_Column, hv_Radius, hv_Number, hv_i, hv_DistanceMin;
  //HTuple  hv_DistanceMax, hv_SVal, hv_Ascent, hv_Descent;
  //HTuple  hv_Width1, hv_Height1;


// Construction
public:
	CControlSystemDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CONTROLSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListData;
	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedSaveAs();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCameraParam();
	afx_msg void OnBnClickedButtonImageProc();
};