// ----------------------------------------------------------------------------
// petalosim | Tile.h
//
// Geometry of a basic tile.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TILE_H
#define TILE_H

#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>

class G4GenericMessenger;
class SiPMpetFBK;

using namespace nexus;

  class Tile: public GeometryBase
  {
  public:
    /// Constructor
    Tile();
    /// Destructor
    ~Tile();

    /// Invoke this method to build the volumes of the geometry
    void Construct();

    G4ThreeVector GetDimensions();

  private:

    // Visibility of the tracking plane
    G4bool visibility_;

    // Tile dimensions
    G4double tile_x_, tile_y_, tile_z_;

    // SiPM pitch
    G4double sipm_pitch_;

    // Rows and columns of SiPMs
    G4int n_rows_, n_columns_;

     // Messenger for the definition of control commands
    G4GenericMessenger* msg_;

    SiPMpetFBK* sipm_;

  };

#endif
