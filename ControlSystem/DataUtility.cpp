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
			double d;
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