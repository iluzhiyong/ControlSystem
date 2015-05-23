#ifndef _DEVICEPARAM_DEFINE_H_
#define _DEVICEPARAM_DEFINE_H_

#define DESCR_LEN_MAX   32

/*---------------------------------------------------------------------------------*\
|                          相机列表的信息结构
\*---------------------------------------------------------------------------------*/
typedef struct tagEnumDeviceParam
{
	UCHAR	devIndex;			// 设备序列号， 用于OF_BYPOSITION方式打开相机
	UCHAR	usbAddress;			// 设备USB地址，用于OF_BYUSBADDRESS方式打开相机
	BYTE	devSN[32];			// 通过WriteSerialNumber函数写入的相机序列号，用于OF_USERSERIALNUMBER方式打开相机
	INT		index;				// 枚举到的设备序列(0 - n)
	TCHAR	lpDeviceDesc[256];	// 相机名称
}EnumDeviceParam, *LPEnumDeviceParam;

/*---------------------------------------------------------------------------------*\
|                          打开相机的参数结构
\*---------------------------------------------------------------------------------*/
typedef union tagUionOpenDeviceParam
{
	UCHAR	devIndex;			// 设备序列号， 用于OF_BYPOSITION方式打开相机
	UCHAR	usbAddress;			// 设备USB地址，用于OF_BYUSBADDRESS方式打开相机
	BYTE	devSN[32];			// 通过WriteSerialNumber函数写入的相机序列号，用于OF_USERSERIALNUMBER方式打开相机
}UionOpenDeviceParam;

/*------------------------------------------------------------------------*\
|                               图像数据类型
\*------------------------------------------------------------------------*/
typedef struct tagDeviceMediaType
{
    INT   iIndex;                        // 图像数据类型索引号，值从0开始
	TCHAR strDescription[DESCR_LEN_MAX]; // 图像数据类型的描述名称
    INT   iDataType;                     // 图像数据类型，参考“DVP_DeviceDefine.h”头文件中枚举类型“emDeviceDataType”的值
}DeviceMediaType;

/*------------------------------------------------------------------------*\
|                               帧率信息
\*------------------------------------------------------------------------*/
typedef struct tagDeviceFrameSpeed
{
    INT		iIndex;							  // 帧率索引号，值从0开始
    TCHAR	strDescription[DESCR_LEN_MAX];    // 帧率描述名称
	INT		iFrameSpeed;					  // 相机帧速定义，参考“DVP_DeviceDefine.h”头文件中枚举类型“DeviceSpeed”的值
} DeviceFrameSpeed;

/*------------------------------------------------------------------------*\
|                            ROI描述结构
\*------------------------------------------------------------------------*/
typedef struct tagDeviceROISize
{
	INT  iHOffset;		// ROI水平偏移量
    INT  iVOffset;		// ROI垂直偏移量
    INT  iWidth;		// ROI宽度
    INT  iHeight;		// ROI高度
}DeviceROISize;

/*------------------------------------------------------------------------*\
|                            分辨率描述结构
\*------------------------------------------------------------------------*/
typedef struct tagDeviceResolutionItem
{
	DeviceROISize		devSize;			// ROI结构数据
	INT					iSUBSampleMode;		// 图像传感器采样模式
}DeviceResolutionItem, *LPDeviceResolutionItem;

/*------------------------------------------------------------------------*\
|                            分辨率模式
\*------------------------------------------------------------------------*/
typedef struct tagResolutionParam
{
	struct{
		DeviceROISize	devROISize;		//ROI结构数据
		UCHAR			ucIndex;		//预设分辨率的索引值
	}dri;
	UCHAR	type;						//分辨率的类型，参考“DeviceDefine.h”头文件中枚举类型“ResolutionType”的值
}ResolutionParam;

/*------------------------------------------------------------------------*\
|                             帧数据信息
\*------------------------------------------------------------------------*/
typedef struct
{
    UINT    uiMediaType;	// 图像数据类型，参考“DeviceDefine.h”头文件中枚举类型“emDeviceDataType”的值
    UINT    uBytes;			// 图像数据字节数
	UINT    uiWidth;		// 图像宽度
    UINT    uiHeight;		// 图像高度
	BOOL	bMonochrome;	// 描述是否为单色图
    BOOL    bTriggered;		// 描述是否为触发帧
}DeviceFrameInfo;

/*------------------------------------------------------------------------*\
|                            视频尺寸范围
\*------------------------------------------------------------------------*/
typedef struct tagDeviceImageSizeRange
{
    INT   iHeightMax;		// 图像高度最大值
    INT   iHeightMin;		// 图像高度最小值
    INT   iHeightDefault;	// 图像高度默认值
    INT   iWidthMax;		// 图像宽度最大值
    INT   iWidthMin;		// 图像宽度最小值
    INT   iWidthDefault;	// 图像宽度默认值
    BOOL  bROI;				// 是否支持ROI模式。TRUE:支持；FALSE:不支持
} DeviceImageSizeRange;

