// ----------------------------------------------------------------------------
// petalosim | PETit.h
//
// This class implements the geometry of a box of LXe.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PETIT_H
#define PETIT_H

#include "nexus/GeometryBase.h"
#include "TileGeometryBase.h"

class G4GenericMessenger;
class G4LogicalVolume;
class G4VPhysicalVolume;

class PETitBox;
class TileHamamatsuVUV;

using namespace nexus;

class PETit : public GeometryBase
{

public:
  // Constructor
  PETit();
  //Destructor
  ~PETit();

  /// Generate a vertex within a given region of the geometry
  G4ThreeVector GenerateVertex(const G4String &region) const;

  TileGeometryBase* tile_;

private:
  void Construct();
  void BuildBox();
  void BuildSensors();

  G4LogicalVolume* lab_logic_;
  G4LogicalVolume* active_logic_;

  G4bool visibility_;
  G4double reflectivity_;
  G4bool box_vis_, tile_vis_;
  G4double tile_refl_;

  G4ThreeVector source_pos_;

  G4double n_tile_rows_, n_tile_columns_;
  G4double tile_thickn_;
  G4double dist_dice_flange_;

  G4double max_step_size_, pressure_;

  /// Messenger for the definition of control commands
  G4GenericMessenger* msg_;

  PETitBox* box_;
};

#endif
