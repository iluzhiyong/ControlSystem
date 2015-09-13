#include "StdAfx.h"
#include "DataUtility.h"


DataUtility::DataUtility(void)
{
}


DataUtility::~DataUtility(void)
{
}


bool DataUtility::ConvertStringToFloat(CString buffer, float &value, float defaultValue)
{
	try
	{
		if(buffer != "")
		{
			char *endptr;
			endptr = NULL;
			double d = 0.0;
			d = strtod(buffer, &endptr);
			if (errno != 0 || (endptr != NULL && *endptr != '\0'))
			{
				return false;
			}
			else
			{
				value = (float)d;
				return true;
			}
		}
	}
	catch(...)
	{
	}
	value = defaultValue;
	return false;
}


CString DataUtility::GetExePath()
{
	TCHAR exeFullPath[MAX_PATH]; 
	memset(exeFullPath,0,MAX_PATH);  

	GetModuleFileName(NULL,exeFullPath,MAX_PATH);  
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
	return exeFullPath;
}

float DataUtility::GetProfileFloat(CString appName, CString keyName, CString fileName, float defaultValue)
{
	CString ret = _T("");
	char buf[256] = {0};
	float result = 0.0f;

	int len = GetPrivateProfileString(appName, keyName, "", buf, 256, fileName);
	if(len > 0)
	{
		for(int i=0;i<len;i++)
		{
			CString str;
			str.Format("%c",buf[i]);
			ret+=str;
		}
	}
	DataUtility::ConvertStringToFloat(buf, result, defaultValue);
	
	return result;
}

UINT DataUtility::GetProfileInt(CString appName, CString keyName, CString fileName, UINT defaultValue)
{
	return GetPrivateProfileInt(appName, keyName, defaultValue, fileName);
}

CString DataUtility::GetProfileString(CString appName, CString keyName, CString fileName, CString defaultValue)
{
	CString ret = _T("");
	char buf[256] = {0};

	int len = GetPrivateProfileString(appName, keyName, "", buf, 256, fileName);
	if(len > 0)
	{
		for(int i=0;i<len;i++)
		{
			CString str;
			str.Format("%c",buf[i]);
			ret+=str;
		}
	}

	return ret;
}

void DataUtility::SetProfileFloat(CString appName, CString keyName, CString fileName, float Value)
{
	CString setValve;
	setValve.Format(_T("%f"), Value);
	WritePrivateProfileString(appName, keyName, setValve, fileName);
}

void DataUtility::SetProfileInt(CString appName, CString keyName, CString fileName, UINT Value)
{
	CString setValve;
	setValve.Format(_T("%d"), Value);
	WritePrivateProfileString(appName, keyName, setValve, fileName);
}

void DataUtility::ConvertPosByDeviationAngle(float x, float y, float retX, float retY, float angle, float* pX, float* pY)
{
	*pX = retX;
	*pY = retY - retX * tan(angle);
}