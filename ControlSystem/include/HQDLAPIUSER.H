#ifndef _HQDLAPIUSER_H
#define _HQDLAPIUSER_H

#include "DataType.h"

#ifdef HQDLAPI_EXPORTS
#define HQDL_API extern "C" __declspec(dllexport) int WINAPI
#else
#define HQDL_API extern "C" __declspec(dllimport) int WINAPI
#endif


/*==============================================================
	回调函数定义
  *-------------------------------------------------------------*/
typedef VOID (WINAPI *DL_AUTOCALLBACK )( DWORD   dw1, LPVOID lpContext );
typedef VOID (WINAPI *DL_FRAMECALLBACK)( LPVOID lpParam1, LPVOID lpPoint, LPVOID lpContext );

/*==============================================================
    2005.12.13
	库说明:	V1.0.0.1对于所有华旗数码实验室的摄像头提供统一接口
			对于功能最强大的Golden Hawk提供所有可操作接口
			对于其他简化版摄像头有些接口不支持,
			比如对于White Hawk不支持HqDLBitOperation
			对于黑白摄像头不支持HqDLSetDoAWB,并且
			对于黑白摄像头,用原始数据预览并采集是标准的方式,但黑白摄像头也可以用
			彩色摄像头的方式操作.
    2005.12.19
	库说明:	V1.0.0.3加入噪点检测函数HqDLNoiseCalibration
			加入位置调节函数HqDLSizeCalibration
    2006.1.4
	库说明:	V1.0.0.4加入控制电机HqDLAutoFocus及光源函数HqDLLedControl
    2006.1.18
	库说明:	V1.0.0.6加入avi录影HqDLCaptureToAvi及自动调焦功能HqDLSetDoAF

    2006.1.18
	库说明:	V1.0.0.7修改了avi录影HqDLCaptureToAvi函数，当要停止录影时，
			录影关闭时，文件名可以为空
			修改了预览窗口不能创建的错误
    2006.4.18
	库说明:	V1.0.0.8加入了直接保存JPEG图像功能,HqDLGetRgbFrameToJpeg
    2006.7.13
	库说明:	V1.0.0.9加入了设置avi采集帧数的设置HqDLCaptureToAviEx
    2006.7.25
	库说明:	V1.0.0.10加入了对多个摄像头的控制功能
			HqDLGetTotalDeviceNum用于取得PC连接的摄像头数
			HqDLGetDeviceID用于得到指定摄像头的ID号，用于与其他摄像头区分

  ==============================================================*/


 /*==============================================================
	函数:	初始化设备，返回摄像头句柄，用于其它函数的调用
	功能:

	参数:	pIndex			返回摄像头索引
			pFilterName		保留
			pCapInfo		初始化摄像头参数   
	说明:   必须要给pCapInfo->Buffer分配足够的内存空间，一般要分配比最大的采集空间要大一些
			推荐用MAXWIDTH*MAXHEIGHT+512，用于传递采集数据
  --------------------------------------------------------------*/
HQDL_API 
HqDLInitialize(	IN  LPCTSTR			pFilterName,
				OUT int				*pIndex,	
				IN  CapInfoStruct	*pCapInfo,
				OUT HANDLE			*hCamera
				);	
/*==============================================================
	函数:	HqDLStartView
	功能:	打开预览窗口,并启动视频流
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLStartView( IN HANDLE	hCamera, 
			  IN LPCTSTR	lpszWindowName, 
			  IN DWORD		dwStyle, 
			  IN int		x, 
			  IN int		y, 
			  IN int		nWidth, 
			  IN int		nHeight, 
			  IN HWND		hwndParent, 
			  IN HMENU		nIDorHMenu
			  );
/*==============================================================
	函数:	HqDLPauseView
	功能:	停止视频流
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLStopView( IN HANDLE  hCamera );

/*==============================================================
	函数:	HqDLPauseView
	功能:	暂停视频流
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLPauseView( IN HANDLE hCamera, IN BOOL bPause );


/*==============================================================
	函数:	HqDLUninitialize
	功能:	反初始化设备
	参数:   
	说明:   必须在程序退出时调用,用于释放内存分配空间
  --------------------------------------------------------------*/
