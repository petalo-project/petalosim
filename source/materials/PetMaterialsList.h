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

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

class G4Material;

using namespace CLHEP;

/// Definition of materials of common use.
/// (This is a stateless class. All methods must be defined as static.)

class PetMaterialsList
{
public:

  // Carbon Fiber
  static G4Material *CarbonFiber();

  //LYSO
  static G4Material *LYSO();

  // FR4
  static G4Material *FR4();

private:
  /// Constructor (hidden)
  PetMaterialsList();
  /// Destructor (hidden)
  ~PetMaterialsList();
};

#endif
