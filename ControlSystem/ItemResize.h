#pragma once
#include <stdio.h>
#include <vector>

using namespace std;

typedef struct _RESIZEINFO
{
	UINT nID;
	CWnd* pParent;
	float fLeft;
	float fRight;
	float fTop;
	float fBottom;

}RESIZEINFO;

class CItemResize
{
	CRect m_rectDlg;
	vector<RESIZEINFO> m_vResizeID;

public:
	CItemResize(void);
	~CItemResize(void);

	void AddItemRect(UINT nID, CWnd* pParent);
	void ResizeItem();
};

