// ----------------------------------------------------------------------------
// petalosim | FullRingInfinity.cc
//
// This class implements the geometry of a cylindric ring of LXe,
// of configurable length and diameter.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "FullRingInfinity.h"
#include "SiPMpetVUV.h"
#include "PetMaterialProperties.h"
#include "PetIonizationSD.h"
#include "ChargeSD.h"
#include "JaszczakPhantom.h"

#include "nexus/SpherePointSampler.h"
#include "nexus/Visibilities.h"
#include "nexus/FactoryBase.h"
#include "nexus/MaterialProperties.h"
#include "nexus/MaterialsList.h"

#include <G4GenericMessenger.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Orb.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <G4LogicalVolume.hh>
#include <G4UserLimits.hh>
#include <G4SDManager.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4OpticalSurface.hh>
#include <Randomize.hh>
#include <G4UnionSolid.hh>

using namespace nexus;

REGISTER_CLASS(FullRingInfinity, GeometryBase)

FullRingInfinity::FullRingInfinity() :
  GeometryBase(),
  // Detector dimensions
  sipm_pitch_(4. * mm),
  n_sipm_rows_(16),
  instr_faces_(2),
  charge_det_(false),
  separators_(false),
  wire_pitch_(4. * mm),
  wire_time_bin_(1.*microsecond),
  chdet_thickn_(1.*micrometer),
  chdet_offset_(0.005 * mm),
  kapton_thickn_(0.3 * mm),
  lxe_depth_(5. * cm),
  offset_(0.1 * mm),
  inner_radius_(15. * cm),
  lxe_container_int_thickn_(1.*mm),
  lxe_container_ext_thickn_(2.*cm),
  vessel_int_thickn_(3.*mm),
  vessel_ext_thickn_(1.*cm),
  vacuum_thickn_(10.*cm),
  wall_refl_(0),
  max_step_size_(1. * mm),
  n_sep_z_(10),
  n_sep_phi_(10),
  specific_vertex_{},
  phantom_(false),
  pt_Lx_(0.),
  pt_Ly_(0.),
  pt_Lz_(0.),
  sensitivity_(false),
  events_per_point_(1),
  sensitivity_point_id_(0),
  sensitivity_index_(0),
  sensitivity_binning_(1 * mm),
  sens_x_min_(-inner_radius_),
  sens_x_max_(inner_radius_),
  sens_y_min_(-inner_radius_),
  sens_y_max_(inner_radius_)
{
  // Messenger
  msg_ = new G4GenericMessenger(this, "/Geometry/FullRingInfinity/",
    "Control commands of geometry FullRingInfinity.");
  G4GenericMessenger::Command &depth_cmd =
      msg_->DeclareProperty("depth", lxe_depth_, "Dimension in DOI");
  depth_cmd.SetUnitCategory("Length");
  depth_cmd.SetParameterName("depth", false);
  depth_cmd.SetRange("depth>0.");

  G4GenericMessenger::Command &pitch_cmd =
      msg_->DeclareProperty("sipm_pitch", sipm_pitch_, "Pitch of SiPMs");
  pitch_cmd.SetUnitCategory("Length");
  pitch_cmd.SetParameterName("pitch", false);
  pitch_cmd.SetRange("pitch>0.");

  G4GenericMessenger::Command &inner_r_cmd =
      msg_->DeclareProperty("inner_radius", inner_radius_,
                            "Inner radius of ring");
  inner_r_cmd.SetUnitCategory("Length");
  inner_r_cmd.SetParameterName("inner_radius", false);
  inner_r_cmd.SetRange("inner_radius>0.");

  msg_->DeclareProperty("sipm_rows", n_sipm_rows_, "Number of SiPM rows");
  msg_->DeclareProperty("instrumented_faces", instr_faces_,
                        "Number of instrumented faces");
  msg_->DeclareProperty("charge_detector", charge_det_,
                        "True if charge is detected");
  msg_->DeclareProperty("separators", separators_,
                        "True if separator panels are present");
  msg_->DeclareProperty("phantom", phantom_,
                        "True if Jaszczak phantom is used");

  G4GenericMessenger::Command& wire_pitch_cmd =
      msg_->DeclareProperty("wire_pitch", wire_pitch_, "Pitch of wires");
  wire_pitch_cmd.SetUnitCategory("Length");
  wire_pitch_cmd.SetParameterName("wire_pitch", false);
  wire_pitch_cmd.SetRange("wire_pitch>0.");

  G4GenericMessenger::Command& wire_time_cmd =
      msg_->DeclareProperty("wire_time_bin", wire_time_bin_,
                            "Time binning of wires");
  wire_time_cmd.SetUnitCategory("Time");
  wire_time_cmd.SetParameterName("wire_time_bin", false);
  wire_time_cmd.SetRange("wire_time_bin>0.");

  msg_->DeclareProperty("z_separators", n_sep_z_,
    "Number of teflon separators in the z direction");

  msg_->DeclareProperty("phi_separators", n_sep_phi_,
    "Number of teflon separators in the phi direction");

  msg_->DeclarePropertyWithUnit("specific_vertex", "mm",  specific_vertex_,
                                "Generation vertex.");

  // Read in the point distribution.
  msg_->DeclareMethod("pointFile", &FullRingInfinity::BuildPointfile,
    "Location of file containing distribution of event generation points.");

  G4GenericMessenger::Command &table_cmd =
      msg_->DeclareProperty("sensitivity_binning", sensitivity_binning_,
                            "Pitch for sensitivity point generation");
  table_cmd.SetUnitCategory("Length");
  table_cmd.SetParameterName("sensitivity_binning", false);
  table_cmd.SetRange("sensitivity_binning>0.");

  msg_->DeclareProperty("sensitivity", sensitivity_,
                        "True if sensitivity map is being run");
  msg_->DeclareProperty("sensitivity_point_id", sensitivity_point_id_,
                        "Starting point for sensitivity run");
  msg_->DeclareProperty("events_per_point", events_per_point_,
                        "Number of events to be generated per point");

  G4GenericMessenger::Command &sns_x_min_cmd =
      msg_->DeclareProperty("sens_x_min", sens_x_min_,
                            "Minimum x for sensitivity map");
  sns_x_min_cmd.SetUnitCategory("Length");
  sns_x_min_cmd.SetParameterName("sens_x_min", false);

  G4GenericMessenger::Command &sns_x_max_cmd =
      msg_->DeclareProperty("sens_x_max", sens_x_max_,
                            "Maxmimum x for sensitivity map");
  sns_x_max_cmd.SetUnitCategory("Length");
  sns_x_max_cmd.SetParameterName("sens_x_max", false);

  G4GenericMessenger::Command &sns_y_min_cmd =
      msg_->DeclareProperty("sens_y_min", sens_y_min_,
                            "Minimum y for sensitivity map");
  sns_y_min_cmd.SetUnitCategory("Length");
  sns_y_min_cmd.SetParameterName("sens_y_min", false);

  G4GenericMessenger::Command &sns_y_max_cmd =
      msg_->DeclareProperty("sens_y_max", sens_y_max_,
                            "Maxmimum y for sensitivity map");
  sns_y_max_cmd.SetUnitCategory("Length");
  sns_y_max_cmd.SetParameterName("sens_y_max", false);

  G4GenericMessenger::Command &sns_z_min_cmd =
      msg_->DeclareProperty("sens_z_min", sens_z_min_,
                            "Minimum z for sensitivity map");
  sns_z_min_cmd.SetUnitCategory("Length");
  sns_z_min_cmd.SetParameterName("sens_z_min", false);

  G4GenericMessenger::Command &sns_z_max_cmd =
      msg_->DeclareProperty("sens_z_max", sens_z_max_,
                            "Maxmimum z for sensitivity map");
  sns_z_max_cmd.SetUnitCategory("Length");
  sns_z_max_cmd.SetParameterName("sens_z_max", false);

  sipm_ = new SiPMpetVUV();
}

