#pragma once

class CProcThread;
// CAxialDeviationAngle dialog

class CAxialDeviationAngle : public CDialog
{
	DECLARE_DYNAMIC(CAxialDeviationAngle)

public:
	CAxialDeviationAngle(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAxialDeviationAngle();

	void SetParent(CProcThread* parent){ m_UIProcThread = parent;};

// Dialog Data
	enum { IDD = IDD_AXIAL_DEVIATION_ANGLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_axialAngle;
	float m_axialAnglePos1X;
	float m_axialAnglePos1Y;
	float m_axialAnglePos1Z;
	float m_axialAnglePos2X;
	float m_axialAnglePos2Y;
	float m_axialAnglePos2Z;
	afx_msg void OnBnClickedBtnAxialAnglePos1();
	afx_msg void OnBnClickedBtnAxialAnglePos2();
	afx_msg void OnBnClickedBtnAxialAngleCac();

private:
	float m_rstPos1X;
	float m_rstPos1Y;
	float m_rstPos1Z;
	float m_rstPos2X;
	float m_rstPos2Y;
	float m_rstPos2Z;

public:
	CProcThread* m_UIProcThread;
};
