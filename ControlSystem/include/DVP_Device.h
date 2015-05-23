#ifndef _CGIMAGETECH_CAMAPI_H_
#define _CGIMAGETECH_CAMAPI_H_

#include <DVP_DeviceDefine.h>

#ifdef CGUSBDEVICE_EXPORTS 
	#define CG_API extern "C" __declspec(dllexport)
#else 
	#define CG_API extern "C" __declspec(dllimport) 
#endif

typedef void		*HDEVICE;				//相机设备句柄
typedef HDEVICE		*PHDEVICE;				//指向相机设备句柄的指针

/*--------------------------------------------------------------------------*\
| 功  能：获取SDK版本号
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：
|         1  DWORD  adwVersion			SDK版本号			输出参数
\*--------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceGetSDKVersion(DWORD adwVersion[4]);

/*--------------------------------------------------------------------------*\
| 功  能：初始化SDK
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：                                                            
|         1  HANDLE		handle		事件通知窗口句柄	 输入参数
|		  2  BOOL		bUsedStatus 启用设备使用标记	 输入参数
\*--------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceInitialSDK(HANDLE hHandle, BOOL bUsedStatus);

/*--------------------------------------------------------------------------------------*\
| 功  能：反初始化SDK
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：                                                            
|         1  HANDLE		handle		事件通知窗口句柄	 输入参数
\*--------------------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceUnInitialSDK();

/*------------------------------------------------------------------------*\
| 功  能：枚举相机设备                                                     
|                                                                          
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|         1  LPEnumDeviceParam	pDeviceList		相机列表的信息结构数组指针					输入参数
|         2  INT				*piNums			输入和返回相机列表数组的个数指标			输入参数
\*------------------------------------------------------------------------*/
CG_API DeviceStatus EnumDevice(LPEnumDeviceParam pDeviceList, INT *piNums);

/*------------------------------------------------------------------------*\
| 功  能：打开相机设备                                                     
|                                                                          
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数： 
|         1  PHDEVICE			pDevice			设备句柄指针		输出参数
\*------------------------------------------------------------------------*/
CG_API DeviceStatus SmartOpenDevice(PHDEVICE pDevice);

/*------------------------------------------------------------------------*\
| 功  能：打开指定相机设备                                                     
|                                                                          
| 返回值：调用成功返回STATUS_OK，否则返回错误代码	
|
| 参  数： 
|         1  UionOpenDeviceParam	param			打开相机的参数结构	 输入参数
|         2  PHDEVICE				pDevice			设备句柄指针		 输出参数
|		  3  OpenDeviceFlag			flag			打开设备的方式		 输入参数
\*------------------------------------------------------------------------*/
CG_API DeviceStatus OpenDevice(UionOpenDeviceParam param, PHDEVICE pDevice, emOpenDeviceFlag flag = OF_BYPOSITION);

/*------------------------------------------------------------------------*\
| 功  能：打开指定相机设备（相机序列方式）                                                     
|                                                                          
| 返回值：调用成功返回STATUS_OK，否则返回错误代码	
|
| 参  数： 
|         1  UCHAR					devIndex		相机设备对应序列	 输入参数
|         2  PHDEVICE				pDevice			设备句柄指针		 输出参数
\*------------------------------------------------------------------------*/
CG_API DeviceStatus OpenDeviceByIndex(UCHAR devIndex, PHDEVICE pDevice);

/*------------------------------------------------------------------------*\
| 功  能：打开指定相机设备(相机USB地址方式）                                                     
|                                                                          
| 返回值：调用成功返回STATUS_OK，否则返回错误代码	
|
| 参  数： 
|         1  UCHAR					devUSBAddr		相机设备的USB地址	 输入参数
|         2  PHDEVICE				pDevice			设备句柄指针		 输出参数
\*------------------------------------------------------------------------*/
CG_API DeviceStatus OpenDeviceByUSBAddress(UCHAR devUSBAddr, PHDEVICE pDevice);

