#ifndef DATATYPE_H
#define DATATYPE_H

/*-------------------------------------------------------------
	返回值定义
  *===========================================================*/
#define		ResSuccess					0x0000		// 返回成功
#define		ResNullHandleErr			0x0001		// 无效句柄
#define		ResNullPointerErr			0x0002		// 指针为空
#define		ResFileOpenErr				0x0003
#define		ResNoDeviceErr				0x0004
#define		ResInvalidParameterErr		0x0005
#define		ResOutOfMemoryErr			0x0006
#define		ResNoPreviewRunningErr		0x0007		// 预览没有开启
#define		ResOSVersionErr				0x0008
#define		ResUsbNotAvailableErr		0x0009
#define		ResNotSupportedErr			0x000a
#define		ResNoSerialString			0x000b
#define		ResVerificationErr			0x000c
#define		ResTimeoutErr	            0x000d
#define		ResScaleModeErr				0x000f
#define		ResUnknownErr				0x00ff

#define     ResFail						0x0010
#define		ResDisplayWndExist			0x0011		// 应该关闭预览窗口
#define		ResAllocated				0x0012		// 内存已经分配
#define		ResAllocateFail				0x0013		// 内存分配失败
#define		ResReadError				0x0014      // USB读取失败
#define		ResWriteError				0x0015		// USB命令发出失败
#define		ResUsbOpen					0x0016      // USB端口已经打开
#define     ResCreateStreamErr			0x0017		// 创建avi流失败
#define     ResSetStreamFormatErr		0x0018		// 设置AVI流格式失败

typedef struct _tagDLVIDEORECT
{
	int     Left;		// 相对于父窗口的水平偏移
	int     Top;		// 相对于父窗口的垂直偏移
	int     Width;		// 视频窗口宽度
	int     Height;		// 视频窗口高度
}DLVIDEORECT, *PDLVIDEORECT;

/*-------------------------------------------------------------
	摄像头相关参数结构
  *===========================================================*/
struct CapInfoStruct 
{
	UCHAR	*Buffer;		// 用户分配，用于返回原始数据8bit
	ULONG	Height;			// 采集高度
	ULONG	Width;			// 采集宽度
	ULONG	OffsetX;		// 水平偏移
	ULONG	OffsetY;		// 垂直偏移
	ULONG	Exposure;		// 曝光值 1-500MS
	UCHAR	Gain[3];		// R G B 增益 1-63
	UCHAR	Control;		// 控制位
	UCHAR	InternalUse;	// 用户不要对此字节进行操作
	UCHAR	ColorOff[3];	// 用户从外面不要改变此数组的值，要调用EnableColorOffset来改变
	UCHAR	Reserved[4];	// 保留位
};


/*-----------------------------------------------------------
	Control 控制位说明
	BIT7       BIT6      BIT5     BIT4     BIT3     BIT2     BIT1     BIT0
	UV         IR        VB       RESERVED RESERVED SENSE1   SENSE0   Camera Switch(Golden Eagle only)
  ===========================================================*/

enum     DLPARAM {  BWSHOW = 0,		// 是否黑白显示
					RAWDATASHOW,	// 是否直接显示原始数据
					CLEARVIEW,		// 是否利用优化算法进行转换
					UPDOWN,			// 上下翻转
	};

#endif