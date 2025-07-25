// ----------------------------------------------------------------------------
// petalosim | PETitFilter.h
//
// This class implements the geometry of a box of LXe with Hamamatsu tiles with
// the VUV filter.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PETITFILTER_H
#define PETITFILTER_H

#include "nexus/GeometryBase.h"

class G4GenericMessenger;
class G4LogicalVolume;
class G4VPhysicalVolume;

class PETitBox;

using namespace nexus;

class PETitFilter : public GeometryBase
{

public:
  // Constructor
  PETitFilter();
  //Destructor
  ~PETitFilter();

  /// Generate a vertex within a given region of the geometry
  G4ThreeVector GenerateVertex(const G4String &region) const;

private:
  void Construct();
  void BuildBox();
  void BuildSensors();

  G4LogicalVolume* lab_logic_;
  G4LogicalVolume* active_logic_;

  G4bool visibility_;
  G4bool box_vis_, tile_vis_;
  G4double tile_refl_;
  G4double dist_dice_flange_;
  G4double n_tile_rows_, n_tile_columns_;

  G4ThreeVector specific_vertex_;

  G4bool sipm_cells_;
  G4double max_step_size_, pressure_;

  /// Messenger for the definition of control commands
  G4GenericMessenger* msg_;

  PETitBox* box_;
};

#endif
