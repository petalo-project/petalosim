// ----------------------------------------------------------------------------
// petalosim | TileHamamatsuVUV.cc
//
// Hamamatsu VUV sensor tile geometry.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "TileHamamatsuVUV.h"
#include "PetMaterialsList.h"
#include "PetOpticalMaterialProperties.h"
#include "SiPMHamamatsuVUV.h"
#include "SiPMCells.h"

#include "nexus/Visibilities.h"
#include "nexus/IonizationSD.h"
#include "nexus/MaterialsList.h"
#include "nexus/OpticalMaterialProperties.h"

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

TileHamamatsuVUV::TileHamamatsuVUV() : TileGeometryBase(),
                                       tile_x_(30.9 * mm),
                                       tile_y_(30.7 * mm),
                                       tile_z_(2.3 * mm),
                                       sipm_pitch_(7.5 * mm),
                                       n_rows_(4),
                                       n_columns_(4),
                                       lxe_thick_(0.6 * mm),
                                       quartz_rindex_(1.6), //Given by Hamamatsu
                                       quartz_thick_(0.6 * mm)

{
}

TileHamamatsuVUV::~TileHamamatsuVUV()
{
}

void TileHamamatsuVUV::Construct()
{
  SetDimensions(G4ThreeVector(tile_x_, tile_y_, tile_z_));

  G4Box *tile_solid = new G4Box("TILE_PLASTIC", tile_x_ / 2., tile_y_ / 2., tile_z_ / 2);

  G4Material *fr4 = petmaterials::FR4();
  G4LogicalVolume *tile_logic =
      new G4LogicalVolume(tile_solid, fr4, "TILE_PLASTIC");

  this->SetLogicalVolume(tile_logic);

  // OPTICAL SURFACE FOR REFLECTION
  G4OpticalSurface *fr4_opsurf =
      new G4OpticalSurface("FR4_OPSURF", unified, polished, dielectric_metal);
  fr4_opsurf->SetMaterialPropertiesTable(petopticalprops::ReflectantSurface(GetTileReflectivity()));

  new G4LogicalSkinSurface("FR4_OPSURF", tile_logic, fr4_opsurf);
  SiPMHamamatsuVUV sipm;
  SiPMCells sipm_sat;
  G4ThreeVector sipm_dim;
  if (GetSiPMCells()) {
    sipm_sat.Construct();
    sipm_dim = sipm_sat.GetDimensions();
  } else {
    sipm.SetSensorDepth(1);
    sipm.SetMotherDepth(2);
    sipm.SetBoxGeom(GetBoxGeom());
    // The SiPMs will have the same visibility as the tile
    sipm.SetVisibility(GetTileVisibility());
    
    sipm.Construct();
    sipm_dim = sipm.GetDimensions();
  }

  G4double offset_x = (tile_x_ - ((n_columns_ - 1) * sipm_pitch_) - sipm_dim.x()) / 2.;
  G4double offset_y = (tile_y_ - ((n_rows_ - 1) * sipm_pitch_) - sipm_dim.y()) / 2.;

  // LXe
  G4double lxe_x = tile_x_ - offset_x;
  G4double lxe_y = tile_y_ - offset_y;
  G4double lxe_z = lxe_thick_ + quartz_thick_;
  G4Box *lxe_solid = new G4Box("TILE_LXE", lxe_x / 2., lxe_y / 2., lxe_z / 2.);

  G4Material *LXe = G4NistManager::Instance()->FindOrBuildMaterial("G4_lXe");
  LXe->SetMaterialPropertiesTable(opticalprops::LXe());
  G4LogicalVolume *lxe_logic =
      new G4LogicalVolume(lxe_solid, LXe, "TILE_LXE");

  G4double zpos_lxe = tile_z_ / 2. - (lxe_thick_ + quartz_thick_) / 2;
  new G4PVPlacement(0, G4ThreeVector(0., 0., zpos_lxe), lxe_logic,
                    "TILE_LXE", tile_logic, false, 0, false);

  // Layer of quartz defined inside the LXe
  G4double quartz_x = tile_x_ - offset_x;
  G4double quartz_y = tile_y_ - offset_y;

  G4Box *quartz_solid = new G4Box("TILE_QUARTZ_WINDOW", quartz_x / 2., quartz_y / 2.,
                                  quartz_thick_ / 2);

  G4Material *quartz = materials::FusedSilica();
  quartz->SetMaterialPropertiesTable(petopticalprops::FakeGenericMaterial(quartz_rindex_));

  G4LogicalVolume *quartz_logic =
      new G4LogicalVolume(quartz_solid, quartz, "TILE_QUARTZ_WINDOW");

  G4double zpos_quartz = (lxe_thick_ + quartz_thick_) / 2. - quartz_thick_ / 2.;
  new G4PVPlacement(0, G4ThreeVector(0., 0., zpos_quartz), quartz_logic,
                    "TILE_QUARTZ_WINDOW", lxe_logic, false, 0, false);

  // The real LXe region as active
  G4Box *active_solid =
      new G4Box("ACTIVE_LXE_TILE", lxe_x / 2., lxe_y / 2., lxe_thick_ / 2.);
  G4LogicalVolume *active_logic =
      new G4LogicalVolume(active_solid, LXe, "ACTIVE_LXE_TILE");

  G4double zpos_active = -(lxe_thick_ + quartz_thick_) / 2. + lxe_thick_ / 2.;
  new G4PVPlacement(0, G4ThreeVector(0., 0., zpos_active), active_logic,
                    "ACTIVE_LXE_TILE", lxe_logic, false, 0, false);

  // Set the ACTIVE volume as an ionization sensitive det
  IonizationSD *ionisd = new IonizationSD("/PETALO/ACTIVE_LXE_TILE");
  active_logic->SetSensitiveDetector(ionisd);
  G4SDManager::GetSDMpointer()->AddNewDetector(ionisd);
  
  // SiPMs
  G4LogicalVolume* sipm_logic;
  if (GetSiPMCells()) {
    sipm_logic = sipm_sat.GetLogicalVolume();
  } else {
    sipm_logic = sipm.GetLogicalVolume();
  }

  for (int j = 0; j < n_rows_; j++)
  {
    for (int i = 0; i < n_columns_; i++)
    {
      G4int copy_no = (j + 1) * 10 + i + 1;
      G4double x_pos = -tile_x_ / 2. + offset_x + sipm_dim.x() / 2. + i * sipm_pitch_;
      G4double y_pos = tile_y_ / 2. - offset_y - sipm_dim.y() / 2. - j * sipm_pitch_;
      G4double z_pos = tile_z_ / 2. - quartz_thick_ - lxe_thick_ - sipm_dim.z() / 2.;
      G4String vol_name = "SiPMHmtsuVUV_" + std::to_string(copy_no);
      new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos),
                        sipm_logic, vol_name, tile_logic, false, copy_no, false);
    }
  }

  // Visibilities
  if (GetTileVisibility())
  {
    G4VisAttributes tile_col = nexus::Lilla();
    //tile_col.SetForceSolid(true);
    tile_logic->SetVisAttributes(tile_col);
    G4VisAttributes quartz_col = nexus::Brown();
    //quartz_col.SetForceSolid(true);
    quartz_logic->SetVisAttributes(quartz_col);
    G4VisAttributes lxe_col = nexus::LightBlue();
    //lxe_col.SetForceSolid(true);
    lxe_logic->SetVisAttributes(lxe_col);
    G4VisAttributes active_col = nexus::White();
    //active_col.SetForceSolid(true);
    active_logic->SetVisAttributes(active_col);
  }
  else
  {
    tile_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
  }
}