FullRingInfinity::~FullRingInfinity()
{
  delete[] pt_;
}

void FullRingInfinity::Construct()
{
  // LAB. This is just a volume of air surrounding the detector
  G4double lab_size = 10. * m;
  G4Box* lab_solid =
    new G4Box("LAB", lab_size / 2., lab_size / 2., lab_size / 2.);

  lab_logic_ =
      new G4LogicalVolume(lab_solid,
          G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR"), "LAB");
  lab_logic_->SetVisAttributes(G4VisAttributes::GetInvisible());
  this->SetLogicalVolume(lab_logic_);

  axial_length_ = sipm_pitch_ *  n_sipm_rows_;
  G4cout << "Axial dimensions (mm) = " << axial_length_/mm << G4endl;

  external_radius_ = inner_radius_ + lxe_depth_;
  G4cout << "Radial dimensions (mm): "<< inner_radius_/mm << ", "
	  << external_radius_/mm << G4endl;

  sipm_->SetSensorDepth(1);
  sipm_->Construct();
  sipm_dim_ = sipm_->GetDimensions();
  G4cout << "SiPM size = " << sipm_dim_ << G4endl;

  BuildCryostat();
  BuildSensors();

  if (charge_det_) {
    if (separators_) {
      BuildSeparators();
    }
    BuildWires();
    wall_refl_ = 0.8;
  }

  if (phantom_)
    BuildPhantom();

  if (sensitivity_)
    CalculateSensitivityVertices(sensitivity_binning_);
  }

void FullRingInfinity::BuildCryostat()
{
    const G4double extra_int_space = 2. * mm;
    const G4double elec_ext_space  = 2. * cm;

    const G4double lxe_int_radius = inner_radius_ - extra_int_space;
    const G4double lxe_ext_radius = external_radius_ + elec_ext_space;
    const G4double lxe_width      = axial_length_ + 2.*kapton_thickn_;

    const G4double lxe_container_int_radius =
      lxe_int_radius - lxe_container_int_thickn_;
    const G4double lxe_container_ext_radius =
      lxe_ext_radius + lxe_container_ext_thickn_;
    const G4double lxe_container_width =
      lxe_width + 2.*lxe_container_ext_thickn_;

    const G4double vacuum_int_radius =
      lxe_container_int_radius - vacuum_thickn_;
    const G4double vacuum_ext_radius =
      lxe_container_ext_radius + vacuum_thickn_ ;

    const G4double vessel_int_radius = vacuum_int_radius - vessel_int_thickn_;
    const G4double vessel_ext_radius = vacuum_ext_radius + vessel_ext_thickn_;
    const G4double vessel_width =
      lxe_container_width + 2.*vessel_ext_thickn_;

    G4Tubs* vessel_solid =
      new G4Tubs("VACUUM_VESSEL", vessel_int_radius, vessel_ext_radius,
                 vessel_width/2., 0, twopi);
    G4Material* steel = materials::Steel();
    steel->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
    G4LogicalVolume* vessel_logic =
      new G4LogicalVolume(vessel_solid, steel, "VACUUM_VESSEL");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), vessel_logic,
		      "VACUUM_VESSEL", lab_logic_, false, 0, true);


    G4Tubs* vacuum_solid =
      new G4Tubs("VACUUM", vacuum_int_radius, vacuum_ext_radius,
                 lxe_container_width/2., 0, twopi);
    G4Material* vacuum =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
    vacuum->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
    G4LogicalVolume* vacuum_logic =
          new G4LogicalVolume(vacuum_solid, vacuum, "VACUUM");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), vacuum_logic,
		      "VACUUM", vessel_logic, false, 0, true);

    G4Tubs* lxe_container_solid =
      new G4Tubs("CONTAINER", lxe_container_int_radius,
                 lxe_container_ext_radius, lxe_container_width/2., 0, twopi);

    G4Material* aluminum =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
    aluminum->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
    G4LogicalVolume* lxe_container_logic =
      new G4LogicalVolume(lxe_container_solid, aluminum, "CONTAINER");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), lxe_container_logic,
		      "CONTAINER", vacuum_logic, false, 0, true);


    G4Tubs* LXe_solid =
      new G4Tubs("LXE", lxe_int_radius, lxe_ext_radius,
                 lxe_width/2., 0, twopi);
    LXe_ = G4NistManager::Instance()->FindOrBuildMaterial("G4_lXe");
    LXe_->SetMaterialPropertiesTable(materialprops::LXe());
    LXe_logic_ =
      new G4LogicalVolume(LXe_solid, LXe_, "LXE");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), LXe_logic_,
		      "LXE", lxe_container_logic, false, 0, true);


    G4double wide_active_depth = lxe_depth_ + sipm_dim_.z() + offset_;
    G4Tubs* active_solid =
      new G4Tubs("ACTIVE", inner_radius_, inner_radius_ + wide_active_depth,
                 axial_length_/2., 0, twopi);
    active_logic_ =
      new G4LogicalVolume(active_solid, LXe_, "ACTIVE");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), active_logic_,
                      "ACTIVE", LXe_logic_, false, 0, true);

    // Set the ACTIVE volume as an ionization sensitive det
    PetIonizationSD* ionisd = new PetIonizationSD("/PETALO/ACTIVE");
    active_logic_->SetSensitiveDetector(ionisd);
    G4SDManager::GetSDMpointer()->AddNewDetector(ionisd);

    // Limit the step size in ACTIVE volume for better tracking precision
    active_logic_->SetUserLimits(new G4UserLimits(max_step_size_));

    // Reflectant panels
    G4Material* kapton =
        G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
    kapton->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());

    G4Tubs* kapton_int_solid =
        new G4Tubs("KAPTON", inner_radius_ - kapton_thickn_, inner_radius_,
                    axial_length_ / 2., 0, twopi);
    G4LogicalVolume* kapton_int_logic =
        new G4LogicalVolume(kapton_int_solid, kapton, "KAPTON");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), kapton_int_logic,
                      "KAPTON_INT", LXe_logic_, false, 0, false);

    G4Tubs* kapton_ext_solid =
      new G4Tubs("KAPTON", inner_radius_ + wide_active_depth,
                 inner_radius_ + wide_active_depth + kapton_thickn_,
                 axial_length_/2., 0, twopi);
    G4LogicalVolume* kapton_ext_logic =
      new G4LogicalVolume(kapton_ext_solid, kapton, "KAPTON");
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), kapton_ext_logic,
                    "KAPTON_EXT", LXe_logic_, false, 0, false);

    G4Tubs* kapton_lat_solid =
      new G4Tubs("KAPTON", inner_radius_ - kapton_thickn_,
                 inner_radius_ + wide_active_depth + kapton_thickn_,
                 kapton_thickn_/2., 0, twopi);
    G4LogicalVolume* kapton_lat_logic =
      new G4LogicalVolume(kapton_lat_solid, kapton, "KAPTON");
    G4double z_pos = axial_length_ / 2. + kapton_thickn_ / 2.;
    new G4PVPlacement(0, G4ThreeVector(0., 0., z_pos), kapton_lat_logic,
                      "KAPTON_LAT_POS", LXe_logic_, false, 0, false);
    new G4PVPlacement(0, G4ThreeVector(0., 0., -z_pos), kapton_lat_logic,
                      "KAPTON_LAT_NEG", LXe_logic_, false, 1, false);

    // OPTICAL SURFACE FOR REFLECTION
    G4OpticalSurface* db_opsur = new G4OpticalSurface("BORDER");
    db_opsur->SetType(dielectric_metal);
    db_opsur->SetModel(unified);
    db_opsur->SetFinish(ground);
    db_opsur->SetSigmaAlpha(0.1);
    db_opsur->SetMaterialPropertiesTable(petmaterialprops::ReflectantSurface(wall_refl_));
    new G4LogicalSkinSurface("BORDER", kapton_lat_logic, db_opsur);
    new G4LogicalSkinSurface("BORDER", kapton_int_logic, db_opsur);
    new G4LogicalSkinSurface("BORDER", kapton_ext_logic, db_opsur);

    // G4cout << (external_radius_  - kapton_thickn_) / cm << G4endl;

    G4VisAttributes kapton_col = nexus::CopperBrown();
    //kapton_col.SetForceSolid(true);
    kapton_int_logic->SetVisAttributes(kapton_col);
    kapton_ext_logic->SetVisAttributes(kapton_col);
    kapton_lat_logic->SetVisAttributes(kapton_col);
    // G4VisAttributes active_col = nexus::Blue();
    // active_col.SetForceSolid(true);
    // active_logic->SetVisAttributes(active_col);
}

