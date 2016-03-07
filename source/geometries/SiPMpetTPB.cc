// ----------------------------------------------------------------------------
//  $Id$
//
//  Author:  
//  Created: 2 March 2010
//  
//  Copyright (c) 2010-2013 NEXT Collaboration. All rights reserved.
// ---------------------------------------------------------------------------- 

#include "SiPMpetTPB.h"
#include "PmtSD.h"
#include "MaterialsList.h"
#include "OpticalMaterialProperties.h"
#include "Visibilities.h"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4PhysicalConstants.hh>
#include <G4GenericMessenger.hh>

#include <CLHEP/Units/SystemOfUnits.h>


namespace nexus {

  using namespace CLHEP;
  
  SiPMpetTPB::SiPMpetTPB(): BaseGeometry(),
			    _visibility(0), _refr_index(1.), _phys(1)

  {
    /// Messenger
    _msg = new G4GenericMessenger(this, "/Geometry/SiPMpet/", "Control commands of geometry.");
    _msg->DeclareProperty("SiPMpet_vis", _visibility, "SiPMpet Visibility");
    _msg->DeclareProperty("refr_index", _refr_index, "Refraction index for epoxy");
    //Are we using physical opt properties?
    _msg->DeclareProperty("physics", _phys, "physical optical properties");
  }
  
  
  
  SiPMpetTPB::~SiPMpetTPB()
  {
  }



  // G4ThreeVector SiPMpetTPB::GetDimensions() const
  // {
  //   return _dimensions;
  // }
  
  
  
