// ----------------------------------------------------------------------------
// petalosim | PetMaterialsList.h
//
// Definition of materials of common use.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef P_MATERIALS_LIST_H
#define P_MATERIALS_LIST_H

#include <globals.hh>

#include <CLHEP/Units/PhysicalConstants.h>

class G4Material;

namespace petmaterials {

  using namespace CLHEP;

  // Carbon Fiber
  G4Material* CarbonFiber();

  //LYSO
  G4Material* LYSO();

  // FR4
  G4Material* FR4();
}

#endif
