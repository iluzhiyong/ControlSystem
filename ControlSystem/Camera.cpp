#include "StdAfx.h"
#include "Camera.h"

VOID CALLBACK AWBCallback( DWORD dw1, LPVOID lpContext )
{
	Camera *pCamera = ( Camera*)lpContext;
	DWORD     dwResult = dw1;
	pCamera->FinishAWB( dwResult );
}

VOID CALLBACK AECallback( DWORD dw1, LPVOID lpContext )
{
	Camera *pCamera = ( Camera*)lpContext;
	DWORD     dwResult = dw1;
	pCamera->FinishAE( dwResult );
}

VOID CALLBACK WINAPI FrameCallBack( LPVOID lpParam1, LPVOID lpPoint, LPVOID lpContext )
{
	UCHAR *pRgb = (UCHAR*)lpParam1;
	OutputDebugString( "Frame CallBack!!!!!!!");
	Camera* pCamera = (Camera*)lpContext;

	//static   int Count = 0;
	//TCHAR    Buf[256];
	//_stprintf( Buf, "%d", Count );
	//OutputDebugString( Buf );
	//// 每读一帧，就读取一个引脚
	//pDLDlg->ReadPin( Count % 8 );
	//Count++;
}


Camera::Camera(void)
	: m_bPlay(FALSE)
	, m_strFileName(_T( "Raw.bmp" ))
	, m_hDevice(NULL)
	, m_pRawData(NULL)
	, m_pRgbData(NULL)
{
}


Camera::~Camera(void)
{
	Destroy();
}

void Camera::Initialize(void)
{
	m_pRawData = NULL;
	m_pRawData = ( BYTE* ) new BYTE [ MAXWIDTH * MAXHEIGHT + 512 ];
	m_pRgbData = NULL;
	m_pRgbData = ( BYTE* ) new BYTE [ MAXWIDTH * MAXHEIGHT * 3 ];
	memset( &m_CapInfo, 0, sizeof( CapInfoStruct ) );
	m_CapInfo.Buffer = m_pRawData;
	m_CapInfo.Width    = 640;	//1280, 640, 320
	m_CapInfo.Height   = 480;	//1024, 480, 240
	m_CapInfo.Exposure = 60;
	memset( m_CapInfo.Gain, 44, 3 );

	int    nIndex;
	if( ResSuccess != HqDLInitialize(_T("Golden Hawk"), &nIndex, &m_CapInfo, &m_hDevice ) )
	{
		HqDLUninitialize( &m_hDevice );
		m_hDevice = NULL;
		::MessageBox( NULL, _T("相机初始化失败! 请检查相机是否正常连接。"), _T("警告"), 0 );
		return;
	}

	m_CamFeature.bBW = 0;
	m_CamFeature.bRawDataShow = 0;
	m_CamFeature.bClearView = 0;
	m_CamFeature.bUpDown = 0;
	SetCamFeature();
}

void Camera::Destroy(void)
{
	DoPlay( FALSE,NULL);
	if( m_pRawData )
	{
		delete [] m_pRawData;
		m_pRawData = NULL;
	}

	if( m_pRgbData )
	{
		delete [] m_pRgbData;
		m_pRgbData = NULL;
	}

	if( m_hDevice != NULL )
	{
		HqDLUninitialize( &m_hDevice );
	}
}

void Camera::SetCamFeature(void)
{
	if( m_hDevice == NULL )    return;
#ifdef DLC131
	// for mono camera, you must set RAWDATASHOW is true
	m_CamFeature.bRawDataShow = TRUE;
#endif
	HqDLSetParam( m_hDevice, BWSHOW, m_CamFeature.bBW );
	HqDLSetParam( m_hDevice, RAWDATASHOW, m_CamFeature.bRawDataShow );
	HqDLSetParam( m_hDevice, CLEARVIEW, m_CamFeature.bClearView );
	HqDLSetParam( m_hDevice, UPDOWN, m_CamFeature.bUpDown );
}

