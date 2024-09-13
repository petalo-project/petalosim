// ----------------------------------------------------------------------------
// petalosim | SiPMquadFBK.cc
//
// Specific SiPM geometry proposed by FBK.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "SiPMquadFBK.h"
#include "ToFSD.h"
#include "PetMaterialsList.h"
#include "PetMaterialProperties.h"

#include "nexus/SensorSD.h"
#include "nexus/Visibilities.h"
#include "nexus/MaterialsList.h"

#include <G4GenericMessenger.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>

using namespace nexus;

using namespace CLHEP;

SiPMquadFBK::SiPMquadFBK() : GeometryBase(),
                             visibility_(0),
                             refr_index_(1.54),
                             eff_(1.)
{
  /// Messenger
  msg_ = new G4GenericMessenger(this, "/Geometry/SiPMpet/",
                                "Control commands of geometry.");
  msg_->DeclareProperty("visibility", visibility_, "SiPMpet Visibility");
  msg_->DeclareProperty("refr_index", refr_index_,
                        "Refraction index for epoxy");
  msg_->DeclareProperty("efficiency", eff_, "Efficiency of SiPM");
}

SiPMquadFBK::~SiPMquadFBK()
{
}

void SiPMquadFBK::Construct()
{

  // PACKAGE ///////////////////////////////////////////////////////
  G4double offset = 0.1 * mm;

  G4double sipm_x = 5.9 / 2. * mm;
  G4double sipm_y = 5.5 / 2. * mm;
  G4double sipm_z = 1 * mm;

  SetDimensions(G4ThreeVector(sipm_x, sipm_y, sipm_z));

  G4Box* sipm_solid =
    new G4Box("SiPMquadFBK", sipm_x / 2., sipm_y / 2., sipm_z / 2);

  G4Material* epoxy = materials::Epoxy();
  epoxy->SetMaterialPropertiesTable(petmaterialprops::EpoxyFixedRefr(refr_index_));

  G4LogicalVolume* sipm_logic =
      new G4LogicalVolume(sipm_solid, epoxy, "SiPMquadFBK");

  this->SetLogicalVolume(sipm_logic);

  // ACTIVE WINDOW /////////////////////////////////////////////////

  G4double active_x = sipm_x;
  G4double active_y = sipm_y;
  G4double active_depth = 0.01 * mm;

  G4Box* active_solid =
      new G4Box("PHOTODIODES", active_x / 2., active_y / 2., active_depth / 2);

  G4Material *silicon =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");

  G4LogicalVolume* active_logic =
      new G4LogicalVolume(active_solid, silicon, "PHOTODIODES");

  new G4PVPlacement(0,
                    G4ThreeVector(0., 0., sipm_z/2. - active_depth/2. - offset),
                    active_logic, "PHOTODIODES", sipm_logic, false, 0, false);

  // OPTICAL SURFACES /////////////////////////////////////////////

  const G4int entries = 12;

  G4double energies[entries] =
    {1.5 * eV, 6.19919 * eV, 6.35814 * eV, 6.52546 * eV,
     6.70182 * eV, 6.88799 * eV, 7.08479 * eV,
     7.29316 * eV, 7.51417 * eV, 7.74898 * eV,
     7.99895 * eV, 8.26558 * eV};
  G4double reflectivity[entries] = {0., 0., 0., 0.,
                                    0., 0., 0.,
                                    0., 0., 0.,
                                    0., 0.};
  G4double efficiency[entries] = {eff_, eff_, eff_,
                                  eff_, eff_, eff_,
                                  eff_, eff_, eff_,
                                  eff_, eff_};

  G4MaterialPropertiesTable* sipm_mt = new G4MaterialPropertiesTable();
  sipm_mt->AddProperty("EFFICIENCY", energies, efficiency, entries);
  sipm_mt->AddProperty("REFLECTIVITY", energies, reflectivity, entries);

  G4OpticalSurface* sipm_opsurf =
      new G4OpticalSurface("SIPM_OPSURF", unified, polished, dielectric_metal);
  sipm_opsurf->SetMaterialPropertiesTable(sipm_mt);

  new G4LogicalSkinSurface("SIPM_OPSURF", active_logic, sipm_opsurf);

  // SENSITIVE DETECTOR ////////////////////////////////////////////

  G4String sdname = "/SIPM/SiPMquadFBK";
  G4SDManager* sdmgr = G4SDManager::GetSDMpointer();

  if (!sdmgr->FindSensitiveDetector(sdname, false))
  {
    //SensorSD* sipmsd = new SensorSD(sdname);
    ToFSD* sipmsd = new ToFSD(sdname);
    sipmsd->SetDetectorVolumeDepth(0);
    sipmsd->SetDetectorNamingOrder(1000.);
    sipmsd->SetMotherVolumeDepth(1);

    G4SDManager::GetSDMpointer()->AddNewDetector(sipmsd);
    sipm_logic->SetSensitiveDetector(sipmsd);
  }

  G4cout << visibility_ << G4endl;
  // Visibilities
  if (visibility_)
  {
    G4VisAttributes sipm_col = nexus::Yellow();
    sipm_logic->SetVisAttributes(sipm_col);
    G4VisAttributes active_col = nexus::Blue();
    active_col.SetForceSolid(true);
    active_logic->SetVisAttributes(active_col);
  }
  else
  {
    sipm_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
    active_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
  }
}
