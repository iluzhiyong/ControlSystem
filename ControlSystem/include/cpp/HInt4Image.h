/*****************************************************************************
 * HInt4Image.h
 ***************************************************************************** 
 *
 * Project:     Halcon/C++
 * Description: Types for long-images 
 *
 * (c) 1996-2006 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 ****************************************************************************
 *
 * $Revision: 1.12 $
 * $Date: 2006/07/27 14:02:38 $
 *
 */


#ifndef H_INT4_IMAGE_H
#define H_INT4_IMAGE_H


namespace Halcon {


class LIntExport HInt4Image: public HImage {
public:
  HInt4Image(void);
  HInt4Image(const char *file);
  HInt4Image(const HImage &image);
  HInt4Image(const HInt4Image &image);
  HInt4Image(int width, int height);
  HInt4Image(int *ptr, int width, int height);
  virtual ~HInt4Image(void);
  
  // Tools
  const char *ClassName(void) const {return "HInt4Image";}

  // Pixel Access
  int operator[] (Hlong k) const
  {if (!privat) ((HInt4Image*)this)->Private();return ((INT4*)ptr)[k];}
  INT4 &operator[] (Hlong k)
  {if (!init) Init();return ((INT4*)ptr)[k];}
  INT4 &operator()(int x, int y) 
  {if (!init) Init();return ((INT4*)ptr)[HLinearCoord(x,y,width)];}
  INT4 &operator()(Hlong k) 
  {if (!init) Init();return ((INT4*)ptr)[k];}
  void SetPixVal(Hlong k, const HPixVal &val); 
  void SetPixVal(int x, int y, const HPixVal &val); 
};


}


#endif