/*------------------------------------------------------------------------*\
| 功  能：打开指定相机设备(用户设置的序列号方式）                                              
|                                                                          
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数： 
|         1  BYTE					devUserSN[32]		用户设置的序列号		 输入参数
|         2  PHDEVICE				pDevice				设备句柄指针		 输出参数
\*------------------------------------------------------------------------*/
CG_API DeviceStatus OpenDeviceByUserSN(BYTE devUserSN[32], PHDEVICE pDevice);

/*------------------------------------------------------------------------*\
| 功  能：增加相机的引用                                                     
|                                                                          
| 返回值：返回上一次的引用数，句柄无效时返回-1
|
| 参  数：                                                             
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
\*------------------------------------------------------------------------*/
CG_API ULONG DeviceAddReference(HDEVICE hDevice);

/*------------------------------------------------------------------------*\
| 功  能：减少相机的引用                                                     
|                                                                          
| 返回值：返回上一次的引用数，句柄无效时返回-1	
| 注意： 当设备的引用数为0时，设备会自动删除
|
| 参  数：                                                             
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
\*------------------------------------------------------------------------*/
CG_API ULONG DeviceRelease(HDEVICE hDevice);

/*------------------------------------------------------------------------*\
| 功  能：关闭相机设备  
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：                                                             
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
\*------------------------------------------------------------------------*/
CG_API DeviceStatus CloseDevice(HDEVICE hDevice);

/*------------------------------------------------------------------------*\
| 功  能：同步关闭相机设备                                                     
|                                                                          
| 返回值：调用成功返回STATUS_OK，否则返回错误代码	
| 参  数：                                                             
|         1  IDevice		**pDevice	设备接口			 输入参数
| 注意： 该函数主要用于即插即用功能时，防止多个进程之间枚举设备时的冲突。
| 如不使用即插即用功能时，则不需要调用此函数，可以直接调用IDevice的CloseDevice()接口来释放设备。
\*------------------------------------------------------------------------*/
CG_API DeviceStatus SyncCloseDevice(HDEVICE hDevice);

/*------------------------------------------------------------------------*\
| 功  能：得到设备地址 
|                                                                          
| 返回值：调用成功返回设备的地址，否则返回0
|
| 参  数：                                                             
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
\*------------------------------------------------------------------------*/
CG_API UCHAR GetDeviceAddress(HDEVICE hDevice);

/*------------------------------------------------------------------------*\
| 功  能：得到设备句柄 
|                                                                          
| 返回值：调用成功返回设备的句柄，否则返回NULL
|
| 参  数：                                                             
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
\*------------------------------------------------------------------------*/
CG_API HANDLE GetDeviceHandle(HDEVICE hDevice);

/*-----------------------------------------------------------------------*\
| 功  能：写入相机序列号
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：
|         1  HDEVICE hDevice	 相机设备句柄						输入参数
|         2  BYTE    *pbySN      序列号数据                         输入参数
|         3  INT     iSize       序列号字节数                       输入参数
|         4  INT     *piWritten  实际写入的字节数                   输出参数
|
| 注  意：
|         1、序列号的字节数最大为：32字节
\*-------------------------------------------------------------------------*/
CG_API DeviceStatus WriteSerialNumber(HDEVICE hDevice, BYTE *pbySN, INT iSize, INT *piWritten);

/*-------------------------------------------------------------------------*\
| 功  能：获取相机序列号
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：
|         1  HDEVICE	hDevice	   相机设备句柄						  输入参数
|         2  BYTE		*pbySN     序列号数据                         输出参数
|         3  INT		iSize      序列号字节数                       输入参数
|         4  INT		*piReaded  实际获取的字节数                   输出参数
|
| 注  意：
|         1、序列号的字节数最大为：32字节
\*--------------------------------------------------------------------------*/
CG_API DeviceStatus ReadSerialNumber(HDEVICE hDevice, BYTE *pbySN, INT iSize, INT *piReaded);

