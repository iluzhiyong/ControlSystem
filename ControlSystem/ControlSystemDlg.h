
// ControlSystemDlg.h : header file
//

#pragma once
#include "HalconAction.h"
#include "MotorCtrl.h"
#include "ImageProcSettingDlg.h"
#include "OPButton.h"
#include "afxwin.h"
#include "ItemResize.h"
#include <IDevice.h>
#include "SetupDlg.h"

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
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedAutoMear();
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnBnClickedCustomMear();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedSetPosX();
	afx_msg void OnBnClickedSetPosY();
	afx_msg void OnBnClickedSetPosZ();
	afx_msg void OnBnClickedLimitX();
	afx_msg LRESULT OnUpdateMotorStatus(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedBtnOutputImage(UINT nID);
	afx_msg void OnCameraConnect();
	afx_msg void OnCameraCapture();
	afx_msg void OnCameraParamSet();
	afx_msg void OnMotorConnect();
	afx_msg void OnImageProc();
	afx_msg void OnImageParamSet();
	afx_msg void OnCaculateAxialDeviationAngle();
	afx_msg LRESULT OnMainThreadDoCapture(WPARAM wParam,LPARAM lParam);
	void ReCaculateResultByCompensation();

private:
	CStatic m_staticPicture;

private:
	HTuple  hv_WindowID;
	bool m_HalconWndOpened;
	CImageProcess* m_IImageProcess;
	CImageProcSettingDlg* m_ImageProcSetDlg;

private:
	CListCtrl	m_ListData;
	bool		m_excelLoaded;
	int			m_columnNum;
	int			m_rowNum;

public:
	void OnOpButtonUp(UINT nID);
	void OnOpButtonDown(UINT nID);
private:
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

	float m_compensationX;
	float m_compensationY;
	float m_compensationZ;

	CEdit m_ZCurPosAbs;
	CEdit m_XCurPosAbs;
	CEdit m_YCurPosAbs;


private:
	int m_Process;
	
public:
	bool m_IsMeasuring;	
	void EnableOtherControls();

private:
	CItemResize m_itemSize;

public:
	void UpdateCameraRunStatus();
	void UpDateMotorRunStatus();

private:
	HICON m_hIconRed;
	HICON m_hIconGray;
	HICON m_hIconGreen;

private:
	CWinThread* m_UIProcThread;

private:
	//相机相关变量开始:考虑到相机需要实时的显示镜头前的影响，直接做到主线程下。
	CSetupDlg			*m_pCameraSetupDlg;
	BOOL				m_bImageAspectRatio;
	IDevice				*m_pCameraDevice;
	CCriticalSection	m_csImageData;
	BYTE				*m_pImageData;
	int					m_nImageDataSize;
	int					m_nImageWidth;
	int					m_nImageHeight;
	CString				m_sImagefilename;
	//相机相关变量结束

	//相机相关方法开始
	void CameraDestroy(void);
	void CameraInit(void);
	void CameraReceiveDataProc(BYTE *pImgData, int nWidth, int nHeight);
	static void CALLBACK CameraInitReceiveDataProc(LPVOID pDevice, BYTE *pImageBuffer, DeviceFrameInfo *pFrInfo, LPVOID lParam);
	void CameraUpdatePictureDisp(void);
	void CameraPlay(void);
	void CameraStop(void);
	void CameraCapture(void);
	//相机相关方法结束
public:
	bool m_bMotorRunStatus;
	bool m_bCameraRunStatus;
	CStatic m_MotorRunStatus;
	CStatic m_CameraRunStatus;

private:
	CMenu m_Menu;
};
