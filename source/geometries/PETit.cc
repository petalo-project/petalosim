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

  PETitBox box = PETitBox();
  G4Material* LXe = G4NistManager::Instance()->FindOrBuildMaterial("G4_lXe");
  LXe->SetMaterialPropertiesTable(petopticalprops::LXe(pressure_));

  // Set the ACTIVE volume as an ionization sensitive det
  IonizationSD* ionisd = new IonizationSD("/PETALO/ACTIVE");
  G4SDManager::GetSDMpointer()->AddNewDetector(ionisd);

  box.SetLXePressure(pressure_);
  box.SetXeMaterial(LXe);
  box.SetIoniSD(ionisd);
  box.Construct();
  G4LogicalVolume* box_logic = box.GetLogicalVolume();

  new G4PVPlacement(0, G4ThreeVector(0., 0, 0.),
                    box_logic, "ALUMINUM_BOX", lab_logic_,
                    false, 0, false);
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