/*-------------------------------------------------------------------------*\
| 功  能：创建相机的属性配置窗口
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：
|		  1  HDEVICE	hDevice		相机设备句柄						输入参数
|		  2  HWND		hParent		配置窗口的父窗口句柄				输入参数
|         3  TCHAR		*pWinText	配置窗口的标题栏字符串指针          输入参数
|
\*--------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceCreateSettingPage(HDEVICE hDevice, HWND hParent, LPCTSTR pText);

/*-------------------------------------------------------------------------*\
| 功  能：显示相机配置窗口
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：
|		  1  HDEVICE	hDevice		相机设备句柄						输入参数
|         2  BOOL		bShow		是否显示配置窗口					输入参数
|
\*--------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceShowSettingPage(HDEVICE hDevice, BOOL bShow);

/*------------------------------------------------------------------------*\
| 功  能：是否为单色相机
|                                                                          
| 返回值：设备是单色相机返回TRUE，否则返回FALSE
|
| 参  数：                                                             
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
\*------------------------------------------------------------------------*/
CG_API BOOL IsMonochromeDevice(HDEVICE hDevice);

/*--------------------------------------------------------------------*\
| 功  能：初始化相机                                   
|                                                                      
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                      
|                                                                      
| 参  数：         
|         1  HDEVICE			hDevice		相机设备句柄								输入参数
|         2  HWND				hWndDisplay	显示窗口句柄								输入参数
|		  3  BOOL				bGetMode	是否使用DeviceGetImageBuffer函数调用方式	输入参数	
|		  4  BOOL				bAutoParam	是否启用参数自动读写功能					输入参数	
\*--------------------------------------------------------------------*/
CG_API DeviceStatus DeviceInit(HDEVICE hDevice, HWND hWndDisplay, BOOL bGetMode, BOOL bAutoParam);

/*--------------------------------------------------------------------*\
| 功  能：初始化相机                                   
|                                                                      
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                      
|                                                                      
| 参  数：         
|         1  HDEVICE			hDevice		相机设备句柄							  输入参数
|         2  ReceiveFrameProc	pFun		回调函数指针							  输入参数
|         3  LPVOID				lParam		指向一个值的指针，该值传递给回调函数	  输入参数
|         4  HWND				hWndDisplay	显示窗口句柄							  输入参数
|		  5  BOOL				bAutoParam	是否启用参数自动读写功能				  输入参数	
\*--------------------------------------------------------------------*/
CG_API DeviceStatus DeviceInitEx(HDEVICE hDevice, ReceiveFrameProc *pFun, LPVOID lParam, HWND hWndDisplay, BOOL bAutoParam);

/*----------------------------------------------------------------------*\
| 功  能：反初始化相机                                                 
|                                                                     
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|                                                             
| 参  数：
|        1  HDEVICE			hDevice		相机设备句柄							  输入参数
|                                                                     
| 注  意：                  
|         用于释放内存分配空间，必须在程序退出前调用
\*----------------------------------------------------------------------*/
CG_API DeviceStatus DeviceUnInit(HDEVICE hDevice);

/*--------------------------------------------------------------------*\
| 功  能：打开视频流                                     
|                                                                      
| 返回值：调用成功返回STATUS_OK，否则返回错误代码     
|
| 参  数：                                                             
|         1  HDEVICE	hDevice		相机设备句柄		输入参数
\*--------------------------------------------------------------------*/
CG_API DeviceStatus Start(HDEVICE hDevice);

/*--------------------------------------------------------------------*\
| 功  能：停止视频流                                     
|                                                                      
| 返回值：调用成功返回STATUS_OK，否则返回错误代码     
|
| 参  数：                                                             
|         1  HDEVICE	hDevice		相机设备句柄		输入参数
\*--------------------------------------------------------------------*/
CG_API DeviceStatus Stop(HDEVICE hDevice);

/*--------------------------------------------------------------------*\
| 功  能：视频流是否打开                                     
|                                                                      
| 返回值：视频流已经打开返回TRUE，否则返回FALSE     
|
| 参  数：                                                             
|         1  HDEVICE	hDevice		相机设备句柄		输入参数
\*--------------------------------------------------------------------*/
CG_API BOOL IsReceivingData(HDEVICE hDevice);

