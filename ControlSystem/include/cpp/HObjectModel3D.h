/*****************************************************************************
 * HObjectModel3D.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Class HObjectModel3D
 *
 * (c) 2007-2008 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.7 $
 * $Date: 2010/07/06 10:34:50 $
 *
 */

#ifndef H_OBJECT_MODEL_3D_H
#define H_OBJECT_MODEL_3D_H


#include "HCPPToolRef.h"


namespace Halcon {


class LIntExport HObjectModel3D: public HToolBase
{
public:

  // Tool-specific functional class constructors

  HObjectModel3D(const HTuple &FileName, 
                 const HTuple &Scale,
                 const HTuple &GenParamNames, 
                 const HTuple &GenParamValues,
                 HTuple *DxfStatus);

  HObjectModel3D(const HImage &X, 
                 const HImage &Y, 
                 const HImage &Z);
                 

  // Common tool class management functionality

  HObjectModel3D();
  HObjectModel3D(Hlong ID);
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Get the result of a calibrated measurement performed with the 
  // sheet-of-light technique as a 3D object model.
  virtual void GetSheetOfLightResultObjectModel3d(const Halcon::HSheetOfLightModel &SheetOfLightModelID);
  // Fit 3D primitives into a set of 3D points.
  virtual HTuple FitPrimitivesObjectModel3d(const Halcon::HTuple &ParamName, const Halcon::HTuple &ParamValue) const;
  // Segment a set of 3D points into sub-sets with similar characteristics.
  virtual HTuple SegmentObjectModel3d(const Halcon::HTuple &ParamName, const Halcon::HTuple &ParamValue) const;
  // Reconstruct surface from calibrated multi-view stereo images.
  virtual void ReconstructSurfaceStereo(const HImageArray &Images, const Halcon::HStereoModel &StereoModelID);
  // Refine the pose of a surface model in a 3D scene.
  virtual HTuple RefineSurfaceModelPose(const Halcon::HSurfaceModel &SurfaceModelID, const Halcon::HTuple &InitialPose, const Halcon::HTuple &MinScore, const Halcon::HTuple &ReturnResultHandle, const Halcon::HTuple &GenParamName, const Halcon::HTuple &GenParamValue, Halcon::HTuple *Score, Halcon::HTuple *SurfaceMatchingResultID) const;
  // Find the best matches of a surface model in a 3D scene.
  virtual HTuple FindSurfaceModel(const Halcon::HSurfaceModel &SurfaceModelID, const Halcon::HTuple &RelSamplingDistance, const Halcon::HTuple &KeyPointFraction, const Halcon::HTuple &MinScore, const Halcon::HTuple &ReturnResultHandle, const Halcon::HTuple &GenParamName, const Halcon::HTuple &GenParamValue, Halcon::HTuple *Score, Halcon::HTuple *SurfaceMatchingResultID) const;
  // Create the data structure needed to perform surface-based matching.
  virtual HSurfaceModel CreateSurfaceModel(const Halcon::HTuple &RelSamplingDistance, const Halcon::HTuple &GenParamName, const Halcon::HTuple &GenParamValue) const;
  // Copy a 3D object model.
  virtual HObjectModel3D CopyObjectModel3d(const Halcon::HTuple &Attributes) const;
  // Prepare a 3D object model for a certain operation.
  virtual void PrepareObjectModel3d(const Halcon::HTuple &Purpose, const Halcon::HTuple &OverwriteData, const Halcon::HTuple &GenParamName, const Halcon::HTuple &GenParamValue) const;
  // Transform 3D points from a 3D object model to images.
  virtual HImage ObjectModel3dToXyz(HImage *Y, HImage *Z, const Halcon::HTuple &Type, const Halcon::HTuple &CamParam, const Halcon::HTuple &Pose) const;
  // Transform 3D points from images to a 3D object model.
  virtual void XyzToObjectModel3d(const HImage &X, const HImage &Y, const HImage &Z);
  // Return the attributes of a 3D object model.
  virtual HTuple GetObjectModel3dParams(const Halcon::HTuple &ParamName) const;
  // Return the attributes of a 3D object model.
  virtual HTuple GetObjectModel3dParams(const char *ParamName) const;
  // Project a 3D object model into image coordinates.
  virtual HXLDContArray ProjectObjectModel3d(const Halcon::HTuple &CamParam, const Halcon::HTuple &Pose, const Halcon::HTuple &GenParamName, const Halcon::HTuple &GenParamValue) const;
  // Apply an arbitrary affine 3D transformation to a 3D object model.
  virtual HObjectModel3D AffineTransObjectModel3d(const Halcon::HTuple &HomMat3D) const;
  // Writes a 3D object model to a file.
  virtual void WriteObjectModel3d(const Halcon::HTuple &FileType, const Halcon::HTuple &FileName, const Halcon::HTuple &GenParamName, const Halcon::HTuple &GenParamValue) const;
  // Read a 3D object model from a file.
  virtual HTuple ReadObjectModel3d(const Halcon::HTuple &FileName, const Halcon::HTuple &Scale, const Halcon::HTuple &GenParamName, const Halcon::HTuple &GenParamValue);
  // Read a 3D object model from a DXF file.
  virtual HTuple ReadObjectModel3dDxf(const Halcon::HTuple &FileName, const Halcon::HTuple &Scale, const Halcon::HTuple &GenParamNames, const Halcon::HTuple &GenParamValues);
};

}

#endif
