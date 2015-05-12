
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
	afx_msg void OnBnClickedButtonCapture();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedAutoMear();

public:
	bool CalculatePoint(float x, float y, float z, float &retx, float &rety, float &retz);
	bool GetMeasureTargetValue(int row, float &x, float &y, float &z);
	bool SetMeasureResultValue(int row, float resultX, float resultY, float resultZ);
	bool GetFloatItem(int row, int column, float &value);
	bool SetFloatItem(int row, int column, float value);
	bool ConvertStringToFloat(CString buffer, float &value);
	void OpenHalconWind();
	void OnOpButtonUp(UINT nID);
	void OnOpButtonDown(UINT nID);

private:
	IMotorCtrl* m_IMotoCtrl;
	CImageProcess* m_IImageProcess;
	IHeightDectector* m_IHeightDectector;
	HTuple  hv_WindowID;
	bool m_excelLoaded;
	int m_columnNum;
	int m_rowNum;

	bool m_HalconWndOpened;

	CImageProcSettingDlg* m_ImageProcSetDlg;

public:
	bool MotoMoveToXY(float x, float y);
	afx_msg void OnBnClickedCustomMear();
	float m_CustomX;
	float m_CustomY;
	float m_CustomZ;
	afx_msg void OnBnClickedImageProcSettingBtn();
	afx_msg void OnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLinkclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMtConnect();
	afx_msg void OnClose();

	CEdit m_ZCurPosAbs;
	CEdit m_XCurPosAbs;
	CEdit m_YCurPosAbs;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedManualMear();
	int m_Process;

	bool m_IsMotroCtrlConnected;
	bool m_IsMeasuring;
	void EnableOtherControls();

	//¶Ô»°¿òResize
	void ReSize(void);
	POINT m_OldPoint;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedClearZeroX();
	afx_msg void OnBnClickedClearZeroY();
	afx_msg void OnBnClickedClearZeroZ();
	OPButton m_LeftXBtn;
	OPButton m_LeftYBtn;
	OPButton m_LeftZBtn;
	OPButton m_RightXBtn;
	OPButton m_RightYBtn;
	OPButton m_RightZBtn;
	afx_msg void OnBnClickedLimitX();

public:
	HICON m_hIconRed;
	HICON m_hIconGray;
	HICON m_hIconGreen;

	void UpdateXAlarmOn();
	void UpdateYAlarmOn();
	void UpdateZAlarmOn();

	CStatic m_LimitErrX;
	CStatic m_LimitErrY;
	CStatic m_LimitErrZ;

	bool m_XAxisLimit;
	bool m_YAxisLimit;
	bool m_ZAxisLimit;

	void OnAxisLimtiTimer();

public:
	void StartProcess();

private:
	CWinThread* m_ProcessThread;


};