/*---------------------------------------------------------------------------------*\
| 功  能：设置图像数据处理输出类型                                                            
|                                                                                
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                            
|                                                                                
| 参  数： 
|         1  HDEVICE				hDevice		相机设备句柄			输入参数
|         2  emDeviceISPDataType	nType		图像数据处理输出类型	输入参数                                                                
\*---------------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceSetISPDataType(HDEVICE hDevice, emDeviceISPDataType nType);

/*---------------------------------------------------------------------------------*\
| 功  能：图像数据处理                                                            
|                                                                                
| 返回值：调用成功返回RGB24图像数据首地址，否则返回NULL                             
|                                                                                
| 参  数： 
|         1  HDEVICE			hDevice		相机设备句柄							输入参数
|         2  BYTE				*pbyRAW		图像数据首地址，回调函数参数			输入参数
|         3  DeviceFrameInfo	*psFrInfo	图像帧数据信息，参见DeviceFrameInfo定义	输入参数                                                                   
\*---------------------------------------------------------------------------------*/
CG_API BYTE* DeviceISP(HDEVICE hDevice, BYTE *pbyRAW, DeviceFrameInfo *psFrInfo);

/*-----------------------------------------------------------------------------------*\
| 功  能：显示RGB24位图像数据                                                        
|                                                                                   
| 返回值：调用成功返回TRUE，否则返回FALSE                                     
|                                                                                   
| 参  数：
|         1  HDEVICE			hDevice		相机设备句柄							输入参数
|         2  BYTE				*pbRGB		RGB24图像数据首地址 					输入参数 
|         3  DeviceFrameInfo    *psFrInfo   图像帧数据信息，参见DeviceFrameInfo定义	输入参数
\*-----------------------------------------------------------------------------------*/
CG_API BOOL DeviceDisplayRGB24(HDEVICE hDevice, BYTE *pbRGB, DeviceFrameInfo *psFrInfo);

/*-------------------------------------------------------------------------*\
| 功  能：设置显示模式
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：
|		  1  HDEVICE			hDevice			相机设备句柄				输入参数
|         2  emDisplayMode		displayMode		显示模式					输入参数
|
\*--------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceSetDisplayMode(HDEVICE hDevice, emDisplayMode displayMode);

/*-------------------------------------------------------------------------*\
| 功  能：设置显示的起始偏移值
|
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|
| 参  数：
|		  1  HDEVICE	hDevice			相机设备句柄				输入参数
|         2  INT		iOffsetX		偏移的X坐标					输入参数
|         3  INT        iOffsetY		偏移的Y坐标					输入参数
|
\*--------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceSetDisplayOffset(HDEVICE hDevice, INT iOffsetX, INT iOffsetY);

/*---------------------------------------------------------------------------*\
| 功  能：设置预览图像尺寸
|           
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|       
| 参  数：         
|		  1  HDEVICE	hDevice			相机设备句柄				输入参数
|         2  INT		iWidth			预览图像宽度                输入参数          
|         3  INT		iHeight			预览图像高度                输入参数                                              
\*---------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceSetDisplaySize(HDEVICE hDevice, INT iWidth, INT iHeight);

/*---------------------------------------------------------------------------*\
| 功  能：获取一帧图像数据
|           
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|       
| 参  数：  
|		  1  HDEVICE			hDevice		相机设备句柄							输入参数
|         2  BYTE				**pbyBuffer	图像数据保存的首地址					输入参数
|         3  DWORD				wTimes		抓取图像的超时时间,单位毫秒。			输入参数
|         4  DeviceFrameInfo    *psFrInfo   图像帧数据信息，参见DeviceFrameInfo定义	输出参数
\*---------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceGetImageBuffer(HDEVICE hDevice, BYTE **pbyBuffer, DWORD wTimes, DeviceFrameInfo *psFrInfo);


/*---------------------------------------------------------------------------*\
| 功  能：获取一帧图像数据(该接口获得的图像是经过处理后的ISP数据格式)
|           
| 返回值：成功时，返回数据缓冲区的首地址，否则返回NULL。
|       
| 参  数：       
|		  1  HDEVICE			hDevice		相机设备句柄							输入参数
|         2  DWORD				wTimes		抓取图像的超时时间,单位毫秒。			输入参数           
|         3  DeviceFrameInfo    *psFrInfo   图像帧数据信息，参见DeviceFrameInfo定义	输出参数
\*---------------------------------------------------------------------------*/
CG_API BYTE* DeviceGetImageBufferEx(HDEVICE hDevice, DWORD wTimes, DeviceFrameInfo *psFrInfo);

