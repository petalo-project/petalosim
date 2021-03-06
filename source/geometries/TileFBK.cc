// ----------------------------------------------------------------------------
// petalosim | TileFBK.cc
//
// Geometry of a FBK sensor tile.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "TileFBK.h"
#include "PetMaterialsList.h"
#include "PetOpticalMaterialProperties.h"
#include "SiPMFBKVUV.h"

#include "nexus/IonizationSD.h"
#include "nexus/Visibilities.h"

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

TileFBK::TileFBK() : TileGeometryBase(),
                     tile_x_(29.0 * mm),
                     tile_y_(29.0 * mm),
                     tile_z_(1.6 * mm),
                     sipm_pitch_(3.5 * mm),
                     n_rows_(8),
                     n_columns_(8)

{
  sipm_ = new SiPMFBKVUV();
}

TileFBK::~TileFBK()
{
}

void TileFBK::Construct()
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

  sipm_->SetSensorDepth(1);
  sipm_->SetMotherDepth(2);
  sipm_->SetBoxGeom(GetBoxGeom());
  sipm_->SetVisibility(GetTileVisibility());
  sipm_->SetTimeBinning(GetTimeBinning());
  sipm_->SetPDE(GetPDE());

  sipm_->Construct();
  G4ThreeVector sipm_dim = sipm_->GetDimensions();

  G4double offset_x = 0.95 * mm;
  G4double offset_y = 0.55 * mm;

  // SiPMs
  G4LogicalVolume *sipm_logic = sipm_->GetLogicalVolume();

  G4int copy_no;
  G4int init_val = 10;
  G4int k = 0;
  for (int i = 0; i < n_rows_; i++)
  {
    if ((i != 0) && (i % 2 == 0))
    {
      k += 1;
    }
    copy_no = init_val + 10 * k;
    for (int j = 0; j < n_columns_; j++)
    {
      if (j % 2 == 0)
      {
        copy_no += 1;
      }
      G4double x_pos = -tile_x_ / 2. + offset_x + sipm_dim.x() / 2. + j * sipm_pitch_;
      G4double y_pos = tile_y_ / 2. - offset_y - sipm_dim.y() / 2. - i * sipm_pitch_;
      G4double z_pos = tile_z_ / 2. - sipm_dim.z() / 2.;
      G4String vol_name = "SiPMpetFBK_" + std::to_string(copy_no);
      new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos),
                        sipm_logic, vol_name, tile_logic, false, copy_no, false);
    }
  }

  // Visibilities
  if (GetTileVisibility())
  {
    G4VisAttributes tile_col = nexus::Lilla();
    tile_logic->SetVisAttributes(tile_col);
  }
  else
  {
    tile_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
  }
}