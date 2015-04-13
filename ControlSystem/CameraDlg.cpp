// Camera.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "CameraDlg.h"
#include "afxdialogex.h"

// CCameraParaDlg dialog

IMPLEMENT_DYNAMIC(CCameraParaDlg, CDialog)

CCameraParaDlg::CCameraParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraParaDlg::IDD, pParent)
	, m_pCamera(NULL)
	, m_bRawDataShow(FALSE)
	, m_bUpDown(FALSE)
	, m_bClearPreview(FALSE)
	, m_bBW(FALSE)
{
}

CCameraParaDlg::~CCameraParaDlg()
{
}

void CCameraParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_BW, m_bBW);
	DDX_Check(pDX, IDC_CHECK_CLEAR, m_bClearPreview);
	DDX_Check(pDX, IDC_CHECK_RAWDATA, m_bRawDataShow);
	DDX_Check(pDX, IDC_CHECK_UPDOWN, m_bUpDown);
	DDX_Control(pDX, IDC_COMBO_COLOR, m_comboxColor);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_comboSize);
	DDX_Control(pDX, IDC_SLIDER_COLOR, m_sliderColor);
}


BEGIN_MESSAGE_MAP(CCameraParaDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_PLAY, &CCameraParaDlg::OnBnClickedCheckPlay)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, &CCameraParaDlg::OnCbnSelchangeComboSize)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR, &CCameraParaDlg::OnCbnSelchangeComboColor)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_COLOR, &CCameraParaDlg::OnNMCustomdrawSliderColor)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_COLOR, &CCameraParaDlg::OnReleasedcaptureSliderColor)
	ON_BN_CLICKED(IDC_BUTTON_CAP, &CCameraParaDlg::OnBnClickedButtonCap)
	ON_BN_CLICKED(IDC_CHECK_RAWDATA, &CCameraParaDlg::OnBnClickedCheckRawdata)
	ON_BN_CLICKED(IDC_CHECK_BW, &CCameraParaDlg::OnBnClickedCheckBw)
	ON_BN_CLICKED(IDC_CHECK_UPDOWN, &CCameraParaDlg::OnBnClickedCheckUpdown)
	ON_BN_CLICKED(IDC_CHECK_CLEAR, &CCameraParaDlg::OnBnClickedCheckClear)
	ON_BN_CLICKED(IDC_BUTTON_AWB, &CCameraParaDlg::OnBnClickedButtonAwb)
	ON_BN_CLICKED(IDC_BUTTON_AE, &CCameraParaDlg::OnBnClickedButtonAe)
	ON_BN_CLICKED(IDC_CHECK_PAUSE, &CCameraParaDlg::OnBnClickedCheckPause)
END_MESSAGE_MAP()


// CCameraParaDlg message handlers


BOOL CCameraParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//////////////////////////////////////

	m_nROff = m_nGOff = m_nBOff = 0;
	memset( &m_CamFeature, 0, sizeof( CAMFEATURE ) );

	m_comboSize.SetCurSel( 1 );
	m_comboxColor.SetCurSel( 0 );

	SetCamFeature();
	UpdateData( FALSE );

	return TRUE;
}

VOID CCameraParaDlg::SetCamFeature()
{
	if( m_pCamera != NULL )
	{
	#ifdef DLC131
		// for mono camera, you must set RAWDATASHOW is true
		m_CamFeature.bRawDataShow = TRUE;
	#endif
		m_pCamera->SetParam( BWSHOW, m_CamFeature.bBW );
		m_pCamera->SetParam( RAWDATASHOW, m_CamFeature.bRawDataShow );
		m_pCamera->SetParam( CLEARVIEW, m_CamFeature.bClearView );
		m_pCamera->SetParam( UPDOWN, m_CamFeature.bUpDown );
	}
}

void CCameraParaDlg::OnBnClickedCheckPlay()
{
	UpdateData(TRUE);
}

BOOL CCameraParaDlg::DestroyWindow()
{
	return CDialog::DestroyWindow();
}


void CCameraParaDlg::OnCbnSelchangeComboSize()
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
	UpdateData( FALSE );

	if(NULL != m_pCamera)
	{
		m_pCamera->SetCapInfo(&m_CapInfo);
	}
}

