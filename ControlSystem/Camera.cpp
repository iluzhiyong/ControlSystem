// Camera.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "Camera.h"
#include "afxdialogex.h"

VOID CALLBACK AWBCallback( DWORD dw1, LPVOID lpContext )
{
	CCamera *pDlg = ( CCamera*)lpContext;
	DWORD     dwResult = dw1;
	pDlg->FinishAWB( dwResult );
}

VOID CALLBACK AECallback( DWORD dw1, LPVOID lpContext )
{
	CCamera *pDlg = ( CCamera*)lpContext;
	DWORD     dwResult = dw1;
	pDlg->FinishAE( dwResult );
}

VOID CALLBACK WINAPI FrameCallBack( LPVOID lpParam1, LPVOID lpPoint, LPVOID lpContext )
{
	UCHAR *pRgb = (UCHAR*)lpParam1;
	OutputDebugString( "Frame CallBack!!!!!!!");
	CCamera* pDLDlg = (CCamera*)lpContext;

	//static   int Count = 0;
	//TCHAR    Buf[256];
	//_stprintf( Buf, "%d", Count );
	//OutputDebugString( Buf );
	//// 每读一帧，就读取一个引脚
	//pDLDlg->ReadPin( Count % 8 );
	//Count++;
}

// CCamera dialog

IMPLEMENT_DYNAMIC(CCamera, CDialog)

CCamera::CCamera(CWnd* pParent /*=NULL*/)
	: CDialog(CCamera::IDD, pParent)
{
	m_bPlay = FALSE;
	m_strMessage = _T("");
	m_bRawDataShow = FALSE;
	m_bUpDown = FALSE;
	m_bClearPreview = FALSE;
	m_bBW = FALSE;
	m_bPause = FALSE;
	m_hDevice = NULL;
}

CCamera::~CCamera()
{
}

void CCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_BW, m_bBW);
	DDX_Check(pDX, IDC_CHECK_CLEAR, m_bClearPreview);
	DDX_Check(pDX, IDC_CHECK_PAUSE, m_bPause);
	DDX_Check(pDX, IDC_CHECK_PLAY, m_bPlay);
	DDX_Check(pDX, IDC_CHECK_RAWDATA, m_bRawDataShow);
	DDX_Check(pDX, IDC_CHECK_UPDOWN, m_bUpDown);
	DDX_Control(pDX, IDC_COMBO_COLOR, m_comboxColor);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_comboSize);
	DDX_Control(pDX, IDC_SLIDER_COLOR, m_sliderColor);
	DDX_Text(pDX, IDC_STATIC_MESSAGE, m_strMessage);
	DDX_Control(pDX, IDC_STATIC_VIDEO, m_staticVideo);
}


BEGIN_MESSAGE_MAP(CCamera, CDialog)
	ON_BN_CLICKED(IDC_CHECK_PLAY, &CCamera::OnBnClickedCheckPlay)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, &CCamera::OnCbnSelchangeComboSize)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR, &CCamera::OnCbnSelchangeComboColor)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR, &CCamera::OnNMCustomdrawSliderColor)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_COLOR, &CCamera::OnReleasedcaptureSliderColor)
	ON_BN_CLICKED(IDC_BUTTON_CAP, &CCamera::OnBnClickedButtonCap)
	ON_BN_CLICKED(IDC_CHECK_RAWDATA, &CCamera::OnBnClickedCheckRawdata)
	ON_BN_CLICKED(IDC_CHECK_BW, &CCamera::OnBnClickedCheckBw)
	ON_BN_CLICKED(IDC_CHECK_UPDOWN, &CCamera::OnBnClickedCheckUpdown)
	ON_BN_CLICKED(IDC_CHECK_CLEAR, &CCamera::OnBnClickedCheckClear)
	ON_BN_CLICKED(IDC_BUTTON_AWB, &CCamera::OnBnClickedButtonAwb)
	ON_BN_CLICKED(IDC_BUTTON_AE, &CCamera::OnBnClickedButtonAe)
	ON_BN_CLICKED(IDC_CHECK_PAUSE, &CCamera::OnBnClickedCheckPause)
END_MESSAGE_MAP()


// CCamera message handlers


