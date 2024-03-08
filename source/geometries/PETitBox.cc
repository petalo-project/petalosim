// ----------------------------------------------------------------------------
// petalosim | PETitBox.cc
//
// Basic geometry common to the different configurations of PETit.
// It includes the aluminum box, the xenon and the source tube.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PETitBox.h"
#include "PetMaterialsList.h"
#include "PetOpticalMaterialProperties.h"
#include "Na22Source.h"
#include "PetIonizationSD.h"

#include "nexus/Visibilities.h"
#include "nexus/OpticalMaterialProperties.h"
#include "nexus/SpherePointSampler.h"

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Material.hh>
#include <G4SDManager.hh>
#include <G4UserLimits.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4Material.hh>

using namespace nexus;

PETitBox::PETitBox() : GeometryBase(),
                       visibility_(0),
                       source_pos_{},
                       box_size_(194.4 * mm),
                       box_thickness_(2. * cm),
                       ih_x_size_(6. * cm),
                       ih_y_size_(12. * cm),
                       ih_z_size_(4. * cm),
                       ih_thick_wall_(3. * mm),
                       ih_thick_roof_(6. * mm),
                       source_tube_thick_wall_(1. * mm),
                       source_tube_int_radius_(1.4 * cm),
                       dist_source_roof_(10. * mm),
                       source_tube_thick_roof_(5. * mm),
                       max_step_size_(1. * mm),
                       pressure_(1 * bar)
{
}

PETitBox::~PETitBox()
{
}

