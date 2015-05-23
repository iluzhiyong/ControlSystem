/*---------------------------------------------------------------------------------*\
|						SDK当前版本号 version 1.0.1.3/Jan/2015
\*---------------------------------------------------------------------------------*/
#ifndef _DEVICEDEFINE_H_
#define _DEVICEDEFINE_H_


#include "DVP_DeviceParamDef.h"

/*---------------------------------------------------------------------------------*\
|                    检测状态值是否成功（非负数表示成功）
\*---------------------------------------------------------------------------------*/

#ifndef SUCCEEDED
#define SUCCEEDED(Status)     ((HRESULT)(Status) >= 0)
#endif

/*---------------------------------------------------------------------------------*\
|                    检测状态值是否失败（负数表示失败）
\*---------------------------------------------------------------------------------*/

#ifndef FAILED
#define FAILED(Status)		  ((HRESULT)(Status) < 0)
#endif

/*---------------------------------------------------------------------------------*\
|                                    错误码定义
\*---------------------------------------------------------------------------------*/

typedef enum{
    STATUS_OK                     = 0,       // 动作成功
	STATUS_FAILED				  = -1,		 // 操作失败	
    STATUS_INTERNAL_ERROR         = -2,      // 内部错误
    STATUS_NOT_SUPPORTED          = -3,      // 设备不支持
    STATUS_NOT_INITIALIZED        = -4,      // 初始化错误
    STATUS_PARAMETER_INVALID      = -5,      // 无效的参数
	STATUS_THREAD_WORKING		  = -6,		 // 线程正在运行
	STATUS_CHECK_ENCRYPT_ERROR	  = -7,	     // 检查加密错误
	STATUS_CHECK_ENCRYPT_LOST	  = -8,		 // 加密信息丢失
	STATUS_INVALIDATE_HANDLE	  = -9,		 // 无效的设备句柄
	STATUS_CAPMODE_NOT_SUPPORTED  = -10,	 // 当前触发模式不支持
	STATUS_BAD_FRAME			  = -11,	 // 采集图像出现坏帧
	STATUS_RECEIVING_STOP		  = -12,	 // 停止采集图像
	STATUS_RECEIVING_SWITCH		  = -13,     // 采集切换中请稍候重试
    STATUS_TIME_OUT               = -1000,   // 设备超时
    STATUS_IO_ERROR               = -1001,   // 设备IO错误
    STATUS_DEVICE_NOT_DETECTED    = -1100,   // 检测不到设备
	STATUS_NOTCONNECT_ENCRYPTION  = -1101,	 // 连接加密设备错误
    STATUS_DEVICE_IS_OPENED       = -1102,   // 设备已经打开
	STATUS_DEVICE_IS_CLOSED		  = -1103,   // 设备已经关闭
    STATUS_MEMORY_NOT_ENOUGH      = -1200,   // 没有足够的内存
} DeviceStatus;

/*---------------------------------------------------------------------------------*\
|                                   打开相机的方式
\*---------------------------------------------------------------------------------*/
typedef enum
{
	OF_BYPOSITION = 0x00,		// 通过位置序列打开
	OF_BYUSBADDRESS,			// 通过USB地址打开
	OF_USERSERIALNUMBER,		// 通过指定用户序列号打开
}emOpenDeviceFlag;


/*---------------------------------------------------------------------------------*\
|                                  相机图像数据类型
\*---------------------------------------------------------------------------------*/
typedef enum
{
	// BAYER 数据类型
	DATA_TYPE_MONO8			  = 0,
	DATA_TYPE_MONO10          = 1,
	DATA_TYPE_MONO12          = 2,
    DATA_TYPE_BAYGR8          = 10,
	DATA_TYPE_BAYGR10         = 11,
	DATA_TYPE_BAYGR12         = 12,
	DATA_TYPE_UNKNOWN		  = -1,
}emDeviceDataType;

/*---------------------------------------------------------------------------------*\
|                                 图像数据处理输出类型
\*---------------------------------------------------------------------------------*/
typedef enum
{
	DATA_ISP_RGB24			= 0x00,
	DATA_ISP_RGB32,
	DATA_ISP_MON8,
}emDeviceISPDataType;

/*---------------------------------------------------------------------------------*\
|                                   相机触发模式
\*---------------------------------------------------------------------------------*/
typedef enum
{
	CONTINUE_CAPTURE_MODE = 0x00,	// 连续模式
	SOFT_TRIGGER_MODE,				// 软触发模式
	HARD_TRIGGER_MODE,				// 硬件触发模式
	SOFT_TRIGGER_RESERVERD_MODE,	// 软触发（预留）模式
	HARD_TRIGGER_RESERVERD_MODE,	// 硬件触发（预留）模式
	UNDEFINED_MODE,					// 未知模式
}emDeviceCaptureMode;


/*---------------------------------------------------------------------------------*\
|                               图像传感器采样模式
\*---------------------------------------------------------------------------------*/
typedef enum
{
	SUBSAMPLE_MODE_ROI = 0x00,
    SUBSAMPLE_MODE_BIN,
    SUBSAMPLE_MODE_SKIP,
	SUBSAMPLE_MODE_SUM
}emDeviceSubsampleMode;

/*---------------------------------------------------------------------------------*\
|                                   相机帧速定义
\*---------------------------------------------------------------------------------*/
typedef enum
{
	HIGHEST_SPEED = 0x00,		// 超高速
	HIGH_SPEED,					// 高速
	LOW_SPEED,					// 低速
	LOWEST_SPEED,				// 超低速
}emDeviceFrameSpeed;

/*---------------------------------------------------------------------------------*\
|                                   图像文件的格式
\*---------------------------------------------------------------------------------*/
typedef enum
{
    FILE_JPG  = 0,
    FILE_PNG  = 1,
	FILE_BMP  = 2,
	FILE_RAW  = 3,
}emDSFileType;

/*---------------------------------------------------------------------------------*\
|                                   图像镜像方向定义
\*---------------------------------------------------------------------------------*/
typedef enum
{
	MD_HORIZONTAL	= 0x00,
	MD_VERTICAL		= 0x01,
}emMirrorDirection;

/*---------------------------------------------------------------------------------*\
|                                内部显示接口的显示方式定义
\*---------------------------------------------------------------------------------*/
typedef enum
{
	DISPLAYMODE_SCALE = 0x00,
	DISPLAYMODE_REAL,
}emDisplayMode;

/*---------------------------------------------------------------------------------*\
|                                分辨率类型定义
\*---------------------------------------------------------------------------------*/
typedef enum
{
	RT_PRESET	= 0x00,
	RT_ROI,
}emResolutionType;

/*---------------------------------------------------------------------------------*\
|                               相机参数设置保存组别
\*---------------------------------------------------------------------------------*/
typedef enum
{
	PARAMETER_TEAM_DEFAULT  = 255,
    PARAMETER_TEAM_A        = 0,
    PARAMETER_TEAM_B        = 1,
    PARAMETER_TEAM_C        = 2,
    PARAMETER_TEAM_D        = 3,
	PARAMETER_TEAM_NUM		= 5,
}emParameterTeam;

/*---------------------------------------------------------------------------------*\
|                                 相机数据回调函数
\*---------------------------------------------------------------------------------*/
typedef void CALLBACK  ReceiveFrameProc( LPVOID pDevice, BYTE *pImageBuffer, DeviceFrameInfo *pFrInfo, LPVOID	lParam);

#endif