  void SiPMpetTPB::Construct()
  {
    G4double size = 3. * mm;
    // PACKAGE ///////////////////////////////////////////////////////

    G4double sipm_x = size;
    G4double sipm_y = size;
    G4double sipm_z = 1.55 * mm;

    SetDimensions(G4ThreeVector(sipm_x, sipm_y, sipm_z));

    // _dimensions.setX(sipm_x);
    // _dimensions.setY(sipm_y);
    // _dimensions.setZ(sipm_z);
   

    G4Box* sipm_solid = new G4Box("SIPMpet", sipm_x/2., sipm_y/2., sipm_z/2);

    G4Material* epoxy = MaterialsList::Epoxy();
    G4cout << "Epoxy used with constant refraction index = " <<  _refr_index << G4endl;
    epoxy->SetMaterialPropertiesTable(OpticalMaterialProperties::EpoxyFixedRefr(_refr_index));
      
    
    G4LogicalVolume* sipm_logic = 
      new G4LogicalVolume(sipm_solid, epoxy, "SIPMpet");

    this->SetLogicalVolume(sipm_logic);

    // TPB coating
    G4double tpb_z = 0.001 * mm;
    G4Box* tpb_solid = new G4Box("TPB", sipm_x/2., sipm_y/2., tpb_z/2);
    G4Material* TPB = MaterialsList::TPB();
    if (_phys) {
      G4cout << "TPB with refraction index equal to LXe" << G4endl;
      TPB->SetMaterialPropertiesTable(OpticalMaterialProperties::TPB_LXe());
    } else {
      G4cout << "TPB with constant refraction index equal to 1.7 "<< G4endl;
      TPB->SetMaterialPropertiesTable(OpticalMaterialProperties::TPB_LXe_nconst());
    }

    G4LogicalVolume* tpb_logic =
      new G4LogicalVolume(tpb_solid, TPB, "TPB");

    G4double pos_z = (sipm_z - tpb_z) / 2.;

    new G4PVPlacement(0, G4ThreeVector(0.,0.,pos_z), tpb_logic,
                      "TPB", sipm_logic, false, 0, true);
    //   G4cout << "TPB starts in " << pos_z - tpb_z/2 << " in the ref system of SiPM" << G4endl;

    // PCB ///////////////////////////////////////////////////////

    // G4double pcb_z = 0.550 * mm;
    
    // G4Material* plastic = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYCARBONATE");

    // G4Box* plastic_solid = new G4Box("PLASTIC", sipm_x/2., sipm_y/2., pcb_z/2);
    
    // G4LogicalVolume* plastic_logic = 
    // new G4LogicalVolume(plastic_solid, plastic, "PLASTIC");
    
    // G4double epoxy_z = 0.300 * mm;

    // new G4PVPlacement(0, G4ThreeVector(0, 0., epoxy_z/2), plastic_logic,
    // 		      "PLASTIC", sipm_logic, false, 0, false);

    // ACTIVE WINDOW /////////////////////////////////////////////////

    G4double active_side     = size;
    G4double active_depth    = 0.01   * mm;
    
    G4Box* active_solid =
      new G4Box("PHOTODIODES", active_side/2., active_side/2., active_depth/2);
    
    G4Material* silicon = 
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");

    G4LogicalVolume* active_logic =
      new G4LogicalVolume(active_solid, silicon, "PHOTODIODES");

    new G4PVPlacement(0, G4ThreeVector(0., 0., sipm_z/2. - active_depth/2. - .1*mm), active_logic,
		      "PHOTODIODES", sipm_logic, false, 0, false);
    
    
    // OPTICAL SURFACES //////////////////////////////////////////////

    // SiPM efficiency set using the official Hamamatsu specs.
    
    const G4int entries = 21;


     G4double energies[entries] = {1.54980241262*eV,1.59979603883*eV,
                                 1.65312257346*eV,1.71012680013*eV,1.77120275727*eV,
                                 1.8368028594*eV,1.90744912322*eV,1.98374708815*eV,
                                 2.06640321682*eV,2.15624683494*eV,2.25425805471*eV,
                                 2.36160367637*eV,2.47968386018*eV,2.61019353704*eV,
                                 2.75520428909*eV,2.91727512963*eV,3.09960482523*eV,
                                 3.30624514691*eV,3.54240551455*eV,3.81489824644*eV,
                                 3.96749*eV};
    G4double reflectivity[entries] = {0.      ,0.      ,0.      ,0.      ,0.      ,
                                      0.      ,0.      ,0.      ,0.      ,0.      ,
                                      0.      ,0.      ,0.      ,0.      ,0.      ,
                                      0.      ,0.      ,0.      ,0.      ,0.,      
                                       0. };
    G4double efficiency[entries]   = {1., 1., 
                                      1., 1., 1., 
                                      1., 1., 1.,  
                                      1., 1., 1., 
                                      1., 1., 1., 
                                      1.,  1., 1.,
                                      1.,  1., 1.,
                                      1.};

    // G4double efficiency[entries]   = {0.036, 0.048, 
    //                                   0.06, 0.07, 0.09, 
    //                                   0.105, 0.12, 0.145,  
    //                                   0.17, 0.2, 0.235, 
    //                                   0.275, 0.32, 0.37, 
    //                                   0.42,  0.425, 0.415,
    //                                   0.35,  0.315, 0.185,
    //                                   0.06};

    // G4double efficiency_red[entries];
    // for (G4int i=0; i<entries; ++i) {
    //   efficiency_red[i] = efficiency[i]*.20;
    // }

    
    G4MaterialPropertiesTable* sipm_mt = new G4MaterialPropertiesTable();
    sipm_mt->AddProperty("EFFICIENCY", energies, efficiency, entries);
    sipm_mt->AddProperty("REFLECTIVITY", energies, reflectivity, entries);

    G4OpticalSurface* sipm_opsurf = 
      new G4OpticalSurface("SIPM_OPSURF", unified, polished, dielectric_metal);
    sipm_opsurf->SetMaterialPropertiesTable(sipm_mt);

    new G4LogicalSkinSurface("SIPM_OPSURF", active_logic, sipm_opsurf);    
    
    
    // SENSITIVE DETECTOR ////////////////////////////////////////////

    G4String sdname = "/SIPM/SiPMpetTPB";
    G4SDManager* sdmgr = G4SDManager::GetSDMpointer();
    
    if (!sdmgr->FindSensitiveDetector(sdname, false)) {
      PmtSD* sipmsd = new PmtSD(sdname);
      sipmsd->SetDetectorVolumeDepth(0);
      sipmsd->SetDetectorNamingOrder(1000.);
      sipmsd->SetTimeBinning(25.*picosecond);
      //    sipmsd->SetMotherVolumeDepth(1);
      //      sipmsd->SetGrandMotherVolumeDepth(3);
      sipmsd->SetMotherVolumeDepth(2);
      
      G4SDManager::GetSDMpointer()->AddNewDetector(sipmsd);
      sipm_logic->SetSensitiveDetector(sipmsd);
    }

    // Visibilities
    if (_visibility) {
      G4VisAttributes red_col = nexus::Red();
      sipm_logic->SetVisAttributes(red_col);

      G4VisAttributes active_col(G4Colour(.6, .8, .79));
      active_col.SetForceSolid(true);
      active_logic->SetVisAttributes(active_col);
    }
    else {
      sipm_logic->SetVisAttributes(G4VisAttributes::Invisible);
      active_logic->SetVisAttributes(G4VisAttributes::Invisible);
    }
  }
  
  
} // end namespace nexus
