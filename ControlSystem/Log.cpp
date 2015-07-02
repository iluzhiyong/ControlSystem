/********************************************************************
	created:	2005/03/02
	created:	2:3:2005   13:29
	filename: 	Log.cpp
	file path:	Updater
	file base:	Log
	file ext:	cpp
	author:		Geert van Horrik
	
	purpose:	
*********************************************************************/

//*********************************************************************
// INCLUDES
//*********************************************************************

#include "stdafx.h"
//#include "Updater.h"
#include "Log.h"

//*********************************************************************
// INITIALISATION
//*********************************************************************

std::auto_ptr<CLog> CLog::sm_inst;

//*********************************************************************
// CONSTRUCTOR & DESTRUCTOR
//*********************************************************************

CLog::CLog()
{
	// Set file handler to NULL
	m_pLogFile = NULL;
}

//*********************************************************************
// PUBLIC FUNCTIONS
//*********************************************************************

CLog * CLog::Instance()
{
	if(sm_inst.get() == 0)
	sm_inst = auto_ptr<CLog>(new CLog);
	return sm_inst.get();

	/* FOLLOWING CODE WORKS ONLY IN VC7
	if(sm_inst.get() == 0)
	sm_inst.reset(new CLog);
	return sm_inst.get();
	*/
}

//=====================================================================

void CLog::CreateLog(CString sFilename, bool bEnableTimestamp)
{
	// Create the new log
	m_pLogFile = new CStdioFile(sFilename, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary);

	// Set timestamp option
	m_bEnableTimestamp = bEnableTimestamp;

	// Write first part so the file is unicode
	TCHAR bom = (TCHAR)0xFEFF;
	m_pLogFile->Write(&bom, sizeof(TCHAR));
	
	CString sText;
	sText.Insert(0,_T("\r\n"));
	m_pLogFile->Write(sText, sText.GetLength() * sizeof(TCHAR));
}

//=====================================================================

void CLog::Log(CString sText)
{
	USES_CONVERSION;

	// Check if log is already created
	if (m_pLogFile == NULL)
		return;

	// Should we add time to beginning of string?
	if (m_bEnableTimestamp)
	{
		sText.Insert(0, GetTime() + _T(" => "));
	}

	// Add \n\r to end of text
	sText.Insert(sText.GetLength(), _T("\r\n"));

	// Log text
	m_pLogFile->Write(sText, sText.GetLength() * sizeof(TCHAR));
}

void CLog::Log(int value)
{
	CString msg;
	msg.Format("%d", value);
	Log(msg);
}

void CLog::Log(float value)
{
	CString msg;
	msg.Format("%f", value);
	Log(msg);
}
//=====================================================================

void CLog::CloseLog()
{
	// Close file
	m_pLogFile->Close();

	// Delete object
	delete m_pLogFile;

	// Set handle to NULL
	m_pLogFile = NULL;
}

//=====================================================================

void CLog::EnableTimestamp(bool bEnable)
{
	// Set timestamp option
	m_bEnableTimestamp = bEnable;
}

//*********************************************************************
// PRIVATE FUNCTIONS
//*********************************************************************

CString CLog::GetTime()
{
	// Declare variables
	SYSTEMTIME time;
	CString sTime;

	// Get time
	GetLocalTime(&time);

	// Set up time
	sTime.Format(_T("%02d:%02d:%02d:%03d"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	// Return value
	return sTime;
}