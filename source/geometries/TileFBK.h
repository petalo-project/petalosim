// ----------------------------------------------------------------------------
// petalosim | TileFBK.h
//
// Geometry of a FBK sensor tile.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TILE_FBK_H
#define TILE_FBK_H

#include "TileGeometryBase.h"

#include <G4ThreeVector.hh>

class G4GenericMessenger;

using namespace nexus;

  class TileFBK: public TileGeometryBase
  {
  public:
    /// Constructor
    TileFBK();
    /// Destructor
    ~TileFBK();

    /// Return dimensions of the SiPM
    //G4ThreeVector GetDimensions() const;

    /// Invoke this method to build the volumes of the geometry
    void Construct();

    void SetNamingOrder(G4int nord);

  private:

    // Tile dimensions
    G4double tile_x_, tile_y_, tile_z_;

    // SiPM pitch
    G4double sipm_pitch_;

    // Rows and columns of SiPMs
    G4int n_rows_, n_columns_;

    G4int sipm_naming_order_;

  };

inline void TileFBK::SetNamingOrder(G4int nord) {sipm_naming_order_ = nord;}

#endif