/*---------------------------------------------------------------------------*\
| 功  能：获得一帧图像数据(该接口获得的图像是经过处理后的ISP数据格式)
|           
| 返回值：调用成功返回STATUS_OK，否则返回错误代码
|       
| 参  数：         
|		  1  HDEVICE			hDevice				相机设备句柄							输入参数
|         2  BYTE				*pImageData			接收图像数据的缓冲区					输入参数
|		  3  UINT				wTimes				抓取图像的超时时间,单位毫秒。			输入参数 
|         4  DeviceFrameInfo    *psFrInfo			图像帧数据信息，参见DeviceFrameInfo定义	输出参数
\*---------------------------------------------------------------------------*/
CG_API DeviceStatus DeviceGetImageBufferEx2(HDEVICE hDevice, BYTE* pImageData, UINT wTimes, DeviceFrameInfo *psFrInfo);

/*-----------------------------------------------------------------------------------*\
| 功  能：采集单帧或多帧图像到内存                                                        
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                    
|                                                                                   
| 参  数：
|         1  HDEVICE			hDevice		相机设备句柄							输入参数
|         2  BYTE				*pbyRAW		图像数据保存的首地址					输入参数 
|         3  UINT				nFrames		采集图像的帧数							输入参数
|         4  DeviceFrameInfo    *psFrInfo   图像帧数据信息，参见DeviceFrameInfo定义	输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SnapShot(HDEVICE hDevice, BYTE *pbyRAW, UINT nFrames, DeviceFrameInfo *psFrInfo);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取相机设备参数范围                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                      
|                                                                                   
| 参  数：
|         1  HDEVICE			hDevice			相机设备句柄			输入参数
|         2  DeviceCapability	*pDeviceInfo	相机设备参数范围结构	输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetCapability(HDEVICE hDevice, DeviceCapability *pDeviceInfo);


/*-----------------------------------------------------------------------------------*\
| 功  能：获取相机设备最大图像范围大小                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                      
|                                                                                   
| 参  数：                      
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
|         2  INT		*piWidth		最大宽度				输出参数
|         3  INT		*piHeight		最大高度				输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetCapabilityImageSize(HDEVICE hDevice, INT *piWidth, INT *piHeight);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取相机设备当前图像大小                                                 
|                                                                                  
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                  
|                                                                                  
| 参  数：     
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
|         1  INT		*piWidth		宽度					输出参数
|         2  INT		*piHeight		高度					输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetImageSize(HDEVICE hDevice, INT *piWidth, INT *piHeight);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取相机预设分辨率数据                                                       
|                                                                                   
| 返回值：调用成功返回预设分辨率的数量，否则返回0                                      
|                                                                                   
| 参  数：
|         1  HDEVICE			hDevice			相机设备句柄					输入参数
|         2  LPDeviceResolutionItem	lpDevRes	指向相机分辨率描述结构的指针	输出参数
\*-----------------------------------------------------------------------------------*/
CG_API int GetResolutions(HDEVICE hDevice, LPDeviceResolutionItem lpDevRes = NULL);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置相机当前分辨率                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE			hDevice	相机设备句柄		输入参数
|         2  ResolutionParam	param	相机分辨率模式参数	        输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetResolution(HDEVICE hDevice, ResolutionParam param);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取相机当前分辨率                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE			hDevice	相机设备句柄					输入参数
|         2  ResolutionParam	*pParam	指向相机分辨率模式结构的指针	输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetResolution(HDEVICE hDevice, ResolutionParam *pParam);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置输出图像数据的类型                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：    
|         1  HDEVICE		hDevice		相机设备句柄		输入参数
|         2  DeviceDataType	dataType	图像数据类型		输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetMediaType(HDEVICE hDevice, emDeviceDataType dataType);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取输出图像数据的类型                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE		hDevice		相机设备句柄		输入参数
|         2  DeviceDataType	*pDataType	图像数据类型		输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetMediaType(HDEVICE hDevice, emDeviceDataType *pDataType);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置帧率                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE		hDevice	相机设备句柄			输入参数
|         2  DeviceSpeed	dSpeed	帧率					输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetFrameSpeed(HDEVICE hDevice, emDeviceFrameSpeed dSpeed);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取帧率                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE			hDevice	相机设备句柄		输入参数
|         2  ResolutionParam	*pParam	帧率				输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetFrameSpeed(HDEVICE hDevice, emDeviceFrameSpeed *pdSpeed);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置速度微调                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|		  1  HDEVICE		hDevice		相机设备句柄				输入参数 
|         2  FLOAT			fTune		微调系数（范围0.0 - 1.0）	输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetFrameSpeedTune(HDEVICE hDevice, FLOAT fTune);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取速度微调                                                      
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：         
|		  1  HDEVICE			hDevice		相机设备句柄				输入参数
|         2  FLOAT				*pfTune		微调系数（范围0.0 - 1.0）	输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetFrameSpeedTune(HDEVICE hDevice, FLOAT *pfTune);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置触发模式                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE			hDevice	相机设备句柄			输入参数
|         2  DeviceCaptureMode	mode	触发模式				输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetCaptureMode(HDEVICE hDevice, emDeviceCaptureMode mode);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取触发模式                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE			hDevice		相机设备句柄			输入参数
|         2  DeviceCaptureMode	*pCapMode	触发模式				输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetCaptureMode(HDEVICE hDevice, emDeviceCaptureMode  *pCapMode);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置软触发一次帧数                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE	hDevice	相机设备句柄			输入参数
|         2  UINT		nFrame	软触发一次帧数			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetSoftTriggerFrameNumber(HDEVICE hDevice, UINT nFrame);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取软触发一次帧数                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
|         2  UINT		*pFrameNum		软触发一次帧数			输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetSoftTriggerFrameNumber(HDEVICE hDevice, UINT *pFrameNum);