BOOL CCamera::OnInitDialog()
{
	CDialog::OnInitDialog();
	//////////////////////////////////////

	m_nROff = m_nGOff = m_nBOff = 0;
	memset( &m_CamFeature, 0, sizeof( CAMFEATURE ) );

	m_pRawData = NULL;
	m_pRawData = ( BYTE* ) new BYTE [ MAXWIDTH * MAXHEIGHT + 512 ];
	m_pRgbData = NULL;
	m_pRgbData = ( BYTE* ) new BYTE [ MAXWIDTH * MAXHEIGHT * 3 ];
	memset( &m_CapInfo, 0, sizeof( CapInfoStruct ) );
	m_CapInfo.Buffer = m_pRawData;
	m_CapInfo.Width    = 640;
	m_CapInfo.Height   = 480;
	m_CapInfo.Exposure = 60;
	memset( m_CapInfo.Gain, 44, 3 );

	m_comboSize.SetCurSel( 2 );
	m_comboxColor.SetCurSel( 0 );

	int    nIndex;
	if( ResSuccess != HqDLInitialize(_T("Golden Hawk"), &nIndex, &m_CapInfo, &m_hDevice ) ) 
	{
		HqDLUninitialize( &m_hDevice );	
		m_hDevice = NULL;
		::MessageBox( NULL, _T("Initial Error"), _T("Demo"), 0 );
		return FALSE;
	}

	SetCamFeature();

	UpdateData( FALSE );

	return TRUE;
}

VOID CCamera::SetCamFeature()
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

void CCamera::OnBnClickedCheckPlay()
{
	UpdateData(TRUE);
	Play( m_bPlay );						// 开始采集/停止采集
}

