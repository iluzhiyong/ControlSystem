
// ControlSystemDlg.h : header file
//

#pragma once
#include "HalconAction.h"
#include "MotorCtrl.h"
#include "Camera.h"
#include "ImageProcSettingDlg.h"
#include "OPButton.h"
#include "afxwin.h"

// CControlSystemDlg dialog
class IMotorCtrl;
class CImageProcess;
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

public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	// Generated message map functions	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedSaveAs();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCameraParam();
	afx_msg void OnBnClickedButtonImageProc();	
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedButtonCapture();
	afx_msg void OnBnClickedAutoMear();
	afx_msg void OnBnClickedImageProcSettingBtn();
	afx_msg void OnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLinkclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMtConnect();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCustomMear();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedManualMear();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedClearZeroX();
	afx_msg void OnBnClickedClearZeroY();
	afx_msg void OnBnClickedClearZeroZ();
	afx_msg void OnBnClickedLimitX();

private:
	CStatic m_staticPicture;

public:
	bool CalculatePoint(float x, float y, float z, float &retx, float &rety, float &retz);

public:
	void OpenHalconWind();
private:
	HTuple  hv_WindowID;
	bool m_HalconWndOpened;
	CImageProcess* m_IImageProcess;
	CImageProcSettingDlg* m_ImageProcSetDlg;

public:
	bool ConvertStringToFloat(CString buffer, float &value);
	bool GetFloatItem(int row, int column, float &value);
	bool SetFloatItem(int row, int column, float value);
	bool GetMeasureTargetValue(int row, float &x, float &y, float &z);
	bool SetMeasureResultValue(int row, float resultX, float resultY, float resultZ);
private:
	CListCtrl   m_ListData;
	bool		m_excelLoaded;
	int			m_columnNum;
	int			m_rowNum;

private:
	Camera*     m_pCamera;

public:
	bool MotoMoveToXY(float x, float y);
	void OnOpButtonUp(UINT nID);
	void OnOpButtonDown(UINT nID);
private:
	CMotorCtrl* m_pMotorCtrl;
	IMotorCtrl* m_IMotoCtrl;
	IHeightDectector* m_IHeightDectector;
	bool m_IsMotroCtrlConnected;

	OPButton m_LeftXBtn;
	OPButton m_LeftYBtn;
	OPButton m_LeftZBtn;
	OPButton m_RightXBtn;
	OPButton m_RightYBtn;
	OPButton m_RightZBtn;

public:
	float m_CustomX;
	float m_CustomY;
	float m_CustomZ;

	CEdit m_ZCurPosAbs;
	CEdit m_XCurPosAbs;
	CEdit m_YCurPosAbs;


private:
	int m_Process;
	
public:
	bool m_IsMeasuring;	
	void EnableOtherControls();

public:
	void ReSize(void); //¶Ô»°¿òResize
private:
	POINT m_OldPoint;

public:
	void OnAxisLimtiTimer();
	void UpdateXAlarmOn();
	void UpdateYAlarmOn();
	void UpdateZAlarmOn();

private:
	HICON m_hIconRed;
	HICON m_hIconGray;
	HICON m_hIconGreen;

	CStatic m_LimitErrX;
	CStatic m_LimitErrY;
	CStatic m_LimitErrZ;

	bool m_XAxisLimit;
	bool m_YAxisLimit;
	bool m_ZAxisLimit;


public:
	void StartProcess();

private:
	CWinThread* m_ProcessThread;

};
