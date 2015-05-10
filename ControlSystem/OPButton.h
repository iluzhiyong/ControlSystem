#pragma once


// OPButton

class OPButton : public CButton
{
	DECLARE_DYNAMIC(OPButton)

public:
	OPButton();
	virtual ~OPButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


