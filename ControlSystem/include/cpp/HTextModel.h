/*****************************************************************************
 * HTextModel.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Class HTextModel
 *
 * (c) 2009-2010 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.1 $
 * $Date: 2010/06/22 14:12:16 $
 *
 */

#ifndef H_TEXT_MODEL_H
#define H_TEXT_MODEL_H


#include "HCPPToolRef.h"


namespace Halcon {


class LIntExport HTextModel: public HToolBase 
{
  friend class HImage;
  friend class HTextResult;
public:

  // Tool-specific functional class constructors


  // Common tool class management functionality

  HTextModel();
  HTextModel(Hlong ID);
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Segment text in an image.
  virtual HTextResult FindText(const HImage &Image) const;
  // Query parameters of a text model.
  virtual HTuple GetTextModelParam(const Halcon::HTuple &Param) const;
  // Query parameters of a text model.
  virtual HTuple GetTextModelParam(const char *Param) const;
  // Set parameters of a text model.
  virtual void SetTextModelParam(const Halcon::HTuple &Param, const Halcon::HTuple &Value) const;
  // Set parameters of a text model.
  virtual void SetTextModelParam(const char *Param, Hlong Value) const;
  // Create a text model.
  virtual void CreateTextModel(void);
};

}

#endif