void FullRingInfinity::BuildSensors()
{
  G4LogicalVolume* sipm_logic = sipm_->GetLogicalVolume();
  //G4double sipm_pitch = sipm_dim.x() + 1. * mm;

  G4int n_sipm_int = 2 * pi * inner_radius_ / sipm_pitch_;
  if (instr_faces_ == 2)
  {
    G4cout << "Number of sipms in inner face: " << n_sipm_int * n_sipm_rows_
           << G4endl;
  }
  G4double step   = 2. * pi / n_sipm_int;
  G4double radius = inner_radius_ + sipm_dim_.z() / 2.;

  G4RotationMatrix rot;
  rot.rotateX(-pi / 2.);

  G4int copy_no = 999;
  for (G4int j = 0; j < n_sipm_rows_; j++)
  {
    // The first must be positioned outside the loop
    if (j != 0)
      rot.rotateZ(step);
    G4double z_dimension = -axial_length_ / 2. + (j + 1. / 2.) * sipm_pitch_;
    G4ThreeVector position(0., radius, z_dimension);
    copy_no += 1;
    G4String vol_name = "SIPM_" + std::to_string(copy_no);
    if (instr_faces_ == 2)
    {
      new G4PVPlacement(G4Transform3D(rot, position), sipm_logic,
                        vol_name, active_logic_, false, copy_no, false);
    }

    for (G4int i = 2; i <= n_sipm_int; ++i)
    {
      G4double angle = (i - 1) * step;
      rot.rotateZ(step);
      position.setX(-radius * sin(angle));
      position.setY(radius * cos(angle));
      copy_no += 1;
      vol_name = "SIPM_" + std::to_string(copy_no);
      if (instr_faces_ == 2)
      {
        new G4PVPlacement(G4Transform3D(rot, position), sipm_logic,
                          vol_name, active_logic_, false, copy_no, false);
      }
    }
  }

  //G4double sipm_pitch_ext = sipm_dim.x() + 0.5 * mm;
  //G4double offset = 0.1 * mm;
  n_sipm_ext_ = 2 * pi * external_radius_ / sipm_pitch_;
  G4cout << "Number of sipms in external face: " << n_sipm_ext_ * n_sipm_rows_
         << G4endl;
  //radius = external_radius_ - sipm_dim_.z() / 2. - offset_;
  radius = inner_radius_ + lxe_depth_ + sipm_dim_.z()/2.;


  rot.rotateZ(step);
  rot.rotateX(pi);

  step = 2. * pi / n_sipm_ext_;

  //copy_no = 2000;
  if (instr_faces_ == 1)
  {
    copy_no = 999;
  }

  for (G4int j = 0; j < n_sipm_rows_; j++)
  {
    // The first must be positioned outside the loop
    if (j != 0)
      rot.rotateZ(step);
    G4double z_pos = -axial_length_ / 2. + (j + 1. / 2.) * sipm_pitch_;
    G4ThreeVector position(0., radius, z_pos);
    copy_no = copy_no + 1;
    G4String vol_name       = "SIPM_" + std::to_string(copy_no);
    new G4PVPlacement(G4Transform3D(rot, position), sipm_logic,
                      vol_name, active_logic_, false, copy_no, false);

    // G4cout << "INSERT INTO ChannelMatrixP7R410Z1950mm (MinRun, MaxRun, SensorID, PhiNumber, ZNumber) VALUES (0, 100000, "
    //	     << copy_no << ", 0, " << j << ");" << G4endl;
    //G4cout << "INSERT INTO ChannelPositionP7R410Z1950mm (MinRun, MaxRun, SensorID, X, Y, Z) VALUES (0, 100000, "
    //	       << copy_no << ", " << position.getX() << ", " << position.getY() << ", " << position.getZ() << ");" << G4endl;
    for (G4int i = 2; i <= n_sipm_ext_; ++i)
    {
      G4double angle = (i - 1) * step;
      rot.rotateZ(step);
      position.setX(-radius * sin(angle));
      position.setY(radius * cos(angle));
      copy_no = copy_no + 1;
      vol_name       = "SIPM_" + std::to_string(copy_no);
      new G4PVPlacement(G4Transform3D(rot, position), sipm_logic,
                        vol_name, active_logic_, false, copy_no, false);

      //	G4cout << "INSERT INTO ChannelMatrixP7R410Z1950mm (MinRun, MaxRun, SensorID, PhiNumber, ZNumber) VALUES (0, 100000, "
      //       << copy_no << ", " << i-1 << ", " << j << ");" << G4endl;
      //	G4cout << "INSERT INTO ChannelPositionP7R410Z1950mm (MinRun, MaxRun, SensorID, X, Y, Z) VALUES (0, 100000, "
      //	       << copy_no << ", " << position.getX() << ", " << position.getY() << ", " << position.getZ() << ");" << G4endl;
    }
  }
}

