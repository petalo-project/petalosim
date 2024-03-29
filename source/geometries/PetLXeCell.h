// ----------------------------------------------------------------------------
// petalosim | PetLXeCell.ch
//
// Basic cell filled with LXe.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PET_LXE_CELL_H
#define PET_LXE_CELL_H

#include "nexus/GeometryBase.h"

class G4LogicalVolume;
class G4GenericMessenger;
class G4Material;

namespace nexus
{
  class BoxPointSamplerLegacy;
}

class PetPlainDice;
class SiPMHamamatsuBlue;
class SiPMpetTPB;
class SiPMpetVUV;
class SiPMpetPlots;

using namespace nexus;

class PetLXeCell : public GeometryBase
{
public:
  /// Constructor
  PetLXeCell();

  /// Destructor
  ~PetLXeCell();

  /// Generate a vertex within a given region of the geometry
  G4ThreeVector GenerateVertex(const G4String &region) const;

  void Construct();

private:
  //    void BuildDetector();
  void BuildLXe();
  void BuildActive();
  void BuildSiPMPlane();
  //  void BuildSiPMChessPlane();

  //    G4LogicalVolume* det_logic_;
  G4LogicalVolume *lXe_logic_;
  G4LogicalVolume *active_logic_;

  // Detector dimensions
  //   const G4double vacuum_thickn_; // thickness of vaccuum layer between walls
  //  const G4double outer_wall_thickn_; // Thickness of the outer wall
  const G4double det_thickness_; // Thickness of the walls of the detector
  //   const G4double det_size_; /// Size of the detector

  // ACTIVE gas Xenon
  G4Material *lXe_;

  // Parameters
  G4double max_step_size_; /// Maximum Step Size

  //Vertex generator
  BoxPointSamplerLegacy *active_gen_;

  /// Messenger for the definition of control commands
  G4GenericMessenger *msg_;

  /// Vertex generator from a surface just outside the detector walls
  BoxPointSamplerLegacy *surf_gen_;

  // Dice class
  // PetKDBFixedPitch* db_;
  PetPlainDice *pdb_;

  SiPMpetVUV *sipm_;
  /* SiPMpetVUV *sipmVUV_; */
  /* SiPMpetTPB *sipmTPB_; */
  /* SiPMpetPlots *sipmPlots_; */
  /* SiPMHamamatsuBlue* sipmBlue_; */

  G4double pdb_z_;

  G4double z_size_;  ///< Z size of the LXe active volume
  G4double xy_size_; /// XY size of the LXe active volume

  G4bool phys_; ///< are we using physical optical properties for LXe?

  G4int rows_, columns_;
  G4double sipm_pitch_; ///< Distance between the centres of the SiPMs

  G4double z_LXe_;      ///< z dimension of LXe which exceeds active
  G4double lXe_size_z_; ///< z dimension of LXe
};

#endif
