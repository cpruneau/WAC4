//
//  MomentumGenerator.hpp
//  Created by Victor Gonzalez on 20190206 based in the work of Claude Pruneau and the ALICE AliPhysics SW framework
//  Copyright © 2016-2021 Claude Pruneau. All rights reserved.
//

#ifndef WAC_MomentumGenerator
#define WAC_MomentumGenerator
#include <vector>
#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TRandom.h>
#include <TLorentzVector.h>

using std::vector;
using std::cout;
using std::endl;

class MomentumGenerator
{
public:

  enum GeneratorType
  {
    IsotropicGaussP, IsotropicExpP, IsotropicUniformP,
    IsotropicUniformDensity, IsotropicMaxwellP, IsotropicHistoP,
    CylindricalGaussPtUniformY, CylindricalExpPtUniformY,
    CylindricalMaxwellPtUniformY, CylindricalRadialFlowUniformY,
    CylindricalHistoPtUniformY
  };

  //! stat : 1 Fermi Dirac
  //!     -1 : bose einstein
  MomentumGenerator(TRandom * _selectedRandom);
  MomentumGenerator(const TString & _partName, double _mass, double _tMin, double _tMax, double _tWidth, double _stat, TRandom * _selectedRandom= gRandom);
  virtual ~MomentumGenerator() {}
  virtual TLorentzVector  generate(GeneratorType selectedType, vector<double>& parameters, TH1 * histogramP = nullptr);
  virtual TLorentzVector  generate(double temperature);

protected:

  TString        partName;
  double         mass;
  double         tMin, tMax, tWidth;
  double         stat;
  vector<TH1*>   histograms;
  TRandom *      random;

  ClassDef(MomentumGenerator,0)
};

#endif /* WAC_MomentumGenerator */

