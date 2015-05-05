/*****************************************************************************
 * HBarrier.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Class HBarrier
 *
 * (c) 2009-2010 by MVTec Software GmbH
 *               www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.1 $
 * $Date: 2010/06/25 11:28:00 $
 *
 */

#ifndef H_BARRIER_H
#define H_BARRIER_H


#include "HCPPToolRef.h"


namespace Halcon {


class LIntExport HBarrier: public HToolBase 
{
public:

  // Tool-specific functional class constructors
  HBarrier(const HTuple &AttrName,const HTuple &AttrValue, const long TeamSize);
  //HBarrier(const char *AttrName,const char *AttrValue, const long TeamSize);
	
  // Common tool class management functionality

  HBarrier();
  HBarrier(Hlong ID);
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Wait on the release of a barrier synchronization object.
  virtual void WaitBarrier() const;
  // Create a barrier synchronization object.
  virtual void CreateBarrier(const Halcon::HTuple &AttribName, const Halcon::HTuple &AttribValue, const Halcon::HTuple &TeamSize);
};

}

#endif
