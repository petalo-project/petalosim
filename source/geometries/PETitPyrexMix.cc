// ----------------------------------------------------------------------------
// petalosim | PETitPyrexMix.cc
//
// This class implements the geometry of a continuous box of LXe
// with 4 Hamamatsu tiles on one side and one centered FBK tile
// on the other side. Pyrex panels avoid reflections in the walls
// of the chamber.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PETitPyrexMix.h"
#include "PETitBox.h"
#include "TileHamamatsuVUV.h"
#include "TileHamamatsuBlue.h"
#include "TileFBK.h"
#include "PetMaterialsList.h"
#include "PetOpticalMaterialProperties.h"
#include "TeflonBlockHamamatsu.h"
#include "PetaloUtils.h"

#include "nexus/Visibilities.h"
#include "nexus/IonizationSD.h"
#include "nexus/FactoryBase.h"
#include "nexus/SpherePointSampler.h"
#include "nexus/MaterialsList.h"

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

REGISTER_CLASS(PETitPyrexMix, GeometryBase)

PETitPyrexMix::PETitPyrexMix() : GeometryBase(),
                                 visibility_(0),
                                 box_vis_(0),
                                 tile_vis_(1),
                                 tile_refl_(0.),
                                 panel_refl_(0.),
                                 n_tile_rows_(2),
                                 n_tile_columns_(2),
                                 dist_lat_panels_(69. * mm),
                                 dist_ihat_entry_panel_(5.25 * mm), //z distance between the external surface of the hat and the internal surface of the entry panel
                                 panel_thickness_(1.75 * mm),
                                 entry_panel_x_size_(77.5 * mm),
                                 entry_panel_y_size_(120 * mm),
                                 dist_entry_panel_ground_(12 * mm),
                                 dist_entry_panel_h_panel_(6.2 * mm), //z distance between the internal surface of the entry panel and the edge of the horizontal lateral panel
                                 dist_entry_panel_v_panel_(1.5 * mm),  //z distance between the internal surface of the entry panel and the edge of the vertical lateral panel
                                 lat_panel_len_(66.5 * mm),
                                 h_l_panel_z_size_(42. * mm),
                                 h_l_panel_y_pos_(40.95 * mm),
                                 v_l_panel_z_size_(46.7 * mm),
                                 dist_ham_vuv_(18.6 * mm),
                                 dist_fbk_(21.05 * mm),
                                 sipm_pde_(0.3),
                                 specific_vertex_{},
                                 pressure_(1 * bar)

{
  // Messenger
  msg_ = new G4GenericMessenger(this, "/Geometry/PETitPyrexMix/",
                                "Control commands of geometry PETitPyrexMix.");
  msg_->DeclareProperty("visibility", visibility_, "Visibility");
  msg_->DeclareProperty("box_vis", box_vis_, "Visibility of the basic structure");
  msg_->DeclareProperty("tile_vis", tile_vis_, "Visibility of tiles");
  msg_->DeclareProperty("tile_refl", tile_refl_, "Reflectivity of SiPM boards");
  msg_->DeclareProperty("panel_refl", panel_refl_, "Reflectivity of the panels");
  msg_->DeclareProperty("sipm_pde", sipm_pde_, "FBK SiPM photodetection efficiency");

  msg_->DeclarePropertyWithUnit("specific_vertex", "mm",  specific_vertex_,
                                "Set generation vertex.");

  G4GenericMessenger::Command &press_cmd =
    msg_->DeclareProperty("pressure", pressure_,
                          "Pressure of LXe");
  press_cmd.SetUnitCategory("Pressure");
  press_cmd.SetParameterName("pressure", false);
  press_cmd.SetRange("pressure>0.");



}

PETitPyrexMix::~PETitPyrexMix()
{
}

void PETitPyrexMix::Construct()
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

