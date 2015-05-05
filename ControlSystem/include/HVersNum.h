/*****************************************************************************
 * HVersNum.h
 ***************************************************************************** 
 *
 * Project:     HALCON/libhalcon
 * Description: Current version of the HALCON library
 *
 * (c) 1996-2010 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.71 $
 * $Date: 2010/09/03 15:30:02 $
 *
 */


#ifndef HLIB_VERSION
#define HLIB_VERSION          "10.0"
#endif

#ifndef HLIB_VERSION_NUM
#define HLIB_VERSION_NUM      10.0
#endif

#ifndef HLIB_LAST_UPDATE
#define HLIB_LAST_UPDATE      "15.09.2010"
#endif

#ifndef HLIB_REVISION
#define HLIB_REVISION         ""
#endif

#ifndef HALCON_VERSION_CODE
#define HALCON_VERSION_CODE 0x0A0000
#endif

#ifndef HALCON_VERSION
#define HALCON_VERSION(major,minor,revision) \
  (((major)<<16) + ((minor)<<8) + (revision))
#endif