void FullRingInfinity::BuildWires()
{
  // Add simple detector for charge
  G4Box* chdet_solid = new G4Box("WIRE", wire_pitch_/2.,
                                 axial_length_/2., chdet_thickn_/2);
  G4LogicalVolume* chdet_logic =
    new G4LogicalVolume(chdet_solid, LXe_, "WIRE");

  G4String sdname = "/WIRE/ChargeDet";
  G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
  if (!sdmgr->FindSensitiveDetector(sdname, false))
    {
      ChargeSD* chargesd = new ChargeSD(sdname);
      chargesd->SetTimeBinning(wire_time_bin_);
      G4SDManager::GetSDMpointer()->AddNewDetector(chargesd);
      chdet_logic->SetSensitiveDetector(chargesd);
    }

  G4VisAttributes wire_col = nexus::Yellow();
  wire_col.SetForceSolid(true);
  chdet_logic->SetVisAttributes(wire_col);

  G4double chdet_radius =
    inner_radius_ + lxe_depth_ - chdet_thickn_ / 2. - chdet_offset_;
  G4int n_wires = 2. * pi * chdet_radius / wire_pitch_;
  G4cout << "Number of wires: " << n_wires << G4endl;

  G4ThreeVector chdet_position(0., chdet_radius, 0.);
  G4int chdet_copy_no = 0;
  G4String chdet_vol_name = "WIRE_" + std::to_string(chdet_copy_no);
  G4RotationMatrix rot;
  rot.rotateX(-pi / 2.);
  new G4PVPlacement(G4Transform3D(rot, chdet_position), chdet_logic,
                    chdet_vol_name, active_logic_, false, chdet_copy_no, false);

  G4double step = 2. * pi / n_wires;
  for (G4int i = 1; i < n_wires; ++i) {
    G4double angle = i * step;
    rot.rotateZ(step);
    chdet_position.setX(-chdet_radius * sin(angle));
    chdet_position.setY(chdet_radius * cos(angle));
    chdet_copy_no = chdet_copy_no + 1;
    chdet_vol_name = "WIRE_" + std::to_string(chdet_copy_no);
    new G4PVPlacement(G4Transform3D(rot, chdet_position), chdet_logic,
                      chdet_vol_name, active_logic_, false, chdet_copy_no,
                      false);
  }
}

