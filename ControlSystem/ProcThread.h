#pragma once
#include "HalconAction.h"

enum UserProcMsg
{
	WM_MOTOR_CONNECT			= WM_USER + 50,
	WM_MOTOR_CLEAR_ZERO_X,
	WM_MOTOR_CLEAR_ZERO_Y,
	WM_MOTOR_CLEAR_ZERO_Z,
	WM_MOTOR_MANUAL_START_MOVE,
	WM_MOTOR_MANUAL_STOP_MOVE,
	WM_MOTOR_GET_STATUS,
	WM_MOTOR_UPDATE_STATUS,
	WM_MOTOR_STOP,
	WM_MOTOR_MOVE_TO,

	WM_IMAGE_PROC,
	WM_IMAGE_LOAD,
	WM_IMAGE_PROC_SETTING,

	WM_DO_MANUAL_MEAR,
	WM_DO_AUTO_MEAR,
	WM_MAIN_THREAD_DO_CAPTURE,

	WM_OPEN_HALCON_WINDOW,
};

class IMotorCtrl;
class CCameraParaDlg;
class CImageProcess;
class CImageProcSettingDlg;
// CProcThread

class CProcThread : public CWinThread
{
	DECLARE_DYNCREATE(CProcThread)

protected:
	CProcThread();           // protected constructor used by dynamic creation
	virtual ~CProcThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	afx_msg void OnMotorConnect(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMotorClearZeroX(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMotorClearZeroY(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMotorClearZeroZ(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMotorManualStartMove(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMotorManualStopMove(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMotorGetStatus(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMotorStop(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMotorMoveTo(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoManualMear(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoAutoMear(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoImageProc(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoImageLoad(WPARAM wParam,LPARAM lParam);
	afx_msg void OnImageProcSetting(WPARAM wParam,LPARAM lParam);

	void OpenHalconWindow();
private:
	IMotorCtrl* m_IMotoCtrl;
	bool m_IsMotroCtrlConnected;

private:
	CImageProcess* m_IImageProcess;
	CImageProcSettingDlg* m_ImageProcSetDlg;

private:
	bool m_HalconWndOpened;
	HTuple m_WindowHandle;

protected:
	DECLARE_MESSAGE_MAP()

private:
	CListCtrl* m_pListData;
	bool GetMeasureTargetValue(int row, float &x, float &y, float &z);
	bool SetMeasureResultValue(int row, float resultX, float resultY, float resultZ);
	bool GetFloatItem(int row, int column, float &value);
	bool SetFloatItem(int row, int column, float value);
	bool ConvertStringToFloat(CString buffer, float &value);
	int CalculatePoint(float x, float y, float z, float &retx, float &rety, float &retz);
};


