#include "StdAfx.h"
#include "ItemResize.h"


CItemResize::CItemResize(void)
{
}


CItemResize::~CItemResize(void)
{
}

void CItemResize::AddItemRect(UINT nID, CWnd* pParent)
{
	RESIZEINFO rf = {0};
	rf.nID = nID;
	rf.pParent = pParent;
	CRect rect;
	pParent->GetClientRect(&m_rectDlg);
	CWnd* pItem = pParent->GetDlgItem(nID);
	ASSERT(pItem); //判断控件指针是否有效
	pItem->GetWindowRect(&rect);
	pParent->ScreenToClient(&rect);

	rf.fLeft = (float)(rect.left) / (float)m_rectDlg.Width();
	rf.fRight = (float)(rect.right) / (float)m_rectDlg.Width();
	rf.fTop = (float)(rect.top) / (float)m_rectDlg.Height();
	rf.fBottom = (float)(rect.bottom) / (float)m_rectDlg.Height();

	m_vResizeID.push_back(rf);

}

void CItemResize::ResizeItem()
{
	for(UINT i = 0; i < m_vResizeID.size(); i++)
	{
		m_vResizeID[i].pParent->GetClientRect(&m_rectDlg);
		CWnd* pItem = m_vResizeID[i].pParent->GetDlgItem(m_vResizeID[i].nID);
		if(NULL != pItem)
		{
			CRect rect;
			rect.left = (int)(m_rectDlg.Width() * m_vResizeID[i].fLeft);
			rect.right = (int)(m_rectDlg.Width() * m_vResizeID[i].fRight);
			rect.top = (int)(m_rectDlg.Height() * m_vResizeID[i].fTop);
			rect.bottom = (int)(m_rectDlg.Height() * m_vResizeID[i].fBottom);
			pItem->MoveWindow(rect); //设置控件大小
		}
	}
}