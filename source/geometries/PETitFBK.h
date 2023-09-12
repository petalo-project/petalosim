// ----------------------------------------------------------------------------
// petalosim | PETitFBK.h
//
// This class implements the geometry of a box of LXe with FBK tiles.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PETIT_FBK_H
#define PETIT_FBK_H

#include "nexus/GeometryBase.h"

class G4GenericMessenger;
class G4LogicalVolume;
class G4VPhysicalVolume;

class PETitBox;

using namespace nexus;

class PETitFBK : public GeometryBase
{

public:
  // Constructor
  PETitFBK();
  //Destructor
  ~PETitFBK();

  /// Generate a vertex within a given region of the geometry
  G4ThreeVector GenerateVertex(const G4String &region) const;

private:
  void Construct();
  void BuildBox();
  void BuildSensors();

  G4LogicalVolume* lab_logic_;
  G4LogicalVolume* active_logic_;

  G4bool visibility_;
  G4double reflectivity_;
  G4bool box_vis_, tile_vis_;
  G4double tile_refl_, sipm_pde_;

  G4ThreeVector specific_vertex_;

  G4double n_tile_rows_, n_tile_columns_;
  G4double tile_thickn_;
  G4double dist_dice_flange_;

  G4double max_step_size_, pressure_;

  /// Messenger for the definition of control commands
  G4GenericMessenger* msg_;

  PETitBox* box_;

  G4double end_of_teflon_z_;
};

#endif