/*------------------------------------------------------------------------*\
|                            软件触发一次的帧数范围
\*------------------------------------------------------------------------*/
typedef struct tagDeviceSoftTriggerFrameRange
{
	UINT	nFrameMin;		// 帧数最小值
	UINT	nFrameMax;		// 帧数最大值
	UINT	nFrameDefault;	// 帧数默认值
}DeviceSoftTriggerFrameRange;

/*------------------------------------------------------------------------*\
|                              RGB通道增益范围
\*------------------------------------------------------------------------*/
typedef struct tagDeviceRGBGainRange
{
    UCHAR  ucRGainMin;		// 红通道增益最小值
    UCHAR  ucRGainMax;		// 红通道增益最大值
    UCHAR  ucRGainDefault;	// 红通道增益默认值 
    UCHAR  ucGGainMin;		// 绿通道增益最小值
    UCHAR  ucGGainMax;		// 绿通道增益最大值
    UCHAR  ucGGainDefault;	// 绿通道增益默认值
    UCHAR  ucBGainMin;		// 蓝通道增益最小值
    UCHAR  ucBGainMax;		// 蓝通道增益最大值
    UCHAR  ucBGainDefault;	// 蓝通道增益默认值
}DeviceRGBGainRange;

/*------------------------------------------------------------------------*\
|                              增益范围
\*------------------------------------------------------------------------*/
typedef struct tagDeviceGainRange
{
	UCHAR  ucGainMin;		// 增益最小值
    UCHAR  ucGainMax;		// 增益最大值
    UCHAR  ucGainDefault;	// 增益默认值
}DeviceGainRange;

/*------------------------------------------------------------------------*\
|                            色彩饱和度数值范围
\*------------------------------------------------------------------------*/
typedef struct tagDeviceSaturationRange
{
    BYTE  iMin;				// 色彩饱和度最小值
    BYTE  iMax;				// 色彩饱和度最大值
    BYTE  iDefault;			// 色彩饱和度默认值
} DeviceSaturationRange;

/*------------------------------------------------------------------------*\
|                              图像伽玛数值范围
\*------------------------------------------------------------------------*/
typedef struct tagDeviceGammaRange
{
	FLOAT  fMin;			// 图像伽玛最小值
    FLOAT  fMax;			// 图像伽玛最大值
    FLOAT  fDefault;		// 图像伽玛默认值
}DeviceGammaRange;

/*------------------------------------------------------------------------*\
|                             图像对比度数值范围
\*------------------------------------------------------------------------*/
typedef struct tagDeviceContrastRange
{
	SHORT  shMin;			// 图像对比度最小值
    SHORT  shMax;			// 图像对比度最大值
    SHORT  shDefault;		// 图像对比度默认值
}DeviceContrastRange;

/*------------------------------------------------------------------------*\
|                             曝光时间信息
\*------------------------------------------------------------------------*/
typedef struct tagDeviceExposureRange
{
    BYTE	byTargetMin;			// 最小自动曝光目标值
    BYTE	byTargetMax;			// 最大自动曝光目标值
    BYTE	byTargetDefault;		// 默认自动曝光目标值
    USHORT  usExposureTimeMin;		// 最小曝光时间
    USHORT  usExposureTimeMax;		// 最大曝光时间
    USHORT  usExposureTimeDefault;	// 默认曝光时间
    USHORT  usExposureTimeStep;		// 曝光时间调节的步进值（最小单位）
}DeviceExposureRange;

/*------------------------------------------------------------------------*\
|                       描述相机的各种信息和参数属性。
\*------------------------------------------------------------------------*/
typedef struct tagDeviceCapability{
	DeviceImageSizeRange		sImageSizeRange;			// 图像尺寸的限定范围和默认值
	DeviceSoftTriggerFrameRange sSoftTriggerFrameRange;		// 软触发功能的限定范围和默认值
	DeviceRGBGainRange			sRGBGainRange;				// RGB增益的限定范围和默认值
	DeviceGainRange				sGainRange;					// 增益的限定范围和默认值
	DeviceSaturationRange		sSaturationRange;			// 色彩饱和度的限定范围和默认值
	DeviceGammaRange			sGammaRange;				// 图像伽玛的限定范围和默认值
	DeviceContrastRange			sContrastRange;				// 图像对比度的限定范围和默认值
	DeviceExposureRange			sExposureRange;				// 曝光功能的限定范围和默认值

	DeviceMediaType				*pMediaType;				// 相机输出的图像数据类型
	INT							iMediaType;					// 图像数据类型的数目
	INT							iMediaTypeDefault;			// 图像数据类型的默认项（索引值）

	DeviceFrameSpeed			*pFrameSpeed;				// 帧速
    INT							iFrameSpeed;				// 帧速的数目
    INT							iFrameSpeedDefault;			// 帧速的默认项（索引号）
}DeviceCapability;

#endif