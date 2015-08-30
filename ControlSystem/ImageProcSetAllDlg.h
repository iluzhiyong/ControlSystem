#pragma once

class CImageProcSettingDlg;
class CImageProcSetOblongDlg;
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
	CImageProcSettingDlg*		m_ImageProcSetDlg;
	CImageProcSetOblongDlg*		m_imageProcSetOblongDlg;
	CImageProcSetRectangleDlg*	m_imageProcSetRectangleDlg;
	//CImageProcSetLineDlg*		m_imageProcSetLineDlg;
	CImageProcSettingDlg*		m_ImageProcSetSmallCircleDlg;
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_tabCtrl;
};