void FullRingInfinity::BuildSeparators()
{
  // Separate LXe volume in smaller areas with teflon panels
  G4Material* teflon =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");
  teflon->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());

  //G4double arc_sep_phi = 2 * pi * (inner_radius_ + lxe_depth_) / n_sep_phi_;
  //G4int n_sipm_in_sep_phi = arc_sep_phi / sipm_pitch_;
  G4double sep_angle = 2 * pi / n_sep_phi_;

  G4double segm_sep_z  = axial_length_ / n_sep_z_;
  //G4double sep_pitch_z = G4int(segm_sep_z / sipm_pitch_) * sipm_pitch_;

  G4double sep_thickn = 5.*mm;
  G4double sep_offset = 0.01*mm;
  G4double sep_size   = lxe_depth_ - chdet_thickn_ - chdet_offset_ - sep_offset;
  G4Tubs* sep_z_solid =
    new G4Tubs("SEPARATOR_Z", inner_radius_, inner_radius_ + sep_size,
               sep_thickn/2., 0, twopi);
  G4Box* sep_phi_solid =
    new G4Box("SEPARATOR_PHI", sep_thickn/2., sep_size/2., axial_length_/2.);

  G4double union_r = inner_radius_ + sep_size/2.;
  G4double union_z = axial_length_/2. - segm_sep_z;
  G4ThreeVector union_pos(0, union_r, union_z);
  G4UnionSolid* sep_solid =
    new G4UnionSolid("SEPARATOR", sep_z_solid, sep_phi_solid, 0, union_pos);

  G4RotationMatrix union_rot;

  for (G4int i=1; i<n_sep_phi_; i++) {
    union_rot.rotateZ(sep_angle);
    G4double union_angle = i * sep_angle;
    union_pos.setX(-union_r * sin(union_angle));
    union_pos.setY(union_r * cos(union_angle));
    sep_solid = new G4UnionSolid("SEPARATOR", sep_solid, sep_phi_solid,
                                 G4Transform3D(union_rot, union_pos));
  }

  union_pos = G4ThreeVector(0, 0, 0);
  for (G4int i=1; i<n_sep_z_-1; i++) {
    G4double z = i*segm_sep_z;
    union_pos.setZ(z);
    sep_solid = new G4UnionSolid("SEPARATOR", sep_solid, sep_z_solid,
                                 G4Transform3D(union_rot, union_pos));
  }

  G4LogicalVolume* sep_logic =
    new G4LogicalVolume(sep_solid, teflon, "SEPARATOR");

  new G4PVPlacement(0, G4ThreeVector(0, 0, -axial_length_/2. + segm_sep_z),
                    sep_logic, "SEPARATOR", active_logic_, false, 0, false);

  G4OpticalSurface* teflon_optSurf =
    new G4OpticalSurface("TEFLON_OPSURF", unified, ground, dielectric_metal);
  teflon_optSurf->SetMaterialPropertiesTable(petmaterialprops::PTFE());
  new G4LogicalSkinSurface("TEFLON_OPSURF", sep_logic, teflon_optSurf);

  G4VisAttributes sep_col{{1. ,  .0 ,  .0 , .5}};
  sep_col.SetForceSolid(true);
  sep_logic->SetVisAttributes(sep_col);
}


