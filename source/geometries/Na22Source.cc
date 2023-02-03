// ----------------------------------------------------------------------------
// petalosim | Na22Source.cc
//
// Na-22 calibration specific source with plastic support used in PETit.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "Na22Source.h"

#include "nexus/Visibilities.h"
#include "nexus/MaterialsList.h"

#include <G4Tubs.hh>
#include <G4Orb.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

using namespace CLHEP;

Na22Source::Na22Source(): DiskSource()
{
  source_diam_   = 0.25*mm;
  support_diam_  = 25.4*mm;
  support_thick_ = 6.35*mm;
}

Na22Source::~Na22Source()
{
}

void Na22Source::Construct()
{
  // Plastic support
  G4Tubs* support_solid =
    new G4Tubs("SUPPORT", 0., support_diam_/2., support_thick_/2., 0., twopi);
  
  G4Material* plastic = materials::PS();
  G4LogicalVolume* support_logic =
    new G4LogicalVolume(support_solid, plastic, "NA22_SOURCE_SUPPORT");
  
  this->SetLogicalVolume(support_logic);
  
  G4Orb* source_solid =
    new G4Orb("NA22_SOURCE", source_diam_/2.);
  G4Material* sodium22_mat =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Na");
  G4LogicalVolume* source_logic =
    new G4LogicalVolume(source_solid, sodium22_mat, "NA22_SOURCE");
  
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
                    source_logic, "NA22_SOURCE",
                    support_logic, false, 0, false);
  G4VisAttributes source_col = nexus::LightGreen();
  source_col.SetForceSolid(true);
  source_logic->SetVisAttributes(source_col);
  
  return;
}

