/*****************************************************************************
 * HOCRMlp.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Class HOCRMlp
 *
 * (c) 1996-2008 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.3 $
 * $Date: 2008/12/06 13:35:12 $
 *
 */

#ifndef H_OCRMLP_H
#define H_OCRMLP_H


#include "HCPPToolRef.h"


namespace Halcon {


class LIntExport HOCRMlp: public HToolBase 
{
public:

  // Tool-specific functional class constructors

  HOCRMlp(const HTuple &WidthCharacter, const HTuple &HeightCharacter, 
          const HTuple &Interpolation, const HTuple &Features, 
          const HTuple &Characters, const HTuple &NumHidden,
          const HTuple &Preprocessing, const HTuple &NumComponents,
          const HTuple &RandSeed);

  // Common tool class management functionality

  HOCRMlp();
  HOCRMlp(Hlong ID);
  const char *ClassName(void) const;
  void  SetHandle(Hlong ID);

  // Tool-specific member functions

  // Read an OCR classifier from a file.
  virtual void ReadOcrClassMlp(const Halcon::HTuple &FileName);
  // Read an OCR classifier from a file.
  virtual void ReadOcrClassMlp(const char *FileName);
  // Write an OCR classifier to a file.
  virtual void WriteOcrClassMlp(const Halcon::HTuple &FileName) const;
  // Write an OCR classifier to a file.
  virtual void WriteOcrClassMlp(const char *FileName) const;
  // Compute the features of a character.
  virtual HTuple GetFeaturesOcrClassMlp(const HImage &Character, const Halcon::HTuple &Transform) const;
  // Compute the features of a character.
  virtual HTuple GetFeaturesOcrClassMlp(const HImage &Character, const char *Transform) const;
  // Classify a related group of characters with an OCR classifier.
  virtual HTuple DoOcrWordMlp(const HRegionArray &Character, const HImage &Image, const Halcon::HTuple &Expression, const Halcon::HTuple &NumAlternatives, const Halcon::HTuple &NumCorrections, Halcon::HTuple *Confidence, Halcon::HTuple *Word, Halcon::HTuple *Score) const;
  // Classify a related group of characters with an OCR classifier.
  virtual HTuple DoOcrWordMlp(const HRegionArray &Character, const HImage &Image, const Halcon::HTuple &Expression, const Halcon::HTuple &NumAlternatives, const Halcon::HTuple &NumCorrections, double *Confidence, char *Word, double *Score) const;
  // Classify a related group of characters with an OCR classifier.
  virtual HTuple DoOcrWordMlp(const HRegionArray &Character, const HImage &Image, const char *Expression, Hlong NumAlternatives, Hlong NumCorrections, Halcon::HTuple *Confidence, Halcon::HTuple *Word, Halcon::HTuple *Score) const;
  // Classify a related group of characters with an OCR classifier.
  virtual HTuple DoOcrWordMlp(const HRegionArray &Character, const HImage &Image, const char *Expression, Hlong NumAlternatives, Hlong NumCorrections, double *Confidence, char *Word, double *Score) const;
  // Classify multiple characters with an OCR classifier.
  virtual HTuple DoOcrMultiClassMlp(const HRegionArray &Character, const HImage &Image, Halcon::HTuple *Confidence) const;
  // Classify multiple characters with an OCR classifier.
  virtual HTuple DoOcrMultiClassMlp(const HRegionArray &Character, const HImage &Image, double *Confidence) const;
  // Classify a single character with an OCR classifier.
  virtual HTuple DoOcrSingleClassMlp(const HRegion &Character, const HImage &Image, const Halcon::HTuple &Num, Halcon::HTuple *Confidence) const;
  // Classify a single character with an OCR classifier.
  virtual HTuple DoOcrSingleClassMlp(const HRegion &Character, const HImage &Image, const Halcon::HTuple &Num, double *Confidence) const;
  // Train an OCR classifier.
  virtual double TrainfOcrClassMlp(const Halcon::HTuple &TrainingFile, const Halcon::HTuple &MaxIterations, const Halcon::HTuple &WeightTolerance, const Halcon::HTuple &ErrorTolerance, Halcon::HTuple *ErrorLog) const;
  // Train an OCR classifier.
  virtual double TrainfOcrClassMlp(const char *TrainingFile, Hlong MaxIterations, double WeightTolerance, double ErrorTolerance, Halcon::HTuple *ErrorLog) const;
  // Compute the information content of the preprocessed feature vectors
  // of an OCR classifier.
  virtual HTuple GetPrepInfoOcrClassMlp(const Halcon::HTuple &TrainingFile, const Halcon::HTuple &Preprocessing, Halcon::HTuple *CumInformationCont) const;
  // Compute the information content of the preprocessed feature vectors
  // of an OCR classifier.
  virtual HTuple GetPrepInfoOcrClassMlp(const char *TrainingFile, const char *Preprocessing, Halcon::HTuple *CumInformationCont) const;
  // Return the parameters of an OCR classifier.
  virtual Hlong GetParamsOcrClassMlp(Halcon::HTuple *HeightCharacter, Halcon::HTuple *Interpolation, Halcon::HTuple *Features, Halcon::HTuple *Characters, Halcon::HTuple *NumHidden, Halcon::HTuple *Preprocessing, Halcon::HTuple *NumComponents) const;
  // Return the parameters of an OCR classifier.
  virtual Hlong GetParamsOcrClassMlp(Hlong *HeightCharacter, char *Interpolation, char *Features, Halcon::HTuple *Characters, Hlong *NumHidden, char *Preprocessing, Hlong *NumComponents) const;
  // Create an OCR classifier using a multilayer perceptron.
  virtual void CreateOcrClassMlp(const Halcon::HTuple &WidthCharacter, const Halcon::HTuple &HeightCharacter, const Halcon::HTuple &Interpolation, const Halcon::HTuple &Features, const Halcon::HTuple &Characters, const Halcon::HTuple &NumHidden, const Halcon::HTuple &Preprocessing, const Halcon::HTuple &NumComponents, const Halcon::HTuple &RandSeed);
};

}

#endif
