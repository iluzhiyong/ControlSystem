#pragma once
#include "StdAfx.h"
#include <IDevice.h>

class CGCamera
{
public:
	CGCamera(void);
	~CGCamera(void);

public:
	void Initialize(void);
	void DoPlay( bool bPlay, HWND hwndParent );
	void SetDispRect(RECT rect) 
	{
		m_DispRect = rect;
		m_nWidth = rect.right - rect.left;
		m_nHeight = rect.bottom - rect.top;
	}
	int DoCapture(void);

private:
	void DetectDevice();
	void ReceiveDataProc(BYTE *pImgData, int nWidth, int nHeight);
	static void CALLBACK  InitReceiveDataProc(LPVOID pDevice, BYTE *pImageBuffer, DeviceFrameInfo *pFrInfo, LPVOID lParam);

private:
	RECT				m_DispRect;
	UINT				m_nFrames;
	BOOL				m_bAspectRatio;
	IDevice				*m_pDevice;
	CCriticalSection	m_csImageData;
	BYTE				*m_pImageData;
	int					m_nDataSize;
	int					m_nWidth;
	int					m_nHeight;
	CString				m_strFileName;
};