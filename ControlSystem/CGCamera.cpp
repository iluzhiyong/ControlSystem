#include "StdAfx.h"
#include "CGCamera.h"
#include "ControlSystem.h"
#include "ControlSystemDlg.h"
#include "afxwin.h"

CGCamera::CGCamera(void)
	: m_strFileName(_T( "Raw.bmp" ))
	, m_pDevice(NULL)
	, m_pImageData(NULL)
	, m_nDataSize(0)
	, m_nWidth(0)
	, m_nHeight(0)
{
	
}

CGCamera::~CGCamera(void)
{
	if(m_pDevice != NULL)
	{
		m_pDevice->Stop();
		m_pDevice->CloseDevice();
		m_pDevice->DeviceUnInit();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	IDevice::DeviceUnInitialSDK();

	if(m_pImageData != NULL)
	{
		delete []m_pImageData;
	}
}


void CGCamera::Initialize(void)
{
	IDevice::DeviceInitialSDK(NULL, FALSE);

	DetectDevice();
}

void CGCamera::DetectDevice()
{
	INT nDevice = 0;
	if(SUCCEEDED(IDevice::EnumDevice(NULL, &nDevice)))
	{
		if(nDevice == 0)
		{
			//pCombo->AddString(NODETECTDEVICETEXT);
		}else
		{
			LPEnumDeviceParam lpEDP = new EnumDeviceParam[nDevice];
			if(lpEDP != NULL)
			{
				if(SUCCEEDED(IDevice::EnumDevice(lpEDP, &nDevice)))
				{
					
				}
				delete []lpEDP;
			}
		}
	}

	if(m_pDevice == NULL)
	{
		UionOpenDeviceParam param;
		param.devIndex = 0;
		DeviceStatus devStatus = IDevice::OpenDevice(param, &m_pDevice);
		if(SUCCEEDED(devStatus))
		{
			devStatus = m_pDevice->DeviceInitEx(InitReceiveDataProc, this, NULL, TRUE);
			//if(m_pSetupDlg != NULL) m_pSetupDlg->UpdateDevice(m_pDevice);
		}
		if(FAILED(devStatus))
		{
			CString strText;
			strText.Format(_T("Error Code: %d"), devStatus);
			::AfxMessageBox(strText, MB_ICONERROR|MB_OK);
		}
		else
		{
			m_pDevice->Start();
		}
	}
}

void CGCamera::ReceiveDataProc(BYTE *pImgData, int nWidth, int nHeight)
{
	++m_nFrames;
	int nDataSize = (((nWidth * 24 + 31) & ~31) >> 3) * nHeight;
	if(m_pImageData != NULL && nDataSize != m_nDataSize){
		delete []m_pImageData;
		m_pImageData = NULL;
	}
	if(m_pImageData == NULL){
		m_nDataSize = nDataSize;
		m_pImageData = new BYTE[m_nDataSize];
		m_nWidth = nWidth;
		m_nHeight = nHeight;
	}
	m_csImageData.Lock();
	memcpy(m_pImageData, pImgData, nDataSize);
	m_csImageData.Unlock();
	//PostMessage(WM_UPDATE_PICTURE);
}

void CALLBACK CGCamera::InitReceiveDataProc(LPVOID pDevice, BYTE *pImageBuffer, DeviceFrameInfo *pFrInfo, LPVOID lParam)
{
	BYTE *pRGB24Buff = NULL;
	if((pRGB24Buff = ((IDevice *)pDevice)->DeviceISP(pImageBuffer, pFrInfo)) != NULL)
	{
		((CGCamera *)lParam)->ReceiveDataProc(pRGB24Buff, pFrInfo->uiWidth, pFrInfo->uiHeight);
	}
}

void CGCamera::DoPlay( bool bPlay, HWND hwndParent )
{
	if(m_pDevice != NULL && !m_pDevice->IsReceivingData())
	{
		DeviceStatus devStatus = m_pDevice->Start();
		if(FAILED(devStatus))
		{
			CString strText;
			strText.Format(_T("Error Code: %d"), devStatus);
			::AfxMessageBox(strText, MB_ICONERROR|MB_OK);
		}
	}
}

int CGCamera::DoCapture(void)
{
	if(m_pDevice != NULL )
	{
		//emDSFileType type = FILE_BMP;
		//CString strPath = (m_pSetupDlg != NULL ? m_pSetupDlg->GetPicPath(type) : _T(""));
		//if(strPath.IsEmpty())
		//{
		//	GetModuleFileName(NULL, strPath.GetBuffer(256), 256);
		//	strPath.ReleaseBuffer();
		//	int nPos = strPath.ReverseFind(_T('\\'));
		//	strPath = strPath.Left(nPos + 1);
		//	strPath += _T("Pic\\");
		//}
		//if (!PathIsDirectory(strPath))
		//{
		//	CreateDirectory(strPath, NULL);
		//}
		CTime time = CTime::GetCurrentTime();
		CString sfilename; 
		sfilename.Format(_T("Image_%02d%02d%02d%02d%02d"), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		DeviceStatus devStatus = m_pDevice->CaptureFile(sfilename, FILE_BMP);
		if(SUCCEEDED(devStatus))
		{
			AfxMessageBox(_T("≈ƒ’’≥…π¶!"));
		}
		else
		{
			CString strText;
			strText.Format(_T("Error Code: %d"), devStatus);
			::AfxMessageBox(strText, MB_ICONERROR|MB_OK);
		}
	}

	return 1;
}