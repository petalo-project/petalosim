// ----------------------------------------------------------------------------
// petalosim | PetLYSOCell.h
//
// Basic cell made of LYSO.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PET_LYSO_CELL_H
#define PET_LYSO_CELL_H

#include "nexus/GeometryBase.h"

class G4LogicalVolume;
class G4GenericMessenger;
class G4Material;
//namespace nexus{class PetKDBFixedPitch;}
class PetPlainDice;
namespace nexus
{
  class BoxPointSamplerLegacy;
}
/* namespace nexus {class SiPMpetVUV;} */
/* namespace nexus {class SiPMpetTPB;} */

class PetLYSOCell : public nexus::GeometryBase
{
public:
  /// Constructor
  PetLYSOCell();

  /// Destructor
  ~PetLYSOCell();

  /// Generate a vertex within a given region of the geometry
  G4ThreeVector GenerateVertex(const G4String &region) const;

  void Construct();

private:
  void BuildDetector();
  void BuildLYSO();
  void BuildActive();
  void BuildSiPMPlane();

  G4LogicalVolume *lab_logic_;
  G4LogicalVolume *det_logic_;
  G4LogicalVolume *lyso_logic_;

  // Detector dimensions
  //   const G4double vacuum_thickn_; // thickness of vaccuum layer between walls
  //  const G4double outer_wall_thickn_; // Thickness of the outer wall
  const G4double det_thickness_; // Thickness of the walls of the detector
  //   const G4double det_size_; /// Size of the detector
  const G4double active_size_; /// Size of the LYSO active volume

  // ACTIVE gas Xenon
  G4Material *lyso_;

  // Parameters
  G4double max_step_size_; /// Maximum Step Size

  //Vertex generator
  nexus::BoxPointSamplerLegacy *active_gen_;

  /// Messenger for the definition of control commands
  G4GenericMessenger *msg_;

  /// Vertex generator from a surface just outside the detector walls
  nexus::BoxPointSamplerLegacy *surf_gen_;

  // Dice class
  // PetKDBFixedPitch* db_;
  PetPlainDice *pdb_;

  //SiPMpetVUV* sipm_;
  nexus::GeometryBase *sipm_;

  G4double pdb_z_;

  G4double z_size_;
  G4bool phys_; ///< are we using physical optical properties for LYSO?
};

#endif