void PETitBox::Construct()
{
  // BOX //
  G4Box* box_solid =
      new G4Box("BOX", box_size_/2., box_size_/2., box_size_/2.);

  G4Material* aluminum =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  aluminum->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  G4LogicalVolume* box_logic =
      new G4LogicalVolume(box_solid, aluminum, "BOX");
  this->SetLogicalVolume(box_logic);

  // LXe as ACTIVE //

  G4double LXe_size = box_size_ - 2. * box_thickness_;
  G4Box* active_solid =
      new G4Box("LXe", LXe_size/2., LXe_size/2., LXe_size/2.);
  active_logic_ =
      new G4LogicalVolume(active_solid, LXe_mat_, "ACTIVE");

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
                    active_logic_, "ACTIVE", box_logic, false, 0, false);

  active_logic_->SetSensitiveDetector(ionisd_);
  active_logic_->SetUserLimits(new G4UserLimits(max_step_size_));

  // Aluminum cylinder //
  G4double aluminum_cyl_rad = 40. * mm;
  G4double aluminum_cyl_len = 19. * mm;
  G4Tubs* aluminum_cyl_solid =
      new G4Tubs("ALUMINUM_CYL", 0, aluminum_cyl_rad, aluminum_cyl_len/2.,
                 0, twopi);

  G4LogicalVolume* aluminum_cyl_logic =
      new G4LogicalVolume(aluminum_cyl_solid, aluminum, "ALUMINUM_CYL");

  G4RotationMatrix rot;
  rot.rotateX(pi / 2.);
  G4double al_cyl_ypos =
    box_size_/2. - box_thickness_ - aluminum_cyl_len/2.;

  new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., al_cyl_ypos, 0.)),
                    aluminum_cyl_logic, "ALUMINUM_CYL", active_logic_,
                    false, 0, false);


  // INTERNAL HAT //
  G4Box* internal_hat_solid =
      new G4Box("INTERNAL_HAT", ih_x_size_/2., ih_y_size_/2., ih_z_size_/2.);

  G4LogicalVolume* internal_hat_logic =
      new G4LogicalVolume(internal_hat_solid, aluminum, "INTERNAL_HAT");

  new G4PVPlacement(0, G4ThreeVector(0., (-box_size_/2. + box_thickness_ + ih_y_size_/2.), 0.),
                    internal_hat_logic, "INTERNAL_HAT", active_logic_,
                    false, 0, false);

  G4double vacuum_hat_xsize = ih_x_size_ - 2. * ih_thick_wall_;
  G4double vacuum_hat_ysize = ih_y_size_ - ih_thick_roof_;
  G4double vacuum_hat_zsize = ih_z_size_ - 2. * ih_thick_wall_;
  G4Box* vacuum_hat_solid =
      new G4Box("VACUUM_HAT", vacuum_hat_xsize/2., vacuum_hat_ysize/2., vacuum_hat_zsize/2.);

  G4Material* vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  vacuum->SetMaterialPropertiesTable(opticalprops::Vacuum());
  G4LogicalVolume* vacuum_hat_logic =
      new G4LogicalVolume(vacuum_hat_solid, vacuum, "VACUUM_HAT");

  new G4PVPlacement(0, G4ThreeVector(0., -ih_thick_roof_/2., 0.),
                    vacuum_hat_logic,
                    "VACUUM_HAT", internal_hat_logic, false, 0, false);

  // SOURCE TUBE //
  G4double source_tube_ext_radius = source_tube_int_radius_ + source_tube_thick_wall_;
  G4double source_tube_length = ih_y_size_ - ih_thick_roof_ - dist_source_roof_;
  G4Tubs* source_tube_solid =
      new G4Tubs("SOURCE_TUBE", 0, source_tube_ext_radius, source_tube_length/2., 0, twopi);

  G4Material* carbon_fiber = petmaterials::CarbonFiber();
  carbon_fiber->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  G4LogicalVolume* source_tube_logic =
      new G4LogicalVolume(source_tube_solid, carbon_fiber, "SOURCE_TUBE");

  G4double source_tube_ypos = source_tube_length/2. - (ih_y_size_ - ih_thick_roof_)/2.;
  new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., source_tube_ypos, 0.)),
                    source_tube_logic, "SOURCE_TUBE", vacuum_hat_logic,
                    false, 0, false);

  G4double air_source_tube_len = source_tube_length - source_tube_thick_roof_;
  G4Tubs* air_source_tube_solid =
      new G4Tubs("AIR_SOURCE_TUBE", 0, source_tube_int_radius_,
                 air_source_tube_len/2., 0, twopi);

  G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  air->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  G4LogicalVolume* air_source_tube_logic =
      new G4LogicalVolume(air_source_tube_solid, air, "AIR_SOURCE_TUBE");

  new G4PVPlacement(0, G4ThreeVector(0., 0., source_tube_thick_roof_/2.),
                    air_source_tube_logic,
                    "AIR_SOURCE_TUBE", source_tube_logic, false, 0, false);

  // ENCAPSULATED SOURCE //

  Na22Source na22 = Na22Source();
  na22.Construct();
  G4LogicalVolume* na22_logic = na22.GetLogicalVolume();
  G4double source_offset_y = -0.9 * mm;
  G4double na22_pos =
    - box_size_/2 + box_thickness_ + air_source_tube_len / 2. - source_offset_y;

  new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., 0., na22_pos)),
                    na22_logic,
                    "NA22_SOURCE_SUPPORT", air_source_tube_logic,
                    false, 0, false);

  source_gen_ =
    new SpherePointSampler(0, na22.GetSourceDiameter()/2, 0., twopi, 0, pi,
                           G4ThreeVector(0, source_offset_y, 0.), 0);

  // SOURCE TUBE INSIDE BOX ////////////////////////////////////
  G4Tubs* source_tube_inside_box_solid =
      new G4Tubs("SOURCE_TUBE", 0, source_tube_ext_radius, box_thickness_/2., 0, twopi);

  G4LogicalVolume* source_tube_inside_box_logic =
      new G4LogicalVolume(source_tube_inside_box_solid, carbon_fiber, "SOURCE_TUBE");

  new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., (-box_size_ + box_thickness_)/2., 0.)),
                    source_tube_inside_box_logic, "SOURCE_TUBE", box_logic, false, 0, false);

  G4Tubs* air_source_tube_inside_box_solid =
      new G4Tubs("AIR_SOURCE_TUBE", 0, source_tube_int_radius_, box_thickness_/2., 0, twopi);

  G4LogicalVolume* air_source_tube_inside_box_logic =
      new G4LogicalVolume(air_source_tube_inside_box_solid, air, "AIR_SOURCE_TUBE");

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), air_source_tube_inside_box_logic,
                    "AIR_SOURCE_TUBE", source_tube_inside_box_logic, false, 0, false);


  if (visibility_) {
    G4VisAttributes box_col = nexus::White();
    box_logic->SetVisAttributes(box_col);
    G4VisAttributes al_cyl_col = nexus::DarkGrey();
    //al_cyl_col.SetForceSolid(true);
    aluminum_cyl_logic->SetVisAttributes(al_cyl_col);
    G4VisAttributes active_col = nexus::Blue();
    active_logic_->SetVisAttributes(active_col);
    G4VisAttributes ih_col = nexus::Yellow();
    internal_hat_logic->SetVisAttributes(ih_col);
    G4VisAttributes vacuum_col = nexus::Lilla();
    vacuum_hat_logic->SetVisAttributes(vacuum_col);
    G4VisAttributes source_tube_col = nexus::Red();
    //source_tube_col.SetForceSolid(true);
    source_tube_logic->SetVisAttributes(source_tube_col);
    G4VisAttributes air_source_tube_col = nexus::DarkGrey();
    // air_source_tube_col.SetForceSolid(true);
    air_source_tube_logic->SetVisAttributes(air_source_tube_col);
    G4VisAttributes air_source_tube_inside_box_col = nexus::White();
    //air_source_tube_inside_box_col.SetForceSolid(true);
    source_tube_inside_box_logic->SetVisAttributes(air_source_tube_inside_box_col);
  }
}

G4ThreeVector PETitBox::GenerateVertex(const G4String &region) const
{
  G4ThreeVector vertex;
  if (region == "SOURCE") {
      vertex = source_gen_->GenerateVertex(VOLUME);
  } else {
    G4Exception("[PETitBox]", "GenerateVertex()", FatalException,
                "Unknown vertex generation region!");
  }
  return vertex;
}

