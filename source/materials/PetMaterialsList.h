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
  // Stainless Steel (grade 304L)
  static G4Material *Steel();

  // Stainless steel grade 316Ti
  static G4Material *Steel316Ti();

  // Epoxy resin
  static G4Material *Epoxy();

  // Carbon Fiber
  static G4Material *CarbonFiber();

  // Kovar (nickel-cobalt ferrous alloy)
  static G4Material *Kovar();

  // PEEK (Polyether ether ketone)
  static G4Material *PEEK();

  // Fused silica (synthetic quartz)
  static G4Material *FusedSilica();

  // PS (Polystyrene)
  static G4Material *PS();

  // TPB (tetraphenyl butadiene)
  static G4Material *TPB();

  // PTH (p-terphenyl)
  static G4Material *TPH();

  // KEVLAR (-NH-C6H4-NH-CO-C6H4-CO-)*n
  static G4Material *Kevlar();

  /// High density polyethylene
  static G4Material *HDPE();

  //LYSO
  static G4Material *LYSO();

  // FR4
  static G4Material *FR4();

  // Fake dielectric (to be deprecated)
  static G4Material *FakeDielectric(G4Material *, G4String);

  ///
  static G4Material *CopyMaterial(G4Material *, G4String);

private:
  /// Constructor (hidden)
  PetMaterialsList();
  /// Destructor (hidden)
  ~PetMaterialsList();
};

#endif
