#pragma once
#include "HalconAction.h"
#include "cnComm.h"

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

	WM_DO_CUSTOM_MEAR,
	WM_DO_AUTO_MEAR,
	WM_MAIN_THREAD_DO_CAPTURE,

	WM_OPEN_HALCON_WINDOW,
	WM_OPEN_AXIAL_DEVIATION_ANGLE_WND,

	WM_AUTO_MEAR_FINISH,
};

//从0开始计数
enum ExcelInfo
{
	COLUMN_POS_X = 3,
	COLUMN_POS_Y = 4,
	COLUMN_POS_Z = 5,

	COLUMN_COMPENSATION_X = 6,
	COLUMN_COMPENSATION_Y = 7,
	COLUMN_COMPENSATION_Z = 8,

	COLUMN_MEAR_TYPE = 9,
	COLUMN_TERMINATOR = 0,

	ROW_START = 5,

	ROW_RESULT_START = 6,
	COLUMN_RESULT_START = 3,
	COLUMN_RESULT_END = 5,
};

class IMotorCtrl;
class CCameraParaDlg;
class CImageProcess;
class CImageProcSettingDlg;
class CAxialDeviationAngle;
class CImageProcSetAllDlg;
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
	afx_msg void OnDoCustomMear(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoAutoMear(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoImageProc(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoImageLoad(WPARAM wParam,LPARAM lParam);
	afx_msg void OnImageProcSetting(WPARAM wParam,LPARAM lParam);
	afx_msg void OnOpenCacAngleWnd(WPARAM wParam,LPARAM lParam);

	void OpenHalconWindow();
private:
	IMotorCtrl* m_IMotoCtrl;
	bool m_IsMotroCtrlConnected;

private:
	CImageProcess* m_IImageProcess;
	CImageProcSetAllDlg* m_imageProcSetAllDlg;
	CAxialDeviationAngle* m_AxialDeviationAngleDlg;

private:
	bool m_HalconWndOpened;
	HTuple m_WindowHandle;

protected:
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl* m_pListData;
	bool GetMeasureTargetValue(int row, float &x, float &y, float &z);
	bool GetFloatItem(int row, int column, float &value);
	bool SetFloatItem(int row, int column, float value);
	int MoveToTargetPosXYZ(float x, float y, float z, float &retx, float &rety, float &retz);
	int MoveToTargetPosXY(float x, float y, float z, float &retx, float &rety, bool calX = false, bool calY = false);

private:
	float m_MearTolerance;
	float m_DistenceCameraAndTarget;
	float m_ZMoveTopV;
	float m_XCalV;
	float m_YCalV;

	int m_CalCount;

	int m_workpieceType;
	//轴向偏离角，图纸尺寸为车架的尺寸，车架放到平台上后，并不能保证车架与平台保持横平竖直。
	//因此，增加车架与平台的角度。
	float m_DeviationAngle;

	CnComm m_COM;
};