void FullRingInfinity::BuildPhantom()
{
  jas_phantom_ = new JaszczakPhantom();
  jas_phantom_->Construct();
  G4LogicalVolume* phantom_logic = jas_phantom_->GetLogicalVolume();
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), phantom_logic, "JASZCZAK",
                    lab_logic_, false, 0, true);
  // phantom_diam_ = 6. * cm;

  // G4Orb *phantom_solid = new G4Orb("PHANTOM", phantom_diam_ / 2.);
  // G4LogicalVolume *phantom_logic =
  //     new G4LogicalVolume(phantom_solid, materials::PEEK(), "PHANTOM");
  // G4ThreeVector phantom_origin = specific_vertex_;
  // new G4PVPlacement(0, phantom_origin, phantom_logic, "PHANTOM", lab_logic_, false, 0, true);

  // spheric_gen_ =
  //     new SpherePointSampler(0., phantom_diam_ / 2, phantom_origin);
}

G4ThreeVector FullRingInfinity::GenerateVertex(const G4String &region) const
{

  G4ThreeVector vertex(0., 0., 0.);

  if (region == "CENTER")
  {
    return vertex;
  }
  else if (region == "AD_HOC")
  {
    vertex = specific_vertex_;
  }
  else if (region == "JPHANTOM")
  {
    vertex = jas_phantom_->GenerateVertex("JPHANTOM");
  }
  else if (region == "CUSTOM")
  {
    vertex = RandomPointVertex();
  }
  else if (region == "SENSITIVITY")
  {
    unsigned int i = sensitivity_point_id_ + sensitivity_index_;

    if (i == (sensitivity_vertices_.size() * events_per_point_ - 1))
    {
      G4Exception("[FullRingInfinity]", "GenerateVertex()",
                  RunMustBeAborted, "Reached last event in sensitivity map.");
    }

    try
    {
      auto pos = i % sensitivity_vertices_.size();
      vertex = sensitivity_vertices_.at(pos);
      sensitivity_index_++;
    }
    catch (const std::out_of_range &oor)
    {
      G4Exception("[FullRingInfinity]", "GenerateVertex()",
                  FatalErrorInArgument, "Sensitivity point out of range.");
    }
  }
  else
  {
    G4Exception("[FullRingInfinity]", "GenerateVertex()", FatalException,
                "Unknown vertex generation region!");
  }
  return vertex;
}