HQDL_API
HqDLUninitialize( IN HANDLE  *hCamera );


/*==============================================================
	函数:	HqDLSetCapInfo
	功能:	实时设置视频参数
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLSetCapInfo( IN HANDLE hCamera , IN CapInfoStruct  *pCapInfo );

/*==============================================================
	函数:	HqDLGetFrameRate
	功能:	得到摄像头的当前帧率
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLGetFrameRate( IN HANDLE	hCamera, OUT float *pfFrameRate );

/*==============================================================
	函数:	HqDLGetOneFrame
	功能:	采集一帧原始数据到pCapInfo->Buffer中
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRawFrame(IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo);

/*==============================================================
	函数:	HqDLGetRgbFrame
	功能:	采集一帧24bitRGB数据到pDest中，同时原始数据会保存在pCapInfo->Buffer中
			pDest中的结果会根据CAMPARAM中的设定而改变，如果设置bClearView,pDest中是经过
			优化的转换算法而得到24bit的结果，如果设置bBW,那么是转换为8bit的灰度图像数据
			如果设置UPDOWN则图像数据会做相应的垂直翻转。
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRgbFrame(IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest);

/*==============================================================
	函数:	HqDLGetPausedFrame
	功能:	根据当前暂停的视频，得到暂停的视频数据
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetPausedFrame(IN HANDLE hCamera, OUT BYTE* pDest);

/*==============================================================
	函数:	HqDLGetPausedFrameToBmp
	功能:	将当前暂停视频保存为BMP文件
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetPausedFrameToBmp(IN HANDLE hCamera, OUT BYTE* pDest, LPCTSTR strFileName );



/*==============================================================
	函数:	HqDLGetRawFrameToBmp
	功能:	采集一帧原始数据到pCapInfo->Buffer中
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRawFrameToBmp(IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, LPCTSTR  strFileName );

/*==============================================================
	函数:	HqDLGetRgbFrameToBmp
	功能:	采集一帧图像到文件，文件格式会根据CAMPARAM中的设定改变。
			如果设置bClearView,pDest中是经过优化的转换算法而得到24bit的结果，
			如果设置bBW,那么是转换为8bit的灰度图像数据
			如果设置UPDOWN则图像数据会做相应的垂直翻转。
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRgbFrameToBmp( IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest, LPCTSTR  strFileName ); 


/*==============================================================
	函数:	HqDLGetRgbFrameToJpeg
	功能:	采集一帧图像到Jpeg文件，文件格式会根据CAMPARAM中的设定改变。
			如果设置bClearView,pDest中是经过优化的转换算法而得到24bit的结果，
			如果设置bBW,那么是转换为8bit的灰度图像数据
			如果设置UPDOWN则图像数据会做相应的垂直翻转。

	参数:   nQuality 为压缩质量 1 - 100
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRgbFrameToJpeg( IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest, LPCTSTR  strFileName, int nQuality ); 

/*==============================================================
	函数:	HqDLSetParam
	功能:	设置图像参数
	参数:   DLParam定义如下：
					BWSHOW = 0,		// 是否黑白显示
					RAWDATASHOW,	// 是否直接显示原始数据
					CLEARVIEW,		// 是否利用优化算法进行转换
					PAUSE,			// 是否暂停视频
					COLOROFFSET,	// 是否进行颜色偏移调节
					UPDOWN,			// 上下翻转
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLSetParam(IN HANDLE hCamera, DLPARAM  DLParam, ULONG  Value );


/*==============================================================
	函数:	HqDLSetGammaValue
	功能:	加入Gamma校正功能
	参数:   fGammaValue 推荐范围 1 - 200
	说明:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLSetGammaValue( IN HANDLE hCamera, IN float fGammaValue, IN BOOL bGammaOn);

/*==============================================================
	函数:	HqDLSetFrameCallback
	功能:	用于鼠标在预览窗口上点击的回调，lpParam为用户上下文
	参数:   pfFrameCallback( LPVOID lpParam1, LPVOID lpPoint, LPVOID lpContext )
			lpParam1 - 指向显示数据指针
			lpPoint  - 鼠标点击图像位置
			lpContext- 上下文
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLSetFrameCallback( IN HANDLE hCamera, DL_FRAMECALLBACK pfFrameCallback, LPVOID lpContext );

/*==============================================================
	函数:	HqDLEnableColorOffset
	功能:	进行颜色偏移量的调节
	参数:	nRedOffset    -255 ~ 255   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLEnableColorOffset( IN HANDLE hCamera, IN int nRedOffset, IN int nGreenOffset, IN int nBlueOffset, IN BOOL bColorOffset );


/*==============================================================
	函数:	HqDLBitOperation
	功能:	对于Golden Hawk的特殊操作
	参数:	保密,调用此函数时不允许开启视频
	说明:  
  --------------------------------------------------------------*/
