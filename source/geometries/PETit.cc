// ----------------------------------------------------------------------------
// petalosim | PETit.cc
//
// This class implements the geometry of a box of LXe.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PETit.h"
#include "PETitBox.h"
#include "TileHamamatsuVUV.h"
#include "TileHamamatsuBlue.h"
#include "TileFBK.h"
#include "PetMaterialsList.h"
#include "PetOpticalMaterialProperties.h"
#include "Na22Source.h"
#include "TeflonBlockHamamatsu.h"

#include "nexus/Visibilities.h"
#include "nexus/IonizationSD.h"
#include "nexus/FactoryBase.h"
#include "nexus/MaterialsList.h"
#include "nexus/OpticalMaterialProperties.h"
#include "nexus/SpherePointSampler.h"

#include <G4GenericMessenger.hh>
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
#include <G4SubtractionSolid.hh>

using namespace nexus;

REGISTER_CLASS(PETit, GeometryBase)

PETit::PETit() : GeometryBase(),
                   visibility_(0),
                   max_step_size_(1. * mm),
                   pressure_(1 * bar)

{
  // Messenger
  msg_ = new G4GenericMessenger(this, "/Geometry/PETit/",
                                "Control commands of geometry PETit.");
  msg_->DeclareProperty("visibility", visibility_, "Visibility");

  G4GenericMessenger::Command &press_cmd =
    msg_->DeclareProperty("pressure", pressure_,
                          "Pressure of LXe");
  press_cmd.SetUnitCategory("Pressure");
  press_cmd.SetParameterName("pressure", false);
  press_cmd.SetRange("pressure>0.");

  box_ = new PETitBox();

}

PETit::~PETit()
{
}

void PETit::Construct()
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

void PETit::BuildBox()
{
  G4Material* LXe = G4NistManager::Instance()->FindOrBuildMaterial("G4_lXe");
  LXe->SetMaterialPropertiesTable(petopticalprops::LXe(pressure_));

  // Set the ACTIVE volume as an ionization sensitive det
  IonizationSD* ionisd = new IonizationSD("/PETALO/ACTIVE");
  G4SDManager::GetSDMpointer()->AddNewDetector(ionisd);


  box_->SetLXePressure(pressure_);
  box_->SetXeMaterial(LXe);
  box_->SetIoniSD(ionisd);
  box_->Construct();
  G4LogicalVolume* box_logic = box_->GetLogicalVolume();

  new G4PVPlacement(0, G4ThreeVector(0., 0, 0.),
                    box_logic, "ALUMINUM_BOX", lab_logic_,
                    false, 0, false);

  G4LogicalVolume* active_logic = box_->GetActiveVolume();
  G4double ih_z_size = box_->GetHatZSize();

  TeflonBlockHamamatsu teflon_block_hama = TeflonBlockHamamatsu();
  teflon_block_hama.SetHoleMaterial(LXe);
  teflon_block_hama.SetIoniSD(ionisd);
  teflon_block_hama.SetMaxStepSize(max_step_size_);
  teflon_block_hama.Construct();
  G4LogicalVolume* teflon_block_logic = teflon_block_hama.GetLogicalVolume();

  G4double teflon_block_thick = teflon_block_hama.GetTeflonThickness();
  G4double block_z_pos = ih_z_size/2. + teflon_block_thick/2.;
  new G4PVPlacement(0, G4ThreeVector(0., 0., -block_z_pos), teflon_block_logic,
                    "TEFLON_BLOCK_HAMA", active_logic, false, 0, false);

  G4RotationMatrix rot_teflon;
  rot_teflon.rotateY(pi);
  new G4PVPlacement(G4Transform3D(rot_teflon, G4ThreeVector(0., 0., block_z_pos)),
                    teflon_block_logic,
                    "TEFLON_BLOCK_HAMA", active_logic, false, 1, false);

  // Optical surface for teflon
  G4OpticalSurface* teflon_optSurf =
    new G4OpticalSurface("TEFLON_OPSURF", unified, ground, dielectric_metal);
  teflon_optSurf->SetMaterialPropertiesTable(petopticalprops::PTFE());
  new G4LogicalSkinSurface("TEFLON_OPSURF", teflon_block_logic, teflon_optSurf);
}

void PETit::BuildSensors()
{
}

G4ThreeVector PETit::GenerateVertex(const G4String &region) const
{
  G4ThreeVector vertex(0., 0., 0.);

  if (region == "CENTER")
    {
      return vertex;
    }
  else if (region == "AD_HOC")
    {
      vertex = source_pos_;
    }
  else if (region == "SOURCE")
    {
      //vertex = source_gen_->GenerateVertex("VOLUME");
    }
  else
    {
      G4Exception("[PETit]", "GenerateVertex()", FatalException,
                  "Unknown vertex generation region!");
    }
  return vertex;
}
