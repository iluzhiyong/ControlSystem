#pragma once
class DataUtility
{
public:
	DataUtility(void);
	~DataUtility(void);

public:
	static bool ConvertStringToFloat(CString buffer, float &value, float defaultValue = 0.0);
	static CString GetExePath();
	static float GetProfileFloat(CString appName, CString keyName, CString fileName, float defaultValue);
	static UINT GetProfileInt(CString appName, CString keyName, CString fileName, UINT defaultValue);
	static CString GetProfileString(CString appName, CString keyName, CString fileName, CString defaultValue);
	static void SetProfileFloat(CString appName, CString keyName, CString fileName, float Value);
	static void SetProfileInt(CString appName, CString keyName, CString fileName, UINT Value);
	static void ConvertPosByDeviationAngle(float x, float y, float retX, float retY, float angle, float* pX, float* pY);
};