HQDL_API 
HqDLBitOperation( IN HANDLE hCamera, IN BYTE Out, OUT BYTE *pIn, IN UCHAR Command );


/*==============================================================
	函数:	HqDLBitOperationX
	功能:	IO端口操作
	参数:	
			Out[15:8] 为控制IO端口的方向 0-INPUT 1-OUTPUT
			Out[7:0]  为输出的值
			pIn为IO口的状态

	说明:   在视频传输过程中不要调用
  --------------------------------------------------------------*/
HQDL_API 
HqDLBitOperationX( IN HANDLE hCamera, IN USHORT Out, OUT BYTE *pIn );


/*==============================================================
	函数:	HqDLRegOperation
	功能:	保留
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLRegOperation( IN HANDLE hCamera, IN BYTE RegAddress, IN OUT USHORT* Value, IN BOOL bWrite);


/*==============================================================
	函数:	
	功能:	
	参数:   pfAWBCallback 指定AWB结束后的回调函数指针
			VOID (WINAPI *DL_AUTOCALLBACK )( DWORD dw1, LPVOID lpContext );
			lpParam1	是指向AWB结果的指针，通过lpParam1来获得AWB结果如下：
			ULONG   Result = dw1;				// 0xFF202020
			BYTE btRed		= ( BYTE )(Result);				// 0x20
			BYTE btGreen	= ( BYTE )( Result>>8 );		// 0x20
			BYTE btBlue		= ( BYTE )( Result>>16 );		// 0x20
			ULONG Exposure  = ( BYTE )( Result>>24 ) * 2;	// 255 * 2 = 510 ms
			
	说明:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLSetDoAWB( IN HANDLE hCamera, IN BOOL bAWB, IN BYTE btTarget,
			  IN DL_AUTOCALLBACK pfAWBCallback,	 IN LPVOID lpContext );

/*==============================================================
	函数:	HqDLSetDoAE
	功能:	开始自动曝光操作
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLSetDoAE( IN HANDLE hCamera, IN BOOL bAE, IN BYTE btTarget, 
			 IN DL_AUTOCALLBACK pfAECallback, IN LPVOID lpContext );

/*==============================================================
	函数:	HqDLSetDoAF
	功能:	开始自动聚焦
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLSetDoAF( IN HANDLE hCamera, IN BOOL bAF );


/*==============================================================
	函数:	HqDLSetViewWin
	功能:	设置预览窗口的位置
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLSetViewWin(IN HANDLE hCamera, IN DLVIDEORECT *pRect);


/*==============================================================
	函数:	HqDLGetViewWin
	功能:	得到预览窗口的位置
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetViewWin(IN HANDLE hCamera, IN DLVIDEORECT *pRect);


/*==============================================================
	函数:	HqDLGetTotalDeviceNum
	功能:	得到共有几个摄像头
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetTotalDeviceNum(IN HANDLE hCamera, IN int *pDeviceNum );

/*--------------------------------------------------------------
	函数:	HqDLGetDeviceID
	功能:	得到此摄像头的ID号，用于对指定摄像头进行操作
	参数:   
	说明:   主要用于Multi操作
	
  =============================================================*/
