// ----------------------------------------------------------------------------
// petalosim | PETit.cc
//
// This class implements the geometry of a box of LXe with FBK tiles.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PETitFBK.h"
#include "PETitBox.h"
#include "TileFBK.h"
#include "PetMaterialsList.h"
#include "PetOpticalMaterialProperties.h"
#include "TeflonBlockFBK.h"
#include "PetaloUtils.h"

#include "nexus/Visibilities.h"
#include "nexus/IonizationSD.h"
#include "nexus/FactoryBase.h"
#include "nexus/SpherePointSampler.h"

#include <G4GenericMessenger.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4SDManager.hh>
#include <G4UserLimits.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>

using namespace nexus;

REGISTER_CLASS(PETitFBK, GeometryBase)

PETitFBK::PETitFBK() : GeometryBase(),
                       visibility_(0),
                       box_vis_(0),
                       tile_vis_(1),
                       tile_refl_(0.),
                       sipm_pde_(0.3),
                       specific_vertex_{},
                       max_step_size_(1. * mm),
                       pressure_(1 * bar),
                       dist_dice_flange_(18.6 * mm),
                       n_tile_rows_(2),
                       n_tile_columns_(2)

{
  // Messenger
  msg_ = new G4GenericMessenger(this, "/Geometry/PETitFBK/",
                                "Control commands of geometry PETitFBK.");
  msg_->DeclareProperty("visibility", visibility_, "Visibility");
  msg_->DeclareProperty("box_vis", box_vis_,
                        "Visibility of the basic structure");
  msg_->DeclareProperty("tile_vis", tile_vis_, "Visibility of tiles");
  msg_->DeclareProperty("tile_refl", tile_refl_, "Reflectivity of SiPM boards");
  msg_->DeclareProperty("sipm_pde", sipm_pde_,
                        "SiPM photodetection efficiency");

  msg_->DeclarePropertyWithUnit("specific_vertex", "mm",  specific_vertex_,
                                "Set generation vertex.");

  G4GenericMessenger::Command &press_cmd =
    msg_->DeclareProperty("pressure", pressure_,
                          "Pressure of LXe");
  press_cmd.SetUnitCategory("Pressure");
  press_cmd.SetParameterName("pressure", false);
  press_cmd.SetRange("pressure>0.");



}

PETitFBK::~PETitFBK()
{
}

void PETitFBK::Construct()
{
  // Volume of air surrounding the detector //
  G4double lab_size = 1. * m;
  G4Box *lab_solid = new G4Box("LAB", lab_size/2., lab_size/2., lab_size/2.);

  G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  lab_logic_ = new G4LogicalVolume(lab_solid, air, "LAB");
  lab_logic_->SetVisAttributes(G4VisAttributes::GetInvisible());
  this->SetLogicalVolume(lab_logic_);

  BuildBox();
  BuildSensors();
}

