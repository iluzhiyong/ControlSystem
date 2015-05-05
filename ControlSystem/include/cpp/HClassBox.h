/*****************************************************************************
 * HClassBox.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Class HClassBox
 *
 * (c) 1996-2008 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.9 $
 * $Date: 2008/12/06 13:35:12 $
 *
 */

#ifndef H_CLASS_BOX_H
#define H_CLASS_BOX_H


#include "HCPPToolRef.h"

namespace Halcon {


class LIntExport HClassBox: public HToolBase
{
public:

  // Tool-specific functional class constructors

  HClassBox(void);

  

  // Common tool class management functionality

  HClassBox(Hlong ID);
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Train a classificator using a multi-channel image.
  virtual void LearnNdimBox(const HRegionArray &Foreground, const HRegionArray &Background, const HImageArray &MultiChannelImage) const;
  // Classify pixels using hyper-cuboids.
  virtual HRegionArray ClassNdimBox(const HImageArray &MultiChannelImage) const;
  // Save the classifier in a file.
  virtual void WriteClassBox(const Halcon::HTuple &FileName) const;
  // Save the classifier in a file.
  virtual void WriteClassBox(const char *FileName) const;
  // Set system parameters for classification.
  virtual void SetClassBoxParam(const Halcon::HTuple &Flag, const Halcon::HTuple &Value) const;
  // Set system parameters for classification.
  virtual void SetClassBoxParam(const char *Flag, double Value) const;
  // Read the classifier from a file.
  virtual void ReadClassBox(const Halcon::HTuple &FileName) const;
  // Read the classifier from a file.
  virtual void ReadClassBox(const char *FileName) const;
  // Train the classifier with one data set.
  virtual void LearnSampsetBox(const Halcon::HTuple &SampKey, const Halcon::HTuple &Outfile, const Halcon::HTuple &NSamples, const Halcon::HTuple &StopError, const Halcon::HTuple &ErrorN) const;
  // Train the classifier with one data set.
  virtual void LearnSampsetBox(Hlong SampKey, const char *Outfile, Hlong NSamples, double StopError, Hlong ErrorN) const;
  // Train the classifier.
  virtual void LearnClassBox(const Halcon::HTuple &Features, const Halcon::HTuple &Class) const;
  // Get information about the current parameter.
  virtual double GetClassBoxParam(const Halcon::HTuple &Flag) const;
  // Get information about the current parameter.
  virtual double GetClassBoxParam(const char *Flag) const;
  // Create a new classifier.
  virtual void CreateClassBox(void);
  // Description of the classifier.
  virtual void DescriptClassBox(const Halcon::HTuple &Dimensions) const;
  // Description of the classifier.
  virtual void DescriptClassBox(Hlong Dimensions) const;
  // Classify a set of arrays.
  virtual double TestSampsetBox(const Halcon::HTuple &SampKey) const;
  // Classify a set of arrays.
  virtual double TestSampsetBox(Hlong SampKey) const;
  // Classify a tuple of attributes with rejection class.
  virtual Hlong EnquireRejectClassBox(const Halcon::HTuple &FeatureList) const;
  // Classify a tuple of attributes.
  virtual Hlong EnquireClassBox(const Halcon::HTuple &FeatureList) const;
};

}

#endif