VOID CCamera::Play( BOOL bPlay )
{
	if( m_hDevice == NULL )     return;
	m_bPlay = bPlay;

	RECT    rect;
	m_staticVideo.GetClientRect( &rect );

	if( m_bPlay )
	{
		HqDLStartView( m_hDevice, _T("Digital Lab"), WS_CHILD | WS_VISIBLE, 0, 0, rect.right, rect.bottom, m_staticVideo.m_hWnd, NULL );
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

void CCamera::FinishAWB(DWORD dwResult)
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
	m_strMessage.Format( "AWB Result Gain: Red %d, Green %d, Blue %d, Exposure %d\n", btRed, btGreen, btBlue, Exposure );
	// PostMessage to Update Interface
}

void CCamera::FinishAE(DWORD dwResult)
{
	ULONG Exposure = dwResult;
	m_CapInfo.Exposure = Exposure;
	// YOU MUST CALL HqDLSetCapInfo to Update DLL
	HqDLSetCapInfo( m_hDevice, &m_CapInfo );

	m_strMessage.Format( "AE Result Exposure %d",m_CapInfo.Exposure );
	// PostMessage to Update Interface
}

BOOL CCamera::DestroyWindow()
{
	Play( FALSE );
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
		HqDLUninitialize( &m_hDevice );

	return CDialog::DestroyWindow();
}


void CCamera::OnCbnSelchangeComboSize()
{
	int 	nSize = m_comboSize.GetCurSel();
	ULONG   Width, Height;
	switch( nSize )
	{
	case 0: Width = 320;  Height = 240;  break;
	case 1: Width = 640;  Height = 480;  break;
	case 2: Width = 1280; Height = 1024; break;
	}

	m_CapInfo.Width		= Width;
	m_CapInfo.Height	= Height;
	HqDLSetCapInfo( m_hDevice, &m_CapInfo );
	UpdateData( FALSE );
}


void CCamera::OnCbnSelchangeComboColor()
{
	if( m_hDevice == NULL )    return;
	int 	nColor = m_comboxColor.GetCurSel();
	ULONG   Min = 1;
	ULONG   Max = 1;
	ULONG   Value;
	switch( nColor )
	{
	case 0: Max = 63;  Value = m_CapInfo.Gain[0];    break;
	case 1: Max = 63;  Value = m_CapInfo.Gain[1];    break;
	case 2:	Max = 63;  Value = m_CapInfo.Gain[2];    break;

	case 3: Max = 500; Value = m_CapInfo.Exposure;    break;

	case 4: Min = -255; Max = 255;  Value = m_nROff; break;
	case 5: Min = -255; Max = 255;  Value = m_nGOff; break;
	case 6: Min = -255; Max = 255;  Value = m_nBOff; break;

	case 7: Min = 0; Max = MAXWIDTH - m_CapInfo.Width;		Value = m_CapInfo.OffsetX;		break;
	case 8: Min = 0; Max = MAXHEIGHT - m_CapInfo.Height;    Value = m_CapInfo.OffsetY;		break;

	}

	m_sliderColor.SetRange( Min, Max );
	m_sliderColor.SetPos( Value );
	UpdateData(FALSE);
}


void CCamera::OnNMCustomdrawSliderColor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CCamera::OnReleasedcaptureSliderColor(NMHDR *pNMHDR, LRESULT *pResult)
{
	int      nPos = m_sliderColor.GetPos();
	int      nColor = m_comboxColor.GetCurSel();


	switch( nColor )
	{
#ifdef DLC131
	case 0:  
	case 1:  
	case 2:
		m_CapInfo.Gain[0]  = m_CapInfo.Gain[1]  = m_CapInfo.Gain[2] = nPos; 	
		HqDLSetCapInfo( m_hDevice, &m_CapInfo );
		break;		

#else
	case 0:  m_CapInfo.Gain[0] = nPos;    HqDLSetCapInfo( m_hDevice, &m_CapInfo ); break;
	case 1:  m_CapInfo.Gain[1] = nPos;    HqDLSetCapInfo( m_hDevice, &m_CapInfo ); break;
	case 2:	 m_CapInfo.Gain[2] = nPos;    HqDLSetCapInfo( m_hDevice, &m_CapInfo ); break;
#endif
	case 3:  m_CapInfo.Exposure = nPos;    HqDLSetCapInfo( m_hDevice, &m_CapInfo ); break;

#ifdef DLC131
	// for mono camera, you must set Gain[0] = Gain[1] = Gain[2]
	// and m_nROff = m_nGOff = m_nBOff
	case 4:  
	case 5:  
	case 6:
		m_nROff  = m_nGOff  = m_nBOff = nPos; 	HqDLEnableColorOffset( m_hDevice, m_nROff, m_nGOff, m_nBOff, TRUE ); 
		break;		
#else
	case 4:  m_nROff  = nPos; 	HqDLEnableColorOffset( m_hDevice, m_nROff, m_nGOff, m_nBOff, TRUE ); break;
	case 5:  m_nGOff  = nPos; 	HqDLEnableColorOffset( m_hDevice, m_nROff, m_nGOff, m_nBOff, TRUE ); break;
	case 6:  m_nBOff  = nPos; 	HqDLEnableColorOffset( m_hDevice, m_nROff, m_nGOff, m_nBOff, TRUE ); break;
#endif
	case 7:  m_CapInfo.OffsetX = nPos;		HqDLSetCapInfo( m_hDevice, &m_CapInfo ); break;
	case 8:  m_CapInfo.OffsetY = nPos;		HqDLSetCapInfo( m_hDevice, &m_CapInfo ); break;

	}
	
	*pResult = 0;
}


void CCamera::OnBnClickedButtonCap()
{
	if( m_hDevice == NULL )    return;


	if( m_CamFeature.bRawDataShow )
	{
		m_strMessage = _T( "Raw.bmp" );
		HqDLGetRawFrameToBmp(m_hDevice, &m_CapInfo, m_strMessage );
		UpdateData( FALSE );
		return;
	}

	if( m_CamFeature.bPause )
	{
		m_strMessage = _T( "Pause.bmp" );
		HqDLGetPausedFrameToBmp(m_hDevice, NULL, m_strMessage );
		UpdateData( FALSE );
		return;
	}

	else
	{
		m_strMessage = _T( "Rgb.bmp" );
		HqDLGetRgbFrameToBmp( m_hDevice, &m_CapInfo, NULL, m_strMessage ); 
		UpdateData( FALSE );
	}
}


void CCamera::OnBnClickedCheckRawdata()
{
	UpdateData();
	m_CamFeature.bRawDataShow = m_bRawDataShow;
	SetCamFeature();
}


void CCamera::OnBnClickedCheckBw()
{
	UpdateData();
	m_CamFeature.bBW = m_bBW;
	SetCamFeature();
}


void CCamera::OnBnClickedCheckUpdown()
{
	UpdateData();
	m_CamFeature.bUpDown = m_bUpDown;
	SetCamFeature();
}


void CCamera::OnBnClickedCheckClear()
{
	UpdateData();
	m_CamFeature.bClearView = m_bClearPreview;
	SetCamFeature();
}


void CCamera::OnBnClickedButtonAwb()
{
	// for mono senor, you don't need AWB anymore!!!!
	if( m_hDevice == NULL )    return;
	HqDLSetDoAWB( m_hDevice, TRUE, 180, AWBCallback, (VOID*)this );
}


void CCamera::OnBnClickedButtonAe()
{
	if( m_hDevice == NULL )    return;
	HqDLSetDoAE( m_hDevice, TRUE, 180, AECallback, (VOID*)this );
}


void CCamera::OnBnClickedCheckPause()
{
	if( m_hDevice == NULL )    return;
	UpdateData();
	m_CamFeature.bPause = m_bPause;
	HqDLPauseView( m_hDevice, m_bPause );
}
