// ----------------------------------------------------------------------------
// petalosim | TileHamamatsuBlue.cc
//
// Hamamatsu Blue sensor tile geometry.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "TileHamamatsuBlue.h"
#include "PetMaterialsList.h"
#include "PetMaterialProperties.h"
#include "SiPMHamamatsuBlue.h"

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
#include <G4LogicalBorderSurface.hh>

using namespace nexus;

using namespace CLHEP;

TileHamamatsuBlue::TileHamamatsuBlue() : TileGeometryBase(),
                                         tile_x_(25. * mm),
                                         tile_y_(25. * mm),
                                         tile_z_(1.35 * mm),
                                         epoxy_depth_(0.1 * mm),
                                         sipm_pitch_(6.2 * mm),
                                         refr_index_(1.55), //given by Hammamatsu datasheet
                                         n_rows_(4),
                                         n_columns_(4)

{
  sipm_ = new SiPMHamamatsuBlue();
}

TileHamamatsuBlue::~TileHamamatsuBlue()
{
}

void TileHamamatsuBlue::Construct()
{
  SetDimensions(G4ThreeVector(tile_x_, tile_y_, tile_z_ + epoxy_depth_));

  G4Box *tile_solid = new G4Box("TILE_PLASTIC", tile_x_ / 2., tile_y_ / 2.,
                                (tile_z_ + epoxy_depth_) / 2);

  G4Material *fr4 = petmaterials::FR4();
  G4LogicalVolume *tile_logic =
      new G4LogicalVolume(tile_solid, fr4, "TILE_PLASTIC");

  this->SetLogicalVolume(tile_logic);

  // OPTICAL SURFACE FOR REFLECTION
  G4OpticalSurface *fr4_opsurf =
      new G4OpticalSurface("FR4_OPSURF", unified, polished, dielectric_metal);
  fr4_opsurf->SetMaterialPropertiesTable(petmaterialprops::ReflectantSurface(GetTileReflectivity()));

  new G4LogicalSkinSurface("FR4_OPSURF", tile_logic, fr4_opsurf);

  sipm_->SetSensorDepth(1);
  sipm_->SetMotherDepth(2);
  sipm_->SetBoxConf(GetBoxConf());
  sipm_->SetVisibility(GetTileVisibility());

  sipm_->Construct();
  G4ThreeVector sipm_dim = sipm_->GetDimensions();

  G4double offset_x = (tile_x_ - ((n_columns_ - 1) * sipm_pitch_) - sipm_dim.x()) / 2.;
  G4double offset_y = (tile_y_ - ((n_rows_ - 1) * sipm_pitch_) - sipm_dim.y()) / 2.;

  // EPOXY PROTECTIVE LAYER ////////////////////////////////////////
  G4double epoxy_depth = 0.1 * mm;

  G4Box *epoxy_solid =
      new G4Box("Epoxy", tile_x_ / 2., tile_y_ / 2., epoxy_depth / 2);

  G4Material *epoxy = materials::Epoxy();
  epoxy->SetMaterialPropertiesTable(petmaterialprops::EpoxyFixedRefr(refr_index_));

  G4LogicalVolume *epoxy_logic =
      new G4LogicalVolume(epoxy_solid, epoxy, "Epoxy");

  G4double epoxy_zpos = (tile_z_ + epoxy_depth_) / 2. - epoxy_depth / 2.;

  new G4PVPlacement(0, G4ThreeVector(0., 0., epoxy_zpos), epoxy_logic,
                    "Epoxy", tile_logic, false, 0, false);

  // SiPMs
  G4LogicalVolume *sipm_logic = sipm_->GetLogicalVolume();

  for (int j = 0; j < n_rows_; j++)
  {
    for (int i = 0; i < n_columns_; i++)
    {
      G4int copy_no = (j + 1) * 10 + i + 1;
      G4double x_pos = -tile_x_ / 2. + offset_x + sipm_dim.x() / 2. + i * sipm_pitch_;
      G4double y_pos = tile_y_ / 2. - offset_y - sipm_dim.y() / 2. - j * sipm_pitch_;
      G4double z_pos = (tile_z_ + epoxy_depth_) / 2. - epoxy_depth - sipm_dim.z() / 2.;
      G4String vol_name = "SiPMHmtsuBlue_" + std::to_string(copy_no);
      new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos),
                        sipm_logic, vol_name, tile_logic, false, copy_no, false);
    }
  }

  // Visibilities
  if (GetTileVisibility())
  {
    G4VisAttributes tile_col = nexus::CopperBrown();
    //tile_col.SetForceSolid(true);
    tile_logic->SetVisAttributes(tile_col);
    G4VisAttributes epoxy_col = nexus::White();
    //epoxy_col.SetForceSolid(true);
    epoxy_logic->SetVisAttributes(epoxy_col);
  }
  else
  {
    tile_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
  }
}