void PETitPyrexMix::BuildBox()
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

  G4Box* entry_panel_solid =
    new G4Box("ENTRY_PANEL", entry_panel_x_size_/2., entry_panel_y_size_/2., panel_thickness_/2.);


  G4Material* pyrex = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pyrex_Glass");
  pyrex->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());


  // PYREX PANELS BETWEEN THE INTERNAL HAT AND THE ACTIVE REGIONS //

  G4LogicalVolume* entry_panel_logic =
    new G4LogicalVolume(entry_panel_solid, pyrex, "ENTRY_PANEL");

  G4double entry_panel_ypos = -box_->GetBoxSize()/2. + box_->GetBoxThickness() +
                                dist_entry_panel_ground_ + entry_panel_y_size_/2.;
  G4double entry_panel_zpos = box_->GetHatZSize()/2. + dist_ihat_entry_panel_ + panel_thickness_/2.;
  

  new G4PVPlacement(0, G4ThreeVector(0., entry_panel_ypos, -entry_panel_zpos),
                    entry_panel_logic, "ENTRY_PANEL", active_logic_, false, 1, false);
  
  new G4PVPlacement(0, G4ThreeVector(0., entry_panel_ypos, entry_panel_zpos),
                    entry_panel_logic, "ENTRY_PANEL", active_logic_, false, 2, false);

  // PYREX PANELS SURROUNDING THE SIPM DICE BOARDS //

  // Horizontal lateral panels
  G4Box* h_l_panel_solid =
    new G4Box("LAT_PANEL", lat_panel_len_/2., panel_thickness_/2., h_l_panel_z_size_/2.);
  
  G4LogicalVolume* h_l_panel_logic =
    new G4LogicalVolume(h_l_panel_solid, pyrex, "LAT_PANEL");
  
  G4double h_l_panel_ypos_bot = -box_->GetBoxSize()/2. + box_->GetBoxThickness() +
    h_l_panel_y_pos_ + panel_thickness_/2.;
  G4double h_l_panel_ypos_top = h_l_panel_ypos_bot + panel_thickness_ +
    dist_lat_panels_;
  G4double h_l_panel_zpos = entry_panel_zpos + panel_thickness_/2. +
    dist_entry_panel_h_panel_ + h_l_panel_z_size_/2.;
  
  new G4PVPlacement(0, G4ThreeVector(0., h_l_panel_ypos_bot, -h_l_panel_zpos),
                    h_l_panel_logic, "LAT_PANEL", active_logic_, false, 1, false);
  
  new G4PVPlacement(0, G4ThreeVector(0., h_l_panel_ypos_bot, h_l_panel_zpos),
                    h_l_panel_logic, "LAT_PANEL", active_logic_, false, 2, false);
  
  new G4PVPlacement(0, G4ThreeVector(0., h_l_panel_ypos_top, -h_l_panel_zpos),
                    h_l_panel_logic, "LAT_PANEL", active_logic_, false, 3, false);
  
  new G4PVPlacement(0, G4ThreeVector(0., h_l_panel_ypos_top, h_l_panel_zpos),
                    h_l_panel_logic, "LAT_PANEL", active_logic_, false, 4, false);
  
  // Vertical lateral panels
  G4Box* v_l_panel_solid =
    new G4Box("LAT_PANEL", panel_thickness_/2., lat_panel_len_/2., v_l_panel_z_size_/2.);

  G4LogicalVolume *v_l_panel_logic =
    new G4LogicalVolume(v_l_panel_solid, pyrex, "LAT_PANEL");
  
  G4double v_l_panel_xpos = dist_lat_panels_/2. + panel_thickness_/2.;
  G4double v_l_panel_ypos = h_l_panel_ypos_bot + dist_lat_panels_/2. +
    panel_thickness_/2.;
  G4double v_l_panel_zpos = entry_panel_zpos + panel_thickness_/2. +
    dist_entry_panel_v_panel_ + v_l_panel_z_size_/2.;
  
  new G4PVPlacement(0, G4ThreeVector(-v_l_panel_xpos, v_l_panel_ypos, -v_l_panel_zpos),
                    v_l_panel_logic, "LAT_PANEL", active_logic_, false, 1, false);
  
  new G4PVPlacement(0, G4ThreeVector(-v_l_panel_xpos, v_l_panel_ypos, v_l_panel_zpos),
                    v_l_panel_logic, "LAT_PANEL", active_logic_, false, 2, false);
  
  new G4PVPlacement(0, G4ThreeVector(v_l_panel_xpos, v_l_panel_ypos, -v_l_panel_zpos),
                    v_l_panel_logic, "LAT_PANEL", active_logic_, false, 3, false);
  
  new G4PVPlacement(0, G4ThreeVector(v_l_panel_xpos, v_l_panel_ypos, v_l_panel_zpos),
                    v_l_panel_logic, "LAT_PANEL", active_logic_, false, 4, false);

  // Optical surface for the panels
  G4OpticalSurface* panel_opsur = new G4OpticalSurface("OP_PANEL");
  panel_opsur->SetType(dielectric_metal);
  panel_opsur->SetModel(unified);
  panel_opsur->SetFinish(ground);
  panel_opsur->SetSigmaAlpha(0.1);
  panel_opsur->SetMaterialPropertiesTable(petopticalprops::ReflectantSurface(reflectivity_));
  new G4LogicalSkinSurface("OP_PANEL", entry_panel_logic, panel_opsur);
  new G4LogicalSkinSurface("OP_PANEL_H", h_l_panel_logic, panel_opsur);
  new G4LogicalSkinSurface("OP_PANEL_V", v_l_panel_logic, panel_opsur);
  
  if (visibility_){
    G4VisAttributes panel_col = nexus::Red();
    entry_panel_logic->SetVisAttributes(panel_col);
    h_l_panel_logic->SetVisAttributes(panel_col);
    v_l_panel_logic->SetVisAttributes(panel_col);
  }

}

