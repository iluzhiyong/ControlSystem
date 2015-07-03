//#pragma once
//
////该类用于监测电机的运行状态
////当监测到电机停止时，通知相机采集图像，并处理
////但是，有的运动控制卡，是线程不安全的，即，不支持多线程
////因此，该类是否需要，今后还需要严格测试
////可以考虑，在主线程中，使用定时器，来监测电机的运行状态，定时器设置多大？不能确定
////另外，如果该线程，只读取状态，并不设置状态的话，理论上不会出现问题
//
//// CMotorCtrl
//
//class CMotorCtrl : public CWinThread
//{
//	DECLARE_DYNCREATE(CMotorCtrl)
//
//protected:
//	CMotorCtrl();           // protected constructor used by dynamic creation
//	virtual ~CMotorCtrl();
//
//public:
//	virtual BOOL InitInstance();
//	virtual int ExitInstance();
//
//protected:
//	afx_msg void ReadMotorStatus(WPARAM wParam,LPARAM lParam);
//
//protected:
//	DECLARE_MESSAGE_MAP()
//};
//
//