HQDL_API
HqDLGetDeviceID(IN HANDLE hCamera, IN USHORT *pDeviceID );


/*==============================================================
	函数:	HqDLGetSerialsNum
	功能:	得到摄像头序列号
	参数:   
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetSerialsNum(IN HANDLE hCamera, IN char *SerialsNum );


/*==============================================================
	函数:	HqDLSizeCalibration
	功能:	专门对于采集图像位置的校正，其参数存储在硬件之中
	参数:   bWrite    = TRUE 为写操作   = FALSE 为读操作
			先为*pOffH, *pOffV, *pWidth, *pHeight共8字节赋值，并调用写操作，
			函数操作成功返回后，这8个字节会烧写进硬件存储器中，随时可以通过本函数进行读取
			当读取时，设置bWrite = FALSE, 函数操作成功返回后，*pOffH, *pOffV, *pWidth, *pHeight
			将接受读取到的8个字节
	说明:   调用此函数时，不要开启预览视频流
  --------------------------------------------------------------*/
HQDL_API
HqDLSizeCalibration(IN HANDLE hCamera, IN OUT USHORT *pOffH, IN OUT USHORT *pOffV,
				IN OUT USHORT  *pWidth, IN OUT USHORT *pHeight, BOOL bWrite );

/*==============================================================
	函数:   HqDLNoiseCalibration
	功能:	加载校正图像，主要用于去除噪点。
	参数:	bPreview   指定是否在预览时加入去噪功能
			bSave      指定是否在保存时加入去噪功能
			FileName   加载校正图像的文件名,是采集原始数据得到的256色bmp灰度图,
			当FileName = ""时,可以只设置加入去噪的时机
	说明:   可以在无视频流时调用
	注意:   加载图像的大小必须与视频采集大小相符,否则不会加载
  --------------------------------------------------------------*/
HQDL_API 
HqDLNoiseCalibration( IN HANDLE hCamera, const char*  FileName, BOOL bPreview, BOOL bSave );


/*------------------------------------------------------------------------------------------
	函数：  HqDLBrightnessCalibration
	功能：	亮度均衡

  =========================================================================================*/
HQDL_API 
HqDLBrightnessCalibration( IN HANDLE hCamera, const char*  FileName, BOOL bPreview, BOOL bSave );


/*==============================================================
	函数:	HqDLAutoFocus 
	功能:	进行聚焦调节
	参数:   Step 电机一次移动的步长，(1~255) 越大步进越大
			bFocusNear TRUE 拉近 FALSE 拉远
	说明:   GoldenEagle 特有
  --------------------------------------------------------------*/
HQDL_API 
HqDLAutoFocus( IN HANDLE hCamera, USHORT  Step, BOOL bFocusNear );

/*==============================================================
	函数:	HqDLLedControl
	功能:	进行光源亮度
	参数:   Value (0~10)
	说明:   GoldenEagle 特有
  --------------------------------------------------------------*/
HQDL_API 
HqDLLedControl( IN HANDLE hCamera, USHORT  Value );

/*==============================================================
	函数:	HqDLCaptureToAvi
	功能:	AVI录影
	参数:   bStart = TRUE 启动录影 FALSE 停止录影
	说明:   
  --------------------------------------------------------------*/
HQDL_API
HqDLCaptureToAvi( IN HANDLE hCamera, BOOL bStart, LPCTSTR  strFileName );

/*==============================================================
	函数:	HqDLCaptureX
	功能:	压缩录影
	参数:   bStart = TRUE 启动录影 FALSE 停止录影
	说明:   
			strFileName 为录影保存文件，路径必须存在
			dwRate 用于设置录影帧数
	
			停止录影调用HqDLCaptureX(m_hDevice, FALSE, "", 0 );
  -------------------------------------------------------------*/
HQDL_API
HqDLCaptureX( IN HANDLE hCamera, BOOL bStart, LPCTSTR  strFileName, DWORD dwRate );


HQDL_API
HqDLCaptureToAviEx( IN HANDLE hCamera, IN BOOL bStart, IN LPCTSTR  strFileName, IN DWORD dwRate );


#endif