void CCameraParaDlg::OnCbnSelchangeComboColor()
{
	if( m_pCamera == NULL )    return;
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

	if(NULL != m_pCamera)
	{
		m_pCamera->SetCapInfo(&m_CapInfo);
	}
}

void CCameraParaDlg::OnNMCustomdrawSliderColor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CCameraParaDlg::OnReleasedcaptureSliderColor(NMHDR *pNMHDR, LRESULT *pResult)
{
	int      nPos = m_sliderColor.GetPos();
	int      nColor = m_comboxColor.GetCurSel();
	if(NULL == m_pCamera)
	{
		return;
	}

	switch( nColor )
	{
#ifdef DLC131
	case 0:  
	case 1:  
	case 2:
		m_CapInfo.Gain[0]  = m_CapInfo.Gain[1]  = m_CapInfo.Gain[2] = nPos; 	
		m_pCamera->SetCapInfo(&m_CapInfo);
		break;		

#else
	case 0:  m_CapInfo.Gain[0] = nPos;    m_pCamera->SetCapInfo(&m_CapInfo); break;
	case 1:  m_CapInfo.Gain[1] = nPos;    m_pCamera->SetCapInfo(&m_CapInfo); break;
	case 2:	 m_CapInfo.Gain[2] = nPos;    m_pCamera->SetCapInfo(&m_CapInfo); break;
#endif
	case 3:  m_CapInfo.Exposure = nPos;    m_pCamera->SetCapInfo(&m_CapInfo); break;

#ifdef DLC131
	// for mono camera, you must set Gain[0] = Gain[1] = Gain[2]
	// and m_nROff = m_nGOff = m_nBOff
	case 4:  
	case 5:  
	case 6:
		m_nROff  = m_nGOff  = m_nBOff = nPos; 	m_pCamera->EnableColorOffset(m_nROff, m_nGOff, m_nBOff, TRUE ); 
		break;		
#else
	case 4:  m_nROff  = nPos; 	m_pCamera->EnableColorOffset(m_nROff, m_nGOff, m_nBOff, TRUE ); break;
	case 5:  m_nGOff  = nPos; 	m_pCamera->EnableColorOffset(m_nROff, m_nGOff, m_nBOff, TRUE ); break;
	case 6:  m_nBOff  = nPos; 	m_pCamera->EnableColorOffset(m_nROff, m_nGOff, m_nBOff, TRUE ); break;
#endif
	case 7:  m_CapInfo.OffsetX = nPos;		m_pCamera->SetCapInfo(&m_CapInfo); break;
	case 8:  m_CapInfo.OffsetY = nPos;		m_pCamera->SetCapInfo(&m_CapInfo); break;

	}
	
	*pResult = 0;
}


void CCameraParaDlg::OnBnClickedButtonCap()
{
	if( m_pCamera != NULL )
	{
		m_pCamera->DoCapture();
	}
}

void CCameraParaDlg::OnBnClickedCheckRawdata()
{
	UpdateData();
	m_CamFeature.bRawDataShow = m_bRawDataShow;
	SetCamFeature();
}

void CCameraParaDlg::OnBnClickedCheckBw()
{
	UpdateData();
	m_CamFeature.bBW = m_bBW;
	SetCamFeature();
}

void CCameraParaDlg::OnBnClickedCheckUpdown()
{
	UpdateData();
	m_CamFeature.bUpDown = m_bUpDown;
	SetCamFeature();
}

void CCameraParaDlg::OnBnClickedCheckClear()
{
	UpdateData();
	m_CamFeature.bClearView = m_bClearPreview;
	SetCamFeature();
}

void CCameraParaDlg::OnBnClickedButtonAwb()
{
	// for mono senor, you don't need AWB anymore!!!!
	if( m_pCamera != NULL )
	{
		m_pCamera->SetDoAWB();
	}
}

void CCameraParaDlg::OnBnClickedButtonAe()
{
	if( m_pCamera != NULL )
	{
		m_pCamera->SetDoAE();
	}
}

void CCameraParaDlg::OnBnClickedCheckPause()
{
	if( m_pCamera != NULL )
	{
		UpdateData();
		//m_CamFeature.bPause = m_bPause;
		//m_pCamera->PauseView( m_bPause );
	}
}
