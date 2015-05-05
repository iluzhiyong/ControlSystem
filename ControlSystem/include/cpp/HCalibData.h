/*****************************************************************************
 * HCalibData.h
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
 * $Revision: 1.2 $
 * $Date: 2010/06/22 10:47:39 $
 *
 */

#ifndef H_CALIB_DATA_H
#define H_CALIB_DATA_H


#include "HCPPToolRef.h"


namespace Halcon {


class LIntExport HCalibData: public HToolBase 
{
public:

  // Tool-specific functional class constructors

  HCalibData(const HTuple &CalibSetup, const HTuple &NumCameras,
             const HTuple &NumCalibPatterns);
  HCalibData(const char *CalibSetup, Hlong NumCameras,
             Hlong NumCalibPatterns);
             
  // Common tool class management functionality

  HCalibData();
  HCalibData(Hlong ID);
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Restores a calibration data model from a file.
  virtual void ReadCalibData(const Halcon::HTuple &FileName);
  // Restores a calibration data model from a file.
  virtual void ReadCalibData(const char *FileName);
  // Store a calibration data model into a file.
  virtual void WriteCalibData(const Halcon::HTuple &FileName) const;
  // Store a calibration data model into a file.
  virtual void WriteCalibData(const char *FileName) const;
  // Calibrate one ore more cameras.
  virtual double CalibrateCameras() const;
  // Set data in a calibration data model.
  virtual void SetCalibData(const Halcon::HTuple &ItemType, const Halcon::HTuple &ItemIdx, const Halcon::HTuple &DataName, const Halcon::HTuple &DataValue) const;
  // Get point-based observation data from a calibration data model.
  virtual HTuple GetCalibDataObservPoints(const Halcon::HTuple &CameraIdx, const Halcon::HTuple &CalibObjIdx, const Halcon::HTuple &CalibObjPoseIdx, Halcon::HTuple *Column, Halcon::HTuple *Index, Halcon::HTuple *Pose) const;
  // Get point-based observation data from a calibration data model.
  virtual HTuple GetCalibDataObservPoints(Hlong CameraIdx, Hlong CalibObjIdx, Hlong CalibObjPoseIdx, Halcon::HTuple *Column, Halcon::HTuple *Index, Halcon::HTuple *Pose) const;
  // Set point-based observation data in a calibration data model.
  virtual void SetCalibDataObservPoints(const Halcon::HTuple &CameraIdx, const Halcon::HTuple &CalibObjIdx, const Halcon::HTuple &CalibObjPoseIdx, const Halcon::HTuple &Row, const Halcon::HTuple &Column, const Halcon::HTuple &Index, const Halcon::HTuple &Pose) const;
  // Query information about the relations between cameras, calibration objects,
  // and calibration object poses.
  virtual HTuple QueryCalibDataObservIndices(const Halcon::HTuple &ItemType, const Halcon::HTuple &ItemIdx, Halcon::HTuple *Index2) const;
  // Query information about the relations between cameras, calibration objects,
  // and calibration object poses.
  virtual HTuple QueryCalibDataObservIndices(const char *ItemType, Hlong ItemIdx, Halcon::HTuple *Index2) const;
  // Query data stored or computed in a calibration data model.
  virtual double GetCalibData(const Halcon::HTuple &ItemType, const Halcon::HTuple &ItemIdx, const Halcon::HTuple &DataName) const;
  // Query data stored or computed in a calibration data model.
  virtual double GetCalibData(const char *ItemType, Hlong ItemIdx, const char *DataName) const;
  // Define a calibration object in a calibration model.
  virtual void SetCalibDataCalibObject(const Halcon::HTuple &CalibObjIdx, const Halcon::HTuple &CalibObjDescr) const;
  // Define a calibration object in a calibration model.
  virtual void SetCalibDataCalibObject(Hlong CalibObjIdx, double CalibObjDescr) const;
  // Sets type and initial parameters of a camera in a calibration data model.
  virtual void SetCalibDataCamParam(const Halcon::HTuple &CameraIdx, const Halcon::HTuple &CameraType, const Halcon::HTuple &CameraParam) const;
  // Create a HALCON calibration data model.
  virtual void CreateCalibData(const Halcon::HTuple &CalibSetup, const Halcon::HTuple &NumCameras, const Halcon::HTuple &NumCalibObjects);
  // Create a HALCON calibration data model.
  virtual void CreateCalibData(const char *CalibSetup, Hlong NumCameras, Hlong NumCalibObjects);
};

}

#endif