void PETitFBK::BuildBox()
{
  G4Material* LXe = G4NistManager::Instance()->FindOrBuildMaterial("G4_lXe");
  LXe->SetMaterialPropertiesTable(petopticalprops::LXe(pressure_));

  // Set the ACTIVE volume as an ionization sensitive det
  IonizationSD* ionisd = new IonizationSD("/PETALO/ACTIVE");
  G4SDManager::GetSDMpointer()->AddNewDetector(ionisd);


  box_ = new PETitBox();
  box_->SetLXePressure(pressure_);
  box_->SetXeMaterial(LXe);
  box_->SetIoniSD(ionisd);
  box_->SetVisibility(box_vis_);
  box_->Construct();
  G4LogicalVolume* box_logic = box_->GetLogicalVolume();

  new G4PVPlacement(0, G4ThreeVector(0., 0, 0.),
                    box_logic, "ALUMINUM_BOX", lab_logic_,
                    false, 0, false);

  active_logic_ = box_->GetActiveVolume();
  G4double ih_z_size = box_->GetHatZSize();

  TeflonBlockFBK teflon_block_fbk = TeflonBlockFBK();
  teflon_block_fbk.SetHoleMaterial(LXe);
  teflon_block_fbk.SetIoniSD(ionisd);
  teflon_block_fbk.SetMaxStepSize(max_step_size_);
  teflon_block_fbk.Construct();
  G4LogicalVolume* teflon_block_logic = teflon_block_fbk.GetLogicalVolume();

  G4double teflon_block_thick = teflon_block_fbk.GetTeflonThickness();
  G4double block_z_pos = ih_z_size/2. + 0.25*mm + teflon_block_thick/2.;
  new G4PVPlacement(0, G4ThreeVector(0., 0., -block_z_pos), teflon_block_logic,
                    "TEFLON_BLOCK_FBK", active_logic_, false, 0, false);

  G4RotationMatrix rot_teflon;
  rot_teflon.rotateY(pi);
  new G4PVPlacement(G4Transform3D(rot_teflon,
                                  G4ThreeVector(0., 0., block_z_pos)),
                    teflon_block_logic,
                    "TEFLON_BLOCK_FBK", active_logic_, false, 1, false);

  end_of_teflon_z_ = block_z_pos + teflon_block_thick/2.;

  // Optical surface for teflon
  G4OpticalSurface* teflon_optSurf =
    new G4OpticalSurface("TEFLON_OPSURF", unified, ground, dielectric_metal);
  teflon_optSurf->SetMaterialPropertiesTable(petopticalprops::PTFE());
  new G4LogicalSkinSurface("TEFLON_OPSURF", teflon_block_logic, teflon_optSurf);

  if (visibility_) {
    G4VisAttributes block_col = nexus::LightBlue();
    teflon_block_logic->SetVisAttributes(block_col);
  }
}

void PETitFBK::BuildSensors()
{
  TileFBK tile = TileFBK();
  tile.SetBoxConf(fbk);
  tile.SetNamingOrder(100);
  tile.SetTileVisibility(tile_vis_);
  tile.SetTileReflectivity(tile_refl_);
  tile.SetPDE(sipm_pde_);
  tile.Construct();

  G4double tile_size_x = tile.GetDimensions().x();
  G4double tile_size_y = tile.GetDimensions().y();
  G4double tile_thickn = tile.GetDimensions().z();
  G4double full_row_size = n_tile_columns_ * tile_size_x;
  G4double full_col_size = n_tile_rows_ * tile_size_y;

  G4LogicalVolume* tile_logic = tile.GetLogicalVolume();
  G4String vol_name;
  G4int copy_no = 1;
  G4double z_pos = end_of_teflon_z_ + 0.5*mm + tile_thickn/2.;

  for (G4int j = 0; j < n_tile_rows_; j++) {
    G4double y_pos = full_col_size/2. - tile_size_y/2. - j*tile_size_y;
    for (G4int i = 0; i < n_tile_columns_; i++) {
      G4double x_pos = -full_row_size/2. + tile_size_x/2. + i*tile_size_x;
      vol_name = "TILE_" + std::to_string(copy_no);

      new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, -z_pos), tile_logic,
                        vol_name, active_logic_, false, copy_no, false);
      copy_no += 1;
    }
  }

  G4RotationMatrix rot;
  rot.rotateY(pi);

  for (G4int j=0; j<n_tile_rows_; j++) {
    G4double y_pos = full_col_size/2. - tile_size_y/2. - j*tile_size_y;
    for (G4int i=0; i<n_tile_columns_; i++) {
      G4double x_pos = full_row_size/2. - tile_size_x/2. - i*tile_size_x;
      vol_name = "TILE_" + std::to_string(copy_no);

      new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(x_pos, y_pos, z_pos)),
                        tile_logic, vol_name, active_logic_,
                        false, copy_no, false);
      copy_no += 1;
    }
  }

}

G4ThreeVector PETitFBK::GenerateVertex(const G4String &region) const
{
  G4ThreeVector vertex(0., 0., 0.);

  if (region == "CENTER") {
    return vertex;
  } else if (region == "AD_HOC") {
    vertex = specific_vertex_;
  } else if (region == "SOURCE") {
    vertex = box_->GenerateVertex("SOURCE");
  } else {
    G4Exception("[PETitFBK]", "GenerateVertex()", FatalException,
                "Unknown vertex generation region!");
  }
  return vertex;
}
