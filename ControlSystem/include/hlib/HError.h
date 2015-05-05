/*****************************************************************************
 * HError.h
 ***************************************************************************** 
 *
 * Project:     HALCON/libhalcon
 * Description: Error messages
 *
 * (c) 1996-2008 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 * 
 * $Revision: 1.13 $
 * $Date: 2008/12/06 13:13:00 $
 *
 */


#ifndef HERROR_H
#define HERROR_H


#if defined(__cplusplus)
extern "C" {
#endif

extern HLibExport Herror HSetErrText(char *text);

extern HLibExport Herror HErrText(Herror message,char *err_text);

extern HLibExport Herror HMessage(Herror message,char *msg_text);



#if defined(__cplusplus)
}
#endif


#endif
