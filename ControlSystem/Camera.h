#pragma once

#include "HQDLAPIUSER.h"

#define MAXWIDTH     1280
#define MAXHEIGHT    1024

typedef struct _tagCamFeature
{
	BOOL    bBW;
	BOOL    bRawDataShow;
	BOOL    bClearView;
	BOOL    bPause;
	BOOL    bUpDown;
	BOOL    bCaptureRaw;
}CAMFEATURE, *PCAMFEATURE;

class Camera
{
public:
	Camera(void);
	~Camera(void);

public:
	void Initialize(void);
	void Destroy(void);
	void SetCamFeature(void);
	void FinishAE( DWORD dwResult );
	void FinishAWB( DWORD dwResult );
	void DoPlay( bool bPlay, HWND hwndParent );
	void SetDispRect(RECT rect) {m_DispRect = rect;}
	int DoCapture(void);
	int SetCapInfo(void);
	void EnableColorOffset(int nRedOffset, int nGreenOffset, int nBlueOffset, BOOL bColorOffset );
	void SetParam( DLPARAM  DLParam, ULONG  Value );
	void SetDoAWB(void);
	void SetDoAE(void);
	void PauseView( BOOL bPause );

public:
	CapInfoStruct	m_CapInfo;
	CAMFEATURE		m_CamFeature;
	CString			m_strFileName;

private:
	HANDLE			m_hDevice;
	
	BOOL			m_bPlay;
	RECT			m_DispRect;
	BYTE			*m_pRawData;
	BYTE			*m_pRgbData;
};

