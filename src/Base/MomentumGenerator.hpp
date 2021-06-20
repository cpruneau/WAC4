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

  MomentumGenerator(TRandom * selectedRandom = gRandom);
  virtual ~MomentumGenerator() {}
  virtual TLorentzVector  generate(GeneratorType selectedType, vector<double>& parameters, TH1 * histogramP = nullptr);

protected:

  TRandom *      random;

  ClassDef(MomentumGenerator,0)
};

#endif /* WAC_MomentumGenerator */

