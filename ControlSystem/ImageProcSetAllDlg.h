#pragma once

class CImageProcSettingDlg;
class CImageProcSetCircleDlg;
class CImageProcSetRectangleDlg;
class CImageProcSetLineDlg;

// CImageProcSetAllDlg dialog

class CImageProcSetAllDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageProcSetAllDlg)

public:
	CImageProcSetAllDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageProcSetAllDlg();

// Dialog Data
	enum { IDD = IDD_IMAGE_PROC_SETTING_ALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	afx_msg void OnTcnSelchangeTabSetup(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

public:
	CImageProcSetCircleDlg*		m_imageProcSetCircleDlg;
	CImageProcSetRectangleDlg*	m_imageProcSetRectangleDlg;
	CImageProcSetLineDlg*		m_imageProcSetLineDlg;
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_tabCtrl;
};
