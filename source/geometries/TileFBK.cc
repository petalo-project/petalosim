// ----------------------------------------------------------------------------
// petalosim | TileFBK.cc
//
// Geometry of a FBK sensor tile.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "TileFBK.h"
#include "PetMaterialsList.h"
#include "PetMaterialProperties.h"
#include "SiPMFBKVUV.h"
#include "SiPMCells.h"

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
                     n_columns_(8),
                     sipm_naming_order_(0)

{
}

TileFBK::~TileFBK()
{
}

void TileFBK::Construct()
{
  SetDimensions(G4ThreeVector(tile_x_, tile_y_, tile_z_));

  G4Box *tile_solid =
    new G4Box("TILE_PLASTIC", tile_x_/2., tile_y_/2., tile_z_/2);

  G4Material *fr4 = petmaterials::FR4();
  G4LogicalVolume *tile_logic =
      new G4LogicalVolume(tile_solid, fr4, "TILE_PLASTIC");

  this->SetLogicalVolume(tile_logic);

  // OPTICAL SURFACE FOR REFLECTION
  G4OpticalSurface *fr4_opsurf =
      new G4OpticalSurface("FR4_OPSURF", unified, polished, dielectric_metal);
  fr4_opsurf->SetMaterialPropertiesTable(petmaterialprops::ReflectantSurface(GetTileReflectivity()));

  new G4LogicalSkinSurface("FR4_OPSURF", tile_logic, fr4_opsurf);

  SiPMCells sipm_cells;
  SiPMFBKVUV sipm;

  G4ThreeVector sipm_dim;

  if (GetSiPMCells()) {
    sipm_cells.SetDim(G4ThreeVector(3. * mm, 3.4 * mm, 0.6 * mm));
    sipm_cells.SetNumbOfMicroCells(8245);
    sipm_cells.SetPitch(0.035 * mm);
    sipm_cells.SetMicroCell("MicroCellFBK");
    sipm_cells.SetPDE(GetPDE());
    sipm_cells.Construct();
    sipm_dim = sipm_cells.GetDim();
  } else {
    sipm.SetSensorDepth(1);
    sipm.SetMotherDepth(2);
    sipm.SetNamingOrder(sipm_naming_order_);
    sipm.SetBoxConf(GetBoxConf());
    sipm.SetVisibility(GetTileVisibility());
    sipm.SetPDE(GetPDE());

    sipm.Construct();
    sipm_dim = sipm.GetDimensions();
  }

  G4double offset_x = 0.95 * mm;
  G4double offset_y = 0.55 * mm;

  // SiPMs
  G4LogicalVolume* sipm_logic;

  if (GetSiPMCells()) {
    sipm_logic = sipm_cells.GetLogicalVolume();
  } else {
    sipm_logic = sipm.GetLogicalVolume();
  }

  G4int copy_no;
  G4String vol_name;
  G4int counter = 0;
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
      G4double x_pos = -tile_x_/2. + offset_x + sipm_dim.x()/2. + j * sipm_pitch_;
      G4double y_pos = tile_y_/2. - offset_y - sipm_dim.y()/2. - i * sipm_pitch_;
      G4double z_pos = tile_z_/2. - sipm_dim.z()/2.;

      if (sipm_naming_order_>0) {
        // In this case, SiPM IDs are as follows:
        // 100, 101, ..., 163
        // 200, 201, ..., 263
        // ...
        vol_name = "SiPMFBKVUV_" + std::to_string(counter);
        new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos),
                          sipm_logic, vol_name, tile_logic, false, counter, false);
        counter++;
      } else {
        // In this case, SiPM IDs are as follows:
        // 111, 112, ..., 144. The SiPMs are read in groups of four
        // and only one ID is assigned to a group.
        // This way we mimic what the electronic does.
        vol_name = "SiPMFBKVUV_" + std::to_string(copy_no);
        new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos),
                          sipm_logic, vol_name, tile_logic, false, copy_no, false);
      }
    }
  }

  // Visibilities
  if (GetTileVisibility()) {
    G4VisAttributes tile_col = nexus::Lilla();
    tile_logic->SetVisAttributes(tile_col);
  } else {
    tile_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
  }
}