void PETitPyrexMix::BuildSensors()
{

  // Construct here the tiles to use their distance to flange
  TileHamamatsuVUV tile0 = TileHamamatsuVUV();
  tile0.SetBoxConf(hama);
  tile0.SetTileVisibility(tile_vis_);
  tile0.SetTileReflectivity(tile_refl_);
  tile0.SetSiPMCells(false);

  tile0.Construct();

  G4double tile0_size_x = tile0.GetDimensions().x();
  G4double tile0_size_y = tile0.GetDimensions().y();
  G4double tile0_thickn = tile0.GetDimensions().z();
  G4double full_row_size = n_tile_columns_ * tile0_size_x;
  G4double full_col_size = n_tile_rows_ * tile0_size_y;

  G4LogicalVolume* tile0_logic = tile0.GetLogicalVolume();
  G4String vol_name;
  G4int copy_no = 0;
  G4double z_pos0 =
    -box_->GetBoxSize()/2. + box_->GetBoxThickness() + dist_ham_vuv_ + tile0_thickn/2.;

  for (G4int j = 0; j < n_tile_rows_; j++) {
    G4double y_pos = full_col_size/2. - tile0_size_y/2. - j*tile0_size_y;
    for (G4int i = 0; i < n_tile_columns_; i++) {
      G4double x_pos = -full_row_size/2. + tile0_size_x/2. + i*tile0_size_x;
      vol_name = "TILE_" + std::to_string(copy_no);

      new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos0), tile0_logic,
                        vol_name, active_logic_, false, copy_no, false);
      copy_no += 1;
    }
  }

  TileFBK tile2 = TileFBK();
  tile2.SetPDE(sipm_pde_);
  tile2.SetBoxConf(hama); // we use hama because the IDs must be 111, 112, ... 118 etc.
  tile2.SetTileVisibility(tile_vis_);
  tile2.SetTileReflectivity(tile_refl_);
  tile2.SetSiPMCells(false);

  tile2.Construct();

  G4double tile2_thickn = tile2.GetDimensions().z();
  G4LogicalVolume* tile2_logic = tile2.GetLogicalVolume();

  G4RotationMatrix rot;
  rot.rotateY(pi);

  G4double z_pos2 =
    box_->GetBoxSize()/2. - box_->GetBoxThickness() - dist_fbk_ - tile2_thickn/2.;

  vol_name = "TILE_" + std::to_string(copy_no);
      new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., 0., z_pos2)),
                        tile2_logic, vol_name, active_logic_, false, copy_no, false);

}

G4ThreeVector PETitPyrexMix::GenerateVertex(const G4String &region) const
{
  G4ThreeVector vertex(0., 0., 0.);

  if (region == "CENTER") {
    return vertex;
  } else if (region == "AD_HOC") {
    vertex = specific_vertex_;
  } else if (region == "SOURCE") {
    vertex = box_->GenerateVertex("SOURCE");
  } else {
    G4Exception("[PETitPyrexMix]", "GenerateVertex()", FatalException,
                "Unknown vertex generation region!");
  }
  return vertex;
}