G4int FullRingInfinity::binarySearchPt(G4int low,
                                       G4int high, G4double rnd) const
{

  // Error
  if (high < 0 || low >= pt_Nx_ * pt_Ny_ * pt_Nz_)
    return -1;

  // Return the element before the first element greater than rnd.
  if (pt_[low] > rnd)
    return low - 1;

  G4int mid = (low + high) / 2;
  if (pt_[mid] <= rnd)
    return binarySearchPt(mid + 1, high, rnd);
  return binarySearchPt(low, mid - 1, rnd);
}

// Generates a vertex corresponding to a random point from the custom volume.
G4ThreeVector FullRingInfinity::RandomPointVertex() const
{
  // Select the index in the cumulative distribution.
  G4double rnd = G4UniformRand();
  G4int ipt = binarySearchPt(0, pt_Nx_ * pt_Ny_ * pt_Nz_ - 1, rnd);

  if (ipt < 0)
  {
    std::cerr << "ERROR: random point vertex selection failed." << std::endl;
    return G4ThreeVector(0, 0, 0);
  }
  //G4int jpt = 0;
  //while(pt_[jpt] < rnd) jpt++;
  //jpt -= 1;
  //if(ipt != jpt) std::cout << "WARNING: algorithms don't match i = " << ipt
  // << ", j = " << jpt << "***" << std::endl;

  // Compute the vertex.
  G4int nx = ipt / (pt_Ny_ * pt_Nz_);
  G4int ny = (ipt / pt_Nz_) % pt_Ny_;
  G4int nz = ipt % pt_Nz_;
  G4double x = pt_Lx_ * (((G4double)nx) / pt_Nx_ - 0.5);
  G4double y = pt_Ly_ * (((G4double)ny) / pt_Ny_ - 0.5);
  G4double z = pt_Lz_ * (((G4double)nz) / pt_Nz_ - 0.5);

  G4double xrnd = G4UniformRand() - 0.5;
  G4double yrnd = G4UniformRand() - 0.5;
  G4double zrnd = G4UniformRand() - 0.5;

  //std::cout << "Generated at point (" << x << ", " << y << ", " << z << "),
  // index " << ipt << std::endl;
  return G4ThreeVector(x + xrnd, y + yrnd, z + zrnd);
}