/*-----------------------------------------------------------------------------------*\
| 功  能：软触发一次                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码       
|                                                                                   
| 参  数： 
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SoftTriggerOnce(HDEVICE hDevice);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置曝光时间                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
|         2  USHORT		usExposureTime	曝光时间				输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetExposureTime(HDEVICE hDevice, USHORT usExposureTime);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取曝光时间                                                        
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice				相机设备句柄		输入参数
|         2  USHORT		*pusExposureTime	曝光时间			输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetExposureTime(HDEVICE hDevice, USHORT *pusExposureTime);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置自动曝光目标值                                                      
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
|         2  BYTE		byTarget		自动曝光目标值			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetAutoExposureTarget(HDEVICE hDevice, BYTE byTarget);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取自动曝光目标值                                                       
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice		相机设备句柄			输入参数
|         1  BYTE		*pbyTarget	自动曝光目标值			输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetAutoExposureTarget(HDEVICE hDevice, BYTE *pbyTarget);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置各色彩通道的增益                                                      
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
|         2  UCHAR		RGain			红通道增益			输入参数
|         3  UCHAR		GGain			绿通道增益			输入参数
|         4  UCHAR		BGain			蓝通道增益			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetGain(HDEVICE hDevice, UCHAR RGain, UCHAR GGain, UCHAR BGain);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取各色彩通道的增益                                                      
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE		hDevice		相机设备句柄		输入参数
|         1  UCHAR			*pRGain		红通道增益			输出参数
|         2  UCHAR			*pGGain		绿通道增益			输出参数
|         3  UCHAR			*pBGain		蓝通道增益			输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetGain(HDEVICE hDevice, UCHAR *pRGain, UCHAR *pGGain, UCHAR *pBGain);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置曝光增益                                                      
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice		相机设备句柄		输入参数
|         1  UCHAR		uGain		曝光增益			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetAnalogGain(HDEVICE hDevice, UCHAR uGain);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取曝光增益                                                      
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice		相机设备句柄		输入参数
|         2  UCHAR		*pGain		曝光增益			输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetAnalogGain(HDEVICE hDevice, UCHAR *pGain);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置伽玛状态                                                      
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice		相机设备句柄	输入参数
|         2  BOOL		bEnable		状态			输入参数
| 注意： 当bEnable为FALSE时，SetGamma函数设置的值会被忽略
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetGammaState(HDEVICE hDevice, BOOL bEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取伽玛状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE		hDevice		相机设备句柄		输入参数
|         1  BOOL			*pEnable	状态				输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetGammaState(HDEVICE hDevice, BOOL *pEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置伽玛值                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：  
|         1  HDEVICE	hDevice		相机设备句柄	输入参数
|         2  FLOAT		fGamma		伽玛值			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetGamma(HDEVICE hDevice, FLOAT fGamma);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取伽玛值                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice		相机设备句柄		输入参数
|         2  FLOAT		*pfGamma	伽玛值				输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetGamma(HDEVICE hDevice, FLOAT *pfGamma);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置亮度状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
|         2  BOOL		bEnable			状态				输入参数
| 注意： 当bEnable为FALSE时，SetBright函数设置的值会被忽略
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetBrightState(HDEVICE hDevice, BOOL bEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取亮度状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE	hDevice			相机设备句柄	输入参数
|         1  BOOL 		*pEnable		状态			输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetBrightState(HDEVICE hDevice, BOOL *pEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置亮度值                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE		hDevice		相机设备句柄			输入参数
|         2  SHORT			shBright	亮度值					输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetBright(HDEVICE hDevice, SHORT shBright);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取亮度值                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE		hDevice		相机设备句柄		输入参数
|         2  SHORT			*pshBright	亮度值				输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetBright(HDEVICE hDevice, SHORT *pshBright);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置对比度状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE		hDevice			相机设备句柄		输入参数
|         2  BOOL			bEnable			状态				输入参数
| 注意： 当bEnable为FALSE时，SetContrast函数设置的值会被忽略
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetContrastState(HDEVICE hDevice, BOOL bEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取对比度状态                                                    
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE		hDevice		相机设备句柄	输入参数
|         2  BOOL			*pEnable	状态			输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetContrastState(HDEVICE hDevice, BOOL *pEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置对比度值                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE		hDevice		相机设备句柄		输入参数
|         2  SHORT			shContrast	对比度值			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetContrast(HDEVICE hDevice, SHORT shContrast);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取对比度值                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE		hDevice			相机设备句柄			输入参数
|         2  SHORT			*pshContrast	对比度值				输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetContrast(HDEVICE hDevice, SHORT *pshContrast);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置饱和度状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE		hDevice			相机设备句柄		输入参数
|         2  BOOL			bEnable			状态				输入参数
| 注意： 当bEnable为FALSE时，SetSaturation函数设置的值会被忽略
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetSaturationState(HDEVICE hDevice, BOOL bEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取饱和度状态                                                   
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice		相机设备句柄		输入参数
|         2  BOOL		*pEnable	状态				输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetSaturationState(HDEVICE hDevice, BOOL *pEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置饱和度值                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
|         2  BYTE		bySaturation	饱和度值				输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetSaturation(HDEVICE hDevice, BYTE bySaturation);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取饱和度值                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
|         2  BYTE		*pbySaturation	饱和度值			输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetSaturation(HDEVICE hDevice, BYTE *pbySaturation);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置图像翻转状态                                                    
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE			hDevice			相机设备句柄					输入参数
|         2  emMirrorDirection	mDir			图像翻转方向标识（水平或垂直）	输入参数
|         3  BOOL				bEnable			图像翻转状态					输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetMirror(HDEVICE hDevice, emMirrorDirection mDir, BOOL bEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取图像翻转状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE			hDevice		相机设备句柄					输入参数
|         2  MirrorDirection	mDir		图像翻转方向标识（水平或垂直）	输入参数
|         3  BOOL				*pEnable	图像翻转状态					输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetMirror(HDEVICE hDevice, emMirrorDirection mDir, BOOL *pEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置图像单色状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice		相机设备句柄		输入参数
|         2  BOOL		bEnable		图像单色状态		输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetMonochrome(HDEVICE hDevice, BOOL bEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取图像单色状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice		相机设备句柄		输入参数
|         2  BOOL		*pEnable	图像单色状态		输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetMonochrome(HDEVICE hDevice, BOOL *pEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置图像反色状态                                                    
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
|         2  BOOL		bEnable			图像反色状态			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetInverse(HDEVICE hDevice, BOOL bEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取图像反色状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
|         2  BOOL		*pEnable		伽玛值				输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetInverse(HDEVICE hDevice, BOOL *pEnable);

/*-----------------------------------------------------------------------------------*\
| 功  能：一次白平衡功能                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码   
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetOnceWBalace(HDEVICE hDevice);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置自动曝光状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
|         2  BOOL		bAEState		自动曝光状态		输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetAutoExposureState(HDEVICE hDevice, BOOL bAEState);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取自动曝光状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
|         2  BOOL		*pAEState		自动曝光状态		输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetAutoExposureState(HDEVICE hDevice, BOOL *pAEState);

/*-----------------------------------------------------------------------------------*\
| 功  能：设置自动白平衡状态                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
|         2  BOOL		bAWBState		自动白平衡状态			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SetAutoWBalaceState(HDEVICE hDevice, BOOL bAWBState);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取自动白平衡状态                                                      
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：
|         1  HDEVICE	hDevice			相机设备句柄		输入参数
|         2  BOOL		*pAWBState		伽玛值				输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetAutoWBalaceState(HDEVICE hDevice, BOOL *pAWBState);

/*-----------------------------------------------------------------------------------*\
| 功  能：保存当前图像到文件                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数： 
|         1  HDEVICE		hDevice			相机设备句柄			输入参数
|         2  LPCTSTR		lpszFileName	图像文件路径和名称		输入参数
|         3  emDSFileType	type			图像文件类型            输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus CaptureFile(HDEVICE hDevice, LPCTSTR lpszFileName, emDSFileType type);

/*-----------------------------------------------------------------------------------*\
| 功  能：开启录影功能                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：  
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
|         2  LPCTSTR	lpszFileName	视频文件的路径和名称	输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus StartRecordVideo(HDEVICE hDevice, LPCTSTR lpszFileName);

/*-----------------------------------------------------------------------------------*\
| 功  能：停止录影功能                                                    
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码  
|                                                                                   
| 参  数：  
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus StopRecordVideo(HDEVICE hDevice);

/*-----------------------------------------------------------------------------------*\
| 功  能：读出相机默认出厂参数设置                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码   
|                                                                                   
| 参  数：  
|         1  HDEVICE	hDevice			相机设备句柄			输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus LoadDefaultParameter(HDEVICE hDevice);

/*-----------------------------------------------------------------------------------*\
| 功  能：获取当前使用的保存参数组别                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：        
|         1  HDEVICE		hDevice			相机设备句柄			输入参数
|         2  ParameterTeam	*pTeam			保存参数的组别（共4组）	输出参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus GetCurrentParameterTeam(HDEVICE hDevice, emParameterTeam *pTeam);

/*-----------------------------------------------------------------------------------*\
| 功  能：读取相机的参数设置，并使其生效                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：     
|         1  HDEVICE		hDevice			相机设备句柄			输入参数
|         2  ParameterTeam	eTeam			保存参数的组别（共4组）	输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus ReadParameter(HDEVICE hDevice, emParameterTeam eTeam);

/*-----------------------------------------------------------------------------------*\
| 功  能：保存相机的参数设置                                                     
|                                                                                   
| 返回值：调用成功返回STATUS_OK，否则返回错误代码                                       
|                                                                                   
| 参  数：      
|         1  HDEVICE		hDevice			相机设备句柄			输入参数
|         2  ParameterTeam	eTeam			保存参数的组别（共4组）	输入参数
\*-----------------------------------------------------------------------------------*/
CG_API DeviceStatus SaveParameter(HDEVICE hDevice, emParameterTeam eTeam);


#endif