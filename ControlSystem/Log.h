/********************************************************************
	created:	2005/03/02
	created:	2:3:2005   13:30
	filename: 	Log.h
	file path:	Updater
	file base:	Log
	file ext:	h
	author:		Geert van Horrik
	
	purpose:	
*********************************************************************/

// Redefinition protection
#ifndef LOG_H
#define LOG_H

//*********************************************************************
// INCLUDES
//*********************************************************************

#include <memory>				// Needed for singleton class
using namespace std;

//*********************************************************************
// CLog
//*********************************************************************

class CLog  
{
public:
	// Functions
	static CLog * Instance();
	void CreateLog(CString sFilename, bool bEnableTimestamp);
	void Log(CString sText);
	void EnableTimestamp(bool bEnable = true);
	void CloseLog();

private:
	// Constructor & destructor
	CLog();			// Private constructor because singleton!

	// Functions
	CString GetTime();

	// Variables
	static std::auto_ptr<CLog> sm_inst;
	CStdioFile * m_pLogFile;
	bool    m_bEnableTimestamp;
};

// End of redefinition protection
#endif // LOG_H