void FullRingInfinity::BuildPointfile(G4String pointFile)
{
  int Nx, Ny, Nz;
  float Lx, Ly, Lz;

  // Open the file containing the point distribution.
  std::ifstream is;
  is.open(pointFile, std::ifstream::binary);

  // Read the header.
  is.read(reinterpret_cast<char *>(&Nx), sizeof(int));
  is.read(reinterpret_cast<char *>(&Ny), sizeof(int));
  is.read(reinterpret_cast<char *>(&Nz), sizeof(int));
  is.read(reinterpret_cast<char *>(&Lx), sizeof(float));
  is.read(reinterpret_cast<char *>(&Ly), sizeof(float));
  is.read(reinterpret_cast<char *>(&Lz), sizeof(float));
  pt_Nx_ = Nx;
  pt_Ny_ = Ny;
  pt_Nz_ = Nz;
  pt_Lx_ = Lx;
  pt_Ly_ = Ly;
  pt_Lz_ = Lz;

  // Read the distribution.
  int i = 0;
  float f;
  int length = pt_Nx_ * pt_Ny_ * pt_Nz_;
  pt_ = new G4float[length];
  while (is.read(reinterpret_cast<char *>(&f), sizeof(float)))
  {
    pt_[i] = f;
    i++;
  }

  is.close();

  std::cout << "Read distribution of (" << pt_Nx_ << ", " << pt_Ny_ << ", "
            << pt_Nz_ << "); Len (" << pt_Lx_ << ", " << pt_Ly_ << ", "
            << pt_Lz_ << "); with total elements = " << length
            << ", and first two = " << pt_[0] << " , " << pt_[1] << std::endl;
}

void FullRingInfinity::CalculateSensitivityVertices(G4double binning)
{
  sensitivity_vertices_.clear();

  G4int i_max = floor((sens_x_max_ - sens_x_min_) / binning);
  G4int j_max = floor((sens_y_max_ - sens_y_min_) / binning);
  G4int k_max = floor((sens_z_max_ - sens_z_min_) / binning);

  for (G4int i = 0; i < i_max; i++)
  {
    G4double x = sens_x_min_ + i * binning;
    for (G4int j = 0; j < j_max; j++)
    {
      G4double y = sens_y_min_ + j * binning;
      for (G4int k = 0; k < k_max; k++)
      {
        G4double z = sens_z_min_ + k * binning;
        if ((x * x + y * y) < inner_radius_ * inner_radius_)
        {
          G4ThreeVector point(x, y, z);
          sensitivity_vertices_.push_back(point);
        }
      }
    }
  }
  G4cout << "Number of points in sensitivity map = "
         << sensitivity_vertices_.size() << G4endl;
}
