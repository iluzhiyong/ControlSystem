
// ControlSystemDlg.h : header file
//

#pragma once

#include "MotorCtrl.h"
#include "Camera.h"

// CControlSystemDlg dialog
class IMotorCtrl;
class IImageProcess;
class IHeightDectector;
class CControlSystemDlg : public CDialogEx
{
// Construction
public:
	CControlSystemDlg(CWnd* pParent = NULL);	// standard constructor
	~CControlSystemDlg();

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

protected:
	CMotorCtrl* m_pMotorCtrl;
	Camera		*m_pCamera;
	afx_msg LRESULT AcquireImage(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnBnClickedStart();
	CStatic m_staticPicture;
	afx_msg void OnBnClickedButton2();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedAutoMear();

public:
	IMotorCtrl* m_IMotoCtrl;
	IImageProcess* m_IImageProcess;
	IHeightDectector* m_IHeightDectector;
	bool CalculatePoint(float x, float y, float z, float &retx, float &rety, float &retz);

public:
	bool m_excelLoaded;
	int m_columnNum;
	int m_rowNum;
	bool GetMeasureTargetValue(int row, float &x, float &y, float &z);
	bool SetMeasureResultValue(int row, float resultX, float resultY, float resultZ);
	bool GetFloatItem(int row, int column, float &value);
	bool SetFloatItem(int row, int column, float value);
};
