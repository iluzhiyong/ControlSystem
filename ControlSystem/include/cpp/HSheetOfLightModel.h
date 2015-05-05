/*****************************************************************************
 * HSheetOfLightModel.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Class HSheetOfLightModel
 *
 * (c) 1996-2008 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.2 $
 * $Date: 2008/12/06 13:35:12 $
 *
 */

#ifndef H_SHEET_OF_LIGHT_MODEL_H
#define H_SHEET_OF_LIGHT_MODEL_H


#include "HCPPToolRef.h"


namespace Halcon {


class LIntExport HSheetOfLightModel: public HToolBase 
{
public:

  // Tool-specific functional class constructors
  
  HSheetOfLightModel(const HRegion &ProfileRegion, 
                     const HTuple &GenParamNames, 
                     const HTuple &GenParamValues);

  HSheetOfLightModel(const HRegion &ProfileRegion, 
                     const char    *GenParamNames, 
                     const char    *GenParamValues);

  // Common tool class management functionality

  HSheetOfLightModel();
  HSheetOfLightModel(Hlong ID);
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Get the result of a calibrated measurement performed with the 
  // sheet-of-light technique as a 3D object model.
  virtual HObjectModel3D GetSheetOfLightResultObjectModel3d() const;
  // Get the iconic results of a measurement performed with the sheet-of light
  // technique.
  virtual HImage GetSheetOfLightResult(const Halcon::HTuple &ResultName) const;
  // Get the iconic results of a measurement performed with the sheet-of light
  // technique.
  virtual HImage GetSheetOfLightResult(const char *ResultName) const;
  // Apply the calibration transformations to the input disparity image.
  virtual void ApplySheetOfLightCalibration(const HImage &Disparity) const;
  // Process the profile image provided as input and store the resulting
  // disparity to the sheet-of-light model.
  virtual void MeasureProfileSheetOfLight(const HImage &ProfileImage, const Halcon::HTuple &MovementPose) const;
  // Set selected parameters of the sheet-of-light model.
  virtual void SetSheetOfLightParam(const Halcon::HTuple &GenParamName, const Halcon::HTuple &GenParamValue) const;
  // Get the value of a parameter, which has been set in a sheet-of-light model.
  virtual HTuple GetSheetOfLightParam(const Halcon::HTuple &GenParamName) const;
  // Get the value of a parameter, which has been set in a sheet-of-light model.
  virtual HTuple GetSheetOfLightParam(const char *GenParamName) const;
  // For a given sheet-of-light model get the names of the generic iconic
  // or control parameters that can be used in the different
  // sheet-of-light operators.
  virtual HTuple QuerySheetOfLightParams(const Halcon::HTuple &QueryName) const;
  // For a given sheet-of-light model get the names of the generic iconic
  // or control parameters that can be used in the different
  // sheet-of-light operators.
  virtual HTuple QuerySheetOfLightParams(const char *QueryName) const;
  // Create a model to perform 3D-measurements using the sheet-of-light
  // technique.
  virtual void CreateSheetOfLightModel(const HRegion &ProfileRegion, const Halcon::HTuple &GenParamNames, const Halcon::HTuple &GenParamValues);
  // Create a model to perform 3D-measurements using the sheet-of-light
  // technique.
  virtual void CreateSheetOfLightModel(const HRegion &ProfileRegion, const char *GenParamNames, Hlong GenParamValues);
};

}

#endif
