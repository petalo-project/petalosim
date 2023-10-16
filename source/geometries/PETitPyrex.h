// ----------------------------------------------------------------------------
// petalosim | PETitPyrex.h
//
// This class implements the geometry of a continuous box of LXe
// with pyrex panels to avoid reflections.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PETIT_PYREX_H
#define PETIT_PYREX_H

#include "TileGeometryBase.h"

#include "nexus/GeometryBase.h"

class G4GenericMessenger;
class G4LogicalVolume;
class G4VPhysicalVolume;

class PETitBox;
class TileHamamatsuVUV;
class TileHamamatsuBlue; 

using namespace nexus;

class PETitPyrex : public GeometryBase
{

public:
  // Constructor
  PETitPyrex();
  //Destructor
  ~PETitPyrex();

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
  G4double tile_refl_, panel_refl_;
  G4bool blue_tiles_;

  G4double n_tile_rows_, n_tile_columns_;
  G4double tile_thickn_;
  G4double dist_dice_flange_;

  G4double dist_lat_panels_;

  G4double dist_ihat_entry_panel_, panel_thickness_;
  G4double entry_panel_x_size_, entry_panel_y_size_;
  G4double dist_entry_panel_ground_;

  G4double dist_entry_panel_h_panel_, dist_entry_panel_v_panel_;

  G4double lat_panel_len_;
  G4double h_l_panel_z_size_, h_l_panel_y_pos_;
  G4double v_l_panel_z_size_;
  G4double dist_ham_vuv_, dist_ham_blue_;
  G4double panel_sipm_xy_size_, dist_sipms_panel_sipms_;
  G4double wls_depth_;

  G4ThreeVector specific_vertex_;
  G4double pressure_;

  /// Messenger for the definition of control commands
  G4GenericMessenger* msg_;

  PETitBox* box_;
};

#endif
