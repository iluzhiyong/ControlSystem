/*****************************************************************************
 * HCameraSetupModel.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Class HCameraSetupModel
 *
 * (c) 1996-2010 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.1 $
 * $Date: 2010/06/21 08:18:12 $
 *
 */

#ifndef H_CAMERA_SETUP_MODEL_H
#define H_CAMERA_SETUP_MODEL_H


#include "HCPPToolRef.h"


namespace Halcon {


class LIntExport HCameraSetupModel: public HToolBase 
{
public:

  // Tool-specific functional class constructors
  
  HCameraSetupModel(Hlong NumCameras);
  HCameraSetupModel(const Halcon::HTuple &NumCameras);
  
  // Common tool class management functionality

  HCameraSetupModel();
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Create a HALCON stereo model.
  virtual HStereoModel CreateStereoModel(const Halcon::HTuple &Method, const Halcon::HTuple &GenParamName, const Halcon::HTuple &GenParamValue) const;
  // Create a HALCON stereo model.
  virtual HStereoModel CreateStereoModel(const char *Method, const char *GenParamName, double GenParamValue) const;
  // Store a camera setup model into a file.
  virtual void WriteCameraSetupModel(const Halcon::HTuple &FileName) const;
  // Store a camera setup model into a file.
  virtual void WriteCameraSetupModel(const char *FileName) const;
  // Restores a camera setup model from a file.
  virtual void ReadCameraSetupModel(const Halcon::HTuple &FileName);
  // Restores a camera setup model from a file.
  virtual void ReadCameraSetupModel(const char *FileName);
  // Get generic camera setup model parameters.
  virtual double GetCameraSetupParam(const Halcon::HTuple &CameraIdx, const Halcon::HTuple &ParamName) const;
  // Get generic camera setup model parameters.
  virtual double GetCameraSetupParam(Hlong CameraIdx, const char *ParamName) const;
  // Set generic camera setup model parameters.
  virtual void SetCameraSetupParam(const Halcon::HTuple &CameraIdx, const Halcon::HTuple &ParamName, const Halcon::HTuple &ParamValue) const;
  // Set generic camera setup model parameters.
  virtual void SetCameraSetupParam(Hlong CameraIdx, const char *ParamName, double ParamValue) const;
  // Defines type, parameters, and relative pose of a camera in a camera setup
  // model.
  virtual void SetCameraSetupCamParam(const Halcon::HTuple &CameraIdx, const Halcon::HTuple &CameraType, const Halcon::HTuple &CameraParam, const Halcon::HTuple &CameraPose) const;
  // Create model for a setup of calibrated cameras.
  virtual void CreateCameraSetupModel(const Halcon::HTuple &NumCameras);
  // Create model for a setup of calibrated cameras.
  virtual void CreateCameraSetupModel(Hlong NumCameras);
};

}

#endif
