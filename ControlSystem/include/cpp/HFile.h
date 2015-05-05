/*****************************************************************************
 * HFile.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Class HFile
 *
 * (c) 1996-2008 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.9 $
 * $Date: 2008/12/06 13:35:12 $
 *
 */

#ifndef H_FILE_H
#define H_FILE_H


#include "HCPPToolRef.h"


namespace Halcon {


class LIntExport HFile: public HToolBase 
{
public:

  // Tool-specific functional class constructors
  
  HFile(const HTuple &FileName, const HTuple &FileType);
  HFile(const char   *FileName, const char   *FileType);


  // Common tool class management functionality

  HFile();
  HFile(Hlong ID);
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Open text file.
  virtual void OpenFile(const Halcon::HTuple &FileName, const Halcon::HTuple &FileType);
  // Open text file.
  virtual void OpenFile(const char *FileName, const char *FileType);
  // Write values in a text file.
  virtual void FwriteString(const Halcon::HTuple &String) const;
  // Read a line from a text file.
  virtual HTuple FreadLine(Halcon::HTuple *IsEOF) const;
  // Read a line from a text file.
  virtual HTuple FreadLine(Hlong *IsEOF) const;
  // Read strings from a text file.
  virtual HTuple FreadString(Halcon::HTuple *IsEOF) const;
  // Read strings from a text file.
  virtual HTuple FreadString(Hlong *IsEOF) const;
  // Read a character from a text file.
  virtual HTuple FreadChar() const;
  // Create a line feed.
  virtual void FnewLine() const;
};

}

#endif
