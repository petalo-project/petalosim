// ----------------------------------------------------------------------------
// petalosim | set-up_filter_transmitance.cc
//
// This class is used to obtain the neutral VUV filter transmitance. It is a
// simple set-up with only the filter and a panel to detect photons.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "NeutralFilterVUV.h"
#include "filter_transmitance.h"
#include "PetMaterialsList.h"
#include "nexus/Visibilities.h"
#include "nexus/SensorSD.h"
#include "nexus/FactoryBase.h"
#include "nexus/SpherePointSampler.h"
#include "nexus/MaterialsList.h"
#include "nexus/OpticalMaterialProperties.h"

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

REGISTER_CLASS(filter_transmitance, GeometryBase)

filter_transmitance::filter_transmitance() : GeometryBase(),
                                             visibility_(0)


{
  // Messenger
  msg_ = new G4GenericMessenger(this, "/Geometry/PETitFilter/",
                                "Control commands of geometry PETitFilter.");
  msg_->DeclareProperty("visibility", visibility_, "Visibility");

}

filter_transmitance::~filter_transmitance()
{
}

void filter_transmitance::Construct()
{
  // Volume of air surrounding the detector //
  G4double lab_size = 1. * m;
  G4Box *lab_solid = new G4Box("LAB", lab_size/2., lab_size/2., lab_size/2.);

  G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  air->SetMaterialPropertiesTable(opticalprops::Vacuum());
  G4LogicalVolume* lab_logic = new G4LogicalVolume(lab_solid, air, "LAB");
  lab_logic->SetVisAttributes(G4VisAttributes::GetInvisible());

  this->SetLogicalVolume(lab_logic);


  // Add neutral VUV filter
  NeutralFilterVUV filter = NeutralFilterVUV();
  filter.Construct();
  G4LogicalVolume* filter_logic = filter.GetLogicalVolume();

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), filter_logic, "FILTER_VUV",
                    lab_logic, false, 0, false);


  //Build a detector plane

  G4double plane_xy = 999 * mm;
  G4double plane_z  = 1 * mm;

  G4Box* detector_plane = new G4Box("DETECT_PLANE", plane_xy/2., plane_xy/2., plane_z/2.);
  G4Material* steel = materials::Steel();

  G4LogicalVolume* plane_logic =
    new G4LogicalVolume(detector_plane, steel, "DETECTOR_PLANE");


  // Apply optical surface with efficiency 1
  G4OpticalSurface *detector_opsurf =
      new G4OpticalSurface("DETECTOR_OPSURF", unified, polished, dielectric_metal);

      const G4int entries = 2;

  G4double energies[entries] = {1.37755 * eV, 8.      * eV};

  G4double efficiency[entries] = {1., 1.};
  G4double reflectivity[entries] = {0., 0.};

  G4MaterialPropertiesTable* detector_mpt = new G4MaterialPropertiesTable();
  detector_mpt->AddProperty("EFFICIENCY", energies, efficiency, entries);
  detector_mpt->AddProperty("REFLECTIVITY", energies, reflectivity, entries);
  detector_opsurf->SetMaterialPropertiesTable(detector_mpt);

  new G4LogicalSkinSurface("SIPM_OPSURF", plane_logic, detector_opsurf);


  // Set the ACTIVE volume as an ionization sensitive det

  SensorSD* sensorsd = new SensorSD("DETECTOR_PLANE_SENSOR");
  G4SDManager::GetSDMpointer()->AddNewDetector(sensorsd);
  plane_logic->SetSensitiveDetector(sensorsd);


  G4double dist_filt_plane = 5 * cm;

  new G4PVPlacement(0, G4ThreeVector(0., 0., dist_filt_plane), plane_logic,
                    "DETECTOR_PLANE", lab_logic, false, 0, false);

  if (visibility_) {
    G4VisAttributes filter_col = nexus::LightBlue();
    filter_logic->SetVisAttributes(filter_col);
    G4VisAttributes plane_col = nexus::LightGrey();
    plane_logic->SetVisAttributes(plane_col);
  }

}


G4ThreeVector filter_transmitance::GenerateVertex(const G4String &region) const
{
  G4ThreeVector vertex(0., 0., -3.);

  if (region == "CENTER") {
    return vertex;
    }
  else {
    return vertex;
  }
}