void Camera::FinishAWB(DWORD dwResult)
{
	char buf[256];
	sprintf_s( buf, "AWB Result: 0x%x\n", dwResult );
	OutputDebugString( buf );
	BYTE btRed   = ( BYTE )dwResult;
	BYTE btGreen = ( BYTE ) ( dwResult>>8 );
	BYTE btBlue  = ( BYTE ) ( dwResult>>16 );
	ULONG Exposure = (BYTE)( dwResult>>24 ) * 2;

	m_CapInfo.Gain[0] = btRed;
	m_CapInfo.Gain[1] = btGreen;
	m_CapInfo.Gain[2] = btBlue;
	m_CapInfo.Exposure = Exposure;

	// YOU MUST CALL HqDLSetCapInfo to Update DLL
	HqDLSetCapInfo( m_hDevice, &m_CapInfo );
	//m_strMessage.Format( "AWB Result Gain: Red %d, Green %d, Blue %d, Exposure %d\n", btRed, btGreen, btBlue, Exposure );
	// PostMessage to Update Interface
}

void Camera::FinishAE(DWORD dwResult)
{
	ULONG Exposure = dwResult;
	m_CapInfo.Exposure = Exposure;
	// YOU MUST CALL HqDLSetCapInfo to Update DLL
	HqDLSetCapInfo( m_hDevice, &m_CapInfo );

	//m_strMessage.Format( "AE Result Exposure %d",m_CapInfo.Exposure );
	// PostMessage to Update Interface
}

void Camera::DoPlay( bool bPlay, HWND hwndParent )
{
	if( m_hDevice == NULL )     return;
	m_bPlay = bPlay;

	if( m_bPlay )
	{
		HqDLStartView( m_hDevice, _T("Digital Lab"), WS_CHILD | WS_VISIBLE, m_DispRect.left, m_DispRect.top, m_DispRect.right - m_DispRect.left, m_DispRect.bottom - m_DispRect.top, hwndParent, NULL );
		// You must call HqDLSetFrameCallback After Video Show - Warning!!!!!!!!!!!!
		HqDLSetFrameCallback( m_hDevice, FrameCallBack, this );
	}
	else
	{
		HqDLSetFrameCallback( m_hDevice, NULL, NULL );
		Sleep( 1000 );
		HqDLStopView( m_hDevice );
	}
}

int Camera::DoCapture(void)
{
	int ret = 1;

	if( m_hDevice == NULL )
	{
		ret = 0;
	}
	else
	{
		if( m_CamFeature.bRawDataShow == TRUE )
		{
			m_strFileName = _T( "Raw.bmp" );
			HqDLGetRawFrameToBmp(m_hDevice, &m_CapInfo, m_strFileName );
			AfxMessageBox(m_strFileName + " 拍摄成功！");
		}
		else if( m_CamFeature.bPause  == TRUE)
		{
			m_strFileName = _T( "Pause.bmp" );
			HqDLGetPausedFrameToBmp(m_hDevice, NULL, m_strFileName );
			AfxMessageBox(m_strFileName + " 拍摄成功！");
		}
		else
		{
			m_strFileName = _T( "Rgb.bmp" );
			HqDLGetRgbFrameToBmp( m_hDevice, &m_CapInfo, NULL, m_strFileName );
			AfxMessageBox(m_strFileName + " 拍摄成功！");
		}
	}

	return ret;
}

int Camera::SetCapInfo(void)
{
	HqDLSetCapInfo( m_hDevice, &m_CapInfo );

	return 0;
}

void Camera::EnableColorOffset(int nRedOffset, int nGreenOffset, int nBlueOffset, BOOL bColorOffset )
{
	HqDLEnableColorOffset( m_hDevice, nRedOffset, nGreenOffset, nBlueOffset, bColorOffset );
}

void Camera::SetParam( DLPARAM  DLParam, ULONG Value )
{
	HqDLSetParam( m_hDevice, DLParam, Value );
}

void Camera::SetDoAWB(void)
{
	// for mono senor, you don't need AWB anymore!!!!
	if( m_hDevice == NULL )    return;
	HqDLSetDoAWB( m_hDevice, TRUE, 180, AWBCallback, (VOID*)this );
}

void Camera::SetDoAE(void)
{
	if( m_hDevice == NULL )    return;

	HqDLSetDoAE( m_hDevice, TRUE, 180, AECallback, (VOID*)this );
}

void Camera::PauseView( BOOL bPause )
{
	HqDLPauseView( m_hDevice, bPause );
}