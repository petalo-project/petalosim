// ----------------------------------------------------------------------------
// petalosim | PetOpticalMaterialProperties.h
//
// Optical properties of relevant materials.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef P_OPTICAL_MATERIAL_PROPERTIES_H
#define P_OPTICAL_MATERIAL_PROPERTIES_H

#include <globals.hh>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

class G4MaterialPropertiesTable;

using namespace CLHEP;

/// This is a stateless class where all methods are static functions.

class PetOpticalMaterialProperties
{
public:
  static G4MaterialPropertiesTable *Epoxy();
  static G4MaterialPropertiesTable *GlassEpoxy();
  static G4MaterialPropertiesTable *EpoxyFixedRefr(G4double n);
  static G4MaterialPropertiesTable *EpoxyLXeRefr();

  static G4MaterialPropertiesTable *FakeGenericMaterial(G4double quartz_rindex = 1.6);

  static G4MaterialPropertiesTable *LXe_nconst();

  static G4MaterialPropertiesTable *Pyrex_vidrasa();

  static G4MaterialPropertiesTable *TPB(G4double decay_time = 2.2 * nanosecond);
  static G4MaterialPropertiesTable *TPB_LXe(G4double decay_time = 2.2 * nanosecond);
  static G4MaterialPropertiesTable *TPB_LXe_nconst(G4double decay_time = 2.2 * nanosecond);

  static G4MaterialPropertiesTable *LYSO();
  static G4MaterialPropertiesTable *LYSO_nconst();
  static G4MaterialPropertiesTable *ReflectantSurface(G4double reflectivity = 0.95);

private:
  //static constexpr G4double optPhotMinE_ = 1. * eV;
  //static constexpr G4double optPhotMaxE_ = 8.21 * eV;
  //static constexpr G4double noAbsLength_ = 1.e8 * m;

  static constexpr G4double lyso_minE_ = 1.9630 * eV;
  static constexpr G4double lyso_maxE_ = 3.3429 * eV;

  /// Constructor (hidden)
  PetOpticalMaterialProperties();
  /// Destructor (hidden)
  ~PetOpticalMaterialProperties();
};

#endif
