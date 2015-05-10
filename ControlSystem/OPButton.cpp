// OPButton.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "OPButton.h"
#include "ControlSystemDlg.h"

// OPButton

IMPLEMENT_DYNAMIC(OPButton, CButton)

OPButton::OPButton()
{

}

OPButton::~OPButton()
{
}


BEGIN_MESSAGE_MAP(OPButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// OPButton message handlers




void OPButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CButton::OnLButtonDown(nFlags, point);

	((CControlSystemDlg*)GetParent())->OnOpButtonDown(this->GetDlgCtrlID());
}


void OPButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	CButton::OnLButtonUp(nFlags, point);

	((CControlSystemDlg*)GetParent())->OnOpButtonUp(this->GetDlgCtrlID());
}
