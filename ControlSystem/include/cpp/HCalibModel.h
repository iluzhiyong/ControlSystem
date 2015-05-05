/*****************************************************************************
 * HCalibModel.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Class HCalibModel
 *
 * (c) 1996-2010 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.1 $
 * $Date: 2009/12/01 15:51:35 $
 *
 */

#ifndef H_CALIB_MODEL_H
#define H_CALIB_MODEL_H


#include "HCPPToolRef.h"


namespace Halcon {


class LIntExport HCalibModel: public HToolBase 
{
public:

  // Tool-specific functional class constructors

  HCalibModel(const HTuple &CalibModel, const HTuple &NumCameras,
              const HTuple &NumCalibObjs);
  // Common tool class management functionality

  HCalibModel();
  HCalibModel(Hlong ID);
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Free the memory of all calibration models from main memory.
  static void ClearAllCalibModels(void);
  // Restores a calibration model from a file.
  virtual void ReadCalibModel(const Halcon::HTuple &FileName);
  // Restores a calibration model from a file.
  virtual void ReadCalibModel(const char *FileName);
  // Store a calibration model into a file.
  virtual void WriteCalibModel(const Halcon::HTuple &FileName) const;
  // Store a calibration model into a file.
  virtual void WriteCalibModel(const char *FileName) const;
  // Perform camera calibration.
  virtual double PerformCameraCalibration() const;
  // Get generic parameters for a specific calibration item.
  virtual HTuple GetCalibModelParamsSpecific(const Halcon::HTuple &ItemType, const Halcon::HTuple &ItemID, const Halcon::HTuple &GenParamNames) const;
  // Get generic parameters for a specific calibration item.
  virtual HTuple GetCalibModelParamsSpecific(const char *ItemType, Hlong ItemID, const char *GenParamNames) const;
  // Set generic parameters for a specific calibration item.
  virtual void SetCalibModelParamsSpecific(const Halcon::HTuple &ItemType, const Halcon::HTuple &ItemID, const Halcon::HTuple &GenParamNames, const Halcon::HTuple &GenParamValues) const;
  // Get generic calibration model parameters.
  virtual HTuple GetCalibModelParams(const Halcon::HTuple &GenParamNames) const;
  // Get generic calibration model parameters.
  virtual HTuple GetCalibModelParams(const char *GenParamNames) const;
  // Set generic calibration model parameters.
  virtual void SetCalibModelParams(const Halcon::HTuple &GenParamNames, const Halcon::HTuple &GenParamValues) const;
  // Get point-based observation data from a calibration model.
  virtual HTuple GetCalibModelObservPoints(const Halcon::HTuple &CameraID, const Halcon::HTuple &CalibPatternID, const Halcon::HTuple &CalibObjID, Halcon::HTuple *Column, Halcon::HTuple *Index, Halcon::HTuple *Pose) const;
  // Get point-based observation data from a calibration model.
  virtual HTuple GetCalibModelObservPoints(Hlong CameraID, Hlong CalibPatternID, Hlong CalibObjID, Halcon::HTuple *Column, Halcon::HTuple *Index, Halcon::HTuple *Pose) const;
  // Set point-based observation data into a calibration model.
  virtual void SetCalibModelObservPoints(const Halcon::HTuple &CameraID, const Halcon::HTuple &CalibPatternID, const Halcon::HTuple &CalibObjID, const Halcon::HTuple &Row, const Halcon::HTuple &Column, const Halcon::HTuple &Index, const Halcon::HTuple &Pose) const;
  // Query information about camera-to-calibration-objects observation
  // relations.
  virtual HTuple QueryCalibModelObservIndices(const Halcon::HTuple &ItemType, const Halcon::HTuple &ItemID, Halcon::HTuple *Index2) const;
  // Query information about camera-to-calibration-objects observation
  // relations.
  virtual HTuple QueryCalibModelObservIndices(const char *ItemType, Hlong ItemID, Halcon::HTuple *Index2) const;
  // Query diverse results of the calibration process.
  virtual double GetCalibModelResult(const Halcon::HTuple &ItemType, const Halcon::HTuple &ItemID, const Halcon::HTuple &ResultName) const;
  // Query diverse results of the calibration process.
  virtual double GetCalibModelResult(const char *ItemType, Hlong ItemID, const char *ResultName) const;
  // Defines a calibration pattern in a calibration model.
  virtual void SetCalibModelCalibPattern(const Halcon::HTuple &CalibPatternID, const Halcon::HTuple &CalibPatternDescr) const;
  // Defines a calibration pattern in a calibration model.
  virtual void SetCalibModelCalibPattern(Hlong CalibPatternID, double CalibPatternDescr) const;
  // Defines camera type and initial parameters in a calibration model.
  virtual void SetCalibModelCamera(const Halcon::HTuple &CameraID, const Halcon::HTuple &CameraType, const Halcon::HTuple &CameraParam) const;
  // Create a HALCON calibration model.
  virtual void CreateCalibModel(const Halcon::HTuple &CalibSetup, const Halcon::HTuple &NumCameras, const Halcon::HTuple &NumCalibPatterns);
  // Create a HALCON calibration model.
  virtual void CreateCalibModel(const char *CalibSetup, Hlong NumCameras, Hlong NumCalibPatterns);
};

}

#endif
